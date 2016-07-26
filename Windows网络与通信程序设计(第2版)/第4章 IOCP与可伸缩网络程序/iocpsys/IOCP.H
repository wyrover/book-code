////////////////////////////////////////
// IOCP.h文件

#ifndef __IOCP_H__
#define __IOCP_H__

#include <winsock2.h>
#include <windows.h>
#include <Mswsock.h>

#define BUFFER_SIZE 1024*4		// I/O请求的缓冲区大小
#define MAX_THREAD	2			// I/O服务线程的数量


// 这是per-I/O数据。它包含了在套节字上处理I/O操作的必要信息
struct CIOCPBuffer
{
	WSAOVERLAPPED ol;

	SOCKET sClient;			// AcceptEx接收的客户方套节字

	char *buff;				// I/O操作使用的缓冲区
	int nLen;				// buff缓冲区（使用的）大小

	ULONG nSequenceNumber;	// 此I/O的序列号

	int nOperation;			// 操作类型
#define OP_ACCEPT	1
#define OP_WRITE	2
#define OP_READ		3

	CIOCPBuffer *pNext;
};

// 这是per-Handle数据。它包含了一个套节字的信息
struct CIOCPContext
{
	SOCKET s;						// 套节字句柄

	SOCKADDR_IN addrLocal;			// 连接的本地地址
	SOCKADDR_IN addrRemote;			// 连接的远程地址

	BOOL bClosing;					// 套节字是否关闭

	int nOutstandingRecv;			// 此套节字上抛出的重叠操作的数量
	int nOutstandingSend;


	ULONG nReadSequence;			// 安排给接收的下一个序列号
	ULONG nCurrentReadSequence;		// 当前要读的序列号
	CIOCPBuffer *pOutOfOrderReads;	// 记录没有按顺序完成的读I/O

	CRITICAL_SECTION Lock;			// 保护这个结构

	CIOCPContext *pNext;
};


class CIOCPServer   // 处理线程
{
public:
	CIOCPServer();
	~CIOCPServer();

	// 开始服务
	BOOL Start(int nPort = 4567, int nMaxConnections = 2000, 
			int nMaxFreeBuffers = 200, int nMaxFreeContexts = 100, int nInitialReads = 4);
	// 停止服务
	void Shutdown();

	// 关闭一个连接和关闭所有连接
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnections();	

	// 取得当前的连接数量
	ULONG GetCurrentConnection() { return m_nCurrentConnection; }

	// 向指定客户发送文本
	BOOL SendText(CIOCPContext *pContext, char *pszText, int nLen); 

protected:

	// 申请和释放缓冲区对象
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);

	// 申请和释放套节字上下文
	CIOCPContext *AllocateContext(SOCKET s);
	void ReleaseContext(CIOCPContext *pContext);

	// 释放空闲缓冲区对象列表和空闲上下文对象列表
	void FreeBuffers();
	void FreeContexts();

	// 向连接列表中添加一个连接
	BOOL AddAConnection(CIOCPContext *pContext);

	// 插入和移除未决的接受请求
	BOOL InsertPendingAccept(CIOCPBuffer *pBuffer);
	BOOL RemovePendingAccept(CIOCPBuffer *pBuffer);

	// 取得下一个要读取的
	CIOCPBuffer *GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer);


	// 投递接受I/O、发送I/O、接收I/O
	BOOL PostAccept(CIOCPBuffer *pBuffer);
	BOOL PostSend(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	BOOL PostRecv(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);


		// 事件通知函数
	// 建立了一个新的连接
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// 一个连接关闭
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// 在一个连接上发生了错误
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError);
	// 一个连接上的读操作完成
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// 一个连接上的写操作完成
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

protected:

	// 记录空闲结构信息
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	int m_nFreeBufferCount;
	int m_nFreeContextCount;	
	CRITICAL_SECTION m_FreeBufferListLock;
	CRITICAL_SECTION m_FreeContextListLock;

	// 记录抛出的Accept请求
	CIOCPBuffer *m_pPendingAccepts;   // 抛出请求列表。
	long m_nPendingAcceptCount;
	CRITICAL_SECTION m_PendingAcceptsLock;

	// 记录连接列表
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	CRITICAL_SECTION m_ConnectionListLock;

	// 用于投递Accept请求
	HANDLE m_hAcceptEvent;
	HANDLE m_hRepostEvent;
	LONG m_nRepostCount;

	int m_nPort;				// 服务器监听的端口

	int m_nInitialAccepts;
	int m_nInitialReads;
	int m_nMaxAccepts;
	int m_nMaxSends;
	int m_nMaxFreeBuffers;
	int m_nMaxFreeContexts;
	int m_nMaxConnections;

	HANDLE m_hListenThread;			// 监听线程
	HANDLE m_hCompletion;			// 完成端口句柄
	SOCKET m_sListen;				// 监听套节字句柄
	LPFN_ACCEPTEX m_lpfnAcceptEx;	// AcceptEx函数地址
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs; // GetAcceptExSockaddrs函数地址

	BOOL m_bShutDown;		// 用于通知监听线程退出
	BOOL m_bServerStarted;	// 记录服务是否启动


private:	// 线程函数
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
};


#endif // __IOCP_H__