#include <windows.h>
#include <stdlib.h>
#include <Winsock.h>
#include "resource.h"

#define WM_ASYNC (WM_USER +1)
#define PORT_NUMBER 25000
#define SERVER_ADDR "127.0.0.1"

BOOL CALLBACK HandleDialog(HWND, UINT, WPARAM, LPARAM);
BOOL InitSocket(HWND hWnd);

WNDPROC OldProc;
char msg[1024];
SOCKET sClient;
SOCKADDR_IN m_addr;
IN_ADDR m_in;
HWND hParent;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	if((hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_CLIENT),
		NULL, HandleDialog))==NULL){
		MessageBox(hWnd,"Initialize Error", "확인",MB_OK);
		return FALSE;
	}
	hParent = hWnd;
	ShowWindow(hWnd, nCmdShow);
	while(GetMessage(&msg,NULL,0,0)){
		if(!IsDialogMessage(hWnd,&msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}
BOOL InitSocket(HWND hWnd) 
{
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nError;

	nError = WSAStartup(wVersionRequested, &wsaData);
	if(nError != 0 ) {
		MessageBox(hWnd,"윈속 초기화 에러","확인",MB_OK);
		return FALSE;
	}
	sClient = socket(AF_INET,SOCK_STREAM, 0);
	if(sClient == INVALID_SOCKET)
	{
		MessageBox(hWnd,"소켓생성 에러", "확인",MB_OK);
		return FALSE;
	} 
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(PORT_NUMBER);
	m_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	if(WSAAsyncSelect(sClient, hWnd, WM_ASYNC, FD_CONNECT)==SOCKET_ERROR)
	{
		MessageBox(hWnd,"연결 에러","확인",MB_OK);
		return FALSE;
	}
	if(connect(sClient, (LPSOCKADDR)&m_addr, sizeof(m_addr)) == SOCKET_ERROR) {
		if(WSAGetLastError() != WSAEWOULDBLOCK) {
			MessageBox(hWnd,"연결 에러","확인",MB_OK);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CALLBACK HandleDialog(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	static HWND hListBox;
	char msgInfo[60];
	int recv_len;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		hListBox = GetDlgItem(hWnd,IDC_CLIENT);
		memset(msg,'\0',sizeof(msg));
		if(!InitSocket(hWnd))
			return FALSE;
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_COMMAND:
		switch(wParam) 
		{
		case IDC_SEND:
			GetDlgItemText(hWnd,IDC_EDIT,msgInfo,256);
			send(sClient, msgInfo, strlen(msgInfo)+1, 0);
			SendMessage(hListBox,LB_INSERTSTRING, 0 , (LPARAM) msgInfo);
			return TRUE;
		case IDC_EXIT:
			closesocket(sClient);
			sClient = INVALID_SOCKET;
			WSACleanup();
			DestroyWindow(hWnd);
			return TRUE;
		}
		break;
	case WM_ASYNC:
		hListBox = GetDlgItem(hWnd, IDC_CLIENT);
		switch((WORD)(lParam))
		{
		case FD_CONNECT:
			if(WSAAsyncSelect(sClient,hWnd,WM_ASYNC, FD_READ|FD_WRITE|FD_CLOSE)==SOCKET_ERROR) 
			{
					wsprintf(msgInfo,"Async Select를 할수 없음");
					SendMessage(hListBox, LB_INSERTSTRING,0, (LPARAM) msgInfo);
					return FALSE;
			}
			break;
		case FD_CLOSE:
			closesocket(sClient);
			sClient = INVALID_SOCKET;
			wsprintf(msgInfo,"서버와 연결이 끊어졌습니다.");
			SendMessage(hListBox, LB_INSERTSTRING, 0 , (LPARAM)msgInfo);
			WSACleanup();
			break;

		case FD_READ:
			recv_len = recv(sClient,msg,sizeof(msg),0);
			if( recv_len == 0)
				return FALSE;
			SendMessage(hListBox,LB_INSERTSTRING,0,(LPARAM)msg);

			break;
		case FD_WRITE:
			wsprintf(msgInfo,"연결되었습니다.");
			SendMessage(hListBox, LB_INSERTSTRING,0, (LPARAM)msgInfo);
			wsprintf(msg, "connect");
			send(sClient, msg, sizeof(msg), 0 );
			//////////////////////////////////
			break;
		default :
			break;
		}
		return FALSE;
	}
	return FALSE;
}
