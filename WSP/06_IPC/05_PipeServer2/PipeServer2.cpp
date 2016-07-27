#include <stdio.h>
#include <windows.h>

// 익명의 Pipe : 단방향의 이름없는 pipe.(Named Pipe 는 양방향, 단방향 모두 가능)

void main()
{
	HANDLE hRead, hWrite;

	CreatePipe( &hRead, &hWrite, 0, 4096);

	// hWrite 에 쓰면 hRead에서 읽을수 있다.
	// hRead를 다른 프로세스에 전달해 주어야 한다.
	// 이미 다른 프로세스가 실행중이라면 DuplicateHandle()로
	// 새롭게 자식을 만들면서 전달하려면 KO 상속을 이용한다.

	// 읽기 위한 핸들을 상속 가능하게 한다.
	SetHandleInformation( hRead, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

	char cmd[256];
	wsprintf( cmd, "child.exe %d", hRead); // 명령형 전달인자 사용

	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si)};

	BOOL b = CreateProcess( 0, cmd, 0, 0, TRUE, CREATE_NEW_CONSOLE,
		
		0,0,&si, &pi);
	
	if ( b )
	{
		CloseHandle( pi.hProcess);
		CloseHandle( pi.hThread);
		CloseHandle( hRead );
	}
	//--------------------------------------------
	char buf[4096];
	while ( 1 )
	{
		printf("전달할 메세지를 입력하세요 >> ");
		gets( buf );

		DWORD len;
		WriteFile( hWrite, buf, strlen(buf)+1, &len, 0);
	}
}


















