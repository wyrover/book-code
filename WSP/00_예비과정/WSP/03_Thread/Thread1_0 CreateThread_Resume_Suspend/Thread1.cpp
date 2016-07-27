#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
#include <commctrl.h>

// Window 환경에서의 스레드 함수의 모양. - 외우세요.
DWORD WINAPI foo( void* p)
{
	HWND hPrg = (HWND)p;

	for ( int i = 0; i < 1000; ++i )
	{
		SendMessage( hPrg, PBM_SETPOS, i, 0); // 프로그래스 전진
		for ( int k = 0; k < 5000000; ++k ); // 0 6개 - some work.!!
	}
	return 0;
}



LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hPrg;
	static HANDLE hThread;
	switch( msg )
	{	
	case WM_CREATE:
		hPrg = CreateWindow( PROGRESS_CLASS, "", 
							 WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_SMOOTH,
							 10, 10, 500,30, hwnd, (HMENU)1, 0, 0);

		//범위:0 ~1000 초기위치:0으로 초기화.
		SendMessage( hPrg, PBM_SETRANGE32, 0, 1000);
		SendMessage( hPrg, PBM_SETPOS,     0, 0);
		return 0;
	case WM_LBUTTONDOWN: 
		{
			// 새로운 스레드를 만들어서 작업을 시키고 주스레드는 최대한 빨리
			// 메세지 루프로 돌아 가서 다음 메세지를 처리한다.
			DWORD tid;
			hThread = CreateThread( 0, 0, // TKO 보안, Stack 크기
								foo, (void*)hPrg, // 스레드로 실행할 함수,인자
								CREATE_SUSPENDED,// 생성하지만 실행은 하지 않는다.
								&tid);			// 생성된 스레드 ID를 담을 변수
			//CloseHandle( hThread ); // TKO의 참조개수를 초기에 2이다.
							// 스레드 종료와 함께 즉시 파괴되도록 1 줄인다.
		}
		return 0;

	case WM_RBUTTONDOWN:
		{
			static BOOL bRun = FALSE;

			bRun = !bRun; // Toggle
			if ( bRun )
				ResumeThread( hThread );	// 스레드 재개
			else
				SuspendThread( hThread );  // 스레드 일시 중지
		}
		return 0;


	case WM_DESTROY:
		CloseHandle( hThread);
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}












int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
{
	ATOM atom;
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "First";
	wc.lpszMenuName = 0;
	wc.style		= 0;

	atom = RegisterClass( &wc);
	
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( 0, "first", "Hello", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT,0, 0, 0, 
								hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}

