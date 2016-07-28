/*++

Copyright (c) Microsoft Corporation

Module Name:

    kmdf_vdev.cpp

Abstract:

    Entry file for the kmdf component of the 1394 virtual device hybrid driver sample.

--*/



#include "kmdf_vdev.h"

#if DBG

unsigned char kmdf_vdev_DebugLevel = TL_WARNING;

#endif

NTSTATUS
DriverEntry (
             IN PDRIVER_OBJECT   DriverObject,
             IN PUNICODE_STRING  RegistryPath)
/*++

Routine Description:

    Installable driver initialization entry point.
    This entry point is called directly by the I/O system.

Arguments:

    DriverObject - pointer to the driver object

    RegistryPath - pointer to a unicode string representing the path,
                   to driver-specific key in the registry.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
    NTSTATUS                      ntStatus = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG   Config;

    ENTER ("DriverEntry");

    TRACE (TL_TRACE, ("1394VDev Hybrid Sample\n"));
    TRACE (TL_TRACE, ("Built %s %s\n", __DATE__, __TIME__));

    //
    // Initialize the Driver Config structure..
    //
    WDF_DRIVER_CONFIG_INIT (&Config, kmdf1394_EvtDeviceAdd);

    //
    // Create a WDFDRIVER object.
    //
    ntStatus = WdfDriverCreate (
        DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &Config,
        WDF_NO_HANDLE);
    if (!NT_SUCCESS (ntStatus)) 
    {
      TRACE(TL_ERROR, ("WdfDriverCreate failed with status %x\n", ntStatus));
    }

    EXIT("DriverEntry", ntStatus);
    return ntStatus;
} // DriverEntry
