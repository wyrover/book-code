#include <windows.h>
#include <winsock.h>
#include <process.h>
#include "resource.h"

#define CLIENTNUM	50
#define PORT_NUMBER 25000
#define DATALEN		1024
#define SERVER_ADDR "220.68.226.99"

typedef struct _data
{
	int type;	// 0: 왼쪽클릭(사각형), 1: 오른쪽 클릭(원)
	POINTS pt;
}DATA;

BOOL CALLBACK HandleDialog(HWND, UINT, WPARAM, LPARAM) ;

HWND hWnd;
SOCKET sClient;
HANDLE hThreadArr[CLIENTNUM];
DATA g_data = {100};
char strMsg[1024] = "클라이언트가 시작됨";
int clientNum = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	MSG msg;
	if((hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_CLIENT),
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
	client_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

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
	DATA rdata;
	RECT rect;
	int recv_len;
	while( 1 )
	{
		recv_len = recv(sClient, (char*)&rdata, sizeof(DATA), 0);
		if(recv_len == SOCKET_ERROR || recv_len == 0)
		{
			MessageBox(hWnd,"recv()실패 : ",NULL,MB_OK);
			return ;
		}
		else if(rdata.type == 2)
		{
			DestroyWindow(hWnd);			
		}
		else
		{
			g_data = rdata;
			SetRect(&rect,rdata.pt.x,rdata.pt.y,rdata.pt.x+30,rdata.pt.y+30);
			InvalidateRect(hWnd, &rect, FALSE);
		}
	}
}
BOOL CALLBACK HandleDialog(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	DATA data;
	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		if( !InitSocket() )
			return TRUE;
		hThread = (HANDLE)_beginthread(ClientThread, NULL, NULL);
		return TRUE;
	case WM_LBUTTONDOWN:
		data.type = 0;
		data.pt = MAKEPOINTS(lParam);
		
		send(sClient, (char*)&data, sizeof(DATA), 0);
		return TRUE;
	case WM_RBUTTONDOWN:
		data.type = 1;
		data.pt = MAKEPOINTS(lParam);
		send(sClient, (char*)&data, sizeof(DATA), 0);
		return TRUE;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if(g_data.type == 0)
			Rectangle(hdc, g_data.pt.x, g_data.pt.y, g_data.pt.x+30, g_data.pt.y+30);
		else if(g_data.type==1)
			Ellipse(hdc, g_data.pt.x, g_data.pt.y, g_data.pt.x+30, g_data.pt.y+30);
		EndPaint(hwnd, &ps);
		return TRUE;
	case WM_COMMAND:
		switch(wParam) 
		{
		case IDC_EXIT:
			data.type = 2;
			send(sClient, (char*)&data, sizeof(DATA), 0);
			DestroyWindow(hwnd);
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