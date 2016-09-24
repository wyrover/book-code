/*++

Copyright (c)  Microsoft Corporation

Module Name:

    kmdf_vdev_util.c

Abstract

    Utilies for the 1394 Virtual Device Driver

--*/
#include "kmdf_vdev.h"

NTSTATUS
kmdf1394_SubmitIrpSynch (
                         IN WDFIOTARGET IoTarget,
                         IN WDFREQUEST Request,
                         IN PIRB Irb)
/*++

Routine Description:

    Synchronous Irp submission routine

Arguments:

    IoTarget - pointer to the IoTarget object.

    Request - the WDF Request packet to use

    Irb - 1394 I/O Request Block to submit

Return Value:

    NTSTATUS value
--*/
{
    NTSTATUS ntStatus = STATUS_SUCCESS;

    WDF_REQUEST_SEND_OPTIONS option;
    WDF_MEMORY_DESCRIPTOR descriptor;

    UNREFERENCED_PARAMETER (Request);

    ENTER ("kmdf1394_SubmitIrpSynch");

    ASSERT (KeGetCurrentIrql() < DISPATCH_LEVEL);
    ASSERT (Irb);

    WDF_REQUEST_SEND_OPTIONS_INIT (&option, WDF_REQUEST_SEND_OPTION_SYNCHRONOUS);

    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER (&descriptor, Irb, sizeof (IRB));

    ntStatus = WdfIoTargetSendInternalIoctlOthersSynchronously (
        IoTarget, 
        NULL, 
        IOCTL_1394_CLASS, 
        &descriptor, 
        NULL, 
        NULL, 
        &option, 
        NULL);
    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("WdfIoTargetSendInternalIoctlSynchronously " 
            "Failed with status %x\n",ntStatus));
    }

    EXIT ("kmdf1394_SubmitIrpSynch", ntStatus);

    return ntStatus;
} //kmdf1394_SubmitIrpSynch

VOID
kmdf1394_SubmitIrpAsyncCompletion (
                                   IN WDFREQUEST  Request,    
                                   IN WDFIOTARGET  Target,    
                                   IN PWDF_REQUEST_COMPLETION_PARAMS  Params,    
                                   IN WDFCONTEXT  Context)
/*++

Routine Description:

    Asynchronous request completion routine

Arguments:

    Request - current request object

    Target - I/O Target object

    Params - WDF request Parameter structure
    
    Context - any additional objects the request may 
    need to clean up from the inital submission.

Return Value:

    NTSTATUS value
--*/
{
    // 
    // Here we'll free our memory object from the async request.
    //
    UNREFERENCED_PARAMETER (Target);
    UNREFERENCED_PARAMETER (Request);

    ASSERT (Context);

    
    if (!NT_SUCCESS(Params->IoStatus.Status))
    {
        TRACE(
            TL_ERROR, 
            ("Request %x Failed with status %x\n",
            Request,
            Params->IoStatus.Status));
    }
    

    WdfObjectDelete (Context);
}

NTSTATUS
kmdf1394_SubmitIrpAsync (
                         IN WDFIOTARGET IoTarget,
                         IN WDFREQUEST  Request,
                         IN WDFMEMORY   Memory)
/*++

Routine Description:

    Asynchronous request submission routine

Arguments:

    IoTarget - I/O Target object
    
    Request - current request object

    Irb - 1394 I/O Request Block to submit


Return Value:

    NTSTATUS value
--*/
{
    NTSTATUS ntStatus = STATUS_SUCCESS;

    ENTER("kmdf1394_SubmitIrpAsync");

    ASSERT (KeGetCurrentIrql() < DISPATCH_LEVEL);
    ASSERT (Memory);

    if (!NT_SUCCESS (ntStatus))
    {
        return ntStatus;
    }


    ntStatus = WdfIoTargetFormatRequestForInternalIoctlOthers (
        IoTarget, 
        Request, 
        IOCTL_1394_CLASS, 
        Memory, 
        NULL, 
        NULL, 
        NULL, 
        NULL,
        NULL);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE(
            TL_ERROR, 
            ("WdfIoTargetFormatRequestForInternalIoctlOthers failed "
            "with status %x\n",
            ntStatus));

        return ntStatus;
    }

    if (FALSE == (WdfRequestSend (Request, IoTarget, NULL)))
    {
        ntStatus = WdfRequestGetStatus (Request);
    }


    EXIT("kmdf1394_SubmitIrpAsync", ntStatus);

    return ntStatus;

} // kmdf1394_SubmitIrpAsync


NTSTATUS
kmdf1394_UpdateGenerationCount (
                                IN WDFDEVICE Device)
/*++

Routine Description:

    Routine to get and update generation count for the 1394 controller

Arguments:

    Device - WDF Device object

Return Value:

    NTSTATUS value
--*/
{
    PDEVICE_EXTENSION  deviceExtension = GetDeviceContext(Device);
    NTSTATUS    ntStatus = STATUS_SUCCESS;
    PIRB Irb = NULL;
    WDFREQUEST request = NULL;

    ENTER("kmdf1394_UpdateGenerationCountWorkItem");

    ntStatus = WdfRequestCreate ( 
                            WDF_NO_OBJECT_ATTRIBUTES,
                            deviceExtension->StackIoTarget,
                            &request);

    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("Failed to allocate request!\n"));

        return STATUS_INSUFFICIENT_RESOURCES;
    }

    Irb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!Irb)
    {
        TRACE(TL_ERROR, ("Failed to allocate Irb!\n"));

        WdfObjectDelete (request);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (Irb, sizeof (IRB));
    Irb->FunctionNumber = REQUEST_GET_GENERATION_COUNT;
    Irb->Flags = 0;

    ntStatus = kmdf1394_SubmitIrpSynch (deviceExtension->StackIoTarget, request, Irb);
    if (NT_SUCCESS(ntStatus))
    {
        TRACE(TL_TRACE, ("GenerationCount = 0x%x\n", deviceExtension->GenerationCount));
       
        //
        // Update our local copy of the GenerationCount
        //
        deviceExtension->GenerationCount = Irb->u.GetGenerationCount.GenerationCount;       
    }
    else
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    WdfObjectDelete(request);
    ExFreePoolWithTag(Irb,POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_UpdateGenerationCountWorkItem", ntStatus);

    return ntStatus;
} // kmdf1394_UpdateGenerationCount 


//
// TODO: this will be used in our Attach / Detach buffer routines
//
BOOLEAN
kmdf1394_IsOnList (
                  PLIST_ENTRY        Entry,
                  PLIST_ENTRY        List)
{
    PLIST_ENTRY TempEntry;

    for (TempEntry = List->Flink; TempEntry != List; TempEntry = TempEntry->Flink)
    {
        if (TempEntry == Entry)
        {
            TRACE(TL_TRACE, ("Entry 0x%x found on list 0x%x\n", Entry, List));
            return TRUE;
        }
    }

    TRACE(TL_TRACE, ("Entry 0x%x not found on list 0x%x\n", Entry, List));
    return FALSE;
}
