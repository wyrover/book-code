
#pragma once

#include "DrvClass.h"

class CY001Drv: public DrvClass
{
	WDFQUEUE          m_hIoRWQueue;
	WDFQUEUE          m_hIoCtlEntryQueue;
	WDFQUEUE          m_hIoCtlSerialQueue;
	WDFQUEUE          m_hInterruptManualQueue;
	WDFQUEUE		  m_hAppSyncManualQueue;

	WDFUSBPIPE		  m_hUsbCtlPipe;
	WDFUSBPIPE        m_hUsbIntOutPipe;
	WDFUSBPIPE        m_hUsbIntInPipe;
	WDFUSBPIPE        m_hUsbBulkInPipe;
	WDFUSBPIPE        m_hUsbBulkOutPipe;
	BYTE			  byPreLEDs;
	BYTE			  byPre7Seg;

	ULONG			  m_ulLastUSBErrorStatusValue; // 这个值不断被更新，体现最新的错误值
	BOOLEAN			  m_bIntPipeConfigured;

public:	
	CY001Drv(){m_nSize = sizeof(CY001Drv);KdPrint(("CY001Drv"));}
	int GetSize(){return m_nSize;}
	
	virtual VOID 
		PnpSurpriseRemove();

	virtual NTSTATUS 
		PwrD0Entry(
		IN WDF_POWER_DEVICE_STATE  PreviousState
		);

	virtual NTSTATUS 
		PwrD0Exit(
		IN WDF_POWER_DEVICE_STATE  TargetState
		);

private:
	virtual NTSTATUS
		CreateWDFQueues();

	virtual NTSTATUS
		GetUsbPipes();

private:
	//=============IOCTL接口（Util.cpp）
	// 并行处理
	static VOID DeviceIoControlParallel_sta(IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  OutputBufferLength,
		IN size_t  InputBufferLength,
		IN ULONG  IoControlCode);
	
	VOID DeviceIoControlParallel(IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  OutputBufferLength,
		IN size_t  InputBufferLength,
		IN ULONG  IoControlCode);

	// 序列化处理
	static VOID DeviceIoControlSerial_sta(IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  OutputBufferLength,
		IN size_t  InputBufferLength,
		IN ULONG  IoControlCode);

	VOID DeviceIoControlSerial(IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  OutputBufferLength,
		IN size_t  InputBufferLength,
		IN ULONG  IoControlCode);

	NTSTATUS
		FirmwareReset(
		IN UCHAR resetBit
		);

	NTSTATUS
		FirmwareUpload(
		PUCHAR pData, 
		ULONG ulLen,
		WORD offset
		);

	NTSTATUS
		ReadRAM(
		WDFREQUEST request,
		ULONG* pLen
		);

	ULONG
		GetCurrentFrameNumber();

	NTSTATUS
		AbortPipe(
		IN ULONG nPipeNum
		);

	void 
		BulkPipeReadWrite_Ctl(
		IN WDFREQUEST Request
		);

	NTSTATUS 
		UsbSetOrClearFeature(
		WDFREQUEST Request
		);

	NTSTATUS
		GetStringDes(
		USHORT shIndex,
		USHORT shLanID,
		VOID*  pBufferOutput, 
		ULONG  OutputBufferLength,
		ULONG* pulRetLen
		);

	NTSTATUS
		UsbControlRequest(
		IN WDFREQUEST Request
		);

	void
		ClearSyncQueue();

	void 
		CompleteSyncRequest(
		DRIVER_SYNC_ORDER_TYPE type,
		int info
		);

	NTSTATUS 
		GetOneSyncRequest(
		WDFREQUEST* pRequest
		);

	NTSTATUS 
		InterruptReadStart();

	NTSTATUS
		InterruptReadStop();

	NTSTATUS 
		SetDigitron(
		IN UCHAR chSet
		);

	NTSTATUS
		GetDigitron(
		IN UCHAR* pchGet
		);

	NTSTATUS 
		SetLEDs(
		IN UCHAR chSet
		);

	NTSTATUS 
		GetLEDs(
		IN UCHAR* pchGet
		);


	//=================读写函数（ReadWrite.cpp）
	static VOID
		InterruptRead_sta(
		WDFUSBPIPE  Pipe,
		WDFMEMORY  Buffer,
		size_t  NumBytesTransferred,
		WDFCONTEXT  pContext
		);

	static VOID
		BulkRead_sta(
		IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  Length
		);

	static VOID
		BulkWrite_sta(
		IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  Length
		);

	static VOID
		BulkReadComplete_sta(
		IN WDFREQUEST  Request,
		IN WDFIOTARGET  Target,
		IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
		IN WDFCONTEXT  pContext
		);

	static VOID
		BulkWriteComplete_sta(
		IN WDFREQUEST  Request,
		IN WDFIOTARGET  Target,
		IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
		IN WDFCONTEXT  pContext
		);

	VOID
		InterruptRead(
		WDFMEMORY  Buffer,
		size_t  NumBytesTransferred
		);

	VOID
		BulkRead(
		IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  Length
		);

	VOID
		BulkWrite(
		IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request,
		IN size_t  Length
		);

	VOID
		BulkReadComplete(
		IN WDFREQUEST  Request,
		IN WDFIOTARGET  Target,
		IN PWDF_REQUEST_COMPLETION_PARAMS  Params
		);

	VOID
		BulkWriteComplete(
		IN WDFREQUEST  Request,
		IN WDFIOTARGET  Target,
		IN PWDF_REQUEST_COMPLETION_PARAMS  Params
		);
};