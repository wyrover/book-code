//NewProcessDlg.cpp


#include "std.h"


void GetFileName( HWND hwnd, char *s, int bufsize)
{
	char temp[MAX_PATH];
	strcpy( temp, s);
	OPENFILENAME ofn	= { 0 };
	ofn.lStructSize		= sizeof( ofn);
	ofn.hwndOwner		= hwnd;
	ofn.lpstrFile		= temp;
	ofn.nMaxFile		= MAX_PATH;
	ofn.Flags			= OFN_HIDEREADONLY;
	ofn.lpstrFilter		= "Executable File(*.exe)\0*.exe\0\0";
	if( GetOpenFileName( &ofn) == IDOK)
	{
		strcpy( s, temp);
	}
}


BOOL CALLBACK NewProcessProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND		hCombo;
	switch(msg)
	{
	// Dialog가 처음 나타날때 발생. 각 콘트롤을 초기화 한다.
	case WM_INITDIALOG:
		{
			hCombo	= GetDlgItem(hDlg, IDC_CBPROCESSNAME);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK: 
			{
				char name[256];
				GetDlgItemText(hDlg, IDC_CBPROCESSNAME, name,  256);
				//--------------------------------------------------
				PROCESS_INFORMATION pi;
				STARTUPINFO			si = { sizeof(si) };	

				// Process 생성 함수 .
				BOOL b = CreateProcess(0, name, 0, 0, 
							FALSE, NORMAL_PRIORITY_CLASS, 0, 0, 
							&si, &pi);
				EndDialog(hDlg, IDOK); 
			}
			return TRUE;

		case ID_BTN_SEARCH:
			{
				// 내가 가져올 실행파일의 파일명을 얻기..(디렉토리 포함)
				char name[256] = { 0 };
				GetFileName( hDlg, name, 256);
				SetDlgItemText(hDlg, IDC_CBPROCESSNAME, name);
			}
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
	}
	return FALSE; 
}

