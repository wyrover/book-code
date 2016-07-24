#ifndef _SP_COMM_H_
#define _SP_COMM_H_

#include <afxsock.h>

//#include "winsock.h"

//定义错误类型
//成功
#define SP_ERR_SUCCESS			0
//网络错误
#define SP_ERR_NETWORK			1
//没有完成读或写,只读入或写出部分要求要求数据
#define SP_ERR_NOT_FINISH		2
//超过时间
#define SP_ERR_TIMEOUT			3
//无数据
#define SP_ERR_NODATA			4
//未实现该调用
#define SP_ERR_NOT_IMPLEMENT	98
//一般错误
#define SP_ERR_GENERAL			99


//pszLocalAddr为本地所使用的IP地址,如果为NULL,表明使用任意IP地址,系统自动选择
//pszLocalAddr需要绑定的本地地址，如果为NULL表明所有地址
//返回INVALID_SOCKET表明失败
SOCKET SPCreateListenSocket(LPCSTR pszLocalAddr,UINT uPort);
//创建客户端SOCKET
//pszLocalAddr需要绑定的本地地址，如果为NULL表明所有地址
//返回INVALID_SOCKET表明失败
SOCKET SPCreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr=NULL);

class CIPCComm
{
public :
	CIPCComm(){};
	~CIPCComm(){};
public:

	//读:直到完全读入数据，或是错误发生
	//返回：SP_ERR_SUCCESS，SP_ERR_NETWORK
	//iWant:需要读入的字节数
	//iRead:返回实际读入的字节数
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead)=0;

	//写:直到完全写出数据，或是错误发生
	//返回：SP_ERR_SUCCESS，SP_ERR_NETWORK
	//iWant:需要写出的字节数
	//iWrote:返回实际写出的字节数
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote)=0;

	//测试是否有数据到达
	//返回：SP_ERR_SUCCESS(有数据到达)，SP_ERR_NETWORK，SP_ERR_NODATA
	//iTimeOut: 超时时间，秒
	virtual int TestRead(int iTimeOut)=0;

	//测试是否有连接到达
	//返回：SP_ERR_SUCCESS(有数据到达)，SP_ERR_NETWORK，SP_ERR_NODATA（无连接到达）
	//iTimeOut: 超时时间，秒
	virtual int TestConnect(int iTimeOut)=0;

	//在规定时间内读入要求数量的数据,尽可能多的读入数据
	//返回SP_ERR_SUCCESS，SP_ERR_NETWORK，SP_ERR_NOT_FINISH（超时，只读入部分数据），SP_ERR_NODATA（未读入任何数据）
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)=0;

	//检测数据并读入，在检测到数据后只进行一次读操作，
	//返回：SP_ERR_SUCCESS（读入要求数据），SP_ERR_NETWORK，SP_ERR_NOT_FINISH（检测到数据，但只读入部分数据），SP_ERR_NODATA（超时，未读入任何数据）
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead)=0;
};

class CIPCSocket: public CIPCComm
{
public:
	//fAutoDel 是否在删除对象时自动关闭Socket
	CIPCSocket(SOCKET sockComm,BOOL fAutoDel=FALSE);
	~CIPCSocket();
public:
	//创建Socket helper function
	static SOCKET CreateListenSocket(LPCSTR pszLocalAddr,UINT uPort)
	{
		return SPCreateListenSocket(pszLocalAddr,uPort);
	}
	static SOCKET CreateConnectSocket(LPCSTR pszRemoteAddr,int iPort,LPCSTR pszLocalAddr)
	{
		return SPCreateConnectSocket( pszRemoteAddr, iPort, pszLocalAddr);
	}
	//
	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead);
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote);
	virtual int TestRead(int iTimeOut);
	virtual int TestConnect(int iTimeOut);
	static int TestRead(SOCKET sock,int iTimeOut)
	{
		CIPCSocket sTemp(sock,FALSE);
		return sTemp.TestRead(iTimeOut);
	};
	static int TestConnect(SOCKET sock,int iTimeOut)
	{
		CIPCSocket sTemp(sock,FALSE);
		return sTemp.TestConnect(iTimeOut);
	};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
protected:
	SOCKET m_sockComm;
	BOOL m_fAutoDel;
};

//pszPipeName \\.\pipe\pipe_name
//返回 INVALID_HANDLE_VALUE 表明失败
//创建服务器端管道,并等待连接
//iTimeout 为等待连接的超时市场，以秒计算
HANDLE SPCreateServerPipe(LPCSTR pszPipeName,int iTimeout=10);
//创建客户端管道，并连接到服务器端
//pszPipeName \\server\pipe\pipe_name
//iTimeout 为超时时长
HANDLE SPCreateClientPipe(LPCSTR pszPipeName,int iTimeout=10);

class CIPCNamedPipe: public CIPCComm
{
public:
	//fAutoDel 是否在删除对象时自动关闭Socket
	CIPCNamedPipe(HANDLE hPipe,BOOL fAutoDel=FALSE);
	~CIPCNamedPipe();
public:
	static HANDLE CreateServerPipe(LPCSTR pszPipeName,int iTimeout=10)
	{
		return SPCreateServerPipe(pszPipeName,iTimeout);
	};
	static HANDLE CreateClientPipe(LPCSTR pszPipeName,int iTimeout=10)
	{
		return SPCreateClientPipe(pszPipeName,iTimeout);
	};

	virtual int Recv(const int iWant,BYTE* pbBuf,int& iRead);
	virtual int Send(const int iWant,const BYTE *pbBuf,int& iWrote);
	virtual int TestRead(int iTimeOut);
	virtual int TestConnect(int iTimeOut);
	static int TestRead(HANDLE hPipe,int iTimeOut)
	{
		CIPCNamedPipe sTemp(hPipe,FALSE);
		return sTemp.TestRead(iTimeOut);
	};
	static int TestConnect(HANDLE hPipe,int iTimeOut)
	{
		CIPCNamedPipe sTemp(hPipe,FALSE);
		return sTemp.TestConnect(iTimeOut);
	};
	virtual int RecvWithTimeOut(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
	virtual int TestAndRecv(int iTimeout,const int iWant,BYTE* pbBuf,int& iRead);
protected:
	HANDLE m_hPipe;
	BOOL m_fAutoDel;
};

#endif

