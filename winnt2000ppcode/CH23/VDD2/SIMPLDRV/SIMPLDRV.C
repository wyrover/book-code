/*++

Copyright (c) 1993  Microsoft Corporation

Module Name:

    simpldrv.c

Abstract:

    A simple kernel-mode driver sample.

Environment:

    kernel mode only

Notes:

    See readme.txt

Revision History:

    06-25-93 : created

--*/

#include "ntddk.h"
#include "stdarg.h"
#include "stdio.h"
#include "simpldrv.h"



//
// The following is the debug print macro- when we are building checked
// drivers "DBG" will be defined (by the \ddk\setenv.cmd script), and we
// will see debug messages appearing on the KD screen on the host debug
// machine. When we build free drivers "DBG" is not defined, and calls
// to SimplDrvKdPrint are removed.
//

#ifdef DBG
#define SimplDrvKdPrint(arg) DbgPrint arg
#else
#define SimplDrvKdPrint(arg)
#endif



NTSTATUS
SimplDrvDispatch(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

VOID
SimplDrvUnload(
    IN PDRIVER_OBJECT DriverObject
    );



NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:

    Installable driver initialization entry point.
    This entry point is called directly by the I/O system.

Arguments:

    DriverObject - pointer to the driver object

    RegistryPath - pointer to a unicode string representing the path
                   to driver-specific key in the registry

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise

--*/
{

    PDEVICE_OBJECT         deviceObject        = NULL;
    NTSTATUS               ntStatus;
    WCHAR                  deviceNameBuffer[]  = L"\\Device\\SimplDrv";
    UNICODE_STRING         deviceNameUnicodeString;
    PDEVICE_EXTENSION      deviceExtension;
    WCHAR                  deviceLinkBuffer[]  = L"\\DosDevices\\SIMPLDRV";
    UNICODE_STRING         deviceLinkUnicodeString;


    SimplDrvKdPrint (("SIMPLDRV.SYS: entering DriverEntry\n"));



    //
    // A real driver would:
    //
    //     1. Report it's resources (IoReportResourceUsage)
    //
    //     2. Attempt to locate the device(s) it supports



    //
    // OK, we've claimed our resources & found our h/w, so create
    // a device and initialize stuff...
    //

    RtlInitUnicodeString (&deviceNameUnicodeString,
                          deviceNameBuffer
                          );



    //
    // Create an EXCLUSIVE device, i.e. only 1 thread at a time can send
    // i/o requests.
    //

    ntStatus = IoCreateDevice (DriverObject,
                               sizeof (DEVICE_EXTENSION),
                               &deviceNameUnicodeString,
                               FILE_DEVICE_SIMPLDRV,
                               0,
                               TRUE,
                               &deviceObject
                               );

    if (NT_SUCCESS(ntStatus))
    {
        deviceExtension = (PDEVICE_EXTENSION) deviceObject->DeviceExtension;



        //
        // Set up synchronization objects, state info,, etc.
        //



        //
        // Create a symbolic link that Win32 apps can specify to gain access
        // to this driver/device
        //

        RtlInitUnicodeString (&deviceLinkUnicodeString,
                              deviceLinkBuffer
                              );

        ntStatus = IoCreateSymbolicLink (&deviceLinkUnicodeString,
                                         &deviceNameUnicodeString
                                         );


        if (!NT_SUCCESS(ntStatus))
        {
            SimplDrvKdPrint (("SIMPLDRV.SYS: IoCreateSymbolicLink failed\n"));
        }



        //
        // Create dispatch points for device control, create, close.
        //

        DriverObject->MajorFunction[IRP_MJ_CREATE]         =
        DriverObject->MajorFunction[IRP_MJ_CLOSE]          =
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SimplDrvDispatch;
        DriverObject->DriverUnload                         = SimplDrvUnload;
    }


done_DriverEntry:

    if (!NT_SUCCESS(ntStatus))
    {
        //
        // Something went wrong, so clean up (free resources, etc.)
        //

        if (deviceObject)

            IoDeleteDevice (deviceObject);
    }

    return ntStatus;
}



NTSTATUS
SimplDrvDispatch(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
/*++

Routine Description:

    Process the IRPs sent to this device.

Arguments:

    DeviceObject - pointer to a device object

    Irp          - pointer to an I/O Request Packet

Return Value:


--*/
{

    PIO_STACK_LOCATION  irpStack;
    PDEVICE_EXTENSION   deviceExtension;
    PVOID               ioBuffer;
    ULONG               inputBufferLength;
    ULONG               outputBufferLength;
    ULONG               ioControlCode;
    NTSTATUS            ntStatus;

    // added by PLT
    unsigned char ucValue = 13;

    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;


    //
    // Get a pointer to the current location in the Irp. This is where
    //     the function codes and parameters are located.
    //

    irpStack = IoGetCurrentIrpStackLocation (Irp);



    //
    // Get a pointer to the device extension
    //

    deviceExtension = DeviceObject->DeviceExtension;



    //
    // Get the pointer to the input/output buffer and it's length
    //

    ioBuffer           = Irp->AssociatedIrp.SystemBuffer;
    inputBufferLength  = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;



    switch (irpStack->MajorFunction)
    {
    case IRP_MJ_CREATE:

        SimplDrvKdPrint (("SIMPLDRV.SYS: IRP_MJ_CREATE\n"));

        break;

    case IRP_MJ_CLOSE:

        SimplDrvKdPrint (("SIMPLDRV.SYS: IRP_MJ_CLOSE\n"));

        break;

    case IRP_MJ_DEVICE_CONTROL:

        SimplDrvKdPrint (("SIMPLDRV.SYS: IRP_MJ_DEVICE_CONTROL\n"));

        ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;

        switch (ioControlCode)
        {

        case IOCTL_SIMPLDRV_HELLO:
        {
            //
            // Some app is saying hello
            //

            // added by PLT
            RtlMoveMemory(ioBuffer, &ucValue, sizeof(ucValue));
            Irp->IoStatus.Information = sizeof(ucValue);

            break;
        }

        default:

            Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;

            SimplDrvKdPrint (("SIMPLDRV.SYS: unknown IRP_MJ_DEVICE_CONTROL\n"));

            break;

        }

        break;
    }


    //
    // DON'T get cute and try to use the status field of
    // the irp in the return status.  That IRP IS GONE as
    // soon as you call IoCompleteRequest.
    //

    ntStatus = Irp->IoStatus.Status;

    IoCompleteRequest (Irp,
                       IO_NO_INCREMENT
                       );


    //
    // We never have pending operation so always return the status code.
    //

    return ntStatus;
}



VOID
SimplDrvUnload(
    IN PDRIVER_OBJECT DriverObject
    )
/*++

Routine Description:

    Free all the allocated resources, etc.

Arguments:

    DriverObject - pointer to a driver object

Return Value:


--*/
{
    WCHAR                  deviceLinkBuffer[]  = L"\\DosDevices\\SIMPLDRV";
    UNICODE_STRING         deviceLinkUnicodeString;



    //
    // Free any resources
    //



    //
    // Delete the symbolic link
    //

    RtlInitUnicodeString (&deviceLinkUnicodeString,
                          deviceLinkBuffer
                          );

    IoDeleteSymbolicLink (&deviceLinkUnicodeString);



    //
    // Delete the device object
    //

    IoDeleteDevice (DriverObject->DeviceObject);

    SimplDrvKdPrint (("SIMPLDRV.SYS: unloading\n"));
}
