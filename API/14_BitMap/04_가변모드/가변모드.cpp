// 가변모드..

#include <windows.h>

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hwnd, &ps);
			// 맵핑 모드 변경
			SetMapMode(hdc, MM_ISOTROPIC);	// 가변 모드로 변경...MM_ANISOTROPIC
//			SetMapMode(hdc, MM_LOENGLISH);	// 비례 가변 모드.. MM_ISOTROPIC


			// 현재 클라이언트의 크기 
			RECT r;
			GetClientRect(hwnd, &r);
			int cx = r.right	- r.left;	// 폭
			int cy = r.bottom	- r.top;	// 높이

			// 논리 좌표의 크기 고정( 윈도우의 범위를 100, 100으로 지정)
			// 논리 좌표 : Window라고도 부름 
			//             논리 좌표가 사용되는 표현(내부적으로 그래픽 출력)
			SetWindowExtEx(hdc, 100, 100, 0);

			// 물리좌표의 크기는 클라이언트의 크기로 지정.
			// 물리 좌표 : viewport라고도 부름
			//             눈에 보이는 좌표 영역 
			SetViewportExtEx(hdc, cx, cy, 0);

			Rectangle(hdc, 10, 10, 90, 90 );
			EndPaint(hwnd, &ps);
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
	wc.style				= CS_HREDRAW | CS_VREDRAW;

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