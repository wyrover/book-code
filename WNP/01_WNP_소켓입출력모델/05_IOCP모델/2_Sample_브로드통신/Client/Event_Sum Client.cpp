#include <windows.h>
#include <winsock.h>
#include <process.h>
#include "resource.h"

#define CLIENTNUM	50
#define PORT_NUMBER 25000
#define DATALEN		1024
#define SERVER "127.0.0.1"

BOOL CALLBACK hDlg(HWND, UINT, WPARAM, LPARAM);

HWND hWnd;
SOCKET sClient;
HANDLE hThreadArr[CLIENTNUM];
char strMsg[1024];
int clientNum = 0;

struct DATA
{
	int type;
	char str[100];
	int num1;
	int num2;
};

DATA data;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	MSG msg;
	if((hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_CLIENT),
		NULL, hDlg))==NULL){
		MessageBox(NULL, "초기화 에러","확인",MB_OK);
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	while(GetMessage(&msg,NULL,0,0)){
		if(!IsDialogMessage(hWnd,&msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
BOOL InitSocket( )
{
	WORD wVer = MAKEWORD(1,1);
	WSADATA wsaData;
	SOCKADDR_IN client_addr;

	if(WSAStartup(wVer,&wsaData) != 0 )
	{
		MessageBox(hWnd, "WSAStartup() 실패 ", NULL, MB_OK);
		return FALSE;
	}
	sClient = socket(AF_INET,SOCK_STREAM, 0);
	if(sClient == INVALID_SOCKET){
		MessageBox(hWnd, "socket() 실패",NULL, MB_OK);
		return FALSE;
	}
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT_NUMBER);
	client_addr.sin_addr.s_addr = inet_addr(SERVER);

	if( connect(sClient, (LPSOCKADDR)&client_addr, sizeof(client_addr)) == SOCKET_ERROR) 
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK ) {
			MessageBox(hWnd, "connect( ) 실패", NULL, MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}
void ClientThread(LPVOID vp)
{
	HWND hBox;
	//hBox = GetDlgItem(hWnd,IDC_LIST1);
	int recv_len;

	while( 1 )
	{
		recv_len = recv(sClient, (char*)&data, sizeof(data), 0);
		hBox = GetDlgItem(hWnd,IDC_LIST1);
		if(recv_len == SOCKET_ERROR || recv_len == 0)
		{
			MessageBox(hWnd,"recv()실패 : ",NULL,MB_OK);
			return ;
		}
		else if(data.type == 2)
		{
			wsprintf(strMsg,"클라이언트 종료");
			SendMessage(hBox,LB_INSERTSTRING, 0 , (LPARAM) strMsg);
			closesocket(sClient);
			return;
		}		
		else
		{
			if(data.type == 0)
				SendMessage(hBox, LB_INSERTSTRING, 0 , (LPARAM)data.str);
			else if(data.type == 1)
			{
				wsprintf(strMsg, "%d", data.num1 + data.num2);
				SendMessage(hBox, LB_INSERTSTRING, 0 , (LPARAM)strMsg);
			}
		}
	}
}

BOOL CALLBACK hDlg(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	static HWND hListBox;
	static HANDLE hThread;
	static int head;
		
	switch(iMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hWnd,IDC_EDIT1,strMsg);
		if( !InitSocket() )
			return TRUE;
		hThread = (HANDLE)_beginthread(ClientThread, 0, NULL);

		hListBox = GetDlgItem(hWnd,IDC_LIST1);
		SendMessage(hListBox, LB_INSERTSTRING, 0 , (LPARAM) strMsg);
		return TRUE;
	case WM_COMMAND:
		if(HIWORD(wParam) == BN_CLICKED)
		{
			switch(LOWORD(wParam))
			{
			case IDC_STRING :
				head = 0;
				return 0;
			case IDC_SUM :
				head = 1;
				return 0;
			}
		}
		switch(wParam) 
		{
		case IDC_SEND:
			GetDlgItemText(hWnd, IDC_EDIT1, strMsg, 256);
			
			if(head == 0)
			{
				data.type = 0;
				strcpy(data.str, strMsg);

				send(sClient, (char*)&data , sizeof(data), 0);
			}
			else if(head == 1)
			{
				int i = 0;
				char temp[100];

				data.type = 1;
								
				while(1)
				{
					if(strMsg[i] == '+')
						break;
					i++;
				}

				memcpy(temp, strMsg, i);
				data.num1 = atoi(temp);
						
				strcpy(temp, &strMsg[i+1]);
				data.num2 = atoi(temp);
				
				send(sClient, (char*)&data, sizeof(data), 0);
			}
			break;
		case IDC_END:
			DestroyWindow(hWnd);
		}
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	default :
		break;
	}
	return 0L;
}