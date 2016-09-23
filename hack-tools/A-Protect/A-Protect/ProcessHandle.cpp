#include "stdafx.h"
#include "ProcessHandle.h"
#include "A-ProtectView.h"

VOID QueryProcessHandle(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	/*argv[1]=L"QQ";
		argv[2]=NULL;*/
	setlocale(LC_ALL,"chs");

	TCHAR privilege[17]=SE_DEBUG_NAME;
	HANDLE hprocess,hThread;
	hprocess=GetCurrentProcess();
	if(!UpPrivilege(hprocess,privilege))
		printf("%s","UpPrivilege failed \r\n");

	PSYSTEM_HANDLE_INFORMATION pinfo={0};
	GET_HANDLE_FILENAME pfn={0};
	//TCHAR FileName[MAX_PATH];
	ULONG ulSize,NumOfHandle;
	LPVOID tmp=NULL;
	int i = 0;

	char lpszNum[20] = {0};

	WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum)*2,NULL,FALSE);
	ulPID = atoi(lpszNum);

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描进程打开文件句柄，请稍后...");

	if(LocateNtdllEntry())
	{
		HANDLE hFile = CreateFile(_T("NUL"), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
		pinfo=(PSYSTEM_HANDLE_INFORMATION)((ULONG)GetSystemHandleInformation(&NumOfHandle,&ulSize)+4);
		UCHAR TypeNumber=GetFileHandleType(hFile,pinfo,NumOfHandle);
		for (ULONG r = 0; r < NumOfHandle; r++,pinfo++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",NumOfHandle,r);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			if(pinfo->ObjectTypeNumber==TypeNumber)
			{
				pfn.FileHandle=DupHandle(pinfo->ProcessId,(HANDLE)pinfo->Handle);
				HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)GetHandleFileName, &pfn, 0, NULL);
				if (WaitForSingleObject(hThread, 100) == WAIT_TIMEOUT)
					TerminateThread(hThread, 0);
				else 
				{
					DWORD pthread;
					GetExitCodeThread(hThread,&pthread);

// 					if(wcsstr(pfn.FileName,argv[1])>0 && pthread)
//                     
// 					WCHAR lpwzDebug[260];
// 					wsprintfW(lpwzDebug,L"STR KEY: %d   PID: %d   OPEN PATH: %s   OPEN HANDLE: 0x%X OBJECT:0x%08X\r\n",ulPID,pinfo->ProcessId,pfn.FileName,pinfo->Handle,pinfo->Object);
// 					MessageBoxW(0,lpwzDebug,0,0);

					if (ulPID == pinfo->ProcessId)
					{
						WCHAR lpwzNumber[260] = {0};
						WCHAR lpwzType[260] = {0};
						WCHAR lpwzPath[260] = {0};
						WCHAR lpwzHandle[260] = {0};
						WCHAR lpwzObject[260] = {0};
						
						memset(lpwzNumber,0,260);
						memset(lpwzType,0,260);
						memset(lpwzPath,0,260);
						memset(lpwzHandle,0,260);
						memset(lpwzObject,0,260);

						wsprintfW(lpwzNumber,L"%d",pinfo->ObjectTypeNumber);

						switch (pinfo->ObjectTypeNumber)
						{
						case 28:
							StrCatW(lpwzType,L"File");
							break;
						}
						//wsprintfW(lpwzPath,L"%ws",pinfo->ObjectTypeNumber);
// 						char lpszFile[260] = {0};
// 
// 						memset(lpszFile,0,sizeof(lpszFile));
// 						wsprintfA(lpszFile,"File --> %s",pfn.FileName);
// 						WideCharToMultiByte (CP_OEMCP,NULL,lpwzPath,-1,lpszFile,wcslen(lpwzPath)*2,NULL,FALSE);
// 
// 						OutputDebugStringW(pfn.FileName);
// 						OutputDebugStringA(lpszFile);
// 						OutputDebugStringW(lpwzPath);

						//pfn.FileName

						i++;

						wsprintfW(lpwzHandle,L"0x%08X",pinfo->Handle);
						wsprintfW(lpwzObject,L"0x%08X",pinfo->Object);

						m_list->InsertItem(0,lpwzNumber,RGB(77,77,77));
						m_list->SetItemText(0,1,lpwzType);
						m_list->SetItemText(0,2,pfn.FileName);
						m_list->SetItemText(0,3,lpwzHandle);
						m_list->SetItemText(0,4,lpwzObject);
					}
					CloseHandle(hThread);
				}
			}
		}
		WCHAR lpwzTextOut[100];
		memset(lpwzTextOut,0,sizeof(lpwzTextOut));
		wsprintfW(lpwzTextOut,L"进程所打开文件句柄扫描完毕，共有 %d 个数据",i);
		SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
	}
	return;
}


BOOL LocateNtdllEntry ( void )
{
    BOOL    ret         = FALSE;
    wchar_t    NTDLL_DLL[] = L"ntdll.dll";
    HMODULE ntdll_dll   = NULL;


    if ( ( ntdll_dll = GetModuleHandle( NTDLL_DLL ) ) == NULL )
    {
        printf("%s", "GetModuleHandle(\"ntdll.dll\") failed \r\n");
        return( FALSE );
    }
    if ( !( ZwQuerySystemInformation = ( ZWQUERYSYSTEMINFORMATION )GetProcAddress( ntdll_dll, "ZwQuerySystemInformation" ) ) )
    {
        printf( "GetProcAddress(ZwQuerySystemInformation) failed \r\n");
		ntdll_dll = NULL;
    }
	else if(!( ZwQueryInformationFile = ( ZWQUERYINFORMATIONFILE )GetProcAddress( ntdll_dll, "ZwQueryInformationFile" ) ) )
	{
		printf( "GetProcAddress(ZwQueryInformationFile) failed \r\n");
		ntdll_dll = NULL;
	}
	else ret = TRUE;

    return( ret );
}

LPVOID GetSystemHandleInformation(__out ULONG *NumOfHandle,__out ULONG *ulSize)
{
	*ulSize = 0x4000;
	ULONG ulRequired;
	NTSTATUS Status;
	LPVOID pvBuffer;

	do {
		pvBuffer = HeapAlloc(GetProcessHeap(), 0, *ulSize);

		if (!pvBuffer)
		{
			printf("HeapAlloc() failed \r\n");
			return FALSE;
		}

		Status = ZwQuerySystemInformation(SystemHandleInformation,
		pvBuffer,
		*ulSize,
		&ulRequired);

		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			HeapFree(GetProcessHeap(), 0, pvBuffer);
			*ulSize =ulRequired+25;
		}
	} while(Status == STATUS_INFO_LENGTH_MISMATCH);

	if (NT_SUCCESS(Status))
	{
		*NumOfHandle= *(ULONG*)pvBuffer;
		return pvBuffer;
	}

	HeapFree(GetProcessHeap(), 0, pvBuffer);
	return FALSE;
}

BOOL GetHandleFileName(PGET_HANDLE_FILENAME Finfo)
{
	NTSTATUS status;
	FILE_NAME_INFORMATION * psi = {0};
	IO_STATUS_BLOCK ioStatus;
	psi = (FILE_NAME_INFORMATION*)new WCHAR[sizeof(FILE_NAME_INFORMATION) + 1024];
	memset(psi, 0, (sizeof(FILE_NAME_INFORMATION) + 1024)*2);
	//psi->FileNameLength = 1024;

	status = ZwQueryInformationFile(Finfo->FileHandle, &ioStatus, psi, sizeof(FILE_NAME_INFORMATION) + 1024 * sizeof(WCHAR), FileNameInformation);
	//status = ZwQueryInformationFile(FileHandle, &ioStatus, psi, 2000, FileNameInformation);
	if(NT_SUCCESS(status))
	{
		wcscpy(Finfo->FileName,psi->FileName);
		delete[] psi;
		return TRUE;
	}
	delete[] psi;
	return FALSE;
}
UCHAR GetFileHandleType(HANDLE hFile,PSYSTEM_HANDLE_INFORMATION Info,ULONG NumOfHandle)
{

	UCHAR Result = 0;
	DWORD mypid=GetCurrentProcessId();
	
	//TCHAR namebuf[MAX_PATH];
	//GetModuleFileName(NULL,namebuf,MAX_PATH);
	//h2 = CreateFile(namebuf,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

	if (hFile != INVALID_HANDLE_VALUE)
	{

		if (Info)
		{
			for (ULONG r = 0; r < NumOfHandle; r++,Info++)
			{
				if (Info->Handle==(USHORT)hFile &&
				Info->ProcessId ==mypid )
				{
					Result = Info->ObjectTypeNumber;
					break;
				}
			}

		}
		CloseHandle(hFile);
	}
	return Result;
}


HANDLE DupHandle(DWORD PId, HANDLE handle)
{
	HANDLE DupHandle;
	HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, 0, PId);
	if(hProcess == NULL)
		return 0;

	if (!DuplicateHandle(hProcess, handle, GetCurrentProcess(), &DupHandle, 0, 0, 2))
		DupHandle = 0;

	CloseHandle( hProcess );

	return DupHandle;
}



void InitVolumeName()
{
	DWORD disk = GetLogicalDrives();
	for (int i=0; i<26; i++)
	{
		if (disk&(1<<i))
		{
			char str[] = "A-:\\";
			str[0] += i;
			GetVolumeInformationA(str, NULL, 0, &Volumeserial[i], 0, 0, 0, 0);
		}
	}
}

wchar_t GetVolumeName(HANDLE hFile)
{
	BY_HANDLE_FILE_INFORMATION info;
	if (GetFileInformationByHandle(hFile, &info))
	{
		for (int i=0; i<26; i++)
		if (info.dwVolumeSerialNumber == Volumeserial[i])
			return L'A'+i;
	}

	return L'!';
}


BOOL UpPrivilege(HANDLE hprocess,LPCTSTR lpname)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES Privileges;
    LUID luid;
    OpenProcessToken(hprocess,TOKEN_ADJUST_PRIVILEGES,&hToken);
    Privileges.PrivilegeCount=1;
    LookupPrivilegeValue(NULL,lpname,&luid);
    Privileges.Privileges[0].Luid=luid;
    Privileges.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
    if(AdjustTokenPrivileges(hToken,FALSE,&Privileges,NULL,NULL,NULL)!=0)
        return TRUE;
    return FALSE;
}