// 윈도우 스타일 변경
#include <windows.h>

// 실행시 윈도우 객채의 내용을 변경해서 실행 시간에 윈도우 style 변경하기
void ModifyStyle( HWND hwnd, UINT Remove, UINT Add, BOOL bReDraw = TRUE)
{
	// 1. 현재 스타일  얻어 오기	: GetWindowLong
	UINT style = GetWindowLong(hwnd, GWL_STYLE);

	// 2. style 변경				: bit 연산..
	style = style & ~Remove;
	style = style | Add;
	SetWindowLong( hwnd, GWL_STYLE, style);

	// 3. style 변경 후 다시 그려야 함.. : SetWindowPos
	if( bReDraw)
	{
		SetWindowPos( hwnd, 0, 0, 0, 0, 0, // z order, x, y, w, h
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_DRAWFRAME);
	}

}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
//	case WM_LBUTTONDOWN:
//		ModifyStyle( hwnd, WS_CAPTION, 0);	return 0;
//	case WM_RBUTTONDOWN:
//		ModifyStyle( hwnd, 0, WS_CAPTION);	return 0;
	
	case WM_LBUTTONDOWN:
		// 실행시 Icon 변경..
		{
			HICON hIcon = LoadIcon(0, IDI_QUESTION);
			SetClassLong(hwnd, GCL_HICON, (LONG)hIcon);
		}
		return 0;
	case WM_RBUTTONDOWN:
		// 배경색 변경..
		{
//			HBRUSH	h = (HBRUSH)GetStockObject(BLACK_BRUSH);
			HBRUSH h = CreateSolidBrush(RGB(255, 0, 0));
			SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)h);
			InvalidateRect(hwnd, 0, TRUE);

			DeleteObject(h);

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
	wc.hbrBackground		= //(HBRUSH)GetStockObject(WHITE_BRUSH);
							  (HBRUSH)CreateSolidBrush(RGB(255, 255, 0));
	\
	wc.hCursor				= LoadCursor(0, IDC_ARROW);
	wc.hIcon				= LoadIcon(0, IDI_WINLOGO);
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