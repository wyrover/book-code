//handler.cpp
#include "std.h"

struct DATA
{
	char message[30];
	char nickname[20];
	int  h, m, s;
};

HWND hSendButton;
HWND hListBox;
HWND hServer;
//접속 버튼 클릭 
void OnConnect(HWND hDlg)
{
	// 정보 획득 
	char nickname[20];
	GetDlgItemText(hDlg, IDC_EDIT1, nickname, 20);

	// 서버 탐색 
	hServer = FindWindow(NULL, "SERVER");
	if ( hServer == 0 )
	{
		MessageBox(hDlg, "접속실패", "에러", MB_OK);
		return;
	}
	
	//데이터 보내기
	COPYDATASTRUCT cds;
	cds.cbData = strlen(nickname)+1;// 전달한 data 크기
	cds.dwData = 1;					// 1 : 넥네임(접속) 
	cds.lpData = nickname;			// 전달할 Pointer

	SendMessage( hServer, WM_COPYDATA, 0,  (LPARAM)&cds);

	//ui처리
	EnableWindow(hSendButton, TRUE);
}

void OnSend(HWND hDlg)
{
	//1
	char msg[30];
	char msg1[20];
	GetDlgItemText(hDlg, IDC_EDIT2, msg, sizeof(msg));
	SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)msg);

	//2
	SYSTEMTIME mt;
	GetLocalTime(&mt);
	GetDlgItemText(hDlg, IDC_EDIT1, msg1, sizeof(msg1));
	DATA d;
	strcpy(d.message, msg);
	strcpy(d.nickname, msg1);
	d.h = mt.wHour;  d.m =  mt.wMinute; d.s =  mt.wSecond;

	//데이터 보내기
	COPYDATASTRUCT cds;
	cds.cbData = sizeof(DATA);// 전달한 data 크기
	cds.dwData = 2;					// 2 : 데이터 전송
	cds.lpData = &d;			// 전달할 Pointer

	SendMessage( hServer, WM_COPYDATA, 0,  (LPARAM)&cds);
	
}

int OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	//1. 핸들값 획득
	hSendButton = GetDlgItem(hDlg, IDC_BUTTON2);
	hListBox	= GetDlgItem(hDlg, IDC_LIST1);

	//ui처리
	EnableWindow(hSendButton, FALSE);
	return true;
}

int OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD( wParam ) )
	{
	case IDC_BUTTON1: OnConnect(hDlg);			break;
	case IDC_BUTTON2: OnSend(hDlg);				break;
		
	case IDCANCEL: EndDialog(hDlg, IDCANCEL);	return 0;
	}
	return true;
}