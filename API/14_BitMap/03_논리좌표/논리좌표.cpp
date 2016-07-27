// 논리좌표

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc		 = BeginPaint(hwnd, &ps);

			Rectangle(hdc, 100, 100, 200, 200);

			// 논리 1= 0.1mm(물리),  y축 증가 위 )
			SetMapMode(hdc, MM_LOMETRIC);	

			Rectangle(hdc, 0, 0, 100, -100);

			EndPaint(hwnd, &ps);
		}
		return 0;


// default 매핑 모드는 논리좌표와 물리좌표가 일치되어 변환이 일어나지 않는다...

	case WM_LBUTTONDOWN:
		{
			HDC hdc = GetDC(hwnd);
			// 물리좌표                              (클라이언트) 좌표..
//			POINTS pt = MAKEPOINTS(lParam);

			POINT  pt = { LOWORD(lParam), HIWORD(lParam) };

//			SetMapMode(hdc, MM_LOMETRIC);		//0.1mm 오른쪽, 위쪽 증가

			// 물리(클라이언트) ==> 논리 좌표로...
			DPtoLP(hdc, &pt, 1);

			// 논리좌표가 전달되어야 정상적 출력 가능..
			Rectangle(hdc, pt.x, pt.y, pt.x+50, pt.y+50);

			// 논리 ==> 물리
			POINT buf[10];
			DPtoLP(hdc, buf, 10);

			// 결국, 2개의 Point,로 볼 수 있다..
			RECT r = { 10, 10, 20, 20 };
			DPtoLP(hdc, (POINT*)&r, 2);




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