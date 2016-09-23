#include "File.h"

#define ObjectNameInformation 1

#define MAX_PATH  260


BOOLEAN DosPathToNtPathW(WCHAR* wzFullDosPath,WCHAR* wzFullNtPath)
{
	WCHAR  wzDosDevice[4] = {0};
	WCHAR  wzNtDevice[64] = {0};
	size_t NtDeviceLen   = 0;
	if (!wzFullNtPath||!wzFullDosPath)
	{
		return FALSE;
	}
	wzDosDevice[0] = wzFullDosPath[0];
	wzDosDevice[1] = L':';

	if (NtQueryDosDevice(wzDosDevice,wzNtDevice,64))
	{
		if (wzNtDevice)
		{
			wcscpy(wzFullNtPath,wzNtDevice);
			wcscat(wzFullNtPath,wzFullDosPath+2);

			return TRUE;
		}
	}

	return FALSE;

}




ULONG NtQueryDosDevice(WCHAR* wzDosDevice,WCHAR* wzNtDevice,ULONG ucchMax)
{
	NTSTATUS Status;
	POBJECT_DIRECTORY_INFORMATION ObjectDirectoryInfor;
	OBJECT_ATTRIBUTES oa;
	UNICODE_STRING uniString;
	HANDLE hDirectory;
	HANDLE hDevice;
	ULONG  ulReturnLength;
	ULONG  ulNameLength;
	ULONG  ulLength;
	ULONG       Context;
	BOOLEAN     bRestartScan;
	WCHAR*      Ptr = NULL;
	UCHAR       szBuffer[512] = {0};

	RtlInitUnicodeString (&uniString,L"\\??");
	InitializeObjectAttributes(&oa,
		&uniString,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL); 
	Status = ZwOpenDirectoryObject(&hDirectory,DIRECTORY_QUERY,&oa);
	if(!NT_SUCCESS(Status))
	{
		return 0;
	}
	ulLength = 0;
	if (wzDosDevice != NULL)
	{
		RtlInitUnicodeString (&uniString,(PWSTR)wzDosDevice);
		InitializeObjectAttributes(&oa,&uniString,OBJ_CASE_INSENSITIVE,hDirectory,NULL);
		Status = ZwOpenSymbolicLinkObject(&hDevice,GENERIC_READ,&oa);
		if(!NT_SUCCESS (Status))
		{
			ZwClose(hDirectory);
			return 0;
		}
		uniString.Length = 0;
		uniString.MaximumLength = (USHORT)ucchMax * sizeof(WCHAR);
		uniString.Buffer = wzNtDevice;
		ulReturnLength = 0;
		Status = ZwQuerySymbolicLinkObject (hDevice,&uniString,&ulReturnLength);
		ZwClose(hDevice);
		ZwClose(hDirectory);
		if (!NT_SUCCESS (Status))
		{
			return 0;
		}
		ulLength = uniString.Length / sizeof(WCHAR);
		if (ulLength < ucchMax)
		{
			wzNtDevice[ulLength] = UNICODE_NULL;
			ulLength++;
		}
		else
		{
			
			return 0;
		}
	}
	else
	{
		bRestartScan = TRUE;
		Context = 0;
		Ptr = wzNtDevice;
		ObjectDirectoryInfor = (POBJECT_DIRECTORY_INFORMATION)szBuffer;
		while (TRUE)
		{
			Status = ZwQueryDirectoryObject(hDirectory,szBuffer,sizeof (szBuffer),TRUE,bRestartScan,&Context,&ulReturnLength);
			if(!NT_SUCCESS(Status))
			{
				if (Status == STATUS_NO_MORE_ENTRIES)
				{
					*Ptr = UNICODE_NULL;
					ulLength++;
					Status = STATUS_SUCCESS;
				}
				else
				{
					ulLength = 0;
				}
				break;
			}
			if (!wcscmp (ObjectDirectoryInfor->TypeName.Buffer, L"SymbolicLink"))
			{
				ulNameLength = ObjectDirectoryInfor->Name.Length / sizeof(WCHAR);
				if (ulLength + ulNameLength + 1 >= ucchMax)
				{
					ulLength = 0;
					break;
				}
				memcpy(Ptr,ObjectDirectoryInfor->Name.Buffer,ObjectDirectoryInfor->Name.Length);
				Ptr += ulNameLength;
				ulLength += ulNameLength;
				*Ptr = UNICODE_NULL;
				Ptr++;
				ulLength++;
			}
			bRestartScan = FALSE;
		}
		ZwClose(hDirectory);
	}
	return ulLength;
}


NTSTATUS  ForceDeleteProcessHandle(WCHAR* wzFileFullPath)
{
	NTSTATUS						 Status;
	PSYSTEM_HANDLE_INFOR             SystemHandleInfor = NULL;
	PSYSTEM_HANDLE_ENTRY_INFOR SystemHandleTableEntryInfor = NULL;
	ULONG ulLength = 0x1000;
	int   i = 0;
	OBJECT_ATTRIBUTES  oa;
	CLIENT_ID          Cid;
	KIRQL			   OldIrql;
	HANDLE             hCurrentProcess = NULL;
	HANDLE             hTargetProcess = NULL;
	HANDLE             hCurrentHandle  = NULL;
	char               szBuffer[260] = {0};
	PETHREAD           EThread = NULL;
	CHAR               PreMode;
	UNICODE_STRING     uniString;

	UNICODE_STRING  uniFileFullPath;
	POBJECT_NAME_INFORMATION  NameInfor;

	EThread = PsGetCurrentThread();
	PreMode = ChangePreMode(EThread);

	NameInfor = (POBJECT_NAME_INFORMATION)szBuffer;

	RtlInitUnicodeString(&uniFileFullPath,wzFileFullPath);

	Cid.UniqueProcess = PsGetCurrentProcessId(); 
	Cid.UniqueThread = 0;

	InitializeObjectAttributes(&oa,NULL,OBJ_KERNEL_HANDLE,NULL,NULL);

	Status = NtOpenProcess(&hCurrentProcess,PROCESS_ALL_ACCESS,&oa,&Cid);  

	if (!NT_SUCCESS(Status))
	{
		return Status;
	}

	SystemHandleInfor = ExAllocatePool(PagedPool,ulLength);

	if (SystemHandleInfor==NULL)
	{
		ZwClose(hCurrentProcess);

		return STATUS_INSUFFICIENT_RESOURCES;
	}


	Status = ZwQuerySystemInformation(SystemHandleInformation,SystemHandleInfor,
		ulLength,&ulLength);

	if (Status==STATUS_INFO_LENGTH_MISMATCH)
	{
		ExFreePool(SystemHandleInfor);


		SystemHandleInfor = ExAllocatePool(PagedPool,ulLength);


		if (SystemHandleInfor==NULL)
		{
			ZwClose(hCurrentProcess);

			return STATUS_INSUFFICIENT_RESOURCES;
		}


		Status = ZwQuerySystemInformation(SystemHandleInformation,SystemHandleInfor,
			ulLength,&ulLength);
	}

	if (!NT_SUCCESS(Status))
	{
		ZwClose(hCurrentProcess);

		ExFreePool(SystemHandleInfor);

		return Status;
	}


	for (i=0;i<SystemHandleInfor->NumberOfHandles;i++)
	{
		SystemHandleTableEntryInfor = &SystemHandleInfor->Handles[i];

		if (SystemHandleTableEntryInfor->ObjectTypeIndex == OB_TYPE_PROCESS)  //进程对象
		{
			PEPROCESS EProcess;
			WCHAR     wzFIleName[260] = {0};
			CHAR      szFIleName[260] = {0};

			PsLookupProcessByProcessId((HANDLE)SystemHandleTableEntryInfor->UniqueProcessId,&EProcess);
			strcpy(szFIleName,(const char*)(PsGetProcessImageFileName(EProcess)));
			mbtowc(wzFIleName,szFIleName,strlen(szFIleName));

			if (wcscmp(wzFIleName,uniFileFullPath.Buffer)==0)
			{
				KillProcessByProcessId(SystemHandleTableEntryInfor->UniqueProcessId);
		   }
	}

	ZwClose(hCurrentProcess);
	RecoverPreMode(EThread, PreMode);
	ExFreePool(SystemHandleInfor);

	return Status;
 }
}

BOOLEAN GetPathByFileObject(PFILE_OBJECT FileObject, WCHAR* wzPath)
{
	BOOLEAN bGetPath = FALSE;

	POBJECT_NAME_INFORMATION ObjectNameInfor = NULL;
	__try
	{
		if (FileObject && MmIsAddressValid(FileObject) && wzPath)
		{

			if (NT_SUCCESS(IoQueryFileDosDeviceName(FileObject,&ObjectNameInfor)))   //注意函数调用后要释放内存
			{
				wcsncpy(wzPath,ObjectNameInfor->Name.Buffer,ObjectNameInfor->Name.Length);

				bGetPath = TRUE;

				ExFreePool(ObjectNameInfor);
			}


			if (!bGetPath)
			{
				if (IoVolumeDeviceToDosName||RtlVolumeDeviceToDosName)
				{
					NTSTATUS  Status = STATUS_UNSUCCESSFUL;

					ULONG_PTR ulRet= 0;

					PVOID     Buffer = ExAllocatePool(PagedPool,0x1000);

					if (Buffer)
					{
						// ObQueryNameString :C:\Program Files\VMware\VMware Tools\VMwareTray.exe
						memset(Buffer, 0, 0x1000);
						Status = ObQueryNameString(FileObject, (POBJECT_NAME_INFORMATION)Buffer, 0x1000, &ulRet);
						if (NT_SUCCESS(Status))
						{
							POBJECT_NAME_INFORMATION Temp = (POBJECT_NAME_INFORMATION)Buffer;

							WCHAR szHarddiskVolume[100] = L"\\Device\\HarddiskVolume";

							if (Temp->Name.Buffer!=NULL)
							{
								if (Temp->Name.Length / sizeof(WCHAR) > wcslen(szHarddiskVolume) &&
									!_wcsnicmp(Temp->Name.Buffer, szHarddiskVolume, wcslen(szHarddiskVolume)))
								{
									// 如果是以"\\Device\\HarddiskVolume" 这样的形式存在的 那么再查询其卷名
									UNICODE_STRING uniDosName;

									if (NT_SUCCESS(IoVolumeDeviceToDosName(FileObject->DeviceObject, &uniDosName)))
									{
										if (uniDosName.Buffer!=NULL)
										{

											wcsncpy(wzPath, uniDosName.Buffer, uniDosName.Length);
											wcsncat(wzPath, Temp->Name.Buffer + wcslen(szHarddiskVolume) + 1, Temp->Name.Length - (wcslen(szHarddiskVolume) + 1));
											bGetPath = TRUE;
										}	

										ExFreePool(uniDosName.Buffer);
									}

									else if (NT_SUCCESS(RtlVolumeDeviceToDosName(FileObject->DeviceObject, &uniDosName)))
									{
										if (uniDosName.Buffer!=NULL)
										{

											wcsncpy(wzPath, uniDosName.Buffer, uniDosName.Length);
											wcsncat(wzPath, Temp->Name.Buffer + wcslen(szHarddiskVolume) + 1, Temp->Name.Length - (wcslen(szHarddiskVolume) + 1));
											bGetPath = TRUE;
										}	

										ExFreePool(uniDosName.Buffer);
									}

								}
								else
								{
									// 如果不是以 "\\Device\\HarddiskVolume" 这样的形式开头的，那么直接复制名称

									wcsncpy(wzPath, Temp->Name.Buffer, Temp->Name.Length);
									bGetPath = TRUE;
								}
							}
						}

						ExFreePool(Buffer);
					}
				}
			}
		}
	}
	__except(1)
	{
		DbgPrint("GetPathByFileObject Catch __Except\r\n");
		bGetPath = FALSE;
	}

	return bGetPath;
}
