//驱动开发模板_Win64
//作者：Tesla.Angela(GDUT.HWL)

#include <ntddk.h>
#include <windef.h>
#include "MyDriver.h"

#pragma intrinsic(__readmsr)

typedef struct _SYSTEM_SERVICE_TABLE {
    PVOID       ServiceTableBase;
    PVOID       ServiceCounterTableBase;
    ULONGLONG   NumberOfServices;
    PVOID       ParamTableBase;
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE {
    SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
    SYSTEM_SERVICE_TABLE win32k;    // win32k.sys   (gdi/user)
    SYSTEM_SERVICE_TABLE Table3;    // not used
    SYSTEM_SERVICE_TABLE Table4;    // not used
} SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;

PSYSTEM_SERVICE_TABLE KeServiceDescriptorTableShadow = 0;
ULONG64 ul64W32pServiceTable = 0;

ULONGLONG GetKeServiceDescriptorTableShadow64()
{
    PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
    PUCHAR i = NULL;
    UCHAR b1 = 0, b2 = 0, b3 = 0;
    ULONG templong = 0;
    ULONGLONG addr = 0;

    for (i = StartSearchAddress; i < EndSearchAddress; i++) {
        if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2)) {
            b1 = *i;
            b2 = *(i + 1);
            b3 = *(i + 2);

            if (b1 == 0x4c && b2 == 0x8d && b3 == 0x1d) { //4c8d1d
                memcpy(&templong, i + 3, 4);
                addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
                return addr;
            }
        }
    }

    return 0;
}

//在WINDBG里查看：
//ln win32k!W32pServiceTable+((poi(win32k!W32pServiceTable+4*(<syscall nbr>-1000))&0x00000000`ffffffff)>>4)-10000000
//u win32k!W32pServiceTable+((poi(win32k!W32pServiceTable+4*(Index-0x1000))&0x00000000`ffffffff)>>4)-0x10000000
//u poi(win32k!W32pServiceTable+4*(1-0x1000))
//u poi(win32k!W32pServiceTable+4*(1-0x1000))&0x00000000`ffffffff
//u (poi(win32k!W32pServiceTable+4*(1-0x1000))&0x00000000`ffffffff)>>4
//u win32k!W32pServiceTable+((poi(win32k!W32pServiceTable+4*(1-0x1000))&0x00000000`ffffffff)>>4)-0x10000000
ULONGLONG GetSSSDTFuncCurAddr64(ULONG64 Index)
{
    ULONGLONG               W32pServiceTable = 0, qwTemp = 0;
    LONG                    dwTemp = 0;
    PSYSTEM_SERVICE_TABLE   pWin32k;
    pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + sizeof(SYSTEM_SERVICE_TABLE));  //sizeof(SYSTEM_SERVICE_TABLE)
    W32pServiceTable = (ULONGLONG)(pWin32k->ServiceTableBase);
    ul64W32pServiceTable = W32pServiceTable;
    //DbgPrint("W32pServiceTable: %llx",W32pServiceTable);
    //DbgPrint("Service Count: %lld",pWin32k->NumberOfServices);
    qwTemp = W32pServiceTable + 4 * (Index - 0x1000); //这里是获得偏移地址的位置，要HOOK的话修改这里即可
    dwTemp = *(PLONG)qwTemp;
    dwTemp = dwTemp >> 4;
    qwTemp = W32pServiceTable + (LONG64)dwTemp;
    return qwTemp;
}

VOID ListAllShadowFunctionAddress()
{
    ULONGLONG               W32pServiceTable = 0, FunctionCount = 0, i = 0, Index = 0;
    ULONGLONG               qwTemp = 0;
    LONG                    dwTemp;
    PSYSTEM_SERVICE_TABLE   pWin32k;
    pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)KeServiceDescriptorTableShadow + 4 * 8);
    W32pServiceTable = (ULONGLONG)(pWin32k->ServiceTableBase);
    FunctionCount = pWin32k->NumberOfServices;

    for (i = 0; i < FunctionCount; i++) {
        Index = 0x1000 + i;
        qwTemp = W32pServiceTable + 4 * (Index - 0x1000); //DbgPrint("qwTemp: %llx",qwTemp);
        dwTemp = *(PLONG)qwTemp;
        dwTemp = dwTemp >> 4;
        qwTemp = W32pServiceTable + (LONG64)dwTemp;
        DbgPrint("Address: %llx", qwTemp);
    }
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;
    RtlInitUnicodeString(&strLink, LINK_NAME);
    IoDeleteSymbolicLink(&strLink);
    IoDeleteDevice(pDriverObj->DeviceObject);
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    pIrp->IoStatus.Status = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
    NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
    PIO_STACK_LOCATION pIrpStack;
    ULONG uIoControlCode;
    PVOID pIoBuffer;
    ULONG uInSize;
    ULONG uOutSize;
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
    pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
    uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
    uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

    switch (uIoControlCode) {
    case IOCTL_GET_W32PSRVT: {
        GetSSSDTFuncCurAddr64(0x1001);
        memcpy(pIoBuffer, &ul64W32pServiceTable, 8);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_GET_3SDTADDR: {
        //DbgPrint("NtUserPeekMessage: %llx",GetSSSDTFuncCurAddr64(0x1001));    //NtUserPeekMessage
        //ListAllShadowFunctionAddress();
        ULONG64 qw = 0;
        memcpy(&qw, pIoBuffer, 8);
        qw = GetSSSDTFuncCurAddr64(qw);
        memcpy(pIoBuffer, &qw, 8);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_UNHOOK_SSSDT: {
        status = STATUS_SUCCESS;
        break;
    }
    }

    if (status == STATUS_SUCCESS)
        pIrp->IoStatus.Information = uOutSize;
    else
        pIrp->IoStatus.Information = 0;

    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    NTSTATUS status = STATUS_SUCCESS;
    UNICODE_STRING ustrLinkName;
    UNICODE_STRING ustrDevName;
    PDEVICE_OBJECT pDevObj;
    pDriverObj->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
    pDriverObj->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
    pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
    pDriverObj->DriverUnload = DriverUnload;
    RtlInitUnicodeString(&ustrDevName, DEVICE_NAME);
    status = IoCreateDevice(pDriverObj, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);

    if (!NT_SUCCESS(status)) return status;

    if (IoIsWdmVersionAvailable(1, 0x10))
        RtlInitUnicodeString(&ustrLinkName, LINK_GLOBAL_NAME);
    else
        RtlInitUnicodeString(&ustrLinkName, LINK_NAME);

    status = IoCreateSymbolicLink(&ustrLinkName, &ustrDevName);

    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(pDevObj);
        return status;
    }

    //test
    KeServiceDescriptorTableShadow = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTableShadow64();
    //DbgPrint("SSSDT: %llx[TA's method]",(ULONG64)KeServiceDescriptorTableShadow);
    //test
    return STATUS_SUCCESS;
}