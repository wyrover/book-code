#ifndef  CXX_SSDT_H
#define  CXX_SSDT_H

#include <ntifs.h>
#include <ntimage.h>

typedef struct _SYSTEM_SERVICE_TABLE
{
	PVOID ServiceTableBase;                          			
	PULONG ServiceCounterTableBase;
	ULONG NumberOfService;                       				 
	ULONG ParamTableBase;
}SYSTEM_SERVICE_TABLE,*PSYSTEM_SERVICE_TABLE;


#define SEC_IMAGE 0x01000000
NTSYSAPI PIMAGE_NT_HEADERS NTAPI RtlImageNtHeader(PVOID Base); 

NTSTATUS  MapFileInUserSpace(WCHAR* wzFilePath, HANDLE hProcess OPTIONAL,
	PVOID *BaseAddress,
	PSIZE_T ViewSize OPTIONAL);

LONG      GetSSDTFunctionIndexFromNtdll(char* szFindFunctionName);
PVOID     GetFunctionAddressByNameFormNtos(WCHAR *wzFunction);
ULONG_PTR GetFunctionAddressByIndexFromSSDT(ULONG_PTR ulIndex,ULONG_PTR SSDTDescriptor);
PVOID     GetFunctionAddressByNameFromSSDT(CHAR* szFunctionName,ULONG_PTR SSDTDescriptor);



#endif