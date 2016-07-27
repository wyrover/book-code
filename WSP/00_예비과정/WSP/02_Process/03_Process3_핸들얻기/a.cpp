#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{	
	case WM_LBUTTONDOWN: SetCapture( hwnd ); return 0;
	case WM_LBUTTONUP:
		if ( GetCapture() == hwnd )
		{
			ReleaseCapture();
			POINT pt;

			// 현재 마우스 포인터 얻기(스크린 좌표)
			GetCursorPos(&pt);
			// 윈도우 핸들 얻기..
			HWND h = WindowFromPoint( pt );

			// 해당하는 윈도우(h)를 만든 프로세스의 ID를 구한다.
			DWORD pid;
			DWORD tid = GetWindowThreadProcessId( h, &pid );

			// 프로세스 ID를 가지고 프로세스 핸들을 얻는다.(Table에 항목을만든다)
			HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );

			// 프로세스 핸들 사용
			TerminateProcess( hProcess, 0 );
			WaitForSingleObject( hProcess, INFINITE );

			CloseHandle( hProcess);
		}
		return 0;
				// 어떤 일을 하는 코드인지를 잘 보고 실행해 보세요.







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

