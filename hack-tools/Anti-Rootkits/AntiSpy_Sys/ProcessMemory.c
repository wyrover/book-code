#include "ProcessMemory.h"

#define HighUserAddress  0x80000000
extern ULONG_PTR SSDTDescriptor;

NTSTATUS EnumProcessMemory(ULONG ulProcessID,PVOID OutBuffer,ULONG_PTR ulOutSize)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	PEPROCESS EProcess = NULL;
	ULONG_PTR  ulCount = 0;

	if (ulProcessID)
	{
		Status = PsLookupProcessByProcessId((HANDLE)ulProcessID, &EProcess);

		if (!NT_SUCCESS(Status))
		{
			return Status;
		}
	}

	ObfDereferenceObject(EProcess);

	ulCount = (ulOutSize - sizeof(PROCESS_MEMORY_INFORMATION)) / sizeof(PROCESS_MEMORY_ENTRY_INFOR);

	Status = GetProcessMemorys(EProcess, (PPROCESS_MEMORY_INFORMATION)OutBuffer,ulCount);

	if (NT_SUCCESS(Status))
	{
		if (ulCount >= ((PPROCESS_MEMORY_INFORMATION)OutBuffer)->NumberOfMemorys)
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



NTSTATUS GetProcessMemorys(PEPROCESS EProcess, PPROCESS_MEMORY_INFORMATION OutBuffer, ULONG_PTR ulCount)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	HANDLE   hProcess = NULL;
	PROCESS_MEMORY_BASIC_INFORMATION  ProcessMemoryBasicInformation;
	ULONG_PTR ulRet = 0;
	pfnNtQueryVirtualMemory  NtQueryVirtualMemoryAddress = NULL;

	NtQueryVirtualMemoryAddress = (pfnNtQueryVirtualMemory)GetFunctionAddressByNameFromSSDT("NtQueryVirtualMemory",SSDTDescriptor);

	if(NtQueryVirtualMemoryAddress==NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}

	Status = ObOpenObjectByPointer(EProcess, 
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
		NULL, 
		GENERIC_ALL, 
		*PsProcessType, 
		KernelMode, 
		&hProcess
		);

	if (NT_SUCCESS(Status))
	{
		ULONG_PTR ulBase = 0;
		PETHREAD EThread = PsGetCurrentThread();
		CHAR PreMode     = ChangePreMode(EThread);

		while (ulBase < (ULONG_PTR)HighUserAddress)
		{
			Status = NtQueryVirtualMemoryAddress(hProcess, 
				(PVOID)ulBase, 
				MemoryBasicInformation, 
				&ProcessMemoryBasicInformation, 
				sizeof(PROCESS_MEMORY_BASIC_INFORMATION), 
				&ulRet);


			if (NT_SUCCESS(Status))
			{	
				if (ulCount > OutBuffer->NumberOfMemorys)
				{
					OutBuffer->Memorys[OutBuffer->NumberOfMemorys].ulBase       = ulBase;
					OutBuffer->Memorys[OutBuffer->NumberOfMemorys].ulSize       = ProcessMemoryBasicInformation.RegionSize;
					OutBuffer->Memorys[OutBuffer->NumberOfMemorys].ulProtect    = ProcessMemoryBasicInformation.Protect;
					OutBuffer->Memorys[OutBuffer->NumberOfMemorys].ulState      = ProcessMemoryBasicInformation.State;
					OutBuffer->Memorys[OutBuffer->NumberOfMemorys].ulType       = ProcessMemoryBasicInformation.Type;
				}

				OutBuffer->NumberOfMemorys++;
				ulBase += ProcessMemoryBasicInformation.RegionSize;
			}
			else
			{
				ulBase += PAGE_SIZE;
			}
		}

		NtClose(hProcess);
		RecoverPreMode(EThread,PreMode);
	}

	return STATUS_SUCCESS;
}


NTSTATUS ModifyMemoryProtect(PVOID InBuffer, ULONG InSize)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	PMODIFY_MEMORY ModifyMemory = (PMODIFY_MEMORY)InBuffer;
	PEPROCESS EProcess = NULL;
	ULONG_PTR ulPid = 0;
	ULONG_PTR ulBase = 0;
	ULONG_PTR ulSize = 0;
	ULONG_PTR ulNewProtect = 0;
	HANDLE  hProcess = NULL;
	pfnNtProtectVirtualMemory NtProtectVirtualMemoryAddress = (pfnNtProtectVirtualMemory)GetFunctionAddressByNameFromSSDT("NtProtectVirtualMemory",SSDTDescriptor);

	ulBase       = ModifyMemory->ulBase;
	ulSize       = ModifyMemory->ulSize;
	ulNewProtect = ModifyMemory->ulNewProtect;
	ulPid        = ModifyMemory->ulProcessID;

	if (InSize<sizeof(MODIFY_MEMORY))
	{
		return Status;
	}

	if (ulPid)
	{
		Status = PsLookupProcessByProcessId((HANDLE)ulPid, &EProcess);
		if (!NT_SUCCESS(Status))
		{
			return Status;
		}
	}

	ObfDereferenceObject(EProcess);

	Status = ObOpenObjectByPointer(EProcess, 
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
		NULL, 
		GENERIC_ALL, 
		*PsProcessType, 
		KernelMode, 
		&hProcess);

	if (NT_SUCCESS(Status))
	{
		PETHREAD EThread = PsGetCurrentThread();
		CHAR PreMode = ChangePreMode(EThread);
		ULONG ulOldProtect = 0;

		Status = NtProtectVirtualMemoryAddress(hProcess,&((PVOID)ulBase), &ulSize,ulNewProtect, &ulOldProtect);

		DbgPrint("ulNewProtect%d\r\n",ulNewProtect);
		DbgPrint("ulOldProtect%d\r\n",ulOldProtect);

		NtClose(hProcess);
		RecoverPreMode(EThread, PreMode);
	}


	return Status;
}


NTSTATUS ReadProcessMemory(PVOID InBuffer,PVOID OutBuffer)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	PREAD_MEMORY ReadMemory = (PREAD_MEMORY)InBuffer;
	ULONG ulPid      = ReadMemory->ulProcessID;
	ULONG_PTR ulBase = ReadMemory->ulBase;
	ULONG_PTR ulSize = ReadMemory->ulSize;

	if(ulBase < HighUserAddress && ulBase > 0 && ulPid && OutBuffer!=NULL)
	{
		Status = ReadRing3Memory(ulBase,ulPid,ulSize,OutBuffer);
	}

	return Status;
}


NTSTATUS ReadRing3Memory(ULONG_PTR ulBase,ULONG ulPid,ULONG ulSize,PVOID OutBuffer)
{
	NTSTATUS  Status   = STATUS_UNSUCCESSFUL;
	PEPROCESS EProcess;
	PVOID     Buffer = NULL;
	BOOLEAN   bAttach = FALSE;
	KAPC_STATE ApcState;
	if (ulPid)
	{
		Status = PsLookupProcessByProcessId((HANDLE)ulPid,&EProcess);

		if (NT_SUCCESS(Status)&& EProcess!=NULL)
		{
			ObfDereferenceObject(EProcess);


			Buffer = ExAllocatePool(PagedPool,ulSize);

			if (Buffer==NULL)
			{
				return STATUS_UNSUCCESSFUL;
			}

			memset(Buffer,0,ulSize);

			__try
			{

				KeStackAttachProcess(EProcess, &ApcState);
				bAttach = TRUE;

				ProbeForRead((PVOID)ulBase,ulSize,1);

				memcpy(Buffer,(PVOID)ulBase,ulSize);             

				if (bAttach)
				{
					KeUnstackDetachProcess(&ApcState);
					bAttach = FALSE;
				}


				memcpy(OutBuffer,Buffer,ulSize);

				if (Buffer!=NULL)
				{
					ExFreePool(Buffer);
				}

				Status = STATUS_SUCCESS;
			}
			__except(1)
			{

				if (bAttach==TRUE)
				{
					KeUnstackDetachProcess(&ApcState);
				}

				if (Buffer!=NULL)
				{
					ExFreePool(Buffer);
				}

				Status = STATUS_UNSUCCESSFUL;
			}


		}
	}

	return Status;
}