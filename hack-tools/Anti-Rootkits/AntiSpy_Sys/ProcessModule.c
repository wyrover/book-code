#include "ProcessModule.h"

extern ULONG_PTR SSDTDescriptor;

NTSTATUS EnumMoudleByNtQueryVirtualMemory(ULONG ProcessId,PPROCESS_MODULE_INFORMATION  OutBuffer,ULONG_PTR ulOutSize)
{
	NTSTATUS   Status;
	PEPROCESS  EProcess = NULL;
	HANDLE    hProcess = NULL;
	ULONG_PTR HighUserAddress  = 0;
	ULONG     ulRet = 0;
	ULONG_PTR ulCount = (ulOutSize - sizeof(PROCESS_MODULE_INFORMATION)) / sizeof(PROCESS_MODULE_ENTRY_INFOR);

	pfnNtQueryVirtualMemory NtQueryVirtualMemoryAddress = NULL;

	HighUserAddress = 0x80000000;

    NtQueryVirtualMemoryAddress = (pfnNtQueryVirtualMemory)GetFunctionAddressByNameFromSSDT("NtQueryVirtualMemory",SSDTDescriptor);

	if(NtQueryVirtualMemoryAddress==NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}

	if (ProcessId)
	{
		Status = PsLookupProcessByProcessId((HANDLE)ProcessId, &EProcess);
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
		&hProcess
		);
	if (NT_SUCCESS(Status))
	{
		ULONG_PTR ulBase = 0;

		PETHREAD EThread = PsGetCurrentThread();
		CHAR PreMode     = ChangePreMode(EThread);  

		do 
		{
			MEMORY_BASIC_INFORMATION mbi = {0};  //内存基本信息

			Status = NtQueryVirtualMemoryAddress(hProcess, 
				(PVOID)ulBase, 
				MemoryBasicInformation, 
				&mbi, 
				sizeof(MEMORY_BASIC_INFORMATION), 
				&ulRet);

			if (NT_SUCCESS(Status))
			{    
				if (mbi.Type==MEM_IMAGE)
				{
					if ((ULONG)mbi.AllocationBase==ulBase)
					{
						ULONG  ulEnd = 0;
						MEMORY_SECTION_NAME msn = {0};

						Status = NtQueryVirtualMemoryAddress(hProcess,
							(PVOID)ulBase,
							MemorySectionName,
							&msn,
							sizeof(MEMORY_SECTION_NAME),
							&ulRet);

						for (ulEnd = ulBase + mbi.RegionSize;
							ulEnd < 0x7FFFFFFF; 
							ulEnd += mbi.RegionSize) 
						{

							Status = NtQueryVirtualMemoryAddress(hProcess, 
								(PVOID)ulEnd,
								MemoryBasicInformation, 
								&mbi, 
								sizeof(MEMORY_BASIC_INFORMATION), 
								&ulRet);

							if (NT_SUCCESS(Status) && mbi.Type != SEC_IMAGE) 
							{
								break;
							}
						}

						if (NT_SUCCESS(Status)) 
						{
							if(ulCount > OutBuffer->NumberOfModule)
							{
								OutBuffer->Module[OutBuffer->NumberOfModule].ModuleBase =  ulBase; 
								OutBuffer->Module[OutBuffer->NumberOfModule].ModuleSize = ulEnd-ulBase;
								wcscpy(OutBuffer->Module[OutBuffer->NumberOfModule].ModulePath,msn.Name.Buffer);

								OutBuffer->NumberOfModule++;
					     	}
					    }

			    	}
				}
				ulBase += mbi.RegionSize;
			}
			else 
			{
				ulBase += PAGE_SIZE;    
			}
		} while (ulBase < (ULONG_PTR)HighUserAddress);

		NtClose(hProcess);
		RecoverPreMode(EThread,PreMode);
	}
	
	return Status;
}