#include <windows.h>
#include <iostream>
#include <queue> // STL의 Q
#include <time.h>
using namespace std;

queue<int> Q;		// 2개의 스레드가 동시에 사용하는 공유 자원

HANDLE hMutex;	// Q에 접근을 동기화 하기 위해 Mutex사용(CRITICAL_SECTION 이 
				// 더 좋긴 하지만.mutex예제를 위해)

HANDLE hSemaphore;	// Q의 갯수를 Count 하기 위해.



// 생산자
DWORD WINAPI Produce( void* ) 
{
	static int value = 0;
	while ( 1 )
	{
		// Q에 생산을 한다.
		++value;
		
		// Q의 접근에 대한 독점권을 얻는다.
		WaitForSingleObject( hMutex, INFINITE); 
		//---------------------------------------------
		Q.push( value );
		printf("Produce : %d\n", value );
		
		LONG old;
		ReleaseSemaphore( hSemaphore, 1, &old); // 세마포어 갯수를 증가한다.
		//------------------------------------------------
		ReleaseMutex( hMutex );



		Sleep( (rand() % 20 )* 100); // 0.1s ~ 2s간 대기.
	}
	return 0;
}

// 소비자
DWORD WINAPI Consume( void* p)
{
	while ( 1 )
	{
		WaitForSingleObject( hSemaphore, INFINITE ); // Q가 비어 있다면 대기.
		WaitForSingleObject( hMutex, INFINITE );
		//----------------------------------------------
		int n = Q.front();  // Q의 제일 앞요소 얻기(제거하지 않는다.)
		Q.pop();			// 제거.
		printf("		Consume : %d\n", n );
		//----------------------------------------------
		ReleaseMutex( hMutex );







		Sleep( (rand()%20)*100); // 0.1s ~ 2s간 대기
	}
	return 0;
}



void main()
{
	hMutex = CreateMutex( 0, FALSE, "Q_ACCESS_GUARD");
	hSemaphore = CreateSemaphore( 0, 0, 1000, "Q_RESOURCE_COUNT"); //최대
													// 1000개의 , 초기 0

	srand( time(0));
	HANDLE h[2];
	h[0] = CreateThread( 0, 0, Produce, 0, 0,0);
	h[1] = CreateThread( 0, 0, Consume, 0, 0,0);
  
	WaitForMultipleObjects( 2, h, TRUE, INFINITE );

	CloseHandle( h[0] );
	CloseHandle( h[1] );
	CloseHandle( hMutex );
	CloseHandle( hSemaphore );
}















