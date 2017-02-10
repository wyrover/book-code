#pragma once

#include "ioctrl.h"

#define DRV_POOL_TAG 'Dlek'
#define DRV_LEAK_ALLOCATION_SIZE 1024

VOID
__cdecl
DrvDbgPrint(
    __in_z __drv_formatString(printf) PCSTR pszFormat,
    ...
    );

#if defined(DBG) || defined(DRV_FORCE_KD_PRINT)
    #define DRVPRINT(...) DrvDbgPrint(__VA_ARGS__)
#else
    #define DRVPRINT(...)
#endif

//
// Driver entry/unload
//
__drv_functionClass(DRIVER_INITIALIZE)
__drv_sameIRQL
NTSTATUS
DriverEntry(
    __inout PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    );

__drv_functionClass(DRIVER_UNLOAD)
__drv_requiresIRQL(PASSIVE_LEVEL)
__drv_sameIRQL
VOID
LeakDriverUnload(
   __in PDRIVER_OBJECT DriverObject
   );

NTSTATUS
LeakRoutine(
    __in ULONG Length
    );
