//pipe.cpp
#include <windows.h>
#include <process.h>
#include "pipe.h"
#include <stdio.h>

HANDLE g_hWrite, g_hRead;
FUNC g_func;

void pipe_Create(FUNC func, int size)
{
	g_func = func;
	 
	HANDLE 	peer_hRead,  peer_hWrite;

	//1) 두개의 파이프 생성
	CreatePipe( &g_hRead, &peer_hWrite, 0, size);
	CreatePipe( &peer_hRead, &g_hWrite, 0, size);

	printf("%d\n", peer_hRead);
	//2) 전달핸들값 2개를 상속 가능하게 변경
	SetHandleInformation( peer_hRead, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
	SetHandleInformation( peer_hWrite, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

	//3) 명령행 인자 생성 : 프로세스 명 + 전달핸들값 + 전달핸들값 
	char cmd[256];
	wsprintf( cmd, "0636_실습.exe %d %d", peer_hRead,peer_hWrite); 
	
	//4) 프로세스 생성 
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si)};
	BOOL b = CreateProcess( 0, cmd, 0, 0, TRUE, CREATE_NEW_CONSOLE,		
		0,0,&si, &pi);
	
	if ( b )
	{
		CloseHandle( pi.hProcess);
		CloseHandle( pi.hThread);
		CloseHandle( peer_hWrite );
		CloseHandle( peer_hRead);
	}

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
//		printf("%s\n", buf );
	}
	free(buf);
	CloseHandle( g_hRead );
}

void pipe_Write(char *buf, int length)
{
	DWORD len;
	WriteFile( g_hWrite, buf, length, &len, 0);
}