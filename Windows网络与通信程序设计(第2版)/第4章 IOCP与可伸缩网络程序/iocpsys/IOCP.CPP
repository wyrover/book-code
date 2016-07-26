//////////////////////////////////////////////////
// IOCP.cpp文件

#include "iocp.h"
#pragma comment(lib, "WS2_32.lib")

CIOCPServer::CIOCPServer()
{
	// 列表
	m_pFreeBufferList = NULL;
	m_pFreeContextList = NULL;	
	m_pPendingAccepts = NULL;
	m_pConnectionList = NULL;

	m_nFreeBufferCount = 0;
	m_nFreeContextCount = 0;
	m_nPendingAcceptCount = 0;
	m_nCurrentConnection = 0;

	::InitializeCriticalSection(&m_FreeBufferListLock);
	::InitializeCriticalSection(&m_FreeContextListLock);
	::InitializeCriticalSection(&m_PendingAcceptsLock);
	::InitializeCriticalSection(&m_ConnectionListLock);

	// Accept请求
	m_hAcceptEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hRepostEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_nRepostCount = 0;

	m_nPort = 4567;

	m_nInitialAccepts = 10;
	m_nInitialReads = 4;
	m_nMaxAccepts = 100;
	m_nMaxSends = 20;
	m_nMaxFreeBuffers = 200;
	m_nMaxFreeContexts = 100;
	m_nMaxConnections = 2000;

	m_hListenThread = NULL;
	m_hCompletion = NULL;
	m_sListen = INVALID_SOCKET;
	m_lpfnAcceptEx = NULL;
	m_lpfnGetAcceptExSockaddrs = NULL;

	m_bShutDown = FALSE;
	m_bServerStarted = FALSE;
	
	// 初始化WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);
}

CIOCPServer::~CIOCPServer()
{
	Shutdown();

	if(m_sListen != INVALID_SOCKET)
		::closesocket(m_sListen);
	if(m_hListenThread != NULL)
		::CloseHandle(m_hListenThread);

	::CloseHandle(m_hRepostEvent);
	::CloseHandle(m_hAcceptEvent);

	::DeleteCriticalSection(&m_FreeBufferListLock);
	::DeleteCriticalSection(&m_FreeContextListLock);
	::DeleteCriticalSection(&m_PendingAcceptsLock);
	::DeleteCriticalSection(&m_ConnectionListLock);

	::WSACleanup();	
}


///////////////////////////////////
// 自定义帮助函数

CIOCPBuffer *CIOCPServer::AllocateBuffer(int nLen)
{
	CIOCPBuffer *pBuffer = NULL;
	if(nLen > BUFFER_SIZE)
		return NULL;

	// 为缓冲区对象申请内存
	::EnterCriticalSection(&m_FreeBufferListLock);
	if(m_pFreeBufferList == NULL)  // 内存池为空，申请新的内存
	{
		pBuffer = (CIOCPBuffer *)::HeapAlloc(GetProcessHeap(), 
						HEAP_ZERO_MEMORY, sizeof(CIOCPBuffer) + BUFFER_SIZE);
	}
	else	// 从内存池中取一块来使用
	{
		pBuffer = m_pFreeBufferList;
		m_pFreeBufferList = m_pFreeBufferList->pNext;	
		pBuffer->pNext = NULL;
		m_nFreeBufferCount --;
	}
	::LeaveCriticalSection(&m_FreeBufferListLock);

	// 初始化新的缓冲区对象
	if(pBuffer != NULL)
	{
		pBuffer->buff = (char*)(pBuffer + 1);
		pBuffer->nLen = nLen;
	}
	return pBuffer;
}

void CIOCPServer::ReleaseBuffer(CIOCPBuffer *pBuffer)
{
	::EnterCriticalSection(&m_FreeBufferListLock);

	if(m_nFreeBufferCount <= m_nMaxFreeBuffers)	// 将要释放的内存添加到空闲列表中
	{
		memset(pBuffer, 0, sizeof(CIOCPBuffer) + BUFFER_SIZE);
		pBuffer->pNext = m_pFreeBufferList;
		m_pFreeBufferList = pBuffer;

		m_nFreeBufferCount ++ ;
	}
	else			// 已经达到最大值，真正的释放内存
	{
		::HeapFree(::GetProcessHeap(), 0, pBuffer);
	}

	::LeaveCriticalSection(&m_FreeBufferListLock);
}


CIOCPContext *CIOCPServer::AllocateContext(SOCKET s)
{
	CIOCPContext *pContext;

	// 申请一个CIOCPContext对象
	::EnterCriticalSection(&m_FreeContextListLock);
	if(m_pFreeContextList == NULL)
	{
		pContext = (CIOCPContext *)
				::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(CIOCPContext)); 

		::InitializeCriticalSection(&pContext->Lock);
	}
	else	
	{
		// 在空闲列表中申请
		pContext = m_pFreeContextList;
		m_pFreeContextList = m_pFreeContextList->pNext;
		pContext->pNext = NULL;

		m_nFreeBufferCount --;
	}
	::LeaveCriticalSection(&m_FreeContextListLock);

	// 初始化对象成员
	if(pContext != NULL)
	{
		pContext->s = s;
	}
	return pContext;
}

void CIOCPServer::ReleaseContext(CIOCPContext *pContext)
{
	if(pContext->s != INVALID_SOCKET)
		::closesocket(pContext->s);

	// 首先释放（如果有的话）此套节字上的没有按顺序完成的读I/O的缓冲区
	CIOCPBuffer *pNext;
	while(pContext->pOutOfOrderReads != NULL)
	{
		pNext = pContext->pOutOfOrderReads->pNext;
		ReleaseBuffer(pContext->pOutOfOrderReads);
		pContext->pOutOfOrderReads = pNext;
	}

	::EnterCriticalSection(&m_FreeContextListLock);
	
	if(m_nFreeContextCount <= m_nMaxFreeContexts) // 添加到空闲列表
	{
		// 先将关键代码段变量保存到一个临时变量中
		CRITICAL_SECTION cstmp = pContext->Lock;
		// 将要释放的上下文对象初始化为0
		memset(pContext, 0, sizeof(CIOCPContext));

		// 再放会关键代码段变量，将要释放的上下文对象添加到空闲列表的表头
		pContext->Lock = cstmp;
		pContext->pNext = m_pFreeContextList;
		m_pFreeContextList = pContext;
		
		// 更新计数
		m_nFreeContextCount ++;
	}
	else
	{
		::DeleteCriticalSection(&pContext->Lock);
		::HeapFree(::GetProcessHeap(), 0, pContext);
	}

	::LeaveCriticalSection(&m_FreeContextListLock);
}

void CIOCPServer::FreeBuffers()
{
	// 遍历m_pFreeBufferList空闲列表，释放缓冲区池内存
	::EnterCriticalSection(&m_FreeBufferListLock);

	CIOCPBuffer *pFreeBuffer = m_pFreeBufferList;
	CIOCPBuffer *pNextBuffer;
	while(pFreeBuffer != NULL)
	{
		pNextBuffer = pFreeBuffer->pNext;
		if(!::HeapFree(::GetProcessHeap(), 0, pFreeBuffer))
		{
#ifdef _DEBUG
			::OutputDebugString("  FreeBuffers释放内存出错！");
#endif // _DEBUG
			break;
		}
		pFreeBuffer = pNextBuffer;
	}
	m_pFreeBufferList = NULL;
	m_nFreeBufferCount = 0;

	::LeaveCriticalSection(&m_FreeBufferListLock);
}

void CIOCPServer::FreeContexts()
{
	// 遍历m_pFreeContextList空闲列表，释放缓冲区池内存
	::EnterCriticalSection(&m_FreeContextListLock);
	
	CIOCPContext *pFreeContext = m_pFreeContextList;
	CIOCPContext *pNextContext;
	while(pFreeContext != NULL)
	{
		pNextContext = pFreeContext->pNext;
		
		::DeleteCriticalSection(&pFreeContext->Lock);
		if(!::HeapFree(::GetProcessHeap(), 0, pFreeContext))
		{
#ifdef _DEBUG
			::OutputDebugString("  FreeBuffers释放内存出错！");
#endif // _DEBUG
			break;
		}
		pFreeContext = pNextContext;
	}
	m_pFreeContextList = NULL;
	m_nFreeContextCount = 0;

	::LeaveCriticalSection(&m_FreeContextListLock);
}


BOOL CIOCPServer::AddAConnection(CIOCPContext *pContext)
{
	// 向客户连接列表添加一个CIOCPContext对象

	::EnterCriticalSection(&m_ConnectionListLock);
	if(m_nCurrentConnection <= m_nMaxConnections)
	{
		// 添加到表头
		pContext->pNext = m_pConnectionList;
		m_pConnectionList = pContext;
		// 更新计数
		m_nCurrentConnection ++;

		::LeaveCriticalSection(&m_ConnectionListLock);
		return TRUE;
	}
	::LeaveCriticalSection(&m_ConnectionListLock);

	return FALSE;
}

void CIOCPServer::CloseAConnection(CIOCPContext *pContext)
{
	// 首先从列表中移除要关闭的连接
	::EnterCriticalSection(&m_ConnectionListLock);

	CIOCPContext* pTest = m_pConnectionList;
	if(pTest == pContext)
	{
		m_pConnectionList =  pContext->pNext;
		m_nCurrentConnection --;
	}
	else
	{
		while(pTest != NULL && pTest->pNext !=  pContext)
			pTest = pTest->pNext;
		if(pTest != NULL)
		{
			pTest->pNext =  pContext->pNext;
			m_nCurrentConnection --;
		}
	}
	
	::LeaveCriticalSection(&m_ConnectionListLock);

	// 然后关闭客户套节字
	::EnterCriticalSection(&pContext->Lock);

	if(pContext->s != INVALID_SOCKET)
	{
		::closesocket(pContext->s);	
		pContext->s = INVALID_SOCKET;
	}
	pContext->bClosing = TRUE;

	::LeaveCriticalSection(&pContext->Lock);
}

void CIOCPServer::CloseAllConnections()
{
	// 遍历整个连接列表，关闭所有的客户套节字

	::EnterCriticalSection(&m_ConnectionListLock);

	CIOCPContext *pContext = m_pConnectionList;
	while(pContext != NULL)
	{	
		::EnterCriticalSection(&pContext->Lock);

		if(pContext->s != INVALID_SOCKET)
		{
			::closesocket(pContext->s);
			pContext->s = INVALID_SOCKET;
		}

		pContext->bClosing = TRUE;

		::LeaveCriticalSection(&pContext->Lock);	
		
		pContext = pContext->pNext;
	}

	m_pConnectionList = NULL;
	m_nCurrentConnection = 0;

	::LeaveCriticalSection(&m_ConnectionListLock);
}


BOOL CIOCPServer::InsertPendingAccept(CIOCPBuffer *pBuffer)
{
	// 将一个I/O缓冲区对象插入到m_pPendingAccepts表中

	::EnterCriticalSection(&m_PendingAcceptsLock);

	if(m_pPendingAccepts == NULL)
		m_pPendingAccepts = pBuffer;
	else
	{
		pBuffer->pNext = m_pPendingAccepts;
		m_pPendingAccepts = pBuffer;
	}
	m_nPendingAcceptCount ++;

	::LeaveCriticalSection(&m_PendingAcceptsLock);

	return TRUE;
}

BOOL CIOCPServer::RemovePendingAccept(CIOCPBuffer *pBuffer)
{
	BOOL bResult = FALSE;

	// 遍历m_pPendingAccepts表，从中移除pBuffer所指向的缓冲区对象
	::EnterCriticalSection(&m_PendingAcceptsLock);

	CIOCPBuffer *pTest = m_pPendingAccepts;
	if(pTest == pBuffer)	// 如果是表头元素
	{
		m_pPendingAccepts = pBuffer->pNext;
		bResult = TRUE;
	}
	else					// 不是表头元素的话，就要遍历这个表来查找了
	{
		while(pTest != NULL && pTest->pNext != pBuffer)
			pTest = pTest->pNext;
		if(pTest != NULL)
		{
			pTest->pNext = pBuffer->pNext;
			 bResult = TRUE;
		}
	}
	// 更新计数
	if(bResult)
		m_nPendingAcceptCount --;

	::LeaveCriticalSection(&m_PendingAcceptsLock);

	return  bResult;
}


CIOCPBuffer *CIOCPServer::GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	if(pBuffer != NULL)
	{
		// 如果与要读的下一个序列号相等，则读这块缓冲区
		if(pBuffer->nSequenceNumber == pContext->nCurrentReadSequence)
		{
			return pBuffer;
		}
		
		// 如果不相等，则说明没有按顺序接收数据，将这块缓冲区保存到连接的pOutOfOrderReads列表中

		// 列表中的缓冲区是按照其序列号从小到大的顺序排列的

		pBuffer->pNext = NULL;
		
		CIOCPBuffer *ptr = pContext->pOutOfOrderReads;
		CIOCPBuffer *pPre = NULL;
		while(ptr != NULL)
		{
			if(pBuffer->nSequenceNumber < ptr->nSequenceNumber)
				break;
			
			pPre = ptr;
			ptr = ptr->pNext;
		}
		
		if(pPre == NULL) // 应该插入到表头
		{
			pBuffer->pNext = pContext->pOutOfOrderReads;
			pContext->pOutOfOrderReads = pBuffer;
		}
		else			// 应该插入到表的中间
		{
			pBuffer->pNext = pPre->pNext;
			pPre->pNext = pBuffer->pNext;
		}
	}

	// 检查表头元素的序列号，如果与要读的序列号一致，就将它从表中移除，返回给用户
	CIOCPBuffer *ptr = pContext->pOutOfOrderReads;
	if(ptr != NULL && (ptr->nSequenceNumber == pContext->nCurrentReadSequence))
	{
		pContext->pOutOfOrderReads = ptr->pNext;
		return ptr;
	}
	return NULL;
}


BOOL CIOCPServer::PostAccept(CIOCPBuffer *pBuffer)	// 在监听套节字上投递Accept请求
{
		// 设置I/O类型
		pBuffer->nOperation = OP_ACCEPT;

		// 投递此重叠I/O  
		DWORD dwBytes;
		pBuffer->sClient = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		BOOL b = m_lpfnAcceptEx(m_sListen, 
			pBuffer->sClient,
			pBuffer->buff, 
			pBuffer->nLen - ((sizeof(sockaddr_in) + 16) * 2),
			sizeof(sockaddr_in) + 16, 
			sizeof(sockaddr_in) + 16, 
			&dwBytes, 
			&pBuffer->ol);
		if(!b && ::WSAGetLastError() != WSA_IO_PENDING)
		{
			return FALSE;
		}
		return TRUE;
};

BOOL CIOCPServer::PostRecv(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	// 设置I/O类型
	pBuffer->nOperation = OP_READ;	
	
	::EnterCriticalSection(&pContext->Lock);

	// 设置序列号
	pBuffer->nSequenceNumber = pContext->nReadSequence;

	// 投递此重叠I/O
	DWORD dwBytes;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.buf = pBuffer->buff;
	buf.len = pBuffer->nLen;
	if(::WSARecv(pContext->s, &buf, 1, &dwBytes, &dwFlags, &pBuffer->ol, NULL) != NO_ERROR)
	{
		if(::WSAGetLastError() != WSA_IO_PENDING)
		{
			::LeaveCriticalSection(&pContext->Lock);
			return FALSE;
		}
	}

	// 增加套节字上的重叠I/O计数和读序列号计数

	pContext->nOutstandingRecv ++;
	pContext->nReadSequence ++;

	::LeaveCriticalSection(&pContext->Lock);

	return TRUE;
}

BOOL CIOCPServer::PostSend(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{	
	// 跟踪投递的发送的数量，防止用户仅发送数据而不接收，导致服务器抛出大量发送操作
	if(pContext->nOutstandingSend > m_nMaxSends)
		return FALSE;

	// 设置I/O类型，增加套节字上的重叠I/O计数
	pBuffer->nOperation = OP_WRITE;

	// 投递此重叠I/O
	DWORD dwBytes;
	DWORD dwFlags = 0;
	WSABUF buf;
	buf.buf = pBuffer->buff;
	buf.len = pBuffer->nLen;
	if(::WSASend(pContext->s, 
			&buf, 1, &dwBytes, dwFlags, &pBuffer->ol, NULL) != NO_ERROR)
	{
		if(::WSAGetLastError() != WSA_IO_PENDING)
			return FALSE;
	}	
	
	// 增加套节字上的重叠I/O计数
	::EnterCriticalSection(&pContext->Lock);
	pContext->nOutstandingSend ++;
	::LeaveCriticalSection(&pContext->Lock);

	return TRUE;
}


BOOL CIOCPServer::Start(int nPort, int nMaxConnections, 
			int nMaxFreeBuffers, int nMaxFreeContexts, int nInitialReads)
{
	// 检查服务是否已经启动
	if(m_bServerStarted)
		return FALSE;

	// 保存用户参数
	m_nPort = nPort;
	m_nMaxConnections = nMaxConnections;
	m_nMaxFreeBuffers = nMaxFreeBuffers;
	m_nMaxFreeContexts = nMaxFreeContexts;
	m_nInitialReads = nInitialReads;

	// 初始化状态变量
	m_bShutDown = FALSE;
	m_bServerStarted = TRUE;


	// 创建监听套节字，绑定到本地端口，进入监听模式
	m_sListen = ::WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN si;
	si.sin_family = AF_INET;
	si.sin_port = ::ntohs(m_nPort);
	si.sin_addr.S_un.S_addr = INADDR_ANY;
	if(::bind(m_sListen, (sockaddr*)&si, sizeof(si)) == SOCKET_ERROR)
	{
		m_bServerStarted = FALSE;
		return FALSE;
	}
	::listen(m_sListen, 200);

	// 创建完成端口对象
	m_hCompletion = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	// 加载扩展函数AcceptEx
	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes;
	::WSAIoctl(m_sListen, 
		SIO_GET_EXTENSION_FUNCTION_POINTER, 
		&GuidAcceptEx, 
		sizeof(GuidAcceptEx),
		&m_lpfnAcceptEx, 
		sizeof(m_lpfnAcceptEx), 
		&dwBytes, 
		NULL, 
		NULL);
	
	// 加载扩展函数GetAcceptExSockaddrs
	GUID GuidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
	::WSAIoctl(m_sListen,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidGetAcceptExSockaddrs,
		sizeof(GuidGetAcceptExSockaddrs),
		&m_lpfnGetAcceptExSockaddrs,
		sizeof(m_lpfnGetAcceptExSockaddrs),
		&dwBytes,
		NULL,
		NULL
		);
	
	
	// 将监听套节字关联到完成端口，注意，这里为它传递的CompletionKey为0
	::CreateIoCompletionPort((HANDLE)m_sListen, m_hCompletion, (DWORD)0, 0);

	// 注册FD_ACCEPT事件。
	// 如果投递的AcceptEx I/O不够，线程会接收到FD_ACCEPT网络事件，说明应该投递更多的AcceptEx I/O
	WSAEventSelect(m_sListen, m_hAcceptEvent, FD_ACCEPT);

	// 创建监听线程
	m_hListenThread = ::CreateThread(NULL, 0, _ListenThreadProc, this, 0, NULL);
	
	return TRUE;
}

void CIOCPServer::Shutdown()
{
	if(!m_bServerStarted)
		return;

	// 通知监听线程，马上停止服务
	m_bShutDown = TRUE;
	::SetEvent(m_hAcceptEvent);
	// 等待监听线程退出
	::WaitForSingleObject(m_hListenThread, INFINITE);
	::CloseHandle(m_hListenThread);
	m_hListenThread = NULL;

	m_bServerStarted = FALSE;
}

DWORD WINAPI CIOCPServer::_ListenThreadProc(LPVOID lpParam)
{
	CIOCPServer *pThis = (CIOCPServer*)lpParam;

	// 先在监听套节字上投递几个Accept I/O
	CIOCPBuffer *pBuffer;
	for(int i=0; i<pThis->m_nInitialAccepts; i++)
	{
		pBuffer = pThis->AllocateBuffer(BUFFER_SIZE);
		if(pBuffer == NULL)
			return -1;
		pThis->InsertPendingAccept(pBuffer);
		pThis->PostAccept(pBuffer);
	}

	// 构建事件对象数组，以便在上面调用WSAWaitForMultipleEvents函数
	HANDLE hWaitEvents[2 + MAX_THREAD];
	int nEventCount = 0;
	hWaitEvents[nEventCount ++] = pThis->m_hAcceptEvent;
	hWaitEvents[nEventCount ++] = pThis->m_hRepostEvent;

	// 创建指定数量的工作线程在完成端口上处理I/O
	for(i=0; i<MAX_THREAD; i++)
	{
		hWaitEvents[nEventCount ++] = ::CreateThread(NULL, 0, _WorkerThreadProc, pThis, 0, NULL);
	}

	// 下面进入无限循环，处理事件对象数组中的事件
	while(TRUE)
	{
		int nIndex = ::WSAWaitForMultipleEvents(nEventCount, hWaitEvents, FALSE, 60*1000, FALSE);
	
		// 首先检查是否要停止服务
		if(pThis->m_bShutDown || nIndex == WSA_WAIT_FAILED)
		{
			// 关闭所有连接
			pThis->CloseAllConnections();
			::Sleep(0);		// 给I/O工作线程一个执行的机会
			// 关闭监听套节字
			::closesocket(pThis->m_sListen);
			pThis->m_sListen = INVALID_SOCKET;
			::Sleep(0);		// 给I/O工作线程一个执行的机会

			// 通知所有I/O处理线程退出
			for(int i=2; i<MAX_THREAD + 2; i++)
			{	
				::PostQueuedCompletionStatus(pThis->m_hCompletion, -1, 0, NULL);
			}

			// 等待I/O处理线程退出
			::WaitForMultipleObjects(MAX_THREAD, &hWaitEvents[2], TRUE, 5*1000);

			for(i=2; i<MAX_THREAD + 2; i++)
			{	
				::CloseHandle(hWaitEvents[i]);
			}
		
			::CloseHandle(pThis->m_hCompletion);

			pThis->FreeBuffers();
			pThis->FreeContexts();
			::ExitThread(0);
		}	

		// 1）定时检查所有未返回的AcceptEx I/O的连接建立了多长时间
		if(nIndex == WSA_WAIT_TIMEOUT)
		{
			pBuffer = pThis->m_pPendingAccepts;
			while(pBuffer != NULL)
			{
				int nSeconds;
				int nLen = sizeof(nSeconds);
				// 取得连接建立的时间
				::getsockopt(pBuffer->sClient, 
					SOL_SOCKET, SO_CONNECT_TIME, (char *)&nSeconds, &nLen);	
				// 如果超过2分钟客户还不发送初始数据，就让这个客户go away
				if(nSeconds != -1 && nSeconds > 2*60)
				{   
					closesocket(pBuffer->sClient);
                    pBuffer->sClient = INVALID_SOCKET;
				}

				pBuffer = pBuffer->pNext;
			}
		}
		else
		{
			nIndex = nIndex - WAIT_OBJECT_0;
			WSANETWORKEVENTS ne;
            int nLimit=0;
			if(nIndex == 0)			// 2）m_hAcceptEvent事件对象受信，说明投递的Accept请求不够，需要增加
			{
				::WSAEnumNetworkEvents(pThis->m_sListen, hWaitEvents[nIndex], &ne);
				if(ne.lNetworkEvents & FD_ACCEPT)
				{
					nLimit = 50;  // 增加的个数，这里设为50个
				}
			}
			else if(nIndex == 1)	// 3）m_hRepostEvent事件对象受信，说明处理I/O的线程接受到新的客户
			{
				nLimit = InterlockedExchange(&pThis->m_nRepostCount, 0);
			}
			else if(nIndex > 1)		// I/O服务线程退出，说明有错误发生，关闭服务器
			{
				pThis->m_bShutDown = TRUE;
				continue;
			}

			// 投递nLimit个AcceptEx I/O请求
			int i = 0;
			while(i++ < nLimit && pThis->m_nPendingAcceptCount < pThis->m_nMaxAccepts)
			{
				pBuffer = pThis->AllocateBuffer(BUFFER_SIZE);
				if(pBuffer != NULL)
				{
					pThis->InsertPendingAccept(pBuffer);
					pThis->PostAccept(pBuffer);
				}
			}
		}
	}
	return 0;
}

DWORD WINAPI CIOCPServer::_WorkerThreadProc(LPVOID lpParam)
{
#ifdef _DEBUG
			::OutputDebugString("	WorkerThread 启动... \n");
#endif // _DEBUG

	CIOCPServer *pThis = (CIOCPServer*)lpParam;

	CIOCPBuffer *pBuffer;
	DWORD dwKey;
	DWORD dwTrans;
	LPOVERLAPPED lpol;
	while(TRUE)
	{
		// 在关联到此完成端口的所有套节字上等待I/O完成
		BOOL bOK = ::GetQueuedCompletionStatus(pThis->m_hCompletion, 
					&dwTrans, (LPDWORD)&dwKey, (LPOVERLAPPED*)&lpol, WSA_INFINITE);

		if(dwTrans == -1) // 用户通知退出
		{
#ifdef _DEBUG
			::OutputDebugString("	WorkerThread 退出 \n");
#endif // _DEBUG
			::ExitThread(0);
		}

		pBuffer = CONTAINING_RECORD(lpol, CIOCPBuffer, ol);
		int nError = NO_ERROR;
		if(!bOK)						// 在此套节字上有错误发生
		{
			SOCKET s;
			if(pBuffer->nOperation == OP_ACCEPT)
			{
				s = pThis->m_sListen;
			}
			else
			{
				if(dwKey == 0)
					break;
				s = ((CIOCPContext*)dwKey)->s;
			}
			DWORD dwFlags = 0;
			if(!::WSAGetOverlappedResult(s, &pBuffer->ol, &dwTrans, FALSE, &dwFlags))
			{
				nError = ::WSAGetLastError();
			}
		}
		pThis->HandleIO(dwKey, pBuffer, dwTrans, nError);
	}

#ifdef _DEBUG
			::OutputDebugString("	WorkerThread 退出 \n");
#endif // _DEBUG
	return 0;
}


void CIOCPServer::HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError)
{
	CIOCPContext *pContext = (CIOCPContext *)dwKey;

#ifdef _DEBUG
			::OutputDebugString("	HandleIO... \n");
#endif // _DEBUG
	
	// 1）首先减少套节字上的未决I/O计数
	if(pContext != NULL)
	{
		::EnterCriticalSection(&pContext->Lock);
		
		if(pBuffer->nOperation == OP_READ)
			pContext->nOutstandingRecv --;
		else if(pBuffer->nOperation == OP_WRITE)
			pContext->nOutstandingSend --;
		
		::LeaveCriticalSection(&pContext->Lock);
		
		// 2）检查套节字是否已经被我们关闭
		if(pContext->bClosing) 
		{
#ifdef _DEBUG
			::OutputDebugString("	检查到套节字已经被我们关闭 \n");
#endif // _DEBUG
			if(pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend == 0)
			{		
				ReleaseContext(pContext);
			}
			// 释放已关闭套节字的未决I/O
			ReleaseBuffer(pBuffer);	
			return;
		}
	}
	else
	{
		RemovePendingAccept(pBuffer);
	}

	// 3）检查套节字上发生的错误，如果有的话，通知用户，然后关闭套节字
	if(nError != NO_ERROR)
	{
		if(pBuffer->nOperation != OP_ACCEPT)
		{
			OnConnectionError(pContext, pBuffer, nError);
			CloseAConnection(pContext);
			if(pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend == 0)
			{		
				ReleaseContext(pContext);
			}
#ifdef _DEBUG
			::OutputDebugString("	检查到客户套节字上发生错误 \n");
#endif // _DEBUG
		}
		else // 在监听套节字上发生错误，也就是监听套节字处理的客户出错了
		{
			// 客户端出错，释放I/O缓冲区
			if(pBuffer->sClient != INVALID_SOCKET)
			{
				::closesocket(pBuffer->sClient);
				pBuffer->sClient = INVALID_SOCKET;
			}
#ifdef _DEBUG
			::OutputDebugString("	检查到监听套节字上发生错误 \n");
#endif // _DEBUG
		}

		ReleaseBuffer(pBuffer);
		return;
	}


	// 开始处理
	if(pBuffer->nOperation == OP_ACCEPT)
	{
		if(dwTrans == 0)
		{
#ifdef _DEBUG
			::OutputDebugString("	监听套节字上客户端关闭 \n");
#endif // _DEBUG
			
			if(pBuffer->sClient != INVALID_SOCKET)
			{
				::closesocket(pBuffer->sClient);
				pBuffer->sClient = INVALID_SOCKET;
			}
		}
		else
		{
			// 为新接受的连接申请客户上下文对象
			CIOCPContext *pClient = AllocateContext(pBuffer->sClient);
			if(pClient != NULL)
			{
				if(AddAConnection(pClient))
				{	
					// 取得客户地址
					int nLocalLen, nRmoteLen;
					LPSOCKADDR pLocalAddr, pRemoteAddr;
					m_lpfnGetAcceptExSockaddrs(
						pBuffer->buff,
						pBuffer->nLen - ((sizeof(sockaddr_in) + 16) * 2),
						sizeof(sockaddr_in) + 16,
						sizeof(sockaddr_in) + 16,
						(SOCKADDR **)&pLocalAddr,
						&nLocalLen,
						(SOCKADDR **)&pRemoteAddr,
						&nRmoteLen);
					memcpy(&pClient->addrLocal, pLocalAddr, nLocalLen);
					memcpy(&pClient->addrRemote, pRemoteAddr, nRmoteLen);
					
					// 关联新连接到完成端口对象
					::CreateIoCompletionPort((HANDLE)pClient->s, m_hCompletion, (DWORD)pClient, 0);
					
					// 通知用户
					pBuffer->nLen = dwTrans;
					OnConnectionEstablished(pClient, pBuffer);
					
					// 向新连接投递几个Read请求，这些空间在套节字关闭或出错时释放
					for(int i=0; i<5; i++)
					{
						CIOCPBuffer *p = AllocateBuffer(BUFFER_SIZE);
						if(p != NULL)
						{
							if(!PostRecv(pClient, p))
							{
								CloseAConnection(pClient);
								break;
							}
						}
					}
				}
				else	// 连接数量已满，关闭连接
				{
					CloseAConnection(pClient);
					ReleaseContext(pClient);
				}
			}
			else
			{
				// 资源不足，关闭与客户的连接即可
				::closesocket(pBuffer->sClient);
				pBuffer->sClient = INVALID_SOCKET;
			}
		}
		
		// Accept请求完成，释放I/O缓冲区
		ReleaseBuffer(pBuffer);	

		// 通知监听线程继续再投递一个Accept请求
		::InterlockedIncrement(&m_nRepostCount);
		::SetEvent(m_hRepostEvent);
	}
	else if(pBuffer->nOperation == OP_READ)
	{
		if(dwTrans == 0)	// 对方关闭套节字
		{
			// 先通知用户
			pBuffer->nLen = 0;
			OnConnectionClosing(pContext, pBuffer);	
			// 再关闭连接
			CloseAConnection(pContext);
			// 释放客户上下文和缓冲区对象
			if(pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend == 0)
			{		
				ReleaseContext(pContext);
			}
			ReleaseBuffer(pBuffer);	
		}
		else
		{
			pBuffer->nLen = dwTrans;
			// 按照I/O投递的顺序读取接收到的数据
			CIOCPBuffer *p = GetNextReadBuffer(pContext, pBuffer);
			while(p != NULL)
			{
				// 通知用户
				OnReadCompleted(pContext, p);
				// 增加要读的序列号的值
				::InterlockedIncrement((LONG*)&pContext->nCurrentReadSequence);
				// 释放这个已完成的I/O
				ReleaseBuffer(p);
				p = GetNextReadBuffer(pContext, NULL);
			}

			// 继续投递一个新的接收请求
			pBuffer = AllocateBuffer(BUFFER_SIZE);
			if(pBuffer == NULL || !PostRecv(pContext, pBuffer))
			{
				CloseAConnection(pContext);
			}
		}
	}
	else if(pBuffer->nOperation == OP_WRITE)
	{

		if(dwTrans == 0)	// 对方关闭套节字
		{
			// 先通知用户
			pBuffer->nLen = 0;
			OnConnectionClosing(pContext, pBuffer);	

			// 再关闭连接
			CloseAConnection(pContext);

			// 释放客户上下文和缓冲区对象
			if(pContext->nOutstandingRecv == 0 && pContext->nOutstandingSend == 0)
			{		
				ReleaseContext(pContext);
			}
			ReleaseBuffer(pBuffer);	
		}
		else
		{
			// 写操作完成，通知用户
			pBuffer->nLen = dwTrans;
			OnWriteCompleted(pContext, pBuffer);
			// 释放SendText函数申请的缓冲区
			ReleaseBuffer(pBuffer);
		}
	}
}


BOOL CIOCPServer::SendText(CIOCPContext *pContext, char *pszText, int nLen)
{
	CIOCPBuffer *pBuffer = AllocateBuffer(nLen);
	if(pBuffer != NULL)
	{
		memcpy(pBuffer->buff, pszText, nLen);
		return PostSend(pContext, pBuffer);
	}
	return FALSE;
}


void CIOCPServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}

void CIOCPServer::OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}


void CIOCPServer::OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}

void CIOCPServer::OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
}

void CIOCPServer::OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError)
{
}