#include <stdio.h>
#include <windows.h>

void main()
{
	// 현재 프로세스의 ID얻기 - > EPROCESS 에서 꺼내오는 함수.
	DWORD pid = GetCurrentProcessId( );
	printf("프로세스 ID : %d\n\n", pid );

	// 현재 디렉토리 얻기.
	char dir[MAX_PATH]; 
	GetCurrentDirectory( MAX_PATH, dir );
	printf("현재 디렉토리 : %s\n\n",dir );

	// Command Line argument 얻기
	char* p = GetCommandLine();
	printf("Command Line Argument : %s\n\n", p);

	// 환경 변수 꺼내기
	char temp[1024];
	GetEnvironmentVariable( "PATH", temp, 1024);

	printf("PATH라는 환경 변수 : %s\n", temp );
}











