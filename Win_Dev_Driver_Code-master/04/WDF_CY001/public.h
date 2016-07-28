/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：public.h
	说明：共用头文件
	历史：
	创建：
*/

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#pragma warning(disable:4200)  // nameless struct/union
#pragma warning(disable:4201)  // nameless struct/union
#pragma warning(disable:4115)  // named typedef in parenthesis
#pragma warning(disable:4214)  // bit field types other than int

#include <ntddk.h>
#include <usbdi.h>
#include <wdf.h>
#include <Wdfusb.h>
#include <usbdlib.h>
#include <usbbusif.h>

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short  WORD;
typedef unsigned int	UINT;
#endif 

#include "../common/structure.h"

#define DRIVER_CY001	"CY001: "
#define POOLTAG			'00YC'

#define  MAX_INSTANCE_NUMBER 8
#define  MAX_INTERFACES 8

// WDF设备环境
typedef struct _DEVICE_CONTEXT {
  WDFUSBDEVICE      UsbDevice;
  WDFUSBINTERFACE   UsbInterface;
  WDFUSBINTERFACE   MulInterfaces[MAX_INTERFACES];

  WDFQUEUE          IoRWQueue;
  WDFQUEUE          IoControlEntryQueue;
  WDFQUEUE          IoControlSerialQueue;
  WDFQUEUE          InterruptManualQueue;
  WDFQUEUE			AppSyncManualQueue;
  
  WDFUSBPIPE		UsbCtlPipe;
  WDFUSBPIPE        UsbIntOutPipe;
  WDFUSBPIPE        UsbIntInPipe;
  WDFUSBPIPE        UsbBulkInPipe;
  WDFUSBPIPE        UsbBulkOutPipe;
  BYTE				byPreLEDs;
  BYTE				byPre7Seg;
  
  ULONG				LastUSBErrorStatusValue; // 这个值不断被更新，体现最新的错误值
  USB_BUS_INTERFACE_USBDI_V1 busInterface;
  BOOLEAN			bIsDeviceHighSpeed;
  BOOLEAN			bIntPipeConfigured;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

typedef struct _IRP_ENTRY
{
	LIST_ENTRY  entry;
	PIRP		pIrp;
}IRP_ENTRY, *PIRP_ENTRY;

//===============================函数声明

// 定义析取环境块的函数指针，
// 通过GetDeviceContext能从任一框架对象取得一个指向结构体DEVICE_CONTEXT的指针。
// 使用GetDeviceContext的前提是，目标对象已经定义了一个名称为DEVICE_CONTEXT的环境块。
// 在本工程中，我们在PnpAdd函数中为设备对象定义了DEVICE_CONTEXT环境块。
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

//===============================配置函数
NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject, 
    IN PUNICODE_STRING  RegistryPath
	);

NTSTATUS
PnpAdd(
    IN WDFDRIVER        Driver,
    IN PWDFDEVICE_INIT  DeviceInit
    );

NTSTATUS
PnpPrepareHardware(
    IN WDFDEVICE    Device,
    IN WDFCMRESLIST ResourceList,
    IN WDFCMRESLIST ResourceListTranslated
    );

NTSTATUS
PnpReleaseHardware(
    IN WDFDEVICE    Device,
    IN WDFCMRESLIST ResourceList
    );

NTSTATUS
CreateWDFQueues(
    WDFDEVICE Device,
    PDEVICE_CONTEXT pContext
    );

NTSTATUS
ConfigureUsbDevice(
    WDFDEVICE Device,
    PDEVICE_CONTEXT DeviceContext
    );

NTSTATUS
GetUsbPipes(
    PDEVICE_CONTEXT DeviceContext
    );

//================================读写函数
VOID
InterruptRead(
    WDFUSBPIPE  Pipe,
    WDFMEMORY  Buffer,
    size_t  NumBytesTransferred,
    WDFCONTEXT  pContext
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
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  pContext
    );

VOID
BulkWriteComplete(
    IN WDFREQUEST  Request,
    IN WDFIOTARGET  Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  pContext
    );

VOID
DeviceIoControlParallel(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    );

VOID
DeviceIoControlSerial(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    );

//===================================PNP/Power接口
NTSTATUS 
PwrD0Entry(
	IN WDFDEVICE  Device, 
	IN WDF_POWER_DEVICE_STATE  PreviousState
	);

NTSTATUS 
PwrD0Exit(
	IN WDFDEVICE  Device, 
	IN WDF_POWER_DEVICE_STATE  TargetState
	);

NTSTATUS 
PwrD0ExitPreInterruptsDisabled(
	IN WDFDEVICE  Device, 
	IN WDF_POWER_DEVICE_STATE  TargetState
	);

VOID 
PnpSurpriseRemove(
	IN WDFDEVICE  Device
	);

NTSTATUS
InitPowerManagement(
	IN WDFDEVICE  Device
	);

char* 
PowerName(
	WDF_POWER_DEVICE_STATE PowerState
	);

//==================================公用接口（Util.cpp）
NTSTATUS
FirmwareReset(
	IN WDFDEVICE Device, 
	IN UCHAR resetBit
	);

NTSTATUS
FirmwareUpload(
	WDFDEVICE Device, 
	PUCHAR pData, 
	ULONG ulLen,
	WORD offset
	);

NTSTATUS
ReadRAM(
	WDFDEVICE Device,
	WDFREQUEST request,
	ULONG* pLen
	);

ULONG
GetCurrentFrameNumber(
	IN WDFDEVICE Device
	);

NTSTATUS
AbortPipe(
	IN WDFDEVICE Device, 
	IN ULONG nPipeNum
	);

void 
BulkPipeReadWrite_Ctl(
	IN WDFDEVICE Device, 
	IN WDFREQUEST Request
	);

NTSTATUS 
UsbSetOrClearFeature(
	WDFDEVICE Device,
	WDFREQUEST Request
	);

NTSTATUS
GetStringDes(
	USHORT shIndex,
	USHORT shLanID,
	VOID* pBufferOutput, 
	ULONG OutputBufferLength,
	ULONG* pulRetLen,
	PDEVICE_CONTEXT pContext
	);

NTSTATUS
UsbControlRequest(
	IN WDFDEVICE Device,
	IN WDFREQUEST Request
	);

void 
ControlRequestComplete(
    IN WDFREQUEST  Request,
    IN WDFIOTARGET  Target,
    IN PWDF_REQUEST_COMPLETION_PARAMS  Params,
    IN WDFCONTEXT  Context
    );

void
ClearSyncQueue(
	WDFDEVICE Device
	);

void 
CompleteSyncRequest(
	WDFDEVICE Device, 
	DRIVER_SYNC_ORDER_TYPE type,
	int info
	);

NTSTATUS 
InterruptReadStart(
	WDFDEVICE Device
	);

NTSTATUS
InterruptReadStop(
	WDFDEVICE Device);

NTSTATUS 
SetDigitron(
	IN WDFDEVICE Device, 
	IN UCHAR chSet
	);

NTSTATUS
GetDigitron(
	IN WDFDEVICE Device,
	IN UCHAR* pchGet
	);

NTSTATUS 
SetLEDs(
	IN WDFDEVICE Device,
	IN UCHAR chSet
	);

NTSTATUS 
GetLEDs(
	IN WDFDEVICE Device, 
	IN UCHAR* pchGet
	);

WDFUSBPIPE 
GetInterruptPipe(
	BOOLEAN bInput, 
	WDFDEVICE Device
	);

WDFUSBPIPE
GetBulkPipe(
	BOOLEAN bInput,
	WDFDEVICE Device
	);

__inline KDBG(int nLevel, char* msg, ...);
#endif