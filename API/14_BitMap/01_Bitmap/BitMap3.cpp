
// Bitmap.. 
// < 가상 화면 >
//  화면과 동일한 형태의 보이지 않는 화면 
//  - 가상 화면에 그림을 그리고 실제 화면에 고속 복사(BitBlt)를
//    한다.
//  - 사용 이유 ? 깜박임 효과 방지 및....

#include <windows.h>
#include <vector>
using namespace std;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE	hInst;
	static vector<POINTS> buffer;

	//------------------------------------
	static HBITMAP		hBitmap;
	//------------------------------------
	switch( msg )
	{
	case WM_CREATE:

		hInst =  ( ( LPCREATESTRUCT)lParam)->hInstance;
		{
			HDC		hdc = GetDC(hwnd);
			// 화면 DC와 호환되는 비트맵 생성..
			// CreateBitmap : 흑백
			hBitmap     = CreateCompatibleBitmap(hdc, 1024, 768);

			// 비트맵 초기화..
			HDC memDC	= CreateCompatibleDC(hdc);
			HBITMAP old = (HBITMAP)SelectObject(memDC, hBitmap);

			RECT r = { 0, 0, 1024, 768 };
			FillRect(memDC, &r, (HBRUSH)GetStockObject(WHITE_BRUSH));

			SelectObject(memDC, old);
			DeleteObject(memDC);
			ReleaseDC(hwnd, hdc);
		}

		return 0;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP old = (HBITMAP)SelectObject(memDC, hBitmap);
			BitBlt(hdc, 0, 0, 1024, 768, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, old);
			DeleteDC(memDC);
			EndPaint(hwnd, &ps);
		}
		return 0;

	// client 영역이 지워질 때 메세지...
	// 이 메시지가 DefwinProc에 전달되면 화면이 지워진다.
	case WM_ERASEBKGND:			return 0;

	case WM_LBUTTONDOWN:
		{
			POINTS pt = MAKEPOINTS(lParam);
			HDC hdc   = GetDC(hwnd);
			Rectangle(hdc, pt.x, pt.y, pt.x+50, pt.y+50);

			// 비트맵에 동일한 그림을 그릴 수 있다..
			HDC memDC	 = CreateCompatibleDC(hdc);
			HBITMAP old  = (HBITMAP)SelectObject(memDC, hBitmap);

			Rectangle(memDC, pt.x, pt.y, pt.x+50, pt.y+50);

			SelectObject(memDC, old);

			DeleteDC(memDC);
			ReleaseDC(hwnd, hdc);
//			buffer.push_back(pt);
		}
		return 0;
/*
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			for( int i=0; i< buffer.size(); i++)
			{
				Rectangle(hdc, buffer[i].x, buffer[i].y, buffer[i].x+50,
							buffer[i].y+50);
			}
			EndPaint(hwnd, &ps);
		}
		return 0;
*/
	case WM_RBUTTONDOWN:
		return 0;
	case WM_DESTROY:
		//-----------------------------------
		DeleteObject(hBitmap);
		//-----------------------------------
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
