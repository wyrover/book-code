#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;

	switch( msg )
	{	
	case WM_CREATE:
		
		hEdit = CreateWindow( "edit", "", 
							WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
							10, 10, 500,500, hwnd, (HMENU)1, 0, 0);
		return 0;
	/*
	case WM_LBUTTONDOWN:
		{
			HANDLE hRead, hWrite;
			CreatePipe( & hRead, & hWrite, 0, 1024);
			SetHandleInformation( hWrite, HANDLE_FLAG_INHERIT, 
										  HANDLE_FLAG_INHERIT );

			PROCESS_INFORMATION pi = {0};
			STARTUPINFO si = { 0};
				
			si.cb = sizeof(si );
			si.hStdOutput = hWrite;//쓰기위한 파이프 핸들을 자식의 표준출력으로
			si.dwFlags = STARTF_USESTDHANDLES;

			BOOL b = CreateProcess( 0, "ping.exe www.imbc.com", 0,0, TRUE,
									CREATE_NO_WINDOW, 0, 0, &si, &pi );
			if ( b )
			{
				CloseHandle( hWrite ); // 쓰기위한 핸들은 필요 없다.

				DWORD len;
				while ( 1 )
				{
					char s[4096] = {0};
					BOOL bRet = ReadFile( hRead, s, 4096, &len, 0);

					if ( bRet == FALSE ) break; // 파이프가 닫힌 경우.

					// SetWindowText()를 사용하면 기존의 edit data 가 지워지고
					// 새롭게 추가 된다. - 한줄씩 덧붙이려면!
					SendMessage( hEdit, EM_REPLACESEL, 0, (LPARAM)s);
				}
				CloseHandle( hRead );
				CloseHandle( pi.hProcess);
				CloseHandle( pi.hThread);
			}
		}
		return 0;
	*/





	case WM_LBUTTONDOWN: 
		{
			PROCESS_INFORMATION pi = { 0 } ;
			
			// ping 출력을 redirect 할 파일 생성
			HANDLE hFile = CreateFile( "a.txt", GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										0, 
										CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			// 모든 KO 는 기본적으로 상속 불가능 하다.
			// 특정 KO를 상속가능하게 변경한다.
			SetHandleInformation( hFile, HANDLE_FLAG_INHERIT, // 상속가능여부를
										 HANDLE_FLAG_INHERIT ); // 상속가능하게.
			//===============================================================
			STARTUPINFO si = {0};
			si.cb = sizeof( si );

			si.hStdOutput = hFile;  // ping 의 stdout과 연결할 파일.
			si.dwFlags    = STARTF_USESTDHANDLES;


			BOOL b = CreateProcess( 0, "ping.exe www.imbc.com", 0,0, TRUE,
							CREATE_NO_WINDOW, // flag. console을 만들지말라
									0, 0, &si, &pi);
			if ( b )
			{
				// ping이 종료 될때까지 대기한다.
				WaitForSingleObject( pi.hProcess, INFINITE );

				// a.txt 에서 모든 data를 읽어서 edit에 출력한다.
				char s[4096] = {0};

				SetFilePointer( hFile, 0, 0, FILE_BEGIN); // 화일 포인터를 제일 
														  // 앞으로
				DWORD len;
				ReadFile( hFile, s, 4096, &len, 0);

				SetWindowText( hEdit, s );

				CloseHandle( hFile );
				//===========================================
					
				


				CloseHandle( pi.hProcess );
				CloseHandle( pi.hThread );
			}
		}
		return 0;

	


//레지스트리 찾는법...
//HKEY_CURRENT_USER			\software \ 회사이름 \ 프로그램이름	\ 각종 세팅값.
//HKEY_LOCAL_MACHINE
//regedit.exe


//www.codeproject.com	// 윈도우 개발 관련 소스코드 사이트
//www.catch22.net		//
//www.codeguru.com		// 3개는 전부 외국 사이트..
//www.devia.com			// -한글











	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
{
	ATOM atom;
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "First";
	wc.lpszMenuName = 0;
	wc.style		= 0;

	atom = RegisterClass( &wc);
	
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( 0, "first", "Hello", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT,0, 0, 0, 
								hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}

