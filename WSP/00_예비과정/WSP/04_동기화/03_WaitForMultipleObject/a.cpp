#include <windows.h>
#include <stdio.h>

// 지역변수는 멀티 스레드 환경에서도 안전하다 - 마음대로 사용해도 된다.
// 전역(static지역)변수는 멀티 스레드 환경에서는 안전하지 않다.
// 반드시 동기화 해야 한다.
// CriticalSection, Mutex, Semaphore, Event 등의 기법을 사용해야 한다.


void Delay() { for(int i = 0; i< 5000000; ++i); }  // 시간 지연

BOOL bWait = TRUE;

CRITICAL_SECTION cs; // 임계영역 cs 안에는 1개의 스레드만 들어 올수 있다.

DWORD WINAPI foo( void* p )
{
	char* name = (char*)p;
	static int x = 0;
	
	for ( int i = 0; i < 20; ++i )
	{
		EnterCriticalSection( &cs ); // cs에 들어 간다.
		//---------------------------------------------
		x = 100; Delay();
		x = x+1; Delay();
		printf("%s : %d\n", name, x); Delay();
		//---------------------------------------------
		LeaveCriticalSection( &cs ); // cs에서 나온다.

	}

	printf("Finish...%s\n", name);

	return 0;
}









//===========================================
void main()
{
	InitializeCriticalSection( &cs ); // main 제일아래에
									 // DeleteCriticalSection(&cs); 추가.


	HANDLE h1 = CreateThread( 0, 0, foo, "A", 0, 0);
	HANDLE h2 = CreateThread( 0, 0, foo, "\tB", 0, 0);

	HANDLE h[2] = { h1, h2 };
	// 64 개 까지의 KO 를 대기할수 있다.
	WaitForMultipleObjects( 2, h, TRUE, // 2개 모두 signal 될때 까지 대기
								INFINITE );

	// Thread KO 는 죽을때 Signal 된다.
//	WaitForSingleObject( h1, INFINITE );
//	while( bWait );  // 절대 사용하면 안되는 최악의 방법 - 하는일 없이
					 // CPU를 사용하게 된다.
	CloseHandle( h1 );
	CloseHandle( h2 );

	//ExitThread( 0 ); // 현재 스레드만 종료.
	//return ;       // 프로세스의 종료

	DeleteCriticalSection(&cs); 
}


























