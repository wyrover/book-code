#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
// TransparentBlt 는 msimg32.dll 에 있다. msimg32.dll 을 사용하려면
#pragma comment( lib, "msimg32.lib")

// 전역변수.
HBITMAP g_pan, g_black, g_white;

#define BLACK -1
#define WHITE  1
#define EMPTY  0

int g_count = 1; // 돌은 놓은 순서
int g_board[15][15] = { EMPTY }; // 초기 바둑판은 비어 있는 상태.
int g_next = BLACK;	// 바둑돌 놓을 차례.






// x,y : pixel 이 아니라 바둑판 x, y 줄 ..
void DrawStone( HDC hdc, int x, int y, int stone )
{
	HBITMAP old;
	HDC mem = CreateCompatibleDC( hdc );
	if ( stone < 0 ) old = (HBITMAP)SelectObject( mem, g_black);
	else             old = (HBITMAP)SelectObject( mem, g_white);

//	BitBlt( hdc, 7 + x*26, 7 + y*26, 26,26, mem, 0, 0, SRCCOPY );

	// 특정 색은 투명하게 출력 한다.(사각형이 아닌 형태의 출력)
	TransparentBlt( hdc, 7 + x*26, 7+y*26, 26,26,
					mem, 0, 0, 26, 26,
					RGB(255,0,255)); // 투명 처리할 색.

	SelectObject( mem, old );
	DeleteDC( mem );
}








LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_RBUTTONDOWN:
		{
			if ( g_next != WHITE ) return 0; // 내차례가 아니다.

			POINTS pt = MAKEPOINTS(lParam);
			int x = (( pt.x - 20 ) + 13 ) / 26;
			int y = (( pt.y - 20 ) + 13 ) / 26;
			if ( x < 0 || x > 14 || y < 0 || y > 14 ) return 0;

			if ( g_board[x][y] != EMPTY ) // 이미 다른 돌이 있다.
			{
				MessageBeep( 0 ); // "삑"
				return 0;
			}
			//----------------------------------
			g_board[x][y] = WHITE * g_count; // 
	
			HDC hdc = GetDC( hwnd );
			DrawStone( hdc, x, y, WHITE );
			ReleaseDC( hwnd, hdc );
			
			++g_count;
			g_next = BLACK;
		}
		return 0;		

	case WM_LBUTTONDOWN:
		{
			if ( g_next != BLACK ) return 0; // 내차례가 아니다.

			POINTS pt = MAKEPOINTS(lParam);
			int x = (( pt.x - 20 ) + 13 ) / 26;
			int y = (( pt.y - 20 ) + 13 ) / 26;
			if ( x < 0 || x > 14 || y < 0 || y > 14 ) return 0;

			if ( g_board[x][y] != EMPTY ) // 이미 다른 돌이 있다.
			{
				MessageBeep( 0 ); // "삑"
				return 0;
			}
			//----------------------------------
			g_board[x][y] = BLACK * g_count; // 
	
			HDC hdc = GetDC( hwnd );
			DrawStone( hdc, x, y, BLACK );
			ReleaseDC( hwnd, hdc );
			
			++g_count;
			g_next = WHITE;
		}
		return 0;
								// 이제 LBUTTONDOWN을 복사 하세요..
								//      RBUTTONDOWN으로 만드세요.
								//      BLACK 대신 -> WHITE 로 변경하세요.





	// 1. WM_CREATE: 필요한 자원 Load,     
	case WM_CREATE:
		g_pan=(HBITMAP)LoadImage(0,"pan.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		g_black=(HBITMAP)LoadImage(0,"black.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		g_white=(HBITMAP)LoadImage(0,"white.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		return 0;


	// 2. WM_PAINT : 바둑판 배경 출력.
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );

			HDC mem = CreateCompatibleDC( hdc );
			HBITMAP old = (HBITMAP)SelectObject( mem, g_pan);
			
			BitBlt( hdc, 0, 0, 407, 407, mem, 0, 0, SRCCOPY );

			SelectObject( mem, old );
			DeleteDC( mem );
			
			//--------------------------
			// 바둑돌도 그려준다.
			for ( int x = 0; x < 15; ++x )
			{
				for ( int y = 0; y < 15; ++y)
				{
					if ( g_board[x][y] != EMPTY ) // 빈곳이 아니라면
						DrawStone( hdc, x, y, g_board[x][y]);
				}
			}

			EndPaint( hwnd, &ps );
		}
		return 0;


	//    WM_DESTROY:자원 파괴.
	case WM_DESTROY:
		DeleteObject( g_pan );
		DeleteObject( g_white );
		DeleteObject( g_black );

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
	//-------------------------------------------
	UINT style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	RECT rc = { 0, 0, 407, 407 }; // 클라이 언트 크기

	// 클라이언트의 크기를 윈도우 크기로 변경한다.
	AdjustWindowRect( &rc,    
					  style,	// 윈도우 style
					  FALSE);	// 메뉴가 있는가 ?

	int width  = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	
	int ox = (GetSystemMetrics( SM_CXSCREEN ) - width ) / 2;
	int oy = (GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;

	hwnd = CreateWindowEx( 0, "first", "Hello", style,
								ox, oy, width, height,
								0, 0, 
								hInstance, 0); 
	//------------------------------------------------------------





	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}








