#include "Services.h"



NTSTATUS QueryServicesRegistry(PSERVICESREGISTRY ServicesRegistry)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	WCHAR InterfacesPath[] = L"\\Parameters\\";
	WCHAR DriverServiceNamePath[] = L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\";
	OBJECT_ATTRIBUTES NetworkClassKeyObject;
	OBJECT_ATTRIBUTES SubKeyObject;
	HANDLE NetworkClassKeyHandle;
	HANDLE SubKeyHandle;
	ULONG i, SubkeyIndex, ResultLength, InterfacesKeyStringLength;
	PWCHAR InterfacesKeyString;
	UNICODE_STRING NetworkClassKey, InterfacesKey, ServiceDll, DriverServiceName, ImagePath;
	PKEY_BASIC_INFORMATION KeyInformation;
	PKEY_VALUE_PARTIAL_INFORMATION KeyValueInformation;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwOpenKey,L"ZwOpenKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);

	ReLoadNtosCALL(&RZwEnumerateKey,L"ZwEnumerateKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwQueryValueKey,L"ZwQueryValueKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RZwOpenKey &&
		RExAllocatePool &&
		RExFreePool &&
		RZwEnumerateKey &&
		RZwQueryValueKey &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
	{
		return Status;
	}
	if (DebugOn)
		KdPrint(("Starting"));

	RRtlInitUnicodeString(&ServiceDll, L"ServiceDll");
	RRtlInitUnicodeString(&ImagePath, L"ImagePath");
	RRtlInitUnicodeString(&NetworkClassKey,DriverServiceNamePath);

	InitializeObjectAttributes(
		&NetworkClassKeyObject, 
		&NetworkClassKey, 
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
		NULL, 
		NULL
		);
	Status = RZwOpenKey(
		&NetworkClassKeyHandle, 
		KEY_ALL_ACCESS, 
		&NetworkClassKeyObject
		);
	if (!NT_SUCCESS(Status))
	{
		if (DebugOn)
			DbgPrint("Failed to open Network Class key\n");

		return Status;
	}
	if (DebugOn)
		KdPrint(("ZwOpenKey success"));

	SubkeyIndex = 0;
	i = 0;

	while ((Status = RZwEnumerateKey(NetworkClassKeyHandle, SubkeyIndex, KeyBasicInformation, NULL, 0, &ResultLength)) != STATUS_NO_MORE_ENTRIES) 
	{
		if ((Status != STATUS_SUCCESS) && (Status != STATUS_BUFFER_OVERFLOW) && (Status != STATUS_BUFFER_TOO_SMALL))
		{
			if (DebugOn)
				DbgPrint("ZwEnumerateKey 1 failed in SetupRegistry (%lx)\n", Status);
			Status = STATUS_UNSUCCESSFUL;
			break;
		}
		if ((KeyInformation = (PKEY_BASIC_INFORMATION)RExAllocatePool(NonPagedPool, ResultLength)) == NULL)
		{
			if (DebugOn)
				DbgPrint("ExAllocatePool KeyData failed in SetupRegistry\n");
			Status = STATUS_UNSUCCESSFUL;
			break;
		}
		memset(KeyInformation,0,ResultLength);
		Status = RZwEnumerateKey(
			NetworkClassKeyHandle,
			SubkeyIndex, 
			KeyBasicInformation, 
			KeyInformation, 
			ResultLength, 
			&ResultLength
			);
		if (!NT_SUCCESS(Status))
		{
			if (DebugOn)
				DbgPrint("ZwEnumerateKey 2 failed in SetupRegistry\n");

			Status = STATUS_UNSUCCESSFUL;
			RExFreePool(KeyInformation);
			break;
		}
		//键值
		//DbgPrint("KeyInformation:%ws\n",KeyInformation->Name);
		memset(ServicesRegistry->SrvReg[i].lpwzSrvName,0,sizeof(ServicesRegistry->SrvReg[i].lpwzSrvName));
		SafeCopyMemory(KeyInformation->Name,ServicesRegistry->SrvReg[i].lpwzSrvName,KeyInformation->NameLength);

		if (DebugOn)
			KdPrint(("RZwEnumerateKey success:%ws",ServicesRegistry->SrvReg[i].lpwzSrvName));

		//读取 ImagePath
		RRtlInitUnicodeString(&DriverServiceName,ServicesRegistry->SrvReg[i].lpwzSrvName);

		InitializeObjectAttributes(
			&SubKeyObject, 
			&DriverServiceName, 
			OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
			NetworkClassKeyHandle, 
			NULL
			);
		Status = RZwOpenKey(
			&SubKeyHandle, 
			KEY_ALL_ACCESS, 
			&SubKeyObject
			);
		if (NT_SUCCESS(Status)) 
		{
			if ((Status = RZwQueryValueKey(SubKeyHandle, &ImagePath, KeyValuePartialInformation, NULL, 0, &ResultLength)) != STATUS_OBJECT_NAME_NOT_FOUND)
			{
				if ((Status != STATUS_SUCCESS) && (Status != STATUS_BUFFER_OVERFLOW) && (Status != STATUS_BUFFER_TOO_SMALL))
				{
					RZwClose(SubKeyHandle);
					RExFreePool(KeyInformation);
					break;
				}
				KeyValueInformation = (PKEY_VALUE_PARTIAL_INFORMATION)RExAllocatePool(NonPagedPool, ResultLength);
				if (KeyValueInformation)
				{
					memset(KeyValueInformation,0,ResultLength);
					Status = RZwQueryValueKey(
						SubKeyHandle, 
						&ImagePath, 
						KeyValuePartialInformation,
						KeyValueInformation,
						ResultLength, 
						&ResultLength
						);
					if (NT_SUCCESS(Status))
					{
						//DbgPrint("ImagePath:%ws\n",KeyValueInformation->Data);
						memset(ServicesRegistry->SrvReg[i].lpwzImageName,0,sizeof(ServicesRegistry->SrvReg[i].lpwzImageName));
						if (MmIsAddressValidEx(KeyValueInformation->Data) &&
							KeyValueInformation->DataLength > 0)
						{
							SafeCopyMemory(KeyValueInformation->Data,ServicesRegistry->SrvReg[i].lpwzImageName,KeyValueInformation->DataLength);
							if (DebugOn)
								KdPrint(("lpwzImageName success:%ws",ServicesRegistry->SrvReg[i].lpwzImageName));
						}
					}
					RExFreePool(KeyValueInformation);
				}

			}
			RZwClose(SubKeyHandle);
		}
		//开始读取Parameters
		InterfacesKeyStringLength = KeyInformation->NameLength + sizeof(InterfacesPath);
		InterfacesKeyString = (PWCHAR)RExAllocatePool(NonPagedPool, InterfacesKeyStringLength);
		if (!InterfacesKeyString)
		{
			Status = STATUS_UNSUCCESSFUL;
			RExFreePool(KeyInformation);
			break;
		}
		memset(InterfacesKeyString,0,InterfacesKeyStringLength);
		memcpy(InterfacesKeyString, KeyInformation->Name, KeyInformation->NameLength);
		memcpy(&InterfacesKeyString[(KeyInformation->NameLength / sizeof(WCHAR))], InterfacesPath, sizeof(InterfacesPath));

		RRtlInitUnicodeString(&InterfacesKey, InterfacesKeyString);
		//Parameters
		//DbgPrint("KeyInformation:%ws\n",InterfacesKeyString);

		//如果打开成功，说明是svchost启动，则读取servicedll
		InitializeObjectAttributes(
			&SubKeyObject, 
			&InterfacesKey, 
			OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
			NetworkClassKeyHandle, 
			NULL
			);
		Status = RZwOpenKey(
			&SubKeyHandle, 
			KEY_ALL_ACCESS, 
			&SubKeyObject
			);
		if (NT_SUCCESS(Status)) 
		{
			if ((Status = RZwQueryValueKey(SubKeyHandle, &ServiceDll, KeyValuePartialInformation, NULL, 0, &ResultLength)) != STATUS_OBJECT_NAME_NOT_FOUND)
			{
				if ((Status != STATUS_SUCCESS) && (Status != STATUS_BUFFER_OVERFLOW) && (Status != STATUS_BUFFER_TOO_SMALL))
				{
					RZwClose(SubKeyHandle);
					RExFreePool(InterfacesKeyString);
					RExFreePool(KeyInformation);
					break;
				}
				KeyValueInformation = (PKEY_VALUE_PARTIAL_INFORMATION)RExAllocatePool(NonPagedPool, ResultLength);
				if (KeyValueInformation)
				{
					memset(KeyValueInformation,0,ResultLength);
					Status = RZwQueryValueKey(
						SubKeyHandle, 
						&ServiceDll, 
						KeyValuePartialInformation,
						KeyValueInformation,
						ResultLength, 
						&ResultLength
						);
					if (NT_SUCCESS(Status))
					{
						//DbgPrint("KeyValueInformation:%ws\n",KeyValueInformation->Data);
						memset(ServicesRegistry->SrvReg[i].lpwzDLLPath,0,sizeof(ServicesRegistry->SrvReg[i].lpwzDLLPath));
						if (MmIsAddressValidEx(KeyValueInformation->Data) &&
							KeyValueInformation->DataLength > 0)
						{
							SafeCopyMemory(KeyValueInformation->Data,ServicesRegistry->SrvReg[i].lpwzDLLPath,KeyValueInformation->DataLength);
						}
					}
					RExFreePool(KeyValueInformation);
				}
				
			}
			RZwClose(SubKeyHandle);
		}
		RExFreePool(InterfacesKeyString);
 		RExFreePool(KeyInformation);

		InterfacesKeyString = NULL;
		KeyInformation = NULL;

		i++;
		SubkeyIndex++;
	}
	ServicesRegistry->ulCount = SubkeyIndex;

	if (NetworkClassKeyHandle)
		RZwClose(NetworkClassKeyHandle);

	if (ServicesRegistry->ulCount > 10)
	{
		Status = STATUS_SUCCESS;
	}
	return Status;        
}
NTSTATUS _stdcall NewZwSetValueKey(
	__in      HANDLE KeyHandle,
	__in      PUNICODE_STRING ValueName,
	__in_opt  ULONG TitleIndex,
	__in      ULONG Type,
	__in_opt  PVOID Data,
	__in      ULONG DataSize
	)
{
	NTSTATUS Status;
	ZWSETVALUEKEY OldZwSetValueKey;
	PVOID KeyObject;
	POBJECT_NAME_INFORMATION KeyNameInfo;
	WCHAR *KeyPath = NULL;
	UNICODE_STRING UnicodeRegLocation;
	UNICODE_STRING UnicodeComPareKey;
	UNICODE_STRING UnicodeStartValueName;
	UNICODE_STRING UnicodeServiceDllValueName;
	UNICODE_STRING UnicodeImagePathValueName;
	BOOL bInitApi = FALSE;
	BOOL bIsTrue = FALSE;
	KPROCESSOR_MODE PreviousMode;

	if (!bIsInitSuccess)
		goto _FunctionRet;

	//初步过滤
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
		goto _FunctionRet;

	if (RPsGetCurrentProcess){
		if (RPsGetCurrentProcess() == ProtectEProcess){
			goto _FunctionRet;
		}
	}
	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RPsGetCurrentProcessId,L"PsGetCurrentProcessId",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RRtlCompareUnicodeString,L"RtlCompareUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RObReferenceObjectByHandle &&
		RPsGetCurrentProcessId &&
		RRtlInitUnicodeString &&
		RRtlCompareUnicodeString &&
		RExAllocatePool &&
		RExFreePool)
	{
		bInitApi = TRUE;
	}
	if (!bInitApi)
		goto _FunctionRet;

	//严格判断下PUNICODE_STRING ValueName
	if (ValueName &&
		ValidateUnicodeString(ValueName) &&
		ValueName->Length > 0 &&
		ValueName->Buffer != 0)
	{
		//只监控这三个键值
		RRtlInitUnicodeString(&UnicodeStartValueName,L"Start");
		RRtlInitUnicodeString(&UnicodeServiceDllValueName,L"ServiceDll");
		RRtlInitUnicodeString(&UnicodeImagePathValueName,L"ImagePath");
		
		if (RRtlCompareUnicodeString(&UnicodeStartValueName,ValueName,TRUE) == 0 ||
			RRtlCompareUnicodeString(&UnicodeServiceDllValueName,ValueName,TRUE) == 0 ||
			RRtlCompareUnicodeString(&UnicodeImagePathValueName,ValueName,TRUE) == 0)
		{
			bIsTrue = TRUE;
		}
		if (!bIsTrue)
			goto _FunctionRet;

		if (bDisResetSrv == FALSE)  //禁止服务回写
			return STATUS_UNSUCCESSFUL;

		if (!ARGUMENT_PRESENT(KeyHandle))
			goto _FunctionRet;

		PreviousMode = KeGetPreviousMode();
		if (PreviousMode != KernelMode)
		{
			__try{
				ProbeForRead(KeyHandle,sizeof(HANDLE),sizeof(ULONG));
			}__except (EXCEPTION_EXECUTE_HANDLER) {
				goto _FunctionRet;
			}
		}
		Status = RObReferenceObjectByHandle(
			KeyHandle,
			0,
			NULL,
			KernelMode,
			&KeyObject,
			NULL
			);
		if (NT_SUCCESS(Status))
		{
			ObDereferenceObject(KeyObject);

			__try
			{
				if (SafeQueryNameString(KeyObject,&KeyNameInfo) == STATUS_SUCCESS)
				{
					if (ValidateUnicodeString(&KeyNameInfo->Name) &&
						KeyNameInfo->Name.Length > 0 &&
						KeyNameInfo->Name.Buffer != 0)
					{
						KeyPath = (WCHAR *)RExAllocatePool(NonPagedPool,KeyNameInfo->Name.Length*sizeof(WCHAR));
						if (!KeyPath)
						{
							if (KeyNameInfo)
								RExFreePool(KeyNameInfo);
							goto _FunctionRet;
						}
						//L"\\Registry\\Machine\\SYSTEM\\ControlSet001\\services"
						RRtlInitUnicodeString(&UnicodeRegLocation,L"\\Registry\\Machine\\SYSTEM\\ControlSet001\\serviceS");

						if (KeyNameInfo->Name.Length > UnicodeRegLocation.Length &&
							Data != NULL &&
							MmIsAddressValidEx(Data) &&
							DataSize > 0 &&
							DataSize < 1024)
						{
							memset(KeyPath,0,KeyNameInfo->Name.Length*sizeof(WCHAR));
							SafeCopyMemory(KeyNameInfo->Name.Buffer,KeyPath,UnicodeRegLocation.Length); //和谐copy相等长度的，不然见到蓝蓝的天空~~~~~

							RRtlInitUnicodeString(&UnicodeComPareKey,KeyPath);
							if (RRtlCompareUnicodeString(&UnicodeRegLocation,&UnicodeComPareKey,TRUE) == NULL)  //不区分大小写
							{
								if (DebugOn)
									KdPrint(("ValueName:%ws Key:%ws\r\n",ValueName->Buffer,KeyNameInfo->Name.Buffer));

								if (LogDefenseInfo->ulCount < 1000 &&
									ulLogCount < 1000)   //记录超过1000条，则不记录。
								{
									LogDefenseInfo->LogDefense[ulLogCount].Type = 5;  //服务监控
									LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
									LogDefenseInfo->LogDefense[ulLogCount].ulInheritedFromProcessId = GetInheritedProcessPid(RPsGetCurrentProcess());
									LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();

									memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
									memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents));
									//下面开始记录
									switch (Type)
									{
									case REG_SZ:
										SafeCopyMemory(KeyNameInfo->Name.Buffer,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,KeyNameInfo->Name.Length);
										swprintf(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents,L"%ws ==> %ws",ValueName->Buffer,Data);
										break;
									case REG_EXPAND_SZ:
										SafeCopyMemory(KeyNameInfo->Name.Buffer,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,KeyNameInfo->Name.Length);
										swprintf(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents,L"%ws ==> %ws",ValueName->Buffer,Data);
										break;
									case REG_DWORD:
										SafeCopyMemory(KeyNameInfo->Name.Buffer,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,KeyNameInfo->Name.Length);
										swprintf(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents,L"%ws ==> %d",ValueName->Buffer,*(PULONG)Data);
										break;
									}
									ulLogCount++;
								}
							}
						}
					}
					if (KeyPath)
						RExFreePool(KeyPath);

					if (KeyNameInfo)
						RExFreePool(KeyNameInfo);
				}
			}__except (EXCEPTION_EXECUTE_HANDLER) {
				goto _FunctionRet;
			}
		}
	}

_FunctionRet:

	if (MmIsAddressValidEx(OriginalServiceDescriptorTable->ServiceTable[ZwSetValueKeyIndex]))
		OldZwSetValueKey = OriginalServiceDescriptorTable->ServiceTable[ZwSetValueKeyIndex];
	else
		OldZwSetValueKey = KeServiceDescriptorTable->ServiceTable[ZwSetValueKeyIndex];

	return OldZwSetValueKey(
		KeyHandle,
		ValueName,
		TitleIndex,
		Type,
		Data,
		DataSize
		);
}
BOOL InitZwSetValueKey()
{
	UNICODE_STRING UnicdeFunction;

	//RtlInitUnicodeString(&UnicdeFunction,L"ZwWriteFile");
	if (SystemCallEntryTableHook(
		"ZwSetValueKey",
		&ZwSetValueKeyIndex,
		NewZwSetValueKey) == TRUE)
	{
		if (DebugOn)
			KdPrint(("Create Control Thread success 5\r\n"));
	}
}