//ui.cpp
#include "std.h"

HWND g_hcombo1, g_hcombo2, g_hbtnlogin, g_hbtnsend;
HWND g_heditname,g_heditsend,g_heditview;

void ui_init(HWND hDlg)
{
	//================= 컨트롤 핸들값 얻기 =============
	g_hcombo1	= GetDlgItem(hDlg, IDC_CB_SELECT1);
	g_hcombo2	= GetDlgItem(hDlg, IDC_CB_SELECT2);
	g_hbtnlogin = GetDlgItem(hDlg, IDC_BTN_LOGIN);
	g_hbtnsend	= GetDlgItem(hDlg, IDC_BTN_SEND);
	g_heditname = GetDlgItem(hDlg,IDC_EDIT_NAME);
	g_heditsend = GetDlgItem(hDlg,IDC_EDIT_SENDDATA);
	g_heditview	= GetDlgItem(hDlg,IDC_EDIT_VIEW);
	//===================================================

	//================== combobox 데이터 초기화 =========
	SendMessage(g_hcombo1, CB_ADDSTRING, 0, (LPARAM)"전체");
	SendMessage(g_hcombo1, CB_ADDSTRING, 0, (LPARAM)"1조");
	SendMessage(g_hcombo1, CB_ADDSTRING, 0, (LPARAM)"2조");
	SendMessage(g_hcombo1, CB_ADDSTRING, 0, (LPARAM)"3조");
	SendMessage(g_hcombo2, CB_ADDSTRING, 0, (LPARAM)"전체");
	SendMessage(g_hcombo2, CB_ADDSTRING, 0, (LPARAM)"1조");
	SendMessage(g_hcombo2, CB_ADDSTRING, 0, (LPARAM)"2조");
	SendMessage(g_hcombo2, CB_ADDSTRING, 0, (LPARAM)"3조");

	SendMessage(g_hcombo1, CB_SETCURSEL, 0, 0);
	SendMessage(g_hcombo2, CB_SETCURSEL, 0, 0);
	//====================================================
}

void ui_EnableButton(HWND hDlg, BOOL flogin, BOOL fsend)
{
	EnableWindow(g_hbtnlogin, flogin);
	EnableWindow(g_hbtnsend, fsend);
}

void ui_GetName(char* name)
{
	GetWindowText(g_heditname,name,20);
	
}
int ui_GetGroup()
{
	return SendMessage(g_hcombo1,CB_GETCURSEL,0,0);
}

void ui_SetName(char* name)
{
	SetWindowText(g_heditname,name);
}
void ui_SetGroup(int group)
{
	SendMessage(g_hcombo1,CB_SETCURSEL,group,0);
}
int ui_GetSendGroup()
{
	return SendMessage(g_hcombo2,CB_GETCURSEL,0,0);
}
void ui_GetSendData(char* senddata)
{
	GetWindowText(g_heditsend,senddata,50);
}
void ui_SetSendData(char* name)
{
		SetWindowText(g_heditsend,name);
}