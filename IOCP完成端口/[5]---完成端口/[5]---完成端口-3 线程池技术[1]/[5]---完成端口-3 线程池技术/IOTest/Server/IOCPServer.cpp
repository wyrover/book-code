#include "StdAfx.h"
#include "IOCPServer.h"

CIOCPServer::CIOCPServer(void)
{
	m_ThreadNums = 0;
	m_pWnd = NULL;
	m_ListenSocket = INVALID_SOCKET;
	m_hCompletionPort = NULL;
}

CIOCPServer::~CIOCPServer(void)
{
	StopServer();
}

void CIOCPServer::AssociateWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
}

void CIOCPServer::SetLastErrorMsg(CString strErrorMsg)
{
	m_strErrorMsg = strErrorMsg;
}
CString CIOCPServer::GetLastErrorMsg()
{
	return m_strErrorMsg;
}

BOOL CIOCPServer::WinSockInit()
{
	WSADATA data = {0};
	if(WSAStartup(MAKEWORD(2, 2), &data))
		return FALSE;
	if ( LOBYTE(data.wVersion) !=2 || HIBYTE(data.wVersion) != 2 ){
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

CPerSocketData *CIOCPServer::AssignSockToCompletionPort(SOCKET tSocket)
{
	ASSERT(tSocket != INVALID_SOCKET);
	CPerSocketData *pSockData = new CPerSocketData();
	pSockData->m_Socket = tSocket;
	m_ArrSocketData.AddTail(pSockData); //listen socket is also in the list;
	CreateIoCompletionPort((HANDLE)tSocket, m_hCompletionPort, (ULONG_PTR)pSockData, 0);
	return pSockData;
}

BOOL CIOCPServer::PostAccept(CPerSocketData *pSockData)
{
	DWORD dwBytesRecv = 0;
	ASSERT(pSockData != NULL);
	COverLappedEx *m_pOverLap = new COverLappedEx(IO_TYPE_ACCEPT);
	m_ArrOverLapEx.AddTail(m_pOverLap);
	pSockData->m_AccSocket = WSASocket(AF_INET , SOCK_STREAM , IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	BOOL bRet = AcceptEx(pSockData->m_Socket, pSockData->m_AccSocket, m_pOverLap->m_szBuf, 0, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &dwBytesRecv, &m_pOverLap->m_OLap);
	if (!bRet) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CIOCPServer::PostRecv(CPerSocketData *pSockData)
{
	ASSERT(pSockData != NULL);
	WSABUF wsaBuf = {0};
	COverLappedEx *m_pOverLap = new COverLappedEx(IO_TYPE_READ);
	m_ArrOverLapEx.AddTail(m_pOverLap);
	wsaBuf.buf = m_pOverLap->m_szBuf;
	wsaBuf.len = MAX_BUF_SIZE;
	DWORD dwBytesRecv = 0, dwFlags = 0;
	int iRet = WSARecv(pSockData->m_Socket, &wsaBuf, 1, &dwBytesRecv, &dwFlags, &(m_pOverLap->m_OLap), NULL);
	if (iRet != NO_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CIOCPServer::PostSend(LPCTSTR lpszText, DWORD dwSizeInBytes)
{
	DWORD dwSend = 0;
	CPerSocketData *pSockData = NULL;
	COverLappedEx *pOverLapEx = NULL;

	POSITION pos = m_ArrSocketData.GetHeadPosition();
	while (pos != NULL) {
		pSockData = m_ArrSocketData.GetNext(pos);
		if (pSockData->m_Socket != m_ListenSocket) {
			pOverLapEx = new COverLappedEx(IO_TYPE_WRITE);
			memcpy_s(pOverLapEx->m_szBuf, MAX_BUF_SIZE, lpszText, dwSizeInBytes);
			WSABUF wsaBuf = {0};
			wsaBuf.buf = pOverLapEx->m_szBuf;
			wsaBuf.len = dwSizeInBytes;
			m_ArrOverLapEx.AddTail(pOverLapEx);
			WSASend(pSockData->m_Socket, &wsaBuf, 1, &dwSend, 0, &(pOverLapEx->m_OLap), NULL);
		}
	}
	return TRUE;
}

DWORD WINAPI CIOCPServer::ThreadPoolProc(LPVOID lpParam)
{
	COverLappedEx *pOverLaps = NULL;
	CPerSocketData *pPerSockData = NULL;
	CIOCPServer *pThis = (CIOCPServer *)lpParam;
	ASSERT(pThis != NULL);

	InterlockedIncrement(&pThis->m_ThreadNums);
	BOOL bIORet = FALSE;
	DWORD dwTrans = 0;

	while (TRUE) {
		bIORet = GetQueuedCompletionStatus(pThis->m_hCompletionPort, &dwTrans, (PULONG_PTR)&pPerSockData, (LPOVERLAPPED *)&pOverLaps, INFINITE);
		if (!bIORet) continue;

		if (dwTrans == 0 && pOverLaps && (
			pOverLaps->m_IOType == IO_TYPE_READ ||
			pOverLaps->m_IOType == IO_TYPE_WRITE ))
		{
				closesocket(pPerSockData->m_Socket);
				pThis->m_ArrSocketData.RemoveAt(pThis->m_ArrSocketData.Find(pPerSockData));
				delete pPerSockData;
				pThis->m_ArrOverLapEx.RemoveAt(pThis->m_ArrOverLapEx.Find(pOverLaps));
				delete pOverLaps;
				continue;
		}
		if(bIORet && pOverLaps && pPerSockData) {
			switch (pOverLaps->m_IOType)
			{
			case IO_TYPE_READ:
				pThis->m_pWnd->SendMessage(WM_SHOW_MSG, 0, (LPARAM)pOverLaps->m_szBuf);
				pThis->m_ArrOverLapEx.RemoveAt(pThis->m_ArrOverLapEx.Find(pOverLaps));
				delete pOverLaps;
				pThis->PostRecv(pPerSockData);
				break;
			case IO_TYPE_WRITE:
				pThis->m_ArrOverLapEx.RemoveAt(pThis->m_ArrOverLapEx.Find(pOverLaps));
				delete pOverLaps;
				break;
			case IO_TYPE_ACCEPT:
				{
					if (pPerSockData->m_AccSocket == INVALID_SOCKET) {
						continue;
					}
					QueueUserWorkItem(ThreadPoolProc, pThis, WT_EXECUTELONGFUNCTION);
					CPerSocketData *pData = pThis->AssignSockToCompletionPort(pPerSockData->m_AccSocket);
					pThis->PostRecv(pData);
					pThis->PostAccept(pPerSockData);
					pThis->m_ArrOverLapEx.RemoveAt(pThis->m_ArrOverLapEx.Find(pOverLaps));
					delete pOverLaps;
				}
				break;
			default:break;
			}
		}else if (!pOverLaps && !pPerSockData) {
			//exit the thread;
			break;
		}
	}

	InterlockedDecrement(&pThis->m_ThreadNums);
	return TRUE;
}

BOOL CIOCPServer::StartServer(UINT uListenPort)
{
	if (!WinSockInit()) {
		SetLastErrorMsg(_T("Socket ¿â³õÊ¼»¯Ê§°Ü£¡"));
		return FALSE;
	}

	m_ListenSocket = WSASocket(AF_INET , SOCK_STREAM , IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if ( m_ListenSocket == INVALID_SOCKET ) {
		SetLastErrorMsg(_T("ÐÂ½¨SocketÊ§°Ü£¡"));
		goto __Error_End;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(uListenPort);

	if ( bind(m_ListenSocket, (sockaddr*)&service, sizeof(sockaddr_in)) == SOCKET_ERROR ) {
		SetLastErrorMsg(_T("°ó¶¨¶Ë¿ÚÊ§°Ü£¡"));
		goto __Error_End;
	}

	if( listen(m_ListenSocket, SOMAXCONN) == SOCKET_ERROR ) {
		SetLastErrorMsg(_T("¼àÌýÊ§°Ü£¡"));
		goto __Error_End;
	}

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if ( m_hCompletionPort == NULL ) {
		SetLastErrorMsg(_T("Íê³É¶Ë¿Ú´´½¨Ê§°Ü£¡"));
		goto __Error_End;
	}

	CPerSocketData *pSockData = AssignSockToCompletionPort(m_ListenSocket);

	QueueUserWorkItem(ThreadPoolProc, this, WT_EXECUTELONGFUNCTION);

	PostAccept(pSockData);
	return TRUE;

__Error_End:
	if (m_ListenSocket != INVALID_SOCKET) {
		closesocket(m_ListenSocket);
	}

	if (m_hCompletionPort != NULL) {
		CloseHandle(m_hCompletionPort);
	}

	WSACleanup();
	return TRUE;
}

void CIOCPServer::StopServer()
{
	CPerSocketData *pSocketData = NULL;
	POSITION pos = m_ArrSocketData.GetHeadPosition();
	while (pos != NULL) {
		pSocketData = m_ArrSocketData.GetNext(pos);
		closesocket(pSocketData->m_Socket);
		delete pSocketData;
	}
	m_ArrSocketData.RemoveAll();

	COverLappedEx *pOverLap = NULL;
	pos = m_ArrOverLapEx.GetHeadPosition();
	while (pos != NULL) {
		pOverLap = m_ArrOverLapEx.GetNext(pos);
		delete pOverLap;
	}
	m_ArrOverLapEx.RemoveAll();

	while(m_ThreadNums > 0){
		PostQueuedCompletionStatus(m_hCompletionPort, 0, 0, NULL);
		Sleep(100);
	}

	if (m_hCompletionPort != NULL) {
		CloseHandle(m_hCompletionPort);
	}
	WSACleanup();
}