#include "KernelFilterDriver.h"

NTSTATUS kernel_filterDriverEnumFunc(PKERNEL_FILTERDRIVER KernelFilterDriver,UNICODE_STRING ustrDirPath,ULONG ulDirType)
{
	HANDLE hDriver;
	NTSTATUS status;
	ULONG ulContext,ulRet=0;
	ULONG ulSize = 0x1000;
	OBJECT_ATTRIBUTES objectAttribute;
	PDEVICE_OBJECT pDeviceObject;
	PDRIVER_OBJECT pDriverObject;
	PDEVICE_OBJECT pAttachObject;
	UNICODE_STRING ustrDriverName;
	PDIRECTORY_BASIC_INFORMATION pDirectoryBasicInformation = NULL,pDirectoryBasicInformation_tmp;
	PLDR_DATA_TABLE_ENTRY pLdr;
	WCHAR DriverName[260]={0};
	ULONG ulNameLength;
	KIRQL oldIrql = NULL; 
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwOpenDirectoryObject,L"ZwOpenDirectoryObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwQueryDirectoryObject,L"ZwQueryDirectoryObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByName,L"ObReferenceObjectByName",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenDirectoryObject &&
		RExFreePool &&
		RExAllocatePool &&
		RZwQueryDirectoryObject &&
		RObReferenceObjectByName &&
		RRtlInitUnicodeString)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	__try
	{
		//初始化结构体
		InitializeObjectAttributes(
			&objectAttribute,
			&ustrDirPath,
			OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,
			NULL,
			NULL
			);
		//打开驱动目录对象
		status=RZwOpenDirectoryObject(
			&hDriver,
			DIRECTORY_QUERY,
			&objectAttribute
			);
		//如果成功
		if (!NT_SUCCESS(status))
		{
			KdPrint(("kernel_filterDriverEnumFunc ZwOpenDirectoryObject error!"));
			__leave;
		}
		do 
		{
			if (pDirectoryBasicInformation)
				RExFreePool(pDirectoryBasicInformation);

			//两倍大小扩大
			ulSize = 2*ulSize;
			//分配
			pDirectoryBasicInformation = NULL;
			pDirectoryBasicInformation = (PDIRECTORY_BASIC_INFORMATION)RExAllocatePool(NonPagedPool,ulSize);
			//如果分配内存失败
			if (!pDirectoryBasicInformation)
			{
				KdPrint(("kernel_filterDriverEnumFunc pDirectoryBasicInformation error!"));
				__leave;
			}
			status = RZwQueryDirectoryObject(
				hDriver,
				pDirectoryBasicInformation,
				ulSize,
				FALSE,
				TRUE,
				&ulContext,
				&ulRet
				);

		} while (status == STATUS_BUFFER_TOO_SMALL || status == STATUS_MORE_ENTRIES);

		//如果没有成功
		if (!NT_SUCCESS(status))
		{
			KdPrint(("kernel_filterDriverEnumFunc ZwQueryDirectoryObject error!"));
			__leave;
		}
		pDirectoryBasicInformation_tmp = pDirectoryBasicInformation;

		while (pDirectoryBasicInformation_tmp->ObjectName.Length!=0 && pDirectoryBasicInformation_tmp->ObjectTypeName.Length!=0 )
		{
			__try
			{
				if (ulDirType==0)
				{
					wcscpy(DriverName,L"\\Driver\\");
					wcsncat(DriverName,pDirectoryBasicInformation_tmp->ObjectName.Buffer,pDirectoryBasicInformation_tmp->ObjectName.Length);
					RRtlInitUnicodeString(&ustrDriverName,DriverName);
				}
				else
				{
					wcscpy(DriverName,L"\\FileSystem\\");
					wcsncat(DriverName,pDirectoryBasicInformation_tmp->ObjectName.Buffer,pDirectoryBasicInformation_tmp->ObjectName.Length);
					RRtlInitUnicodeString(&ustrDriverName,DriverName);
				}

				//引用计数增一
				status = RObReferenceObjectByName(
					&ustrDriverName,
					OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,
					NULL,
					0,
					*IoDriverObjectType,
					KernelMode,
					NULL,
					(PVOID *)&pDriverObject
					);
				//如果引用失败
				if (!NT_SUCCESS(status))
				{
					pDirectoryBasicInformation_tmp++;
					//KdPrint(("ObReferenceObjectByName failed:%d",RtlNtStatusToDosError(status)));
					continue;
				}
				//引用计数减一
				ObDereferenceObject(pDriverObject);

				if (!MmIsAddressValidEx(pDriverObject) ||
					!MmIsAddressValidEx(pDriverObject->DeviceObject))
				{
					//KdPrint(("pDriverObject failed"));
					pDirectoryBasicInformation_tmp++;
					continue;
				}

				pDeviceObject = pDriverObject->DeviceObject;

				while (pDeviceObject)
				{
					// 判断当前设备上是否有过滤驱动
					if (pDeviceObject->AttachedDevice)
					{
						//附加设备对象
						pAttachObject = pDeviceObject->AttachedDevice;
						//遍历设备栈
						while (pAttachObject)
						{
							switch (pAttachObject->DeviceType)
							{
							case FILE_DEVICE_DISK:
							case FILE_DEVICE_DISK_FILE_SYSTEM:
							case FILE_DEVICE_FILE_SYSTEM:
							case FILE_DEVICE_KEYBOARD:
							case FILE_DEVICE_MOUSE:
							case FILE_DEVICE_NETWORK:
							case FILE_DEVICE_8042_PORT:
							case FILE_DEVICE_DFS_VOLUME:
							case FILE_DEVICE_DFS_FILE_SYSTEM:
							case FILE_DEVICE_UNKNOWN:	

								//设备类型
								KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].ulObjType = pAttachObject->DeviceType;
								//附加设备
								KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].ulAttachDevice = (ULONG)pAttachObject;

								//驱动名称
								if (ValidateUnicodeString(&pAttachObject->DriverObject->DriverName))
								{
									memset(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FileName,0,sizeof(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FileName));
									SafeCopyMemory(pAttachObject->DriverObject->DriverName.Buffer,
										KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FileName,
										pAttachObject->DriverObject->DriverName.Length
										);
								}

								//宿主驱动名称
								if (ValidateUnicodeString(&pAttachObject->DriverObject->DriverName))
								{
									memset(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].HostFileName,0,sizeof(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].HostFileName));
									SafeCopyMemory(pDriverObject->DriverName.Buffer,
										KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].HostFileName,
										pDriverObject->DriverName.Length
										);
								}
								//驱动路径
								pLdr=(PLDR_DATA_TABLE_ENTRY)(pAttachObject->DriverObject->DriverSection);
								if (pLdr!=NULL && pLdr->FullDllName.Buffer!=NULL)
								{
									if (ValidateUnicodeString(&pLdr->FullDllName))
									{
										//路径
										memset(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FilePath,0,sizeof(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FilePath));
										SafeCopyMemory(pLdr->FullDllName.Buffer,
											KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FilePath,
											pLdr->FullDllName.Length
											);
									}
								}
								else
								{
									if (ValidateUnicodeString(&pAttachObject->DriverObject->DriverName))
									{
										memset(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FilePath,0,sizeof(KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FilePath));
										SafeCopyMemory(pAttachObject->DriverObject->DriverName.Buffer,
											KernelFilterDriver->KernelFilterDriverInfo[IntFilterCount].FilePath,
											pAttachObject->DriverObject->DriverName.Length
											);
									}
								}
								IntFilterCount++;
								KernelFilterDriver->ulCount = IntFilterCount;
								break;
							}
							//指向上一个附加设备
							pAttachObject = pAttachObject->AttachedDevice;
						}
					}
					// 得到建立在此驱动上的下一个设备 DEVICE_OBJECT 
					pDeviceObject = pDeviceObject->NextDevice;
				}
A1:
				//指针移动到下一个结构体
				pDirectoryBasicInformation_tmp++;
				if (!MmIsAddressValidEx(pDirectoryBasicInformation_tmp))
				{
					goto A1;
				}

			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				//指针移动到下一个结构体
A2:
				pDirectoryBasicInformation_tmp++;
				if (!MmIsAddressValidEx(pDirectoryBasicInformation_tmp))
				{
					goto A2;
				}
			}
		}
		if (pDirectoryBasicInformation)
			RExFreePool(pDirectoryBasicInformation);

		pDirectoryBasicInformation = NULL;

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){

		if (pDirectoryBasicInformation)
			RExFreePool(pDirectoryBasicInformation);

		return STATUS_UNSUCCESSFUL;
	}
	if (pDirectoryBasicInformation)
		RExFreePool(pDirectoryBasicInformation);

	pDirectoryBasicInformation = NULL;

	return STATUS_UNSUCCESSFUL;
}

//枚举过滤驱动
NTSTATUS KernelFilterDriverEnum(PKERNEL_FILTERDRIVER KernelFilterDriver)
{
	NTSTATUS status;
	UNICODE_STRING ustrDirPath;
	int i = 0;
	BOOL bInit = FALSE;
	
	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	RRtlInitUnicodeString(&ustrDirPath,L"\\FileSystem");

	IntFilterCount = 0;

	status = kernel_filterDriverEnumFunc(KernelFilterDriver,ustrDirPath,1);

	RRtlInitUnicodeString(&ustrDirPath,L"\\Driver");
	status = kernel_filterDriverEnumFunc(KernelFilterDriver,ustrDirPath,0);	
	
	return status;
}
//参数：1:\\FileSystem\\Ntfs
//      2:设备对象
VOID ClearFilters(PWSTR lpwName,ULONG ulDeviceObject)
{
	UNICODE_STRING TName;
	PDRIVER_OBJECT TDrvObj;
	PDEVICE_OBJECT CurrentDevice;
	NTSTATUS status;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByName,L"ObReferenceObjectByName",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RObReferenceObjectByName)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	RRtlInitUnicodeString(&TName, lpwName);
	status = RObReferenceObjectByName(&TName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		0,
		*IoDriverObjectType,
		KernelMode,                   
		NULL,
		&TDrvObj);

	if (!NT_SUCCESS(status)) return ;
	if(!TDrvObj) return ;

	CurrentDevice = TDrvObj->DeviceObject;

	while(CurrentDevice != NULL )
	{
		if (CurrentDevice->AttachedDevice == ulDeviceObject)
		{
			if (DebugOn)
				KdPrint(("AttachedDevice:%08x  NextDevice:%08x",CurrentDevice->AttachedDevice,CurrentDevice->NextDevice));

			CurrentDevice->AttachedDevice = NULL;
			CurrentDevice = CurrentDevice->NextDevice;
			break;
		}
		CurrentDevice = CurrentDevice->NextDevice;
	}
	ObDereferenceObject(TDrvObj);
	return;
}