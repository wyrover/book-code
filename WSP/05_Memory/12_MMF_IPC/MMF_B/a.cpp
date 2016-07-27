#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
typedef struct _LINE
{
	POINTS ptFrom;
	POINTS ptTo;
} LINE;

DWORD WINAPI foo( void* p )
{
	HWND hwnd = (HWND)p;
	HANDLE hEvent = OpenEvent( EVENT_ALL_ACCESS, FALSE, "DRAW_SIGNAL");
	HANDLE hMap   = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, "map");

	if ( hMap == 0 )
	{
		MessageBox( 0, "MMF_A를 먼저 실행하세요", "", MB_OK);
		return 0;
	}
	//-------------------------------------------------------------------
	LINE* pData = (LINE*)MapViewOfFile( hMap, FILE_MAP_READ, 0, 0, 0);

	while ( 1 )
	{
		// Event를 대기한다.
		WaitForSingleObject( hEvent, INFINITE );

		// 이제 Line의 정보가 pData에 있다.
		HDC hdc = GetDC( hwnd );
		MoveToEx( hdc, pData->ptFrom.x, pData->ptFrom.y, 0);
		LineTo  ( hdc, pData->ptTo.x,   pData->ptTo.y);
		ReleaseDC( hwnd, hdc );
	}
	UnmapViewOfFile( pData );
	CloseHandle( hMap );
	CloseHandle( hEvent );
	return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{	
	case WM_CREATE:
		CloseHandle( CreateThread( 0, 0, foo, (void*)hwnd, 0, 0));
		return 0;
								// MMF_A 를 먼저 실행하세요
								// MMF_B 를 실행하세요
								// MMF_A 에서 그림을 그려 보세요.
								










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

	hwnd = CreateWindowEx( 0, "first", "MMF_B", WS_OVERLAPPEDWINDOW,
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

