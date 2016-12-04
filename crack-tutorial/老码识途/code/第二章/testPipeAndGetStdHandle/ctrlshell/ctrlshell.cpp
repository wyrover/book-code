// ctrlshell.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <windows.h>


void main()
{
	STARTUPINFOA si; 	
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));	
	si.dwFlags = STARTF_USESTDHANDLES;
	//si.wShowWindow = SW_HIDE;
	si.hStdInput = (HANDLE)123456;
	si.hStdOutput = (HANDLE)888888;
	si.hStdError = (HANDLE)654321;
	//要不3个句柄都替换，要不不替换，不能只替换一部分。
	if (!CreateProcessA(NULL, "myprg.exe", NULL, NULL, true, CREATE_DEFAULT_ERROR_MODE
		, NULL, NULL, &si, &pi))
	{
		printf("cannot create myprg.exe\n");
		return;
	}	
	getchar();
}

