#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit, hIP, hBtn;

	switch( msg )
	{	
	// 버튼을 누를때 Pipe 서버에 접속한다.
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case 2:
			{
				// 결국 Pipe는 화일처럼 사용하면 된다.
				char name[256] = "\\\\";
				char ip[256];
				GetWindowText( hIP, ip, 256);
				strcat( name, ip );
				strcat( name, "\\pipe\\source_server");				
				HANDLE hPipe = CreateFile( name, GENERIC_READ, 
									FILE_SHARE_WRITE | FILE_SHARE_READ,
									0, OPEN_EXISTING, 
									FILE_ATTRIBUTE_NORMAL, 0);

				if ( hPipe == INVALID_HANDLE_VALUE )
				{
					MessageBox( 0, "실패", "", MB_OK);
					return 0;
				}

				char buf[4096] = {0};
				DWORD len;
				ReadFile( hPipe, buf, 4096, &len, 0);
				
				CloseHandle( hPipe );
				SetWindowText( hEdit, buf );
			}
		}
		return 0;
					






	case WM_CREATE:
		
		hIP = CreateWindow( "edit", ".", 
							WS_CHILD | WS_VISIBLE | WS_BORDER,
							10,10,100,30, hwnd, (HMENU)1, 0, 0);

		hBtn = CreateWindow( "button", "connect", 
							WS_CHILD | WS_VISIBLE | WS_BORDER,
							120,10,170,30, hwnd, (HMENU)2, 0, 0);

		hEdit = CreateWindow( "edit", "", 
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | 
					WS_HSCROLL | WS_VSCROLL,
					10,50,500,500, hwnd, (HMENU)3, 0, 0);
		return 0;



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

