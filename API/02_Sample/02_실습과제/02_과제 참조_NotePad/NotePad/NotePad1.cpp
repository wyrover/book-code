// ver_3 . 최소화 메시지 처리

#include <windows.h>
#include "resource.h"

void ModifyStyle( HWND hwnd, UINT Remove, UINT Add, BOOL bRedraw = TRUE)
{
	UINT style = GetWindowLong(hwnd, GWL_STYLE);
	style = style | Add;
	style = style & ~Remove;
	SetWindowLong(hwnd, GWL_STYLE, style);

	if( bRedraw )
		SetWindowPos( hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

BOOL bFull = FALSE;	// 전체 보기인가??

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//-------------
	static HWND hEdit;
	//--------------

	switch( msg )
	{
	case WM_SETFOCUS:
		SetFocus(hEdit);
		return 0;
	case WM_CREATE:
		hEdit = CreateWindowEx(0, "edit", "",
					WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_VSCROLL  | WS_HSCROLL,
					10, 10, 100, 100, hwnd, (HMENU)1, 0, 0);
		return 0;
	case WM_SIZE:
		{
			int cx = LOWORD(lParam);
			int cy = HIWORD(lParam);
			
			MoveWindow(hEdit, 0, 0, cx, cy, TRUE);
		}
		return 0;

	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE)
		{
			SendMessage(hwnd, WM_COMMAND, IDM_VIEW_FULL, 0);
		}
//			MessageBox(0, "다시 작게", "", MB_OK);
		return 0;
	case WM_COMMAND:
		switch( LOWORD(wParam))
		{
		case IDM_VIEW_FULL:
			{
				static HMENU	hMenu;
				bFull = !bFull;
				if( bFull )
				{
					ModifyStyle(hwnd, WS_CAPTION, 0);
					hMenu =  GetMenu(hwnd);
					SetMenu(hwnd, 0);
					ShowWindow(hwnd, SW_MAXIMIZE);
				}
				else
				{
					ModifyStyle(hwnd, 0, WS_CAPTION);
					SetMenu(hwnd, hMenu);
					ShowWindow(hwnd, SW_SHOWNORMAL);
				}
			}
		}
		return 0;
	case WM_LBUTTONDOWN:
		return 0;
	case WM_RBUTTONDOWN:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{
	// 1. 윈도우 클래스 만들기 
	WNDCLASS wc;
	wc.cbWndExtra			= 0;
	wc.cbClsExtra			= 0;
	wc.hbrBackground		= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor				= LoadCursor(0, IDC_ARROW);
	wc.hIcon				= LoadIcon(0, IDI_APPLICATION);
	wc.hInstance			= hInstance;
	wc.lpfnWndProc			= WndProc;			// DefWindowProc;
	wc.lpszClassName		= "First";
	wc.lpszMenuName			= MAKEINTRESOURCE(IDR_MENU1);
	wc.style				= 0;

	// 2. 등록(레지스트리에)
	RegisterClass(&wc);

	// 3. 윈도우 창 만들기 
	HWND hwnd = CreateWindowEx( 0,					// WS_EX_TOPMOST
				 "first",				// 클래스 명
				 "NotePad",				// 캡션바 내용
				 WS_OVERLAPPEDWINDOW, 
				 0 , 0, 500, 500,	// 초기 위치 
				 0, 0,			// 부모 윈도우 핸들, 메뉴 핸들
				 hInstance,		// WinMain의 1번째 파라미터 (exe 주소)
				 0);			// 생성 인자



	// 4. 윈도우 보여주기
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	

	// 5. Message 
	MSG msg;
	while( GetMessage( &msg, 0, 0, 0 ) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}