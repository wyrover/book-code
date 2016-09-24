
extern "C"{
#include "public.h"
};

#include "DrvClass.h"

// 多接口设备配置初始化Alter值
UCHAR MultiInterfaceSettings[MAX_INTERFACES] = {1, 1, 1, 1, 1, 1, 1, 1};

// USB总线接口GUID
// {B1A96A13-3DE0-4574-9B01-C08FEAB318D6}
static GUID USB_BUS_INTERFACE_USBDI_GUID1 =
{ 0xb1a96a13, 0x3de0, 0x4574, {0x9b, 0x1, 0xc0, 0x8f, 0xea, 0xb3, 0x18, 0xd6}};

// 用“GUID Generator”工具生成一个唯一的GUID。
static	GUID DeviceInterface = 
{0xdb713b3f, 0xea3f, 0x4d74, {0x89, 0x46, 0x0, 0x64, 0xdb, 0xa4, 0xfc, 0x6a}};

DrvClass* DrvClass::GetDrvClassFromDriver(WDFDRIVER Driver)
{
	PDRIVER_CONTEXT pContext = GetDriverContext(Driver);
	return (DrvClass*)pContext->par1;
}

DrvClass* DrvClass::GetDrvClassFromDevice(WDFDEVICE Device)
{
	PDEVICE_CONTEXT pContext = GetDeviceContext(Device);
	return (DrvClass*)pContext->par1;
}

NTSTATUS DrvClass::PnpAdd_sta(IN WDFDRIVER Driver, IN PWDFDEVICE_INIT DeviceInit)
{
	PDRIVER_CONTEXT pContext = GetDriverContext(Driver);
	DrvClass* pThis = (DrvClass*)pContext->par1;
	return pThis->PnpAdd(DeviceInit);
}

NTSTATUS DrvClass::PnpPrepareHardware_sta( 
					IN WDFDEVICE Device, 
					IN WDFCMRESLIST ResourceList, 
					IN WDFCMRESLIST ResourceListTranslated 
					)
{
	DrvClass* pThis = DrvClass::GetDrvClassFromDevice(Device);
	ASSERT(pThis);
	return pThis->PnpPrepareHardware(ResourceList, ResourceListTranslated);
}

NTSTATUS DrvClass::PnpReleaseHardware_sta(IN WDFDEVICE Device, 
										IN WDFCMRESLIST ResourceList)
{
	DrvClass* pThis = DrvClass::GetDrvClassFromDevice(Device);
	ASSERT(pThis);
	return pThis->PnpReleaseHardware(ResourceList);
}

VOID 
DrvClass::PnpSurpriseRemove_sta( IN WDFDEVICE  Device)
{
	DrvClass* pThis = DrvClass::GetDrvClassFromDevice(Device);
	ASSERT(pThis);
	pThis->PnpSurpriseRemove();

}

VOID DrvClass::PnpSurpriseRemove()
{
}

NTSTATUS 
DrvClass::PwrD0Entry_sta(
			   IN WDFDEVICE  Device, 
			   IN WDF_POWER_DEVICE_STATE  PreviousState
			   )
{
	DrvClass* pThis = DrvClass::GetDrvClassFromDevice(Device);
	ASSERT(pThis);
	return pThis->PwrD0Entry(PreviousState);
}

NTSTATUS 
DrvClass::PwrD0Exit_sta(
						 IN WDFDEVICE  Device, 
						 IN WDF_POWER_DEVICE_STATE  PreviousState
						 )
{
	DrvClass* pThis = DrvClass::GetDrvClassFromDevice(Device);
	ASSERT(pThis);
	return pThis->PwrD0Exit(PreviousState);
}

void DrvClass::InitPnpPwrEvents(WDF_PNPPOWER_EVENT_CALLBACKS* pPnpPowerCallbacks)
{
	pPnpPowerCallbacks->EvtDevicePrepareHardware	= PnpPrepareHardware_sta; // 初始化
	pPnpPowerCallbacks->EvtDeviceReleaseHardware	= PnpReleaseHardware_sta; // 停止
	pPnpPowerCallbacks->EvtDeviceSurpriseRemoval	= PnpSurpriseRemove_sta;  // 异常移除
	pPnpPowerCallbacks->EvtDeviceD0Entry	= PwrD0Entry_sta; // 进入D0电源状态（工作状态），比如初次插入、或者唤醒
	pPnpPowerCallbacks->EvtDeviceD0Exit		= PwrD0Exit_sta;  // 离开D0电源状态（工作状态），比如休眠或设备移除
}

void DrvClass::InitPnpCap(WDF_DEVICE_PNP_CAPABILITIES* pPnpCapabilities)
{
	pPnpCapabilities->Removable = WdfTrue;
	pPnpCapabilities->SurpriseRemovalOK = WdfTrue;
}

//
// 作用相当于WDM中的AddDevice函数。
// 他是PNP过程中首当其冲被调用的回调函数。主要任务是创建设备对象。
// 设备对象在系统中以软件形式，代表外部的某个物理硬件设备。
//NTSTATUS PnpAdd(IN WDFDRIVER  Driver, IN PWDFDEVICE_INIT  DeviceInit)
NTSTATUS DrvClass::PnpAdd(IN PWDFDEVICE_INIT DeviceInit)
{
	NTSTATUS status;
	WDFDEVICE device;
	int nInstance = 0;
	PDEVICE_CONTEXT pContext = NULL;
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

	KDBG(DPFLTR_INFO_LEVEL, "[PnpAdd]");

	RtlInitUnicodeString(&DeviceName, wcsDeviceName) ;
	RtlInitUnicodeString(&DosDeviceName, wcsDosDeviceName);
	RtlInitUnicodeString(&RefString, wcsRefString);

	WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
	InitPnpPwrEvents(&pnpPowerCallbacks);
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

	m_hDevice = device;// 保存设备对象句柄

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

	// 初始化环境块
	// GetDeviceContext是一个函数指针，由宏WDF_DECLARE_CONTEXT_TYPE_WITH_NAME定义。
	// 参看pulibc.h中的说明。
	pContext = GetDeviceContext(device);
	RtlZeroMemory(pContext, sizeof(DEVICE_CONTEXT));
	pContext->par1 = this; // 通过this指针可以得到一切

	// PNP属性。允许设备异常拔除，系统不会弹出错误框。
	WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpCapabilities);
	InitPnpCap(&pnpCapabilities);
	WdfDeviceSetPnpCapabilities(device, &pnpCapabilities);

	status = CreateWDFQueues();
	if(!NT_SUCCESS(status))
		return status;

	return status;
}

// 
// 创建队列。
NTSTATUS DrvClass::CreateWDFQueues()
{
	NTSTATUS status = STATUS_SUCCESS;

	WDF_IO_QUEUE_CONFIG ioQConfig;
	KDBG(DPFLTR_INFO_LEVEL, "[CreateWDFQueues]");

	// 创建默认并发队列，处理DeviceIOControl命令。
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQConfig, WdfIoQueueDispatchParallel);
	ioQConfig.EvtIoDefault  = EventDefault_sta;
	status = WdfIoQueueCreate(m_hDevice, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &m_hDefQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
	}

	return status;
}

VOID DrvClass::EventDefault_sta(IN WDFQUEUE  Queue, IN WDFREQUEST  Request)
{
	DrvClass* pThis = GetDrvClassFromDevice(WdfIoQueueGetDevice(Queue));
	pThis->EventDefault(Queue, Request);
}

VOID DrvClass::EventDefault(IN WDFQUEUE  Queue, IN WDFREQUEST  Request)
{
	// 默认的处理方式是，立刻完成所有的请求命令。
	WdfRequestComplete(Request, STATUS_SUCCESS);
}

// 此函数类似于WDM中的PNP_MN_START_DEVICE函数，紧接着PnpAdd之后被调用。
// 此时PNP管理器经过甄别之后，已经决定将那些系统资源分配给当前设备。
// 参数ResourceList和ResourceListTranslated代表了这些系统资源。
// 当个函数被调用时候，设备已经进入了D0电源状态；函数完成后，设备即正式进入工作状态。
NTSTATUS DrvClass::PnpPrepareHardware(IN WDFCMRESLIST ResourceList, IN WDFCMRESLIST ResourceListTranslated)
{
	NTSTATUS status;
	PDEVICE_CONTEXT pContext = NULL; 
	ULONG ulNumRes = 0;
	ULONG ulIndex;
	CM_PARTIAL_RESOURCE_DESCRIPTOR*  pResDes = NULL;

	KDBG(DPFLTR_INFO_LEVEL, "[PnpPrepareHardware]");

	pContext = GetDeviceContext(m_hDevice);

	// 配置设备
	status = ConfigureUsbDevice();
	if(!NT_SUCCESS(status))
		return status;

	// 获取Pipe句柄
	status = GetUsbPipes();
	if(!NT_SUCCESS(status))
		return status;

	// 初始电源策略，
	status = InitPowerManagement();

	// 获取USB总线驱动接口。总线接口中包含总线驱动提供的回调函数和其他信息。
	// 总线接口由系统共用GUID标识。
	status = WdfFdoQueryForInterface(
		m_hDevice,
		&USB_BUS_INTERFACE_USBDI_GUID1,		// 总线接口ID
		(PINTERFACE)&m_busInterface,		// 保存在设备环境块中
		sizeof(USB_BUS_INTERFACE_USBDI_V1),
		1, NULL);

	// 调用接口函数，判断USB版本。
	if(NT_SUCCESS(status) && m_busInterface.IsDeviceHighSpeed){
		if(m_busInterface.IsDeviceHighSpeed(m_busInterface.BusContext))
			KDBG(DPFLTR_INFO_LEVEL, "USB 2.0");
		else
			KDBG(DPFLTR_INFO_LEVEL, "USB 1.1");
	}

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

// 此函数类似于WDM中的PNP_MN_STOP_DEVICE函数，在设备移除时被调用。
// 当个函数被调用时候，设备仍处于工作状态。
NTSTATUS DrvClass::PnpReleaseHardware(IN WDFCMRESLIST ResourceListTranslated)
{
	KDBG(DPFLTR_INFO_LEVEL, "[PnpReleaseHardware]");

	// 如果PnpPrepareHardware调用失败,UsbDevice为空；
	// 这时候直接返回即可。
	if (m_hUsbDevice == NULL)
		return STATUS_SUCCESS;

	// 取消USB设备的所有IO操作。它将连带取消所有Pipe的IO操作。
	WdfIoTargetStop(WdfUsbTargetDeviceGetIoTarget(m_hUsbDevice), WdfIoTargetCancelSentIo);

	// Deconfiguration或者“反配置”
	WDF_USB_DEVICE_SELECT_CONFIG_PARAMS  configParams;
	WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_DECONFIG(&configParams);
	return WdfUsbTargetDeviceSelectConfig(m_hUsbDevice, WDF_NO_OBJECT_ATTRIBUTES, &configParams);
}

// 配置设备驱动的电源管理功能
NTSTATUS DrvClass::InitPowerManagement()
{
	NTSTATUS status = STATUS_SUCCESS;
	WDF_USB_DEVICE_INFORMATION usbInfo;

	KDBG(DPFLTR_INFO_LEVEL, "[InitPowerManagement]");

	// 获取设备信息
	WDF_USB_DEVICE_INFORMATION_INIT(&usbInfo);
	WdfUsbTargetDeviceRetrieveInformation(m_hUsbDevice, &usbInfo);

	// USB设备信息以掩码形式被保存在Traits中。
	KDBG( DPFLTR_INFO_LEVEL, "Device self powered: %s", 
		usbInfo.Traits & WDF_USB_DEVICE_TRAIT_SELF_POWERED ? "TRUE" : "FALSE");
	KDBG( DPFLTR_INFO_LEVEL, "Device remote wake capable: %s",
		usbInfo.Traits & WDF_USB_DEVICE_TRAIT_REMOTE_WAKE_CAPABLE ? "TRUE" : "FALSE");
	KDBG( DPFLTR_INFO_LEVEL, "Device high speed: %s",
		usbInfo.Traits & WDF_USB_DEVICE_TRAIT_AT_HIGH_SPEED ? "TRUE" : "FALSE");

	m_bIsDeviceHighSpeed = usbInfo.Traits & WDF_USB_DEVICE_TRAIT_AT_HIGH_SPEED;

	// 设置设备的休眠和远程唤醒功能
	if(usbInfo.Traits & WDF_USB_DEVICE_TRAIT_REMOTE_WAKE_CAPABLE)
	{
		WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS idleSettings;
		WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS wakeSettings;

		// 设置设备为闲时休眠。闲时超过10S，自动进入休眠状态。
		WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT(&idleSettings, IdleUsbSelectiveSuspend);
		idleSettings.IdleTimeout = 10000;
		status = WdfDeviceAssignS0IdleSettings(m_hDevice, &idleSettings);
		if(!NT_SUCCESS(status))
		{
			KDBG( DPFLTR_ERROR_LEVEL, "WdfDeviceAssignS0IdleSettings failed with status 0x%0.8x!!!", status);
			return status;
		}

		// 设置为可远程唤醒。包含设备自身醒来，已经当PC系统进入休眠后，设备可以将系统唤醒，两个方面。
		WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS_INIT(&wakeSettings);
		status = WdfDeviceAssignSxWakeSettings(m_hDevice, &wakeSettings);
		if(!NT_SUCCESS(status))
		{
			KDBG( DPFLTR_ERROR_LEVEL, "WdfDeviceAssignSxWakeSettings failed with status 0x%0.8x!!!", status);
			return status;
		}
	}

	return status;
}

//下面两个Power回调，和WDM中的PnpSetPower类似。
NTSTATUS DrvClass::PwrD0Entry(IN WDF_POWER_DEVICE_STATE  PreviousState)
{
	KDBG(DPFLTR_INFO_LEVEL, "[PwrD0Entry] from %s", PowerName(PreviousState));
	return STATUS_SUCCESS;
}

// 离开D0状态
NTSTATUS DrvClass::PwrD0Exit(IN WDF_POWER_DEVICE_STATE  TargetState)
{
	KDBG(DPFLTR_INFO_LEVEL, "[PwrD0Exit] to %s", PowerName(TargetState));
	return STATUS_SUCCESS;
}

// 初始化结构体WDF_USB_INTERFACE_SETTING_PAIR。
// 用来配置多接口设备。
int  InitSettingPairs(IN WDFUSBDEVICE UsbDevice,				// 设备对象
					  OUT PWDF_USB_INTERFACE_SETTING_PAIR Pairs,// 结构体指针。
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
NTSTATUS DrvClass::ConfigureUsbDevice()
{
	WDF_USB_DEVICE_SELECT_CONFIG_PARAMS usbConfig;
	WDF_USB_INTERFACE_SELECT_SETTING_PARAMS  interfaceSelectSetting;

	KDBG(DPFLTR_INFO_LEVEL, "[ConfigureUsbDevice]");

	// 创建Usb设备对象。
	// USB设备对象是我们进行USB操作的起点。大部分的USB接口函数，都是针对它进行的。
	// USB设备对象被创建后，由驱动自己维护；框架本身不处理它，也不保持它。
	NTSTATUS status = WdfUsbTargetDeviceCreate(m_hDevice, WDF_NO_OBJECT_ATTRIBUTES, &m_hUsbDevice);
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
	UCHAR numInterfaces = WdfUsbTargetDeviceGetNumInterfaces(m_hUsbDevice);
	if(1 == numInterfaces)
	{
		KDBG(DPFLTR_INFO_LEVEL, "There is one interface.", status);
		WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_SINGLE_INTERFACE(&usbConfig);
	}
	else// 多接口
	{
		KDBG(DPFLTR_INFO_LEVEL, "There are %d interfaces.", numInterfaces);

		PWDF_USB_INTERFACE_SETTING_PAIR settingPairs = (WDF_USB_INTERFACE_SETTING_PAIR*)
			ExAllocatePoolWithTag(PagedPool,
				sizeof(WDF_USB_INTERFACE_SETTING_PAIR) * numInterfaces, POOLTAG);

		if (settingPairs == NULL)
			return STATUS_INSUFFICIENT_RESOURCES;

		int nNum = InitSettingPairs(m_hUsbDevice, settingPairs, numInterfaces);
		WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_MULTIPLE_INTERFACES(&usbConfig, nNum, settingPairs);
	}

	status = WdfUsbTargetDeviceSelectConfig(m_hUsbDevice, WDF_NO_OBJECT_ATTRIBUTES, &usbConfig);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfUsbTargetDeviceSelectConfig failed with status 0x%08x\n", status);
		return status;
	}

	// 保存接口
	if(1 == numInterfaces)
	{
		m_hUsbInterface = usbConfig.Types.SingleInterface.ConfiguredUsbInterface;

		// 使用SINGLE_INTERFACE接口配置宏，接口的AltSetting值默认为0。
		// 下面两行代码演示了如何手动修改某接口的AltSetting值（此处为1）.
		WDF_USB_INTERFACE_SELECT_SETTING_PARAMS_INIT_SETTING(&interfaceSelectSetting, 1);
		status = WdfUsbInterfaceSelectSetting(m_hUsbInterface, WDF_NO_OBJECT_ATTRIBUTES, &interfaceSelectSetting);
	}
	else
	{
		int i;
		m_hUsbInterface = usbConfig.Types.MultiInterface.Pairs[0].UsbInterface;
		for(i = 0; i < numInterfaces; i++)
			m_hMulInterfaces[i] = usbConfig.Types.MultiInterface.Pairs[i].UsbInterface;
	}

	return status;
}

//
// 设备配置好后，接口、管道就已存在了。
NTSTATUS DrvClass::GetUsbPipes()
{
	return STATUS_SUCCESS;
}


char* PowerName(WDF_POWER_DEVICE_STATE PowerState)
{
	char *name;

	switch(PowerState)
	{
	case WdfPowerDeviceInvalid :	
		name = "PowerDeviceUnspecified"; 
		break;
	case WdfPowerDeviceD0:			
		name = "WdfPowerDeviceD0"; 
		break;
	case WdfPowerDeviceD1:			
		name = "WdfPowerDeviceD1"; 
		break;
	case WdfPowerDeviceD2:		
		name = "WdfPowerDeviceD2"; 
		break;
	case WdfPowerDeviceD3:		
		name = "WdfPowerDeviceD3"; 
		break;
	case WdfPowerDeviceD3Final:	
		name = "WdfPowerDeviceD3Final";
		break;
	case WdfPowerDevicePrepareForHibernation:									
		name = "WdfPowerDevicePrepareForHibernation"; 
		break;
	case WdfPowerDeviceMaximum:		
		name = "WdfPowerDeviceMaximum";
		break;
	default:					
		name = "Unknown Power State";
		break;
	}

	return name;
}