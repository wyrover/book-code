#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

typedef struct _LINE
{
	POINTS ptFrom;
	POINTS ptTo;
} LINE;

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hEvent, hMap;
	static LINE*  pData;
	static POINTS ptFrom;
	switch( msg )
	{	
	case WM_LBUTTONDOWN: ptFrom = MAKEPOINTS(lParam);return 0;
	case WM_MOUSEMOVE:
		if ( wParam & MK_LBUTTON )
		{
			POINTS pt = MAKEPOINTS(lParam);
			HDC hdc = GetDC( hwnd );

			MoveToEx( hdc, ptFrom.x, ptFrom.y, 0);
			LineTo  ( hdc, pt.x,     pt.y );
			ReleaseDC( hwnd, hdc );
			
			// MMF 에 넣는다.
			pData->ptFrom = ptFrom;
			pData->ptTo   = pt;

			SetEvent( hEvent );

			ptFrom = pt;
		}
		return 0;
			





	case WM_CREATE:
		hEvent = CreateEvent( 0, 0, 0, "DRAW_SIGNAL");

		hMap = CreateFileMapping( (HANDLE)-1, // Paging 화일을 사용해서 매핑
								  0, PAGE_READWRITE, 0, sizeof(LINE), "map");

		pData = (LINE*)MapViewOfFile( hMap, FILE_MAP_WRITE, 0, 0,0);

		if ( pData == 0 )
			MessageBox( 0, "Fail", "", MB_OK);		
		return 0;


	case WM_DESTROY:
		UnmapViewOfFile( pData );
		CloseHandle( hMap );
		CloseHandle( hEvent );


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

	hwnd = CreateWindowEx( 0, "first", "MMF_A", WS_OVERLAPPEDWINDOW,
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

