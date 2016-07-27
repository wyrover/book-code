#define _WIN32_WINNT	0x500
#define WINVER		    0x500
#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH	edit_brush;
	switch (msg)
	{
	// 초기화... 
	case WM_INITDIALOG:
		{
			HBITMAP bitmap = (HBITMAP)LoadImage(0, "c:\\WINDOWS\\부채.bmp", 
				IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

			// Menu Info 구조체를 통한 메뉴의 이미지 변경..
			// #define _WIN32_WINNT	0x500
			// #define WINVER		    0x500
			MENUINFO mi;
			mi.cbSize	= sizeof(mi);
			mi.hbrBack	= CreatePatternBrush(bitmap);
			mi.fMask	= MIM_BACKGROUND;
			HMENU	hMenu = GetMenu(hDlg);
			SetMenuInfo(hMenu, &mi);

			DeleteObject(bitmap);
			//-----------------------------------------------
			SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(0,IDI_APPLICATION));
			SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(0, 
				IDI_APPLICATION));
			edit_brush = CreateSolidBrush(RGB(255, 255, 0));
			SetDlgItemText(hDlg, IDC_EDIT1, "ABCD");
			SetDlgItemText(hDlg, IDC_EDIT2, "ABCDE");
		}
		return TRUE;
	// Dialog 자신이 보내는 메시지를 통해 Dialog 의 색상 변경 
	case WM_CTLCOLORDLG:
		return (BOOL)edit_brush;

	case WM_CTLCOLOREDIT:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor(hdc, RGB(255, 0, 0));
			SetBkColor(hdc, RGB(200, 200, 0));
		}
		return (BOOL)edit_brush;

	case WM_CTLCOLORSTATIC:
		{
			HWND hwnd = (HWND)lParam;
			HDC  hdc  = (HDC)wParam;

			UINT id   = GetDlgCtrlID(hwnd);
			if( id == IDC_EDIT2)
			{
				SetTextColor(hdc, RGB(0, 0, 255));
				SetBkColor(hdc, RGB(200, 200, 200));
			}
			else if( id == IDC_STATIC_A)
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				SetBkColor(hdc, RGB(200, 200, 0));
			}
		}
		return (BOOL)edit_brush;

	case WM_NCPAINT:

		// 먼저 Def 로 전달해서 그리게 한다.--> 주로 Dialog가 아닌 일반 
		// 윈도우에서 처리
		DefWindowProc(hDlg, msg, wParam, lParam);

		// 더 그리고 싶은 부분 처리
		{
			RECT  r = { 100, 5, 115, 20 };
			HDC hdc = GetWindowDC(hDlg);
			DrawFrameControl(hdc, &r, DFC_CAPTION, DFCS_BUTTONPUSH);
			ReleaseDC(hDlg, hdc);
		}
		return TRUE;



	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDOK:	EndDialog(hDlg, IDOK);			return 0;
		case IDCANCEL: EndDialog(hDlg, IDCANCEL);	return 0;
		}
		return TRUE;

	case WM_DESTROY:
		return 0;
	}
//	return DefWindowProc(hDlg, msg, wParam, lParam);
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE , PSTR lpCmdLine, int nShowCmd)
{
				
	UINT	ret = DialogBox(hInst,					// instance
					MAKEINTRESOURCE(IDD_DIALOG2),	// 다이얼로그 선택
					0,								// 부모 윈도우
					DlgProc);						// Proc..
	return 0;
}
























