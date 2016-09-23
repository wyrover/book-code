#include "ntifs.h"
#include "Common.h"
#include "InitWindowsVersion.h"
#include "FileSystem.h"


NTSTATUS
IoCompletionRoutine(
					IN PDEVICE_OBJECT DeviceObject,
					IN PIRP Irp,
					IN PVOID Context)
{
	*Irp->UserIosb = Irp->IoStatus;
	if (Irp->UserEvent)
		KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, 0);
	if (Irp->MdlAddress)
	{
		IoFreeMdl(Irp->MdlAddress);
		Irp->MdlAddress = NULL;
	}
	IoFreeIrp(Irp);
	return STATUS_MORE_PROCESSING_REQUIRED;
}





NTSTATUS
IrpCreateFile(
			  IN PUNICODE_STRING FilePath,
			  IN ACCESS_MASK DesiredAccess,
			  IN ULONG FileAttributes,
			  IN ULONG ShareAccess,
			  IN ULONG CreateDisposition,
			  IN ULONG CreateOptions,
			  IN PDEVICE_OBJECT DeviceObject,
			  IN PDEVICE_OBJECT RealDevice,
			  OUT PFILE_OBJECT *FileObject
			  )
// 			  OUT PFILE_OBJECT *FileObject,
// 			  IN ACCESS_MASK DesiredAccess,
// 			  IN PUNICODE_STRING FilePath,
// 			  OUT PIO_STATUS_BLOCK IoStatusBlock,
// 			  IN PLARGE_INTEGER AllocationSize OPTIONAL,
// 			  IN ULONG FileAttributes,
// 			  IN ULONG ShareAccess,
// 			  IN ULONG CreateDisposition,
// 			  IN ULONG CreateOptions,
// 			  IN PVOID EaBuffer OPTIONAL,
// 			  IN ULONG EaLength)
{
	NTSTATUS ntStatus;

	HANDLE hFile;
	PFILE_OBJECT  _FileObject;
	UNICODE_STRING UniDeviceNameString;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;
	WCHAR *FileNameBuffer=NULL;
	WORD FileObjectSize;
	WIN_VER_DETAIL WinVer;

	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;
	ACCESS_STATE AccessState;
	AUX_ACCESS_DATA AuxData;
	IO_SECURITY_CONTEXT SecurityContext;

	InitializeObjectAttributes(&ObjectAttributes, NULL, OBJ_CASE_INSENSITIVE, 0, NULL);
	WinVer=GetWindowsVersion();
	if (WinVer==WINDOWS_VERSION_2K||
		WinVer==WINDOWS_VERSION_XP||
		WinVer==WINDOWS_VERSION_2K3||
		WinVer==WINDOWS_VERSION_2K3_SP1_SP2)
	{
		FileObjectSize=0x70;
	}
	else if (WinVer==WINDOWS_VERSION_VISTA_2008||WinVer==WINDOWS_VERSION_7_7000||WinVer==WINDOWS_VERSION_7_7600_UP)
	{
		FileObjectSize=0x80;
	}
	else
	{
		return STATUS_UNSUCCESSFUL;
	}

	ntStatus = ObCreateObject(KernelMode,
		*IoFileObjectType,
		&ObjectAttributes,
		KernelMode,
		NULL,
		FileObjectSize,
		0,
		0,
		&_FileObject);

	if(!NT_SUCCESS(ntStatus))
	{
		if (DebugOn)
			KdPrint(("[PsVoid] IrpCreateFile: ObCreateObject 0x%X.\n",ntStatus));
		return ntStatus;
	}

	Irp = IoAllocateIrp(DeviceObject->StackSize, FALSE);
	if(Irp == NULL)
	{
		if (DebugOn)
			KdPrint(("[PsVoid] IrpCreateFile: IoAllocateIrp 0x%X.\n",ntStatus));
		ObDereferenceObject(_FileObject);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	RtlZeroMemory(_FileObject, FileObjectSize);
	_FileObject->Type = IO_TYPE_FILE;
	_FileObject->Size = FileObjectSize;
	_FileObject->DeviceObject = RealDevice;
	_FileObject->Flags = FO_SYNCHRONOUS_IO;
	FileNameBuffer=ExAllocatePool(NonPagedPool,FilePath->MaximumLength);
	if (FileNameBuffer==NULL)
	{
		ObDereferenceObject(_FileObject);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlCopyMemory(FileNameBuffer,FilePath->Buffer,FilePath->Length);
    _FileObject->FileName.Buffer=FileNameBuffer;
	_FileObject->FileName.Length=FilePath->Length;
	_FileObject->FileName.MaximumLength=FilePath->MaximumLength;
	if (WinVer==WINDOWS_VERSION_VISTA_2008||WinVer==WINDOWS_VERSION_7_7000||WinVer==WINDOWS_VERSION_7_7600_UP)
	{
		PLIST_ENTRY IrpList;
		IrpList=(PLIST_ENTRY)((DWORD)FileObject+0x74);
		IrpList->Flink=IrpList;
		IrpList->Blink=IrpList;
	}

	KeInitializeEvent(&_FileObject->Lock, SynchronizationEvent, FALSE);
	KeInitializeEvent(&_FileObject->Event, NotificationEvent, FALSE);

	RtlZeroMemory(&AuxData, sizeof(AUX_ACCESS_DATA));
	ntStatus = SeCreateAccessState( &AccessState,
		&AuxData,
		DesiredAccess,
		IoGetFileObjectGenericMapping());

	if (!NT_SUCCESS(ntStatus))
	{
		if (DebugOn)
			KdPrint(("[PsVoid] IrpCreateFile: SeCreateAccessState 0x%X.\n",ntStatus));
		IoFreeIrp(Irp);
		ObDereferenceObject(_FileObject);
		ExFreePool(FileNameBuffer);
		return ntStatus;
	}

	SecurityContext.SecurityQos = NULL;
	SecurityContext.AccessState = &AccessState;
	SecurityContext.DesiredAccess = DesiredAccess;
	SecurityContext.FullCreateOptions = 0;

	Irp->MdlAddress = NULL;
	Irp->AssociatedIrp.SystemBuffer = NULL;
	Irp->Flags = IRP_CREATE_OPERATION|IRP_SYNCHRONOUS_API;
	Irp->RequestorMode = KernelMode;
	Irp->UserIosb = &IoStatusBlock;
	Irp->UserEvent = &kEvent;
	Irp->PendingReturned = FALSE;
	Irp->Cancel = FALSE;
	Irp->CancelRoutine = NULL;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.AuxiliaryBuffer = NULL;
	Irp->Tail.Overlay.OriginalFileObject = _FileObject;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_CREATE;
	IrpSp->DeviceObject = DeviceObject;
	IrpSp->FileObject = _FileObject;
	IrpSp->Parameters.Create.SecurityContext = &SecurityContext;
	IrpSp->Parameters.Create.Options = (CreateDisposition << 24) | CreateOptions;
	IrpSp->Parameters.Create.FileAttributes = (USHORT)FileAttributes;
	IrpSp->Parameters.Create.ShareAccess = (USHORT)ShareAccess;
	IrpSp->Parameters.Create.EaLength = 0;

	IoSetCompletionRoutine(Irp, IoCompletionRoutine, 0, TRUE, TRUE, TRUE);
	ntStatus = IoCallDriver(DeviceObject, Irp);
	if(ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, 0);

	ntStatus = IoStatusBlock.Status;

	if(!NT_SUCCESS(ntStatus))
	{
		if (DebugOn)
			KdPrint(("[PsVoid] IrpCreateFile: IoCallDriver 0x%X.\n",ntStatus));
		_FileObject->DeviceObject = NULL;
		ObDereferenceObject(_FileObject);

	}
	else
	{
		InterlockedIncrement(&_FileObject->DeviceObject->ReferenceCount);
		if (_FileObject->Vpb)
			InterlockedIncrement(&_FileObject->Vpb->ReferenceCount);
		*FileObject = _FileObject;
	}


	return ntStatus;
}

NTSTATUS
IrpClose(
		 IN PFILE_OBJECT FileObject)
{
	NTSTATUS ntStatus;
	IO_STATUS_BLOCK IoStatusBlock;
	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;
	PDEVICE_OBJECT pBaseDeviceObject = FileObject->Vpb->DeviceObject;

	if (FileObject->Vpb == 0 || FileObject->Vpb->DeviceObject == NULL)
		return STATUS_UNSUCCESSFUL;

	Irp = IoAllocateIrp(FileObject->Vpb->DeviceObject->StackSize, FALSE);
	if(Irp == NULL) return STATUS_INSUFFICIENT_RESOURCES;

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	Irp->UserEvent = &kEvent;
	Irp->UserIosb = &IoStatusBlock;
	Irp->RequestorMode = KernelMode;
	Irp->Flags = IRP_CLOSE_OPERATION|IRP_SYNCHRONOUS_API;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.OriginalFileObject = FileObject;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_CLEANUP;
	IrpSp->FileObject = FileObject;

	ntStatus = IoCallDriver(pBaseDeviceObject, Irp);
	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);

	ntStatus = IoStatusBlock.Status;
	if(!NT_SUCCESS(ntStatus))
	{
		IoFreeIrp(Irp);
		return ntStatus;
	}

	KeClearEvent(&kEvent);
	IoReuseIrp(Irp , STATUS_SUCCESS);

	Irp->UserEvent = &kEvent;
	Irp->UserIosb = &IoStatusBlock;
	Irp->Tail.Overlay.OriginalFileObject = FileObject;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->AssociatedIrp.SystemBuffer = (PVOID)NULL;
	Irp->Flags = IRP_CLOSE_OPERATION|IRP_SYNCHRONOUS_API;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_CLOSE;
	IrpSp->FileObject = FileObject;

	if (FileObject->Vpb && !(FileObject->Flags & FO_DIRECT_DEVICE_OPEN))
	{
		InterlockedDecrement(&FileObject->Vpb->ReferenceCount);
		FileObject->Flags |= FO_FILE_OPEN_CANCELLED;

	}

	ntStatus = IoCallDriver(pBaseDeviceObject, Irp);

	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);

	IoFreeIrp(Irp);

	ntStatus = IoStatusBlock.Status;

	return ntStatus;
}

NTSTATUS
IrpQueryDirectoryFile(
					  IN PFILE_OBJECT FileObject,
					  OUT PIO_STATUS_BLOCK IoStatusBlock,
					  OUT PVOID FileInformation,
					  IN ULONG Length,
					  IN FILE_INFORMATION_CLASS FileInformationClass,
					  IN PUNICODE_STRING FileName OPTIONAL)
{
	NTSTATUS ntStatus;
	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;

	if (FileObject->Vpb == 0 || FileObject->Vpb->DeviceObject == NULL)
		return STATUS_UNSUCCESSFUL;

	Irp = IoAllocateIrp(FileObject->Vpb->DeviceObject->StackSize, FALSE);
	if(Irp == NULL) return STATUS_INSUFFICIENT_RESOURCES;

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	RtlZeroMemory(FileInformation, Length);
	Irp->UserEvent = &kEvent;
	Irp->UserIosb = IoStatusBlock;
	Irp->UserBuffer = FileInformation;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.OriginalFileObject = FileObject;
	Irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_DIRECTORY_CONTROL;
	IrpSp->MinorFunction = IRP_MN_QUERY_DIRECTORY;
	IrpSp->FileObject = FileObject;
	IrpSp->Flags = SL_RESTART_SCAN;
	IrpSp->Parameters.QueryDirectory.Length = Length;
	IrpSp->Parameters.QueryDirectory.FileName = FileName;
	IrpSp->Parameters.QueryDirectory.FileInformationClass = FileInformationClass;

	IoSetCompletionRoutine(Irp, IoCompletionRoutine, 0, TRUE, TRUE, TRUE);
	ntStatus = IoCallDriver(FileObject->Vpb->DeviceObject, Irp);

	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, 0);

	return IoStatusBlock->Status;
}

NTSTATUS
IrpQueryInformationFile(
						IN PFILE_OBJECT FileObject,
						IN PDEVICE_OBJECT DeviceObject,
						OUT PVOID FileInformation,
						IN ULONG Length,
						IN FILE_INFORMATION_CLASS FileInformationClass)
{
	NTSTATUS ntStatus;
	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;
	IO_STATUS_BLOCK IoStatusBlock;

// 	if (FileObject->Vpb == 0 || FileObject->Vpb->DeviceObject == NULL)
// 		return STATUS_UNSUCCESSFUL;

	Irp = IoAllocateIrp(DeviceObject->StackSize, FALSE);
	if(Irp == NULL) 
		return STATUS_INSUFFICIENT_RESOURCES;

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	RtlZeroMemory(FileInformation, Length);
	Irp->AssociatedIrp.SystemBuffer = FileInformation;
	Irp->UserEvent = &kEvent;
	Irp->UserIosb = &IoStatusBlock;
	Irp->RequestorMode = KernelMode;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.OriginalFileObject = FileObject;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_QUERY_INFORMATION;
	IrpSp->DeviceObject = DeviceObject;
	IrpSp->FileObject = FileObject;
	IrpSp->Parameters.QueryFile.Length = Length;
	IrpSp->Parameters.QueryFile.FileInformationClass = FileInformationClass;

	IoSetCompletionRoutine(Irp, IoCompletionRoutine, 0, TRUE, TRUE, TRUE);
	ntStatus = IoCallDriver(DeviceObject, Irp);

	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, 0);

	return IoStatusBlock.Status;
}

NTSTATUS
IrpSetInformationFile(
					  IN PFILE_OBJECT FileObject,
					  OUT PIO_STATUS_BLOCK IoStatusBlock,
					  IN PVOID FileInformation,
					  IN ULONG Length,
					  IN FILE_INFORMATION_CLASS FileInformationClass,
					  IN BOOLEAN ReplaceIfExists)
{
	NTSTATUS ntStatus;
	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;

	if (FileObject->Vpb == 0 || FileObject->Vpb->DeviceObject == NULL)
		return STATUS_UNSUCCESSFUL;

	Irp = IoAllocateIrp(FileObject->Vpb->DeviceObject->StackSize, FALSE);
	if(Irp == NULL) return STATUS_INSUFFICIENT_RESOURCES;

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	Irp->AssociatedIrp.SystemBuffer = FileInformation;
	Irp->UserEvent = &kEvent;
	Irp->UserIosb = IoStatusBlock;
	Irp->RequestorMode = KernelMode;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.OriginalFileObject = FileObject;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	IrpSp->DeviceObject = FileObject->Vpb->DeviceObject;
	IrpSp->FileObject = FileObject;
	IrpSp->Parameters.SetFile.ReplaceIfExists = ReplaceIfExists;
	IrpSp->Parameters.SetFile.FileObject = FileObject;
	IrpSp->Parameters.SetFile.AdvanceOnly = FALSE;
	IrpSp->Parameters.SetFile.Length = Length;
	IrpSp->Parameters.SetFile.FileInformationClass = FileInformationClass;

	IoSetCompletionRoutine(Irp, IoCompletionRoutine, 0, TRUE, TRUE, TRUE);
	ntStatus = IoCallDriver(FileObject->Vpb->DeviceObject, Irp);
	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, 0);

	return IoStatusBlock->Status;
}

NTSTATUS
IrpReadFile(
			IN PFILE_OBJECT FileObject,
			IN PDEVICE_OBJECT DeviceObject,
			OUT PIO_STATUS_BLOCK IoStatusBlock,
			OUT PVOID Buffer,
			IN ULONG Length,
			IN PLARGE_INTEGER ByteOffset OPTIONAL)
{
	NTSTATUS ntStatus;
	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;
// 


	if(ByteOffset == NULL)
	{
		if(!(FileObject->Flags & FO_SYNCHRONOUS_IO))
			return STATUS_INVALID_PARAMETER;
		ByteOffset = &FileObject->CurrentByteOffset;
	}

	Irp = IoAllocateIrp(DeviceObject->StackSize, FALSE);
	if(Irp == NULL) return STATUS_INSUFFICIENT_RESOURCES;

	RtlZeroMemory(Buffer, Length);
	if(FileObject->DeviceObject->Flags & DO_BUFFERED_IO)
	{
		Irp->AssociatedIrp.SystemBuffer = Buffer;
	}
	else if(FileObject->DeviceObject->Flags & DO_DIRECT_IO)
	{
		Irp->MdlAddress = IoAllocateMdl(Buffer, Length, 0, 0, 0);
		if (Irp->MdlAddress == NULL)
		{
			IoFreeIrp(Irp);
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		MmBuildMdlForNonPagedPool(Irp->MdlAddress);
	}
	else
	{
		Irp->UserBuffer = Buffer;
	}

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	Irp->UserEvent = &kEvent;
	Irp->UserIosb = IoStatusBlock;
	Irp->RequestorMode = KernelMode;
	Irp->Flags = IRP_READ_OPERATION;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.OriginalFileObject = FileObject;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_READ;
	IrpSp->MinorFunction = IRP_MN_NORMAL;
	IrpSp->DeviceObject = DeviceObject;
	IrpSp->FileObject = FileObject;
	IrpSp->Parameters.Read.Length = Length;
	IrpSp->Parameters.Read.ByteOffset = *ByteOffset;

	IoSetCompletionRoutine(Irp, IoCompletionRoutine, 0, TRUE, TRUE, TRUE);
	ntStatus = IoCallDriver(DeviceObject, Irp);
	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, 0);

	return IoStatusBlock->Status;
}

NTSTATUS 
IrpWriteFile(
			 IN PFILE_OBJECT FileObject,
			 OUT PIO_STATUS_BLOCK IoStatusBlock,
			 IN PVOID Buffer,
			 IN ULONG Length,
			 IN PLARGE_INTEGER ByteOffset OPTIONAL)
{
	NTSTATUS ntStatus;
	PIRP Irp;
	KEVENT kEvent;
	PIO_STACK_LOCATION IrpSp;

	if (FileObject->Vpb == 0 || FileObject->Vpb->DeviceObject == NULL)
		return STATUS_UNSUCCESSFUL;

	if (ByteOffset == NULL)
	{
		if (!(FileObject->Flags & FO_SYNCHRONOUS_IO))
			return STATUS_INVALID_PARAMETER;
		ByteOffset = &FileObject->CurrentByteOffset;
	}

	Irp = IoAllocateIrp(FileObject->Vpb->DeviceObject->StackSize, FALSE);
	if (Irp == NULL) return STATUS_INSUFFICIENT_RESOURCES;

	if(FileObject->DeviceObject->Flags & DO_BUFFERED_IO)
	{
		Irp->AssociatedIrp.SystemBuffer = Buffer;
	}
	else
	{
		Irp->MdlAddress = IoAllocateMdl(Buffer, Length, 0, 0, 0);
		if (Irp->MdlAddress == NULL)
		{
			IoFreeIrp(Irp);
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		MmBuildMdlForNonPagedPool(Irp->MdlAddress);
	}

	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	Irp->UserEvent = &kEvent;
	Irp->UserIosb = IoStatusBlock;
	Irp->RequestorMode = KernelMode;
	Irp->Flags = IRP_WRITE_OPERATION;
	Irp->Tail.Overlay.Thread = PsGetCurrentThread();
	Irp->Tail.Overlay.OriginalFileObject = FileObject;

	IrpSp = IoGetNextIrpStackLocation(Irp);
	IrpSp->MajorFunction = IRP_MJ_WRITE;
	IrpSp->MinorFunction = IRP_MN_NORMAL;
	IrpSp->DeviceObject = FileObject->Vpb->DeviceObject;
	IrpSp->FileObject = FileObject;
	IrpSp->Parameters.Write.Length = Length;
	IrpSp->Parameters.Write.ByteOffset = *ByteOffset;

	IoSetCompletionRoutine(Irp, IoCompletionRoutine, NULL, TRUE, TRUE, TRUE);
	ntStatus = IoCallDriver(FileObject->Vpb->DeviceObject, Irp);

	if (ntStatus == STATUS_PENDING)
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, NULL);

	return IoStatusBlock->Status;
} 





