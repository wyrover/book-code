// SDK Header

#define _WIN32_WINNT 0x0500
#define WINVER		 0x0500

//#pragma comment( linker, "/subsystem:windows")

/*
// 윈도우 버전... _WIN32_WINNT 0x400

#define _WIN32_WINNT 0x500		// 현재 OS의 버전을 지정..
#define WINVER       0x500

#include <windows.h>

void main()
{
	AnimateWindow(0, 1, 0);
}
*/

#include <windows.h>

#define HK_SHOW  1
#define HK_HIDE  2


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	// 윈도우가 만들어 질때 발생.. 주로 여기에서 초기화 코드 작성..
	case WM_CREATE:
		RegisterHotKey(hwnd, HK_SHOW, MOD_CONTROL, 'S');
		RegisterHotKey(hwnd, HK_HIDE, MOD_CONTROL, 'H');
		AnimateWindow(hwnd, 1000, AW_BLEND);
		return 0;
	
	case WM_CLOSE:
		AnimateWindow(hwnd, 1000, AW_BLEND | AW_HIDE);
		DestroyWindow(hwnd);
		return 0;

	case WM_HOTKEY:
		if( wParam == HK_SHOW)
			AnimateWindow(hwnd, 1000, AW_SLIDE | AW_VER_NEGATIVE);
		else if(wParam == HK_HIDE)
			AnimateWindow(hwnd, 1000, AW_SLIDE | AW_VER_POSITIVE | AW_HIDE);
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




















