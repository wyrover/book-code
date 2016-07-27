#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

WNDPROC old; // 원래의 EditBox의 메세지 처리함수의 주소를 담을 변수
LRESULT CALLBACK foo( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_CHAR:
		if ( ( wParam >= '0' && wParam <= '9') || wParam == 8 )
			// 원래의 메세지처리 함수로 전달한다.
			wParam = 'a';
			return CallWindowProc( old, hwnd, msg, wParam, lParam);

		return 0; // 숫자 이외의 경우는 무시한다.
	}
	// 나머지 모든 메세지는 원래의 함수로 전달한다.
	return CallWindowProc( old, hwnd, msg, wParam, lParam);
}
	



LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;

	switch( msg )
	{	
	case WM_CREATE:
		hEdit = CreateWindow( "edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
							10,10,200,200, hwnd, (HMENU)1, 0, 0);

		old = (WNDPROC)SetWindowLong( hEdit, GWL_WNDPROC, (LONG)foo );
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
		// 여기서 메세지를 처리하면 부모뿐 아니라 모든 자식윈도우로 전달되는
		// 메세지도 먼저 처리할수 있다.
//		if ( msg.message == WM_SYSKEYDOWN  && msg.wParam == 'X' )
//		{
//			MessageBox( hwnd, "종료~~", "", MB_OK);
//			SendMessage( hwnd, WM_CLOSE, 0, 0);
//			continue;   
//		}

//	  PreTranslateMessage( &msg ); // MFC 에 있는 함수.*****
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}

// 정리 1. DispatchMessage() 전에 메세지를 처리하는 기법.
//      2. Subclassing.













