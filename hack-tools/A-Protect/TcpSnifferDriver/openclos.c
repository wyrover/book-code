#include <ntddk.h>
#include <ndis.h>

#include "packet.h"

NTSTATUS
NdisuioOpen(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CREATE.
    We simply succeed this.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus = STATUS_SUCCESS;

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pIrpSp->FileObject->FsContext = NULL;

    DEBUGP(DL_INFO, ("Open: FileObject %p\n", pIrpSp->FileObject));

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return NtStatus;
}

NTSTATUS
NdisuioClose(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CLOSE.
    We simply succeed this.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    NTSTATUS                NtStatus;
    PIO_STACK_LOCATION      pIrpSp;
    PNDISUIO_OPEN_CONTEXT   pOpenContext;

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    DEBUGP(DL_INFO, ("Close: FileObject %p\n",
        IoGetCurrentIrpStackLocation(pIrp)->FileObject));

    if (pOpenContext != NULL)
    {
        NUIO_STRUCT_ASSERT(pOpenContext, oc);

        //
        //  Deref the endpoint
        //
        NUIO_DEREF_OPEN(pOpenContext);  // Close
    }

    pIrpSp->FileObject->FsContext = NULL;
    NtStatus = STATUS_SUCCESS;
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return NtStatus;
}
   
NTSTATUS
NdisuioCleanup(
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    )
/*++

Routine Description:

    This is the dispatch routine for handling IRP_MJ_CLEANUP.

Arguments:

    pDeviceObject - Pointer to the device object.

    pIrp - Pointer to the request packet.

Return Value:

    Status is returned.

--*/
{
    PIO_STACK_LOCATION      pIrpSp;
    NTSTATUS                NtStatus;
    NDIS_STATUS             NdisStatus;
    PNDISUIO_OPEN_CONTEXT   pOpenContext;
    ULONG                   PacketFilter;
    ULONG                   BytesProcessed;

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;

    DEBUGP(DL_VERY_LOUD, ("Cleanup: FileObject %p, Open %p\n",
        pIrpSp->FileObject, pOpenContext));

    if (pOpenContext != NULL)
    {
        NUIO_STRUCT_ASSERT(pOpenContext, oc);

        //
        //  Mark this endpoint.
        //
        NUIO_ACQUIRE_LOCK(&pOpenContext->Lock);

        NUIO_SET_FLAGS(pOpenContext->Flags, NUIOO_OPEN_FLAGS, NUIOO_OPEN_IDLE);
        pOpenContext->pFileObject = NULL;

        NUIO_RELEASE_LOCK(&pOpenContext->Lock);

        //
        //  Set the packet filter to 0, telling NDIS that we aren't
        //  interested in any more receives.
        //
        PacketFilter = 0;
        NdisStatus = ndisuioValidateOpenAndDoRequest(
			pOpenContext,
			NdisRequestSetInformation,
			OID_GEN_CURRENT_PACKET_FILTER,
			&PacketFilter,
			sizeof(PacketFilter),
			&BytesProcessed,
			FALSE   // Don't wait for device to be powered on
			);
		
        if (NdisStatus != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_INFO, ("Cleanup: Open %p, set packet filter (%x) failed: %x\n",
                    pOpenContext, PacketFilter, NdisStatus));
            //
            //  Ignore the result. If this failed, we may continue
            //  to get indicated receives, which will be handled
            //  appropriately.
            //
            NdisStatus = NDIS_STATUS_SUCCESS;
        }

        //
        //  Cancel any pending reads.
        //
        ndisuioCancelPendingReads(pOpenContext);

		// no file linked to it, so unbind it.
		NdisuioUnbindAdapter(&NdisStatus, pOpenContext, 0);
    }

    NtStatus = STATUS_SUCCESS;

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = NtStatus;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    DEBUGP(DL_INFO, ("Cleanup: OpenContext %p\n", pOpenContext));

    return (NtStatus);
}

