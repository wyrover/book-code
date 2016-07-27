#include <iostream>
#include <queue>  
#include <windows.h>
#include <conio.h>
using namespace std;

// 스레드 풀링..!


//-------------------------------------------------
// 작업의 모양(결국 작업이란 -> 함수 를 의미.)
typedef void(*WORK)();

queue<WORK> WORK_Q;
HANDLE hSemaphore; // 작업 Q의 갯수를  count;

DWORD WINAPI Thread( void* p )
{
	while ( 1 )
	{
		// 작업 Q에 작업이 들어오는 것을 대기한다.
		WaitForSingleObject( hSemaphore, INFINITE );
		WORK f = WORK_Q.front();
		WORK_Q.pop();
		f(); // 작업 수행.
	}
	return 0;
}
void InitPool( int n )
{
	hSemaphore = CreateSemaphore( 0, 0, 1000, "WORK_COUNT");
	for ( int i = 0; i < n; ++i)
	{
		CloseHandle( CreateThread( 0, 0, Thread, 0, 0, 0));
	}
}
void AddWork( WORK f)
{
	// 작업 Q에 작업(함수주소)을 넣는다.
	WORK_Q.push( f );
	// 세마포어를 증가해서 자고 있는 쓰레드를 깨운다.
	LONG old;
	ReleaseSemaphore( hSemaphore, 1, &old);
}
//=============================================================
// 실제 작업 함수.
CRITICAL_SECTION cs;
void foo()
{
	for ( int i = 0; i < 10; ++i )
	{
		EnterCriticalSection(&cs);
		printf("%d : %d\n", GetCurrentThreadId(), i );
		fflush(stdout);
		LeaveCriticalSection(&cs);

		Sleep( 1000 );
	}
	return;
}
void main()
{
	InitializeCriticalSection(&cs);
	InitPool( 1 );
	// 5개의 작업을 Q에 넣는다.
	AddWork( foo ); AddWork( foo ); AddWork( foo ); AddWork( foo );AddWork( foo );

	getch(); // 끝나지 않도록.
	DeleteCriticalSection( &cs);
}




































