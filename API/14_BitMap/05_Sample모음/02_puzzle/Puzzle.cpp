#include <windows.h>
#include <TCHAR.h>
#include <windowsX.h>
#include "resource.h"
#include <time.h>   // <<== 추가해 주세요..

//--------------------------------------------------------------------------
// Function Declaration
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void OnPaint(HWND hwnd);
void OnDestroy( HWND hwnd );
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);

void Swap(int* a, int* b);
BOOL MoveBlock( HWND hwnd, int x, int y );
void Shuffle( HWND hwnd );

//--------------------------------------------------------------------------
// Global Variable

//--------------------------------------------------------------------------
// WinMain
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE,PSTR lpCmdLine, int nShowCmd)
{
	TCHAR szAppName[] = _T("Puzzle Game");
	WNDCLASS wc;
	wc.cbClsExtra    	= 0;
	wc.cbWndExtra   	= 0;
	wc.hbrBackground 	= (HBRUSH)GetStockObject( LTGRAY_BRUSH);
	wc.hCursor       	= LoadCursor( 0, IDC_ARROW );
	wc.hIcon         	= LoadIcon  ( 0, IDI_WINLOGO);
	wc.hInstance     	= hInstance; 
	wc.lpfnWndProc 	= WndProc;
	wc.lpszClassName 	= szAppName; 
	wc.lpszMenuName 	= MAKEINTRESOURCE(IDR_PUZZLEMENU);
	wc.style         = 0; 

	ATOM atom = RegisterClass( &wc );

	HWND hwnd = CreateWindowEx( 0, szAppName, szAppName, 
						WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
						CW_USEDEFAULT, 0, CW_USEDEFAULT,  0, 0, 0, hInstance, 0);

	ShowWindow( hwnd, nShowCmd ); 
	UpdateWindow( hwnd );

	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}



//-------------------------------------------------------------------------
// Window Procedure
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	HANDLE_MSG(hwnd, WM_CREATE,      OnCreate);
	HANDLE_MSG(hwnd, WM_PAINT,       OnPaint);
	HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLButtonDown);
	HANDLE_MSG(hwnd, WM_COMMAND,     OnCommand); // 메뉴 처리..!!
	HANDLE_MSG(hwnd, WM_DESTROY,     OnDestroy);
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}



//--------------------------------------------------------------------------
// Message Handler
//--------------------------------------------------------------------------
// Global Variable
#define COUNT   5
#define EMPTY   (COUNT*COUNT-1)
#define OX		10
#define OY		10   

HBITMAP 	g_hBitmap;
SIZE    	g_szFull;
SIZE    	g_szBlock;
BOOL    	g_bRunning;
int	g_image[COUNT][COUNT];

//--------------------------------------------------------------------------
// Message Handler
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	HINSTANCE hInstace = lpCreateStruct->hInstance;

	// Bitmap Load
	g_hBitmap = (HBITMAP)LoadImage(0, "Puzzle.bmp",IMAGE_BITMAP, 0,0,LR_LOADFROMFILE);

	// 비트맵 헤더를 구해서 전역변수 초기화.
	BITMAP bm;
	GetObject( g_hBitmap, sizeof(bm), &bm);

	g_szFull.cx = bm.bmWidth;
	g_szFull.cy = bm.bmHeight;

	g_szBlock.cx = g_szFull.cx / COUNT;
	g_szBlock.cy = g_szFull.cy / COUNT;
	//=---------------------------------------------------
	// 클라이언트 크기가 520*520 이 되는 윈도우 크기 구하기.
	RECT r = {0, 0, g_szFull.cx + OX * 2, g_szFull.cy + OY*2 };
	AdjustWindowRect( &r, GetWindowLong(hwnd, GWL_STYLE), TRUE);

	int width  = r.right-r.left;
	int height = r.bottom - r.top;

	// 윈도우가 중앙에 오도록..
	int x = (GetSystemMetrics( SM_CXSCREEN ) - width) / 2;
	int y = (GetSystemMetrics( SM_CYSCREEN ) - height) / 2;

	MoveWindow( hwnd, x,y, width, height, TRUE);

	g_bRunning = FALSE;

	int k = 0;
	for ( y = 0; y < COUNT; y++)
		for (int x = 0; x < COUNT; x++)
			g_image[y][x] = k++;


	return TRUE;
}

void OnDestroy( HWND hwnd )
{
	DeleteObject( g_hBitmap );
	PostQuitMessage( 0 );
}
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( hwnd, &ps );
	Rectangle( hdc, OX - 5, OY - 5,	OX + g_szFull.cx + 5,  OY + g_szFull.cy + 5 );

	SetViewportOrgEx( hdc, OX, OY, 0);

	HDC dcMem = CreateCompatibleDC( hdc );
	HBITMAP old = (HBITMAP)SelectObject( dcMem, g_hBitmap);

	for ( int y = 0; y < COUNT; y++)
	{
		for (int x = 0; x < COUNT; x++)
		{
			if ( g_image[y][x] == EMPTY ) // EMPTY
			{
				// 흰색 사각형 그리기.(테두리 없이 )
				PatBlt( hdc, x*g_szBlock.cx, y*g_szBlock.cy, g_szBlock.cx, g_szBlock.cy, WHITENESS);
			}
			else
			{
				POINTS pt = {g_image[y][x] % COUNT, g_image[y][x]/COUNT }; 

				BitBlt( hdc, 
					x * g_szBlock.cx, 
					y * g_szBlock.cy, 
					g_szBlock.cx - 1,  // <<== 실제 블록 크기 보다 1적게 
					g_szBlock.cy - 1,  // <<===
					dcMem, 
					pt.x * g_szBlock.cx , pt.y * g_szBlock.cy, SRCCOPY);
			}
		}
	}
	SelectObject( dcMem, old );
	DeleteDC( dcMem );
	EndPaint( hwnd, &ps );
}

/*
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint( hwnd, &ps );

	// 흰색 사각형을 그린다.
	Rectangle( hdc, OX - 5, OY - 5,	OX + g_szFull.cx + 5,  OY + g_szFull.cy + 5 );

	// 원점을 10,10 으로 이동한다.
	SetViewportOrgEx( hdc, OX, OY, 0);

	// 퍼즐 그림을 출력한다.
	HDC dcMem = CreateCompatibleDC( hdc );
	HBITMAP old = (HBITMAP)SelectObject( dcMem, g_hBitmap);

	int n = 7;
	POINTS pt = { n % COUNT, n / COUNT};

	BitBlt( hdc, 0, 0, g_szBlock.cx, g_szBlock.cy, 
			dcMem, 

			pt.x * g_szBlock.cx ,	// <<===== 
			pt.y * g_szBlock.cy,	// <<=====
			
			SRCCOPY);
	//-------------------------------------------------------------

	SelectObject( dcMem, old );
	DeleteDC( dcMem );
	EndPaint( hwnd, &ps );
}

*/



void Swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
} 

BOOL MoveBlock( HWND hwnd, int x, int y )
{
	// Rectangle Current Block - 마우스를 클릭한 x,y 블록의 크기.
	RECT r = { x * g_szBlock.cx,y * g_szBlock.cy,(x+1) * g_szBlock.cx, (y+1)* g_szBlock.cy };

	// 4방향중 이동 가능 한 곳을 조사.
	if ( y > 0 && g_image[y-1][x] == EMPTY )
	{
		Swap( &g_image[y][x], &g_image[y-1][x]);
		r.top -= g_szBlock.cy;
	}
	else if ( y < COUNT-1 && g_image[y+1][x] == EMPTY )
	{
		Swap( &g_image[y][x], &g_image[y+1][x]);
		r.bottom += g_szBlock.cy;
	}
	else if ( x > 0 && g_image[y][x-1] == EMPTY )
	{
		Swap( &g_image[y][x], &g_image[y][x-1]);
		r.left -= g_szBlock.cx;
	}
	else if ( x < COUNT-1 && g_image[y][x+1] == EMPTY )
	{
		Swap( &g_image[y][x], &g_image[y][x+1]);
		r.right += g_szBlock.cx;
	}
	else
		return FALSE;

	// 계산된 무효화할 영역을 원점 만큼 이동해 준다.
	OffsetRect(&r, OX, OY); 

	// 무효화하면 WM_PAINT 가 해당 2블록을 다시 그려준다 -> 이동하는 효과.
	InvalidateRect( hwnd, &r, FALSE);
	UpdateWindow( hwnd );

	return TRUE;
} 

void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	// 게임이 실행중이 아니거나 게임판을 벗어난 경우
	if ( (!g_bRunning) || ( x < OX ) || ( x > OX + g_szFull.cx) ||
		( y < OY ) || ( y > OY + g_szFull.cy ) )
	return ;

	// 좌표(pixel)  블록 x,y 구하기
	int xBlock = (x - OX) / g_szBlock.cx;
	int yBlock = (y - OY) / g_szBlock.cy;

	// 이동 가능 하면 이동. 불가능하는 FALSE 가 나오는 함수.
	if ( ! MoveBlock( hwnd, xBlock, yBlock) )
		MessageBeep( 0 );
}


void Shuffle( HWND hwnd )
{
	enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3};
	srand(time(0) );
	// 초기 빈곳은 4,4 이다.
	int empty_x = COUNT-1;  // <<---
	int empty_y = COUNT-1;  // <<---
	int count = 0;
	while( 1 )
	{
		// 랜덤 값을 0~3을 얻는다.
		switch( ( rand() % 4) )
		{
		// 랜덤 값에 따라 빈곳의 위,아래, 좌,우 중 1개를 얻는다.
		case LEFT: empty_x = max(0,       empty_x - 1 ); break;
		case RIGHT:empty_x = min(COUNT-1, empty_x + 1 ); break; // <<----
		case UP:   empty_y = max(0,       empty_y - 1 ); break;
		case DOWN: empty_y = min(COUNT-1, empty_y + 1 ); break; // <<----
		}
		// 선택된 블록을 이동 해본다. 이동 불가능 할수도 있다. 250번 섞을때까지!
		if ( MoveBlock( hwnd, empty_x, empty_y) ) 
		{
			++count;
			if ( count == COUNT * 50 ) break; // 250 번 섞는다.
			Sleep(50); // 1블록을 섞은후에 0.05s 대기 한다.
		}
	}
}


void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch( id )
	{
	case ID_GAME_NEW: 
		if ( g_bRunning == TRUE )
		{
			UINT ret = MessageBox(hwnd, "현재 게임을 포기 하시겠습니까 ?", "확인", MB_YESNO);
			if (ret != IDYES ) return;
		} 
		Shuffle( hwnd );
		g_bRunning = TRUE;
			return;


	case ID_GAME_HINT: return;

	case ID_GAME_EXIT:
		SendMessage( hwnd, WM_CLOSE, 0,0);
		return;
	}
}








