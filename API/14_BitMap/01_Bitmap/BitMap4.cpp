// Bitmap _오목...
#define _WIN32_WINNT	0x500
#define WINVER			0x500
#include <windows.h>
#pragma comment( lib, "msimg32.lib")		// TransparentBlt는 msimg32.lib안에 존재..
//-----------------------------------------------------------
static HBITMAP	hPen, hWhite, hBlack;

enum { EMPTY = 1, BLACK = 2, WHITE = 3 };
char board[19][19] = { EMPTY };
//-----------------------------------------------------------
void DrawStone( HDC hdc, int x, int y, char stone)
{
	HDC memDC		= CreateCompatibleDC(hdc);
	HBITMAP old;
	if( stone == BLACK)	
		old = (HBITMAP)SelectObject(memDC, hBlack);
	else
		old = (HBITMAP)SelectObject(memDC, hWhite);
//	BitBlt(hdc, x, y, 29, 29, memDC, 0, 0, SRCCOPY);
	TransparentBlt(hdc, x, y, 29, 29, memDC, 0, 0, 29, 29, RGB(255, 0, 0));
	SelectObject(memDC, old);

	DeleteDC(memDC);
}
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_LBUTTONDOWN:
		{
			// 1. 검은돌 차례인가?
			// 2. 현재 좌표에 돌이 없는가? ( board 배열 확인 )
			// 3. 돌을 그린다...
			POINTS  pt = MAKEPOINTS(lParam);
			pt.x	= (( pt.x) / 30 ) * 30 + 5;
			pt.y    = (( pt.y)/ 30 ) * 30 + 5;

			HDC hdc = GetDC(hwnd);
			// 돌을 화면상에 그리는 함수 구현
			DrawStone(hdc, pt.x, pt.y, BLACK);
			ReleaseDC(hwnd, hdc);

			// 4. board 배열 수정
			// 5. 이겼는가?
			// 6. 흰돌차례가 되도록 변수 수정..
		}
		return 0;

	case WM_CREATE:
		{
		hPen = (HBITMAP)LoadImage(0, "Pan.bmp", IMAGE_BITMAP, 0, 0, 
										LR_LOADFROMFILE);
		hBlack = (HBITMAP)LoadImage(0, "black.bmp", IMAGE_BITMAP, 0, 0, 
										LR_LOADFROMFILE);
		hWhite = (HBITMAP)LoadImage(0, "white.bmp", IMAGE_BITMAP, 0, 0, 
										LR_LOADFROMFILE);
		//--------------------------------------------------------------
		RECT r = { 0, 0, 610, 610 };

		// 원하는 클라이언트의 크기로 설정...
		AdjustWindowRect(&r, 
					GetWindowLong(hwnd, GWL_STYLE), FALSE);

		int cx = r.right	- r.left	;
		int cy = r.bottom	- r.top		;
		MoveWindow(hwnd, 0, 0, cx, cy, TRUE);
		}
		//----------------------------------------------------
		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc		= BeginPaint(hwnd, &ps);
			HDC memDC	= CreateCompatibleDC(hdc);
			HBITMAP old = (HBITMAP)SelectObject(memDC, hPen);

			BitBlt(hdc, 0, 0, 610, 610, memDC, 0, 0, SRCCOPY);

			SelectObject(memDC, old);
			DeleteDC(memDC);

			EndPaint(hwnd , &ps);
		}
		return 0;

	case WM_RBUTTONDOWN:
		return 0;
	case WM_DESTROY:
		//---------------------------------
		DeleteObject(hPen);
		DeleteObject(hBlack);
		//---------------------------------
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
				 0 , 0, 100, 100,	// 초기 위치 
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