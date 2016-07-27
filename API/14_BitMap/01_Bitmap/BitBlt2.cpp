//----------------------------------------------------------------------
// 배경화면을 나타내는 DC 에서 윈도우의 ClientDC로 픽셀을 전송하는 예제
//  - StretchBlt
//    BitBlt()함수는 크기를 늘이거나 줄일 수는 없다. 
//    복사하면서 이미지의 크기를 늘리거나 줄이려는 경우 사용되는 함수 
//----------------------------------------------------------------------

#include <windows.h>


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		SetCapture(hwnd);
		return 0;
	case WM_LBUTTONUP:
		if( GetCapture() == hwnd)
		{
			ReleaseCapture();
		}
		return 0;
	case WM_MOUSEMOVE:
		if( GetCapture() == hwnd)
		{
			POINT	pt;
			GetCursorPos(&pt);
			HDC		hdc		= GetDC(hwnd);
			HDC		scrDC	= CreateDC("DISPLAY", 0, 0, 0);
	
						// hdc의 윈도우창의 0, 0, 200, 200의 사각형 영역에 
			StretchBlt(hdc, 0, 0, 200, 200,		
						// scrDC(화면전체)에 있는 비트맵 중 pt.x, pt.y 100, 100,
						// 의 사각형 영역에 있는 것을 고속복사하라..
						scrDC, pt.x, pt.y, 100,100, 
						SRCCOPY);

			ReleaseDC(hwnd, hdc);
			DeleteDC(scrDC);
		}
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
	wc.hbrBackground		= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor				= LoadCursor(0, IDC_ARROW);
	wc.hIcon				= LoadIcon(0, IDI_APPLICATION);
	wc.hInstance			= hInstance;
	wc.lpfnWndProc			= WndProc;			// DefWindowProc;
	wc.lpszClassName		= "First";
	wc.lpszMenuName			= 0;
	wc.style				= 0;

	// 2. 등록(레지스트리에)
	RegisterClass(&wc);

	// 3. 윈도우 창 만들기 
	HWND hwnd = CreateWindowEx( 0,					// WS_EX_TOPMOST
				 "first",				// 클래스 명
				 "Hello",				// 캡션바 내용
				 WS_OVERLAPPEDWINDOW, 
				 CW_USEDEFAULT , 0, CW_USEDEFAULT, 0,	// 초기 위치 
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