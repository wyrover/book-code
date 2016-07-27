// STEP  3	
// WM_LBUTTONDOWN.... 마우스 캡쳐(아이콘 영역 안일 경우만 capture) 
//                    마우스 캡쳐 해지...

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HICON		hFill, hEmpty;
	static HCURSOR		hCursor;
	static RECT			rc = { 50, 50 , 82, 82 };	// 32 * 32
	static HINSTANCE	hInst;

	switch( msg )
	{
	// 초기화
	case WM_CREATE:
		// Instance 얻기..
		hInst		= ( ( LPCREATESTRUCT)lParam)->hInstance;

		hFill		= LoadIcon  ( hInst, MAKEINTRESOURCE(IDI_ICONFILL));
		hEmpty		= LoadIcon  ( hInst, MAKEINTRESOURCE(IDI_ICON_EMPTY));
		hCursor		= LoadCursor( hInst, MAKEINTRESOURCE(IDC_CURSOR_TARGET));
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc			= BeginPaint(hwnd, &ps);
			DrawIcon( hdc, rc.left, rc.top, hFill);	
			EndPaint(hwnd, &ps);
		}
		return 0;

	case WM_LBUTTONDOWN:
		{
			POINT  pt		=  { LOWORD(lParam), HIWORD(lParam) };

			// 아이콘 영역 안인가??
			if( PtInRect( &rc, pt ) )
			{
				// 1. 마우스 캡쳐 
				SetCapture(hwnd);

				// 2. 아이콘 변경
				HDC  hdc = GetDC(hwnd);
				DrawIcon(hdc, rc.left, rc.top, hEmpty);

				ReleaseDC(hwnd, hdc);

				// 3. 커서 변경
				SetCursor( hCursor);
			}
		}
		return 0;
	// 마우스 캡쳐가 해지 될 때 호출되는 핸들러..
	// 이 메시지에서 화면을 무효화 하면 Icon이 다시 그려진다...
	case WM_CAPTURECHANGED:
		InvalidateRect(hwnd, &rc, TRUE);
		return 0;

	case WM_LBUTTONUP:
		if( GetCapture() == hwnd )
			ReleaseCapture();
		return 0;

	case WM_RBUTTONDOWN:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	// 1. 윈도우 클래스 만들기 
	WNDCLASS wc;
	wc.cbWndExtra			= 0;
	wc.cbClsExtra			= 0;
	//------------------------------------------------------------------
	wc.hbrBackground		= (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//-----------------------------------------------------------------
	wc.hCursor				= LoadCursor(0, IDC_ARROW);
	wc.hIcon				= LoadIcon(0, IDI_APPLICATION);
	wc.hInstance			= hInstance;
	wc.lpfnWndProc			= WndProc;			// DefWindowProc;
	wc.lpszClassName		= "Finder";
	wc.lpszMenuName			= 0;
	wc.style				= 0;

	// 2. 등록(레지스트리에)
	RegisterClass(&wc);

	// 3. 윈도우 창 만들기 
	HWND hwnd = CreateWindowEx( 0,					// WS_EX_TOPMOST
				 "Finder",				// 클래스 명
				 "Finder",				// 캡션바 내용
				 //--------------------------------------------------------------
				 WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~ WS_MAXIMIZEBOX , 
				 100 , 100, 300, 300,	// 초기 위치 
				 //----------------------------------------------------------------
				 0, 0,			// 부모 윈도우 핸들, 메뉴 핸들
				 hInstance,		// WinMain의 1번째 파라미터 (exe 주소)
				 0);			// 생성 인자

	// 4. 윈도우 보여주기
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	

	// 5. Message 
	MSG msg;
	while( GetMessage( &msg, 0, 0, 0 ) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}