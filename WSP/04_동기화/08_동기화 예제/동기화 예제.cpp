#include <iostream>
#include <list>  //STL의 linked list
#include <windows.h>
using namespace std;


// list<int> st;  //전역 list, 결국 st의 모든 멤버는 static에 있다. 동기화 필요

// Linked List 클래스 자체를 스레드에 안전하게 변경한다..

//Thread - Safe 클래스...

template<typename T> class SafeList
{
	list<T>s;
	CRITICAL_SECTION cs;
public :
	SafeList()  {  InitializeCriticalSection(&cs);	}
	~SafeList() {  DeleteCriticalSection(&cs);}

	void push_back(T a)
	{
		EnterCriticalSection(&cs);
		s.push_back(a);
		LeaveCriticalSection(&cs);
	}
};

// cout 의 정체
// ostream cout

SafeList<int> st;  // 스레드에 안전한 Linked list ....

#define COUNT 10

DWORD WINAPI foo(void *p)
{
	for(int i=0; i<10; i++)
	{
		for(int j=0; j<COUNT; j++)
			st.push_back(1);

	//	cout << "foo : " << i << endl;
	}

	return 0;
}

DWORD WINAPI goo(void *p)
{
	for(int i=0; i<10; i++)
	{
		for(int j=0; j<COUNT; j++)
			st.push_back(1);

	//	cout << "goo : " << i << endl;
	}

	return 0;
}

void main()
{
	HANDLE h[2];
	h[0] = CreateThread(0, 0, foo, 0, 0, 0);
	h[1] = CreateThread(0, 0, goo, 0, 0, 0);

	WaitForMultipleObjects(2, h, TRUE, INFINITE);

	CloseHandle(h[0]);
	CloseHandle(h[1]);
}



/*
list<int> st;  //전역 list, 결국 st의 모든 멤버는 static에 있다. 동기화 필요

#define COUNT 10000
CRITICAL_SECTION cs;

DWORD WINAPI foo(void *p)
{
	for(int i=0; i<10; i++)
	{
		for(int j=0; j<COUNT; j++){
			EnterCriticalSection(&cs);
			st.push_back(1);
			LeaveCriticalSection(&cs);
		}	
	}

	return 0;
}

DWORD WINAPI goo(void *p)
{
	for(int i=0; i<10; i++)
	{
		for(int j=0; j<COUNT; j++){
			EnterCriticalSection(&cs);
			st.push_back(1);
			LeaveCriticalSection(&cs);
		}	
	}

	return 0;
}

void main()
{
	InitializeCriticalSection(&cs);
	HANDLE h[2];
	h[0] = CreateThread(0, 0, foo, 0, 0, 0);
	h[1] = CreateThread(0, 0, goo, 0, 0, 0);

	WaitForMultipleObjects(2, h, TRUE, INFINITE);

	CloseHandle(h[0]);
	CloseHandle(h[1]);

	DeleteCriticalSection(&cs);
}

*/

// 죽는 이유는 st.push_back()가 아니라.. cout << " ... "  구문이다...
// cout 자체가 동기화 안되서 나오는 문제 입니다...
// cout 부분을 주석으로 막으세요

// cout 도 ostream 클래스의 전역 객체 이다, 2개 이상의 스레드에서 cout
// 을 사용하려면 반드시 동기화가 필요...