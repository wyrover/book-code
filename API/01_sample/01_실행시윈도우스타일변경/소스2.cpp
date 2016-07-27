// Cursor 변경...

#include <windows.h>

// 마우스 커서가 움직일 때마다 WM_SETCURSOR 메시지가 발생한다..
// 디폴트 처리 루틴...
// 커서를 윈도우 클래스에 등록된 것으로 바꾸어 버린다...

// SetCursor,  WM_SETCURSOR
// SetClassLong 을 통한 변경 차이 이해..

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR	hNowCursor;

	static HCURSOR  hArrow, hIBeam, hWait, hNow;
	switch( msg )
	{

	case WM_CREATE:
		// LoadCursor
		hArrow = LoadCursor(NULL, IDC_ARROW);
		hIBeam = LoadCursor(NULL, IDC_IBEAM);
		hWait  = LoadCursor(NULL, IDC_WAIT);
		hNow   = hArrow;
		return 0;

	case WM_LBUTTONDOWN:
		// L Button 누를때마다 커서의 모양 변경
		if( hNow == hArrow)			hNow = hIBeam;
		else if( hNow == hIBeam)	hNow = hWait;
		else						hNow = hArrow;

//		SetCursor(hNow);
		// 적용 : SetClassLong
		SetClassLong(hwnd, GCL_HCURSOR, (LONG)hNow);
		return 0;


/*
	case WM_SETCURSOR:
//		if( LOWORD(lParam) == HTCAPTION)
		if( LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(hNowCursor);
			return TRUE;
		}
		break;
	case WM_LBUTTONDOWN:
		hNowCursor = LoadCursor(NULL, IDC_WAIT);
		return 0;

	case WM_RBUTTONDOWN:
		hNowCursor = LoadCursor(NULL, IDC_ARROW);
		return 0;
*/
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

	// 커서 변경
	// 현재 windows 가 설치된 directory얻기
	char dir[256];
	GetWindowsDirectory(dir, 256);
	strcat(dir, "\\cursors\\dinosaur.ani");
	HCURSOR hc				= LoadCursorFromFile(dir);
//	HCURSOR hc				= LoadCursorFromFile("c:\\WINDOWS\\Cursors\\banana.ani");
	

	wc.hCursor				= hc;  //LoadCursor(0, IDC_CROSS);
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