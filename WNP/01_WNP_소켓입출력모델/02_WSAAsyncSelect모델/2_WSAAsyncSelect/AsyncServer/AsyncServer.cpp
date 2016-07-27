#include <windows.h>
#include <winsock.h>
#include "resource.h"

#define WM_ASYNC WM_USER+1
#define CHAT_PORT 25000
#define MAX_CLIENT 50

BOOL CALLBACK HandleDialog(HWND, UINT, WPARAM, LPARAM);
BOOL InitSocket(HWND);
HWND hWnd;
SOCKET sAccept;
SOCKET sClient[MAX_CLIENT];
char strMsg[80];
int clientNum;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	MSG msg;
	if((hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_SERVER),
		NULL, HandleDialog))==NULL){
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
int FindSock(SOCKET sock)
{
	int i;
	for( i = 0 ; i < clientNum ; i++)
		if( sock == sClient[i] )
			return i;

	return -1;
}
BOOL InitSocket(HWND hWnd )
{
	WORD wVer;
	WSADATA wsaData;
	SOCKADDR_IN serv_addr;

	wVer = MAKEWORD(1,1);
	if(WSAStartup(wVer,&wsaData) != 0 )
	{
		wsprintf(strMsg,"WSAStartup() 실패 : %d", WSAGetLastError());
		return FALSE;
	}

	sAccept = socket(AF_INET, SOCK_STREAM, 0 );
	if(sAccept==INVALID_SOCKET){
		wsprintf(strMsg,"socket() 실패: %d",WSAGetLastError());
		return FALSE;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port=htons(CHAT_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sAccept, (LPSOCKADDR)&serv_addr,sizeof(serv_addr)) ==
		SOCKET_ERROR) {
		wsprintf(strMsg,"bind() 실패 : %d",WSAGetLastError());
		return FALSE;
	}
	if(listen(sAccept, 5) == SOCKET_ERROR) {
		wsprintf(strMsg, "listen() 실패 : %d",WSAGetLastError());
		return FALSE;
	}
	if(WSAAsyncSelect(sAccept,hWnd,WM_ASYNC, FD_ACCEPT) == 
		SOCKET_ERROR)
	{
		wsprintf(strMsg, "WSAAsyncSelect() 실패 : %d",WSAGetLastError());
		return FALSE;
	}
	wsprintf(strMsg,"소켓 초기화 성공");
	return TRUE;
}

BOOL CALLBACK HandleDialog(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	static HWND hListBox;
	static char buf[512];
	char temp[512];
	int cli_len, recv_len, i;
	SOCKET sock_tmp;
	SOCKADDR_IN cli_addr;

	switch(iMsg)
	{
	case WM_INITDIALOG:
			hListBox = GetDlgItem(hWnd,IDC_SERVER);
			if( !InitSocket(hWnd ) )
				return FALSE;
			SendMessage(hListBox,LB_INSERTSTRING, 0 , 
				(LPARAM) strMsg);
			return TRUE;

	case WM_COMMAND:
		switch(wParam) 
		{
		case IDC_EXIT:
			DestroyWindow(hWnd);
			return TRUE;
		}
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_ASYNC:
		sock_tmp = LOWORD(wParam);
		if(sock_tmp == sAccept)
		{
			if(clientNum < MAX_CLIENT-1)
			{
				cli_len=sizeof(cli_addr);
				sClient[clientNum] = accept(sAccept,
					(LPSOCKADDR)&cli_addr,&cli_len);
				if(sClient[clientNum] == INVALID_SOCKET) 
				{
					wsprintf(strMsg, "%d  번 째 accept() 실패",clientNum);
				} else 
				{
					WSAAsyncSelect(sClient[clientNum],hWnd,
						WM_ASYNC, FD_READ|FD_WRITE|FD_CLOSE);
					wsprintf(strMsg,"%d번째 클라이언트 추가", clientNum );
					clientNum++;
				}
				SendMessage(hListBox, LB_INSERTSTRING,0, (LPARAM)strMsg);
			} 
		} else {
			switch(lParam) 
			{
			case FD_READ:
				recv_len = recv(sock_tmp, buf, 512, 0);
				if(recv_len == SOCKET_ERROR)
				{
					wsprintf(strMsg,"recv()실패 : %d", WSAGetLastError());
					SendMessage(hListBox,LB_INSERTSTRING,0,(LPARAM)strMsg);
				} else if(recv_len == 0 )
				{
					return FALSE;
				}
				else
				{
					if(strcmp(buf, "connect") == 0)
					{
						///////////////////////////
						break;
					}

					wsprintf(temp,"%d 번째:", FindSock( sock_tmp ));
					strcat(temp, buf);
					strcpy(buf, temp);
					PostMessage(hWnd, WM_ASYNC, sock_tmp, WSAMAKESELECTREPLY((FD_WRITE),0));
					SendMessage(hListBox,LB_INSERTSTRING,0,(LPARAM)buf);
				}
				break;
			case FD_WRITE:
				for(i = 0 ; i < clientNum ; i++)
				{
					send(sClient[i], buf, sizeof(buf), 0 );
				}
				break;
			case FD_CLOSE:
				for(i = 0 ; i < clientNum; i++)
				{
					if(sock_tmp == sClient[i])
					{
						closesocket(sClient[i]);
						if(i != (clientNum -1 )){
							sClient[i] = sClient[clientNum-1];
						}
						clientNum--;
						break;
					}
				}
				wsprintf(strMsg,"%d번째 클라이언트 종료", i);
				SendMessage(hListBox, LB_INSERTSTRING, 0 , (LPARAM)strMsg);
				if(clientNum == 0 )
					memset(buf, '\0',512);
				break;
			}
		}
		return FALSE;
	default :
		break;
	}
	return FALSE;
}



