/////////////////////////////////////////////// 
// WSAAsyncSelect.cpp文件
//WSAAsyncSelect模型允许程序以Windows消息形式接收网络事件通知.
#include "../common/initsock.h"
#include <stdio.h>

#define WM_SOCKET WM_USER + 101		// 自定义消息
CInitSock theSock;


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int main()
{
	char szClassName[] = "MainWClass";	
	WNDCLASSEX wndclass;
	// 用描述主窗口的参数填充WNDCLASSEX结构
	wndclass.cbSize = sizeof(wndclass);	
	wndclass.style = CS_HREDRAW|CS_VREDRAW;	
	wndclass.lpfnWndProc = WindowProc;	
	wndclass.cbClsExtra = 0;		
	wndclass.cbWndExtra = 0;		
	wndclass.hInstance = NULL;		
	wndclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);	
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);		
	wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);	
	wndclass.lpszMenuName = NULL;		
	wndclass.lpszClassName = szClassName ;
	wndclass.hIconSm = NULL;	
	::RegisterClassEx(&wndclass); 
	// 创建主窗口
	HWND hWnd = ::CreateWindowEx( 
		0,						
		szClassName,			
		"",	
		WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT,	
		CW_USEDEFAULT,		
		CW_USEDEFAULT,	
		CW_USEDEFAULT,			
		NULL,				
		NULL,		
		NULL,	
		NULL);		
	if(hWnd == NULL)
	{
		::MessageBox(NULL, "创建窗口出错！", "error", MB_OK);
		return -1;
	}

	USHORT nPort = 4567;	// 此服务器监听的端口号

	// 创建监听套节字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	// 绑定套节字到本地机器
	if(::bind(sListen, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" Failed bind() \n");
		return -1;
	}

	// 将套接字设为窗口通知消息类型。
	::WSAAsyncSelect(sListen, hWnd, WM_SOCKET, FD_ACCEPT|FD_CLOSE);

	// 进入监听模式
	::listen(sListen, 5);

	// 从消息队列中取出消息
	MSG msg;	
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		// 转化键盘消息
		::TranslateMessage(&msg);
		// 将消息发送到相应的窗口函数
		::DispatchMessage(&msg);
	}
	// 当GetMessage返回0时程序结束
	return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{	
	case WM_SOCKET:
		{
			// 取得有事件发生的套节字句柄
			SOCKET s = wParam;
			// 查看是否出错
			if(WSAGETSELECTERROR(lParam))
			{
				::closesocket(s);
				return 0;
			}
			// 处理发生的事件
			switch(WSAGETSELECTEVENT(lParam))	//将消息取出
			{
			case FD_ACCEPT:		// 监听中的套接字检测到有连接进入
				{
					SOCKET client = ::accept(s, NULL, NULL);
					::WSAAsyncSelect(client, hWnd, WM_SOCKET, FD_READ|FD_WRITE|FD_CLOSE);
				}
				break;
			case FD_WRITE:
				{
				}
				break;
			case FD_READ:
				{
					char szText[1024] = { 0 };
					if(::recv(s, szText, 1024, 0) == -1)
						::closesocket(s);
					else
						printf("接收数据：%s", szText);
				}
				break;
			case FD_CLOSE:
				{ 
					::closesocket(s);
				}
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0) ;
		return 0 ;
	}

	// 将我们不处理的消息交给系统做默认处理
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}