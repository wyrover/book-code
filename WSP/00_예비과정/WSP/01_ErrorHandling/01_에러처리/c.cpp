#include <stdio.h>
#include <windows.h>
#include <errno.h>
#include <conio.h>


void main()
{
	HWND hwnd = CreateWindow( 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0);

	if ( hwnd == 0 ) // 실패
	{

		// thread 당 1개씩 존재하는 에러 코드를 얻는다.
		DWORD e = GetLastError(); 

		printf("Fail : %d\n", e );

		// 에러 번호 -> 문자열 변환
		char* s; 

		// 포인터만 전달하고 필요한 만큼 API 함수가 할당하게 하는 기법.
		// 좀 어렵지만 고급 기법 이므로 알아둘 필요 있음
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | 
					   FORMAT_MESSAGE_ALLOCATE_BUFFER,
						0,  // 문자열을 가진 DLL핸들(0 : 시스템으로 부터얻는다)
						e,	// 에러 번호
						0,  // 언어
						(char*)&s, 0, // 버퍼, 버퍼크기
						0); // va_list 형 변수로 얻을때 사용

		printf("%s\n", s);

		// FormateMessage() 는 내부적으로 LocalAlloc()이라는 함수를 사용해서
		// 메모리를 할당한다. 해지는 사용자 책임
		LocalFree( s );
	}
}








