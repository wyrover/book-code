#include "Startup.h"


//L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\";
NTSTATUS ReadRegistry(PSTARTUP_INFO Startup,WCHAR *DriverServiceNamePath,WCHAR *ReadKey)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	OBJECT_ATTRIBUTES NetworkClassKeyObject;
	OBJECT_ATTRIBUTES SubKeyObject;
	HANDLE NetworkClassKeyHandle;
	HANDLE SubKeyHandle;
	ULONG i, SubkeyIndex, ResultLength, InterfacesKeyStringLength;
	PWCHAR InterfacesKeyString;
	UNICODE_STRING NetworkClassKey, InterfacesKey, UnicodeReadKey, DriverServiceName;
	PKEY_BASIC_INFORMATION KeyInformation;
	PKEY_VALUE_PARTIAL_INFORMATION KeyValueInformation;
	WCHAR *KeyName = NULL;
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

	RRtlInitUnicodeString(&UnicodeReadKey,ReadKey);
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
	i = Startup->ulCount;

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
		if ((KeyName = (WCHAR *)RExAllocatePool(NonPagedPool, 260*sizeof(WCHAR))) == NULL){
			continue;
		}
		memset(KeyName,0,260*sizeof(WCHAR));
		SafeCopyMemory(KeyInformation->Name,KeyName,KeyInformation->NameLength);

		//KdPrint(("Key:%ws\n",KeyName));

		//读取UnicodeReadKey
		RRtlInitUnicodeString(&DriverServiceName,KeyName);
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
			if ((Status = RZwQueryValueKey(SubKeyHandle, &UnicodeReadKey, KeyValuePartialInformation, NULL, 0, &ResultLength)) != STATUS_OBJECT_NAME_NOT_FOUND)
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
						&UnicodeReadKey, 
						KeyValuePartialInformation,
						KeyValueInformation,
						ResultLength, 
						&ResultLength
						);
					if (NT_SUCCESS(Status))
					{
						//KdPrint(("%ws -> %ws\n",KeyName,KeyValueInformation->Data));
						if (MmIsAddressValidEx(KeyValueInformation->Data) &&
							KeyValueInformation->DataLength > 0)
						{
							SafeCopyMemory(DriverServiceName.Buffer,Startup->Startup[i].lpwzName,DriverServiceName.Length);
							SafeCopyMemory(DriverServiceNamePath,Startup->Startup[i].lpwzKeyPath,wcslen(DriverServiceNamePath)*2);
							SafeCopyMemory(KeyValueInformation->Data,Startup->Startup[i].lpwzKeyValue,KeyValueInformation->DataLength);
							i++;
		                    Startup->ulCount = i;

							if (DebugOn)
								KdPrint(("%ws -> %ws\n",KeyName,KeyValueInformation->Data));
						}
					}
					RExFreePool(KeyValueInformation);
				}

			}
			RZwClose(SubKeyHandle);
		}
		RExFreePool(KeyName);
 		RExFreePool(KeyInformation);

		KeyName = NULL;
		KeyInformation = NULL;

		SubkeyIndex++;
	}

	if (NetworkClassKeyHandle)
		RZwClose(NetworkClassKeyHandle);

	return Status;        
}
VOID QueryStartup(PSTARTUP_INFO Startup)
{

	ReadRegistry(Startup,L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Notify",L"DllName");

	//其实是读取Winlogon下的Userinit、UIHost、Shell
	ReadRegistry(Startup,L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"Userinit");
	ReadRegistry(Startup,L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"UIHost");
	ReadRegistry(Startup,L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"Shell");

	ReadRegistry(Startup,L"\\Registry\\Machine\\SOFTWARE\\Microsoft\\Active Setup\\Installed Components",L"Stubpath");

	ReadRegistry(Startup,L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\Print\\Monitors",L"Driver");
	ReadRegistry(Startup,L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\Print\\Providers",L"Name");
}