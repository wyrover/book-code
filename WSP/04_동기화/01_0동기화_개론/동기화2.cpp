// 뮤텍스

// 커널 오브젝트
// - 보안속성,  이름(문자열) 
// 이름을 통해 서로 다른 프로세스에서 뮤텍스의 동기화가 이루어진다.
#include <windows.h>
#include <stdio.h>
#include <conio.h>
void main()
{
	HANDLE hMutex = CreateMutex( 0,    // 보안 속성.
								FALSE, // 소유권여부
								"m");  // 뮤텍스 KO 이름

	printf("뮤텍스를 대기합니다.\n");

	WaitForSingleObject( hMutex, INFINITE );

	printf("뮤텍스를 획득했습니다.\n");

	getch(); // 잠시 대기

	ReleaseMutex( hMutex);
	CloseHandle( hMutex );
}


// 이 프로그램을 2번이상 실행해 보세요(죽이지 않은 상태에서..)
/*
#include <windows.h>
#include <iostream>
using namespace std;

void main()
{
	// 뮤텍스 생성
	HANDLE	hMutex = CreateMutex(NULL,  // 보안속성
				FALSE,					// 생성시 뮤텍스 소유 여부
				"mutex");				// 이름
 
	// 소유가 ture일때 -> Signal 을 nonsignal로 바꾼다.

	cout << "뮤택스를 기다리고 있다." << endl;

	DWORD d = WaitForSingleObject(hMutex, INFINITE);		// 대기
	if( d == WAIT_TIMEOUT)
		MessageBox(NULL, "WAIT_TIMEOUT", "", MB_OK);
	else if( d ==WAIT_OBJECT_0)
		MessageBox(NULL, "WAIT_OBJECT_0", "", MB_OK);
	else if( d == WAIT_ABANDONED_0)
		MessageBox(NULL, "WAIT_ABANDONED_0", "", MB_OK);


	cout << "뮤택스 획득" << endl;
	MessageBox(NULL, "뮤택스를 놓는다." , "", MB_OK);

//	ReleaseMutex(hMutex);	// 뮤텍스 소유 해제.
	// nonsignal -> signal
}
*/
/*
	리턴값이 WAIT_ABANDONED_0 라는 것은 포기된 뮤택스라는 값이 리턴된 경우 
	결론적으로 이런 프로그래밍 방식은 좋지 않다..
	원칙..
*/





















