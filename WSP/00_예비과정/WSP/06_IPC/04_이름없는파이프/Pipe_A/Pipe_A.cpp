	// 이름 없는 Pipe 사용하기
	#include <iostream.h>
	#include <windows.h>

	void main()
	{
		HANDLE hRead, hWrite;
		BOOL b = CreatePipe( &hRead, &hWrite, 0, 1024);

		// 상대 프로세스에 읽기 위한 핸들을 복사해준다.
		//----------------------------------------------------
		HWND hwnd = FindWindow( 0, "B");

		DWORD pid;
		DWORD tid = GetWindowThreadProcessId( hwnd, &pid);

		// 최소 요구의 원칙.
		HANDLE hProcess = OpenProcess( PROCESS_DUP_HANDLE, 0, pid );

		HANDLE hTarget;
		DuplicateHandle( GetCurrentProcess(), hRead, // source
						 hProcess, &hTarget,         // dest
						 0, 0,
						 DUPLICATE_SAME_ACCESS );
		//-------------------------------------------------------
		SendMessage( hwnd, WM_USER+99, 0, (LPARAM) hTarget );

		// 이제 A는 읽기위한 핸들이 필요 없으므로 닫는다.
		CloseHandle( hRead );

		char temp[256] = {0};

		while ( 1 )
		{
			cin.getline( temp, 256);

			// pipe에 Write
			DWORD len;

			WriteFile( hWrite, temp, 256, &len, 0);

			FlushFileBuffers( hWrite ); // 화일 버퍼 비우기..
		}

		CloseHandle( hWrite );
	}











