// ctrlshell.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <windows.h>
#include <memory.h>
#include <string.h>

#define READ_NUM 1023
#define LOOP_NUM_GET_OUTPUT 2
#define LOOP_MS_GET_OUTPUT 100
bool CreateShell(HANDLE * readHnd, HANDLE * writeHnd, PROCESS_INFORMATION * pi)
{
	HANDLE rh1, wh1, rh2, wh2;
    STARTUPINFOA si; 
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = true;
	if (!CreatePipe(&rh1, &wh1, &sa, 1024))
	{
		printf("cannot create pipe1\n");
		return false;
	}
    *readHnd = rh1;
	if (!CreatePipe(&rh2, &wh2, &sa, 1024))
	{
		printf("cannot create pipe2\n");
		return false;
	}
    *writeHnd = wh2;
	memset(&si, 0, sizeof(si));
	
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = rh2;
	si.hStdOutput = wh1;
	si.hStdError = wh1;
	if (!CreateProcessA(NULL, "telnet.exe", NULL, NULL, true, CREATE_DEFAULT_ERROR_MODE
		, NULL, NULL, &si, pi))
	{
		printf("cannot create cmd\n");
		return false;
	}
	CloseHandle(rh2);
	CloseHandle(wh1);	
}

void main()
{
	char cmd[256];	
	HANDLE readH, writeH;
	PROCESS_INFORMATION pi;
	
	if (!CreateShell(&readH, &writeH, &pi))
	{
		return;
	}
	getchar();
}

