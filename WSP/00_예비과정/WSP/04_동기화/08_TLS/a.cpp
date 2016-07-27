#include <windows.h>
#include <stdio.h>
// goo 는 static 지역 변수을 사용해서 원하는 기능을 수행하였다.
// 싱글스레드 환경에서는 아무 문제 없다.   멀티 스레드 라면. ?
void goo( char*  name)
{
	// TLS 공간에 변수를 생성한다.
	__declspec(thread) static int c = 0;

//	static int c = 0;
	++c;
	printf("%s : %d\n", name, c ); // 함수가 몇번 호출되었는지 알고 싶다.
}
DWORD WINAPI foo( void* p )
{
	char* name = (char*)p;
	goo( name );
	goo( name );
	goo( name );
	return 0;
}
void main()
{
	HANDLE h1 = CreateThread( 0, 0, foo, "A", 0, 0);
	HANDLE h2 = CreateThread( 0, 0, foo, "\tB", 0, 0);

	HANDLE h[2] = { h1, h2};
	WaitForMultipleObjects( 2, h, TRUE, INFINITE );

	CloseHandle( h1 );
	CloseHandle( h2 );
}

// 어떤 함수가 내부적으로 지역 변수만 사용한다 -> 
//멀티 스레드에 안전한가 ? OK

// 어떤 함수가 내부적으로 static 변수를 사용한다. -> 
// 싱글스레드는 문제 없다.
//  -> 멀티 스레드는 안전한가 ?




