//myservice.cpp
#include "std.h"

char	gbuf[65536]="메모리 통계 파일\r\n";
char temp[50];

void myservice_init()
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof( si );

	BOOL b = CreateProcess( 0,				// app name
							"0636_실습.exe", // app name + commandline args
							0, 0,			// pko, tko 보안
							FALSE,			// KOHT 의 상속여부
							NORMAL_PRIORITY_CLASS, // 우선순위 | 플래그
							0, 0,			// 환경변수, 현재 디렉토리
							&si,			// startup info 정보
							&pi);	// 생성된 프로세스, 스레드의 핸들과 ID
									// 를 담을 변수.
	if ( b )
	{
		CloseHandle( pi.hProcess); 
		CloseHandle( pi.hThread);
	}
	DWORD ThreadID;

	CloseHandle(CreateThread(NULL,0,MainThread,NULL,0,&ThreadID));
}
void myservice_run()
{
}
DWORD WINAPI PipeThread(LPVOID temp)
{
	HANDLE hPipe=(HANDLE)temp;
	TCHAR szInput[50];
	DWORD dwRead, dwWritten;
	BOOL bSuc;

	// 요청을 받고 연산을 한 후 응답을 보낸다.
	for (;;) {
		bSuc=ReadFile(hPipe,szInput,50,&dwRead,NULL);
		if ((bSuc==FALSE) || (dwRead==0)) {
			break;
		}

		bSuc=WriteFile(hPipe,szInput,50,&dwWritten,NULL);
		if ((bSuc==FALSE) || (dwWritten==0)) {
			break;
		}
	}

	// 파이프 해제
	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	return 0;
}
DWORD WINAPI MainThread(LPVOID temp)
{
	HANDLE hPipe;
	DWORD ThreadID;
	BOOL bCon;

	//이름이 없는 파이프 
	for (;;) {
		// 파이프 생성
		hPipe=CreateNamedPipe("\\\\.\\pipe\\MultiPipe",PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_BYTE, 3,4096,0,1,NULL);

		// 차일드 대기. 접속되면 차일드 요청 처리 스레드 생성
		bCon=ConnectNamedPipe(hPipe,NULL);
		if ((bCon==FALSE) && (GetLastError() == ERROR_PIPE_CONNECTED))
			bCon=TRUE;
		if (bCon==TRUE) {
			CloseHandle(CreateThread(NULL,0,PipeThread,(LPVOID)hPipe,0,&ThreadID));
		} else {
			CloseHandle(hPipe);
		}
	}
	return 0;
}
