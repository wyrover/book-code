//枚举WIN64上的SSDT
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

typedef struct _UNHOOK_SSDT64 {
    ULONGLONG Address;
    ULONGLONG id;
} UNHOOK_SSDT64, *PUNHOOK_SSDT64;

typedef UINT64(__fastcall *SCFN)(UINT64, UINT64);

SCFN scfn;
ULONGLONG ssdt_base_aadress, ssdt_func_address, KiServiceTable;
ULONG32 ssdt_func_index;
UNHOOK_SSDT64 uhssdt64 = {0};
PSYSTEM_SERVICE_TABLE KeServiceDescriptorTable;

ULONGLONG GetKeServiceDescriptorTable64() //鬼佬的方法
{
    char KiSystemServiceStart_pattern[13] = "\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x25\xFF\x0F\x00\x00"; //睇唔明系么春特征码
    ULONGLONG CodeScanStart = (ULONGLONG)&_strnicmp;
    ULONGLONG CodeScanEnd = (ULONGLONG)&KdDebuggerNotPresent;
    UNICODE_STRING Symbol;
    ULONGLONG i, tbl_address, b;

    for (i = 0; i < CodeScanEnd - CodeScanStart; i++) {
        if (!memcmp((char*)(ULONGLONG)CodeScanStart + i, (char*)KiSystemServiceStart_pattern, 13)) {
            for (b = 0; b < 50; b++) {
                tbl_address = ((ULONGLONG)CodeScanStart + i + b);

                if (*(USHORT*)((ULONGLONG)tbl_address) == (USHORT)0x8d4c)
                    return ((LONGLONG)tbl_address + 7) + *(LONG*)(tbl_address + 3);
            }
        }
    }

    return 0;
}

/*ULONGLONG MyGetKeServiceDescriptorTable64() //我的方法
{
    PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
    PUCHAR i = NULL;
    UCHAR b1=0,b2=0,b3=0;
    ULONG templong=0;
    ULONGLONG addr=0;
    for(i=StartSearchAddress;i<EndSearchAddress;i++)
    {
        if( MmIsAddressValid(i) && MmIsAddressValid(i+1) && MmIsAddressValid(i+2) )
        {
            b1=*i;
            b2=*(i+1);
            b3=*(i+2);
            if( b1==0x4c && b2==0x8d && b3==0x15 ) //4c8d15
            {
                memcpy(&templong,i+3,4);
                addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
                return addr;
            }
        }
    }
    return 0;
}*/

VOID Initxxxx()
{
    UCHAR strShellCode[36] = "\x48\x8B\xC1\x4C\x8D\x12\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x4E\x8B\x14\x17\x4D\x63\x1C\x82\x49\x8B\xC3\x49\xC1\xFB\x04\x4D\x03\xD3\x49\x8B\xC2\xC3";
    /*
    mov rax, rcx ;rcx=index
    lea r10,[rdx] ;rdx=ssdt
    mov edi,eax
    shr edi,7
    and edi,20h
    mov r10, qword ptr [r10+rdi]
    movsxd r11,dword ptr [r10+rax*4]
    mov rax,r11
    sar r11,4
    add r10,r11
    mov rax,r10
    ret
    */
    scfn = ExAllocatePool(NonPagedPool, 36);
    memcpy(scfn, strShellCode, 36);
}

KIRQL WPOFFx64()
{
    KIRQL irql = KeRaiseIrqlToDpcLevel();
    UINT64 cr0 = __readcr0();
    cr0 &= 0xfffffffffffeffff;
    __writecr0(cr0);
    _disable();
    return irql;
}

void WPONx64(KIRQL irql)
{
    UINT64 cr0 = __readcr0();
    cr0 |= 0x10000;
    _enable();
    __writecr0(cr0);
    KeLowerIrql(irql);
}

ULONG GetOffsetAddress(ULONGLONG FuncAddr)
{
    ULONG dwtmp = 0;
    PULONG ServiceTableBase = NULL;

    if (KeServiceDescriptorTable == NULL)
        KeServiceDescriptorTable = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTable64();

    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    dwtmp = (ULONG)(FuncAddr - (ULONGLONG)ServiceTableBase);
    return dwtmp << 4;
}

VOID UnHookSSDT(ULONG id, ULONGLONG FuncAddr)
{
    KIRQL irql;
    ULONG dwtmp;
    PULONG ServiceTableBase = NULL;
    dwtmp = GetOffsetAddress(FuncAddr);
    ServiceTableBase = (PULONG)KeServiceDescriptorTable->ServiceTableBase;
    irql = WPOFFx64();
    ServiceTableBase[id] = dwtmp;
    WPONx64(irql);
}

ULONGLONG GetSSDTFunctionAddress64(ULONGLONG NtApiIndex)
{
    ULONGLONG ret = 0;

    if (ssdt_base_aadress == 0)
        ssdt_base_aadress = GetKeServiceDescriptorTable64();

    if (scfn == NULL)
        Initxxxx();

    ret = scfn(NtApiIndex, ssdt_base_aadress);
    return ret;
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
    case IOCTL_ClrSSDTHOOK: {
        memcpy(&uhssdt64, pIoBuffer, sizeof(UNHOOK_SSDT64));
        UnHookSSDT((ULONG)uhssdt64.id, uhssdt64.Address);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_GetKiSrvTab: {
        if (ssdt_base_aadress == 0)
            ssdt_base_aadress = GetKeServiceDescriptorTable64();

        KiServiceTable = *(PULONGLONG)ssdt_base_aadress;
        memcpy(pIoBuffer, &KiServiceTable, 8);
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_GetFuncAddr: {
        memcpy(&ssdt_func_index, pIoBuffer, 4);
        ssdt_func_address = GetSSDTFunctionAddress64((ULONGLONG)ssdt_func_index);
        memcpy(pIoBuffer, &ssdt_func_address, 8);
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

    return STATUS_SUCCESS;
}