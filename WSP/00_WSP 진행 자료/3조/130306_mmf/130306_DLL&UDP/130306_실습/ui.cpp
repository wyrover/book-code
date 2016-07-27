//ui.cpp
#include "std.h"

void ui_init(HWND hDlg)
{
	TCHAR *Items[]={TEXT("전제"),TEXT("1"),TEXT("2"),
	TEXT("3")};

	int i;
	HWND hCombo = GetDlgItem(hDlg,IDC_CB_SELECT1);
	for (i=0;i<4;i++) {
			SendMessage(hCombo,CB_ADDSTRING,0,(LPARAM)Items[i]);
		}
	SendMessage(hCombo,CB_SETCURSEL,0,0);
	hCombo = GetDlgItem(hDlg,IDC_CB_SELECT2);
	for (i=0;i<4;i++) {
			SendMessage(hCombo,CB_ADDSTRING,0,(LPARAM)Items[i]);
		}
	SendMessage(hCombo,CB_SETCURSEL,0,0);
}

void ui_EnableButton(HWND hDlg, BOOL flogin, BOOL fsend)
{
	HWND g_hbtnlogin = GetDlgItem(hDlg,IDC_BTN_LOGIN);
	EnableWindow(g_hbtnlogin, flogin);
	HWND g_hbtnsend = GetDlgItem(hDlg,IDC_BTN_SEND);
	EnableWindow(g_hbtnsend, fsend);
}

void ui_getname(HWND hDlg)
{
	GetDlgItemText(hDlg,IDC_EDIT_NAME,g_name,20);
}

int ui_getgroup(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg,IDC_CB_SELECT1);
	return SendMessage(hCombo, CB_GETCURSEL,0,0);
}

void ui_SetName(HWND hDlg)
{
	strcpy(g_name,"");
	SetDlgItemText(hDlg,IDC_EDIT_NAME,"");
	
}
void ui_SetGroup(HWND hDlg)
{
	g_group = 0;
	HWND hCombo = GetDlgItem(hDlg,IDC_CB_SELECT1);
	SendMessage(hCombo,CB_SETCURSEL,0,0);

}
void ui_getmessage(HWND hDlg)
{
	GetDlgItemText(hDlg,IDC_EDIT_SENDDATA,message,50);
}

void ui_Setmessage(HWND hDlg)
{
	SetDlgItemText(hDlg,IDC_EDIT_SENDDATA,"");
}

void ui_SetGroupsend(HWND hDlg)
{
	HWND hCombo = GetDlgItem(hDlg,IDC_CB_SELECT2);
	SendMessage(hCombo,CB_SETCURSEL,0,0);
}

int ui_getgroupsend(HWND hDlg)
{
	//TCHAR str[128];
	HWND hCombo = GetDlgItem(hDlg,IDC_CB_SELECT2);
	return SendMessage(hCombo, CB_GETCURSEL,0,0);
	//SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)str);
	//SetWindowText(hDlg, str);}
}

void ui_sendthread()
{
	DWORD ThreadID;
	CloseHandle(CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID));
}

DWORD WINAPI ThreadFunc(LPVOID temp)
{
	//server_run();
	HANDLE hEvent = OpenEvent( EVENT_ALL_ACCESS  , FALSE, "MYMESSAGE");
#ifndef MY_DEBUG
	int i = GetLastError();
	printf("[%d]\n",i);
#endif
	HANDLE hMap   = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, "mymmf");
#ifndef MY_DEBUG
	int ii = GetLastError();
	printf("[%d]\n",ii);
#endif

	if ( hEvent == 0 )
	{
		MessageBox( 0, "error", "", MB_OK);
		return 0;
	}

	if ( hMap == 0 )
	{
		MessageBox( 0, "MMF_A를 먼저 실행하세요", "", MB_OK);
		return 0;
	}
	//-------------------------------------------------------------------
	PACKET* pData = (PACKET*)MapViewOfFile( hMap, FILE_MAP_READ, 0, 0, 0);

	while ( 1 )
	{
		// Event를 대기한다.
		WaitForSingleObject( hEvent, INFINITE );
		myrefun(pData,94,0);
	}
	UnmapViewOfFile( pData );
	CloseHandle( hMap );
	CloseHandle( hEvent );

	return 0;
}

void server_run()
{
	
/*	HANDLE hPipe = CreateNamedPipe( "\\\\.\\pipe\\source_server", // UNC
									PIPE_ACCESS_OUTBOUND, //출력 전용
									PIPE_TYPE_BYTE,		  // stream 방식
									5,		// 최대 instance 갯수
									4096, 4096,// 입출력 버퍼 크기
									1000,	// WaitNamedPipe()로 대기할 시간
									0);		// KO 보안
	if ( hPipe == INVALID_HANDLE_VALUE )
	{
		return;
	}
	
	while(1)
	{

		BOOL b = ConnectNamedPipe( hPipe, 0); // Client의 접속을 대기한다.
		// 클라이언트가 접속할때 까지 blocking
		
		if ( b == FALSE && GetLastError() == ERROR_PIPE_CONNECTED ) b = TRUE;
		
		if ( b ) // 접속 되었다면
		{
			// Pipe에 Write
			char *buf;
			buf = (char*)malloc(94);
			memset( buf, 0, 94);
			DWORD len;
			ReadFile( hPipe, buf, 94, &len, 0);
#ifndef MY_DEBUG
			if(buf == NULL)
				printf("...");
	printf("[%d]%s \r\n",*(int*)buf,((PACKET*)buf)->s_message);
#endif
			DisconnectNamedPipe( hPipe ); // 강제로 접속을 끝는다.
			myrefun(buf,94,0);
			free(buf);
		}
	}
	CloseHandle(hPipe);
*/
	
}

void myrefun(PACKET*p,int size,SOCKET sock)
{
		//int *flag = (int*)i;
		//PACKET *p = (PACKET*)i;
		if(p->flag  == 0)
		{
			DisplayText("[%s] --- %s : %s\r\n",p->s_time,p->s_name ,p->s_message   );
		}
		if(p->flag !=0 && p->flag == g_groupsend)
		{
			DisplayText("[%s] --- %s : %s\r\n",p->s_time,p->s_name ,p->s_message   );
		}
}

void DisplayText(char *fmt, ...){
	va_list arg;
	va_start(arg, fmt);
	char cbuf[256];
	vsprintf(cbuf, fmt, arg);
	//EnterCriticalSection(&cs);
	
	HWND hEdit = GetDlgItem(g_hDlg,IDC_EDIT_VIEW);

	int nLength = GetWindowTextLength(hEdit);

	SendMessage(hEdit, EM_SETSEL, nLength, nLength);
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
	//LeaveCriticalSection(&cs);
	
	va_end(arg);
}