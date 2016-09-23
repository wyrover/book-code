#include "ProcessHandle.h"

pfnNtQueryObject             NtQueryObjectAddress = NULL;
pfnNtSetInformationObject    NtSetInformationObjectAddress = NULL;
extern ULONG_PTR SSDTDescriptor ;

NTSTATUS EnumProcessHandle(PVOID InBuffer,PVOID OutBuffer,ULONG_PTR ulOutSize)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	PEPROCESS EProcess = NULL;
	ULONG     ulPid = 0;
	ULONG_PTR ulCount = (ulOutSize - sizeof(PROCESS_HANDLE_INFORMATION)) / sizeof(PROCESS_HANDLE_ENTRY_INFOR);


	NtQueryObjectAddress = (pfnNtQueryObject)GetFunctionAddressByNameFromSSDT("NtQueryObject",SSDTDescriptor);

	NtSetInformationObjectAddress = (pfnNtSetInformationObject)GetFunctionAddressByNameFromSSDT("NtSetInformationObject",SSDTDescriptor);

	ulPid = (ULONG)InBuffer;

	if (ulPid)
	{
		Status = PsLookupProcessByProcessId((HANDLE)ulPid, &EProcess);

		if (!NT_SUCCESS(Status))
		{
			return STATUS_UNSUCCESSFUL;
		}

         //获得进程句柄表
		Status = GetProcessHandles(ulPid, (ULONG_PTR)EProcess, (PPROCESS_HANDLE_INFORMATION)OutBuffer, ulCount);   

		ObfDereferenceObject(EProcess);

	}
	return Status;
}

NTSTATUS  GetProcessHandles(ULONG ulPid, ULONG_PTR EProcess, PPROCESS_HANDLE_INFORMATION OutBuffer, ULONG_PTR ulCount)    
{

	NTSTATUS  Status = STATUS_UNSUCCESSFUL;
	ULONG_PTR ulRet= 0x1000;
	PETHREAD  EThread = NULL;
	CHAR      PreMode = 0;
	ULONG     i = 0;

    //切换当前线程模式切换为内核模式
	EThread = PsGetCurrentThread();

	PreMode = ChangePreMode(EThread);    

	do 
	{
		PVOID Buffer = ExAllocatePool(PagedPool,ulRet);   //0x1000
		if (Buffer)
		{
			memset(Buffer, 0, ulRet);  

			//将所有的系统的句柄枚举出来
			Status = NtQuerySystemInformation(SystemHandleInformation,Buffer, ulRet, &ulRet);   
			

			if (NT_SUCCESS(Status))
			{
				PSYSTEM_HANDLE_INFORMATION  SystemHandleInfor  = (PSYSTEM_HANDLE_INFORMATION)Buffer;  
				
			
				for (i = 0; i < SystemHandleInfor->NumberOfHandles; i++)
				{
					if (ulPid == SystemHandleInfor->Handles[i].UniqueProcessId)
					{
						//ulCount  Ring0 计算的
						// OutBuffer->NumberOfHandles   Ring3  计算的
						if (ulCount > OutBuffer->NumberOfHandles)          
						{
							InsertInformationToRing3Memroy((PEPROCESS)EProcess, 
								(HANDLE)SystemHandleInfor->Handles[i].HandleValue, 
								(ULONG_PTR)SystemHandleInfor->Handles[i].Object, 
								OutBuffer);
						}

						 OutBuffer->NumberOfHandles++;   //真正多少个当前进程的句柄
					}
				}
			}
			ExFreePool(Buffer);
		}
	} while (Status == STATUS_INFO_LENGTH_MISMATCH);

	RecoverPreMode(EThread,PreMode);

	if (NT_SUCCESS(Status))
	{
		if (ulCount >=  OutBuffer->NumberOfHandles)
		{
			Status = STATUS_SUCCESS;
		}
		else
		{
			Status = STATUS_BUFFER_TOO_SMALL;
		}
	}

	return Status;
}

VOID InsertInformationToRing3Memroy(PEPROCESS EProcess, HANDLE Handle, ULONG_PTR Object, PPROCESS_HANDLE_INFORMATION OutBuffer)
{
    //切换进程空间的目的是为了获得在其进程空间的地址
	KAPC_STATE ApcState;
	PPROCESS_HANDLE_ENTRY_INFOR ProcessHandleEntryInfor = NULL;

	if (Object && MmIsAddressValid((PVOID)Object) && 
	(ProcessHandleEntryInfor = (PPROCESS_HANDLE_ENTRY_INFOR)ExAllocatePool(PagedPool,sizeof(PROCESS_HANDLE_ENTRY_INFOR))) != NULL)  
	{
		memset(ProcessHandleEntryInfor,0,sizeof(PROCESS_HANDLE_ENTRY_INFOR));

		ProcessHandleEntryInfor->Handle = (ULONG_PTR)Handle;
		ProcessHandleEntryInfor->Object = Object;

       //Object_Header

		if (MmIsAddressValid((PVOID)(Object - 0x18)))
		{
			ProcessHandleEntryInfor->ReferenceCount = *(ULONG_PTR*)((ULONG_PTR)Object - 0x18);
		}
		else
		{
			ProcessHandleEntryInfor->ReferenceCount = 0;
		}

        //切换到当前进程 重中之重
		KeStackAttachProcess(EProcess, &ApcState);
	
		GetHandleName((HANDLE)Handle,ProcessHandleEntryInfor->wzHandleName);  
 		GetHandleType((HANDLE)Handle,ProcessHandleEntryInfor->wzHandleType);        //HandleType(Event  等等)

		KeUnstackDetachProcess(&ApcState);
		
		memcpy(&OutBuffer->Handles[OutBuffer->NumberOfHandles],ProcessHandleEntryInfor,sizeof(PROCESS_HANDLE_ENTRY_INFOR));

		ExFreePool(ProcessHandleEntryInfor);

		ProcessHandleEntryInfor = NULL;
	}
}


NTSTATUS  KillProcessHandle(PVOID InBuffer)
{
	OBJECT_DATA_INFORMATION ObjectDataInformation = {0};
	NTSTATUS       Status = STATUS_UNSUCCESSFUL;
	PEPROCESS       EProcess = NULL;
	KAPC_STATE     ApcState;
	ULONG_PTR      ulRet = 0;
	
	PETHREAD EThread = NULL;
	CHAR PreMode;

	HANDLE Handle = (HANDLE)((struct _DATA_*)InBuffer)->ulHandle;



	Status = PsLookupProcessByProcessId((HANDLE)((struct _DATA_*)InBuffer)->ulProcessID,&EProcess);

	if (!NT_SUCCESS(Status))
	{
		return Status;
	}

	ObfDereferenceObject(EProcess);

	EThread = PsGetCurrentThread();
	PreMode = ChangePreMode(EThread);

	KeStackAttachProcess(EProcess, &ApcState);

	Status = NtQueryObjectAddress(Handle,HandleFlagInformation,&ObjectDataInformation,sizeof(OBJECT_DATA_INFORMATION),&ulRet);


	if(NT_SUCCESS(Status))
	{
		if (ObjectDataInformation.ProtectFromClose)
		{
			if (NtSetInformationObjectAddress)
			{
				ObjectDataInformation.ProtectFromClose = FALSE;
				Status = NtSetInformationObjectAddress((HANDLE)Handle, HandleFlagInformation, &ObjectDataInformation, sizeof(OBJECT_DATA_INFORMATION));
				if (NT_SUCCESS(Status))
				{
					Status = NtClose((HANDLE)Handle);
				}
			}
		}
		else
		{
			Status = NtClose((HANDLE)Handle);            
		}
	}

	KeUnstackDetachProcess(&ApcState);
	RecoverPreMode(EThread, PreMode);

	return Status;

}

VOID GetHandleName(HANDLE Handle,WCHAR* wzHandleName)
{
	PVOID HandleName = NULL;
	HandleName = ExAllocatePool(PagedPool,0x1000);
	if (HandleName)
	{
		ULONG    ulRet= 0;
		PETHREAD EThread = NULL;
		CHAR     PreMode = 0;
		memset(HandleName, 0, 0x1000);

		EThread = PsGetCurrentThread();
		PreMode = ChangePreMode(EThread);
		__try
		{
			if (NT_SUCCESS( NtQueryObjectAddress((HANDLE)Handle, NameInformation, HandleName, 0x1000, &ulRet)))
			{
				POBJECT_NAME_INFORMATION ObjectNameInformation = (POBJECT_NAME_INFORMATION)HandleName;
				if (ObjectNameInformation->Name.Buffer!=NULL)
				{
					if (IsUnicodeStringValid(&ObjectNameInformation->Name))
					{
						wcsncpy(wzHandleName,
						ObjectNameInformation->Name.Buffer,ObjectNameInformation->Name.Length);
					}
				}
			}
		}
		__except(1)
		{
		
		}

		RecoverPreMode(EThread, PreMode);
		ExFreePool(HandleName);
	}
}

VOID GetHandleType(HANDLE Handle,WCHAR* wzHandleType)
{
	PVOID   HandleType = NULL;
	BOOLEAN bOk = FALSE;

	HandleType = ExAllocatePool(PagedPool,0x1000);

	if (HandleType)
	{
		ULONG    ulRet   = 0;
		PETHREAD EThread = NULL;
		CHAR     PreMode = 0;

		memset(HandleType, 0, 0x1000);

		EThread = PsGetCurrentThread();
		PreMode = ChangePreMode(EThread);

		__try
		{
			if (NT_SUCCESS(NtQueryObjectAddress((HANDLE)Handle, TypeInformation, HandleType, 0x1000, &ulRet)))
			{
				POBJECT_NAME_INFORMATION ObjectNameInformation = (POBJECT_NAME_INFORMATION)HandleType;

				if (ObjectNameInformation->Name.Buffer!=NULL)
				{
					if (IsUnicodeStringValid(&ObjectNameInformation->Name))
					{
						wcsncpy(wzHandleType, ObjectNameInformation->Name.Buffer,ObjectNameInformation->Name.Length);
					}		
				}
			}
		}
		__except(1)
		{

		}

		RecoverPreMode(EThread, PreMode);
		ExFreePool(HandleType);
	}
}


BOOLEAN IsUnicodeStringValid(PUNICODE_STRING uniString)
{
	BOOLEAN bRet = FALSE;
	
	__try
	{
		if (uniString->Length > 0	&&
		uniString->Buffer		&&
		MmIsAddressValid(uniString->Buffer) &&
		MmIsAddressValid(&uniString->Buffer[uniString->Length / sizeof(WCHAR) - 1]))
		{
			bRet = TRUE;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{	
		bRet = FALSE;
	}

	return bRet;
}


