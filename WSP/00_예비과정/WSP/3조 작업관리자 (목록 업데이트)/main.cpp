//main.cpp
#include "std.h"

BOOL CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam,LPARAM lParam)
{
	
	switch( msg )
	{
	case WM_INITDIALOG: return OnInitDialog(hDlg);
	case WM_COMMAND: return OnCommand(hDlg,wParam);
	case WM_NOTIFY:  return OnNotify(hDlg,lParam);
	}
	return FALSE;
	
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, 
						PSTR lpCmdLine, int nShowCmd)
{
	UINT ret = DialogBox(hInst, 
		MAKEINTRESOURCE(IDD_MAINDLG), 0, DlgProc);
	return 0;
}
