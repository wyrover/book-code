// Copyright Ric Vieler, 2006
// Definitions for Ghost IO control

#ifndef _GHOST_IO_H_
#define _GHOST_IO_H_

// Use CreateFile( GHOST_DEVICE_OPEN_NAME,,, externally
// Use GHOST_DEVICE_CREATE_NAME internally to create device
// Use GHOST_DEVICE_LINK_NAME internally to create device link
#define GHOST_DEVICE_CREATE_NAME L"\\Device\\MyDeviceDriver"
#define GHOST_DEVICE_LINK_NAME L"\\DosDevices\\MyDeviceDriver"
#define GHOST_DEVICE_OPEN_NAME "\\\\.\\MyDeviceDriver"

// Set command = GHOST_ON or GHOST_OFF for GHOST_ON_OFF_COMMAND
// Get command = GHOST_ON or GHOST_OFF for GHOST_STATUS_COMMAND
typedef struct
{
	int				command;
} GHOST_IOCTLDATA;

// definitions from ntddk.h
// (these won't be defined in user mode apps)
#ifndef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#endif
#ifndef FILE_DEVICE_UNKNOWN
#define FILE_DEVICE_UNKNOWN             0x00000022
#endif
#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED                 0
#endif
#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS                 0
#endif

// Use these to command the rootkit!
#define GHOST_ON_OFF_COMMAND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) 
#define GHOST_STATUS_COMMAND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) 
#define GHOST_OFF 0
#define GHOST_ON 1

// Internal functions
#ifdef _GHOST_ROOTKIT_

NTSTATUS  OnDeviceControl( PFILE_OBJECT FileObject, BOOLEAN Wait,
	PVOID InputBuffer, ULONG InputBufferLength, 
	PVOID OutputBuffer, ULONG OutputBufferLength, 
	ULONG IoControlCode, PIO_STATUS_BLOCK IoStatus, 
	PDEVICE_OBJECT DeviceObject );
NTSTATUS OnDispatch( PDEVICE_OBJECT DeviceObject, PIRP Irp );

#endif
#endif

