// console_template.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>
#include "iDirectoryMonitor.h"

using namespace std;

IDirectoryMonitor* pDirMon;

typedef struct
{
	TCHAR szPath[MAX_PATH];	
} DirectoryAltered_t;

void Thread(void* pParams)
{
    
}

static void DirectoryAlteredCallback(const TCHAR *szFileName,DWORD dwAction,void *pData)
{
	std::wcout << szFileName << std::endl;
}



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    _tsetlocale(LC_ALL, _T(""));
    CreateDirectoryMonitor(&pDirMon);

	int					iDirMonitorId;

	std::wstring watch_dir = L"E:\\";

	DirectoryAltered_t	*pDirectoryAltered = NULL;

	pDirectoryAltered = (DirectoryAltered_t *)malloc(sizeof(DirectoryAltered_t));

	iDirMonitorId = pDirMon->WatchDirectory(watch_dir.c_str(),FILE_NOTIFY_CHANGE_FILE_NAME|
		FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|
		FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_LAST_ACCESS|FILE_NOTIFY_CHANGE_CREATION|
		FILE_NOTIFY_CHANGE_SECURITY,DirectoryAlteredCallback,FALSE,(void *)pDirectoryAltered);

    _beginthread(Thread, 0, NULL);

	int a;
	cin >> a;

	pDirMon->StopDirectoryMonitor(iDirMonitorId);
	pDirMon->Release();

    return 0;
}

