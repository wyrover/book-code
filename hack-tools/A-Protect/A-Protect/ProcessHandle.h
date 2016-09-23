#include "stdafx.h"

#pragma comment(lib,"Advapi32.lib")

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>



// TODO: 在此处引用程序需要的其他头文件


#define NT_SUCCESS(status)          ((NTSTATUS)(status)>=0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define SystemHandleInformation 0x10
#define FileNameInformation 9
typedef LONG    NTSTATUS;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG            ProcessId;
	UCHAR            ObjectTypeNumber;
	UCHAR            Flags;
	USHORT            Handle;
	PVOID            Object;
	ACCESS_MASK        GrantedAccess;
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef struct _IO_STATUS_BLOCK 
{
	DWORD Status;
	ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _FILE_NAME_INFORMATION 
{
	ULONG FileNameLength;
	WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

typedef struct _GET_HANDLE_FILENAME
{
	HANDLE FileHandle;
	TCHAR FileName[MAX_PATH];
}GET_HANDLE_FILENAME,*PGET_HANDLE_FILENAME;

typedef ULONG (WINAPI *ZWQUERYSYSTEMINFORMATION)(ULONG, PVOID, ULONG, PULONG);

typedef ULONG(WINAPI *ZWQUERYINFORMATIONFILE)(HANDLE,PIO_STATUS_BLOCK,PVOID,ULONG,ULONG);


BOOL LocateNtdllEntry ( void );
LPVOID GetSystemHandleInformation(__out ULONG *NumOfHandle,__out ULONG *ulSize);
BOOL GetHandleFileName(PGET_HANDLE_FILENAME Finfo);
UCHAR GetFileHandleType(HANDLE hFile,PSYSTEM_HANDLE_INFORMATION Info,ULONG NumOfHandle);
HANDLE DupHandle(DWORD PId, HANDLE handle);
void InitVolumeName();
wchar_t GetVolumeName(HANDLE hFile);

BOOL UpPrivilege(HANDLE hprocess,LPCTSTR lpname);

ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = NULL;
ZWQUERYINFORMATIONFILE ZwQueryInformationFile=NULL;

DWORD Volumeserial[26];

extern ULONG ulPID;
extern WCHAR lpwzNum[50];
