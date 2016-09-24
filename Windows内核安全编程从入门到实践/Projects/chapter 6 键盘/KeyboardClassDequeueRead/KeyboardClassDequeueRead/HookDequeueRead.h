#pragma once

extern ULONG uHookedPoint;
extern ULONG uOrgDequeueRead ;
extern ULONG OffsetOrgDequeueRead;
extern ULONG uOrgCompleteRoutine ;

#define IRPOrgCompleteRoutinueNULL 1

typedef PIRP(_stdcall *fnKeyBoardClassDequeueRead)(PVOID pDevExt);

typedef NTSTATUS(_stdcall *fnCompleteRoutine)(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  pIrp,
    IN PVOID  Context
);


typedef struct _KEYBOARD_INPUT_DATA {
    USHORT  UnitID;
    USHORT  MakeCode;
    USHORT  Flags;
    USHORT  Reserved;
    USHORT  ExtraInformation;
} KEYBOARD_I_DATA, *PKEYBOARD_INPUT_DATA;


NTSTATUS
ObReferenceObjectByName(
    __in PUNICODE_STRING ObjectName,
    __in ULONG Attributes,
    __in_opt PACCESS_STATE AccessState,
    __in_opt ACCESS_MASK DesiredAccess,
    __in POBJECT_TYPE ObjectType,
    __in KPROCESSOR_MODE AccessMode,
    __inout_opt PVOID ParseContext,
    __out PVOID *Object
);
extern POBJECT_TYPE     *IoDriverObjectType;



BOOLEAN
HookDequeueRead();

PVOID
GetKbdClassServiceCallBack();

PDRIVER_OBJECT
GetDriverObject(WCHAR *wDrvName);

ULONG
CalcFunctionLength(ULONG uFnStart);

PIRP
NewDequeueRead(PVOID pDevExt);

NTSTATUS
NewCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  pIrp,
    IN PVOID  Context
);