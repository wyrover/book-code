#pragma once
#include <Mswsock.h>

#define MAX_BUF_SIZE 4096
#define WM_SHOW_MSG WM_USER+100

enum IO_TYPE {
	IO_TYPE_ACCEPT,
	IO_TYPE_READ,
	IO_TYPE_WRITE,
	IO_TYPE_UNKNOWN
};

class COverLappedEx {
public:
	OVERLAPPED m_OLap;
	IO_TYPE m_IOType;
	char m_szBuf[MAX_BUF_SIZE];

	COverLappedEx(IO_TYPE ioType) {
		ZeroMemory(&m_OLap, sizeof(OVERLAPPED));
		m_IOType = ioType;
		ZeroMemory(m_szBuf, MAX_BUF_SIZE);
	}
};

class CPerSocketData {
public:
	SOCKET m_Socket;
	SOCKET m_AccSocket;

	CPerSocketData() {
		m_Socket = INVALID_SOCKET;
		m_AccSocket = INVALID_SOCKET;
	}
};


class CIOCPServer
{
public:
	CIOCPServer(void);
	~CIOCPServer(void);
public:
	void SetLastErrorMsg(CString strErrorMsg);
	CString GetLastErrorMsg();
	BOOL WinSockInit();
	void AssociateWnd(CWnd *pWnd);
	BOOL StartServer(UINT uListenPort);
	BOOL PostAccept(CPerSocketData *pSockData);
	BOOL PostRecv(CPerSocketData *pSockData);
	BOOL PostSend(LPCTSTR lpszText, DWORD dwSizeInBytes);
	static DWORD WINAPI ThreadPoolProc(LPVOID lpParam);
	CPerSocketData *AssignSockToCompletionPort(SOCKET tSocket);
	void StopServer();
private:
	LONG m_ThreadNums;
	CWnd *m_pWnd;
	CString m_strErrorMsg;
	SOCKET m_ListenSocket;
	HANDLE m_hCompletionPort;
	CList <CPerSocketData *, CPerSocketData *> m_ArrSocketData;
	CList <COverLappedEx *, COverLappedEx *> m_ArrOverLapEx;
};
