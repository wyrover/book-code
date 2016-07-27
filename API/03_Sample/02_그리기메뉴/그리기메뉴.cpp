// 그리기 메뉴...
#include <windows.h>
#include "resource.h"

// 메뉴 항목을 그리기 메뉴로 변경하고 Icon을 ItemData에 보관
void AddMenuIcon(HMENU hMenu, UINT id, HICON hIcon)
{
	ModifyMenu(hMenu, id, MF_BYCOMMAND | MF_OWNERDRAW, id, 0);

	MENUITEMINFO	mmi = { 0 };
	mmi.cbSize			= sizeof(mmi);
	mmi.dwItemData		= (DWORD)hIcon;
	mmi.fMask			= MIIM_DATA;		// mask 구조체중 실제 적용할 항목만 지정

	// 2번쨰  파라미터가 ID로 사용(TRUE: 위치)
	SetMenuItemInfo(hMenu, id, FALSE, &mmi);
}

// 메뉴에 IDM_MENU_AAA, IDM_MENU_BBB, IDM_MENU_CCC..
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch( msg )
	{
	case WM_CREATE:
		{
			// 그리기 메뉴 1단계 : 메뉴의 속성 변경..
			HMENU  hMenu		= GetMenu(hwnd);
			HMENU hSubMenu		= GetSubMenu(hMenu, 0);

			AddMenuIcon( hSubMenu, IDM_MENU_AAA, LoadIcon(0, IDI_WINLOGO));
			AddMenuIcon( hSubMenu, IDM_MENU_BBB, LoadIcon(0, IDI_APPLICATION));
			AddMenuIcon( hSubMenu, IDM_MENU_CCC, LoadIcon(0, IDI_QUESTION));
		}
		return 0;
		
	case WM_MEASUREITEM:
		{
			// 그리기 메뉴 2단계 : 크기 지정
			PMEASUREITEMSTRUCT p = (PMEASUREITEMSTRUCT)lParam;
			p->itemWidth		 = GetSystemMetrics(SM_CYICON) *4;
			p->itemHeight		 = GetSystemMetrics(SM_CYICON);
		}
		return 0;
		
	case WM_DRAWITEM:
		{
			// 그리기 메뉴 3단계 : 메뉴 항목에 그림을 그림..
			PDRAWITEMSTRUCT p	= (PDRAWITEMSTRUCT)lParam;

			HDC hdc		= p->hDC;
			RECT rc		= p->rcItem;
			int  id		= p->itemID;
			int state   = p->itemState;

			HMENU hMenu = (HMENU)(p->hwndItem);  
			//-----------------------------------------------------
			COLORREF  color;
			if( state & ODS_SELECTED)
				color = GetSysColor(COLOR_HIGHLIGHT); // 진한 청색
			else
				color = RGB(255, 255, 255);

			HBRUSH brush = CreateSolidBrush(color);
			FillRect(hdc, &rc, brush);
//			FrameRect(hdc, &rc, brush);

			DeleteObject(brush);
			SetBkMode(hdc, TRANSPARENT);  // 글자의 배경을 투명하게.
			//--------------------------------------------------------
			// 해당 메뉴Item의 itemData를 바로 얻을 수 있다.
			HICON hIcon = (HICON)(p->itemData);	

			DrawIcon(hdc, rc.left, rc.top, hIcon);
			char str[256];
			GetMenuString(hMenu, id, str,256,  MF_BYCOMMAND);
			TextOut(hdc, rc.left + 35, rc.top + 5, str, strlen(str));
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
				 "Hello",				// 캡션바 내용
				 WS_OVERLAPPEDWINDOW, 
				 CW_USEDEFAULT , 0, CW_USEDEFAULT, 0,	// 초기 위치 
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