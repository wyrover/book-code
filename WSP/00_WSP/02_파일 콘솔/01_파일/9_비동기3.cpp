/****************************************************************************

  4) Sample3

    비동기 I/O(중첩된 입출력) 

              : 주로 네트워크 프로그램이나 병령통신(시리얼)에 사용

 

    대용량 파일 서버에서 주로 사용

    - 대용량 파일 서버 주스레드에서는 accept와 비동기로 Send를 하고 두번째 

       스레드에서는 Send작업들의 Event를 받아 Socket을 닫는 역활을 한다


     1. 비동기 요청하는 법

     2. 비동기 작업의 종료 대기

         -> Event 사용

     3. 요청 스레드와 대기 스레드 분리

****************************************************************************/


#include <stdio.h>
#include <windows.h>
#include <conio.h>

// IOCP 에서 비동기 작업을 대기할 스레드
DWORD WINAPI foo( void* p )
{
	HANDLE hPort = (HANDLE)p;

	// 이제 IOCP 의 완료 Q에 완료된 비동기 작업을 들어 올때 까지 대기한다.
	DWORD key;
	OVERLAPPED* ov;
	DWORD bytes;
	while ( 1 )
	{
		GetQueuedCompletionStatus( hPort, &bytes, &key, &ov, INFINITE );

		printf("비동기 작업이 완료 되었습니다.\n");
		printf("완료키 : %d\n", key);
		printf("Bytes  : %d\n", bytes );
	}
	return 0;
}


void main()
{
	HANDLE hFile = CreateFile( "COM1",// "COM1", // Serial Port Open
								GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								0,
								OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,  // 화일을 비동기로 Open
								0); // template file

	if ( hFile == INVALID_HANDLE_VALUE )
	{
		printf("COM1 을 Open 할수 없습니다.\n"); return;
	}

	// IOCP( 입출력 완료 포트)를 생성한다.
	HANDLE hPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE, // 화일핸들
											0, // 기존에 존재하는 IOCP 핸들
											0, // 완료 키.
											2); // 동시에 대기 가능 스레드수

	// 완성된 IOCP 에 화일의 핸들을 추가한다
	CreateIoCompletionPort( hFile, hPort, 1, // 완료 키
										2);

	// 새로운 스레드를 만들어서 비동기 IO의 완료를 처리한다.
	HANDLE hThread = CreateThread( 0, 0, foo, (void*)hPort, 0, 0);
	CloseHandle( hThread );

	//-----------------------------------------------------------------------
	const int size = 1024;
	char buf[size];
	DWORD len = 0;


	// 이제 비동기 작업을 수행한다.
	OVERLAPPED ov = { 0};
	BOOL b = WriteFile( hFile, buf, size, &len, &ov);

	// 다른 작업 수행..... - 비동기 작업 완료시 처리는 다른 스레드가 하게된다.
	getch();


	CloseHandle( hFile );
}





