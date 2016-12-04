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
	if (!CreateProcessA(NULL, "cmd.exe", NULL, NULL, true, CREATE_DEFAULT_ERROR_MODE
		, NULL, NULL, &si, pi))
	{
		printf("cannot create cmd\n");
		return false;
	}
	CloseHandle(rh2);
	CloseHandle(wh1);	
}

bool printMsg(HANDLE readH)
{
	char output[1024];
	unsigned long n, n2;
	for (int i = 0; i < LOOP_NUM_GET_OUTPUT; i++)//循环几次将剩余的数据读完
	{
		while (true)
		{
			if (!PeekNamedPipe(readH, NULL, 0, NULL, &n2, NULL))
			{
				printf("cannot get msg\n");
				return false;
			}
			if (n2 == 0)
				break;
			if (!ReadFile(readH, output, READ_NUM, &n2, NULL))
			{
				printf("cannot read file\n");
				continue;
			}
			output[n2] = NULL;
			printf("%s", output);		
		}
		Sleep(LOOP_MS_GET_OUTPUT);//休眠一下等待剩余的数据
	}			
	return true;
}

bool getMsg(HANDLE readH, int num)
{
	char output[1024];
	unsigned long n, n2;
	while (num > 0)
	{
		if (!PeekNamedPipe(readH, NULL, 0, NULL, &n2, NULL))
		{
			printf("cannot get msg\n");
			return false;
		}
		if (n2 == 0)
		{
			return num == 0;
		}
		if (!ReadFile(readH, output, num, &n2, NULL))
		{
			printf("cannot read file\n");
			continue;
		}
		output[n2] = NULL;
		num -= n2;		
	}		
	return true;
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
	unsigned long n;
	printMsg(readH);
	while (true)
	{
		gets(cmd);
		if (strcmp(cmd, "exit") == 0)
		{
			TerminateProcess(pi.hProcess, 0);
			return;
		}
		int cmdLen = strlen(cmd);
		cmd[cmdLen] = '\n';
		WriteFile(writeH, cmd, cmdLen + 1, &n, NULL);		
		getMsg(readH, cmdLen + 1);
		if (!printMsg(readH))
		{
			TerminateProcess(pi.hProcess, 0);
			return;
		}
	}
}

