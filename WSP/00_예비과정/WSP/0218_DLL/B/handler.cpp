//handler.cpp
#include "std.h"

HWND g_hCombo;

HMODULE	g_Dll;
typedef int(*FUNC)(int, int);
FUNC Add;
FUNC Sub;
FUNC Mul;
FUNC Div;
void OnCombo(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch(HIWORD(wParam))
	{
	case CBN_SELCHANGE:
		{
			int idx = SendMessage(g_hCombo, CB_GETCURSEL, 0, 0);
			int num1, num2, result;
			num1=GetDlgItemInt(hDlg, IDC_EDIT1,0, 0);
			num2=GetDlgItemInt(hDlg, IDC_EDIT2, 0, 0);
			switch(idx)
			{
				case 1:	result	= Add(num1, num2); break;
				case 2: result	= Sub(num1, num2); break;
				case 3: result	= Mul(num1, num2); break;
				case 4: result	= Div(num1, num2); break;
			}
			SetDlgItemInt(hDlg, IDC_EDIT3, result, FALSE);
		}
		break;
		
	}
}


int OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	g_Dll    = LoadLibrary("cal.dll");
	Add  	 = (FUNC)GetProcAddress( g_Dll, "Add");	
	Sub  	 = (FUNC)GetProcAddress( g_Dll, "Sub");	
	Mul  	 = (FUNC)GetProcAddress( g_Dll, "Add");	
	Div  	 = (FUNC)GetProcAddress( g_Dll, "Div");	

	g_hCombo = GetDlgItem(hDlg, IDC_COMBO1);
	SendMessage(g_hCombo, CB_ADDSTRING, 0, (LPARAM)"º±≈√");
	SendMessage(g_hCombo, CB_ADDSTRING, 0, (LPARAM)"+");
	SendMessage(g_hCombo, CB_ADDSTRING, 0, (LPARAM)"-");
	SendMessage(g_hCombo, CB_ADDSTRING, 0, (LPARAM)"*");
	SendMessage(g_hCombo, CB_ADDSTRING, 0, (LPARAM)"/");
	SendMessage(g_hCombo, CB_SETCURSEL, 0, 0);
	return true;
}

int OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD( wParam ) )
	{
	case IDC_COMBO1: OnCombo(hDlg, wParam, lParam);	break;
		
	case IDCANCEL: EndDialog(hDlg, IDCANCEL);
					FreeLibrary(g_Dll);    return 0;
	}
	return true;
}