//======================================================================
// 
// Ctrl2cap.h
//
// Copyright (C) 1996-1999 Mark Russinovich
//
// Hook onto the keyboard I/O path and massage the input stream
// converting caps-locks into controls. This example works on
// NT 4 and Win2K and the Win2K version is very losely based on the
// i8042 port filter driver sample, kbfiltr, from the Win2K DDK.
//
//======================================================================

//
// Define scan codes of interest here. For scan code tables, see 
// "The Undocumented PC", by Frank Van Gullwe, Addison-Wesley 1994.
//

#define KEY_UP         1
#define KEY_DOWN       0

#define LCONTROL       ((USHORT)0x1D)
#define CAPS_LOCK      ((USHORT)0x3A)

//
// Print macro that only turns on when checked builds are on
//
#if DBG
#define DbgPrint(arg) DbgPrint arg
#else
#define DbgPrint(arg)
#endif

//
// Undocumented NT 4 function to print to the startup screen
//
NTSYSAPI
NTSTATUS
NTAPI
ZwDisplayString( PUNICODE_STRING String );


//
// Our device extension definition
//
typedef struct _DEVICE_EXTENSION
{
    //
    // The top of the stack before this filter was added.  AKA the location
    // to which all IRPS should be directed.
    //
    PDEVICE_OBJECT  TopOfStack;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//
// Forwards
//
NTSTATUS DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath 
    );
NTSTATUS
Ctrl2capAddDevice(
    IN PDRIVER_OBJECT   Driver,
    IN PDEVICE_OBJECT   PDO
    );
NTSTATUS Ctrl2capPnP(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp 
    );
NTSTATUS Ctrl2capPower(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp 
    );
VOID
Ctrl2capUnload(
   IN PDRIVER_OBJECT Driver
   );
NTSTATUS Ctrl2capInit( 
    IN PDRIVER_OBJECT DriverObject 
    );
NTSTATUS Ctrl2capDispatchRead( 
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp 
    );
NTSTATUS Ctrl2capDispatchGeneral(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp 
    );


