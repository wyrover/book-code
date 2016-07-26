//////////////////////////////////////////////////
// WSAEventSelect文件

#include "initsock.h"
#include <stdio.h>
#include <iostream.h>
#include <windows.h>

// 初始化Winsock库
CInitSock theSock;

int main()
{
	// 事件句柄和套节字句柄表
	WSAEVENT	eventArray[WSA_MAXIMUM_WAIT_EVENTS];
	SOCKET		sockArray[WSA_MAXIMUM_WAIT_EVENTS];
	int nEventTotal = 0;

	USHORT nPort = 4567;	// 此服务器监听的端口号

	// 创建监听套节字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if(::bind(sListen, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" Failed bind() \n");
		return -1;
	}
	::listen(sListen, 5);

	// 创建事件对象，并关联到新的套节字
	WSAEVENT event = ::WSACreateEvent();	//创建事件对象
	::WSAEventSelect(sListen, event, FD_ACCEPT|FD_CLOSE);	//将制定的一组网络事件与其关联
	// 添加到表中
	eventArray[nEventTotal] = event;
	sockArray[nEventTotal] = sListen;	
	nEventTotal++;

	// 处理网络事件
	while(TRUE)
	{
		// 在所有事件对象上等待,当事件受信或者指定的时间过去时,便返回
		int nIndex = ::WSAWaitForMultipleEvents(nEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		// 对每个事件调用WSAWaitForMultipleEvents函数，以便确定它的状态
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		for(int i=nIndex; i<nEventTotal; i++)
		{
			nIndex = ::WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 1000, FALSE);
			if(nIndex == WSA_WAIT_FAILED || nIndex == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				// 获取到来的通知消息，WSAEnumNetworkEvents函数会自动重置受信事件
				WSANETWORKEVENTS event;
				::WSAEnumNetworkEvents(sockArray[i], eventArray[i], &event);
				if(event.lNetworkEvents & FD_ACCEPT)				// 处理FD_ACCEPT通知消息
				{
					if(event.iErrorCode[FD_ACCEPT_BIT] == 0)
					{
						if(nEventTotal > WSA_MAXIMUM_WAIT_EVENTS)
						{
							printf(" Too many connections! \n");
							continue;
						}
						SOCKET sNew = ::accept(sockArray[i], NULL, NULL);
						WSAEVENT event = ::WSACreateEvent();
						::WSAEventSelect(sNew, event, FD_READ|FD_CLOSE|FD_WRITE);
						// 添加到表中
						eventArray[nEventTotal] = event;
						sockArray[nEventTotal] = sNew;	
						nEventTotal++;
					}
				}
				else if(event.lNetworkEvents & FD_READ)			// 处理FD_READ通知消息
				{
					if(event.iErrorCode[FD_READ_BIT] == 0)
					{
						char szText[256];
						int nRecv = ::recv(sockArray[i], szText, strlen(szText), 0);
						if(nRecv > 0)				
						{
							szText[nRecv] = '\0';
							printf("接收到数据：%s \n", szText);
						}
					}
				}
				else if(event.lNetworkEvents & FD_CLOSE)		// 处理FD_CLOSE通知消息
				{
					if(event.iErrorCode[FD_CLOSE_BIT] == 0)
					{
						::closesocket(sockArray[i]);
						for(int j=i; j<nEventTotal-1; j++)
						{
							sockArray[j] = sockArray[j+1];
							sockArray[j] = sockArray[j+1];	
						}
						nEventTotal--;
					}
				}
				else if(event.lNetworkEvents & FD_WRITE)		// 处理FD_WRITE通知消息
				{
				}
			}
		}
	}
	return 0;
}