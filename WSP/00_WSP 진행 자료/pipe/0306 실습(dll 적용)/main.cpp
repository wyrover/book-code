//main.cpp
#include "std.h"

CRITICAL_SECTION cs; 

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
	InitializeCriticalSection(&cs);
/*
	AllocConsole(); // console 창 생성

	freopen( "CONOUT$",   // console 을
			 "wt",        // text write 모드로
			 stdout);	  // stdout 과 연결..*/
	UINT ret = DialogBox(hInst,MAKEINTRESOURCE(IDD_MAINDLG),	
					0, DlgProc);				
	DeleteCriticalSection(&cs);
	return 0;
}
























