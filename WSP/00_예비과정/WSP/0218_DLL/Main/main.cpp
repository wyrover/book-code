//main.cpp

#include "std.h"

BOOL CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:	return OnInitDialog(hDlg, wParam, lParam);
	case WM_COMMAND:	return OnCommand(hDlg, wParam, lParam);
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE , PSTR lpCmdLine, int nShowCmd)
{
				
	UINT	ret = DialogBox(hInst,					// instance
					MAKEINTRESOURCE(IDD_MAINDLG),	// 다이얼로그 선택
					0,								// 부모 윈도우
					DlgProc);						// Proc..
	return 0;
}





