#include "ProcessManager.h"

ULONG_PTR ObjectHeaderSize  = 0x18;                 
ULONG_PTR ObjectTypeOffsetOf_Object_Header  = 0x8;           //ObjectType
ULONG_PTR ObjectTableOffsetOf_EPROCESS      = 0x0c4;         //HandleTable
extern ULONG_PTR SSDTDescriptor;

BOOLEAN EnumProcess(PULONG OutputBuffer,ULONG OutputSize)
{  
	NTSTATUS       Status  = STATUS_UNSUCCESSFUL;
	ULONG_PTR      ulCount = (OutputSize - sizeof(RTL_PROCESS_INFORMATION)) / sizeof(PROCESS_INFO);
	ULONG_PTR      ulLength = 0x10000;
	PVOID          SystemProcessInfor = NULL;
	ULONG          i = 0;
	BOOLEAN        bExit = FALSE;
	while ( 1 )
	{
		if (!SystemProcessInfor)
		{
			SystemProcessInfor = (PVOID)ExAllocatePoolWithTag(NonPagedPool,ulLength, 'itnA');

			memset(SystemProcessInfor,0,ulLength);

			if (!SystemProcessInfor)
			{
				return STATUS_NO_MEMORY;
			}
		}
		Status = ZwQuerySystemInformation(SystemProcessInformation,SystemProcessInfor,ulLength, 0);
		if ( Status != STATUS_INFO_LENGTH_MISMATCH)
		{
			break;
		}
		ExFreePool(SystemProcessInfor);
		SystemProcessInfor = NULL;
		ulLength *= 2;
	}

	if (Status < 0)
	{
		ExFreePool(SystemProcessInfor);
	}
	else
	{
		PRTL_SYSTEM_PROCESS_INFORMATION  SystemProcessInforTemp = (PRTL_SYSTEM_PROCESS_INFORMATION)SystemProcessInfor;
		while(SystemProcessInforTemp!=NULL)
		{
			if (ulCount >= ((PRTL_PROCESS_INFORMATION)OutputBuffer)->NumberOfProcess)
			{
				((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].ulProcessID       = (ULONG)SystemProcessInforTemp->UniqueProcessId;
			

				((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].ulEProcess = GetEProcessByProcessID((ULONG)SystemProcessInforTemp->UniqueProcessId);

				((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].ulProcessParentID = (ULONG)SystemProcessInforTemp->InheritedFromUniqueProcessId;

				((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].bHide = FALSE;

				if((ULONG)SystemProcessInforTemp->UniqueProcessId==0)
				{
					wcscpy(((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].wzProcessName,L"Idle");
					wcscpy(((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].wzProcessFileName,L"Idle");
				}
				else if((ULONG)SystemProcessInforTemp->UniqueProcessId==4)
				{
					wcscpy(((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].wzProcessFileName,L"System");
					wcscpy(((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].wzProcessName,SystemProcessInforTemp->ImageName.Buffer);
				}
				else
				{
					WCHAR wzImageFilePath[256] = {0};;
					GetProcessImagePath((ULONG)SystemProcessInforTemp->UniqueProcessId,wzImageFilePath);
					wcscpy(((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].wzProcessFileName,wzImageFilePath);
					wcscpy(((PRTL_PROCESS_INFORMATION)OutputBuffer)->Process[i].wzProcessName,SystemProcessInforTemp->ImageName.Buffer);
				}

				((PRTL_PROCESS_INFORMATION)OutputBuffer)->NumberOfProcess++;
				i++;
			}
			else
			{
				Status = STATUS_BUFFER_TOO_SMALL;
				return Status;
			}

			if(SystemProcessInforTemp->NextEntryOffset==0)
			{
				bExit = TRUE;
			}

			SystemProcessInforTemp = (PRTL_SYSTEM_PROCESS_INFORMATION)((PUCHAR)SystemProcessInforTemp+ SystemProcessInforTemp->NextEntryOffset);

			if(bExit==TRUE)
			{
				break;
			}
		}
	}

	if(SystemProcessInfor!=NULL)
	{
		ExFreePool(SystemProcessInfor);
		SystemProcessInfor = NULL;
	}

	return Status;
}


VOID ForceEnumHideProcessData(ULONG ulProcessID,PULONG OutputBuffer)
{
	CLIENT_ID Cid;    
	HANDLE    hProcess;
	NTSTATUS  Status;
	OBJECT_ATTRIBUTES  oa;
	PEPROCESS  EProcess = NULL;
	PROCESS_INFO ProcessInfor = {0};
	WCHAR*     wzTemp;
	CHAR       szProcessImageName[20] = {0};

	Cid.UniqueProcess = (HANDLE)ulProcessID;
	Cid.UniqueThread = 0;

	InitializeObjectAttributes(&oa,0,0,0,0);
	
	Status = ZwOpenProcess(&hProcess,PROCESS_ALL_ACCESS,&oa,&Cid);    //hProcess

	if (!NT_SUCCESS(Status))
	{
		return ;
	}

	Status = ObReferenceObjectByHandle(hProcess,GENERIC_ALL,NULL,KernelMode,&EProcess,NULL);

	if (!NT_SUCCESS(Status))
	{
		ZwClose(hProcess);
		return ;
	}

	ObDereferenceObject(EProcess);
	
	if(IsRealProcess(EProcess)==TRUE)
	{
		WCHAR wzImageFilePath[256]     = {0};
		ProcessInfor.ulProcessID       = ulProcessID;

		ProcessInfor.ulEProcess        = (ULONG_PTR)EProcess;

		ProcessInfor.ulProcessParentID = *(ULONG_PTR*)((ULONG_PTR)EProcess+0x14c);

		ProcessInfor.bHide             = TRUE;
		GetProcessImagePath(ulProcessID,wzImageFilePath);
		wcscpy(ProcessInfor.wzProcessFileName,wzImageFilePath);


		memcpy(szProcessImageName,(char*)PsGetProcessImageFileName(EProcess),strlen((char*)PsGetProcessImageFileName(EProcess)));     
		charToWCHAR(szProcessImageName,&(wzTemp));

		if(wzTemp!=NULL)
		{
			wcscpy(ProcessInfor.wzProcessName,wzTemp);
		}

		memcpy(OutputBuffer,&ProcessInfor,sizeof(PROCESS_INFO));


		if(wzTemp!=NULL)
		{
			ExFreePool(wzTemp);
		}
		ZwClose(hProcess);
	}
}




NTSTATUS
	GetProcessImagePath(
	ULONG   ulProcessId,
	WCHAR*  ProcessImagePath
	)
{
	NTSTATUS Status;
	HANDLE hProcess;
	PEPROCESS EProcess;
	ULONG ulRet;
	ULONG ulLength;
	PVOID Buffer;
	PUNICODE_STRING ProcessImagePathName;
	pfnZwQueryInformationProcess ZwQueryInformationProcessAddress = NULL;

	Status = PsLookupProcessByProcessId((HANDLE)ulProcessId, &EProcess);

	ZwQueryInformationProcessAddress = (pfnZwQueryInformationProcess)GetFunctionAddressByName(L"ZwQueryInformationProcess");

	if(ZwQueryInformationProcessAddress==NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}

	if (!NT_SUCCESS(Status))
	{
		return Status;
	}
	
	Status = ObOpenObjectByPointer(EProcess,          
		OBJ_KERNEL_HANDLE,  
		NULL,              
		GENERIC_READ,       
		*PsProcessType,     
		KernelMode,       
		&hProcess);
	

	if (!NT_SUCCESS(Status))
	{
		return Status;
	}

	Status = ZwQueryInformationProcessAddress( hProcess,
		ProcessImageFileName,
		NULL, 
		0, 
		&ulRet);


	if (STATUS_INFO_LENGTH_MISMATCH != Status)
	{
		return Status;

	}

	Buffer = ExAllocatePoolWithTag(PagedPool, ulRet, 'itnA');

	ulLength = ulRet;

	if (NULL == Buffer)
	{
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	Status = ZwQueryInformationProcessAddress( hProcess,
		ProcessImageFileName,
		Buffer,
		ulLength,
		&ulRet);

	if (NT_SUCCESS(Status))
	{
		ProcessImagePathName = (PUNICODE_STRING) Buffer;
		wcscpy(ProcessImagePath,ProcessImagePathName->Buffer);
	}

	ZwClose(hProcess);
	ExFreePool(Buffer);
	return Status;
}


BOOLEAN  KillProcessByProcessId(ULONG_PTR ProcessID)
{
	OBJECT_ATTRIBUTES oa =  {0};
	CLIENT_ID         Cid = {0};
	NTSTATUS          Status = STATUS_SUCCESS;
	HANDLE            hProcess = NULL;

	Cid.UniqueProcess = (HANDLE)ProcessID;
	Cid.UniqueThread = 0;

	if ((ULONG_PTR)PsGetCurrentProcessId()==ProcessID)
	{
		return STATUS_SUCCESS;
	}
	Status = ZwOpenProcess(&hProcess,GENERIC_ALL,&oa,&Cid);

	if(!(NT_SUCCESS(Status)))
	{
		return FALSE;
	}

	ZwTerminateProcess(hProcess,0);

	ZwClose(hProcess);

	return TRUE;
}





BOOLEAN DeleteFile(WCHAR* wzFilePath)
{

	NTSTATUS             Status = STATUS_SUCCESS;
	OBJECT_ATTRIBUTES    oa;
	UNICODE_STRING       uniFileName;
	WCHAR                wzTemp[128] ={0};

	memcpy(wzTemp,L"\\??\\",(wcslen(L"\\??\\")+1)*sizeof(WCHAR));
	wcscat(wzTemp,wzFilePath);

	DbgPrint("%S\r\n",wzTemp);

	RtlInitUnicodeString(&uniFileName,wzTemp);
	InitializeObjectAttributes(&oa,&uniFileName,OBJ_CASE_INSENSITIVE|OBJ_KERNEL_HANDLE,NULL,NULL);

	if(!NT_SUCCESS(ZwDeleteFile(&oa)))
	{
		return FALSE;
	}
	return TRUE;
}

ULONG_PTR  GetEProcessByProcessID(ULONG ulProcessID)
{
	NTSTATUS            Status = STATUS_SUCCESS;
	HANDLE              hProcess = NULL;
	CLIENT_ID           Cid = {0};
	OBJECT_ATTRIBUTES   oa = {0};
	PEPROCESS           EProcess;

	Cid.UniqueProcess = (HANDLE)ulProcessID;              
	Cid.UniqueThread  = (HANDLE)0;

	Status = ZwOpenProcess(&hProcess,GENERIC_ALL,&oa,&Cid); 

		if (!NT_SUCCESS(Status))
		{
			return FALSE;
		}

		Status = ObReferenceObjectByHandle(hProcess,GENERIC_ALL,NULL,KernelMode,&EProcess,NULL);

		if (!NT_SUCCESS(Status))
		{
			ZwClose(hProcess);

			return FALSE;
		}

		ObDereferenceObject(EProcess);

		ZwClose(hProcess);
		return (ULONG_PTR)EProcess;
}


BOOLEAN IsRealProcess(PEPROCESS EProcess) 
{ 
	ULONG_PTR ObjectType; 
	ULONG_PTR    ObjectTypeAddress; 
	BOOLEAN bRet = FALSE;

	ULONG_PTR ProcessType = ((ULONG_PTR)*PsProcessType);

	if (ProcessType && EProcess && MmIsAddressValid((PVOID)(EProcess)))
	{ 
		ObjectType = KeGetObjectType((PVOID)EProcess);

		if (ObjectType && 
			ProcessType == ObjectType &&
			!IsProcessDie(EProcess))    
		{
			bRet = TRUE; 
		}
	} 

	return bRet; 
} 

ULONG_PTR KeGetObjectType(PVOID EPROCESS)
{
	ULONG_PTR ObjectType = NULL;
	ULONG SizeOfObjectHeader = 0, ObjectTypeOffset = 0, ObjectTypeAddress = 0;
	pfnObGetObjectType        ObGetObjectType = NULL;    

	if (!MmIsAddressValid ||!EPROCESS||!MmIsAddressValid(EPROCESS))
	{
		return 0;
	}

	ObjectTypeAddress = (ULONG_PTR)EPROCESS - ObjectHeaderSize + ObjectTypeOffsetOf_Object_Header;

	if (MmIsAddressValid((PVOID)ObjectTypeAddress))
	{ 
		ObjectType = *(ULONG_PTR*)ObjectTypeAddress;
	}
	
	if(!ObjectType)
	{
		ObGetObjectType = (pfnObGetObjectType)GetFunctionAddressofName(L"ObGetObjectType");   //函数



		if (ObGetObjectType)
		{
			ObjectType = ObGetObjectType(EPROCESS);
		}
	}

	return ObjectType;
}



BOOLEAN IsProcessDie(PEPROCESS EProcess)
{
	BOOLEAN bDie = FALSE;

	if (MmIsAddressValid &&
		EProcess && 
		MmIsAddressValid(EProcess) &&
		MmIsAddressValid((PVOID)((ULONG_PTR)EProcess + ObjectTableOffsetOf_EPROCESS)))
	{
		PVOID ObjectTable = *(PVOID*)((ULONG_PTR)EProcess + ObjectTableOffsetOf_EPROCESS );

		if (!ObjectTable||!MmIsAddressValid(ObjectTable) )
		{
			DbgPrint("Process is Die\r\n");
			bDie = TRUE;
		}
	}
	else
	{
		bDie = TRUE;
	}

	return bDie;
}


PVOID  GetFunctionAddressofName(WCHAR *wzFunction)
{
	UNICODE_STRING uniFunction;  
	PVOID AddrBase = NULL;

	if (wzFunction && wcslen(wzFunction) > 0)
	{
		RtlInitUnicodeString(&uniFunction, wzFunction);     
		AddrBase = MmGetSystemRoutineAddress(&uniFunction);  // 第一个模块  Ntosknrl.exe  ExportTable
	}
	return AddrBase;
}

BOOLEAN charToWCHAR(char* szSource,WCHAR** wzDest)
{
	if(szSource==NULL)
	{
		return FALSE;
	}

	*wzDest = (WCHAR*)ExAllocatePool(PagedPool,(strlen(szSource)+1)*sizeof(WCHAR));

	if(*wzDest==NULL)
	{
		return FALSE;
	}

	mbstowcs(*wzDest,szSource,(strlen(szSource)+1)*sizeof(WCHAR));

	return TRUE;
}



PVOID  GetFunctionAddressByName(WCHAR *wzFunction)
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


BOOLEAN KillProcessForcedByProcessId_InsertAPC(PEPROCESS EProcess)
{
	HANDLE          hProcess = NULL;
	NTSTATUS        Status = STATUS_SUCCESS;
	KAPC_STATE      ApcState;
	PKAPC           ExitApc;	PETHREAD        EThread;
	ULONG i;	ULONG NumOfActiveThread;        	ULONG_PTR Head;                 	ULONG_PTR ulAddress;
	NumOfActiveThread = *(ULONG *)((ULONG_PTR)EProcess+0x1a0);
	Head = (ULONG_PTR)EProcess + 0x190;	for(i=0;i<NumOfActiveThread;i++)	{		Head       = (ULONG)((PLIST_ENTRY)Head)->Flink;		ulAddress  = Head-0x22c;		EThread=(PETHREAD)ulAddress; 		ExitApc=(PKAPC)ExAllocatePool(NonPagedPool,sizeof(KAPC));		if(ExitApc==NULL)		{			return;		}		KeInitializeApc(ExitApc,			EThread,              			OriginalApcEnvironment,			KernelKillThreadRoutine,			NULL,			NULL,			KernelMode,			NULL);		Status = KeInsertQueueApc(ExitApc,ExitApc,NULL,2);	}
}


VOID KernelKillThreadRoutine(PKAPC Apc,	PKNORMAL_ROUTINE *NormalRoutine,	PVOID *NormalContext,	PVOID *SystemArgument1,	PVOID *SystemArgument2)
{
	//调用PsTerminateSystemThread结束线程	//修改当前线程的ThreadFlags为系统线程	PULONG ThreadFlags;	ExFreePool(Apc); 	ThreadFlags=(ULONG *)((ULONG)PsGetCurrentThread()+0x248); //ETHREAD中CrossThreadFlags的偏移量为0x248	if(MmIsAddressValid(ThreadFlags))	{		*ThreadFlags=(*ThreadFlags) | PS_CROSS_THREAD_FLAGS_SYSTEM;  //修改为系统权限		PsTerminateSystemThread(STATUS_SUCCESS);                     //结束系统线程，需要修改权限		//PspExitThread(STATUS_SUCCESS);                             根据PspTerminateThreadByPointer定位PspExitThread地址	}
}