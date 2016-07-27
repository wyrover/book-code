#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
void Draw3dRect(HDC hdc, int x, int y, int xx, int yy, 
					BOOL down, int width )
{
	COLORREF clrMain  = RGB(192,192,192),
				clrLight = RGB(255,255,255), 
				clrDark  = RGB(128,128,128);

	HPEN hPen1, hPen2, hOldPen;
	HBRUSH hBrush, hOldBrush;     

	if(down)    
	{
		hPen2 = CreatePen(PS_SOLID,1,clrLight);
		hPen1 = CreatePen(PS_SOLID,1,clrDark);
	} 
	else 
	{
		hPen1 = CreatePen(PS_SOLID,1,clrLight);
		hPen2 = CreatePen(PS_SOLID,1,clrDark);
	}  
	hBrush =  CreateSolidBrush( clrMain );
	hOldPen = (HPEN)SelectObject(hdc, hPen1);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  

	Rectangle(hdc, x , y, xx+1, yy+1);  

	for(int i=0; i < width; i++)
	{
		SelectObject(hdc, hPen1);

		MoveToEx(hdc, xx - 1, y, 0 );
		LineTo(hdc, x, y);
		LineTo(hdc, x, yy - 1 );  

		SelectObject(hdc, hPen2);

		MoveToEx(hdc, x, yy,0);
		LineTo(hdc, xx, yy);
		LineTo(hdc, xx, y);  

		x++; y++; xx--; yy--;
	} 
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen1); 
	DeleteObject(hPen2); 
	DeleteObject(hBrush);
} 

enum STATE { EMPTY = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, 
			BOMB = 99};
#define CLOSE 0
#define OPEN  1
#define FLAG  2

typedef struct _BLOCK
{
	STATE state;
	int   isOpen;
} BLOCK;

BLOCK g_board[9][9];

void Init()
{
	for ( int x = 0; x < 9; ++x )
		for ( int y = 0; y < 9; ++y )
			g_board[x][y].isOpen = CLOSE;

	// random 하게 폭탄을 배치하고 배열의 state 항목을 초기화 한다.
}
















LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{	
	case WM_LBUTTONDOWN:
		{



		}
		return 0;
/*
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			int x, y;

			hdc = BeginPaint( hwnd, &ps );
			Draw3dRect( hdc, 0,   0,  168, 211, FALSE, 3 );
			Draw3dRect( hdc, 9,   9,  159, 46,  TRUE,  2 );
			Draw3dRect( hdc, 15,  14, 57,  40,  TRUE,  1 );
			Draw3dRect( hdc, 109, 14, 150, 40,  TRUE,  1 );
			Draw3dRect( hdc, 9,   52, 159, 202, TRUE,  3 );

			for ( x = 12; x <= 140; x += 16 )
				for ( y = 55; y <= 183 ; y += 16)
					Draw3dRect( hdc, x, y, x + 15, y + 15, FALSE, 2 );
			EndPaint( hwnd, &ps );
		}
		return 0;
*/




	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hwnd, &ps );
			Draw3dRect( hdc, 10, 10, 100, 100, FALSE, 2 );
										// 2대신 다른 숫자 넣어 보세요.
										// FALSE 대신 TRUE 넣어 보세요.
			EndPaint( hwnd, &ps );
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

	hwnd = CreateWindowEx( 0, "first", "Hello", 
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
								
		0, 0, 175,240, 
	
		0, 0, 
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

