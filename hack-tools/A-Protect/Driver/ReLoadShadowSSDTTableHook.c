#include "ReLoadShadowSSDTTableHook.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalShadowServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE Safe_ServiceDescriptorShadowSSDTTable;

BOOL GetShadowSSDTFunctionIndexByName(char *FunctionName,int *Index)
{
	WIN_VER_DETAIL WinVer;
	CHAR lpszShadowFuncName[260];
	BOOL bRetOK = FALSE;
	int i = 0;

	for (i=0;i<1000;i++)
	{
		if (!MmIsAddressValidEx(XPProcName[i]) ||
			!MmIsAddressValidEx(Win2003ProcName[i]) ||
			!MmIsAddressValidEx(Win7ProcName[i]))
		{
			break;
		}
		memset(lpszShadowFuncName,0,sizeof(lpszShadowFuncName));
		WinVer = GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:
			memcpy(lpszShadowFuncName,XPProcName[i],strlen(XPProcName[i]));
			break;
		case WINDOWS_VERSION_2K3_SP1_SP2:
			memcpy(lpszShadowFuncName,Win2003ProcName[i],strlen(Win2003ProcName[i]));
			break;
		case WINDOWS_VERSION_7_7000:
		case WINDOWS_VERSION_7_7600_UP:
			memcpy(lpszShadowFuncName,Win7ProcName[i],strlen(Win7ProcName[i]));
			break;
		}
		if (_strnicmp(FunctionName,lpszShadowFuncName,strlen(lpszShadowFuncName)) == 0)
		{
			*Index = i;
			bRetOK = TRUE;
			break;
		}
	}
	return bRetOK;
}
BOOL SystemCallEntryShadowSSDTTableHook(char *FunctionName,int *Index,DWORD NewFuctionAddress)
{
	KIRQL oldIrql; 
	BOOL bRetOK = FALSE;
	DWORD FunctionAddress;

	if (!GetShadowSSDTFunctionIndexByName(FunctionName,Index))
	{
		return bRetOK;
	}
	__try
	{
		if (MmIsAddressValidEx(Safe_ServiceDescriptorShadowSSDTTable) &&
			MmIsAddressValidEx(Safe_ServiceDescriptorShadowSSDTTable->ServiceTable))
		{
			if (*Index >= 0 && *Index < Safe_ServiceDescriptorShadowSSDTTable->TableSize)
			{
				//因为是自己reload表，所以不需要开启cr0
// 				_asm
// 				{
// 					CLI  ;                 
// 					MOV    EAX, CR0  ;    
// 					AND EAX, NOT 10000H ;
// 					MOV    CR0, EAX;        
// 				}

				InterlockedExchange(&Safe_ServiceDescriptorShadowSSDTTable->ServiceTable[*Index],NewFuctionAddress);
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
BOOL SystemCallEntryShadowSSDTTableUnHook(int Index)
{
	KIRQL oldIrql; 

	Safe_ServiceDescriptorShadowSSDTTable->ServiceTable[Index] = 0;

	return TRUE;
}