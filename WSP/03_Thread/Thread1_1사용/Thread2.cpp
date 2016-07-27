// 디렉토리 감시.

#include <windows.h>

// 스레드 함수
DWORD WINAPI	CFunc( LPVOID p)
{
	HWND	hwnd = (HWND)p;

	// hChange : 디렉토리를 감시할 수 있는 커널 오브젝트 
	// 디렉토리의 변화가 있다면 Signal 상태로 변경된다.
	HANDLE	hChange = FindFirstChangeNotification(
		"c:\\"	,	// 감시할 디렉토리
		TRUE,		// 하위 검사 여부 
		FILE_NOTIFY_CHANGE_DIR_NAME);	// 디렉토리 감시 플레그
	
	while(1)
	{
		// Signal 상태가 되면 리턴되는 대기 함수 
		WaitForSingleObject(hChange, INFINITE);
		// 스레드는 변화 감시만 하고, 처리는 주스레드에서 해주는 것이 좋다.
		// file 시스템의 변화를 주스레드에 통지한다.
		PostMessage(hwnd, WM_USER+100, 0, 0);
		MessageBox(hwnd, "WM_USER+100", 0, 0);
		MessageBox(NULL, "Change...", "", MB_OK);
		// 계속해서 감시 가능..
		FindNextChangeNotification(hChange);
	}
	// 메시지 큐는 스레드 생성시마다 각각 따로 존재할 수 있다.
	// 메시지 꺼내는 while문을 통해 구현하면 됨..
	MSG msg;
	while( GetMessage(&msg, 0, 0, 0))
	{
		if( msg.message == WM_USER + 200 )
		{
			MessageBox(NULL, "test", "", MB_OK);
			break;
		}
	}
	// 감시를 종료한다.
	FindCloseChangeNotification(hChange);
	return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static DWORD	ThreadID;
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		// 스레드 생성
		CreateThread(NULL, 0, CFunc, hwnd, 0, &ThreadID);
		return 0;
	case WM_RBUTTONDOWN:
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