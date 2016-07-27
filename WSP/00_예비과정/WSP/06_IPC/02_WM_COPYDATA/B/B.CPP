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
							10,10,400,400, hwnd, (HMENU)1, 0, 0);
		return 0;

	case WM_COPYDATA:
		{
			COPYDATASTRUCT* p = (COPYDATASTRUCT*)lParam;

			if ( p->dwData == 1 ) // 식별자 조사.
			{
				// Edit Box에 추가 한다.
				SendMessage( hEdit, EM_REPLACESEL, 0, (LPARAM)(p->lpData));
				SendMessage( hEdit, EM_REPLACESEL, 0, (LPARAM)"\r\n");
			}
		}
		return 0;
					// B를 먼저 실행해 주세요.. 그리고 A 를 실행해 보세요.
					// A에서 문자열을 입력하고 Enter !!












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

	hwnd = CreateWindowEx( 0, "first", "B", WS_OVERLAPPEDWINDOW,
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

