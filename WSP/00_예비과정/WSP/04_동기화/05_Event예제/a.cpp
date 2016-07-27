#include <windows.h>
#include <stdio.h>

DWORD WINAPI foo(void* p)
{
	DWORD pid = (DWORD)p;
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );
	if ( hProcess == 0 )
	{
		printf("Can't Get Process Handle...\n");
		return 0;
	}
	//------------------------------------------------
	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, 0, "EXIT_EVENT");
	HANDLE h[2] = { hProcess, hEvent };

	// 리턴값을 조사하는 코드를 잘 보아 두세요..!
	DWORD ret = WaitForMultipleObjects( 2, h, FALSE, // 2중 1개라도 signal 되면
											  INFINITE );

	if ( ret == WAIT_OBJECT_0 ) // h[0] 즉 프로세스가 signal
	{
		printf("\n --  %d 번 프로세스 가 종료 되었습니다. --\n", pid);
	}
	else if ( ret == WAIT_OBJECT_0 + 1)
	{
		printf("Event가 Signal 되어서 종료 합니다.\n");
	}
	CloseHandle( hEvent );
	CloseHandle( hProcess );
	return 0;
}













//--------------------------------------------------
void main()
{
	HANDLE hEvent = CreateEvent( 0, TRUE, 0, "EXIT_EVENT");
	HANDLE hThread[1000];
	int count = 0;
	while ( 1 )
	{
		printf("감시할 프로세스 ID를 넣어 주세요 >> ");
		DWORD pid;
		scanf( "%d", &pid );
		if ( pid == -1 ) break; // 종료...
		hThread[count++] = CreateThread( 0, 0, foo, (void*)pid, 0, 0);
	}
	// 대기 중인 모든 스레드를 깨운다.
	SetEvent( hEvent );

	// 모든 스레드가 정말로 종료 할때까지 대기한다.
	WaitForMultipleObjects( count, hThread, TRUE, INFINITE );

	for ( int i = 0; i < count; ++i ) CloseHandle( hThread[i]);
}











// 계산기를 실행한후에 PID를 확인하세요.( 작업관리자나 예전에 만든 ps로)
// 현재 프로그램을 실행하고 계산기의 PID를 넣으세요..


















