// MultiCast.h: interface for the CMultiCast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Winsock2.h"
#pragma comment(lib,"Ws2_32.lib")

#define WM_MULTIPOINTEVENT WM_USER + 1001

class CMultiCast : public CObject  
{
public:
	void Close();
	BOOL IsConnected();
	static void Unintilize();
	static void Initilize();
	int Receive(char* lpData, int size);
	int Send(char* lpData, int size);
	BOOL Create(CString lpstrAddr, unsigned short port, HWND hWnd);
	CMultiCast();
	virtual ~CMultiCast();

private:
	HWND	m_hWnd;
	SOCKET	m_hSocket;
	SOCKET  m_hGroupSocket;
	BOOL	m_bConnected;

	SOCKADDR_IN addr;
	SOCKADDR_IN srcaddr;
};

#endif // !defined(AFX_MULTICAST_H__0F627997_5A90_11D5_B587_00E04C39B036__INCLUDED_)
