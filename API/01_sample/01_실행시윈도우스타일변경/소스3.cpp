#include <windows.h>


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR h1 = LoadCursorFromFile("c:\\WINDOWS\\Cursors\\sizens.ani");
	static HCURSOR h2 = LoadCursorFromFile("c:\\WINDOWS\\Cursors\\sizewe.ani");

	static int cx = 350, cy = 150;
	switch( msg )
	{
	case WM_CREATE:
		MoveWindow(hwnd, 0, 0, cx, cy, TRUE);
		return 0;

	case WM_KEYDOWN:
		// 가상키...

		// wParam 의 하위 2byte => 키보드에서 발생한 메시지가 저장됨
		// ex) VK_RIGHT 의 키값이 48 이라면 wParam 에는 0048의 값이 저장된다.
		if ( LOWORD (wParam) == VK_RIGHT)
		{
			cx  = cx+5;

			// 윈도우 창 변경
		}
		else if( LOWORD(wParam) == VK_LEFT)
		{
			cx = cx-5;
		}
		else if( LOWORD(wParam ) == VK_UP)
		{
			cy = cy-5;
		}
		else if (LOWORD(wParam) == VK_DOWN)
		{
			cy = cy+5;
		}
		MoveWindow(hwnd, 100, 100, cx, cy, TRUE);

		return 0;











	// 마우스 이동시 호출..
	case WM_SETCURSOR:
		switch( LOWORD(lParam)) // Hit test code
		{
		case HTTOP:
		case HTBOTTOM:
			SetCursor(h1);				break;

		case HTLEFT:
		case HTRIGHT:	
			SetCursor(h2);				break;

		default:
			// 나머지의 위치에 있을때는 반드시 DefWindowProc에서 변경하도록
			// 처리해준다.
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
	case WM_LBUTTONDOWN:
		return 0;
	case WM_RBUTTONDOWN:
		// 창의 제목을 얻고 변경하기( Get/SetWindowText)
		char szTitle[80];
		GetWindowText(hwnd, szTitle, strlen(szTitle));
		MessageBox(hwnd, szTitle, "창 제목", MB_OK);

		SetWindowText(hwnd, "창 제목을 변경한다.");
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
	wc.hCursor				= LoadCursor(0, IDC_CROSS);
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
	ShowWindow(hwnd, nShowCmd);
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