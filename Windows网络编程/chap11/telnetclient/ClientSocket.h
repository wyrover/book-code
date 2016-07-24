#if !defined(AFX_CLIENTSOCKET_H__42C5C9C7_3102_11D2_9A30_00C04FB78B23__INCLUDED_)
#define AFX_CLIENTSOCKET_H__42C5C9C7_3102_11D2_9A30_00C04FB78B23__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ClientSocket.h : header file
//

#include <afxsock.h>

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CTelnetView;

class CClientSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket(CTelnetView * cView);
	virtual ~CClientSocket();

// Overrides
public:
	CTelnetView * cView;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__42C5C9C7_3102_11D2_9A30_00C04FB78B23__INCLUDED_)
