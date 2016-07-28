/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：ReadWrite.c
	说明：USB设备读写
	历史：
	创建：
*/

#include "Cy001Drv.h"

VOID CY001Drv::InterruptRead_sta(
				  WDFUSBPIPE  Pipe,
				  WDFMEMORY  Buffer,
				  size_t  NumBytesTransferred,
				  WDFCONTEXT  pContext
				  )
{
	CY001Drv *pThis = (CY001Drv*)pContext;
	pThis->InterruptRead(Buffer, NumBytesTransferred);
}

VOID CY001Drv::BulkRead_sta(
			 IN WDFQUEUE  Queue,
			 IN WDFREQUEST  Request,
			 IN size_t  Length
			 )
{
	CY001Drv* pThis = (CY001Drv*)CY001Drv::GetDrvClassFromDevice(WdfIoQueueGetDevice(Queue));
	pThis->BulkRead(Queue, Request, Length);
}

VOID CY001Drv::BulkWrite_sta(
			  IN WDFQUEUE  Queue,
			  IN WDFREQUEST  Request,
			  IN size_t  Length
			  )
{
	CY001Drv* pThis = (CY001Drv*)CY001Drv::GetDrvClassFromDevice(WdfIoQueueGetDevice(Queue));
	pThis->BulkWrite(Queue, Request, Length);
}

VOID CY001Drv::BulkReadComplete_sta(
					 IN WDFREQUEST  Request,
					 IN WDFIOTARGET  Target,
					 IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
					 IN WDFCONTEXT  pContext
					 )
{
	CY001Drv *pThis = (CY001Drv*)pContext;
	pThis->BulkReadComplete(Request, Target, Params);
}

VOID CY001Drv::BulkWriteComplete_sta(
					  IN WDFREQUEST  Request,
					  IN WDFIOTARGET  Target,
					  IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
					  IN WDFCONTEXT  pContext
					  )
{
	CY001Drv *pThis = (CY001Drv*)pContext;
	pThis->BulkWriteComplete(Request, Target, Params);
}

// 中断Pipe回调函数。这样一旦设备产生了中断信息，驱动就能够读取到。
//
VOID CY001Drv::InterruptRead(WDFMEMORY Buffer, size_t NumBytesTransferred)
{
	NTSTATUS status;
	size_t size = 0;
	WDFREQUEST Request = NULL;
	CHAR *pchBuf = NULL;

	KDBG(DPFLTR_INFO_LEVEL, "[InterruptRead]");

	// Read数据缓冲区。注意到，缓冲区长度总是管道最大包长度的倍数。
	// 我们这里只用缓冲区的第一个有效字节。
	pchBuf = (CHAR*)WdfMemoryGetBuffer(Buffer, &size);
	if(pchBuf == NULL || size == 0)
		return;

	// 第一个字节为确认字节，一定是0xD4
	//if(pchBuf[0] != 0xD4)return;

	// 从队列中提取一个未完成请求
	status = WdfIoQueueRetrieveNextRequest(m_hInterruptManualQueue, &Request);

	if(NT_SUCCESS(status))
	{
		CHAR* pOutputBuffer = NULL;
		status = WdfRequestRetrieveOutputBuffer(Request, 1, (PVOID*)&pOutputBuffer, NULL);

		if(NT_SUCCESS(status))
		{
			// 把结果返回给应用程序
			pOutputBuffer[0] = pchBuf[1];
			WdfRequestCompleteWithInformation(Request, status, 1);
		}
		else
		{
			// 返回错误
			WdfRequestComplete(Request, status);
		}

		KDBG(DPFLTR_INFO_LEVEL, "Get and finish an interrupt read request.");
	}else{
		// 队列空，将放弃从设备获取的数据。
		KDBG(DPFLTR_INFO_LEVEL, "Manual interrupt queue is empty!!!");
	}
}

// Bulk管道写操作
//
VOID CY001Drv::BulkWrite(IN WDFQUEUE  Queue, IN WDFREQUEST  Request, IN size_t  Length)
{
	NTSTATUS status = STATUS_SUCCESS;
	WDFMEMORY hMem = NULL;
	WDFDEVICE hDevice = NULL;
	UCHAR* lpBuf;

	UNREFERENCED_PARAMETER(Length);

	KDBG(DPFLTR_INFO_LEVEL, "[BulkWrite] size: %d", Length);

	// 获取管道句柄
	hDevice = WdfIoQueueGetDevice(Queue);

	if(NULL == m_hUsbBulkOutPipe)
	{
		KDBG(DPFLTR_ERROR_LEVEL, "BulkOutputPipe = NULL");
		WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
		return;
	}

	status = WdfRequestRetrieveInputMemory(Request, &hMem);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_ERROR_LEVEL, "WdfRequestRetrieveInputMemory failed(status = 0x%0.8x)!!!", status);
		WdfRequestComplete(Request, status);
		return;
	}

	// 打印出offset值。
	// 在写缓冲的前两个字节中存有write offset的值
	lpBuf = (UCHAR*)WdfMemoryGetBuffer(hMem, 0);
	KDBG(DPFLTR_TRACE_LEVEL, "write offset: %hd", *(WORD*)lpBuf);

	// 把当前的Request对象进行重利用，发送给USB总线。
	// 格式化Request对象，设置Pipe句柄、完成函数等。
	status = WdfUsbTargetPipeFormatRequestForWrite(m_hUsbBulkOutPipe, Request, hMem, NULL);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_ERROR_LEVEL, "WdfUsbTargetPipeFormatRequestForWrite(status 0x%0.8x)!!!", status);
		WdfRequestComplete(Request, status);
		return;
	}

	WdfRequestSetCompletionRoutine(Request, BulkWriteComplete_sta, this); 
	if(FALSE == WdfRequestSend(Request, WdfUsbTargetPipeGetIoTarget(m_hUsbBulkOutPipe), NULL))
	{
		status = WdfRequestGetStatus(Request);
		KDBG(DPFLTR_ERROR_LEVEL, "WdfRequestSend failed with status 0x%0.8x\n", status);		
		WdfRequestComplete(Request, status);
	}
}

// Bulk管道写操作的完成函数
//
VOID CY001Drv::BulkWriteComplete(IN WDFREQUEST  Request, IN WDFIOTARGET  Target, 
						IN PWDF_REQUEST_COMPLETION_PARAMS  Params)
{
	PWDF_USB_REQUEST_COMPLETION_PARAMS usbCompletionParams;
	NTSTATUS ntStatus;
	ULONG_PTR ulLen;
	LONG* lpBuf;

	KDBG(DPFLTR_INFO_LEVEL, "[BulkWriteComplete]");

	usbCompletionParams = Params->Parameters.Usb.Completion;
	ntStatus = Params->IoStatus.Status;
	ulLen = usbCompletionParams->Parameters.PipeWrite.Length;
	lpBuf = (LONG*)WdfMemoryGetBuffer(usbCompletionParams->Parameters.PipeWrite.Buffer, NULL);

	if(NT_SUCCESS(ntStatus))
		KDBG(DPFLTR_INFO_LEVEL, "%d bytes written to USB device successfully.", ulLen);
	else
		KDBG(DPFLTR_INFO_LEVEL, "Failed to write: 0x%08x!!!", ntStatus);

	// 完成请求。
	// 应用程序将收到通知。
	WdfRequestCompleteWithInformation(Request, ntStatus, ulLen);
}

// Bulk管道读操作
//
VOID CY001Drv::BulkRead(IN WDFQUEUE  Queue, IN WDFREQUEST  Request, IN size_t  Length)
{
	NTSTATUS status = STATUS_SUCCESS;
	WDFMEMORY hMem;

	KDBG(DPFLTR_INFO_LEVEL, "[BulkRead] size: %d", Length);

	if(NULL == m_hUsbBulkInPipe){
		WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
		return;
	}

	status = WdfRequestRetrieveOutputMemory(Request, &hMem);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfRequestRetrieveOutputMemory failed with status 0x%0.8x!!!", status);
		WdfRequestComplete(Request, status);
		return;
	}

	// 和写一样，仍然是对Request对象的重利用
	// 除了重利用外，也可以新建一个Request对象。新建的方法在本工程其他地方用得较多。
	status = WdfUsbTargetPipeFormatRequestForRead(m_hUsbBulkInPipe, Request, hMem, NULL);
	if(!NT_SUCCESS(status))
	{
		KDBG(DPFLTR_INFO_LEVEL, "WdfUsbTargetPipeFormatRequestForRead failed with status 0x%08x\n", status);
		WdfRequestComplete(Request, status);
		return;
	}

	WdfRequestSetCompletionRoutine(Request, BulkReadComplete_sta, this);
	if(FALSE == WdfRequestSend(Request, WdfUsbTargetPipeGetIoTarget(m_hUsbBulkInPipe), NULL))
	{
		status = WdfRequestGetStatus(Request);
		KDBG(DPFLTR_INFO_LEVEL, "WdfRequestSend failed with status 0x%08x\n", status);
		WdfRequestComplete(Request, status);
	}
}

// Bulk管道读操作的完成函数
//
VOID CY001Drv::BulkReadComplete(IN WDFREQUEST  Request, IN WDFIOTARGET  Target,
					   IN PWDF_REQUEST_COMPLETION_PARAMS  Params)
{
	PWDF_USB_REQUEST_COMPLETION_PARAMS usbCompletionParams;
	NTSTATUS ntStatus;
	ULONG_PTR ulLen;
	LONG* lpBuf;

	KDBG(DPFLTR_INFO_LEVEL, "[BulkReadComplete]");

	usbCompletionParams = Params->Parameters.Usb.Completion;
	ntStatus = Params->IoStatus.Status;
	ulLen = usbCompletionParams->Parameters.PipeRead.Length;
	lpBuf = (LONG*)WdfMemoryGetBuffer(usbCompletionParams->Parameters.PipeRead.Buffer, NULL);

	if(NT_SUCCESS(ntStatus))
		KDBG(DPFLTR_INFO_LEVEL, "%d bytes readen from USB device successfully.", ulLen);
	else
		KDBG(DPFLTR_INFO_LEVEL, "Failed to read: 0x%08x!!!", ntStatus);

	// 完成操作
	// 应用程序将收到通知。
	WdfRequestCompleteWithInformation(Request, ntStatus, ulLen);
}
