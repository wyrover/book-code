


// 컴파일러에서 정의 안했다면 정의 
#ifndef _MT
#define _MT	// MSDN이나 다른 소스에서는 반드시 정의하라고 되어 있는 심볼...
			// 결국 _MT심볼은 컴파일러 옵션에서 /MT 를 지정하는 것과 동일한 효과 ..
#endif

#include <iostream>
#include <process.h>  // _beginthreadex() 를 사용하기 위해..
#include <windows.h>
using namespace std;

unsigned int __stdcall foo(void *p)  // 결국 DWORD WINAPI foo() 이다 ~!!
{
	cout << "foo" << endl;
	Sleep(1000);
	cout << "foo finish" << endl;
	return 0;
}

void main()
{	
	unsigned long h = _beginthreadex(0, 0, foo, 0, 0, 0);

	// h가 결국 핸들이다.
	WaitForSingleObject((HANDLE)h, INFINITE);

	CloseHandle((HANDLE)h);
}
//---------------------------------------------------------------------------------------
/*
	VC++에서  C 멀티스레드 라이브러리 버전을 사용하려면 
	1. Project -> Setting 에서 C/C++ tab의 Code Generation 항목에서 
	   multi - thread 버전으로 변경..


	2 . 컴파일러 option 에 /MT를 추가 하던가
	    #define _MT 를 소스에 추가 ..

		VC++을 사용하면 자동으로 /MT 추가 된다.
		하지만 Dos 창에서 Cl.exe 로 직접 컴파일 할때는 직접 컴파일 option을 주던가 
		소스에  #define _MT 를 추가한다. 

	3. Process.h 헤더 필요 ..

*/
//--------------------------------------------------------------------------------------