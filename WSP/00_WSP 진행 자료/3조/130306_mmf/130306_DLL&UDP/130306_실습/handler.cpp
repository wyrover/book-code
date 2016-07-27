//handler.cpp
#include "std.h"

HWND g_hDlg;

int g_group;    //0:전제 1:1조 ...
char g_name[20];
int g_groupsend;
TCHAR message[50];
PACKET pack;


int OnInitDialog(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	g_hDlg = hDlg;
	ui_sendthread();
	ui_init(hDlg);
	ui_EnableButton(hDlg, TRUE, FALSE);
	return TRUE;
}

int OnNotify(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

int OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	switch (LOWORD(wParam)) {
	case IDC_BTN_LOGIN:
		OnLogin(hDlg);					break;
	case IDC_BTN_SEND:
		OnBtnSend(hDlg);				break;
	case ID_LOGOUT:
		return OnLogout(hDlg);
	case IDOK:
		EndDialog(hDlg,IDOK);
		return TRUE;
	case ID_EXIT:
	case IDCANCEL:
		OnExit(hDlg);
		return TRUE;
	}
	return FALSE;;
}


int OnLogout(HWND hDlg)
{
	ui_SetName(hDlg);
	ui_SetGroup(hDlg);
	ui_SetGroupsend(hDlg);
	ui_EnableButton(hDlg, TRUE, FALSE);
	client_exit();
	return TRUE;
}

void OnLogin(HWND hDlg)
{
	ui_getname(hDlg);
	g_group = ui_getgroup(hDlg);

	
	strcpy(pack.s_name,g_name); 
	ui_EnableButton(hDlg, FALSE, TRUE);
	client_init();
}
void OnBtnSend(HWND hDlg)
{	
	ui_getmessage(hDlg);
	g_groupsend = ui_getgroupsend(hDlg);


	ui_Setmessage(hDlg);
	pack.flag = g_groupsend;
	strcpy(pack.s_message,message); 
	SYSTEMTIME st;
	GetLocalTime(&st);
	wsprintf(pack.s_time,TEXT("%d:%d:%d"),
			st.wHour,st.wMinute,st.wSecond);

	client_sendpack((char*)&pack);

	//ui_SetGroupsend(hDlg);
}

void OnExit(HWND hDlg)
{
	//소켓 종료 처리 

	//프로그램 종료
	EndDialog(hDlg, IDCANCEL);
}

