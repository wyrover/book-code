//------------------------------------------------------------
// [ BITMAP 사용하기 ]
// 
//  두종류의 BITMAP  
//     - DDB(Device Dependant Bitmap)
//     - DIB(Device InDependant Bitmap)

//  LoadBitmap : 리소스에 있는 Bitmap 가져오기
//  LoadImage  : 파일에 있는 Bitmap 가져오기 
//------------------------------------------------------------



#include <windows.h>

#include "resource.h"

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS wc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
		
	// 리소스에 있는 비트맵 Load하기.
	//HBITMAP hBitmap = LoadBitmap( hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

	// 화일을 바로 Load하기 - JPG등 다른 그림을 다루고 싶다면 인터넷에서
	//						 intel jpeg 라이브러리를 구해 보세요.
	HBITMAP hBitmap = (HBITMAP)LoadImage( 0,  // 리소스에서 로드할때만 사용
										"C:\\windows\\부채.bmp",
										IMAGE_BITMAP,
										0, 0, // 커서, Icon load시만 사용
										LR_LOADFROMFILE);
										
	// 비트맵으로 브러시 만들기
	HBRUSH brush = CreatePatternBrush( hBitmap );

	// 비트맵은 사용후 만드시 파괴해야 한다.
	DeleteObject( hBitmap);

	wc.hbrBackground = brush; //(HBRUSH)GetStockObject( WHITE_BRUSH ); 
	//-----------------------------------------------------------


	wc.hCursor       = LoadCursor( 0, IDC_ARROW );	
	wc.hIcon         = LoadIcon( 0, IDI_WINLOGO);   
	wc.hInstance     = hInstance;
	wc.lpfnWndProc   = WndProc; 
	wc.lpszClassName = "First"; 
	wc.lpszMenuName  = 0;
	wc.style         = 0;

	ATOM atom = RegisterClass( &wc );

	HWND hwnd = CreateWindowEx( 0, "First", "Hello", WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,0, 0,  0,0);

	ShowWindow( hwnd, nShowCmd );

	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0) )
	{
		DispatchMessage( &msg );
	}

	return 0;
}
