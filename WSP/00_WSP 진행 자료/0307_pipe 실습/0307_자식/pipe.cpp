//pipe.cpp
#include <windows.h>
#include <process.h>
#include "pipe.h"
#include <stdio.h>

static HANDLE g_hRead, g_hWrite;

FUNC g_func;

void pipe_Create1(HANDLE hread, HANDLE hwrite, FUNC func, int size)
{
	g_hRead		= hread;
	g_hWrite	= hwrite;
	g_func		= func;

	//5) 수신 스레드 생성 
	_beginthread(recvthread, 0, (void*)size);
}

void recvthread(void* p)
{
	int size = (int)p;
	
	char *buf = (char*)malloc(size);
	while ( 1 )
	{
		memset( buf, 0, size );
		DWORD len;
		BOOL b = ReadFile( g_hRead, buf, size, &len, 0);
		if ( b == FALSE ) break;

		g_func(buf, len);
	}
	free(buf);
	CloseHandle( g_hRead );
}


void pipe_Write(char *buf, int length)
{
	DWORD len;
	WriteFile( g_hWrite, buf, length, &len, 0);
}