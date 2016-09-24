#include "stdafx.h"

ULONG uHookedPoint = 0;
ULONG uOrgDequeueRead = 0;
ULONG OffsetOrgDequeueRead = 0;
ULONG uOrgCompleteRoutine = 0;

BOOLEAN
HookDequeueRead()
{
    ULONG   uKbdClassServiceCallBackAddr = 0, uFnLen = 0;
    PUCHAR  pStart = 0;
    ULONG OffsetNewDequeueRead;
    uKbdClassServiceCallBackAddr = (ULONG)GetKbdClassServiceCallBack();

    if (uKbdClassServiceCallBackAddr == NULL) {
        KdPrintEx((xx, "[HookDequeueRead] uKbdClassServiceCallBackAddr NULL!\n"));
        return FALSE;
    }

    KdPrintEx((xx, "[HookDequeueRead] uKbdClassServiceCallBackAddr: 0x%08x\n",
               uKbdClassServiceCallBackAddr));
    uFnLen = CalcFunctionLength(uKbdClassServiceCallBackAddr);

    if (uFnLen == 0) {
        KdPrintEx((xx, "[HookDequeueRead] CalcFunctionLength Failure!\n"));
        return FALSE;
    }

    KdPrintEx((xx, "[HookDequeueRead] Function len-KeyBoardClassServiceCallBackAddr: %d\n",
               uFnLen));

    for (
        pStart = (PUCHAR)uKbdClassServiceCallBackAddr;
        pStart < (PUCHAR)(uKbdClassServiceCallBackAddr + uFnLen - 4);
        ++pStart
    ) {
        if ((*pStart == 0xe8) && (*(pStart - 7) == 0x56) && (*(pStart - 6) == 0x89) && (*(pStart - 3) == 0x89)) {
            uHookedPoint = (ULONG)pStart;
            KdPrintEx((xx, "[HookDequeueRead] Find it!uHookedPoint: 0x%08x\n",
                       uHookedPoint));
            break;
        }
    }

    if (uHookedPoint == 0) {
        return FALSE;
    }

    uOrgDequeueRead = uHookedPoint + *(ULONG*)(uHookedPoint + 1) + 5;
    OffsetOrgDequeueRead = *(ULONG*)(uHookedPoint + 1);
    OffsetNewDequeueRead = (ULONG)NewDequeueRead - uHookedPoint - 5;
    __asm {
        cli
        push   eax
        mov    eax, cr0
        and    eax, not 10000h
        mov    cr0, eax
        pop    eax
    }
    InterlockedExchange((LONG*)(uHookedPoint+1), (LONG)OffsetNewDequeueRead);
    __asm {
        push   eax
        mov    eax, cr0
        or     eax, 10000h
        mov    cr0, eax
        pop    eax
        sti
    }
    return TRUE;
}


PVOID
GetKbdClassServiceCallBack()
{
    PDRIVER_OBJECT  pI8042prtDrvObj = NULL;
    PDEVICE_OBJECT  pDevObj = NULL;
    ULONG           pDevExt = 0;
    pI8042prtDrvObj = GetDriverObject(L"\\Driver\\i8042prt");

    if (pI8042prtDrvObj == NULL) {
        KdPrintEx((xx, "[GetKbdClassServiceCallBack] GetDriverObject Failure!\n"));
        return NULL;
    }

    KdPrintEx((xx, "[GetKbdClassServiceCallBack] i8042prt drvobj: 0x%08x\n",
               pI8042prtDrvObj));
    pDevObj = pI8042prtDrvObj->DeviceObject->NextDevice;
    pDevExt = (ULONG)pDevObj->DeviceExtension;
    return (PVOID)(*(ULONG*)(pDevExt + 0xf8));
}


PDRIVER_OBJECT
GetDriverObject(WCHAR *wDrvName)
{
    UNICODE_STRING  usDrvName;
    NTSTATUS        ntStatus;
    PDRIVER_OBJECT  pDrvObj;
    RtlInitUnicodeString(&usDrvName, wDrvName);
    KdPrintEx((xx, "[GetDriverObject]usDrvName: %wZ\n", &usDrvName));
    ntStatus = ObReferenceObjectByName(&usDrvName, OBJ_CASE_INSENSITIVE, NULL,
                                       0, *IoDriverObjectType, KernelMode, NULL, (PVOID*)&pDrvObj);

    if (!NT_SUCCESS(ntStatus)) {
        KdPrintEx((xx, "[GetDriverObject] ObReferenceObjectByName Failure! ntStatus=0x%08x\n",
                   ntStatus));
        return NULL;
    }

    ObDereferenceObject(pDrvObj);
    return pDrvObj;
}

ULONG
CalcFunctionLength(ULONG uFnStart)
{
    PUCHAR  pStart = NULL, pEnd = NULL;
    pStart = pEnd = (PUCHAR)uFnStart;

    do {
        if (*pEnd == 0xcc) {
            break;
        }

        ++pEnd;
    } while (TRUE);

    return (ULONG)(pEnd - pStart);
}

PIRP
NewDequeueRead(PVOID pDevExt)
{
    fnKeyBoardClassDequeueRead  pOrgDequeueRead = NULL;
    PIRP                        pIrp = NULL;
    PIO_STACK_LOCATION          pSP = NULL;
    ULONG                       uCompleteRoutineAddr = 0;

    do {
        if (*(ULONG*)((ULONG)pDevExt + 0x70) == (ULONG)pDevExt + 0x70) {
            KdPrintEx((xx, "[NewDequeueRead] IRP Queue is Empty!\n"));
            break;
        }

        pIrp = (PIRP)(*(ULONG*)((ULONG)pDevExt + 0x70) + 0xffffffa8);
        pSP = IoGetCurrentIrpStackLocation(pIrp);
        uCompleteRoutineAddr = (ULONG)pSP->CompletionRoutine;

        if (uOrgCompleteRoutine == 0) {
            uOrgCompleteRoutine = uCompleteRoutineAddr;

            if (uOrgCompleteRoutine == 0) {
                uOrgCompleteRoutine = IRPOrgCompleteRoutinueNULL;
            }
        }

        if (uCompleteRoutineAddr != (LONG)NewCompleteRoutine) {
            InterlockedExchange((LONG*)&pSP->CompletionRoutine, (LONG)NewCompleteRoutine);

            if (uCompleteRoutineAddr == 0) {
                pSP->Context = 0;
                pSP->Control |= SL_INVOKE_ON_SUCCESS;
                pSP->Control |= SL_INVOKE_ON_ERROR;
            }
        }
    } while (0);

    pOrgDequeueRead = (fnKeyBoardClassDequeueRead)uOrgDequeueRead;
    return pOrgDequeueRead(pDevExt);
}


NTSTATUS
NewCompleteRoutine(
    IN PDEVICE_OBJECT  DeviceObject,
    IN PIRP  pIrp,
    IN PVOID  Context
)
{
    PKEYBOARD_INPUT_DATA pBuf;
    pBuf = (PKEYBOARD_INPUT_DATA)pIrp->AssociatedIrp.SystemBuffer;
    KdPrintEx((xx, "[NewCompleteRoutine]Status:%s Information: %d Key: %s %02x \n",
               (pIrp->IoStatus.Status == STATUS_PENDING) ? "pending" : "success", pIrp->IoStatus.Information,
               (pBuf->Flags != 1) ? "Make or else" : "Break", pBuf->MakeCode));

    if (uOrgCompleteRoutine != IRPOrgCompleteRoutinueNULL) {
        fnCompleteRoutine pOrgCpmpleteRoutine = (fnCompleteRoutine)uOrgCompleteRoutine;
        return pOrgCpmpleteRoutine(DeviceObject, pIrp, Context);
    }

    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }

    return STATUS_SUCCESS;
}