///
/// @file			    sfilter.h
/// @author		crazy_chu
/// @date		    2007-5-17
///

#ifndef _TWCN_SFILTER_HEADER_

#define _TWCN_SFILTER_HEADER_

typedef enum{
	SF_IRP_GO_ON = 0,
	SF_IRP_COMPLETED = 1,
	SF_IRP_PASS = 2
} SF_RET;

extern SF_RET OnSfilterIrpPre(
		IN PDEVICE_OBJECT DeviceObject,
		IN PDEVICE_OBJECT NextObject,
		IN PVOID extension,
		IN PIRP Irp,
		OUT NTSTATUS *status,
		PVOID *context);

extern VOID OnSfilterIrpPost(
		IN PDEVICE_OBJECT DeviceObject,
		IN PDEVICE_OBJECT NextObject,
		IN PVOID extension,
		IN PIRP Irp,
		IN NTSTATUS status,
		PVOID context);

extern NTSTATUS OnSfilterDriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath,
	OUT PUNICODE_STRING userNameString,
	OUT PUNICODE_STRING syblnkString,
	OUT PULONG extensionSize);

extern VOID OnSfilterDriverUnload();

extern NTSTATUS OnSfilterCDODispatch(
		IN PDEVICE_OBJECT DeviceObject,
		IN PIRP Irp);

extern BOOLEAN OnSfilterAttachPre(
		IN PDEVICE_OBJECT ourDevice,
		IN PDEVICE_OBJECT theDeviceToAttach,
		IN PUNICODE_STRING DeviceName,
		IN PVOID extension);

extern VOID OnSfilterAttachPost(
		IN PDEVICE_OBJECT ourDevice,
		IN PDEVICE_OBJECT theDeviceToAttach,
		IN PDEVICE_OBJECT theDeviceToAttached,
		IN PVOID extension,
		IN NTSTATUS status);

extern BOOLEAN OnSfFastIoDeviceControl(
		IN PFILE_OBJECT FileObject,
		IN PVOID InputBuffer OPTIONAL,
		IN ULONG InputBufferLength,
		OUT PVOID OutputBuffer OPTIONAL,
		IN ULONG OutputBufferLength,
		IN ULONG IoControlCode,
		OUT PIO_STATUS_BLOCK IoStatus,
		IN PDEVICE_OBJECT DeviceObject);

void sfilterSetCdoAccessForAll();

#endif //_TWCN_SFILTER_HEADER_