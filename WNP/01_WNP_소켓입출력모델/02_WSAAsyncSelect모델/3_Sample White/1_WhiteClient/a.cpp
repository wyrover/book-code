// user.chol.com/~downboard/async.txt 복사해 오세요.
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

struct LINE
{
	POINTS ptFrom;
	POINTS ptTo;
};

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static SOCKET link_socket;
	static POINTS ptFrom;

	switch( msg )
	{	
	case WM_LBUTTONDOWN: ptFrom = MAKEPOINTS(lParam); return 0;
	case WM_MOUSEMOVE:
		if ( wParam & MK_LBUTTON )
		{
			POINTS pt = MAKEPOINTS(lParam);
			HDC hdc = GetDC( hwnd );

			MoveToEx( hdc, ptFrom.x, ptFrom.y, 0);
			LineTo  ( hdc, pt.x,     pt.y );
			ReleaseDC( hwnd, hdc );

			// 서버로 보낸다.
			LINE line;
			line.ptFrom = ptFrom;
			line.ptTo   = pt;

			send( link_socket, (char*)&line, sizeof(line), 0);
			ptFrom = pt;
		}
		return 0;
						// 서버를 실행하세요. 클라이언트를 실행하세요.
						// 접속 이 되면 클라이 언트에서 그림을 그려보세요.
						// 한사람이 서버를, 옆사람이 client를 실행해 보세요.

	case WM_USER+100:
		{
			WORD event = WSAGETSELECTEVENT( lParam);
			WORD error = WSAGETSELECTERROR( lParam );

			switch( event )
			{
			case FD_CONNECT:
				if ( error != 0 ) 
				{
					MessageBox( 0, "접속 에러", "", MB_OK);	return 0;
				}
				SetWindowText( hwnd, "접속 되었습니다.");

				// 이제 data의 도착과 접속 해지에 대해 요청
				WSAAsyncSelect( link_socket, hwnd, WM_USER+100,
										FD_CLOSE | FD_READ );
				
				return 0;

			case FD_CLOSE:
				SetWindowText( hwnd,"접속이 끊어 졌습니다.");
				closesocket( link_socket);
				return 0;

			case FD_READ:
				{
					// data 도착 - 읽어 낸다.
					LINE line;
					recv( link_socket, (char*)&line, sizeof(line), 0);

					HDC hdc = GetDC( hwnd );
					MoveToEx( hdc, line.ptFrom.x, line.ptFrom.y, 0);
					LineTo  ( hdc, line.ptTo.x,   line.ptTo.y);
					ReleaseDC( hwnd, hdc );
				}
				return 0;
			}
		}
		return 0;

	case WM_CREATE:
		{
			link_socket = socket( AF_INET, SOCK_STREAM, 0);

			// 비동기 상태로 변경한다.
			WSAAsyncSelect( link_socket, hwnd, WM_USER+100, FD_CONNECT );

			SOCKADDR_IN addr;
			addr.sin_family		= AF_INET;
			addr.sin_port		= htons( 9000 );
			addr.sin_addr.s_addr = inet_addr("61.81.99.76"); // 자신 IP

			connect( link_socket, (SOCKADDR*)&addr, sizeof(addr));
		}
		return 0;

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


