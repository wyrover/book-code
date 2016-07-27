// Thread Stack

#include <windows.h>
#include <iostream>
using namespace std;

/*
스레드 생성시 IMB stack이 만들어짐
PAGE_READWRITE 영역 : 4K 사용할 수 있는 영역
PAGE_GUARD			: 사용 불가능, 이동 영역
PAGE_RESERVER		: 예약 영역

=> 사용량이 늘어날때마다 4K 단위로 
   PAGE_READWRITE와 PAGE_GUARD 영역이 이동함

만약 끝까지 차게 되면 Exception 에러 발생(overflow)
*/
int Sum( int n )
{
	// Sum을 재귀적 호출
	return n == 0 ? 0 : ( n + Sum(n - 1) );
}

DWORD Handler( DWORD e )
{
	if ( e == EXCEPTION_STACK_OVERFLOW)
		cout << "stack overflow" << endl;
	return EXCEPTION_EXECUTE_HANDLER;
}
DWORD WINAPI ThreadFunc( LPVOID p)
{
	// 합계를 구해야 할 정수의 개수를 저장
	int n= (int)p;

	int s = -1;
	__try{
		// s에서 n까지의 합계를 저장
		// 합계를 계산할 수 없을 경우 -1값 리턴
		s = Sum(n);
	}
	__except( Handler(GetExceptionCode()) )
	{
		// 스택 오버플로우가 발생했을 때 이 부분 실행
		cout << " 예외 발생 " << endl;
	}
	return s;
}

void main()
{
	int n;
	while(1)
	{
		cin >> n;
		HANDLE hThread = CreateThread(NULL, 0, // 1MB Stack Memory
				ThreadFunc, (void*)n, 0, 0);

		cout << "잠시 대기"  << endl;
		WaitForSingleObject( hThread, INFINITE);

		DWORD code;
		GetExitCodeThread(hThread, &code);
		if( code == -1 )
		{
			cout << "값이 너무 크다. " << endl;
		}
		else
		{
			cout << "결과 : " << code << endl;
		}
		CloseHandle(hThread);
	}
}
























