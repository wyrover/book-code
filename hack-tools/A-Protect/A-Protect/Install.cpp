#include "stdafx.h"
#include "Install.h"
#include "Md5.h"
///-------------------------------------------------
extern unsigned int conv(unsigned int a);

VOID GetFileMd5Hash(char *lpszDLLPath,char *lpszMd5)
{
	FILE * fp=fopen(lpszDLLPath,"rb");
	if(fp)
	{
		MD5VAL val;
		val = md5File(fp);
		wsprintfA(lpszMd5,"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
		fclose(fp);
	}
	return;
}
void RunAProcess(char *comline)
{
	STARTUPINFOA   si;   
	memset(&si,0 ,sizeof(LPSTARTUPINFOA));   
	si.cb   =   sizeof(   LPSTARTUPINFOA   );   
	si.dwFlags   =   STARTF_USESHOWWINDOW;   
	si.wShowWindow   =   SW_SHOW;   
	PROCESS_INFORMATION   pi;   
	CreateProcessA(NULL,comline,   NULL,   NULL,   FALSE,   CREATE_NO_WINDOW,   NULL,   NULL,   &si,   &pi);
	//WaitForSingleObject(pi.hProcess, 5*1000);  //等待信号执行结束
	Sleep(3000);
	return;
}
BOOL VerifyEmbeddedSignature( LPCWSTR lpFileName )
{
	BOOL bRet = FALSE;
	WINTRUST_DATA wd = { 0 };
	WINTRUST_FILE_INFO wfi = { 0 };
	WINTRUST_CATALOG_INFO wci = { 0 };
	CATALOG_INFO ci = { 0 };

	HCATADMIN hCatAdmin = NULL;
	if ( !CryptCATAdminAcquireContext( &hCatAdmin, NULL, 0 ) )
	{
		return FALSE;
	}

	HANDLE hFile = CreateFileW( lpFileName, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 0, NULL );
	if ( INVALID_HANDLE_VALUE == hFile )
	{
		CryptCATAdminReleaseContext( hCatAdmin, 0 );
		return FALSE;
	}

	DWORD dwCnt = 100;
	BYTE byHash[100];
	CryptCATAdminCalcHashFromFileHandle( hFile, &dwCnt, byHash, 0 );
	CloseHandle( hFile );

	//LPWSTR pszMemberTag = new WCHAR[dwCnt * 2 + 1];
	//LPWSTR pszMemberTag = (WCHAR *)VirtualAlloc(0, dwCnt * 2 + 1,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	//用静态内存~！
	WCHAR pszMemberTag[260] = {0};
	for ( DWORD dw = 0; dw < dwCnt; ++dw )
	{
		wsprintfW( &pszMemberTag[dw * 2], L"%02X", byHash[dw] );
	}

	HCATINFO hCatInfo = CryptCATAdminEnumCatalogFromHash( hCatAdmin,
		byHash, dwCnt, 0, NULL );
	if ( NULL == hCatInfo )
	{
		wfi.cbStruct       = sizeof( WINTRUST_FILE_INFO );
		wfi.pcwszFilePath  = lpFileName;
		wfi.hFile          = NULL;
		wfi.pgKnownSubject = NULL;

		wd.cbStruct            = sizeof( WINTRUST_DATA );
		wd.dwUnionChoice       = WTD_CHOICE_FILE;
		wd.pFile               = &wfi;
		wd.dwUIChoice          = WTD_UI_NONE;
		wd.fdwRevocationChecks = WTD_REVOKE_NONE;
		wd.dwStateAction       = WTD_STATEACTION_IGNORE;
		wd.dwProvFlags         = WTD_SAFER_FLAG;
		wd.hWVTStateData       = NULL;
		wd.pwszURLReference    = NULL;
	}
	else
	{
		CryptCATCatalogInfoFromContext( hCatInfo, &ci, 0 );
		wci.cbStruct             = sizeof( WINTRUST_CATALOG_INFO );
		wci.pcwszCatalogFilePath = ci.wszCatalogFile;
		wci.pcwszMemberFilePath  = lpFileName;
		wci.pcwszMemberTag       = pszMemberTag;

		wd.cbStruct            = sizeof( WINTRUST_DATA );
		wd.dwUnionChoice       = WTD_CHOICE_CATALOG;
		wd.pCatalog            = &wci;
		wd.dwUIChoice          = WTD_UI_NONE;
		wd.fdwRevocationChecks = WTD_STATEACTION_VERIFY;
		wd.dwProvFlags         = 0;
		wd.hWVTStateData       = NULL;
		wd.pwszURLReference    = NULL;
	}
	GUID action = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	HRESULT hr  = WinVerifyTrust( NULL, &action, &wd );
	bRet        = SUCCEEDED( hr );

	//记得要释放啊，尼玛的不然内存飚的老高了~！！！！
	if (hCatAdmin && hCatInfo)
		 CryptCATAdminReleaseCatalogContext(hCatAdmin,hCatInfo,0);

	if (hCatAdmin)
		CryptCATAdminReleaseContext( hCatAdmin, 0 );

	//delete[] pszMemberTag;
	//VirtualFree(pszMemberTag,dwCnt * 2 + 1,MEM_RESERVE | MEM_COMMIT);

	return bRet;
}
/*
	函数功能:(请采用最大堆栈模式编译-不然无法获取)
		分离完整路径中的,路径部分

	传入参数:
		1.路径(请不要大于MAX_PATH字节)

	返回数据:
		新的字符串地址(用完以后马上Copy到一个新的字符串中-以免被覆盖!)
*/
LPSTR ExtractFilePath(LPSTR lpcFullFileName)
{
	int iLen;
	char szBuffer[MAX_PATH];

	iLen = strlen(lpcFullFileName);
	if (iLen > MAX_PATH) return NULL;

	ZeroMemory(szBuffer, MAX_PATH);
	strcpy(szBuffer, lpcFullFileName);
	lpcFullFileName = szBuffer;
	*(char *)((int)strrchr(lpcFullFileName, '\\') + 1) = 0x00;
	return lpcFullFileName;
}
//QueryUserAgent(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings","User Agent",OutBuffer);
BOOL QueryUserAgent(HKEY HKey,char *lpSubKey,char *lpValueName,char *OutBuffer)
{
	HKEY hKey1;
	DWORD dwBufSize = 256;
	DWORD dwDataType = REG_EXPAND_SZ;
	unsigned char szQueryKey[256];

	if (RegOpenKeyExA(HKey,lpSubKey,NULL,KEY_QUERY_VALUE,&hKey1) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	RegQueryValueExA(hKey1,lpValueName,NULL,&dwDataType,szQueryKey,&dwBufSize);
	RegCloseKey(hKey1);
	wsprintfA(OutBuffer,"%s",(char *)szQueryKey);

	return TRUE;
}
BOOL PrintfDosPath(__in LPCTSTR lpwzNtFullPath,__out LPCTSTR lpwzDosFullPath)
{
	char cDrive = 'A';

	for (int i=0;i<26;i++)  
	{
		memset((WCHAR *)lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
		swprintf(
			(WCHAR *)lpwzDosFullPath,
			L"%c:%s",
			cDrive+i,
			lpwzNtFullPath
			);
		if (GetFileAttributesW((WCHAR *)lpwzDosFullPath) != INVALID_FILE_ATTRIBUTES)
		{
			return TRUE;
		}
	}
	memset((WCHAR *)lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
	wcsncat((WCHAR *)lpwzDosFullPath,lpwzNtFullPath,wcslen(lpwzNtFullPath));
	return FALSE;
}
BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath)
{
	WCHAR lpwzDisk[10] = {0};
	WCHAR lpwzTemp[260] = {0};
	WCHAR lpwzNTDevice[260] = {0};
	char cDrive = 'A';

	//L"\\Device\\Harddisk\\WINDOWS\\system32\\drivers\\AProtect.sys"

	wcsncat(lpwzNTDevice,lpwzNtFilePath+wcslen(L"\\Device\\"),wcslen(lpwzNtFilePath)-wcslen(L"\\Device\\"));
	WCHAR *p = wcsstr(lpwzNTDevice,L"\\");
	if (!p)
	{
		wcsncat((WCHAR *)lpwzDosFilePath,lpwzNtFilePath,wcslen(lpwzNtFilePath));
		return FALSE;
	}
	swprintf(lpwzTemp,L"%ws",p);
	return PrintfDosPath(lpwzTemp,lpwzDosFilePath);
}
void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath)
{
	HANDLE	hFile = CreateFileW(lpwzFilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesWrite = 0;
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile,lpszBuffer,strlen(lpszBuffer), &dwBytesWrite, NULL);
	CloseHandle(hFile);
}
void SaveTitleFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath)
{
	SaveToFile(lpszBuffer,lpwzFilePath);
}
BOOL IsWindows7()   
{   
	BOOL   bOsVersionInfoEx;   
	OSVERSIONINFOEX   osvi;   

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));   
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);   
	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))   
	{   
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);   
		if(!GetVersionEx((OSVERSIONINFO*)&osvi))     
			return   FALSE;   
	}
	if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
	{
		if (osvi.dwBuildNumber == 7000 ||osvi.dwBuildNumber >= 7600)
		{
			return TRUE;
		}
	}
	return FALSE;
}

CHAR* setClipboardText(CHAR* str)
{
	if(OpenClipboard(NULL)==FALSE)
		return NULL;
	if(EmptyClipboard()==FALSE)
	{
		CloseClipboard();
		return NULL;
	}
	int sz=strlen(str)+1;
	HANDLE hMem=GlobalAlloc(GMEM_MOVEABLE, sz);
	if(hMem==NULL)
	{
		CloseClipboard();
		return NULL;
	}
	CHAR *pMem=(CHAR*)GlobalLock(hMem);
	if(pMem==NULL)
	{
		GlobalFree(hMem);
		CloseClipboard();
		return NULL;
	}
	memcpy(pMem,str,sz);
	GlobalUnlock(hMem);
	if(SetClipboardData(CF_TEXT,hMem)==FALSE)
	{
		GlobalFree(hMem);
		CloseClipboard();
		return NULL;
	}
	CloseClipboard();
	return str;
}
BOOL WINAPI EnableDebugPriv(LPCTSTR name)//提权函数
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//打开进程令牌环
	if(!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,
		&hToken
		))
	{
		//  OpenProcessToken Error
		return FALSE;
	}
	//获得进程本地唯一ID
	if(!LookupPrivilegeValue(NULL, name, &luid))
	{
		//  LookupPrivivlegeValue Error;
		return FALSE;
	}

	tp.PrivilegeCount=1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid=luid;

	//调整权限  
	if(!AdjustTokenPrivileges(
		hToken, 
		0,
		&tp, 
		sizeof(TOKEN_PRIVILEGES),
		NULL,
		NULL
		))
	{
		// AdjustTokenPrivileges Error
		return FALSE;
	}

	return TRUE;
}
ULONG GetDLLFileSize(char *lpszDLLPath)
{
	ULONG ulFileSize = NULL;

	HANDLE hFile = CreateFileA(lpszDLLPath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		0,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		ulFileSize = GetFileSize(hFile,NULL);
		ulFileSize = ulFileSize/1024;
		CloseHandle(hFile);
	}
	return ulFileSize;
}
void ShutdownWindows( DWORD dwReason )
{
	EnableDebugPriv(SE_SHUTDOWN_NAME);
	ExitWindowsEx(dwReason, 0);
	EnableDebugPriv(SE_SHUTDOWN_NAME);	
}
DWORD InstallByZwLoadDriver(LPSTR Path,LPSTR lpszServiceName)
{
	DWORD Status, Ret=1, Value=SERVICE_KERNEL_DRIVER;
	UNICODE_STRING usKey;
	HKEY hk;
	char lpszSrvForMat[MAX_PATH] = {0};
	WCHAR lpwzServiceName[MAX_PATH] = {0};
	WCHAR lpwzLoadDriverForMat[MAX_PATH] = {0};

	sprintf(lpszSrvForMat,
		"SYSTEM\\CurrentControlSet\\Services\\%s",
		lpszServiceName);

	if(RegCreateKeyExA(HKEY_LOCAL_MACHINE, 
		lpszSrvForMat, 
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hk,
		NULL)!=ERROR_SUCCESS)
	{
		printf("Error with RegCreateKeyEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}

	if(RegSetValueExA(
		hk, 
		"Type", 
		0, 
		REG_DWORD, 
		(LPBYTE)&Value,
		sizeof(DWORD))!=ERROR_SUCCESS)
	{
		printf("Error with RegSetValueEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}		

	/*
	If dwType is the REG_SZ, REG_MULTI_SZ, or REG_EXPAND_SZ type and the ANSI version of this function is used 
	(either by explicitly calling RegSetValueExA or by not defining UNICODE before including the Windows.h file),
	the data pointed to by the lpData parameter must be an ANSI character string. 
	The string is converted to Unicode before it is stored in the registry
	*/

	if(RegSetValueExA(hk, "ImagePath", 0, REG_EXPAND_SZ, (const PBYTE)Path, lstrlenA(Path))!=ERROR_SUCCESS)
	{
		printf("Error with RegSetValueEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}

	Value=SERVICE_DEMAND_START;

	if(RegSetValueExA(hk, "Start", 0, REG_DWORD, (LPBYTE)&Value, sizeof(DWORD))!=ERROR_SUCCESS)
	{
		printf("Error with RegSetValueEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}
	MultiByteToWideChar (CP_ACP,
		0, 
		lpszServiceName, 
		-1, 
		lpwzServiceName, 
		strlen(lpszServiceName)
		);
	wcscat(lpwzLoadDriverForMat,L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
	wcscat(lpwzLoadDriverForMat,lpwzServiceName);

	RtlInitUnicodeString(&usKey,lpwzLoadDriverForMat);

	Status=NtLoadDriver(&usKey);
	if(Status!=STATUS_SUCCESS)
	{	
		//printf("Error with NtLoadDriver : 0x%x : %d \n", Status, RtlNtStatusToDosError(Status));
		Ret=0;
	}

cleanup:

	RegCloseKey(hk); 

	return Ret; 
}
BOOL BFS_WriteFile(char *targetPath,unsigned char *lpszCode,ULONG ulSize)
{
	DWORD dwBytesWrite;

	HANDLE	hFile = CreateFileA(targetPath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile,lpszCode,ulSize, &dwBytesWrite, NULL);  //写入尾
	CloseHandle(hFile);
	return TRUE;
}
BOOL Install(HWND hwndDlg)
{

	DWORD dwReadByte;
	char lpszInit[8] = {0};
	char lpszWindowsPath[256] = {0};
	char lpszNumber[256] = {0};
	char lpszLoadDriverPath[256] = {0};
	int i=0;

	memset(lpszInit,0,sizeof(lpszInit));
	strcat(lpszInit,"Safe");
	ReadFile((HANDLE)SAFE_SYSTEM,lpszInit,8,&dwReadByte,0);
	if (strcmpi("hehe",lpszInit) == NULL)
	{
		goto InitSuccess;
	}
	if (strcmpi("call",lpszInit) == NULL)
	{
		if (MessageBoxA(hwndDlg,"拒绝启动\r\n\r\n原因：无法验证当前A盾文件的完整性。文件有可能被修改、感染、或者捆绑其他程序\r\n\r\n是否前往官方下载最新版？","“A盾电脑防护”",MB_ICONERROR | MB_YESNO) == IDYES)
		{
			ShellExecuteW(0,0,L"http://www.3600safe.com/",0,0,SW_SHOW);
		}
		ExitProcess(0);
	}
	char lpszAProtectRunKey[100] = {0};
	memset(lpszAProtectRunKey,0,sizeof(lpszAProtectRunKey));
	QueryUserAgent(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run","A-Protect",lpszAProtectRunKey);
	if (strstr(lpszAProtectRunKey,"\\") != 0)
	{
		//如果是开机启动的话，如果上面的无法初始化成功，就说明驱动启动失败，就不往下执行了
		MessageBoxA(hwndDlg,"“A盾电脑防护”初始化失败：\r\n\r\n1:病毒阻止了“A盾电脑防护”的启动\r\n2:某些安全软件恢复、阻止“A盾电脑防护”的钩子\r\n3:和某些杀毒或者安全软件不兼容导致“A盾电脑防护”的初始化失败\r\n4:深度防御、深度服务扫描失败，请重新启动电脑即可。","“A盾电脑防护”",MB_ICONERROR);
		ExitProcess(0);
	}
	GetWindowsDirectoryA(
		lpszWindowsPath,
		sizeof(lpszWindowsPath)
		);
	//sprintf(lpszNumber,"%d",GetTickCount());
	sprintf(lpszNumber,"%s","A-Protect");

	char lpszSrvices[256] = {0};
	sprintf(lpszSrvices,"SYSTEM\\CurrentControlSet\\Services\\%s",lpszNumber);
	SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);

	strcat(lpszWindowsPath,"\\");
	strcat(lpszWindowsPath,lpszNumber);
	strcat(lpszWindowsPath,".sys");

	BFS_WriteFile(
		lpszWindowsPath,
		lpszKernelModule,
		sizeof(lpszKernelModule)
		);
	if (GetFileAttributesA(lpszWindowsPath) == INVALID_FILE_ATTRIBUTES)
	{
		if (IsWindows7())
			MessageBoxA(hwndDlg,"释放驱动文件失败，win7系统下右键“以管理员身份运行”","“A盾电脑防护”",MB_ICONERROR);
		else
			MessageBoxA(hwndDlg,"释放驱动文件失败","“A盾电脑防护”",MB_ICONERROR);

		ExitProcess(0);
	}
	wsprintfA(
		lpszLoadDriverPath,
		"\\??\\%s",
		lpszWindowsPath
		);

	if(!EnableDebugPriv(SE_LOAD_DRIVER_NAME))
	{
		DeleteFileA(lpszWindowsPath);
		MessageBoxA(hwndDlg,"没有足够的权限加载驱动！","“A盾电脑防护”",MB_ICONERROR);
		ExitProcess(0);
	}
// 	if (!InstallByZwLoadDriver(lpszLoadDriverPath,lpszNumber))
// 	{
// 		SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);
// 
// 		if (!LoadNTDriver(lpszNumber,lpszWindowsPath)){
// 			DeleteFileA(lpszWindowsPath);
// 			SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);
// 			MessageBoxA(hwndDlg,"加载驱动失败！","“A盾电脑防护”",MB_ICONERROR);
// 			ExitProcess(0);
// 		}
// 	}
	if (!LoadNTDriver(lpszNumber,lpszWindowsPath)){
		DeleteFileA(lpszWindowsPath);
		SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);
		MessageBoxA(hwndDlg,"加载驱动失败！","“A盾电脑防护”",MB_ICONERROR);
		ExitProcess(0);
	}
	DeleteFileA(lpszWindowsPath);
	SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);

	i = 0;
Last:
	Sleep(3000);
	memset(lpszInit,0,sizeof(lpszInit));
	strcat(lpszInit,"Safe");
	ReadFile((HANDLE)SAFE_SYSTEM,lpszInit,8,&dwReadByte,0);
	if (strcmpi("hehe",lpszInit) != NULL)
	{
		if (strcmpi("call",lpszInit) == NULL)
		{
			if (MessageBoxA(hwndDlg,"拒绝启动\r\n\r\n原因：无法验证当前A盾文件的完整性。文件有可能被修改、感染、或者捆绑其他程序\r\n\r\n是否前往官方下载最新版？","“A盾电脑防护”",MB_ICONERROR | MB_YESNO) == IDYES)
			{
				ShellExecuteW(0,0,L"http://www.3600safe.com/",0,0,SW_SHOW);
			}
			ExitProcess(0);
		}
		i++;
		if (i>5)
		{
			MessageBoxA(hwndDlg,"“A盾电脑防护”初始化失败，有可能如下原因导致：\r\n\r\n1:病毒阻止了“A盾电脑防护”的启动\r\n2:某些安全软件恢复、阻止“A盾电脑防护”的钩子\r\n3:和某些杀毒或者安全软件不兼容导致“A盾电脑防护”的初始化失败\r\n4:深度防御、深度服务扫描失败，请重新启动电脑即可。","“A盾电脑防护”",MB_ICONERROR);
			SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);
			DeleteFileA(lpszWindowsPath);
			ExitProcess(0);
		}
		goto Last;
	}
InitSuccess:

	return TRUE;
}
/////////////////////////////////////////////
VOID CreateDepthSuccessKey(CHAR *KeyPath,CHAR *KeyName)
{
	HKEY regKey;
	LONG result;
	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		KeyPath,
		0,
		KEY_ALL_ACCESS,
		&regKey);

	if (SUCCEEDED(result))
	{
		HKEY	subKey;
		if (SUCCEEDED(RegCreateKeyA(regKey,KeyName, &subKey)))
		{
			RegCloseKey(subKey);
		}
		RegCloseKey(regKey);
	}
}
BOOL InstallDepthServicesScan(CHAR * serviceName)
{
	BOOL	ret = FALSE;
	LONG	result;
	CHAR	sysDirPath[MAX_PATH];
	CHAR	targetPath[MAX_PATH];
	DWORD   dwBytesWrite;

	GetSystemDirectoryA(sysDirPath, sizeof(sysDirPath));
	wsprintfA(targetPath, "%s\\Drivers\\%s.sys", sysDirPath, serviceName);
	if (GetFileAttributesA(targetPath) != INVALID_FILE_ATTRIBUTES)
	{
		return TRUE;
	}
	if (BFS_WriteFile(targetPath,lpszKernelModule,sizeof(lpszKernelModule)) == FALSE)
	{
		printf("create file failed\r\n");
		return FALSE;
	}
	if (GetFileAttributesA(targetPath) == INVALID_FILE_ATTRIBUTES)
	{
		MessageBoxW(0,L"释放文件失败，请关闭杀毒软件避免误删\r\n",L"A盾电脑防护",MB_ICONWARNING);
		return FALSE;
	}
	HKEY regKey;
	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services",
		0,
		KEY_ALL_ACCESS,
		&regKey);

	if (SUCCEEDED(result))
	{
		HKEY	subKey;
		if (SUCCEEDED(RegCreateKeyA(regKey, serviceName, &subKey)))
		{
			DWORD	data = 0x1;
#define SET_DWORD(_key, _valueName, _data) {data = _data; RegSetValueExA(_key, _valueName, NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));}
			data = 0x1;

			SET_DWORD(subKey, "ErrorControl", SERVICE_ERROR_NORMAL);
			SET_DWORD(subKey, "Start", SERVICE_BOOT_START);
			SET_DWORD(subKey, "Type", SERVICE_KERNEL_DRIVER);
			SET_DWORD(subKey, "Tag", 10);
			RegFlushKey(subKey);
			RegCloseKey(subKey);
		}
		RegCloseKey(regKey);
	}

	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}",
		0,
		KEY_READ|KEY_WRITE,
		&regKey);

	if (SUCCEEDED(result))
	{
		CHAR buff[1024];
		DWORD retLen = sizeof(buff);
		ULONG type = REG_MULTI_SZ;

		memset( buff, 0, sizeof(buff));

		RegQueryValueExA( regKey,
			"UpperFilters",
			0,
			&type,
			(LPBYTE)buff, 
			&retLen);

		BOOL	alreadyExists = FALSE;
		CHAR * ptr = NULL;
		for (ptr = buff; *ptr; ptr += lstrlenA(ptr) + 1)
		{
			if(lstrcmpiA(ptr, serviceName) == 0)
			{
				alreadyExists = TRUE;
				break;
			}
		}
		if (!alreadyExists)
		{
			DWORD	added = lstrlenA(serviceName);
			memcpy(ptr, serviceName, added * sizeof(CHAR));

			ptr += added;

			*ptr = '\0';
			*(ptr+1) = '\0';

			result = RegSetValueExA(regKey, "UpperFilters", 0, REG_MULTI_SZ, (LPBYTE)buff, retLen + ((added + 1) * sizeof(CHAR)));
		}

		ret = TRUE;

		RegCloseKey(regKey);
	}

	return ret;
}
BOOL UninstallDepthServicesScan(CHAR * serviceName)
{
	BOOL	ret = FALSE;

	CHAR	sysDirPath[MAX_PATH];
	CHAR	targetPath[MAX_PATH];

	GetSystemDirectoryA(sysDirPath, sizeof(sysDirPath));
	wsprintfA(targetPath, "%s\\Drivers\\%s.sys", sysDirPath, serviceName);

	DeleteFileA(targetPath);

	HKEY regKey;
	LONG result;
	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services",
		0,
		KEY_READ|KEY_WRITE,
		&regKey);
	if( ERROR_SUCCESS == result )
	{
		SHDeleteKeyA(regKey, serviceName);
		// 一定要flush,否则不保存
		RegFlushKey(regKey);
	}

	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}",
		0,
		KEY_READ|KEY_WRITE,
		&regKey);

	if( ERROR_SUCCESS == result )
	{
		CHAR buff[1024];
		DWORD retLen = sizeof(buff);
		ULONG type = REG_MULTI_SZ;

		memset( buff, 0, sizeof(buff));

		RegQueryValueExA( regKey,
			"UpperFilters",
			0,
			&type,
			(LPBYTE)buff, 
			&retLen);

		BOOL	alreadyExists = FALSE;
		for (CHAR * ptr = buff; *ptr; ptr += lstrlenA(ptr) + 1)
		{
			if(lstrcmpiA(ptr, serviceName) == 0)
			{
				DWORD	removeLength = (lstrlenA(ptr) + 1) * sizeof(CHAR);
				memmove(ptr, (char *)ptr + removeLength, ((char *)ptr + removeLength - (char *)buff) *  sizeof(CHAR));

				result = RegSetValueExA(regKey,"UpperFilters", 0, REG_MULTI_SZ, (LPBYTE)buff, retLen - removeLength);
				break;
			}
		}

		ret = TRUE;
		RegFlushKey(regKey);
		RegCloseKey(regKey);
	}
	return ret;
}
BOOL LoadNTDriver(char* lpszDriverName,char* lpszDriverPath)
{
	char szDriverImagePath[256];
	//得到完整的驱动路径
	GetFullPathNameA(lpszDriverPath, 256, szDriverImagePath, NULL);

	BOOL bRet = FALSE;

	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄

	//打开服务控制管理器
	hServiceMgr = OpenSCManagerA( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if( hServiceMgr == NULL )  
	{
		//OpenSCManager失败
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		////OpenSCManager成功
		printf( "OpenSCManager() ok ! \n" );  
	}

	//创建驱动所对应的服务
	hServiceDDK = CreateServiceA( hServiceMgr,
		lpszDriverName, //驱动程序的在注册表中的名字  
		lpszDriverName, // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
		SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
		SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
		SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
		szDriverImagePath, // 注册表驱动程序的 ImagePath 值  
		NULL,  
		NULL,  
		NULL,  
		NULL,  
		NULL);  

	DWORD dwRtn;
	//判断服务是否失败
	if( hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			//由于其他原因创建服务失败
			printf( "CrateService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{
			//服务创建失败，是由于服务已经创立过
			printf( "CrateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n" );  
		}

		// 驱动程序已经加载，只需要打开  
		hServiceDDK = OpenServiceA( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS );  
		if( hServiceDDK == NULL )  
		{
			//如果打开服务也失败，则意味错误
			dwRtn = GetLastError();  
			printf( "OpenService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else
		{
			printf( "OpenService() ok ! \n" );
		}
	}  
	else  
	{
		printf( "CrateService() ok ! \n" );
	}

	//开启此项服务
	bRet= StartServiceA( hServiceDDK, NULL, NULL );  
	if( !bRet )  
	{  
		DWORD dwRtn = GetLastError();  
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{  
			printf( "StartService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				//设备被挂住
				printf( "StartService() Faild ERROR_IO_PENDING ! \n");
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  
			{  
				//服务已经开启
				printf( "StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}
	bRet = TRUE;
	//离开前关闭句柄
BeforeLeave:
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	//删除注册表键值
	char lpszSrvices[256] = {0};
	sprintf(lpszSrvices,"SYSTEM\\CurrentControlSet\\Services\\%s",lpszDriverName);
	SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);

	return bRet;
}
//卸载驱动程序     
BOOL UnloadNTDriver(char * szSvrName)     
{   
	BOOL bRet = FALSE;   
	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄   
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄   
	SERVICE_STATUS SvrSta;   
	//打开SCM管理器   
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );     
	if( hServiceMgr == NULL )     
	{   
		//带开SCM管理器失败   
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );     
		bRet = FALSE;   
		goto BeforeLeave;   
	}     
	else     
	{   
		//带开SCM管理器失败成功   
		printf( "OpenSCManager() ok ! \n" );     
	}   
	//打开驱动所对应的服务   
	hServiceDDK = OpenServiceA( hServiceMgr, szSvrName, SERVICE_ALL_ACCESS );     

	if( hServiceDDK == NULL )     
	{   
		//打开驱动所对应的服务失败   
		printf( "OpenService() Faild %d ! \n", GetLastError() );     
		bRet = FALSE;   
		goto BeforeLeave;   
	}     
	else     
	{     
		printf( "OpenService() ok ! \n" );     
	}     
	//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。     
	if( !ControlService( hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) )     
	{     
		printf( "ControlService() Faild %d !\n", GetLastError() );     
	}     
	else     
	{   
		//打开驱动所对应的失败   
		printf( "ControlService() ok !\n" );     
	}     
	//动态卸载驱动程序。     
	if( !DeleteService( hServiceDDK ) )     
	{   
		//卸载失败   
		printf( "DeleteSrevice() Faild %d !\n", GetLastError() );     
	}     
	else     
	{     
		//卸载成功   
		printf( "DelServer:eleteSrevice() ok !\n" );     
	}     
	bRet = TRUE;   
BeforeLeave:   
	//离开前关闭打开的句柄   
	if(hServiceDDK)   
	{   
		CloseServiceHandle(hServiceDDK);   
	}   
	if(hServiceMgr)   
	{   
		CloseServiceHandle(hServiceMgr);   
	}   
	return bRet;       
}    