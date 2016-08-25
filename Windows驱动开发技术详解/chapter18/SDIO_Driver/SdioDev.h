#pragma once

extern "C"
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,
                     IN PUNICODE_STRING RegistryPath);
                     
NTSTATUS
BulkUsb_AddDevice(
IN PDRIVER_OBJECT DriverObject,
IN PDEVICE_OBJECT PhysicalDeviceObject
);
                     
                     
void
SdioClientDrv_DriverUnload(
IN PDRIVER_OBJECT DriverObject
);

NTSTATUS
SdioClientDrv_AddDevice(
IN PDRIVER_OBJECT DriverObject,
IN PDEVICE_OBJECT PhysicalDeviceObject
);

NTSTATUS
SdioGetProperty(
		IN PDEVICE_OBJECT deviceObject,
               IN SDBUS_PROPERTY Property,
               IN PVOID Buffer,
               IN ULONG Length
               );
               
NTSTATUS
SdioSetProperty(IN PDEVICE_OBJECT deviceObject,
				IN SDBUS_PROPERTY Property,
				IN PVOID Buffer,
				IN ULONG Length);    
				
NTSTATUS HelloWDMDeviceIOControl(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp);

NTSTATUS
SdioReadWriteByte(IN PDEVICE_OBJECT deviceObject,
                 IN ULONG Function,
                 IN PUCHAR Data,
                 IN ULONG Address,
                 IN BOOLEAN WriteToDevice
                 );
NTSTATUS
SdioReadWriteBuffer(IN PDEVICE_OBJECT deviceObject,
                   IN ULONG Function,
                   IN PMDL Mdl,
                   IN ULONG Address,
                   IN ULONG Length,
                   IN BOOLEAN WriteToDevice,
                   OUT PULONG BytesRead
                   );

NTSTATUS
SdioReadBufferFromDevice(IN PDEVICE_OBJECT deviceObject,
                   IN ULONG Function,
                   IN PMDL Mdl,
                   IN ULONG Address,
                   IN ULONG Length,
                   OUT PULONG BytesRead
                   );
NTSTATUS EnableInt(PDEVICE_EXTENSION pdx);
NTSTATUS DisableInt(PDEVICE_EXTENSION pdx);