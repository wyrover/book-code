#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <TIME.h>

#define ID_EDIT1 100
#define ID_EDIT2 101


SOCKET hclnt;


HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Class");

HWND hEdit1, hEdit2;
WNDPROC OldEditProc;
LRESULT CALLBACK EditSubProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
BOOL Init();
BOOL CreateSocket();
VOID ReceiveThread(LPVOID p);
void EditLinePlus(char *string);
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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		600,200,500,400,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
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
		hEdit1=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER 
			| WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			20,10,460,320,hWnd,(HMENU)ID_EDIT1,g_hInst,NULL);
		hEdit2=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL,
			20,340,460,20,hWnd,(HMENU)ID_EDIT2,g_hInst,NULL);
		SetFocus(hEdit1);
		OldEditProc=(WNDPROC)SetWindowLong(hEdit2,GWL_WNDPROC,(LONG)EditSubProc);
		if(!Init())
		{
			return 0;
		}
		if(!CreateSocket())
		{
			return 0;
		}

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
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		return false;
	}
	return true;
}

BOOL CreateSocket()
{
	char ip[] = "203.247.221.203";
	SOCKADDR_IN servAddr;
	hclnt = socket(PF_INET,SOCK_STREAM, 0);
	if(hclnt == INVALID_SOCKET)
	{
		MessageBox(hWndMain,"소켓을 생성하지 못했습니다.","경고",NULL);
		return false;
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(8888);
	
	if(connect(hclnt, (SOCKADDR*)&servAddr,sizeof(servAddr)) == SOCKET_ERROR)
	{
		MessageBox(hWndMain,"서버와의 연결에 실패했습니다.","경고",NULL);
		return false;
	}
	char buf[5] ="";
	_beginthread(ReceiveThread,0,0);
	return true;
}

VOID ReceiveThread(LPVOID p)
{
	char buf[1024] = "";
	int i = 0;
	int code = 0;
	while(int strlen = recv(hclnt,buf,1024,0) != 0)
	{
		EditLinePlus(buf);
		memset(buf,0,1024);
	}		
}

#include <string.h>
LRESULT CALLBACK EditSubProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	char text[256]="";
	char text2[270]="";
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam==VK_RETURN) {
			GetWindowText(hWnd,text,256);
			SetWindowText(hWnd,"");
			wsprintf(text2,"%s%s",text,"\r\n");
			send(hclnt,text2,strlen(text2),0);
			SetFocus(hWnd);
		}
		break;
	}
	return CallWindowProc(OldEditProc,hWnd,iMessage,wParam,lParam);
}

void EditLinePlus(char *string)
{
	int line;
	line = GetWindowTextLength(hEdit1);
	SendMessage(hEdit1,EM_SETSEL,line,line);
	SendMessage(hEdit1,EM_REPLACESEL,0,(LPARAM)(string));
}