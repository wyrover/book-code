#ifndef _HideDemo_H_
#define _HideDemo_H_

#include "stdafx.h"

#define xx DPFLTR_IHVDRIVER_ID ,0xFFFFFFFF

extern POBJECT_TYPE     *IoDriverObjectType;

typedef struct _FILE_BOTH_DIR_INFORMATION {
    ULONG  NextEntryOffset;
    ULONG  FileIndex;
    LARGE_INTEGER  CreationTime;
    LARGE_INTEGER  LastAccessTime;
    LARGE_INTEGER  LastWriteTime;
    LARGE_INTEGER  ChangeTime;
    LARGE_INTEGER  EndOfFile;
    LARGE_INTEGER  AllocationSize;
    ULONG  FileAttributes;
    ULONG  FileNameLength;
    ULONG  EaSize;
    CCHAR  ShortNameLength;
    WCHAR  ShortName[12];
    WCHAR  FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

typedef struct _CompletePara_ {
    PVOID SelfAddr;
    PVOID CompletionRoutine;
    PVOID Context;
    UCHAR Control;
    PVOID reserve1;
} CompletePara, *PCompletePara;

typedef
NTSTATUS
(_stdcall *fnDispatchRoutine)
(
    IN PDEVICE_OBJECT pDevObj,
    IN PIRP pIrp
);

typedef
NTSTATUS
(_stdcall *fnIoCompletion)
(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  Irp,
    IN PVOID  Context
);


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


void HideDemoUnload(IN PDRIVER_OBJECT DriverObject);
PDRIVER_OBJECT  GetNtfsDrvobj();
NTSTATUS HideDemoFile();
NTSTATUS NewDispatchRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp);

NTSTATUS
NewCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  pIrp,
    IN PVOID  Context
);

ULONG HandleBuffer(PFILE_BOTH_DIR_INFORMATION Info, ULONG InfoSize);

#endif _HideDemo_H_