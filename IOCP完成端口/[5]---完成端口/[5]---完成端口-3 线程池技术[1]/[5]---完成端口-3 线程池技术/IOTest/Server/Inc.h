
#pragma once

//#define MAX_BUF_SIZE 4096
//
//enum IO_TYPE {
//	TYPE_ACCEPT,
//	TYPE_READ,
//	TYPE_WRITE,
//	TYPE_UNKNOWN
//};
//
//class CSocketOverlap {
//public:
//	WSAOVERLAPPED m_Olap;
//	SOCKET m_Socket;
//	SOCKET m_AccSocket;
//	IO_TYPE m_IOType;
//	char m_szBuf[MAX_BUF_SIZE];
//
//	CSocketOverlap() {
//		ZeroMemory(&m_Olap, sizeof(m_Olap));
//		m_Socket = INVALID_SOCKET;
//		m_AccSocket = INVALID_SOCKET;
//		m_Olap.hEvent = WSACreateEvent();
//		m_IOType = TYPE_UNKNOWN;
//		ZeroMemory(m_szBuf, MAX_BUF_SIZE);
//	}
//};