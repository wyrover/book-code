// Bitmap  
/*
윈도우즈가 지원하는 비트맵 포맷
1. DDB  : Device Dependent Bitmap
   - 내부적 구성정보 : 이미지 크기와 색상, 이미지 데이터, (이전 3.0 버전에서 사용)

2. DIB  : Device Independent Bitmap
   - 장치에 독립적( 확장자 BMP) 가 여기에 해당 
   -> Win32환경에서 일반적으로 사용
   - 색상 테이블, 해상도 정보... 등의 추가 정보가 있슴

  => 호환 측면에서 DDB 사용, DC에 선택될 수 있는 비트맵은 DDB
     DIB는 직접 DC에 선택될 수 없다..
*/

//------------------------------------------------------
// Bitmap Object는 일반 DC에 선택될 수 없다.
// MemoryDC만이 Bitmap Object를 선택할 수 있다.

//  DDB ==(SelectObject)==> MemoryDC ==(BitBlt)==>Client DC ==> 화면 출력

//-------------------------------------------------------

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE	hInst;
	static HBITMAP		hBitmap;
	static BITMAP		bm;			// DDB 헤더..

	switch( msg )
	{
	case WM_CREATE:
		hInst = ( ( LPCREATESTRUCT)lParam)->hInstance;

		//1. Bitmap 로드
		// 리소스에 작성되는 비트맵은 DIB이미지임
		// LoadBitmap 함수에 의해 호환되는 DDB로 변환됨
		// 따라서 메모리 DC에 선택하여 곧바로 출력 가능..
		hBitmap	= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

		//2. DDB Bitmap 헤더 얻기
		GetObject(hBitmap, sizeof(bm), &bm);
		return 0;

	case WM_LBUTTONDOWN:
		{
			HDC hdc		= GetDC(hwnd);

			// 화면 DC와 호환(동일색상)되는 메모리 DC를 얻음..
			HDC memDC	= CreateCompatibleDC(hdc);		

			// 메모리 DC에 비트맵 선택
			SelectObject(memDC, hBitmap);

			// 비트맵으로출력
			TextOut(memDC, 5, 5, "hello", 5);

			// 메모리 DC --> 화면 DC//
			POINTS pt = MAKEPOINTS(lParam);
			BitBlt(hdc, pt.x, pt.y, bm.bmWidth, bm.bmHeight, 
				memDC, 0, 0, SRCCOPY);

			// 메모리 제거
			DeleteDC(memDC);
			ReleaseDC(hwnd, hdc);
		}

		return 0;
	case WM_RBUTTONDOWN:
		//--------------------------------
		DeleteObject(hBitmap);
		//-----------------------------------
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
	
	HBITMAP hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	// BItmap으로 브러쉬 생성  // CreateSolidBrush, CreateHatchBrush..
	HBRUSH brush   = CreatePatternBrush(hBitmap);

//	wc.hbrBackground		= (HBRUSH)brush;
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