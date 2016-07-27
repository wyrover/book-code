#define _WIN32_WINNT	0x500
#define WINVER		    0x500
#include <windows.h>
#include "resource.h"


#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")


BOOL CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH	edit_brush;
	switch (msg)
	{
	// 초기화... 
	case WM_INITDIALOG:
		{
		SendDlgItemMessage (hDlg, IDC_SLIDER, TBM_SETRANGEMAX, 0, 400);
        SendDlgItemMessage (hDlg, IDC_SLIDER, TBM_SETRANGEMIN, 0, 0);
		}
		return TRUE;

	case WM_HSCROLL:
		{
        int pos;
        pos=SendDlgItemMessage(hDlg,IDC_SLIDER,TBM_GETPOS,0,0);
		SetDlgItemInt(hDlg, IDC_EDIT2, pos, 0);
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
					MAKEINTRESOURCE(IDD_DIALOG1),	// 다이얼로그 선택
					0,								// 부모 윈도우
					DlgProc);						// Proc..
	return 0;
}
























