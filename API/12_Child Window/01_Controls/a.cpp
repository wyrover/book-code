#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

// control들의 ID값...
#define IDC_BUTTON	1
#define IDC_EDIT	2
#define IDC_LISTBOX	3
//----------------------------------------

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hBtn,  hEdit, hListBox;

	static HBRUSH hBrush = CreateSolidBrush( RGB(255,255,0));

	switch( msg )
	{	
	// EditBox는 자신이 그려지기 전에 부모에게 아래 메세지를 전달한다.
	case WM_CTLCOLOREDIT:
		{
			HDC hdc = (HDC)wParam; // Edit 가 사용할 dc
			HWND h  = (HWND)lParam;

			if ( h == hEdit )
			{
				SetTextColor( hdc, RGB(0,0,255));
				SetBkColor  ( hdc, RGB(255,255,0));
			}
		}
		// 색상을 변경하지 않은 EditBox의 경우 디폴트 색상이 적용되게 하기 위해
		// 아래로 전달한다.
		return DefWindowProc( hwnd, msg, wParam, lParam);




	// 자식 control 들이 보내는 메세지
	case WM_COMMAND:
		switch( LOWORD(wParam) ) // ID 조사.
		{
		case IDC_EDIT:		// EditBox 가 보낸경우
			if ( HIWORD(wParam) == EN_CHANGE ) //통지 코드 조사.
			{
				char s[256];
				GetWindowText( hEdit, s, 256); // Edit 에서 값을 얻는다.

				SetWindowText( hwnd, s); // 부모 윈도우의 캡션을 변경한다.
			}
			break;

		case IDC_BUTTON:
			if ( HIWORD( wParam) == BN_CLICKED ) // 버튼을 누를때 나오는 통지 코드 
			{
				MessageBox( hwnd, "Click", "", MB_OK);
			}
			break;
		}
		return 0;







	case WM_CREATE:
		
		hBtn = CreateWindow( "button", "Push", 
							WS_CHILD | WS_VISIBLE | WS_BORDER | BS_AUTOCHECKBOX,
							
							10,10,90,90, hwnd, (HMENU)IDC_BUTTON, 0,0);


		hEdit = CreateWindow( "Edit", "", 
							WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE |
							WS_HSCROLL | WS_VSCROLL,

							110,10,90,90, hwnd, (HMENU)IDC_EDIT, 0,0);

		hListBox = CreateWindow( "listbox", "", 
							WS_CHILD | WS_VISIBLE | WS_BORDER,
							210,10,90,90, hwnd, (HMENU)IDC_LISTBOX, 0,0);
		
		// Edit가 입력 받는 글자 갯수를 제한한다.- 메세지를 보낸다.
	//	SendMessage( hEdit, EM_LIMITTEXT, 5, 0);

		SendMessage( hEdit, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN,
											MAKELONG( 10,10));
		// ListBox 에 항목을 추가한다.
		SendMessage( hListBox, LB_ADDSTRING, 0, (LPARAM)"AAAA");
		SendMessage( hListBox, LB_ADDSTRING, 0, (LPARAM)"한글");

			
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

	hwnd = CreateWindowEx( 0, "first", "Hello", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT,
								0, 0, 0, 
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

