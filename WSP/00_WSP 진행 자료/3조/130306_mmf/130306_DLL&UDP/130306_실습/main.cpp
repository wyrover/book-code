//main.cpp
#include"std.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT imessage,WPARAM wParam,LPARAM lParam)
{
	switch (imessage) 
	{
	case WM_INITDIALOG:		return OnInitDialog(hDlg,wParam,lParam);
	case WM_COMMAND:		return OnCommand(hDlg,wParam,lParam);
	case WM_NOTIFY:			return OnNotify(hDlg,wParam,lParam);
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE , PSTR lpCmdLine, int nShowCmd)
{
	
#ifndef MY_DEBUG
	AllocConsole(); // console 창 생성

	freopen( "CONOUT$",   // console 을
			 "wt",        // text write 모드로
			 stdout);	  // stdout 과 연결..
#endif

	UINT ret = DialogBox(hInstance,MAKEINTRESOURCE(IDD_MAINDLG),NULL,MainDlgProc);
	return 0;
}