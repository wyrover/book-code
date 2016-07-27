#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

struct MOUSE
{
	int message;
	POINTS point;
	WPARAM wParam;
	LPARAM lParam;
};
char* server_ip = "192.168.34.91";

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SOCKET	    s;
	static SOCKADDR_IN addr;
	switch( msg )
	{	
	case WM_CREATE:

		s = socket( AF_INET, SOCK_DGRAM, 0);
		
		addr.sin_family		 = AF_INET;
		addr.sin_port		 = htons( 7000 );
		addr.sin_addr.s_addr = inet_addr( server_ip );
		return 0;

	case WM_KEYDOWN:
	case WM_KEYUP:
		{
			MOUSE mouse = { 0 };
			mouse.message = msg;

			mouse.wParam = wParam;
			mouse.lParam = lParam;

			sendto( s, (char*)&mouse, sizeof(mouse), 0, (SOCKADDR*)&addr, 
													sizeof(addr));
		}
		return 0;
			
	


	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			MOUSE mouse;
			mouse.message = msg;
			mouse.point = MAKEPOINTS( lParam);

			// 서버로 보낸다.
			sendto( s, (char*)&mouse, sizeof(mouse), 0, 
												(SOCKADDR*)&addr, sizeof(addr));
		}
		return 0;
						// 한사람이 서버 실행하고
						// 옆사람이 클라이 언트 실행 해보세요.




	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
{
	WSADATA wsadata;
	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		MessageBox( 0, "Error", "", MB_OK);
		return 0;
	}

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

	WSACleanup();
	return 0;
}


