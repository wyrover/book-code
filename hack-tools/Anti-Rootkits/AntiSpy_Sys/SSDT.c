#include "SSDT.h"

ULONG_PTR SSDTDescriptor ;

PVOID  GetFunctionAddressByNameFromSSDT(CHAR* szFunctionName,ULONG_PTR SSDTDescriptor)
{
	ULONG ulIndex = 0;

	PVOID FunctionAddress = NULL;

	ulIndex = GetSSDTFunctionIndexFromNtdll(szFunctionName);

	if (ulIndex==-1)
	{
		return NULL;
	}

	FunctionAddress = (PVOID)GetFunctionAddressByIndexFromSSDT(ulIndex,SSDTDescriptor);

	return FunctionAddress;
}

PVOID  GetFunctionAddressByNameFormNtos(WCHAR *wzFunction)
{
	UNICODE_STRING uniFunction;  
	PVOID AddrBase = NULL;

	if (wzFunction && wcslen(wzFunction) > 0)
	{
		RtlInitUnicodeString(&uniFunction, wzFunction);      
		AddrBase = MmGetSystemRoutineAddress(&uniFunction);  
	}
	return AddrBase;
}


ULONG_PTR GetFunctionAddressByIndexFromSSDT(ULONG_PTR ulIndex,ULONG_PTR SSDTDescriptor)
{
	ULONG_PTR ServiceTableBase= 0 ;
	PSYSTEM_SERVICE_TABLE SSDT = NULL;

	SSDTDescriptor = (ULONG_PTR)GetFunctionAddressByNameFormNtos(L"KeServiceDescriptorTable");
	SSDT = (PSYSTEM_SERVICE_TABLE)SSDTDescriptor;
	ServiceTableBase=(ULONG)(SSDT->ServiceTableBase);
	return (*(PULONG_PTR)(ServiceTableBase + 4 * ulIndex));
}


LONG GetSSDTFunctionIndexFromNtdll(char* szFindFunctionName)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	PVOID    MapBase = NULL;
	PIMAGE_NT_HEADERS       NtHeader;
	PIMAGE_EXPORT_DIRECTORY ExportTable;
	ULONG*  FunctionAddresses;
	ULONG*  FunctionNames;
	USHORT* FunctionIndexs;
	ULONG   ulIndex;
	ULONG   i;
	CHAR*   szFunctionName;
	SIZE_T  ViewSize=0;
	ULONG_PTR ulFunctionAddress;
	WCHAR wzNtdll[] = L"\\SystemRoot\\System32\\ntdll.dll";

	Status = MapFileInUserSpace(wzNtdll, NtCurrentProcess(), &MapBase, &ViewSize);
	if (!NT_SUCCESS(Status))
	{

		return STATUS_UNSUCCESSFUL;

	}
	else
	{
		__try{
			NtHeader = RtlImageNtHeader(MapBase);
			if (NtHeader && NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress){
				ExportTable =(IMAGE_EXPORT_DIRECTORY*)((ULONG_PTR)MapBase + NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
				FunctionAddresses = (ULONG*)((ULONG_PTR)MapBase + ExportTable->AddressOfFunctions);
				FunctionNames = (ULONG*)((ULONG_PTR)MapBase + ExportTable->AddressOfNames);
				FunctionIndexs = (USHORT*)((ULONG_PTR)MapBase + ExportTable->AddressOfNameOrdinals);
				for(i = 0; i < ExportTable->NumberOfNames; i++)
				{
					szFunctionName = (LPSTR)((ULONG_PTR)MapBase + FunctionNames[i]);
					if (_stricmp(szFunctionName, szFindFunctionName) == 0) 
					{
						ulIndex = FunctionIndexs[i]; 
						ulFunctionAddress = (ULONG_PTR)((ULONG_PTR)MapBase + FunctionAddresses[ulIndex]);
						ulIndex=*(ULONG*)(ulFunctionAddress + 1);
						break;
					}
				}

			}
		}__except(EXCEPTION_EXECUTE_HANDLER)
		{

		}
	}

	if (ulIndex == -1)
	{
		DbgPrint("%s Get Index Error\n", szFindFunctionName);
	}

	ZwUnmapViewOfSection(NtCurrentProcess(), MapBase);
	return ulIndex;
}

NTSTATUS  MapFileInUserSpace(WCHAR* wzFilePath, HANDLE hProcess OPTIONAL,
	 PVOID *BaseAddress,
	 PSIZE_T ViewSize OPTIONAL)
{
	NTSTATUS Status = STATUS_INVALID_PARAMETER;
	HANDLE   hFile = NULL;
	HANDLE   hSection = NULL;
	OBJECT_ATTRIBUTES oa;
	SIZE_T MapViewSize = 0;
	IO_STATUS_BLOCK Iosb;
	UNICODE_STRING uniFilePath;

	if (!wzFilePath || !BaseAddress){
		return Status;
	}

	RtlInitUnicodeString(&uniFilePath, wzFilePath);
	InitializeObjectAttributes(&oa,
		&uniFilePath,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
		);

	Status = IoCreateFile(&hFile,
		GENERIC_READ | SYNCHRONIZE,
		&oa,
		&Iosb,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0,
		CreateFileTypeNone,
		NULL,
		IO_NO_PARAMETER_CHECKING
		);

	if (!NT_SUCCESS(Status))
	{
		return Status;
	}

	oa.ObjectName = NULL;

	Status = ZwCreateSection(&hSection,
		SECTION_QUERY | SECTION_MAP_READ,
		&oa,
		NULL,
		PAGE_WRITECOPY,
		SEC_IMAGE,
		hFile
		);

	ZwClose(hFile);

	if (!NT_SUCCESS(Status))
	{
		return Status;

	}

	if (!hProcess){
		hProcess = NtCurrentProcess();
	}

	Status = ZwMapViewOfSection(hSection, 
		hProcess, 
		BaseAddress, 
		0, 
		0, 
		0, 
		ViewSize ? ViewSize : &MapViewSize, 
		ViewUnmap, 
		0, 
		PAGE_WRITECOPY
		);
	ZwClose(hSection);
	if (!NT_SUCCESS(Status))
	{
		return Status;
	}
	return Status;
}