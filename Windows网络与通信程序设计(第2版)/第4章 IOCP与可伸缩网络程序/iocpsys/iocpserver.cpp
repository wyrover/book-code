////////////////////////////////////////////////
// iocpserver.cpp文件


// CIOCPServer类的测试程序

#include "iocp.h"
#include <stdio.h>
#include <windows.h>

class CMyServer : public CIOCPServer
{
public:

	void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		printf(" 接收到一个新的连接（%d）： %s \n", 
					GetCurrentConnection(), ::inet_ntoa(pContext->addrRemote.sin_addr));

		SendText(pContext, pBuffer->buff, pBuffer->nLen);
	}

	void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		printf(" 一个连接关闭！ \n" );
	}

	void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError)
	{
		printf(" 一个连接发生错误： %d \n ", nError);
	}

	void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		SendText(pContext, pBuffer->buff, pBuffer->nLen);
	}
	
	void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
	{
		printf(" 数据发送成功！\n ");
	}
};

void main()
{
	CMyServer *pServer = new CMyServer;

	// 开启服务
	if(pServer->Start())
	{
		printf(" 服务器开启成功... \n");
	}
	else
	{
		printf(" 服务器开启失败！\n");
		return;
	}

	// 创建事件对象，让ServerShutdown程序能够关闭自己
	HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, "ShutdownEvent");
	::WaitForSingleObject(hEvent, INFINITE);
	::CloseHandle(hEvent);

	// 关闭服务
	pServer->Shutdown();
	delete pServer;

	printf(" 服务器关闭 \n ");

}

