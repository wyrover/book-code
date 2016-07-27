// 프로세스 종료 코드 얻기
/*
 Process 종료 코드 값 얻기 
 1. GetExitCodeProcess 함수 사용 
 2. TerminateProcess함수 ==> 타살... 강제 종료...
    - 비동기.. => 함수 호출시 곧바로 종료를 시키는 것이 아니라
	              함수는 리턴하고 그 다음 명령을 실행한다...
    - OS에서는 이 프로세스를 곧바로 죽이지 않고 종료를 위한 여러가지 작업을 한다.

    - Process에서 Signal 값은 살았을 때 0, 죽었을 때 1(Signal상태)의 값을 갖는다.
	0 -> 1 : 살은놈 죽이기 
	1 -> 0 : 죽이는 놈 살리기(당연히 안됨)..

 3. WaitForSingleObject() 를 사용하여 Signal 상태를 알 수 있다.
    - 대기 함수 
	- 대기하는 객체가 Signal 될 때, 
	- 대기하고자 하는 시간이 지정되 있을 때 (INFINITE  : 무한정 대기)

*/
#include <windows.h>


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//-------------------------------
	static PROCESS_INFORMATION pi;
	//--------------------------------
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			STARTUPINFO	si	= { 0 };
			si.cb			= sizeof(si);
			si.dwXSize		= 200;
			si.dwYSize		= 200;
			si.dwFlags		= STARTF_USESIZE;	

			BOOL b = CreateProcess(NULL,//"C:\\WINDOWS\\NotePad.exe",		// 프로그램 이름
								"calc.exe", // 프로그램 이름 + 파라미터
								0, 0,		// Process, Thread KO의 보안속성 지정
								FALSE,		// 상속여부 
								NORMAL_PRIORITY_CLASS, // 우선순위 (8)
								0, 0,		// 프로세스의 환경블록, 현재 Dir 
								&si,		// 시작 정보를 담은 구조체
								&pi);		// 프로세스, 스레드 핸들, ID를 담을 구조체
			if( b )
			{
				// 프로세스나 Thread의 핸들이 더 이상 필요없을 경우 받드시 닫는다.
				// 메모리...
//				CloseHandle(pi.hProcess);
//				CloseHandle(pi.hThread);
			}
		}
		return 0;
	case WM_RBUTTONDOWN:
		// Ctrl + RBUTTONDOWN
		if( wParam & MK_CONTROL)
		{
			// 비동기함수다..
			TerminateProcess(pi.hProcess, 100);	// 핸들값. 종료 코드값

			// 대기함수
			// 1) 해당되는 핸들이 시그널될때 진행
			// 2) 시간이 경과되면.. 1초가 지나면
			WaitForSingleObject(pi.hProcess, INFINITE);
			SendMessage(hwnd, WM_RBUTTONDOWN, MK_SHIFT, lParam);
		}

		// SHIFT + RBUTTONDOWN
		if( wParam & MK_SHIFT)
		{
			// Process의 종료 코드 얻기
			DWORD code;
			GetExitCodeProcess(pi.hProcess	, &code);
			if( code == STILL_ACTIVE)	// 0x13
			{
				MessageBox(hwnd, "아직 살아 있다.", "알림", MB_OK);
			}
			else
			{
				char buf[256];
				wsprintf(buf, "종료 : %d", code);
				MessageBox(hwnd, buf, "알림", MB_OK);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
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