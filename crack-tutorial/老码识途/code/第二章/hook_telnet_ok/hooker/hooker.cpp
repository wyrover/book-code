// hooker.cpp : 定义 DLL 应用程序的导出函数。
//
#include <stdio.h>
#include <windows.h>
#include "../include/detours.h"
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>


#pragma warning(disable:4100)   // Trampolines don't use formal parameters.

#define READ_NUM 1024

HANDLE gStdInputHandle;

HANDLE gStdOutputHandle;

HANDLE gInputHandle;

HANDLE gOutputHandle;


void Log(char * msg)
{
	/*
	FILE * f;
	f = fopen("c:\\log.txt", "a");
	fwrite(msg, strlen(msg), 1, f);
	fclose(f);
	*/
}

DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleA( HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  CONST VOID *lpBuffer,	// pointer to buffer to write from 
				  DWORD nNumberOfCharsToWrite,	// number of characters to write 
				  LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
				  LPVOID lpReserved),
				  WriteConsoleA);
static BOOL WINAPI Detour_WriteConsoleA( HANDLE hConsoleOutput,	// handle to a console screen buffer 
										CONST VOID *lpBuffer,	// pointer to buffer to write from 
										DWORD nNumberOfCharsToWrite,	// number of characters to write 
										LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
										LPVOID lpReserved)
{
	Log("WriteConsoleA:");
	BOOL ret = WriteFile(gOutputHandle, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, NULL);
	Trampoline_WriteConsoleA(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);
	if (ret == FALSE)
	{
		Log("fail: WriteConsoleA\n");
	}
	return ret;
}


DETOUR_TRAMPOLINE(BOOL WINAPI Trampoline_WriteConsoleW( HANDLE hConsoleOutput,	// handle to a console screen buffer 
				  CONST VOID *lpBuffer,	// pointer to buffer to write from 
				  DWORD nNumberOfCharsToWrite,	// number of characters to write 
				  LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
				  LPVOID lpReserved),
				  WriteConsoleW);
static BOOL WINAPI Detour_WriteConsoleW( HANDLE hConsoleOutput,	// handle to a console screen buffer 
										CONST VOID *lpBuffer,	// pointer to buffer to write from 
										DWORD nNumberOfCharsToWrite,	// number of characters to write 
										LPDWORD lpNumberOfCharsWritten,	// pointer to number of characters written 
										LPVOID lpReserved)
{
	//这时的nNumberOfCharsToWrite是字符数，不是字节数，字节数因此要*2

	int bLen = nNumberOfCharsToWrite * 2; 
	int len = bLen + 2;
	char * wstr = (char *)malloc(len);
	memset(wstr, 0 , len);
	memmove(wstr, lpBuffer, bLen);
	int iTextLen;	
	iTextLen = WideCharToMultiByte( CP_ACP,
		0,
		LPCWSTR(wstr),
		-1,
		NULL,
		0,
		NULL,
		NULL );
	//上面函数返回值是写入的字节长度，因此包含了最后的空字节0，所以分配长度是不用+1
	char * pElementText = (char *)malloc(iTextLen);
	memset( pElementText, 0, sizeof( char ) * ( iTextLen) );
	WideCharToMultiByte( CP_ACP,
		0,
		LPCWSTR(wstr),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	
	int toSend = iTextLen - 1;
	BOOL ret;
	unsigned long wn;
	//其实这里可以用循环每次发两个就不会有一个2字节字符发了一半出去的问题。也不对，发两个还是可能发生只发了一个的
	//问题，只有接收方自己处理
	while (toSend > 0)
	{
		if (!WriteFile(gOutputHandle, pElementText, toSend, &wn, NULL))
		{

			Log("fail WriteConsoleW\n");
			return FALSE;
		}
		toSend -= wn;
	}
	* lpNumberOfCharsWritten = nNumberOfCharsToWrite;
	free(wstr);
	free(pElementText);

	Trampoline_WriteConsoleW(hConsoleOutput, lpBuffer, nNumberOfCharsToWrite, lpNumberOfCharsWritten, lpReserved);	
	return TRUE;
}


DWORD _stdcall inputThdHandle(void * arg)
{	
	DWORD num;
	char buf[READ_NUM];
	INPUT_RECORD inputC;	
	DWORD n;
	inputC.Event.KeyEvent.dwControlKeyState = 0;
	while (true)
	{
		
		if (!ReadFile(gInputHandle, buf, READ_NUM, &n, NULL))
		{
			Log("cannot read input...\n");
			Sleep(1000);
			continue;
		}		
		for (int i = 0; i < n; i++)
		{
			inputC.Event.KeyEvent.bKeyDown = TRUE;
			inputC.Event.KeyEvent.uChar.AsciiChar = buf[i];
			
			inputC.EventType = KEY_EVENT;
			if (!WriteConsoleInputA(gStdInputHandle, &inputC, 1, &num))
			{
				Log("cannot write input buffer\n");
				int err = GetLastError();
				char estr[255];
				memset(estr, 0, 255);
				FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, estr, 255, NULL);
				Log(estr);				
			}
			inputC.Event.KeyEvent.bKeyDown = FALSE;
			
			if (!WriteConsoleInputA(gStdInputHandle, &inputC, 1, &num))
			{
				Log("cannot write input buffer\n");
				int err = GetLastError();
				char estr[255];
				memset(estr, 0, 255);
				FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, estr, 255, NULL);
				Log(estr);				
			}
			/*
			char c[2];
			c[1] = 0;
			c[0] = buf[i];
			Log(c);
			Log("----");
			*/
		}						
		
	}
	return 0;
}

void init()
{
	Log("init begin...\n");
	gInputHandle = GetStdHandle(STD_INPUT_HANDLE);
	gOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	gStdInputHandle = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS	, FILE_ATTRIBUTE_NORMAL, NULL);	
	gStdOutputHandle = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS	, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_INPUT_HANDLE, gStdInputHandle);
	SetStdHandle(STD_OUTPUT_HANDLE, gStdOutputHandle);	

	DWORD tid;
	if (CreateThread(NULL, 0, inputThdHandle, NULL, 0, &tid) == NULL)
	{
		Log("fail to create input thread\n");
	}
	
	Log("init end...\n");
}


bool hooked = false;

void hook()
{
	if (hooked)
	{
		return;
	}
	init();
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleA, (PBYTE)Detour_WriteConsoleA);
	DetourFunctionWithTrampoline((PBYTE)Trampoline_WriteConsoleW, (PBYTE)Detour_WriteConsoleW);
	Log("hook end..\n");
}


