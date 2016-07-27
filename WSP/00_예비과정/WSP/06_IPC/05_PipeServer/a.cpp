#include <windows.h>
#include <stdio.h>

// MultiThread 기반의 Named Pipe Server....!!!
// 하지만 n/w 으로 사용하려면 역시 socket 프로그래밍이 좋다.!
DWORD WINAPI foo( void* )
{
	HANDLE hPipe = CreateNamedPipe( "\\\\.\\pipe\\source_server", // UNC
									PIPE_ACCESS_OUTBOUND, //출력 전용
									PIPE_TYPE_BYTE,		  // stream 방식
									5,		// 최대 instance 갯수
									4096, 4096,// 입출력 버퍼 크기
									1000,	// WaitNamedPipe()로 대기할 시간
									0);		// KO 보안
	if ( hPipe == INVALID_HANDLE_VALUE )
	{
		printf("Pipe 를 생성할수 없습니다.\n");
		return 0;
	}
	//=====================================================
	while ( 1 )
	{
		BOOL b = ConnectNamedPipe( hPipe, 0); // Client의 접속을 대기한다.
									// 클라이언트가 접속할때 까지 blocking

		if ( b == FALSE && GetLastError() == ERROR_PIPE_CONNECTED ) b = TRUE;

		if ( b ) // 접속 되었다면
		{
			printf("Client 접속\n");
			char* buf = 0;
			// 클립보드에 있는 data를 꺼낸다.
			//=====================================
			if ( OpenClipboard( 0 ))
			{
				HANDLE hData = GetClipboardData( CF_TEXT);

				char* p = (char*)GlobalLock( hData );
				
				buf = (char*)malloc( strlen(p) +1);

				strcpy( buf, p );
				GlobalUnlock( hData );
				CloseClipboard();
			}
			//====================================
			// Pipe에 Write
			DWORD len;
			WriteFile( hPipe, buf, strlen(buf)+1, &len, 0);
			FlushFileBuffers( hPipe );
			DisconnectNamedPipe( hPipe ); // 강제로 접속을 끝는다.

			free(buf );
		}
	}
}





void main()
{
	HANDLE h1 = CreateThread( 0, 0, foo, 0, 0, 0);
	HANDLE h2 = CreateThread( 0, 0, foo, 0, 0, 0);
	HANDLE h3 = CreateThread( 0, 0, foo, 0, 0, 0);
	HANDLE h4 = CreateThread( 0, 0, foo, 0, 0, 0);
	HANDLE h5 = CreateThread( 0, 0, foo, 0, 0, 0);

	HANDLE h[5] = { h1,h2, h3, h4, h5 };
	
	WaitForMultipleObjects( 5, h, TRUE, INFINITE );

	CloseHandle( h1 );
	CloseHandle( h2 );
	CloseHandle( h3 );
	CloseHandle( h4 );
	CloseHandle( h5 );
}




















