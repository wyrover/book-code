//handler.cpp
#include "std.h"

char g_name[20];
int  g_group;		// 0 : 전체, 1 : 1조....

DWORD OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// 콤보박스 데이터 초기화
	ui_init(hDlg);	

	// 로그인 버튼 활성화, 전송 버튼 비활성화
	ui_EnableButton(hDlg, TRUE, FALSE);

	// 소켓 초기화 <=========================
	if( mysock_init() == -1)
		MessageBox(hDlg, "소켓 초기화 오류", "에러", MB_OK);

	mysock_inirproc(OnRecv);
	return TRUE;
}

DWORD OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD( wParam ) )
	{
	case IDC_BTN_LOGIN: OnLogin(hDlg);	break;
	case IDC_BTN_SEND:	OnSend(hDlg);	break;
	case ID_LOGOUT:		OnLogOut(hDlg);	break;

	case ID_EXIT:
	case IDCANCEL: 		OnExit(hDlg);	break;
	}
	return TRUE;
}



//로그인 버튼 핸들러 
void OnLogin(HWND hDlg)
{
	//1. 조, 이름 정보를 전역변수에 저장
	char name[20];		ui_GetName(name);
	strcpy(g_name, name);
	g_group		= ui_GetGroup();

#ifndef MY_DEBUG
	printf("%s : %d\n", g_name, g_group);
#endif
	
	//2. 소켓  2개의 소켓(send, recv)
	mysock_recvsock(sizeof(PACKET), RECV_PORT);
	mysock_sendsock(sizeof(PACKET), RECV_PORT);


	//3. 로그인 버튼 비활성화, 전송 버튼 활성화
	ui_EnableButton(hDlg, FALSE, TRUE);
}


void OnLogOut(HWND hDlg)
{
	//1. 전역변수의 데이터 정보 초기화
	strcpy(g_name, "");		g_group = 0;
	ui_SetName(g_name);		ui_SetGroup(g_group);
#ifndef MY_DEBUG
	printf("%s : %d\n", g_name, g_group);
#endif

	//2. 소켓 종료


	//3. 로그인 버튼 활성화, 전송 버튼 비활성화
	ui_EnableButton(hDlg, TRUE, FALSE);
}



// 전송버튼 핸들러
void OnSend(HWND hDlg)
{
	//1. 입력 정보 획득(조, 전송문자열)
	int idx = ui_GetSendGroup();
	char senddata[50];
	ui_GetSendData(senddata);
#ifndef MY_DEBUG
	printf("%s : %d\n", senddata, idx);
#endif

	//2. 패킷 생성
	PACKET pack = {0};
	pack.flag	= idx;
	SYSTEMTIME st;
	GetLocalTime(&st);
	char buf[20];
	wsprintf(buf, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	strcpy(pack.s_time, buf);
	strcpy(pack.s_name, g_name);
	strcpy(pack.s_message, senddata);

	//3. 소켓 전송
	mysock_senddata((char*)&pack, sizeof(pack));
	

	//4. 문자열 입력창 초기화
	ui_SetSendData("");	
}


//종료 처리 핸들러 : 메뉴(종료)
void OnExit(HWND hDlg)
{
	//소켓 종료 처리 
	mysock_exit();		//<=====================

	//프로그램 종료
	EndDialog(hDlg, IDCANCEL);
}

//데이터 수신
void OnRecv(char* data, int length)
{
	PACKET *pack = (PACKET*)data;
	if(g_group != pack->flag)
		return;

	//내가 처리해야 할 데이터
	char buf[256];
	wsprintf(buf, "[%s/%s]%s\r\n", pack->s_name, pack->s_time,pack->s_message);
#ifndef MY_DEBUG
	printf("수신데이터 ==> %s", buf);
#endif

	ui_DisplayText(buf);	
}