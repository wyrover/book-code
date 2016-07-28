
#include "CY001Drv.h"
#include "newdelete.h"

// 子类申明
REGISTER_DRV_CLASS(CY001Drv)

VOID CY001Drv::PnpSurpriseRemove()
{
	CompleteSyncRequest(SURPRISE_REMOVE, 0);
}

// 
// 创建队列。
// 为了演示，我们创建了多种类型的队列。
NTSTATUS CY001Drv::CreateWDFQueues()
{
	NTSTATUS status = STATUS_SUCCESS;

	WDF_IO_QUEUE_CONFIG ioQConfig;
	KDBG(DPFLTR_INFO_LEVEL, "CY001Drv:[CreateWDFQueues]");

	// 创建框架Queue对象。
	// 注意事项：1. 调用WdfIoQueueCreate时候第一个参数Device被默认为Queue对象的父对象，父对象负责并维护子对象的生命周期。
	//				程序不必维护Queue对象。它只需创建，无需销毁。
	//			 2. 可设置其他的父对象，在参数WDF_OBJECT_ATTRIBUTES中设置。
	//			 3. Queue对象类型有：并行、串行、手动三种。
	//			 4. 执行WdfDeviceConfigureRequestDispatching以令某一类型的请求都排队到此Queue。

	// 创建默认并发队列，处理DeviceIOControl命令。
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQConfig, WdfIoQueueDispatchParallel);
	ioQConfig.EvtIoDeviceControl = DeviceIoControlParallel_sta;
	status = WdfIoQueueCreate(m_hDevice, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &m_hIoCtlEntryQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
		return status;
	}

	// 第二个队列：串列化队列，处理默认队列转发给它的需要串行处理的命令。
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchSequential);
	ioQConfig.EvtIoDeviceControl = DeviceIoControlSerial_sta;
	status = WdfIoQueueCreate(m_hDevice, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &m_hIoCtlSerialQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
		return status;
	}

	// 第三个队列：串列化队列（读写操作）
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchSequential);
	ioQConfig.EvtIoWrite = BulkWrite_sta;
	ioQConfig.EvtIoRead  = BulkRead_sta;
	status = WdfIoQueueCreate(m_hDevice, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES,	&m_hIoRWQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate failed with status 0x%08x\n", status);
		return status;
	}

	// 配置第三个队列，只接受读和写两种请求。
	status  = WdfDeviceConfigureRequestDispatching(m_hDevice, m_hIoRWQueue, WdfRequestTypeWrite);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfDeviceConfigureRequestDispatching failed with status 0x%08x\n", status);
		return status;
	}
	status  = WdfDeviceConfigureRequestDispatching(m_hDevice, m_hIoRWQueue, WdfRequestTypeRead);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfDeviceConfigureRequestDispatching failed with status 0x%08x\n", status);
		return status;
	}

	// 第四个队列：手动队列。这个队列中保存应用程序与驱动进行同步的请求。
	// 当驱动有消息通知应用程序时，即从手动队列中提取一个请求完成之。
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchManual);
	status = WdfIoQueueCreate(m_hDevice, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &m_hAppSyncManualQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate for manual queue failed with status 0x%08x\n", status);
		return status;
	}

	// 第五个队列：手动队列。这个队列用于处理中断数据，当驱动收到中断数据时，就从队列中提取一个请求并完成之。
	// 鼠标、键盘等中断设备，需用这种形式。
	WDF_IO_QUEUE_CONFIG_INIT(&ioQConfig, WdfIoQueueDispatchManual);
	status = WdfIoQueueCreate(m_hDevice, &ioQConfig, WDF_NO_OBJECT_ATTRIBUTES, &m_hInterruptManualQueue);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfIoQueueCreate for manual queue failed with status 0x%08x\n", status);
		return status;
	}

	return status;
}

//下面两个Power回调，和WDM中的PnpSetPower类似。
NTSTATUS CY001Drv::PwrD0Entry(IN WDF_POWER_DEVICE_STATE  PreviousState)
{
	KDBG(DPFLTR_INFO_LEVEL, "CY001Drv:[PwrD0Entry] from %s", PowerName(PreviousState));

	if(PreviousState == PowerDeviceD2)
	{
		SetDigitron(byPre7Seg);
		SetLEDs(byPreLEDs);
	}

	CompleteSyncRequest(ENTERD0, PreviousState);

	return STATUS_SUCCESS;
}

// 离开D0状态
NTSTATUS CY001Drv::PwrD0Exit(IN WDF_POWER_DEVICE_STATE  TargetState)
{
	KDBG(DPFLTR_INFO_LEVEL, "CY001Drv:[PwrD0Exit] to %s", PowerName(TargetState));

	if(TargetState == PowerDeviceD2)
	{		
		GetDigitron(&byPre7Seg);
		GetLEDs(&byPreLEDs);
	}

	CompleteSyncRequest(EXITD0, TargetState);

	// 停止中断读操作
	InterruptReadStop();
	ClearSyncQueue();
	return STATUS_SUCCESS;
}

//
// 设备配置好后，接口、管道就已存在了。
NTSTATUS CY001Drv::GetUsbPipes()
{
	KDBG(DPFLTR_INFO_LEVEL, "CY001Drv:[GetUsbPipes]");

	m_hUsbCtlPipe = NULL;
	m_hUsbIntOutPipe = NULL;
	m_hUsbIntInPipe = NULL;
	m_hUsbBulkInPipe = NULL;
	m_hUsbBulkOutPipe = NULL;

	BYTE index = 0;
	WDFUSBPIPE pipe = NULL;
	WDF_USB_PIPE_INFORMATION pipeInfo;

	WDF_USB_PIPE_INFORMATION_INIT(&pipeInfo);

	while(TRUE)
	{
		pipe = WdfUsbInterfaceGetConfiguredPipe(m_hUsbInterface, index, &pipeInfo);
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
			m_hUsbCtlPipe = pipe;
		}
		else if(WdfUsbPipeTypeInterrupt == pipeInfo.PipeType)
		{ 
			if(TRUE == WdfUsbTargetPipeIsInEndpoint(pipe))
				m_hUsbIntInPipe = pipe;
			else
				m_hUsbIntOutPipe = pipe;
		}
		else if(WdfUsbPipeTypeBulk == pipeInfo.PipeType)
		{
			if(TRUE == WdfUsbTargetPipeIsInEndpoint(pipe))
			{
				m_hUsbBulkInPipe = pipe;
			}
			else if(TRUE == WdfUsbTargetPipeIsOutEndpoint(pipe))
			{
				m_hUsbBulkOutPipe = pipe;
			}
		}

		index++;
	}

	// 通过管道判断固件版本
	if((NULL == m_hUsbIntOutPipe)  ||
		(NULL == m_hUsbIntInPipe)  ||
		(NULL == m_hUsbBulkInPipe) ||
		(NULL == m_hUsbBulkOutPipe))
	{
		KDBG(DPFLTR_ERROR_LEVEL, "Not our CY001 firmware!!!");
	}

	return STATUS_SUCCESS;
}