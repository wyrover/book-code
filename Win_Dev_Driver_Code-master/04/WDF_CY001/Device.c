/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：Device.c
	说明：打印Debug信息
	历史：
	创建：
*/
#include "public.h"

// 本页中所有函数都是可换页的，运行于被动级别(PASSIVE_LEVEL)
#pragma code_seg("PAGE")

// 多接口设备配置初始化Alter值
UCHAR MultiInterfaceSettings[MAX_INTERFACES] = {1, 1, 1, 1, 1, 1, 1, 1};

// USB总线接口GUID
// {B1A96A13-3DE0-4574-9B01-C08FEAB318D6}
static GUID USB_BUS_INTERFACE_USBDI_GUID1 =
{ 0xb1a96a13, 0x3de0, 0x4574, {0x9b, 0x1, 0xc0, 0x8f, 0xea, 0xb3, 0x18, 0xd6}};

// 用“GUID Generator”工具生成一个唯一的GUID。
static	GUID DeviceInterface = 
{0xdb713b3f, 0xea3f, 0x4d74, {0x89, 0x46, 0x0, 0x64, 0xdb, 0xa4, 0xfc, 0x6a}};

//
// 作用相当于WDM中的AddDevice函数。
// 他是PNP过程中首当其冲被调用的回调函数。主要任务是创建设备对象。
// 设备对象在系统中以软件形式，代表外部的某个物理硬件设备。
NTSTATUS PnpAdd(IN WDFDRIVER  Driver, IN PWDFDEVICE_INIT  DeviceInit)
{
	NTSTATUS status;
	WDFDEVICE device;
	int nInstance = 0;
	PDEVICE_CONTEXT devCtx = NULL;
	UNICODE_STRING DeviceName;
	UNICODE_STRING DosDeviceName;
	UNICODE_STRING RefString;
	WDFSTRING	   SymbolName;
	WDF_OBJECT_ATTRIBUTES attributes;
	WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks;
	WDF_DEVICE_PNP_CAPABILITIES pnpCapabilities;

	WCHAR wcsDeviceName[] = L"\\Device\\CY001-0";		// 设备名
	WCHAR wcsDosDeviceName[] = L"\\DosDevices\\CY001-0";// 符号链接名
	WCHAR wcsRefString[] = L"CY001-0";					// 符号链接名
	int	  nLen = wcslen(wcsDeviceName);

	UNREFERENCED_PARAMETER(Driver);

	KDBG(DPFLTR_INFO_LEVEL, "[PnpAdd]");

	RtlInitUnicodeString(&DeviceName, wcsDeviceName) ;
	RtlInitUnicodeString(&DosDeviceName, wcsDosDeviceName);
	RtlInitUnicodeString(&RefString, wcsRefString);

	// 注册PNP与Power回调函数
	WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
	pnpPowerCallbacks.EvtDevicePrepareHardware	= PnpPrepareHardware; // 初始化
	pnpPowerCallbacks.EvtDeviceReleaseHardware  = PnpReleaseHardware; // 停止
	pnpPowerCallbacks.EvtDeviceSurpriseRemoval	= PnpSurpriseRemove;  // 异常移除
	pnpPowerCallbacks.EvtDeviceD0Entry	= PwrD0Entry; // 进入D0电源状态（工作状态），比如初次插入、或者唤醒
	pnpPowerCallbacks.EvtDeviceD0Exit	= PwrD0Exit;  // 离开D0电源状态（工作状态），比如休眠或设备移除
	WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

	// 读、写请求的缓冲方式
	// 默认为Buffered方式，另外两种方式是Direct和Neither。
	WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoBuffered);

	// 设定设备环境块长度
	// 宏内部会调用sizeof(DEVICE_CONTEXT)求结构体长度
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);
	
	// 驱动支持最多8个实例，即当多个开发板连到PC上时，驱动对它们给予并行支持。
	// 不同的设备对象，各以其名称的尾数（0-7）相别，并将此尾数称作设备ID。
	// 下面的代码逻辑为当前设备对象寻找一个可用ID。
	for(nInstance = 0; nInstance < MAX_INSTANCE_NUMBER; nInstance++)
	{
		// 修改设备ID
		wcsDeviceName[nLen-1] += nInstance;

		// 尝试命名；命名可能失败，失败的原因包括：名称无效、名称已存在等
		WdfDeviceInitAssignName(DeviceInit, &DeviceName);

		// 创建WDF设备
		// 如能成功，则命名亦成功。
		status = WdfDeviceCreate(&DeviceInit, &attributes, &device);  
		if(!NT_SUCCESS(status))
		{
			if(status == STATUS_OBJECT_NAME_COLLISION)// 名字冲突
			{
				KDBG(DPFLTR_ERROR_LEVEL, "Already exist: %wZ", &DeviceName);
			}
			else
			{
				KDBG(DPFLTR_ERROR_LEVEL, "WdfDeviceCreate failed with status 0x%08x!!!", status);
				return status;
			}
		}else
		{
			KdPrint(("Device name: %wZ", &DeviceName));
			break;
		}
	}

	// 如失败，可能是连接的开发板数量太多；
	// 建议使用WinOBJ查看系统中的设备名称。
	if (!NT_SUCCESS(status)) 
		return status;

	// 创建符号链接，应用程序根据符号链接查看并使用内核设备。
	// 除了创建符号链接外，现在更好的方法是使用WdfDeviceCreateDeviceInterface创建设备接口。
	// 设备接口能保证名字不会冲突，但不具有可读性，所以我们仍采用符号链接形式。
	nLen = wcslen(wcsDosDeviceName);
	wcsDosDeviceName[nLen-1] += nInstance;
	
	status = WdfDeviceCreateSymbolicLink(device, &DosDeviceName);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "Failed to create symbol link: 0x%08X", status);
		return status;
	}

#if 0
	// WdfDeviceCreateDeviceInterface需设置一个引用字符串，内容随便；
	// 用来将同一个接口类中的多个设备接口区别开来。
	nLen = wcslen(wcsRefString);
	wcsRefString[nLen-1] += nInstance;

	status = WdfDeviceCreateDeviceInterface(device, 
			&DeviceInterface,	// 接口GUID
			&RefString);		// 应用字符串

	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "Failed to create the device interface: 0x%08X", status);
		return status;
	}

	status = WdfStringCreate(NULL, WDF_NO_OBJECT_ATTRIBUTES, &string);
	if(NT_SUCCESS(status)
	{
		status = WdfDeviceRetrieveDeviceInterfaceString(device, &DeviceInterface, &RefString, string);
		if(status = STATUS_SUCCESS)
		{
			UNICODE_STRING name;
			WdfStringGetUnicodeString(string, &name);
			KDBG(DPFLTR_INFO_LEVEL, "Interface Name:%wZ", &name);
		}
	}
#endif

	// PNP属性。允许设备异常拔除，系统不会弹出错误框。
	WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpCapabilities);
	pnpCapabilities.Removable = WdfTrue;
	pnpCapabilities.SurpriseRemovalOK = WdfTrue;

	WdfDeviceSetPnpCapabilities(device, &pnpCapabilities);

	// 初始化环境块
	// GetDeviceContext是一个函数指针，由宏WDF_DECLARE_CONTEXT_TYPE_WITH_NAME定义。
	// 参看pulibc.h中的说明。
	devCtx = GetDeviceContext(device);

	RtlZeroMemory(devCtx, sizeof(DEVICE_CONTEXT));

	status = CreateWDFQueues(device, devCtx);
	if(!NT_SUCCESS(status))
		return status;


	// 下面被注释的代码，演示了一种让WDM程序员在WDF程序中嵌入WDM代码的方法。
	// 通过获取Function Device Object和设备栈中下一层的Device Object，
	// 我们可以在得到Request后，从Request中析取出IRP结构，绕过WDK框架而自行处理。
	// 从Request中析取IRP的函数是：WdfRequestWdmGetIrp
#if 0
	PDEVICE_OBJECT pFunctionDevice = WdfDeviceWdmGetDeviceObject(device);// 功能设备对象
	WDFDEVICE DeviceStack = WdfDeviceGetIoTarget(device);
	PDEVICE_OBJECT pDeviceNext = WdfIoTargetWdmGetTargetDeviceObject(DeviceStack);// 被Attached的下层驱动之设备对象。
#endif

	return status;
}

// 
// 创建队列。
// 为了掩饰，我们创建了多种类型的队列。
NTSTATUS CreateWDFQueues(WDFDEVICE Device, PDEVICE_CONTEXT pContext)
{
	NTSTATUS status = STATUS_SUCCESS;

	WDF_IO_QUEUE_CONFIG ioQConfig;
	KDBG(DPFLTR_INFO_LEVEL, "[CreateWDFQueues]");

	// 创建框架Queue对象。
	// 注意事项：1. 调用WdfIoQueueCreate时候第一个参数Device被默认为Queue对象的父对象，父对象负责并维护子对象的生命周期。
	//				程序不必维护Queue对象。它只需创建，无需销毁。
	//			 2. 可设置其他的父对象，在参数WDF_OBJECT_ATTRIBUTES中设置。
	//			 3. Queue对象类型有：并行、串行、手动三种。
	//			 4. 执行WdfDeviceConfigureRequestDispatching以令某一类型的请求都排队到此Queue。

	// 创建默认并发队列，处理DeviceIOControl命令。
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQConfig, WdfIoQueueDispatchParallel);
	ioQConfig.EvtIoDeviceControl = DeviceIoControlParallel;
	status = WdfIoQueueCreate(Device, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &pContext->IoControlEntryQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
		return status;
	}
	
	// 第二个队列：串列化队列，处理默认队列转发给它的需要串行处理的命令。
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchSequential);
	ioQConfig.EvtIoDeviceControl = DeviceIoControlSerial;
	status = WdfIoQueueCreate(Device, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &pContext->IoControlSerialQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
		return status;
	}

	// 第三个队列：串列化队列（读写操作）
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchSequential);
	ioQConfig.EvtIoWrite = BulkWrite;
	ioQConfig.EvtIoRead  = BulkRead;
	status = WdfIoQueueCreate(Device, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES,	&pContext->IoRWQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
		return status;
	}

	// 配置第三个队列，只接受读和写两种请求。
	status  = WdfDeviceConfigureRequestDispatching(Device, pContext->IoRWQueue, WdfRequestTypeWrite);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfDeviceConfigureRequestDispatching failed with status 0x%08x\n", status);
		return status;
	}
	status  = WdfDeviceConfigureRequestDispatching(Device, pContext->IoRWQueue, WdfRequestTypeRead);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfDeviceConfigureRequestDispatching failed with status 0x%08x\n", status);
		return status;
	}

	// 第四个队列：手动队列。这个队列中保存应用程序与驱动进行同步的请求。
	// 当驱动有消息通知应用程序时，即从手动队列中提取一个请求完成之。
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchManual);
	status = WdfIoQueueCreate(Device, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &pContext->AppSyncManualQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate for manual queue failed with status 0x%08x\n", status);
		return status;
	}

	// 第五个队列：手动队列。这个队列用于处理中断数据，当驱动收到中断数据时，就从队列中提取一个请求并完成之。
	// 鼠标、键盘等中断设备，需用这种形式。
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchManual);
	status = WdfIoQueueCreate(Device, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &pContext->InterruptManualQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate for manual queue failed with status 0x%08x\n", status);
		return status;
	}

	return status;
}

// 此函数类似于WDM中的PNP_MN_STOP_DEVICE函数，在设备移除时被调用。
// 当个函数被调用时候，设备仍处于工作状态。
NTSTATUS PnpReleaseHardware(IN WDFDEVICE Device, IN WDFCMRESLIST ResourceListTranslated)
{
	NTSTATUS                             status;
	WDF_USB_DEVICE_SELECT_CONFIG_PARAMS  configParams;
	PDEVICE_CONTEXT                      pDeviceContext;

	UNREFERENCED_PARAMETER(ResourceListTranslated);

	KDBG(DPFLTR_INFO_LEVEL, "[PnpReleaseHardware]");

	pDeviceContext = GetDeviceContext(Device);

	// 如果PnpPrepareHardware调用失败,UsbDevice为空；
	// 这时候直接返回即可。
	if (pDeviceContext->UsbDevice == NULL)
		return STATUS_SUCCESS;

	// 取消USB设备的所有IO操作。它将连带取消所有Pipe的IO操作。
	WdfIoTargetStop(WdfUsbTargetDeviceGetIoTarget(pDeviceContext->UsbDevice), WdfIoTargetCancelSentIo);

	// Deconfiguration或者“反配置”
	WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_DECONFIG(&configParams);
	status = WdfUsbTargetDeviceSelectConfig(
		pDeviceContext->UsbDevice,
		WDF_NO_OBJECT_ATTRIBUTES, 
		&configParams);

	return STATUS_SUCCESS;
}

// 此函数类似于WDM中的PNP_MN_START_DEVICE函数，紧接着PnpAdd之后被调用。
// 此时PNP管理器经过甄别之后，已经决定将那些系统资源分配给当前设备。
// 参数ResourceList和ResourceListTranslated代表了这些系统资源。
// 当个函数被调用时候，设备已经进入了D0电源状态；函数完成后，设备即正式进入工作状态。
NTSTATUS PnpPrepareHardware(IN WDFDEVICE Device, IN WDFCMRESLIST ResourceList, IN WDFCMRESLIST ResourceListTranslated)
{
	NTSTATUS status;
	PDEVICE_CONTEXT devCtx = NULL; 
	ULONG ulNumRes = 0;
	ULONG ulIndex;
	CM_PARTIAL_RESOURCE_DESCRIPTOR*  pResDes = NULL;

	KDBG(DPFLTR_INFO_LEVEL, "[PnpPrepareHardware]");

	devCtx = GetDeviceContext(Device);

	// 配置设备
	status = ConfigureUsbDevice(Device, devCtx);
	if(!NT_SUCCESS(status))
		return status;
	
	// 获取Pipe句柄
	status = GetUsbPipes(devCtx);
	if(!NT_SUCCESS(status))
		return status;

	// 初始电源策略，
	status = InitPowerManagement(Device);
	//if(!NT_SUCCESS(status))
	//	return status;

	// 获取USB总线驱动接口。总线接口中包含总线驱动提供的回调函数和其他信息。
	// 总线接口由系统共用GUID标识。
	status = WdfFdoQueryForInterface(
		Device,
		&USB_BUS_INTERFACE_USBDI_GUID1,		// 总线接口ID
		(PINTERFACE)&devCtx->busInterface,	// 保存在设备环境块中
		sizeof(USB_BUS_INTERFACE_USBDI_V1),
		1, NULL);

	// 调用接口函数，判断USB版本。
	if(NT_SUCCESS(status) && devCtx->busInterface.IsDeviceHighSpeed){
		devCtx->bIsDeviceHighSpeed = devCtx->busInterface.IsDeviceHighSpeed(devCtx->busInterface.BusContext);
		KDBG(DPFLTR_INFO_LEVEL, "USB 2.0");
	}else
		KDBG(DPFLTR_INFO_LEVEL, "USB 1.1");

	// 对系统资源列表，我们不做实质性的操作，仅仅打印一些信息。
	// 读者完全可以把下面的这些代码都注释掉。
	ulNumRes = WdfCmResourceListGetCount(ResourceList);
	KDBG(DPFLTR_INFO_LEVEL, "ResourceListCount:%d\n", ulNumRes);

	// 下面我们饶有兴致地枚举这些系统资源，并打印出它们的相关名称信息。
	for(ulIndex = 0; ulIndex < ulNumRes; ulIndex++)
	{
		pResDes = WdfCmResourceListGetDescriptor(ResourceList, ulIndex);		
		if(!pResDes)continue; // 取得失败，则跳到下一个
		
		switch (pResDes->Type) 
		{
			case CmResourceTypeMemory:
				KDBG(DPFLTR_INFO_LEVEL, "System Resource：CmResourceTypeMemory\n");
				break;

			case CmResourceTypePort:
				KDBG(DPFLTR_INFO_LEVEL, "System Resource：CmResourceTypePort\n");
				break;

			case CmResourceTypeInterrupt:
				KDBG(DPFLTR_INFO_LEVEL, "System Resource：CmResourceTypeInterrupt\n");
				break;

			default:
				KDBG(DPFLTR_INFO_LEVEL, "System Resource：Others %d\n", pResDes->Type);
				break;
		}
	}

	return STATUS_SUCCESS;
}

// 初始化结构体WDF_USB_INTERFACE_SETTING_PAIR。
// 用来配置多接口设备。
int  InitSettingPairs(
	IN WDFUSBDEVICE UsbDevice,					// 设备对象
	OUT PWDF_USB_INTERFACE_SETTING_PAIR Pairs,  // 结构体指针。
	IN ULONG NumSettings						// 接口个数
)
{
	int i;

 	// 最多支持8个接口，把多余的切掉。
	if(NumSettings > MAX_INTERFACES)
		NumSettings = MAX_INTERFACES;

	// 配置接口
	for(i = 0; i < NumSettings; i++) {
		Pairs[i].UsbInterface = WdfUsbTargetDeviceGetInterface(UsbDevice, i);// 设置接口句柄
		Pairs[i].SettingIndex = MultiInterfaceSettings[i];					 // 设置接口可选值(Alternate Setting)
	}

	return NumSettings;
}

// 设备配置
// 按照WDF框架，设备配置选项默认为1；如存在多个配置选项，需要切换选择的话，会比较麻烦。
// 一种办法是：使用初始化宏：WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_INTERFACES_DESCRIPTORS 
// 使用这个宏，需要首先获取配置描述符，和相关接口描述符。
// 另一种办法是：使用WDM方法，先构建一个配置选择的URB，然后要么自己调用IRP发送到总线驱动，
// 要么使用WDF方法调用WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_URB初始化宏。
// 
NTSTATUS ConfigureUsbDevice(WDFDEVICE Device, PDEVICE_CONTEXT DeviceContext)
{
	NTSTATUS status = STATUS_SUCCESS;
	WDF_USB_DEVICE_SELECT_CONFIG_PARAMS usbConfig;
	PWDF_USB_INTERFACE_SETTING_PAIR settingPairs;
	UCHAR numInterfaces;
	WDF_USB_INTERFACE_SELECT_SETTING_PARAMS  interfaceSelectSetting;

	KDBG(DPFLTR_INFO_LEVEL, "[ConfigureUsbDevice]");

	// 创建Usb设备对象。
	// USB设备对象是我们进行USB操作的起点。大部分的USB接口函数，都是针对它进行的。
	// USB设备对象被创建后，由驱动自己维护；框架本身不处理它，也不保持它。
	status = WdfUsbTargetDeviceCreate(Device, WDF_NO_OBJECT_ATTRIBUTES, &DeviceContext->UsbDevice);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfUsbTargetDeviceCreate failed with status 0x%08x\n", status);
		return status;
	}

	// 接口配置
	// WDF提供了多种接口配置的初始化宏，分别针对单一接口、多接口的USB设备，
	// 初始化宏还提供了在多个配置间进行切换的途径，正如上面所讲过的。
	// 在选择默认配置的情况下，设备配置将无比简单，简单到令长期受折磨的内核程序员大跌眼镜；
	// 因为WDM上百行的代码逻辑，这里只要两三行就够了。
	numInterfaces = WdfUsbTargetDeviceGetNumInterfaces(DeviceContext->UsbDevice);
	if(1 == numInterfaces)
	{
		KDBG(DPFLTR_INFO_LEVEL, "There is one interface.", status);
		WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_SINGLE_INTERFACE(&usbConfig);
	}
	else// 多接口
	{
		int nNum;
		KDBG(DPFLTR_INFO_LEVEL, "There are %d interfaces.", numInterfaces);
		settingPairs = ExAllocatePoolWithTag(PagedPool,
						sizeof(WDF_USB_INTERFACE_SETTING_PAIR) * numInterfaces, POOLTAG);

		if (settingPairs == NULL)
			return STATUS_INSUFFICIENT_RESOURCES;

		nNum = InitSettingPairs(DeviceContext->UsbDevice, settingPairs, numInterfaces);
		WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_MULTIPLE_INTERFACES(&usbConfig, nNum, settingPairs);
	}

	status = WdfUsbTargetDeviceSelectConfig(DeviceContext->UsbDevice, WDF_NO_OBJECT_ATTRIBUTES, &usbConfig);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfUsbTargetDeviceSelectConfig failed with status 0x%08x\n", status);
		return status;
	}

	// 保存接口
	if(1 == numInterfaces)
	{
		DeviceContext->UsbInterface = usbConfig.Types.SingleInterface.ConfiguredUsbInterface;

		// 使用SINGLE_INTERFACE接口配置宏，接口的AltSetting值默认为0。
		// 下面两行代码演示了如何手动修改某接口的AltSetting值（此处为1）.
		WDF_USB_INTERFACE_SELECT_SETTING_PARAMS_INIT_SETTING(&interfaceSelectSetting, 1);
		status = WdfUsbInterfaceSelectSetting(DeviceContext->UsbInterface, WDF_NO_OBJECT_ATTRIBUTES, &interfaceSelectSetting);
	}
	else
	{
		int i;
		DeviceContext->UsbInterface = usbConfig.Types.MultiInterface.Pairs[0].UsbInterface;
		for(i = 0; i < numInterfaces; i++)
			DeviceContext->MulInterfaces[i] = usbConfig.Types.MultiInterface.Pairs[i].UsbInterface;
	}

	return status;
}

//
// 设备配置好后，接口、管道就已存在了。
NTSTATUS GetUsbPipes(PDEVICE_CONTEXT DeviceContext)
{
	BYTE index = 0;
	WDF_USB_PIPE_INFORMATION pipeInfo;
	WDFUSBPIPE pipe = NULL;

	KDBG(DPFLTR_INFO_LEVEL, "[GetUsbPipes]");

	DeviceContext->UsbIntOutPipe = NULL;
	DeviceContext->UsbIntInPipe = NULL;
	DeviceContext->UsbBulkInPipe = NULL;
	DeviceContext->UsbBulkOutPipe = NULL;

	WDF_USB_PIPE_INFORMATION_INIT(&pipeInfo);

	while(TRUE)
	{
		pipe = WdfUsbInterfaceGetConfiguredPipe(DeviceContext->UsbInterface, index, &pipeInfo);
		if(NULL == pipe)break;

		// Dump 管道信息
		KDBG(DPFLTR_INFO_LEVEL, "Type:%s\r\nEndpointAddress:0x%x\r\nMaxPacketSize:%d\r\nAlternateValue:%d", 
			pipeInfo.PipeType == WdfUsbPipeTypeInterrupt ? "Interrupt" : 
			pipeInfo.PipeType == WdfUsbPipeTypeBulk ? "Bulk" :  
			pipeInfo.PipeType == WdfUsbPipeTypeControl ? "Control" : 
			pipeInfo.PipeType == WdfUsbPipeTypeIsochronous ? "Isochronous" : "Invalid!!",
			pipeInfo.EndpointAddress, 
			pipeInfo.MaximumPacketSize,
			pipeInfo.SettingIndex);

		// 设置管道属性：忽略包长度检查
		// 如果不设置，那么每次对管道进行写操作的时候，输入缓冲区的长度必须是
		// pipeInfo.MaximumPacketSize的整数倍，否则会操作失败。
		// 框架提供的这个额外检查，可避免驱动从总线获取到意想不到的杂乱信息。但我们此处忽略。
		WdfUsbTargetPipeSetNoMaximumPacketSizeCheck(pipe);

		if(WdfUsbPipeTypeControl == pipeInfo.PipeType)
		{
			DeviceContext->UsbCtlPipe = pipe;
		}
		else if(WdfUsbPipeTypeInterrupt == pipeInfo.PipeType)
		{ 
			if(TRUE == WdfUsbTargetPipeIsInEndpoint(pipe))
				DeviceContext->UsbIntInPipe = pipe;
			else
				DeviceContext->UsbIntOutPipe = pipe;
		}
		else if(WdfUsbPipeTypeBulk == pipeInfo.PipeType)
		{
			if(TRUE == WdfUsbTargetPipeIsInEndpoint(pipe))
			{
				DeviceContext->UsbBulkInPipe = pipe;
			}
			else if(TRUE == WdfUsbTargetPipeIsOutEndpoint(pipe))
			{
				DeviceContext->UsbBulkOutPipe = pipe;
			}
		}

		index++;
	}

	// 通过管道判断固件版本
	if((NULL == DeviceContext->UsbIntOutPipe) ||
		(NULL == DeviceContext->UsbIntInPipe) ||
		(NULL == DeviceContext->UsbBulkInPipe) ||
		(NULL == DeviceContext->UsbBulkOutPipe))
	{
		KDBG(DPFLTR_ERROR_LEVEL, "Not our CY001 firmware!!!");
	}

	return STATUS_SUCCESS;
}

