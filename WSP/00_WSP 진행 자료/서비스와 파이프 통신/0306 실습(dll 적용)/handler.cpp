//handler.cpp
#include "std.h"

char g_name[20];
int  g_group;			//0 : 전체, 1 : 1조.....

SOCKET sendsock;
extern HWND g_heditview;
HANDLE hPipe;

void recvfun(char* data,int size,SOCKET sock)
{
	int *flag=(int*)data;
	switch(*flag)
	{
	case 0:	
	case 1:
		{
			//char message[90];
			PACKET* pack=(PACKET*)data;
			//wsprintf(message,"%s: %s (%s)\r\n",pack->s_name,pack->s_message,pack->s_time);
			DisplayText("%s: %s (%s)\r\n",pack->s_name,pack->s_message,pack->s_time);
		}break;
	case 2:
	case 3: break;
	}
}
void DisplayText(char *fmt, ...){
	va_list arg;
	va_start(arg, fmt);
	char cbuf[256];
	vsprintf(cbuf, fmt, arg);
	EnterCriticalSection(&cs);
	int nLength = GetWindowTextLength(g_heditview);
	SendMessage(g_heditview, EM_SETSEL, nLength, nLength);
	SendMessage(g_heditview, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	LeaveCriticalSection(&cs);
	
	va_end(arg);
}
DWORD OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	// 콤보박스 데이터 초기화
	ui_init(hDlg);	

	// 로그인 버튼 활성화, 전송 버튼 비활성화
	ui_EnableButton(hDlg, TRUE, FALSE);

	mysock_initlibraty();

	mysock_init(recvfun);

	DWORD ThreadID;
	CloseHandle(CreateThread(NULL,0,MainThread,NULL,0,&ThreadID));

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
	char name[20];
	ui_GetName(name);
	strcpy(g_name,name);
	g_group=ui_GetGroup();

	//2. 소켓 생성 2개의 소켓(send, recv)
	sendsock = mysock_sendsock(PORT);
	mysock_recvsock(PORT);

	//3. 로그인 버튼 비활성화, 전송 버튼 활성화
	ui_EnableButton(hDlg, FALSE, TRUE);
}


void OnLogOut(HWND hDlg)
{
	//1. 전역변수의 데이터 정보 초기화
	strcpy(g_name,"");	g_group=0;
	ui_SetName(g_name);		ui_SetGroup(g_group);
	//2. 소켓 종료
	mysock_closesock(sendsock);
	//3. 로그인 버튼 활성화, 전송 버튼 비활성화
	ui_EnableButton(hDlg, TRUE, FALSE);
}


char senddata[50];
// 전송버튼 핸들러
void OnSend(HWND hDlg)
{
	//1. 입력 정보 획득(조, 전송문자열)
	int idx= ui_GetSendGroup();
	
	ui_GetSendData(senddata);

	//2. 패킷 생성
	PACKET pack;
	pack.flag=idx;
	strcpy(pack.s_name,g_name);
	SYSTEMTIME st;
	GetLocalTime(&st);
	wsprintf(pack.s_time,"%0d:%0d:%0d",st.wHour,st.wMinute,st.wSecond);
	strcpy(pack.s_message,senddata);
/*#ifndef MY_DEBUG
	printf("%s : %s\n",pack.s_name,pack.s_time);
#endif*/
	//3. 소켓 전송
	mysock_send(sendsock,(char*)&pack,sizeof(PACKET));
	//4. 문자열 입력창 초기화
	ui_SetSendData("");

	//=======================================================
	// 요청을 보낸 후 응답을 받는다.
	BOOL bSuc;
	DWORD dwRead, dwWritten;
	TCHAR szOutput[50];
	bSuc=WriteFile(hPipe,senddata,lstrlen(senddata)+1,&dwWritten,NULL);
	if ((bSuc==FALSE) || (dwWritten==0))
		return;
	
	ReadFile(hPipe,szOutput,50,&dwRead,NULL);

	MessageBox(0,szOutput,"",MB_OK);

	Sleep(500);

	// 파이프를 해제하고 결과를 메인 윈도우에 출력한다.
	
}

DWORD WINAPI MainThread(LPVOID temp)
{
	// 서버가 파이프를 생성할 때까지 무한 대기한다.
	for (;;) {
		if (WaitNamedPipe("\\\\.\\pipe\\MultiPipe",NMPWAIT_WAIT_FOREVER)==TRUE) {
			hPipe=CreateFile("\\\\.\\pipe\\MultiPipe",GENERIC_READ | GENERIC_WRITE,
				0,NULL,OPEN_EXISTING,0,NULL);
			if (hPipe!=INVALID_HANDLE_VALUE)
				break;
		}
	}
	
	return 0;
}

//종료 처리 핸들러 : 메뉴(종료)
void OnExit(HWND hDlg)
{
	//소켓 종료 처리 
	CloseHandle(hPipe);
	mysock_exitlibrary();
	mysock_closesock(sendsock);
	//프로그램 종료
	EndDialog(hDlg, IDCANCEL);
}
