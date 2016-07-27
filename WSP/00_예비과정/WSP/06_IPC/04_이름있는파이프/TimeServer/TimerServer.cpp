	#include <iostream.h>
	#include <windows.h>

	void main()
	{
		// named pipe 만들기
		HANDLE hPipe = CreateNamedPipe( "\\\\.\\pipe\\TimeServer", // pipe이름
										PIPE_ACCESS_OUTBOUND, // 출력 전용.
										PIPE_TYPE_BYTE, 
										1, // 동일 이름의 파이프를 만들수 있는 최대 갯수.
										1024,1024, // 입출력 버퍼 크기
										1000, // WaitNamedPipe함수로 대기할수 있는 시간
										0); // KO 보안

		if ( hPipe == INVALID_HANDLE_VALUE )
		{
			cout << "Pipe를 만들수가 없습니다." << endl;
			return ;
		}

		while ( 1 )
		{
			// 클라이 언트의 접속을 대기한다.
			BOOL b = ConnectNamedPipe( hPipe, 0);

			if ( b == FALSE && GetLastError() == ERROR_PIPE_CONNECTED )
				b = TRUE;
			
			if ( b == TRUE )
			{
				// pipe를 통해서 현재 시간을 알려준다.
				SYSTEMTIME st;
				GetSystemTime( &st );

				DWORD len;
				WriteFile( hPipe, &st, sizeof(st), &len, 0);

				FlushFileBuffers( hPipe );

				// 접속을 끊는다.
				DisconnectNamedPipe( hPipe ); 

				cout << "Work... Done" << endl;
			}
		}	
	}







