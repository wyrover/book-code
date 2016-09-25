//驱动开发模板_Win64
//作者：Tesla.Angela(GDUT.HWL)

#include <ntddk.h>
#include "MyDriver.h"
#include "hookssdt.h"

VOID DriverUnload(PDRIVER_OBJECT pDriverObj)
{
    UNICODE_STRING strLink;
    UnhookSSDT();
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
    case IOCTL_ULR3IN: {
        /*memcpy(&ulR3IN,pIoBuffer,sizeof(ulR3IN));
        DbgPrint("LONG From R3: %ld",ulR3IN);*/
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_USR3IN: {
        /*usR3IN=*(PCWSTR *)pIoBuffer;
        RtlInitUnicodeString(&r3us,usR3IN);
        DbgPrint("BSTR From R3: %wZ",&r3us);
        hFileHandle = SkillIoOpenFile(usR3IN,FILE_READ_ATTRIBUTES,FILE_SHARE_DELETE);
        if (hFileHandle!=NULL)
        {
            SKillDeleteFile(hFileHandle);
            ZwClose(hFileHandle);
            DbgPrint("delete file succeed!\n");
        }*/
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_GetKPEB: { //output eprocess
        /*PsLookupProcessByProcessId(ulR3IN,&eProcess);
        memcpy(pIoBuffer,&(ULONG)eProcess,sizeof(ULONG));*/
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_ReInline: {
        /*RestoreInlineHook(L"ObReferenceObjectByHandle");
        DbgPrint("Crear ObReferenceObjectByHandle Head Inline Hook!");*/
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_GetBSTR: {
        /*RtlInitUnicodeString(&US,L"Driver String For Visual Basic: 我爱北京天安门！");
        RtlUnicodeStringToAnsiString(&AS,&US,TRUE);
        strcpy(ctmp,AS.Buffer);
        RtlFreeAnsiString(&AS);
        memcpy(pIoBuffer,ctmp,260);*/
        status = STATUS_SUCCESS;
        break;
    }

    case IOCTL_Struct: {
        /*memcpy(&calctest,pIoBuffer,sizeof(CALC));
        num1=calctest.Number1;num2=calctest.Number2;
        DbgPrint("num1=%d;num2=%d",num1,num2);
        addans=num1+num2;subans=num1-num2;
        DbgPrint("AddAns=%d;SubAns=%d",addans,subans);
        calctest.AddAns=addans;calctest.SubAns=subans;
        memcpy(pIoBuffer,&calctest,sizeof(CALC));*/
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

    KeServiceDescriptorTable = (PSYSTEM_SERVICE_TABLE)GetKeServiceDescriptorTable64();
    HookSSDT();
    return STATUS_SUCCESS;
}