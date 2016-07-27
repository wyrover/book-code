#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static POINTS ptFrom, ptOld;

	switch( msg )
	{
	case WM_LBUTTONDOWN: 
		ptFrom = ptOld = MAKEPOINTS(lParam);	
		SetCapture( hwnd );
		return 0;

	case WM_LBUTTONUP:
		if ( GetCapture() == hwnd )
		{
			ReleaseCapture();
			
			// 최종선은 R2_COPYPEN으로 그려야 한다.
			HDC hdc = GetDC( hwnd ); // 디폴트 그리기 모드가 R2_COPYPEN 이다.

			MoveToEx( hdc, ptFrom.x, ptFrom.y, 0);
			LineTo(   hdc, ptOld.x,  ptOld.y );

			ReleaseDC( hwnd, hdc );
		}
		return 0;

	case WM_MOUSEMOVE:
		//if ( wParam & MK_LBUTTON )
		if ( GetCapture() == hwnd )   // 캡쳐중일때만..
		{
			POINTS pt = MAKEPOINTS(lParam);

			HDC hdc = GetDC( hwnd );
			SetROP2( hdc, R2_NOT ); // 그리기 모드 변경 - 반전 모드(~D)

			//From ~ old 까지를 먼저 지운다.
			MoveToEx( hdc, ptFrom.x, ptFrom.y, 0);
			LineTo  ( hdc, ptOld.x,  ptOld.y );

			// From - 현재 까지 그린다.
			MoveToEx( hdc, ptFrom.x, ptFrom.y, 0);
			LineTo  ( hdc, pt.x,     pt.y );

			ptOld = pt; // 현재 점을 Old에 보관
			ReleaseDC( hwnd, hdc );
		}
		return 0;








	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS wc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor       = LoadCursor( 0, IDC_ARROW );	
	wc.hIcon         = LoadIcon( 0, IDI_WINLOGO);   
	wc.hInstance     = hInstance;
	wc.lpfnWndProc   = WndProc; 
	wc.lpszClassName = "First"; 
	wc.lpszMenuName  = 0;
	wc.style         = 0;

	ATOM atom = RegisterClass( &wc );

	HWND hwnd = CreateWindowEx( 0, "First", "Hello", WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,0, 0,  0,0);

	ShowWindow( hwnd, nShowCmd );

	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0) )
	{
		DispatchMessage( &msg );
	}

	return 0;
}
