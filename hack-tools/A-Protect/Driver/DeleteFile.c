#include "DeleteFile.h"

HANDLE
SkillIoOpenFile(
				IN PCWSTR FileName,
				IN ACCESS_MASK DesiredAccess,
				IN ULONG ShareAccess
				)
{
	NTSTATUS              ntStatus;
	UNICODE_STRING        uniFileName;
	OBJECT_ATTRIBUTES     objectAttributes;
	HANDLE                ntFileHandle;
	IO_STATUS_BLOCK       ioStatus;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCreateFile,L"IoCreateFile",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RIoCreateFile)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return 0;

	RRtlInitUnicodeString(&uniFileName, FileName);

	InitializeObjectAttributes(&objectAttributes, &uniFileName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	ntStatus = RIoCreateFile(&ntFileHandle,
		DesiredAccess,
		&objectAttributes,
		&ioStatus,
		0,
		FILE_ATTRIBUTE_NORMAL,
		ShareAccess,
		FILE_OPEN,
		0,
		NULL,
		0,
		0,
		NULL,
		IO_NO_PARAMETER_CHECKING);

	if (!NT_SUCCESS(ntStatus))
	{
		return 0;
	}

	return ntFileHandle;
}

NTSTATUS
SkillSetFileCompletion(
					   IN PDEVICE_OBJECT DeviceObject,
					   IN PIRP Irp,
					   IN PVOID Context
					   )
{
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RKeSetEvent,L"KeSetEvent",SystemKernelModuleBase,ImageModuleBase);
	if (RKeSetEvent)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	Irp->UserIosb->Status = Irp->IoStatus.Status;
	Irp->UserIosb->Information = Irp->IoStatus.Information;

	RKeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, FALSE);

	IoFreeIrp(Irp);

	return STATUS_MORE_PROCESSING_REQUIRED;
}

BOOLEAN
SKillStripFileAttributes(
						 IN HANDLE    FileHandle
						 )
{
	NTSTATUS          ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT      fileObject;
	PDEVICE_OBJECT    DeviceObject;
	PIRP              Irp;
	KEVENT            event;
	FILE_BASIC_INFORMATION    FileInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInitializeEvent,L"KeInitializeEvent",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoAllocateIrp,L"IoAllocateIrp",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCallDriver,L"IoCallDriver",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeWaitForSingleObject,L"KeWaitForSingleObject",SystemKernelModuleBase,ImageModuleBase);
	if (RObReferenceObjectByHandle &&
		RKeInitializeEvent &&
		RIoAllocateIrp &&
		RIoCallDriver &&
		RKeWaitForSingleObject)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	ntStatus = RObReferenceObjectByHandle(FileHandle,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&fileObject,
		NULL);

	if (!NT_SUCCESS(ntStatus))
	{
		return FALSE;
	}

	DeviceObject = IoGetRelatedDeviceObject(fileObject);
	Irp = RIoAllocateIrp(DeviceObject->StackSize, TRUE);

	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}

	RKeInitializeEvent(&event, SynchronizationEvent, FALSE);

	memset(&FileInformation,0,0x28);

	FileInformation.FileAttributes = FILE_ATTRIBUTE_NORMAL;
	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &event;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;

	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_BASIC_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileBasicInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;

	IoSetCompletionRoutine(
		Irp,
		SkillSetFileCompletion,
		&event,
		TRUE,
		TRUE,
		TRUE);

	RIoCallDriver(DeviceObject, Irp);

	RKeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	ObDereferenceObject(fileObject);

	return TRUE;
}


BOOLEAN
SKillDeleteFile(
				IN HANDLE    FileHandle
				)
{
	NTSTATUS          ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT      fileObject;
	PDEVICE_OBJECT    DeviceObject;
	PIRP              Irp;
	KEVENT            event;
	FILE_DISPOSITION_INFORMATION    FileInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PSECTION_OBJECT_POINTERS pSectionObjectPointer;     ////////////////////
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInitializeEvent,L"KeInitializeEvent",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoAllocateIrp,L"IoAllocateIrp",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCallDriver,L"IoCallDriver",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeWaitForSingleObject,L"KeWaitForSingleObject",SystemKernelModuleBase,ImageModuleBase);
	if (RObReferenceObjectByHandle &&
		RKeInitializeEvent &&
		RIoAllocateIrp &&
		RIoCallDriver &&
		RKeWaitForSingleObject)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	SKillStripFileAttributes( FileHandle);          //去掉只读属性，才能删除只读文件

	ntStatus = RObReferenceObjectByHandle(FileHandle,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&fileObject,
		NULL);

	if (!NT_SUCCESS(ntStatus))
	{
		return FALSE;
	}

	DeviceObject = IoGetRelatedDeviceObject(fileObject);
	Irp = RIoAllocateIrp(DeviceObject->StackSize, TRUE);
	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}

	RKeInitializeEvent(&event, SynchronizationEvent, FALSE);

	FileInformation.DeleteFile = TRUE;

	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &event;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;

	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_DISPOSITION_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileDispositionInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;


	IoSetCompletionRoutine(
		Irp,
		SkillSetFileCompletion,
		&event,
		TRUE,
		TRUE,
		TRUE);

	//再加上下面这三行代码 ，MmFlushImageSection    函数通过这个结构来检查是否可以删除文件。
	//这个和Hook
	ulImageSectionObject = NULL;
	ulDataSectionObject = NULL;
	ulSharedCacheMap = NULL;

	pSectionObjectPointer = NULL;
	pSectionObjectPointer = fileObject->SectionObjectPointer;
	if (MmIsAddressValidEx(pSectionObjectPointer))
	{
		ulImageSectionObject = pSectionObjectPointer->ImageSectionObject; // 备份之~~~
		pSectionObjectPointer->ImageSectionObject = 0; //清零，准备删除

		ulDataSectionObject = pSectionObjectPointer->DataSectionObject;  //备份之
		pSectionObjectPointer->DataSectionObject = 0;       //清零，准备删除

		ulSharedCacheMap = pSectionObjectPointer->SharedCacheMap;
		pSectionObjectPointer->SharedCacheMap = 0;
	}

	//发irp删除
	RIoCallDriver(DeviceObject, Irp);

	//等待操作完毕
	RKeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);

	//删除文件之后，从备份那里填充回来
	pSectionObjectPointer = NULL;
	pSectionObjectPointer = fileObject->SectionObjectPointer;

	if (MmIsAddressValidEx(pSectionObjectPointer))
	{
		if (ulImageSectionObject)
			pSectionObjectPointer->ImageSectionObject = ulImageSectionObject; //填充回来，不然蓝屏哦

		if (ulDataSectionObject)
			pSectionObjectPointer->DataSectionObject = ulDataSectionObject;

		if (ulSharedCacheMap)
			pSectionObjectPointer->SharedCacheMap = ulSharedCacheMap;
	}

	ObDereferenceObject(fileObject);
	fileObject = NULL;

	return TRUE;
}