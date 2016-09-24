/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：DeviceIO.c
	说明：DeviceIO处理：并行、序列、手动
	历史：
	创建：
*/

#include <initguid.h>
#include "public.h"
#include "ioctl.h"
#include <ntstrsafe.h>

#pragma code_seg("PAGE")

// 并行处理
VOID DeviceIoControlParallel(IN WDFQUEUE  Queue,
						IN WDFREQUEST  Request,
						IN size_t  OutputBufferLength,
						IN size_t  InputBufferLength,
						IN ULONG  IoControlCode)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ulRetLen = 0;

	size_t size = 0;
	void* pBufferInput = NULL;
	void* pBufferOutput = NULL;
	WDFDEVICE Device = WdfIoQueueGetDevice(Queue);// 取得设备句柄
	PDEVICE_CONTEXT pContext = GetDeviceContext(Device); // 取得WDF设备对象的环境块指针

	KDBG(DPFLTR_INFO_LEVEL, "[DeviceIoControlParallel] CtlCode:0x%0.8X", IoControlCode);

	// 取得输入缓冲区，判断其有效性
	if(InputBufferLength){
		status = WdfRequestRetrieveInputBuffer(Request, InputBufferLength, &pBufferInput, &size);
		if(status != STATUS_SUCCESS || pBufferInput == NULL || size < InputBufferLength){
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
			return;
		}
	}

	// 取得输出缓冲区，判断其有效性
	if(OutputBufferLength){
		status = WdfRequestRetrieveOutputBuffer(Request, OutputBufferLength, &pBufferOutput, &size);
		if(status != STATUS_SUCCESS || pBufferOutput == NULL || size < OutputBufferLength){
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
			return;
		}
	}

	//
	// 下面是主处理过程。
	//
	switch(IoControlCode)
	{
		// 取得驱动的版本信息
	case IOCTL_GET_DRIVER_VERSION:
		{
			PDRIVER_VERSION pVersion = (PDRIVER_VERSION)pBufferOutput;
			ULONG length;
			char tcsBuffer[120];
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_GET_DRIVER_VERSION");

			if(OutputBufferLength < sizeof(DRIVER_VERSION)){
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}

			pVersion->DriverType = DR_WDF;
			pVersion->FirmwareType = FW_NOT_CY001;
			ulRetLen = sizeof(DRIVER_VERSION);// 告示返回长度

			// 根据String描述符，判断Firmware代码是否已经被加载。
			GetStringDes(2, 0, tcsBuffer, 120, &length, pContext);

			if(length){
				WCHAR* pCyName = L"CY001 V";
				size_t len;
				int nIndex;

				if(length < 8)
					break;

				RtlStringCchLengthW(pCyName, 7, &len);
				for(nIndex = 0; nIndex < len; nIndex++){
					if(pCyName[nIndex] != ((WCHAR*)tcsBuffer)[nIndex])
						break;
				}

				if(nIndex == len)
					pVersion->FirmwareType = FW_CY001; // 完全相符，说明新版Firmware已经加载到开发板。
			}
			break;
		}
		
		// 收到App发送过来的一个同步Request，我们应该把它保存到同步Queue中，等到有同步事件发生的时候再从Queue中取出并完成。
	case IOCTL_USB_SYNC:
		KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_SYNC");
		status = WdfRequestForwardToIoQueue(Request, pContext->AppSyncManualQueue);

		// 直接返回，不调用WdfRequestComplete函数。
		// 请求者将不会为此而等待；请求的完成在将来的某个时刻。
		// 这就是所谓的异步处理之要义了。
		if(NT_SUCCESS(status))
			return;
		break;

		// 清空同步队列中的所有请求
	case IOCTL_USB_SYNC_RELEASE:
		KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_SYNC");
		ClearSyncQueue(Device);
		break;

		// 应用程序退出，取消所有被阻塞的请求。
	case IOCTL_APP_EXIT_CANCEL: 
			
		// 取消USB设备的所有IO操作。它将连带取消所有Pipe的IO操作。
		//WdfIoTargetStop(WdfUsbTargetDeviceGetIoTarget(pContext->UsbDevice), WdfIoTargetCancelSentIo);
		break;

		// 取得当前的配置号.总是设置为0,因为在WDF框架中,0以外的配置是不被支持的。
	case IOCTL_USB_GET_CURRENT_CONFIG:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_CURRENT_CONFIG");
			if(InputBufferLength < 4){
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			*(PULONG)pBufferInput = 0;// 直接赋值0，即总是选择0号配置。也可以发送URB到总线获取当前配置选项。
			ulRetLen = sizeof(ULONG);
			break;
		}

	case IOCTL_USB_ABORTPIPE:
		{
			ULONG pipenum = *((PULONG) pBufferOutput);
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_ABORTPIPE");

			status = AbortPipe(Device, pipenum);
		}      
		break;

		// 获取Pipe信息
	case IOCTL_USB_GET_PIPE_INFO:
		{
			// 遍历获取Pipe信息，复制到输出缓冲中。
			BYTE byCurSettingIndex = 0;
			BYTE byPipeNum = 0;
			BYTE index;
			USB_INTERFACE_DESCRIPTOR  interfaceDescriptor;
			WDF_USB_PIPE_INFORMATION  pipeInfor;

			WDFUSBINTERFACE Interface = pContext->UsbInterface;// 接口句柄

			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_PIPE_INFO");

			// 取得Pipe数。根据Pipe数计算缓冲区长度
			byCurSettingIndex = WdfUsbInterfaceGetConfiguredSettingIndex(Interface); 
			WdfUsbInterfaceGetDescriptor(Interface, byCurSettingIndex, &interfaceDescriptor);
			byPipeNum = WdfUsbInterfaceGetNumConfiguredPipes(Interface);		

			if(OutputBufferLength < byPipeNum * sizeof(pipeInfor)){
				status = STATUS_BUFFER_TOO_SMALL; // 缓冲区不足
			}else{

				ulRetLen = byPipeNum*sizeof(pipeInfor);

				// 遍历获取全部管道信息，拷贝到输出缓冲中。
				// 应用程序得到输出缓冲的时候，也应该使用WDF_USB_PIPE_INFORMATION结构体解析缓冲区。
				for(index = 0; index < byPipeNum; index++)
				{
					WDF_USB_PIPE_INFORMATION_INIT(&pipeInfor);
					WdfUsbInterfaceGetEndpointInformation(Interface, byCurSettingIndex, index, &pipeInfor);
					RtlCopyMemory((PUCHAR)pBufferOutput + index*pipeInfor.Size, &pipeInfor, sizeof(pipeInfor));
				}
			}
		}

		break;

		// 获取设备描述符
	case IOCTL_USB_GET_DEVICE_DESCRIPTOR:
		{
			USB_DEVICE_DESCRIPTOR  UsbDeviceDescriptor;
			WdfUsbTargetDeviceGetDeviceDescriptor(pContext->UsbDevice, &UsbDeviceDescriptor);
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_DEVICE_DESCRIPTOR");

			// 判断输入缓冲区的长度是否足够长
			if(OutputBufferLength < UsbDeviceDescriptor.bLength)
				status = STATUS_BUFFER_TOO_SMALL;
			else{
				RtlCopyMemory(pBufferOutput, &UsbDeviceDescriptor, UsbDeviceDescriptor.bLength);
				ulRetLen = UsbDeviceDescriptor.bLength;
			}

			break;
		}

		// 获取字符串描述符
	case IOCTL_USB_GET_STRING_DESCRIPTOR:
		{
			PGET_STRING_DESCRIPTOR Input = (PGET_STRING_DESCRIPTOR)pBufferInput;
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_STRING_DESCRIPTOR");
			status = GetStringDes(Input->Index, Input->LanguageId, pBufferOutput, OutputBufferLength, &ulRetLen, pContext);
			
			// 由字符长度调整为字节长度
			if(NT_SUCCESS(status) && ulRetLen > 0)
				ulRetLen *= (sizeof(WCHAR)/sizeof(char));
			break;
		}

		// 获取配置描述信息。
	case IOCTL_USB_GET_CONFIGURATION_DESCRIPTOR:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_CONFIGURATION_DESCRIPTOR");

			// 首先获得配置描述符的长度。
			status = WdfUsbTargetDeviceRetrieveConfigDescriptor(pContext->UsbDevice, NULL, &size);
			if(!NT_SUCCESS(status) && status != STATUS_BUFFER_TOO_SMALL)
				break;

			// 输出缓冲区不够长
			if(OutputBufferLength < size)
				break;

			// 正式取得配置描述符。
			status = WdfUsbTargetDeviceRetrieveConfigDescriptor(pContext->UsbDevice, pBufferOutput, &size);
			if(!NT_SUCCESS(status))
				break;

			ulRetLen = size;
			break;
		}

		// 根据可选值配置接口
	case IOCTL_USB_SET_INTERFACE:
		{
			BYTE byAlterSetting = *(BYTE*)pBufferInput;
			WDFUSBINTERFACE usbInterface = pContext->UsbInterface;
			BYTE byCurSetting = WdfUsbInterfaceGetConfiguredSettingIndex(usbInterface); // 当前Alternate值

			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_SETINTERFACE");

			if(InputBufferLength < 1 || OutputBufferLength < 1)
			{
				status = STATUS_BUFFER_TOO_SMALL;
				break;
			}
			
			// 如果传入的可选值与当前的不同，则重新配置接口；
			// 否则直接返回。
			if(byCurSetting != byAlterSetting)
			{
				WDF_USB_INTERFACE_SELECT_SETTING_PARAMS par;
				WDF_USB_INTERFACE_SELECT_SETTING_PARAMS_INIT_SETTING(&par, byAlterSetting);
				status = WdfUsbInterfaceSelectSetting(usbInterface, NULL, &par);
			}

			*(BYTE*)pBufferOutput = byCurSetting;
			break;
		}

		// 固件Rest。自定义命令，与Port Rest是两码事。
	case IOCTL_USB_FIRMWRAE_RESET:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_FIRMWRAE_RESET");
			if(InputBufferLength < 1 || pBufferInput == NULL)
				status = STATUS_INVALID_PARAMETER;
			else
				status = FirmwareReset(Device, *(char*)pBufferInput);

			break;
		}

		// 重置USB总线端口
	case IOCTL_USB_PORT_RESET:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_PORT_RESET");			
			WdfUsbTargetDeviceResetPortSynchronously(pContext->UsbDevice);
			break;
		}

		// 管道重置
	case IOCTL_USB_PIPE_RESET:
		{
			UCHAR uchPipe;
			WDFUSBPIPE pipe = NULL;

			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_PIPE_RESET");			

			if(InputBufferLength < 1){
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			// 根据ID找到对应的Pipe
			uchPipe = *(UCHAR*)pBufferInput;
			pipe = WdfUsbInterfaceGetConfiguredPipe(pContext->UsbInterface, uchPipe, NULL);
			if(pipe == NULL){ 
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			status = WdfUsbTargetPipeResetSynchronously(pipe, NULL, NULL);
			break;
		}

		// 中断管道，放弃管道当前正在进行的操作
	case IOCTL_USB_PIPE_ABORT:
		{
			UCHAR uchPipe;
			WDFUSBPIPE pipe = NULL;

			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_PIPE_ABORT");

			if(InputBufferLength < 1){
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			// 根据ID找到对应的Pipe
			uchPipe = *(UCHAR*)pBufferInput;
			pipe = WdfUsbInterfaceGetConfiguredPipe(pContext->UsbInterface, uchPipe, NULL);
			if(pipe == NULL){ 
				status = STATUS_INVALID_PARAMETER;
				break;
			}
			
			status = WdfUsbTargetPipeAbortSynchronously(pipe, NULL, NULL);
			break;
		}

		// 取得驱动错误信息，驱动总是把最后一次发现的错误保存在设备对象的环境块中。
		// 这个逻辑虽然实现了，但目前的版本中，应用程序并没有利用这个接口。
	case IOCTL_USB_GET_LAST_ERROR:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_LAST_ERROR");

			if (OutputBufferLength >= sizeof(ULONG))
				*((PULONG)pBufferOutput) = pContext->LastUSBErrorStatusValue;
			else
				status = STATUS_BUFFER_TOO_SMALL;

			ulRetLen = sizeof(ULONG);
			break;
		}

		// Clear feature命令
	case IOCTL_USB_SET_CLEAR_FEATURE:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_SET_CLEAR_FEATURE");
			status = UsbSetOrClearFeature(Device, Request);
			break;
		}

		// 为USB设备加载固件程序。带有偏移量参数，用这个分支；不带偏移量，可用下一个分支。
		// 带偏移量的情况下，固件代码是一段一段地加载；
		// 不带偏移量的情况，固件代码作为一整块一次性被加载。
	case IOCTL_FIRMWARE_UPLOAD_OFFSET:
		{
			void* pData = pBufferOutput;
			WORD offset = 0;

			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_FIRMWARE_UPLOAD_OFFSET");

			if(InputBufferLength < sizeof(WORD)){
				status = STATUS_INVALID_PARAMETER;
				break;
			}

			offset = *(WORD*)pBufferInput;
			status = FirmwareUpload(WdfIoQueueGetDevice(Queue), pData, OutputBufferLength, offset);
			break;
		}

		// 为USB设备加载固件程序。
	case IOCTL_FIRMWARE_UPLOAD:
		{
			void* pData = pBufferOutput;
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_FIRMWARE_UPLOAD");
			status = FirmwareUpload(WdfIoQueueGetDevice(Queue), pData, InputBufferLength, 0);
			break;
		}

		// 读取开发板设备的RAM内容。RAM也就是内存。
		// 每次从同一地址读取的内容可能不尽相同，开发板中固件程序在不断运行，RAM被用来储数据（包括临时数据）。
	case IOCTL_FIRMWARE_READ_RAM:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_FIRMWARE_READ_RAM");
			status = ReadRAM(WdfIoQueueGetDevice(Queue), Request, &ulRetLen);// inforVal中保存读取的长度
			break;
		}

		// 其他的请求
	default:
		{
			// 一律转发到SerialQueue中去。			
			WdfRequestForwardToIoQueue(Request, pContext->IoControlSerialQueue);

			// 命令转发之后，这里必须直接返回，千万不可调用WdfRequestComplete函数。
			// 否则会导致一个Request被完成两次的错误。
			return;
		}
	}

	// 完成请求
	WdfRequestCompleteWithInformation(Request, status, ulRetLen);
}

// 这里面的IO操作是经过序列化的。一个挨着一个，所以绝不会发生重入问题。
//
VOID DeviceIoControlSerial(IN WDFQUEUE  Queue,
						 IN WDFREQUEST  Request,
						 IN size_t  OutputBufferLength,
						 IN size_t  InputBufferLength,
						 IN ULONG  IoControlCode)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	ULONG ulRetLen = 0;
	SIZE_T size;

	void* pBufferInput = NULL;
	void* pBufferOutput = NULL;
	WDFDEVICE Device = WdfIoQueueGetDevice(Queue);// 取得设备句柄
	PDEVICE_CONTEXT pContext = GetDeviceContext(Device); // 取得WDF设备对象的环境块指针

	KDBG(DPFLTR_INFO_LEVEL, "[DeviceIoControlSerial]");

	// 取得输入/输出缓冲区
	if(InputBufferLength)WdfRequestRetrieveInputBuffer(Request, InputBufferLength, &pBufferInput, &size);
	if(OutputBufferLength)WdfRequestRetrieveOutputBuffer(Request, OutputBufferLength, &pBufferOutput, &size);

	switch(IoControlCode)
	{
		// 设置数码管
	case IOCTL_USB_SET_DIGITRON:
		{
			CHAR ch = *(CHAR*)pBufferInput;
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_SET_DIGITRON");
			SetDigitron(Device, ch);
			break;
		}

		// 读数码管
	case IOCTL_USB_GET_DIGITRON:
		{
			UCHAR* pCh = (UCHAR*)pBufferOutput;
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_DIGITRON");
			GetDigitron(Device, pCh);
			ulRetLen = 1;
			break;
		}

		// 设置LED灯（共4盏）
	case IOCTL_USB_SET_LEDs:
		{
			CHAR ch = *(CHAR*)pBufferInput;
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_SET_LEDs");
			SetLEDs(Device, ch);
			break;
		}

		// 读取LED灯（共4盏）的当前状态
	case IOCTL_USB_GET_LEDs:
		{
			UCHAR* pCh = (UCHAR*)pBufferOutput;
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_GET_LEDs");
			GetLEDs(Device, pCh);
			ulRetLen = 1;
			break;
		}

		// 控制命令。
		// 分为：USB协议预定义命令、Vendor自定义命令、特殊类(class)命令。
	case IOCTL_USB_CTL_REQUEST:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_USB_CTL_REQUEST");
			ntStatus = UsbControlRequest(Device, Request);
			if(NT_SUCCESS(ntStatus))return;
			break;
		}

		// 开启中断读
	case IOCTL_START_INT_READ:	
		KDBG(DPFLTR_INFO_LEVEL, "IOCTL_START_INT_READ");	
		ntStatus = InterruptReadStart(Device);
		break;

		// 控制程序发送读请求。它们是被阻塞的，放至Queue中排队，所以不要即可完成他们。
	case IOCTL_INT_READ_KEYs:
		KDBG(DPFLTR_INFO_LEVEL, "IOCTL_INT_READ_KEYs");
		ntStatus = WdfRequestForwardToIoQueue(Request, pContext->InterruptManualQueue);

		if(NT_SUCCESS(ntStatus))
			return;// 成功，直接返回;异步完成。
		break;

		// 终止中断读
	case IOCTL_STOP_INT_READ:
		KDBG(DPFLTR_INFO_LEVEL, "IOCTL_STOP_INT_READ");
		InterruptReadStop(Device);
		ntStatus = STATUS_SUCCESS;
		break;

	default:
		// 不应该到这里。
		// 对于不能识别的IO控制命令，这里做错误处理。
		KDBG(DPFLTR_INFO_LEVEL, "Unknown Request: %08x(%d)!!!", IoControlCode, IoControlCode);
		ntStatus = STATUS_INVALID_PARAMETER;
		break;
	}

	WdfRequestCompleteWithInformation(Request, ntStatus, ulRetLen);
	return;
}
