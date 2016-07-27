// Critical Section
/*
=> 커널 객체가 아니다... 따라서 아래와 같은 특징들을 나타낸다.
장 점 : 동기화 객체중 가장 빠르다.
단 점 : 서로 다른 프로세스간 동기화에서는 사용할 수 없다.

=> 동기화 수단 : 임계영역..
*/


#include <windows.h>
#include <iostream>
using namespace std;

void WorkFunc()	{	for( int i=0; i <10000000; i++);	}// 시간 지연 함수	
// 공유 자원
int g_x	= 0;
int g_x1 = 0;

//------------------------------------------------
CRITICAL_SECTION	g_cs;	// 전역
CRITICAL_SECTION    g_cs1;
//------------------------------------------------

DWORD WINAPI Func( PVOID p)
{
	InterlockedIncrement(&g_x1);	// ++
	InterlockedDecrement(&g_x1);	// --
	//---------------------------------------------------------------
	EnterCriticalSection(&g_cs);

	for( int i=0; i< 20; i++)
	{
		g_x = 200;
		WorkFunc();
		g_x++;
		cout << "             Func : " << g_x << endl;
	}
	LeaveCriticalSection(&g_cs);
	//------------------------------------------------------------------

	return 0;
}
void main()
{
	InterlockedIncrement(&g_x1);


	InitializeCriticalSection(&g_cs);	// 임계영역 변수 초기화.
	InitializeCriticalSection(&g_cs1);	// 임계영역 변수 초기화.

	DWORD tid;
	HANDLE hThread = CreateThread(NULL, 0, Func, 0, NORMAL_PRIORITY_CLASS, &tid);
	//---------------------------------------------------------------
//	Sleep(1);
	EnterCriticalSection(&g_cs);
	for( int i=0; i< 20; i++)
	{
		g_x = 200;
		WorkFunc();
		g_x--;
		cout << "             Main : " << g_x << endl;
	}
	LeaveCriticalSection(&g_cs);
	//------------------------------------------------------------------
	
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	DeleteCriticalSection(&g_cs);	// 파괴
}



/*
#include <vector>
#include <windows.h>
#include <iostream>
using namespace std;

template<class T> 
class SafeVector : public vector<T> 
{
public:
	CRITICAL_SECTION	cs;
	SafeVector()	{ InitializeCriticalSection(&cs);	}
	~SafeVector()	{ DeleteCriticalSection(&cs);		}

	void push_back( const T n )
	{
		EnterCriticalSection(&cs);
		vector<T>::push_back(n);		// 원래의 기능 사용
		LeaveCriticalSection(&cs);
	}
	void resize(int x)
	{
		EnterCriticalSection(&cs);
		vector<T>::resize(0);		// 원래의 기능 사용
		LeaveCriticalSection(&cs);
	}
};


SafeVector<int> buf;
//vector<int> buf;	// 일반적인 vector 클래스 사용

DWORD WINAPI foo( LPVOID p)
{
	for( int i=0; i<10; i++)
	{
		for( int j=0; j<20000; j++)
		{
			buf.push_back(10);
		}
		buf.resize(0);
	}
	return 0;
}
void main()
{
	HANDLE hThread = CreateThread( 0, 0, foo, 0, 0, 0);
	for( int i=0; i<10; i++)
	{
		for( int j=0; j<20000; j++)
		{
			buf.push_back(10);
		}
		buf.resize(0);
		cout << "main: " << i << endl;
	}
}

*/














