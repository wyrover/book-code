
/*
#include <iostream>
#include <windows.h>
#include <list>
using namespace std;

// stl 의 list 에서 파생된 클래스를 만든다.
// Thread - Safe 클래스 : 싱글 스레드나 멀티 스레드 환경에서 아무 문제 없이 
//                        사용할수 있다.(즉 자신이 스스로를 동기화 하고 있다)
template<typename T> class Safe_List : public list<T>
{
	CRITICAL_SECTION m_cs;
public:
	Safe_List() { InitializeCriticalSection(&m_cs); }
	~Safe_List() { DeleteCriticalSection(&m_cs); }

	// 부모 함수 override
	void push_back( const T& x )
	{
		EnterCriticalSection( &m_cs );
		list<T>::push_back( x ); // 부모의 원래 기능 사용
		LeaveCriticalSection( &m_cs );
	}  
};

Safe_List<int> st; // 전역 객체( 모든 멤버가 static 에 있으므로 모든 스레드에 의해
			  // 공유된다. 접근시 동기화가 필요한다.


// Serialization : 동시에 수행중이던 스레드가 어떤 지점을 직렬로 차례로 
//					통과 하게 되는 것을 일컫는 용어

//				주의 : MFC에서는 다른 용도로 이 용어를 사용하기도한다.



#define COUNT 500





DWORD WINAPI foo( void* p)
{
	for ( int i = 1; i < 10; ++i )
	{
		for ( int j = 1; j < COUNT; ++j )
			st.push_back( 10 );			// linked list 접근

		printf("foo : %d\n", i );
	}
	return 0;
}
DWORD WINAPI goo( void* p)
{
	for ( int i = 1; i < 10; ++i )
	{
		for ( int j = 1; j < COUNT; ++j )
			st.push_back( 10 );			// linked list 접근

		printf("\tgoo : %d\n", i );
	}
	return 0;
}
void main()
{
	HANDLE h[2];
	h[0] = CreateThread( 0, 0, foo, 0, 0, 0);
	h[1] = CreateThread( 0, 0, goo, 0, 0, 0);

	WaitForMultipleObjects( 2, h, TRUE, INFINITE );
	CloseHandle( h[0]);
	CloseHandle( h[1]);
}
								// foo를 복사해서 goo를 만드세요

*/

#include <iostream>
#include <windows.h>
#include <list>
using namespace std;

list<int> st; // 전역 객체( 모든 멤버가 static 에 있으므로 모든 스레드에 의해
			  // 공유된다. 접근시 동기화가 필요한다.
#define COUNT 500

// list 를 동기화하기 위한 Critical Section
CRITICAL_SECTION cs;
CRITICAL_SECTION cs2; // printf를 동기화 하기 위해 

DWORD WINAPI foo( void* p)
{
	for ( int i = 1; i < 10; ++i )
	{
		for ( int j = 1; j < COUNT; ++j )
		{
			EnterCriticalSection( &cs );
			st.push_back( 10 );			// linked list 접근
			LeaveCriticalSection( &cs );
		}
		EnterCriticalSection( &cs2);
		printf("foo : %d\n", i );
		LeaveCriticalSection( &cs2);
	}
	return 0;
}
DWORD WINAPI goo( void* p)
{
	for ( int i = 1; i < 10; ++i )
	{
		for ( int j = 1; j < COUNT; ++j )
		{
			EnterCriticalSection( &cs );
			st.push_back( 10 );			// linked list 접근
			LeaveCriticalSection( &cs );
		}
		EnterCriticalSection(&cs2);
		printf("\tgoo : %d\n", i );
		LeaveCriticalSection(&cs2);
	}
	return 0;
}
void main()
{
	for ( int i = 0;  i < 10000; ++i )
	{
		InitializeCriticalSection( &cs );
		InitializeCriticalSection( &cs2 );

		HANDLE h[2];
		h[0] = CreateThread( 0, 0, foo, 0, 0, 0);
		h[1] = CreateThread( 0, 0, goo, 0, 0, 0);

		WaitForMultipleObjects( 2, h, TRUE, INFINITE );
		CloseHandle( h[0]);
		CloseHandle( h[1]);

		DeleteCriticalSection( &cs );
		DeleteCriticalSection( &cs2 );

		printf("============================================== %d\n", i );
	}
}
								














