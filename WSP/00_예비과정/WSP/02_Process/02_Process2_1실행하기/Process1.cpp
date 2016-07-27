// Process 실행하기..
#include <windows.h>
#include "resource.h"

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

void AddText( HWND hEdit, const char* s)
{
	int n		= SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
	int nStart  = SendMessage(hEdit, EM_LINEINDEX, n, 0);
	SendMessage(hEdit, EM_SETSEL, nStart, nStart+1);
	SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)s);
}

BOOL CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//-----------------------
	static HWND		hMsg;
	//----------------------
	switch( msg )
	{
	// 다이얼로그  초기화
	case WM_INITDIALOG:
		hMsg	= GetDlgItem(hDlg, IDC_EDIT_BOARD);
		return TRUE;

	// 컨트롤에 대한 이벤트..
	case WM_COMMAND:
		switch( LOWORD (wParam))
		{
		// 버튼 컨트롤의 핸들러.
		case IDC_BTN_EXCUTE:
			{
				char name[256];
				GetDlgItemText(hDlg, IDC_EDIT_EXCUTE, name,  256);
				//--------------------------------------------------
				PROCESS_INFORMATION pi;
				STARTUPINFO			si = { sizeof(si) };
				BOOL b = CreateProcess(0, name, 0, 0, 
							FALSE, NORMAL_PRIORITY_CLASS, 0, 0, 
							&si, &pi);
				if( b )
				{
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
				}
				MessageBox(NULL, "생성됬나","" ,MB_OK);
				TerminateProcess(pi.hProcess, 0);
			}
			break;

		// 버튼 컨트롤의 핸들러.
		case IDC_BTN_DIALOG:
			{
				// 내가 가져올 실행파일의 파일명을 얻기..(디렉토리 포함)
				char name[256] = { 0 };
				GetFileName( hDlg, name, 256);

				SetDlgItemText(hDlg, IDC_EDIT_EXCUTE, name);
				AddText( hMsg, name);

			}
			break;

		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		return TRUE;
	}
	return FALSE;
}


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nShowCmd)
{

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc);
	return 0;
}