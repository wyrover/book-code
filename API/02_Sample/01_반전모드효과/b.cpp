// 사각형 그리기
#include <windows.h>



LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bx, by, ex, ey, progx, progy;
	static isdraw = false;
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		isdraw = true;
		bx = LOWORD(lParam);
		by = HIWORD(lParam);
		progx = bx;
		progy = by;
		return 0;
		
		case WM_MOUSEMOVE:
		{
			HDC hdc;
			hdc= GetDC(hwnd);
			if(isdraw)
			{
				//-------------------------------------------
				int nx, ny;
				nx = LOWORD(lParam); 
				ny = HIWORD(lParam);
				SetROP2(hdc,R2_NOTXORPEN);
				SelectObject(hdc,GetStockObject(NULL_BRUSH));            
				Rectangle(hdc, bx, by, progx, progy);
				Rectangle(hdc, bx, by, nx, ny);
				progx = nx;
				progy = ny;
				//--------------------------------------------
			}
			ReleaseDC(hwnd, hdc);
		}
		return 0;
		case WM_LBUTTONUP:
			isdraw = false;

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