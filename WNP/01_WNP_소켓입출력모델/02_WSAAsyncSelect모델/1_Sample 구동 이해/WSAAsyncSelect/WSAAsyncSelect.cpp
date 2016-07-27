#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512
#define WM_SOCKET      (WM_USER+1)
#define WM_READ        (WM_USER+2)
#define WM_WRITE       (WM_USER+3)
#define WM_SOCKETCLOSE (WM_USER+4)


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND g_hWnd;
LPCTSTR lpszClass=TEXT("Class");


void ProcessSocketMessage(HWND,UINT,WPARAM,LPARAM);
void Resiv(SOCKET clnt);

BOOL Init(); // 버전 확인..
BOOL CreateSocket(SOCKET *p);
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=(WNDPROC)WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);



	g_hWnd = hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);


	if(!Init())
	{
		MessageBox(hWnd,"버전이 맞지 않습니다.","알림",0);
		SendMessage(hWnd,WM_DESTROY,0,0);
	}
	SOCKET Serv;
	if(!CreateSocket(&Serv))
	{
		MessageBox(hWnd,"소켓 생성에 실패 했습니다.","알림",0);
		SendMessage(hWnd,WM_DESTROY,0,0);
	}
	int retval = WSAAsyncSelect(Serv,hWnd,WM_SOCKET,FD_ACCEPT|FD_CLOSE);
	if(retval == SOCKET_ERROR)
	{
		SendMessage(hWnd,WM_DESTROY,0,0);
	}



	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch(iMessage) {
	case WM_CREATE:
		return 0;
	case WM_SOCKET:
		ProcessSocketMessage(hWnd,iMessage,wParam,lParam);	
		return 0;
	case WM_SOCKETCLOSE:
		MessageBox(hWnd,"종료","알림",0);
		closesocket((SOCKET)wParam);
		break;
	case WM_READ:
		Resiv((SOCKET)wParam);
		return 0;
	case WM_WRITE:
		MessageBox(hWnd,"WM_WRITE","알림",0);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

BOOL Init()
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CreateSocket(SOCKET *p)
{
	int retval;
	SOCKET Serv = socket(AF_INET,SOCK_STREAM,0);
	if(Serv == SOCKET_ERROR)
	{
		return FALSE;
	}
	SOCKADDR_IN ServAddr;
	ZeroMemory(&ServAddr,sizeof(ServAddr));

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_port = htons(8888);
	ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	retval = bind(Serv,(SOCKADDR*)&ServAddr,sizeof(ServAddr));
	if(retval == SOCKET_ERROR)
	{
		return FALSE;
	}
	retval = listen(Serv,5);
	if(retval == SOCKET_ERROR)
	{
		return FALSE;
	}
	*p = Serv;
	return TRUE;
}

void ProcessSocketMessage(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	if(WSAGETSELECTERROR(lParam))
	{
		return;
	}

	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam,(SOCKADDR*)&clientaddr,&addrlen);
		if(client_sock == INVALID_SOCKET)
		{
			return;
		}
		retval = WSAAsyncSelect(client_sock,hWnd,
			WM_SOCKET,FD_READ|FD_WRITE|FD_CLOSE);
		if(retval == SOCKET_ERROR)
		{
			return;
		}
		break;
	case FD_READ:
		SendMessage(hWnd,WM_READ,wParam,0);
		break;
	case FD_WRITE:
		SendMessage(hWnd,WM_WRITE,wParam,0);
		break;
	case FD_CLOSE:
		SendMessage(hWnd,WM_SOCKETCLOSE,wParam,0);
		break;
	}
}

void Resiv(SOCKET clnt)
{
	char buf[1024] ="";
	int len = recv(clnt,buf,1024,0);
	send(clnt,buf,len,0);
}