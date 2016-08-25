#pragma once

#include <initguid.h>
#include <wdm.h>
#include <ntddsd.h>
#include "guid.h"

typedef struct _SDIOGLOBALS 
{
	UNICODE_STRING Sdio_RegistryPath;
}SDIOGLOBALS;

typedef enum _DEVSTATE 
{
	NotStarted, // not started
	Stopped, // device stopped
	Working, // started and working
	PendingStop, // stop pending
	PendingRemove, // remove pending
	SurpriseRemoved, // removed by surprise
	Removed // removed
}DEVSTATE;


//
// A structure representing the instance information associated with
// this particular device.
//

typedef struct _DEVICE_EXTENSION {

// Functional Device Object
PDEVICE_OBJECT FunctionalDeviceObject;

// Device object we call when submitting Urbs
PDEVICE_OBJECT TopOfStackDeviceObject;

// The bus driver object
PDEVICE_OBJECT PhysicalDeviceObject;

// Name buffer for our named Functional device object link
// The name is generated based on the driver's class GUID
UNICODE_STRING InterfaceName;

//structure holds pointers to the SD bus interface routines
SDBUS_INTERFACE_STANDARD InterfaceStandard;

PIRP PendingIdleIrp;

LONG IdleReqPend;

LONG FreeIdleIrpCount;

KSPIN_LOCK IdleReqStateLock;

KEVENT NoIdleReqPendEvent;

// default power state to power down to on self-susped
ULONG PowerDownLevel;

// remote wakeup variables
PIRP WaitWakeIrp;

LONG FlagWWCancel;

LONG FlagWWOutstanding;

LONG WaitWakeEnable;

// open handle count
LONG OpenHandleCount;

KDPC DeferredProcCall;

// This event is cleared when a DPC/Work Item is queued.
// and signaled when the work-item completes.
// This is essential to prevent the driver from unloading
// while we have DPC or work-item queued up.
KEVENT NoDpcWorkItemPendingEvent;

    //
    // Driver version
    //
    USHORT DriverVersion;
    //
    // Function number on SD card
    //
    UCHAR FunctionNumber;

    //
    // Send data transactions in byte mode (0) or block mode (1)
    //
    UCHAR BlockMode;

	ULONG Channel0_Length;
    

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


typedef struct _IRP_COMPLETION_CONTEXT {

PDEVICE_EXTENSION DeviceExtension;

PKEVENT Event;

} IRP_COMPLETION_CONTEXT, *PIRP_COMPLETION_CONTEXT;

#define arraysize(p) (sizeof(p)/sizeof((p)[0]))


// SDIO registers
#define REG_CCCR_IENx					0x04
#define REG_CCCR_BUS_INTERFACE_CTRL		0x07
#define REG_CCCR_CARD_CAP				0x08

//IFDevice SDIO registers
#define IFDevice_INT0_STATUS	0x00
#define IFDevice_INT0_ENABLE	0x03
#define IFDevice_SDIO_CMD_REG	0x14
#define IFDevice_SDIO_DATA_REG	0x19

//IFDevice Intr Status Mask
#define IFDevice_INT0_STATUS_MASK  0x07

/////////////////SDIO IFDevice Reg//////////////////

#define INT_STATUS_REG	 		0x00   //Read MMIS_INT_STATUS register and clear it
#define AP_CMD_REG			0x14
