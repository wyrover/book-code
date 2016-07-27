#include <winsock2.h>
#include <windows.h>
#include <iostream.h>
#include <process.h>
#include "resource.h"

#define CLIENTNUM	50
#define PORT_NUMBER 25000
#define DATALEN		1024
typedef struct _data
{
	int type;	// 0: 왼쪽클릭(사각형), 1: 오른쪽 클릭(원)
	POINTS pt;
}DATA;
typedef struct{
	SOCKET hSocket;
}PER_HANDLE_DATA,*LPPER_HANDLE_DATA;
typedef struct{
	OVERLAPPED overlapped;
	OVERLAPPED overlapped2;
	WSABUF wsaBuf;
	DATA data;
}PER_IO_DATA,*LPPER_IO_DATA;

BOOL CALLBACK HandleDialog(HWND, UINT, WPARAM, LPARAM) ;
BOOL InitSocket( );
VOID AcceptThread(LPVOID );
VOID CompletionThread(LPVOID pComport);

HWND hWnd;
SOCKET hSocket;
SOCKET g_sockArr[100];
DATA g_data ={1000};
HANDLE CompletionPort;
int g_clientNum;

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
BOOL CALLBACK HandleDialog(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		if( !InitSocket() )
			return TRUE;
		hThread = (HANDLE)_beginthread(AcceptThread, 0, NULL);

		return TRUE;
	case WM_COMMAND:
		switch(wParam) 
		{
			return TRUE;
		case IDC_EXIT:
			DestroyWindow(hWnd);
		}
		return TRUE;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if(g_data.type == 0)
			Rectangle(hdc, g_data.pt.x, g_data.pt.y, g_data.pt.x+30, g_data.pt.y+30);
		else if(g_data.type==1)
			Ellipse(hdc, g_data.pt.x, g_data.pt.y, g_data.pt.x+30, g_data.pt.y+30);
		EndPaint(hWnd, &ps);
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		WSACleanup( );
		return TRUE;
	default :
		break;
	}
	return 0L;
}
BOOL InitSocket( )
{
	SYSTEM_INFO SystemInfo;

	///////////////////////
	WORD wVer;
	WSADATA wsaData;
	SOCKADDR_IN serv_addr;
	int i;

	wVer = MAKEWORD(2,2);
	if(WSAStartup(wVer,&wsaData) != 0 )
	{
		MessageBox(hWnd,"WSAStartup() 실패 ", NULL, MB_OK);
		return FALSE;
	}
	if((CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) ==NULL)
	{
		return FALSE;
	}
	GetSystemInfo(&SystemInfo);
	for( i = 0 ; i < (int) SystemInfo.dwNumberOfProcessors*2; i++)
	{
		HANDLE ThreadHandle;
		if((ThreadHandle =(HANDLE) _beginthread(CompletionThread,0,(LPVOID)CompletionPort))==NULL)
		{
			MessageBox(hWnd,"CreateThread() error", NULL, MB_OK);
			return FALSE;
		}
		CloseHandle(ThreadHandle);
	}

	hSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(hSocket==INVALID_SOCKET){
		MessageBox(hWnd,"socket() 실패", NULL, MB_OK);
		return FALSE;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port=htons(PORT_NUMBER);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(hSocket, (LPSOCKADDR)&serv_addr, sizeof(serv_addr)) ==
		SOCKET_ERROR) {
		MessageBox(hWnd,"bind() 실패 ", NULL, MB_OK);
		return FALSE;
	}
	if(listen(hSocket, 5) == SOCKET_ERROR) {
		MessageBox(hWnd,"listen() 실패", NULL, MB_OK);
		return FALSE;
	}
	return TRUE;
}
VOID AcceptThread(LPVOID)
{
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	SOCKET clientSock;
	DWORD Flags,RecvBytes;
	while(1)
	{
		clientSock = accept(hSocket,NULL, NULL);
		if(clientSock == INVALID_SOCKET)
		{
			cout << "Error!! accept()" << endl;
			return ;
		}


		PerHandleData = (LPPER_HANDLE_DATA)new(PER_HANDLE_DATA);
		g_sockArr[g_clientNum++] = PerHandleData->hSocket = clientSock;

		CreateIoCompletionPort((HANDLE)clientSock, CompletionPort, 
			(DWORD)PerHandleData,0);

		PerIoData = (LPPER_IO_DATA) new PER_IO_DATA;
		ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = sizeof(DATA);
		PerIoData->wsaBuf.buf = (char*)&PerIoData->data;
		Flags = 0;

		if(WSARecv(clientSock, &(PerIoData->wsaBuf), 1, &RecvBytes,
			&Flags, &(PerIoData->overlapped), NULL) == SOCKET_ERROR)
		{
			if(WSAGetLastError( ) != ERROR_IO_PENDING)
			{
				cout << "Error!! WSARecv()" << endl;
				return ;
			}
		}
	}
}

VOID CompletionThread(LPVOID pComport)
{
	HANDLE hCompletionPort = (HANDLE)pComport;
	DWORD BytesTransferred;
	LPPER_IO_DATA PerIoData;
	LPPER_HANDLE_DATA PerHandleData;
	/////////////
	DWORD Flags;
	DWORD RecvBytes,SendBytes;
	RECT rect;
	int i;

	while(1)
	{
		if(GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred,
			(LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE) == 0)
		{
			cout << "Error!! GetQueuedCompletionStatus()" << endl;
			return ;
		}
		if(BytesTransferred == 0 )
		{
			MessageBox(hWnd,"종료",NULL,MB_OK);

			if(closesocket(PerHandleData->hSocket) == SOCKET_ERROR)
			{
				return ;
			}
			delete PerHandleData;
			delete PerIoData;
			continue;
		}
		else
		{
			if(PerIoData->data.type == 2)
			{
				closesocket(PerHandleData->hSocket);
				return;
			}
			else
			{
				g_data.type = PerIoData->data.type;
				g_data.pt.x = PerIoData->data.pt.x;
				g_data.pt.y = PerIoData->data.pt.y;

				SetRect(&rect,PerIoData->data.pt.x,PerIoData->data.pt.y,
					PerIoData->data.pt.x+30,PerIoData->data.pt.y+30);
				InvalidateRect(hWnd,&rect,FALSE);
				Flags = 0;
				/*
				for( i = 0 ; i < g_clientNum ; i++)
				{
					WSASend(g_sockArr[i], &PerIoData->wsaBuf, 1,
						NULL, 0, NULL, NULL);
				}
				*/
				ZeroMemory(&(PerIoData->overlapped2), sizeof(OVERLAPPED));
				WSASend(PerHandleData->hSocket, &PerIoData->wsaBuf, 1,
						&SendBytes, 0, &PerIoData->overlapped2, NULL);

				ZeroMemory(&(PerIoData->overlapped), sizeof(OVERLAPPED));
				if(WSARecv(PerHandleData->hSocket, &(PerIoData->wsaBuf), 1,
					&RecvBytes, &Flags, &(PerIoData->overlapped), NULL) == SOCKET_ERROR)
				{
					if(WSAGetLastError() != ERROR_IO_PENDING)
					{
						cout << "Error!! WSARecv()" << endl;
						return ;
					}
				}
			}
		}
	}
	return;
}