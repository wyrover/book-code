#include "ReLoadSSDTTableHook.h"

extern PSERVICE_DESCRIPTOR_TABLE Safe_ServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

BOOL SystemCallEntryTableHook(char *FunctionName,int *Index,DWORD NewFuctionAddress)
{
	KIRQL oldIrql; 
	BOOL bRetOK = FALSE;
	DWORD FunctionAddress;

	if (!GetFunctionIndexByName(FunctionName,Index))
	{
		return bRetOK;
	}
	__try
	{
		if (MmIsAddressValidEx(Safe_ServiceDescriptorTable) &&
			MmIsAddressValidEx(Safe_ServiceDescriptorTable->ServiceTable))
		{
			if (*Index >= 0 && *Index < Safe_ServiceDescriptorTable->TableSize)
			{
				//因为是自己reload表，所以不需要开启cr0
// 				_asm
// 				{
// 					CLI  ;                 
// 					MOV    EAX, CR0  ;    
// 					AND EAX, NOT 10000H ;
// 					MOV    CR0, EAX;        
// 				}

				InterlockedExchange(&Safe_ServiceDescriptorTable->ServiceTable[*Index],NewFuctionAddress);
				bRetOK = TRUE;

// 				_asm 
// 				{
// 					MOV    EAX, CR0;          
// 					OR    EAX, 10000H;            
// 					MOV    CR0, EAX ;              
// 					STI;                    
// 				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return bRetOK;

}
BOOL SystemCallEntryTableUnHook(int Index)
{
	KIRQL oldIrql; 

	Safe_ServiceDescriptorTable->ServiceTable[Index] = 0;

	return TRUE;
}