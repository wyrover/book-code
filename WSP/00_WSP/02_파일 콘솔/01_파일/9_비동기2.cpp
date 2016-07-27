/****************************************************************************

  4) Sample2

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


#include <windows.h>
#include <stdio.h>
#include <conio.h>

 
/*
//----------------------- 동기 형태 ----------------------------
void main()
{
        HANDLE hFile = CreateFile("COM3", // serial port open

                                  GENERIC_WRITE,

                                  FILE_SHARE_READ | FILE_SHARE_WRITE,

                                  0,

                                  OPEN_EXISTING,

                                  FILE_ATTRIBUTE_NORMAL, 0);

 
        if(hFile == INVALID_HANDLE_VALUE)
        {

                printf("Error, Can't Open com1\n");

                return;

        }

        // 여기서 DCB라는 구조체를 사용해서 baud rate를 맞추고...
        // Write, Read하면 된다

        //----------------------------------------------------------
        const int size = 1024;   // 1k
        char buf[size] = {0};

        DWORD len;

        BOOL b = WriteFile(hFile, buf, size, &len, 0);

        if(b == TRUE)
        {
                printf("동기적으로 쓰기 성공\n");
        }
        else
        {
                printf("에러\n");
        }
        CloseHandle(hFile);
}
*/


//----------------------- 비동기 형태 ------------------------//

 

// 비동기 작업의 종료를 대기하는 스레드
DWORD WINAPI foo( void* p )
{
        HANDLE hEvent = (HANDLE)p;

        // 지금은 event를 1개만대기 하지만 결국은
        // Multiple을 사용해서 여러개를 대기 하게 한다.

        WaitForSingleObject( hEvent, INFINITE );

        printf("작업이 종료 되었습니다.\n");

        return 0;
}

//--------------------------------------------------------------

void main()
{
        HANDLE hFile = CreateFile( "COM1",// serial port open
                                   GENERIC_WRITE,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   0,
                                   OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED , 
                                   0);

        if ( hFile == INVALID_HANDLE_VALUE )
        {
                printf("Error, Can't Open COM1\n");
                return;
        } 

        // 여기서 DCB 라는 구조체를 사용해서 baud rate 를 
        // 맞추고... Write,Read하면 된다
        //--------------------------------------
        const int size = 1024; // 1K
        char buf[size] = { 0};
        DWORD len;

         // 비동기로 쓰기를 요청하는 방법.- 비동기로 열었으면 반드시 
        // 비동기로 써야 한다.
        OVERLAPPED ov = {0};
        ov.hEvent = CreateEvent( 0, 0, 0, "e1");

         // 비동기 작업 전에 스레드를 생성해서 대기한다.

        HANDLE hThread = CreateThread( 0, 0, foo, (void*)ov.hEvent, 0,0);
        CloseHandle( hThread );

        BOOL b = WriteFile( hFile, buf, size, &len, &ov);

        if ( b == TRUE )
        {
                printf("동기적으로 쓰기 성공\n");
        }
        else if ( b == FALSE && GetLastError() == ERROR_IO_PENDING )
        {
                printf("비동기적으로 작업 중...\n");

                getch(); // 종료되지 않게.
                // 비동기 작업이 종료 되면 Overlapped 구조체에 전달된 
                // Event가 signal 된다.
                // WaitForSingleObject( ov.hEvent, INFINITE);
                // printf("비동기 작업이 종료 되었습니다.\n");
        }
        else
        {
                printf("실패\n");
        }
        CloseHandle( hFile );

}

