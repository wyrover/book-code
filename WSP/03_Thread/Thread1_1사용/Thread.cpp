// Thread
#include <windows.h>

HWND  MainWnd;

// Thread 함수
DWORD WINAPI ThreadFunc1( LPVOID temp)
{
	HDC	hdc;
	BYTE Blue;
	HBRUSH hBrush, hOldBrush;

	HWND h = (HWND)temp;

	hdc = GetDC(h);
	while(1)
	{
		Blue++;
		Sleep(1);
		hBrush		= CreateSolidBrush(RGB(0, 0, Blue));
		hOldBrush	= (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 100, 100, 300, 400);
		
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
	}
	ReleaseDC(h, hdc);
	return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThread;
	static DWORD  ThreadID;
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			hThread = CreateThread(NULL, 0, ThreadFunc1, hwnd, 0, &ThreadID); // 2
			CloseHandle(hThread);		// use count : 1
		}
		return 0;
	case WM_RBUTTONDOWN:
		{
			HDC hdc = GetDC(hwnd);
			Ellipse(hdc, LOWORD(lParam), HIWORD(lParam), LOWORD(lParam)+20, 
						HIWORD(lParam)+20);
			ReleaseDC(hwnd, hdc);
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


	MainWnd = hwnd;

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