#include "Function.h"

VOID WcharToChar(__in WCHAR *wzFuncName,__out CHAR *FuncName)
{
	UNICODE_STRING UnicodeFuncName;
	ANSI_STRING AnsiFuncName;

	RtlInitUnicodeString(&UnicodeFuncName,wzFuncName);
	if (RtlUnicodeStringToAnsiString(&AnsiFuncName,&UnicodeFuncName,TRUE) == STATUS_SUCCESS){
		memcpy(FuncName,AnsiFuncName.Buffer,AnsiFuncName.Length);
		RtlFreeAnsiString(&AnsiFuncName);
	}
}
ULONG PsGetProcessCount()
{ 
	NTSTATUS	 status;
	ULONG		 uCount = 0;
	PLIST_ENTRY	 pListActiveProcess;
	PEPROCESS	 pCurrentEprocess = NULL;
	ULONG        ulNextProcess = NULL;
	ULONG        g_Offset_Eprocess_Flink;
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
		g_Offset_Eprocess_Flink = 0x88;
		break;
	case WINDOWS_VERSION_7_7600_UP:
	case WINDOWS_VERSION_7_7000:
		g_Offset_Eprocess_Flink = 0xb8;
		break;
	case WINDOWS_VERSION_VISTA_2008:
		g_Offset_Eprocess_Flink = 0x0a0;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		g_Offset_Eprocess_Flink = 0x98;
		break;
	case WINDOWS_VERSION_2K3:
		g_Offset_Eprocess_Flink = 0x088;
		break;
	}
	if (!g_Offset_Eprocess_Flink){
		return 0;
	}
	pCurrentEprocess = PsGetCurrentProcess();
	ulNextProcess = pCurrentEprocess;
	__try
	{
		while(1)
		{
			pListActiveProcess = (LIST_ENTRY *)((ULONG)pCurrentEprocess + g_Offset_Eprocess_Flink);
			(ULONG)pCurrentEprocess = (ULONG)pListActiveProcess->Flink;
			(ULONG)pCurrentEprocess = (ULONG)pCurrentEprocess - g_Offset_Eprocess_Flink;
			uCount++;

			if (ulNextProcess == pCurrentEprocess){
				__leave;
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return uCount;
	}
	return uCount;
}
//这个函数执行在reload之前，因此要使用原始函数
BOOL KeBugCheckCreateValueKey(PWCHAR SafeKey)
{
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING RegUnicodeString,Unicode_ValueName;
	NTSTATUS ntStatus;
	HANDLE hRegister;
	ULONG ulValue_DWORD;
	ULONG ulResult=NULL;
	BOOL bRetOK = FALSE;

	RtlInitUnicodeString(&RegUnicodeString,SafeKey);
	//初始化objectAttributes
	InitializeObjectAttributes(
		&objectAttributes,
		&RegUnicodeString,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,//对大小写敏感 
		NULL, 
		NULL 
		);
	//创建或带开注册表项目
	ntStatus = ZwCreateKey(
		&hRegister,
		KEY_ALL_ACCESS,
		&objectAttributes,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		&ulResult
		);
	if (NT_SUCCESS(ntStatus))
	{
		bRetOK = TRUE;
		ZwFlushKey(hRegister);
		ZwClose(hRegister);
	}
	return bRetOK;
}
//这个函数执行在reload之前，因此要使用原始函数
BOOL IsRegKeyInSystem(PWCHAR ServicesKey)
{
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING RegUnicodeString;
	NTSTATUS ntStatus;
	HANDLE hRegister;
	BOOL bRetOK = FALSE;

	RtlInitUnicodeString(&RegUnicodeString,ServicesKey);
	InitializeObjectAttributes(
		&objectAttributes,
		&RegUnicodeString,
		OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,//对大小写敏感 
		NULL, 
		NULL 
		);
	ntStatus = ZwOpenKey(&hRegister,
		KEY_QUERY_VALUE,
		&objectAttributes
		);
	if (NT_SUCCESS(ntStatus))
	{
		bRetOK = TRUE;
		ZwClose(hRegister);
	}
	return bRetOK;
}
BOOL IsFileInSystem(WCHAR *lpwzFile)
{
	HANDLE hFile = NULL;
	NTSTATUS status;
	IO_STATUS_BLOCK Io_Status_Block;
	OBJECT_ATTRIBUTES obj_attrib;
	UNICODE_STRING UnicodeFileName;
	BOOL bIsFile = FALSE;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCreateFile,L"IoCreateFile",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RIoCreateFile &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return bIsFile;

	RRtlInitUnicodeString(&UnicodeFileName,lpwzFile);
	InitializeObjectAttributes(
		&obj_attrib,
		&UnicodeFileName,
		OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,
		NULL, 
		NULL
		);
	status = RIoCreateFile(
		&hFile,
		GENERIC_READ,  //以只读的方式打开，不然会提示错误32
		&obj_attrib,
		&Io_Status_Block,
		0,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		0,
		NULL,
		0,
		0,
		NULL,
		IO_NO_PARAMETER_CHECKING);
	if (NT_SUCCESS(status))
	{
		//文件被删除了
		//KdPrint(("file been deleted"));

		RZwClose(hFile);
		bIsFile = TRUE;
	}
	return bIsFile;
}
NTSTATUS SafeQueryFileDosDeviceName(__in WCHAR *wzNtImageName,__out WCHAR *wzDosFullPath)
{
	NTSTATUS Status;
	OBJECT_ATTRIBUTES	ObjectAttributes; 
	struct _IO_STATUS_BLOCK IoStatusBlock;
	HANDLE FileHandle=NULL;
	PFILE_OBJECT FileObject;
	POBJECT_NAME_INFORMATION DosFullPath=NULL;
	UNICODE_STRING NtImageName;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwOpenFile,L"ZwOpenFile",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoQueryFileDosDeviceName,L"IoQueryFileDosDeviceName",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenFile &&
		RObReferenceObjectByHandle &&
		RIoQueryFileDosDeviceName &&
		RZwClose &&
		RExFreePool &&
		RRtlInitUnicodeString)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	RRtlInitUnicodeString(&NtImageName,wzNtImageName);
	InitializeObjectAttributes( 
		&ObjectAttributes,
		&NtImageName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL
		);
	Status = RZwOpenFile(
		&FileHandle,
		GENERIC_READ,
		&ObjectAttributes,
		&IoStatusBlock,
		FILE_SHARE_READ,
		FILE_SYNCHRONOUS_IO_NONALERT
		);
	if (!NT_SUCCESS(Status))
	{
		//DbgPrint("ZwOpenFile[%ws] failed:%d\r\n",NtImageName.Buffer,RtlNtStatusToDosError(Status));
		return Status;
	}
	Status = RObReferenceObjectByHandle(
		FileHandle,
		PROCESS_ALL_ACCESS,
		*IoFileObjectType,
		KernelMode,
		(PVOID *)&FileObject,
		NULL);
	if (NT_SUCCESS(Status))
	{
		if (RIoQueryFileDosDeviceName(FileObject,&DosFullPath) == STATUS_SUCCESS)
		{
			Status = STATUS_UNSUCCESSFUL;
			Status = SafeCopyMemory(DosFullPath->Name.Buffer,wzDosFullPath,DosFullPath->Name.Length);
			RExFreePool(DosFullPath);
		}
		ObDereferenceObject(FileObject);
	}
	RZwClose(FileHandle);
	return Status;
}
NTSTATUS SafeQueryNameString(
	IN PVOID FileObject,
	OUT POBJECT_NAME_INFORMATION *FileNameInfo
	)
{
	ULONG NumberOfBytes;
	ULONG AdditionalLengthNeeded;
	NTSTATUS Status;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RExAllocatePoolWithTag,L"ExAllocatePoolWithTag",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObQueryNameString,L"ObQueryNameString",SystemKernelModuleBase,ImageModuleBase);
	if (RExAllocatePoolWithTag &&
		RExFreePool &&
		RObQueryNameString)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	NumberOfBytes = 1024;

	*FileNameInfo = NULL;

	*FileNameInfo = RExAllocatePoolWithTag (NonPagedPool, NumberOfBytes, ' mM');

	if (*FileNameInfo == NULL) {
		return STATUS_NO_MEMORY;
	}

	Status = RObQueryNameString (FileObject,
		*FileNameInfo,
		NumberOfBytes,
		&AdditionalLengthNeeded);

	if (!NT_SUCCESS (Status)) {

		if (Status == STATUS_INFO_LENGTH_MISMATCH) {

			//
			// Our buffer was not large enough, retry just once with a larger
			// one (as specified by ObQuery). Don't try more than once to
			// prevent broken parse procedures which give back wrong
			// AdditionalLengthNeeded values from causing problems.
			//

			RExFreePool (*FileNameInfo);

			NumberOfBytes += AdditionalLengthNeeded;

			*FileNameInfo = RExAllocatePoolWithTag (NonPagedPool,
				NumberOfBytes,
				' mM');

			if (*FileNameInfo == NULL) {
				return STATUS_NO_MEMORY;
			}

			Status = RObQueryNameString (FileObject,
				*FileNameInfo,
				NumberOfBytes,
				&AdditionalLengthNeeded);

			if (NT_SUCCESS (Status)) {
				return STATUS_SUCCESS;
			}
		}

		RExFreePool (*FileNameInfo);
		*FileNameInfo = NULL;
		return Status;
	}

	return STATUS_SUCCESS;
}
BOOL GetDriverEntryPoint(PVOID ImageBase,DWORD *pOutDriverEntry)
{
	BOOL bRet=FALSE;
	PIMAGE_NT_HEADERS NtHeaders;
	ULONG_PTR EntryPoint;

	NtHeaders = RtlImageNtHeader(ImageBase);
	if (NtHeaders)
	{
		DWORD dwEntryPoint;
		DWORD dwCurAddress;
		DWORD Length=0;
		PUCHAR pOpcode;
		EntryPoint = NtHeaders->OptionalHeader.AddressOfEntryPoint;
		EntryPoint += (ULONG_PTR)ImageBase;
		dwEntryPoint=(DWORD)EntryPoint;
		for(dwCurAddress = dwEntryPoint; dwCurAddress < dwEntryPoint + 0x1000; dwCurAddress+=Length)
		{
			Length = SizeOfCode((PUCHAR)dwCurAddress, &pOpcode);
			if(Length==2&&*(WORD *)dwCurAddress==0xFF8B)
			{  
				//KdPrint(("find:%08x",dwCurAddress));
				*pOutDriverEntry = dwCurAddress;
				bRet = TRUE;
				break;
			}
			//mouclass.sys 开头函数：
			/*
			INIT:00017811 DriverEntry     proc near
			INIT:00017811                 mov     eax, dword_14028
			INIT:00017816                 test    eax, eax
			INIT:00017818                 mov     ecx, 0BB40E64Eh
			INIT:0001781D                 jz      short loc_17823
			INIT:0001781F                 cmp     eax, ecx
			INIT:00017821                 jnz     short loc_1783C
			INIT:00017823
			INIT:00017823 loc_17823:                              ; CODE XREF: DriverEntry+Cj
			INIT:00017823                 mov     eax, ds:KeTickCount
			INIT:00017828                 mov     eax, [eax]            <------特征码 0x008B
			INIT:0001782A                 xor     eax, offset dword_14028
			INIT:0001782F                 mov     dword_14028, eax
			INIT:00017834                 jnz     short loc_1783C
			INIT:00017836                 mov     dword_14028, ecx
			INIT:0001783C
			INIT:0001783C loc_1783C:                              ; CODE XREF: DriverEntry+10j
			INIT:0001783C                                         ; DriverEntry+23j
			INIT:0001783C                 jmp     sub_172CE
			INIT:0001783C DriverEntry     endp
			*/
			else if (Length==2&&*(WORD *)dwCurAddress==0x008B)
			{
				*pOutDriverEntry = dwCurAddress;
				bRet = TRUE;
				break;
			}
		}
	}
	return bRet;
}
NTSTATUS GetDriverObject(WCHAR *lpwzDevice,PDRIVER_OBJECT *PDriverObject)
{
	PDRIVER_OBJECT DriverObject;
	UNICODE_STRING ObjectName;
	NTSTATUS Status;
	BOOL bInit = FALSE;


	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByName,L"ObReferenceObjectByName",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RObReferenceObjectByName)
	{
		bInit = TRUE;
	}
	if (!bInit)
	{
		//KdPrint(("ReLoadNtosCALL failed:%x %x",RRtlInitUnicodeString,RObReferenceObjectByName));
		return STATUS_UNSUCCESSFUL;
	}

	RRtlInitUnicodeString(&ObjectName,lpwzDevice);
	Status = RObReferenceObjectByName(&ObjectName, 
		OBJ_CASE_INSENSITIVE,
		NULL,
		0,
		*IoDriverObjectType,
		KernelMode,
		NULL,
		&DriverObject);
	if (NT_SUCCESS(Status))
	{
		*PDriverObject = DriverObject;
	}else
		KdPrint(("ObReferenceObjectByName failed:%d",RtlNtStatusToDosError(Status)));

	return Status;
}
PVOID LookupKernelModuleByName(PDRIVER_OBJECT DriverObject,char *KernelModuleName,DWORD *ulWin32kSize)
{
	PLDR_DATA_TABLE_ENTRY DriverSection,LdrEntry;
	ANSI_STRING AnsiKernelModuleName;
	UNICODE_STRING UniKernelModuleName;
	UNICODE_STRING ModuleName;
	WCHAR *Buffer;
	int Lentgh,Index;

	__try
	{
		RtlInitAnsiString(&AnsiKernelModuleName,KernelModuleName);
		RtlAnsiStringToUnicodeString(&UniKernelModuleName,&AnsiKernelModuleName,TRUE);
		Buffer=ExAllocatePool(NonPagedPool,260*2);
		if (Buffer==NULL)
		{
			RtlFreeUnicodeString(&UniKernelModuleName);
			return NULL;
		}
		RtlZeroMemory(Buffer,206*2);
		DriverSection=DriverObject->DriverSection;
		LdrEntry=(PLDR_DATA_TABLE_ENTRY)DriverSection->InLoadOrderLinks.Flink;
		while (LdrEntry&&DriverSection!=LdrEntry)
		{

			if ((DWORD)LdrEntry->DllBase>=*(DWORD*)MmSystemRangeStart&&
				LdrEntry->FullDllName.Length>0&&
				LdrEntry->FullDllName.Buffer!=NULL)
			{

				if (MmIsAddressValidEx(&LdrEntry->FullDllName.Buffer[LdrEntry->FullDllName.Length/2-1]))
				{
					Lentgh=LdrEntry->FullDllName.Length/2;
					for (Index=Lentgh-1;Index>0;Index--)
					{
						if (LdrEntry->FullDllName.Buffer[Index]==0x005C)
						{
							break;
						}
					}
					if (LdrEntry->FullDllName.Buffer[Index]==0x005C)
					{
						RtlCopyMemory(Buffer,&(LdrEntry->FullDllName.Buffer[Index+1]),(Lentgh-Index-1)*2);
						ModuleName.Buffer=Buffer;
						ModuleName.Length=(Lentgh-Index-1)*2;
						ModuleName.MaximumLength=260*2;
					}
					else
					{
						RtlCopyMemory(Buffer,LdrEntry->FullDllName.Buffer,Lentgh*2);
						ModuleName.Buffer=Buffer;
						ModuleName.Length=Lentgh*2;
						ModuleName.MaximumLength=260*2;

					}
					//KdPrint(("L:%wZ--%wZ\n",&ModuleName,&UniKernelModuleName));

					if (RtlEqualUnicodeString(&ModuleName,&UniKernelModuleName,TRUE))
					{
						ExFreePool(Buffer);
						RtlFreeUnicodeString(&UniKernelModuleName);

						//KdPrint(("find:%wZ--%X\n",&LdrEntry->FullDllName,LdrEntry->DllBase));
						*ulWin32kSize = LdrEntry->SizeOfImage;
						return LdrEntry->DllBase;
					}

				}

			}	
			LdrEntry=(PLDR_DATA_TABLE_ENTRY)LdrEntry->InLoadOrderLinks.Flink;
		}
		RtlFreeUnicodeString(&UniKernelModuleName);
		ExFreePool(Buffer);

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return NULL;
}
DWORD CsGetFileSize(HANDLE FileHandle,PDWORD HightLength)
{
	FILE_STANDARD_INFORMATION FileStandardInfo;
	NTSTATUS Status;
	IO_STATUS_BLOCK IoStatus;
	Status=ZwQueryInformationFile(FileHandle,
		&IoStatus,
		&FileStandardInfo,
		sizeof(FILE_STANDARD_INFORMATION),
		FileStandardInformation
		);
	if (!NT_SUCCESS(Status))
	{
		return -1;
	}
	if (HightLength)
	{
		__try
		{
			*HightLength=FileStandardInfo.EndOfFile.HighPart;
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("CsGetFileSize:%08x\r\n",GetExceptionCode()));
			return -1;
		}
	}
	return FileStandardInfo.EndOfFile.LowPart;
}
BOOL CsSetFilePointer(HANDLE FileHandle,
	PLARGE_INTEGER ByteOffset)
{
	FILE_STANDARD_INFORMATION FileStandardInfo;
	NTSTATUS Status;
	IO_STATUS_BLOCK IoStatus;
	FILE_POSITION_INFORMATION FilePositionInfo;
	BOOL bRetOK = FALSE;

	FilePositionInfo.CurrentByteOffset.QuadPart=ByteOffset->QuadPart;
	Status=ZwSetInformationFile(
		FileHandle,
		&IoStatus,
		&FilePositionInfo,
		sizeof(FILE_POSITION_INFORMATION),
		FilePositionInformation
		);
	if (NT_SUCCESS(Status))
	{
		bRetOK = TRUE;
	}
	return bRetOK;
}
ULONG  DebugWriteToFile(WCHAR *FileName,PVOID buffer,ULONG ulSize)
{
	HANDLE hFile = NULL;
	NTSTATUS status;
	IO_STATUS_BLOCK Io_Status_Block;
	UNICODE_STRING lpwFileName;
	ULONG ulHighPart;
	ULONG ulLowPart;

	LARGE_INTEGER ByteOffset;

	// 初始化文件路径
	OBJECT_ATTRIBUTES obj_attrib;

	RtlInitUnicodeString(&lpwFileName,FileName);
	InitializeObjectAttributes(
		&obj_attrib,
		&lpwFileName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
		NULL, 
		NULL
		);
	status = IoCreateFile(
		&hFile,
		GENERIC_WRITE,
		&obj_attrib,
		&Io_Status_Block,
		0,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OPEN_IF,
		0,
		NULL,
		0,
		0,
		NULL,
		IO_NO_PARAMETER_CHECKING
		);
	if (NT_SUCCESS(status))
	{
		ulLowPart = CsGetFileSize(hFile,&ulHighPart);
		if (ulLowPart != -1)
		{
			//KdPrint(("FileSize:%d\r\nWriteBuffer:%d",ulLowPart,ulSize));

			ByteOffset.QuadPart = ulLowPart;

			if (CsSetFilePointer(hFile,&ByteOffset) == TRUE)
			{
				status = ZwWriteFile(
					hFile,
					NULL,
					NULL,
					NULL,
					&Io_Status_Block,
					buffer,
					ulSize,
					&ByteOffset,
					NULL
					);
			}
		}
		// 关闭句柄
		if (hFile)
		{
			ZwClose(hFile);
		}
	}else
		KdPrint(("NtCreateFile failed:%d\r\n",RtlNtStatusToDosError(status)));

	return RtlNtStatusToDosError(status);
}
/*
BOOL GetProcessImagePath(    
	IN  ULONG   ulPid,    
	OUT WCHAR *FullProcessImagePath    
	)    
{    
	NTSTATUS Status;    
	HANDLE hProcess;    
	PEPROCESS EProcess;    
	ULONG returnedLength;    
	ULONG bufferLength;    
	PVOID buffer;    
	PUNICODE_STRING ImageName;
	BOOL bInit = FALSE;
	OBJECT_ATTRIBUTES ObjectAttributes;
	CLIENT_ID ClientId={0};

	//ReLoadNtosCALL(&RObOpenObjectByPointer,L"ObOpenObjectByPointer",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwOpenProcess,L"ZwOpenProcess",SystemKernelModuleBase,ImageModuleBase);
	RZwQueryInformationProcess = ReLoadNtosCALL(L"ZwQueryInformationProcess",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenProcess &&
		RZwQueryInformationProcess &&
		RExAllocatePool &&
		RExFreePool &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	PAGED_CODE(); // this eliminates the possibility of the IDLE Thread/Process     

	ClientId.UniqueProcess = ulPid;
	InitializeObjectAttributes(
		&ObjectAttributes, 
		NULL,
		0, 
		NULL,
		NULL 
		);
	Status=RZwOpenProcess(
		&hProcess,
		PROCESS_ALL_ACCESS,
		&ObjectAttributes,
		&ClientId
		);
	if (!NT_SUCCESS(Status))
	{
		return NULL;
	}
// 	Status = RObOpenObjectByPointer(
// 		EProcess,          // Object    
// 		OBJ_KERNEL_HANDLE,  // HandleAttributes    
// 		NULL,               // PassedAccessState OPTIONAL    
// 		GENERIC_READ,       // DesiredAccess    
// 		*PsProcessType,     // ObjectType    
// 		KernelMode,         // AccessMode    
// 		&hProcess
// 		);    
// 	if (!NT_SUCCESS(Status))
// 	{
// 		if (DebugOn)
// 			KdPrint(("ObOpenObjectByPointer Error\r\n"));
// 		return FALSE;  
// 	}
	Status = RZwQueryInformationProcess(
		hProcess,    
		ProcessImageFileName,    
		NULL, // buffer    
		0, // buffer size    
		&returnedLength);
	if (STATUS_INFO_LENGTH_MISMATCH != Status)
	{    
		RZwClose(hProcess);

		if (DebugOn)
			KdPrint(("ZwQueryInformationProcess Error\r\n"));
		return FALSE;    
	}

	buffer = RExAllocatePool(PagedPool, returnedLength);    
	if (buffer)
	{
		memset(buffer,0,returnedLength);
		Status = RZwQueryInformationProcess( 
			hProcess,    
			ProcessImageFileName,    
			buffer,    
			returnedLength,    
			&returnedLength
			);    
		if (NT_SUCCESS(Status))
		{        
			ImageName = (PUNICODE_STRING)buffer;
			if (ValidateUnicodeString(ImageName))
			{
				SafeCopyMemory(
					ImageName->Buffer,
					FullProcessImagePath,
					ImageName->Length
					);
			}   
		}    
		if (buffer)
			RExFreePool(buffer); 
	}
	RZwClose(hProcess);
	return TRUE;    
}*/

PVOID GetZwQueryVirtualMemoryAddress()
{
	PVOID FunctionAddress=0;
	ULONG ulSearchStart;
	int Index;
	PUCHAR i=0;
	
/*
lkd> u ZwQueryVirtualMemory
nt!ZwQueryVirtualMemory:
804ffb90 b8b2000000      mov     eax,0B2h
804ffb95 8d542404        lea     edx,[esp+4]
804ffb99 9c              pushfd
804ffb9a 6a08            push    8
804ffb9c e8f0e80300      call    nt!KiSystemService (8053e491)
804ffba1 c21800          ret     18h
nt!ZwQueryVolumeInformationFile:                 <------------------从这里一直往上减，当Index一样的时候，就是地址了。
804ffba4 b8b3000000      mov     eax,0B3h
804ffba9 8d542404        lea     edx,[esp+4]
*/
	GetFunctionIndexByName("ZwQueryVirtualMemory",&Index);
	if (Index){
		ulSearchStart = (ULONG)ZwQueryVolumeInformationFile;
		for (i=ulSearchStart;i > ulSearchStart - 0x50;i--)
		{
			if (MmIsAddressValidEx(i))
			{
				if (*i == Index){
					FunctionAddress = i - 1;

					if (DebugOn)
						KdPrint(("FunctionAddress:%08x\n",FunctionAddress));
					break;
				}
			}
		}
	}
	return FunctionAddress;
}
BOOL GetProcessFullImagePath(    
	IN  PEPROCESS Eprocess, 
	OUT WCHAR *FullProcessImagePath
	)    
{
	ULONG dwStartAddr = 0x00000000;
	HANDLE hProcess;
	MEMORY_BASIC_INFORMATION mbi;
	PUNICODE_STRING SectionName = NULL;
	NTSTATUS status;
	int count = 0;
	ULONG SectionBaseAddressOffset = 0;
	ULONG SectionBaseAddress = 0;

	ULONG ulPid = 0;
	BOOL bInit = FALSE;

	if (!ARGUMENT_PRESENT(Eprocess) ||
		!Eprocess){
			return FALSE;
	}
	if (Eprocess == SystemEProcess){
		memcpy(FullProcessImagePath,L"System",wcslen(L"System")*2);
		return TRUE;
	}
	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
		SectionBaseAddressOffset = 0x13c;
		if (!MmIsAddressValidEx(RZwQueryVirtualMemory)){
			RZwQueryVirtualMemory = GetZwQueryVirtualMemoryAddress();
		}
		break;
	case WINDOWS_VERSION_7_7600_UP:
	case WINDOWS_VERSION_7_7000:
		SectionBaseAddressOffset = 0x12c;
		ReLoadNtosCALL(&RZwQueryVirtualMemory,L"ZwQueryVirtualMemory",SystemKernelModuleBase,ImageModuleBase); //win7导出了，可以直接用
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		SectionBaseAddressOffset = 0x128;
		if (!MmIsAddressValidEx(RZwQueryVirtualMemory)){
			RZwQueryVirtualMemory = GetZwQueryVirtualMemoryAddress();
		}
		break;
	}
	if (!SectionBaseAddressOffset){
		return bInit;
	}
	ReLoadNtosCALL(&RObOpenObjectByPointer,L"ObOpenObjectByPointer",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RObOpenObjectByPointer &&
		RZwQueryVirtualMemory &&
		RExAllocatePool &&
		RExFreePool &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit){
		//KdPrint(("function failed %08x %08x %08x %08x %08x\n",RObOpenObjectByPointer,RZwQueryVirtualMemory,RExAllocatePool,RExFreePool,RZwClose));
		return NULL;
	}
	bInit = FALSE;

	SectionBaseAddress = *((PULONG)((ULONG)Eprocess + SectionBaseAddressOffset));

// 	ClientId.UniqueProcess = ulPid;
// 	InitializeObjectAttributes(
// 		&ObjectAttributes,
// 		NULL,
// 		0,
// 		NULL, 
// 		NULL 
// 		);
// 	status=RZwOpenProcess(
// 		&hProcess,
// 		PROCESS_ALL_ACCESS,
// 		&ObjectAttributes,
// 		&ClientId
// 		);

	status = RObOpenObjectByPointer(
		Eprocess,          // Object    
		OBJ_KERNEL_HANDLE,  // HandleAttributes    
		NULL,               // PassedAccessState OPTIONAL    
		GENERIC_READ,       // DesiredAccess    
		*PsProcessType,     // ObjectType    
		KernelMode,         // AccessMode    
		&hProcess
		);    
	if (!NT_SUCCESS(status))
	{
		//KdPrint(("ObOpenObjectByPointer failed:%d",RtlNtStatusToDosError(status)));
		return;
	}
	SectionName = (PUNICODE_STRING)RExAllocatePool(NonPagedPool,260*sizeof(WCHAR));
	if (!SectionName)
	{
		RZwClose(hProcess);
		//KdPrint(("RExAllocatePool failed\n"));
		return;
	}
	memset(SectionName,0,260*sizeof(WCHAR));

	__try
	{
		for (dwStartAddr=0; dwStartAddr<0x80000000; dwStartAddr=dwStartAddr+0x10000)
		{
			status = RZwQueryVirtualMemory(
				hProcess,
				(PVOID)dwStartAddr,
				MemoryBasicInformation,
				&mbi,
				sizeof(MEMORY_BASIC_INFORMATION),
				0
				);
			if (NT_SUCCESS(status))
			{
				if(mbi.Type == MEM_IMAGE &&
				   mbi.BaseAddress == SectionBaseAddress) //相等说明此时的VirtualMemory是exe主空间，而非dll
				{
					status = RZwQueryVirtualMemory(
						hProcess,
						(PVOID)dwStartAddr,
						MemorySectionName,
						SectionName,
						260*sizeof(WCHAR),
						0
						);
					if (NT_SUCCESS(status))
					{
						if (DebugOn)
							KdPrint(("SectionName:%08x %ws \n",mbi.BaseAddress,SectionName->Buffer));

						if (ValidateUnicodeString(SectionName))
						{
							SafeCopyMemory(
								SectionName->Buffer,
								FullProcessImagePath,
								SectionName->Length
								);
							bInit = TRUE;
							break;
						}
					}
				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){
		//KdPrint(("EXCEPTION_EXECUTE_HANDLER failed\n"));
		goto _FunctionRet;
	}
_FunctionRet:

	RExFreePool(SectionName);
	RZwClose(hProcess);
	return bInit;
}
NTSTATUS LookupProcessByPid(
	IN HANDLE hProcessPid,
	OUT PEPROCESS *pEprocess
	)
{ 
	NTSTATUS	status;
	ULONG		uCurrentProcessId = 0;
	ULONG		uStartProcessId = 0; 
	ULONG		uCount = 0;
	ULONG		uLength = 0;
	PLIST_ENTRY	pListActiveProcess;
	PEPROCESS	pCurrentEprocess = NULL;
	ULONG g_Offset_Eprocess_ProcessId;
	ULONG g_Offset_Eprocess_Flink;
	WIN_VER_DETAIL WinVer;

	if (!ARGUMENT_PRESENT(hProcessPid))
	{
		return STATUS_INVALID_PARAMETER;
	}
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return STATUS_UNSUCCESSFUL;
	}

	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
		g_Offset_Eprocess_ProcessId = 0x84;
		g_Offset_Eprocess_Flink = 0x88;
		break;
	case WINDOWS_VERSION_7_7600_UP:
	case WINDOWS_VERSION_7_7000:
		g_Offset_Eprocess_ProcessId = 0xb4;
		g_Offset_Eprocess_Flink = 0xb8;
		break;
	case WINDOWS_VERSION_VISTA_2008:
		g_Offset_Eprocess_ProcessId = 0x09c;
		g_Offset_Eprocess_Flink = 0x0a0;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		g_Offset_Eprocess_ProcessId = 0x94;
		g_Offset_Eprocess_Flink = 0x98;
		break;
	case WINDOWS_VERSION_2K3:
		g_Offset_Eprocess_ProcessId = 0x084;
		g_Offset_Eprocess_Flink = 0x088;
		break;
	}
	if (!g_Offset_Eprocess_ProcessId ||
		!g_Offset_Eprocess_Flink){
			return STATUS_UNSUCCESSFUL;
	}
	pCurrentEprocess = PsGetCurrentProcess();
	uStartProcessId = *((PULONG)((ULONG)pCurrentEprocess + g_Offset_Eprocess_ProcessId));
	uCurrentProcessId = uStartProcessId;

	__try
	{
		while(1)
		{
			if (hProcessPid == *((PULONG)((ULONG)pCurrentEprocess + g_Offset_Eprocess_ProcessId)))
			{
				*pEprocess = pCurrentEprocess;
				status = STATUS_SUCCESS;
				break;
			}
			if ((uCount >= 1) && (uStartProcessId == uCurrentProcessId))
			{
				*pEprocess = 0x00000000;
				status = STATUS_NOT_FOUND;
				break;
			}
			pListActiveProcess = (LIST_ENTRY *)((ULONG)pCurrentEprocess + g_Offset_Eprocess_Flink);
			(ULONG)pCurrentEprocess = (ULONG)pListActiveProcess->Flink;
			(ULONG)pCurrentEprocess = (ULONG)pCurrentEprocess - g_Offset_Eprocess_Flink;
			uCurrentProcessId = *(PULONG)((ULONG)pCurrentEprocess + g_Offset_Eprocess_ProcessId);
			uCount++;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		KdPrint(("LookupProcessByPid:%08x\r\n",GetExceptionCode()));
		status = STATUS_NOT_FOUND;
	}
	return status;
}
///////////////////////////////////////////////////////////////////////////////////
//
//  功能实现：根据设备名获取文件句柄或文件对象指针
//  输入参数：FileHandle是要输出的文件句柄指针;
//        FileObject是要输出的文件对象指针
//        DeviceName是要获取设备的设备名
//  输出参数：返回NTSTATUS类型的值
//
///////////////////////////////////////////////////////////////////////////////////

NTSTATUS GetObjectByName(
	OUT PHANDLE FileHandle,
	OUT PFILE_OBJECT  *FileObject,
	IN WCHAR  *DeviceName
	)
{
	UNICODE_STRING    deviceTCPUnicodeString;
	OBJECT_ATTRIBUTES  TCP_object_attr;
	NTSTATUS      status;
	IO_STATUS_BLOCK    IoStatus;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwCreateFile,L"ZwCreateFile",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RZwCreateFile &&
		RObReferenceObjectByHandle
		)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	RRtlInitUnicodeString(&deviceTCPUnicodeString,DeviceName);
	InitializeObjectAttributes(&TCP_object_attr,
		&deviceTCPUnicodeString,
		OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE,
		0,
		0
		);
	status=RZwCreateFile(
		FileHandle,
		GENERIC_READ|GENERIC_WRITE|SYNCHRONIZE,
		&TCP_object_attr,
		&IoStatus,
		0,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		0,
		0,
		0
		);
	if(!NT_SUCCESS(status))
	{
		KdPrint(("Failed to open"));
		return STATUS_UNSUCCESSFUL;
	}
	status=RObReferenceObjectByHandle(
		*FileHandle,
		FILE_ANY_ACCESS,
		*IoFileObjectType,
		KernelMode,
		(PVOID*)FileObject,
		NULL
		);
	return status;
}
HANDLE MapFileAsSection(PUNICODE_STRING FileName,PVOID *ModuleBase)
{
	NTSTATUS status;
	HANDLE  hSection, hFile;
	DWORD dwKSDT;
	PVOID BaseAddress = NULL;
	SIZE_T size=0;
	IO_STATUS_BLOCK iosb;
	OBJECT_ATTRIBUTES oa = {sizeof oa, 0, FileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE};
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwOpenFile,L"ZwOpenFile",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwCreateSection,L"ZwCreateSection",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwMapViewOfSection,L"ZwMapViewOfSection",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenFile &&
		RZwCreateSection &&
		RZwMapViewOfSection &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	*ModuleBase=NULL;

	status=RZwOpenFile(
		&hFile, 
		FILE_EXECUTE | SYNCHRONIZE, 
		&oa, 
		&iosb, 
		FILE_SHARE_READ, 
		FILE_SYNCHRONOUS_IO_NONALERT);

	if(!NT_SUCCESS(status))
	{
		if (DebugOn)
			KdPrint(("ZwOpenFile failed\n"));
		return NULL;
	}
	oa.ObjectName = 0;

	status=RZwCreateSection(
		&hSection, 
		SECTION_ALL_ACCESS, 
		&oa, 
		0,
		PAGE_EXECUTE, 
		SEC_IMAGE, 
		hFile);
	if(!NT_SUCCESS(status))
	{
		RZwClose(hFile);
		KdPrint(("ZwCreateSection failed:%d\n",RtlNtStatusToDosError(status)));
		return NULL;
	}

	status=RZwMapViewOfSection(
		hSection, 
		NtCurrentProcess(),
		&BaseAddress, 
		0,
		1000, 
		0,
		&size,
		(SECTION_INHERIT)1,
		MEM_TOP_DOWN, 
		PAGE_READWRITE); 
	if(!NT_SUCCESS(status))
	{
		RZwClose(hFile);
		RZwClose(hSection);

		if (DebugOn)
			KdPrint(("ZwMapViewOfSection failed %d\n",RtlNtStatusToDosError(status)));
		return NULL;
	}
	RZwClose(hFile);
	__try
	{
		*ModuleBase=BaseAddress;
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return NULL;
	}
	return hSection;
}
BOOL GetFunctionNameByIndex(ULONG ulModuleBase,int *Index,CHAR *lpszFunctionName)
{
	UNICODE_STRING wsNtDllString;

	HANDLE hNtSection;
	ULONG ulNtDllModuleBase;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;

	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,position;
	char* functionName;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	BOOL bRetOK = FALSE;
	BOOL bInit = FALSE;

	__try
	{
		//NtDllHeader=(PIMAGE_NT_HEADERS)GetPeHead((ULONG)ulNtDllModuleBase);
		ulNtDllModuleBase = (PIMAGE_DOS_HEADER)ulModuleBase;
		pDosHeader=(PIMAGE_DOS_HEADER)ulNtDllModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
		{
			if (DebugOn)
				KdPrint(("failed to find NtHeader\r\n"));
			return bRetOK;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
		{
			if (DebugOn)
				KdPrint(("failed to find NtHeader\r\n"));
			return bRetOK;
		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulNtDllModuleBase + opthdr.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulNtDllModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			functionName = (char*)( (BYTE*)ulNtDllModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
			functionAddress = (DWORD)((BYTE*)ulNtDllModuleBase + arrayOfFunctionAddresses[functionOrdinal]);
			position  = *((WORD*)(functionAddress + 1));  //得到服务号

			if (*Index == position)
			{
				if (DebugOn)
					KdPrint(("search success[%s]",functionName));

				strcat(lpszFunctionName,functionName);
				bRetOK = TRUE;
				break;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){
		KdPrint(("EXCEPTION_EXECUTE_HANDLER[%08x]",GetExceptionCode()));
	}
	return bRetOK;
}
BOOL GetFunctionIndexByName(CHAR *lpszFunctionName,int *Index)
{
	UNICODE_STRING wsNtDllString;

	HANDLE hNtSection;
	ULONG ulNtDllModuleBase;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;

	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,position;
	char* functionName;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	BOOL bRetOK = FALSE;
	BOOL bInit = FALSE;

	STRING lpszSearchFunction;
	STRING lpszFunction;

	__try
	{
		ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
		ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
		if (RRtlInitUnicodeString &&
			RZwClose)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return NULL;

		RRtlInitUnicodeString(&wsNtDllString,L"\\SystemRoot\\System32\\ntdll.dll");
		hNtSection = MapFileAsSection(&wsNtDllString,&ulNtDllModuleBase);  //载入到内存
		if (!hNtSection)
		{
			return bRetOK;
		}
		RZwClose(hNtSection);

		//NtDllHeader=(PIMAGE_NT_HEADERS)GetPeHead((ULONG)ulNtDllModuleBase);
		pDosHeader=(PIMAGE_DOS_HEADER)ulNtDllModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
		{
			if (DebugOn)
				KdPrint(("failed to find NtHeader\r\n"));
			return bRetOK;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
		{
			if (DebugOn)
				KdPrint(("failed to find NtHeader\r\n"));
			return bRetOK;
		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulNtDllModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulNtDllModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			functionName = (char*)( (BYTE*)ulNtDllModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
			functionAddress = (DWORD)((BYTE*)ulNtDllModuleBase + arrayOfFunctionAddresses[functionOrdinal]);
			position  = *((WORD*)(functionAddress + 1));  //得到服务号

			RtlInitString(&lpszSearchFunction,functionName);
			RtlInitString(&lpszFunction,lpszFunctionName);
			if (RtlCompareString(&lpszSearchFunction,&lpszFunction,TRUE) == 0)
			{
				if (DebugOn)
					KdPrint(("Find FunctionName:%s\r\nposition:%d\r\n",functionName,position));
				*Index = position;
				bRetOK = TRUE;
				break;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return bRetOK;
}
BOOL IsAddressInSystem(ULONG ulDriverBase,ULONG *ulSysModuleBase,ULONG *ulSize,char *lpszSysModuleImage)
{
	NTSTATUS status;
	ULONG NeededSize,i;
	PMODULES pModuleList;
	BOOL bRet = FALSE;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwQuerySystemInformation,L"ZwQuerySystemInformation",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RZwQuerySystemInformation &&
		RExAllocatePool &&
		RExFreePool)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	__try
	{
		status=RZwQuerySystemInformation(
			SystemModuleInformation,
			NULL,
			0,
			&NeededSize);
		if (status!=STATUS_INFO_LENGTH_MISMATCH)
		{
			//KdPrint(("ZwQuerySystemInformation failed:%d",RtlNtStatusToDosError(status)));
			return bRet;
		}
		pModuleList=(PMODULES)RExAllocatePool(NonPagedPool,NeededSize);
		if (pModuleList)
		{
			status=RZwQuerySystemInformation(
				SystemModuleInformation,
				pModuleList,
				NeededSize,
				&NeededSize);

			if (NT_SUCCESS(status))
			{
				for (i=0;i<pModuleList->ulCount;i++)
				{
					if (ulDriverBase > pModuleList->smi[i].Base && ulDriverBase < pModuleList->smi[i].Base + pModuleList->smi[i].Size)
					{
						bRet = TRUE;
						__try
						{
							*ulSysModuleBase = pModuleList->smi[i].Base;
							*ulSize = pModuleList->smi[i].Size;

							//memset(lpszSysModuleImage,0,sizeof(lpszSysModuleImage));
							memcpy(lpszSysModuleImage,pModuleList->smi[i].ImageName,strlen(pModuleList->smi[i].ImageName));

						}__except(EXCEPTION_EXECUTE_HANDLER){

						}
						break;
					}
				}
			}
			//else
			//	KdPrint(("@@ZwQuerySystemInformation failed:%d",RtlNtStatusToDosError(status)));

			RExFreePool(pModuleList);
			pModuleList = NULL;
		}
		//else
		//	KdPrint(("ExAllocatePool failed"));
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			KdPrint(("IsAddressInSystem:%08x\r\n",GetExceptionCode()));
	}
	if (pModuleList)
		RExFreePool(pModuleList);

	return bRet;
}
//0是char *
//1是wchar*
ULONG GetSystemRoutineAddress(int IntType,PVOID lpwzFunction)
{
	ULONG ulFunction;
	UNICODE_STRING UnicodeFunctionString;
	ANSI_STRING AnsiFunctionString;
	int index;

	__try
	{
		ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
		ReLoadNtosCALL(&RMmGetSystemRoutineAddress,L"MmGetSystemRoutineAddress",SystemKernelModuleBase,ImageModuleBase);
		if (RRtlInitUnicodeString &&
			RMmGetSystemRoutineAddress)
		{
			if (IntType == 1)
			{
				RRtlInitUnicodeString(&UnicodeFunctionString,lpwzFunction);

			}else if (IntType == 0)
			{
				RtlInitAnsiString(&AnsiFunctionString,lpwzFunction);
				RtlAnsiStringToUnicodeString(&UnicodeFunctionString,&AnsiFunctionString,TRUE);
			}else
			{
				return NULL;
			}
			ulFunction = (ULONG)RMmGetSystemRoutineAddress(&UnicodeFunctionString);
			if (IntType == 0)
			{
				RtlFreeUnicodeString(&UnicodeFunctionString);
			}
			if (MmIsAddressValidEx(ulFunction))
			{
				if (IntType == 0)
				{
					if (*((char *)lpwzFunction) == 'Z' &&
						*((char *)lpwzFunction+1) == 'w')
					{
						index=*(DWORD*)(ulFunction+1);
						//是ssdt服务号的范围才返回ssdt表里面的地址
						if (index <= OriginalServiceDescriptorTable->TableSize)
						{
							ulFunction = OriginalServiceDescriptorTable->ServiceTable[index] - (ULONG)ImageModuleBase + SystemKernelModuleBase;
						}
					}
				}
				if (IntType == 1)
				{
					if (*((WCHAR *)lpwzFunction) == 'Z' &&
						*((WCHAR *)lpwzFunction+1) == 'w')
					{
						index=*(DWORD*)(ulFunction+1);
						if (index <= OriginalServiceDescriptorTable->TableSize)
						{
							ulFunction = OriginalServiceDescriptorTable->ServiceTable[index] - (ULONG)ImageModuleBase + SystemKernelModuleBase;
						}
					}
				}
				//return ulFunction;
				if (MmIsAddressValidEx(ulFunction))
				{
					return ulFunction;
				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return NULL;
}
NTSTATUS LookupProcessByName(
	IN PCHAR pcProcessName,
	OUT PEPROCESS *pEprocess
	)
{ 
	NTSTATUS	status;
	ULONG		uCount = 0;
	ULONG		uLength = 0;
	PLIST_ENTRY	pListActiveProcess;
	PEPROCESS	pCurrentEprocess = NULL;
	ULONG ulNextProcess = NULL;
	ULONG g_Offset_Eprocess_Flink;
	WIN_VER_DETAIL WinVer;
	char lpszProName[100];
	char *lpszAttackProName = NULL;


	if (!ARGUMENT_PRESENT(pcProcessName) || !ARGUMENT_PRESENT(pEprocess))
	{
		return STATUS_INVALID_PARAMETER;
	}
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return STATUS_UNSUCCESSFUL;
	}
	uLength = strlen(pcProcessName);

	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
		g_Offset_Eprocess_Flink = 0x88;
		break;
	case WINDOWS_VERSION_7_7600_UP:
	case WINDOWS_VERSION_7_7000:
		g_Offset_Eprocess_Flink = 0xb8;
		break;
	case WINDOWS_VERSION_VISTA_2008:
		g_Offset_Eprocess_Flink = 0x0a0;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		g_Offset_Eprocess_Flink = 0x98;
		break;
	case WINDOWS_VERSION_2K3:
		g_Offset_Eprocess_Flink = 0x088;
		break;
	}
	if (!g_Offset_Eprocess_Flink){
		return STATUS_UNSUCCESSFUL;
	}

	pCurrentEprocess = PsGetCurrentProcess();
	ulNextProcess = pCurrentEprocess;


	__try
	{
		memset(lpszProName,0,sizeof(lpszProName));
		if (uLength > 15)
		{
			strncat(lpszProName,pcProcessName,15);
		}
		while(1)
		{
			lpszAttackProName = NULL;
			lpszAttackProName = (char *)PsGetProcessImageFileName(pCurrentEprocess);

			if (uLength > 15)
			{
				if (lpszAttackProName &&
					strlen(lpszAttackProName) == uLength)
				{
					if(_strnicmp(lpszProName,lpszAttackProName, uLength) == 0)
					{
						*pEprocess = pCurrentEprocess;
						status = STATUS_SUCCESS;
						break;
					}
				}
			}
			else
			{
				if (lpszAttackProName &&
					strlen(lpszAttackProName) == uLength)
				{
					if(_strnicmp(pcProcessName,lpszAttackProName, uLength) == 0)
					{
						*pEprocess = pCurrentEprocess;
						status = STATUS_SUCCESS;
						break;
					}
				}
			}
			if ((uCount >= 1) && (ulNextProcess == pCurrentEprocess))
			{
				*pEprocess = 0x00000000;
				status = STATUS_NOT_FOUND;
				break;
			}
			pListActiveProcess = (LIST_ENTRY *)((ULONG)pCurrentEprocess + g_Offset_Eprocess_Flink);
			(ULONG)pCurrentEprocess = (ULONG)pListActiveProcess->Flink;
			(ULONG)pCurrentEprocess = (ULONG)pCurrentEprocess - g_Offset_Eprocess_Flink;
			uCount++;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		KdPrint(("LookupProcessByName:%08x\r\n",GetExceptionCode()));
		status = STATUS_NOT_FOUND;
	}
	return status;
}

BOOLEAN ValidateUnicodeString(PUNICODE_STRING usStr)
{
	ULONG i;

	__try
	{
		if (!MmIsAddressValidEx(usStr))
		{
			return FALSE;
		}
		if (usStr->Buffer == NULL || usStr->Length == 0)
		{
			return FALSE;
		}
		for (i = 0; i < usStr->Length; i++)
		{
			if (!MmIsAddressValidEx((PUCHAR)usStr->Buffer + i))
			{
				return FALSE;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return TRUE;
}
NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size)
{
	PMDL  pSrcMdl, pDstMdl;
	PUCHAR pSrcAddress, pDstAddress;
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	ULONG r;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RIoAllocateMdl,L"IoAllocateMdl",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RMmBuildMdlForNonPagedPool,L"MmBuildMdlForNonPagedPool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RMmProbeAndLockPages,L"MmProbeAndLockPages",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RMmUnlockPages,L"MmUnlockPages",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoFreeMdl,L"IoFreeMdl",SystemKernelModuleBase,ImageModuleBase);
	if (RIoAllocateMdl &&
		RMmBuildMdlForNonPagedPool &&
		RMmProbeAndLockPages &&
		RMmUnlockPages &&
		RIoFreeMdl)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	pSrcMdl = RIoAllocateMdl(SrcAddr, Size, FALSE, FALSE, NULL);
	if (MmIsAddressValidEx(pSrcMdl))
	{
		RMmBuildMdlForNonPagedPool(pSrcMdl);
		pSrcAddress = MmGetSystemAddressForMdlSafe(pSrcMdl, NormalPagePriority);
		if (MmIsAddressValidEx(pSrcAddress))
		{
			pDstMdl = RIoAllocateMdl(DstAddr, Size, FALSE, FALSE, NULL);
			if (MmIsAddressValidEx(pDstMdl))
			{
				__try
				{
					RMmProbeAndLockPages(pDstMdl, KernelMode, IoWriteAccess);
					pDstAddress = MmGetSystemAddressForMdlSafe(pDstMdl, NormalPagePriority);
					if (MmIsAddressValidEx(pDstAddress))
					{
						RtlZeroMemory(pDstAddress,Size);
						RtlCopyMemory(pDstAddress, pSrcAddress, Size);
						st = STATUS_SUCCESS;
					}
					RMmUnlockPages(pDstMdl);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{                 
					if (pDstMdl)
						RMmUnlockPages(pDstMdl);

					if (pDstMdl)
						RIoFreeMdl(pDstMdl);

					if (pSrcMdl)
						RIoFreeMdl(pSrcMdl);

					return GetExceptionCode();
				}
				RIoFreeMdl(pDstMdl);
			}
		}            
		RIoFreeMdl(pSrcMdl);
	}
	return st;
}
PEPROCESS GetEprocessFromPid(HANDLE Pid)
{
	HANDLE hProcess;
	NTSTATUS status;
	OBJECT_ATTRIBUTES ObjectAttributes;
	PEPROCESS Process=NULL;
	CLIENT_ID ClientId={0};

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwOpenProcess,L"ZwOpenProcess",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RObReferenceObjectByHandle,L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenProcess &&
		RObReferenceObjectByHandle &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	ClientId.UniqueProcess = Pid;
	InitializeObjectAttributes(
		&ObjectAttributes,
		NULL,
		0,
		NULL, 
		NULL
		);
	status=RZwOpenProcess(
		&hProcess,
		PROCESS_ALL_ACCESS,
		&ObjectAttributes,
		&ClientId
		);
	if (!NT_SUCCESS(status))
	{
		return NULL;
	}
	status = RObReferenceObjectByHandle(
		hProcess,
		PROCESS_ALL_ACCESS,
		*PsProcessType,
		KernelMode,
		(PVOID *)&Process,
		NULL);
	if (!NT_SUCCESS(status))
	{
		RZwClose(hProcess);
		return NULL;
	}
	ObDereferenceObject(Process);
	RZwClose(hProcess);
	return Process;
}
ULONG GetInheritedProcessPid(PEPROCESS Eprocess)
{
	NTSTATUS status;
	PROCESS_BASIC_INFORMATION pbi;
	HANDLE hProcess;
	ULONG pid=NULL;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RObOpenObjectByPointer,L"ObOpenObjectByPointer",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwQueryInformationProcess,L"ZwQueryInformationProcess",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RObOpenObjectByPointer &&
		RZwQueryInformationProcess &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
	{
		if (DebugOn)
			KdPrint(("bInit failed:%x %x %x",RObOpenObjectByPointer,RZwQueryInformationProcess,RZwClose));

		return NULL;
	}
	//无效的进程对象，或者已经退出的进程，都直接返回
	if (!MmIsAddressValidEx(Eprocess) ||
		!IsExitProcess(Eprocess)){
		return NULL;
	}
	status = RObOpenObjectByPointer(
		Eprocess,          // Object    
		OBJ_KERNEL_HANDLE,  // HandleAttributes    
		NULL,               // PassedAccessState OPTIONAL    
		PROCESS_ALL_ACCESS,       // DesiredAccess    
		*PsProcessType,     // ObjectType    
		KernelMode,         // AccessMode    
		&hProcess);
	if (!NT_SUCCESS(status))
	{
		if (DebugOn)
			KdPrint(("ObOpenObjectByPointer failed:%d",RtlNtStatusToDosError(status)));
		return NULL;
	}
	status = RZwQueryInformationProcess(hProcess,
		ProcessBasicInformation,
		(PVOID)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION),
		NULL );
	if (!NT_SUCCESS(status))
	{
		RZwClose(hProcess);

		if (DebugOn)
			KdPrint(("ZwQueryInformationProcess failed:%d",RtlNtStatusToDosError(status)));
		return NULL;
	}
	if (DebugOn)
		KdPrint(("InheritedFromUniqueProcessId:%d",pbi.InheritedFromUniqueProcessId));

	pid = pbi.InheritedFromUniqueProcessId;
	RZwClose(hProcess);
	return pid;
}
BOOL KernelStatus(HANDLE hPid)
{
	HANDLE hProcess;
	NTSTATUS status;
	OBJECT_ATTRIBUTES ObjectAttributes;
	CLIENT_ID ClientId={0};
	BOOL bRetOK = FALSE;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwOpenProcess,L"ZwOpenProcess",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenProcess &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	ClientId.UniqueProcess = hPid;
	InitializeObjectAttributes(
		&ObjectAttributes,
		NULL,
		0,
		NULL, 
		NULL 
		);
	status=RZwOpenProcess(
		&hProcess,
		PROCESS_ALL_ACCESS,
		&ObjectAttributes,
		&ClientId
		);
	if (NT_SUCCESS(status))
	{
		bRetOK = TRUE;
		RZwClose(hProcess);
	}
	return bRetOK;
}
BOOL IsExitProcess(PEPROCESS Eprocess)
{
	WIN_VER_DETAIL WinVer;
	ULONG SectionObjectOffset = NULL;
	ULONG SectionObject;
	ULONG SegmentOffset = NULL;
	ULONG Segment;
	BOOL bRetOK = FALSE;

	if (!ARGUMENT_PRESENT(Eprocess) ||
		!Eprocess)
	{
		return bRetOK;
	}
	//排除system进程
	if (Eprocess == SystemEProcess){
		return TRUE;
	}
	__try
	{
                
		if (!WinVersion)
		WinVer = GetWindowsVersion();
	        else
		WinVer = WinVersion;

		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:
			SectionObjectOffset = 0x138;
			SegmentOffset=0x14;
			break;
		case WINDOWS_VERSION_2K3_SP1_SP2:
			SectionObjectOffset = 0x124;
			SegmentOffset=0x14;
			break;
		case WINDOWS_VERSION_7_7600_UP:
			SectionObjectOffset = 0x128;
			SegmentOffset=0x14;
			break;
		case WINDOWS_VERSION_7_7000:
			SectionObjectOffset = 0x128;
			SegmentOffset=0x14;
			break;
		}
		if (SegmentOffset &&
			SectionObjectOffset)
		{
			if (MmIsAddressValidEx(((ULONG)Eprocess + SectionObjectOffset)) ){
				SectionObject = *(PULONG)((ULONG)Eprocess + SectionObjectOffset);

				if (MmIsAddressValidEx(((ULONG)SectionObject + SegmentOffset))){
					Segment = *(PULONG)((ULONG)SectionObject + SegmentOffset);

					if (MmIsAddressValidEx(Segment)){
						bRetOK = TRUE;  //进程是有效的
						__leave;
					}
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		KdPrint(("%08x\r\n",GetExceptionCode()));
	}
	return bRetOK;
}
BOOL DeleteRegKey(WCHAR *ServicesKey)
{
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING RegUnicodeString;
	NTSTATUS ntStatus;
	HANDLE hRegister;
	BOOL bRetOK = FALSE;

	RtlInitUnicodeString(&RegUnicodeString,ServicesKey);
	InitializeObjectAttributes(
		&objectAttributes,
		&RegUnicodeString,
		OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,//对大小写敏感 
		NULL, 
		NULL 
		);
	ntStatus = ZwOpenKey(&hRegister,
		KEY_ALL_ACCESS,
		&objectAttributes
		);
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("ZwOpenKey\n"));

		//汗，当前键值下没有子键才能删除
		ntStatus = ZwDeleteKey(hRegister);
		if (ntStatus == STATUS_SUCCESS)
		{
			KdPrint(("ZwDeleteKey:%ws\n",ServicesKey));
			bRetOK = TRUE;
		}
		else
			KdPrint(("ZwDeleteKey failed:%d\n",RtlNtStatusToDosError(ntStatus)));

		ZwClose(hRegister);
	}
	return bRetOK;
}
BOOL Safe_CreateValueKey(PWCHAR SafeKey,ULONG Reg_Type,PWCHAR ValueName,PWCHAR Value)
{
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING RegUnicodeString,Unicode_ValueName;
	NTSTATUS ntStatus;
	HANDLE hRegister;
	ULONG ulValue_DWORD;
	ULONG ulResult=NULL;
	BOOL bRetOK = FALSE;

	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RRtlInitUnicodeString,L"RtlInitUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwCreateKey,L"ZwCreateKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwSetValueKey,L"ZwSetValueKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwFlushKey,L"ZwFlushKey",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RRtlInitUnicodeString &&
		RZwCreateKey &&
		RZwSetValueKey &&
		RZwFlushKey &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return FALSE;

	RRtlInitUnicodeString(&Unicode_ValueName,ValueName);
	RRtlInitUnicodeString(&RegUnicodeString,SafeKey);
	//初始化objectAttributes
	InitializeObjectAttributes(
		&objectAttributes,
		&RegUnicodeString,
		OBJ_CASE_INSENSITIVE,//对大小写敏感 
		NULL, 
		NULL 
		);
	//创建或带开注册表项目
	ntStatus = RZwCreateKey(
		&hRegister,
		KEY_ALL_ACCESS,
		&objectAttributes,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		&ulResult
		);
	if (NT_SUCCESS(ntStatus))
	{
		bRetOK = TRUE;

		switch (Reg_Type)
		{
		case REG_SZ:
			RZwSetValueKey(
				hRegister,
				&Unicode_ValueName,
				0,
				Reg_Type,
				Value,
				wcslen(Value)*2
				);
			break;
		case REG_EXPAND_SZ:
			RZwSetValueKey(
				hRegister,
				&Unicode_ValueName,
				0,
				Reg_Type,
				Value,
				wcslen(Value)*2
				);
			break;
		case REG_DWORD:
			ulValue_DWORD = sizeof(REG_DWORD);
			RZwSetValueKey(
				hRegister,
				&Unicode_ValueName,
				0,
				Reg_Type,
				&Value,
				sizeof(ulValue_DWORD)
				);
			break;
		}
		RZwFlushKey(hRegister);
		RZwClose(hRegister);
	}
	return bRetOK;
}
VOID KillPro(ULONG ulPidOrEprocess)
{
	PEPROCESS EProcess;
	BOOL bInit = FALSE;

	//自己结束自己，就返回
	if (ulPidOrEprocess == ProtectEProcess){
		return;
	}
	//如果大于MmUserProbeAddress，说明传入的是PEPROCESS
	if (ulPidOrEprocess > MmUserProbeAddress)
	{
		if (DebugOn)
			KdPrint(("kill by object:%08X",ulPidOrEprocess));

		if (!IsExitProcess(ulPidOrEprocess)){
			return;
		}
		EProcess = (PEPROCESS)ulPidOrEprocess;

		if (!ZeroProcessMemory(EProcess)){
			//内存清零失败，就杀线程，双重保证~
			if (IsExitProcess(EProcess)){
				if (KillProcess((ULONG)EProcess) != STATUS_SUCCESS){
					if (DebugOn)
						KdPrint(("failed\r\n"));
				}
			}
		}

	}else
	{
		if (DebugOn)
			KdPrint(("kill by pid:%d",ulPidOrEprocess));

		if (LookupProcessByPid(ulPidOrEprocess,&EProcess) == STATUS_SUCCESS)
		{
			if (!ZeroProcessMemory(EProcess)){
				//内存清零失败，就杀线程，双重保证~
				if (IsExitProcess(EProcess)){
					if (KillProcess((ULONG)EProcess) != STATUS_SUCCESS){
						if (DebugOn)
							KdPrint(("failed\r\n"));
					}
				}
			}
		}
	}
}