// 핸들 복사 ...
#include <windows.h>
/*
4G 프로세스의 주소 공간마다 K O H T 이 존재한다.

  ex) 1) CreateFile 생성 -> 리턴값은 file의 핸들.
      2) 리턴된 핸들은 Process 주소공간의 KOT의 Index 값이다..

      3) 만약 어떤 윈도우의 핸들을 가지고 있다고 해도 그 윈도우의
	     K O H T 에 File에 대한 핸들의 Index가 없다면 WriteFile 할
		 수 없다..

      4) 다른 윈도우에서 동일한 작업을 해주고 싶다면
	     DuplicateHandle을 통해 복사해 사용하면 된다..

  ex) 가짜 복사의 예
      h1 = CreateFile(...)
	  h2 = h1;
	  CloesHandle( h1);
	  WriteFile( h2, "aaa");	// 사용 불가

      CloesHandle 함수의 의미 
	  => K O H T 에서 해당 Index(Handle)을 삭제하는 것임.
	     또한 해당 K O 의 참조 개수가 감소한다...
*/
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//---------------------------------
	static PROCESS_INFORMATION	pi;
	static HANDLE	hProcess1;
	static HANDLE	hProcess2;
	//---------------------------------
	switch( msg )
	{
	case WM_LBUTTONDOWN:
		{
			STARTUPINFO	si	= { sizeof(si) };
			si.dwY			= 0;
			si.dwX			= 0;
			si.dwXSize		= 100;
			si.dwYSize		= 100;
			si.dwFlags		= STARTF_USEPOSITION;
			BOOL b = CreateProcess(NULL, "notePad.exe", 
				NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

			// 가짜 복사 
			hProcess1 = pi.hProcess;

			// 진짜 복사 
			DuplicateHandle(  GetCurrentProcess(),	// 이 프로세스안의
							  pi.hProcess,			// 이 핸들을
							  GetCurrentProcess(),	// 이 프로세스에 복사하라. 
							  &hProcess2,			// hProcess에 핸들값 저장
							  0, FALSE,				// 접근 권한, 상속 
							  DUPLICATE_SAME_ACCESS);
			if( b )
			{
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
		}
		return 0;
	case WM_RBUTTONDOWN:
		TerminateProcess(hProcess2, 0);

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