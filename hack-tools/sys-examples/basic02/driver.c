// BASIC DEVICE DRIVER

#include <ntddk.h>

/* __________________________________________________________________
 . This function just completes all IRP's that come its way.
 . We are ignoring userland completely - so this shouldn't get
 . called anyways -
 . __________________________________________________________________ */
NTSTATUS
OnStubDispatch(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
{
    Irp->IoStatus.Status      = STATUS_SUCCESS;
    IoCompleteRequest (Irp,
                       IO_NO_INCREMENT
                       );
    return Irp->IoStatus.Status;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT theDriverObject, IN PUNICODE_STRING theRegistryPath)
{
	int i;

	DbgPrint("My Driver Loaded!");

	// Register a dispatch function
	for (i = 0; i <IRP_MJ_MAXIMUM_FUNCTION; i++) 
	{
        	theDriverObject->MajorFunction[i] = OnStubDispatch;
    	}

	return STATUS_SUCCESS;
}
