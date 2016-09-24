/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.


Module Name:

   Event.h

--*/

#ifndef __EVENT__
#define __EVENT__

//
// DEFINES
//

#define NTDEVICE_NAME_STRING      L"\\Device\\V-ASIO"
#define SYMBOLIC_NAME_STRING      L"\\DosDevices\\V-ASIO"
#define TAG (ULONG)'TEVE'

#if DBG
#define DebugPrint(_x_) \
                DbgPrint("EVENT.SYS: ");\
                DbgPrint _x_;

#else

#define DebugPrint(_x_)

#endif

#define ASIO_MAX_CHANNELS    20
#define MAX_IN_CHANNELS     20
#define MAX_OUT_CHANNELS    20
#define ASIO_MAX_BUFFERS    2

typedef struct {
    PVOID TotalInChannelBuffer[MAX_IN_CHANNELS][ASIO_MAX_BUFFERS];
}TOTALINPOINTERSTRUCT, *TOTALPINPOINTERSTRUCT;

typedef struct {
    PVOID TotalOutChannelBuffer[MAX_OUT_CHANNELS][ASIO_MAX_BUFFERS];
}TOTALOUTPOINTERSTRUCT, *TOTALPOUTPOINTERSTRUCT;

typedef struct {
    PVOID InChannelBuffer[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
    PVOID OutChannelBuffer[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
    ULONG BufferLength;
}BUFPOINTERSTRUCT, *PBUFPOINTERSTRUCT;

//
// DATA
//
typedef struct _DEVICE_EXTENSION {
    PDEVICE_OBJECT  Self;
    LIST_ENTRY      EventQueueHead; // where all the user notification requests are queued
    KSPIN_LOCK      QueueLock;
    PKEVENT            hEvent[2];

    PMDL            ppOutputBufs_MDL[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
    PMDL            ppInputBufs_MDL[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
    PUCHAR            ppOutputBufs[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];
    PUCHAR            ppInputBufs[ASIO_MAX_CHANNELS][ASIO_MAX_BUFFERS];

    BOOLEAN            bStart;
    ULONG            ulBufSize;
    ULONG            ulLatency;
    ULONG            ulSampleRate;

    int                nBuffer;

    ULONG            ulTicks;    // tick¼ÆÊý
    LARGE_INTEGER    startTime;
    LONGLONG        timerInterval;
    BOOLEAN            bStoppingDPC;

    KEVENT            StopDPCEvent;
    KTIMER            Timer;
    KDPC            TimerDpc;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


typedef struct _FILE_CONTEXT
{
   IO_REMOVE_LOCK  FileRundownLock;
} FILE_CONTEXT, *PFILE_CONTEXT;


//
// Function prototypes
//
typedef VOID KDEFERRED_ROUTINE (
                   __in struct _KDPC *Dpc,
                   __in_opt PVOID DeferredContext,
                   __in_opt PVOID SystemArgument1,
                   __in_opt PVOID SystemArgument2
                   );

DRIVER_INITIALIZE DriverEntry;

__drv_dispatchType(IRP_MJ_CREATE)
__drv_dispatchType(IRP_MJ_CLOSE)
DRIVER_DISPATCH DispatchCreateClose;

__drv_dispatchType(IRP_MJ_CLEANUP)
DRIVER_DISPATCH DispatchCleanup;

__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH DispatchDispatchIoControl;

DRIVER_UNLOAD DrvUnload;
DRIVER_CANCEL DispatchCancelRoutine;
KDEFERRED_ROUTINE CustomTimerDPC;

void TimerDpc (KDPC* pDPC, void* par1, void*, void*);
void StopDPC( IN DEVICE_OBJECT* Device );

NTSTATUS AsioStart(DEVICE_OBJECT* Device);
NTSTATUS AsioStop(DEVICE_OBJECT* Device);

NTSTATUS AsioSetEvent(DEVICE_OBJECT* Device, HANDLE* hEvent);
void     AsioReleaseEvent(DEVICE_OBJECT* Device);
NTSTATUS AsioSetBuffers(DEVICE_OBJECT* Device, BUFPOINTERSTRUCT *Buffers);
void     AsioReleaseBuffers(DEVICE_OBJECT* Device);
NTSTATUS AsioBufReady(DEVICE_OBJECT* DeviceObject, ULONG lpInBuf);

#endif // __EVENT__

