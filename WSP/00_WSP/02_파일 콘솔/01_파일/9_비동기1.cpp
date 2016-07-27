/****************************************************************************

  4) Sample1

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

//---------------------------------------------------
//  비동기 I/O 처리를 이용한 파일 접근
//---------------------------------------------------

#include <stdio.h>
#include <windows.h>
#include <conio.h>

void main()
{
	HANDLE hFile = CreateFile( "COM1", // Serial Port Open
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

	// 비동기로 출력한다.
	OVERLAPPED ov = { 0 };
	ov.hEvent       = CreateEvent( 0, 0, 0, "e"); // 작업당 1개의 event를 준다.
	ov.Offset       = 0;
	ov.OffsetHigh   = 0; // 작업당 별개의 화일 offset 을 제공한다.
	ov.Internal     = 0;
	ov.InternalHigh = 0; // OS가 내부적으로 사용하는 항목-사용자는 변경하지 
						// 않는다.

	const int size = 1000000;
	char buf[size];
	DWORD len = 0;


	BOOL b = WriteFile( hFile, buf, size, &len, &ov);

	if ( b == TRUE )
		printf("동기적으로 화일 출력 완료\n");
	else if ( b == FALSE && GetLastError() == ERROR_IO_PENDING )
	{
		printf("비동기로 작업중..\n");
		
		//...
		// 다른 작업을 마친후 파일작업을 끝날때 까지 기다리려면
		WaitForSingleObject( ov.hEvent, INFINITE );

		//WaitForSingleObject( hFile, INFINITE );
		printf("비동기 작업 완료\n");
	}
	else
		printf("실패\n");


	CloseHandle( hFile );
}







