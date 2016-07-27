//----------------------------------------------------------------------
// 배경화면을 나타내는 DC 에서 윈도우의 ClientDC로 픽셀을 전송하는 예제
//  - BitBlt
//    BitBlt 함수에서 가장 중요한 점은 2개의 DC는 반드시 
//    호환(Compatible, 색상면(plane)과 픽셀당 비트(bpp)가 동일 )되어야 한다.
//----------------------------------------------------------------------

#include <windows.h>


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{

	case WM_PAINT:
	{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Ellipse(hdc, 150, 150, 300, 300);
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			EndPaint(hwnd, &ps);
	}

	return 0;

	case WM_LBUTTONDOWN:
		{
			// 클라이언트 영역에 대한 DC 
			HDC		hdc		= GetDC(hwnd);

//			HDC		hdc     = GetDC(0);	// 전체화면..

			// 바탕화면 : 모니터..DC
			HDC		scrDC	= CreateDC("DISPLAY", 0, 0, 0);	// 바탕화면..

// 하나의 DC에 있는 비트맵을 다른 DC로 복사하는 비트맵 전송함수..
// 이 때 두 DC는 호환되어야 하나 만약 색상 포맷이 호환되지 않을 
// 경우 BitBlt는 복사원의 색상 포맷을 복사처의 포멧에 맞게 변경..
	BitBlt(hdc, 0, 0, 500, 500,   // Dest dc,  x, y, w, h
			scrDC, 0, 0,		  // sorce dc, x, y
//			0x00DD0228);  //  S | ~ D (복사원본의 색 | ~ 복사되는 곳의 색)
		    SRCPAINT);	  //  S | D										

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
//	while( GetMessage( &msg, 0, 0, 0 ) )
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
	
	while(1)
	{
		if( PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if( msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*
			HDC scrDC		= CreateDC("DISPLAY", 0, 0, 0);
			SetTextColor(scrDC, RGB(rand()%255, rand()%255, rand()%255));
			SetBkMode(scrDC, TRANSPARENT);

			HFONT font = CreateFont(rand()%50, 0, rand()%100, 0, 0, 0, 0, 0,
					HANGEUL_CHARSET, 3, 2, 1,
					VARIABLE_PITCH | FF_ROMAN, "바탕체");
			HFONT oldfont = (HFONT)SelectObject(scrDC, font);
			TextOut(scrDC, rand()%1024, rand()%768, "낙서금지", 8);
			Sleep(500);

			SelectObject(scrDC, oldfont);
			DeleteObject(font);
			DeleteObject(scrDC);
			*/
		}
	
	}
	return 0;
}


































