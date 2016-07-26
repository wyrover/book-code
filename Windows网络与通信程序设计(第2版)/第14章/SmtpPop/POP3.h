// POP3.h: Interface for CPOP3.
// In adaption of Wes Clyburn's CSMTP class.
// Copyright (C) 1998 Michael Krebs
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POP3_H__CFFD2FBB_859A_11D2_A529_444553540000__INCLUDED_)
#define AFX_POP3_H__CFFD2FBB_859A_11D2_A529_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxsock.h>
#include "MailMessage.h"

#define POP3_PORT 110		// Standard port for POP3 servers
#define RESPONSE_BUFFER_SIZE 1024

class CPOP3  
{
public:
	CPOP3( LPCTSTR szPOP3ServerName, UINT nPort = POP3_PORT, LPCTSTR sUsername = NULL, LPCTSTR sPassword = NULL );
	virtual ~CPOP3();
	
	void SetServerProperties( LPCTSTR sServerHostName, UINT nPort = POP3_PORT );
	void SetUserProperties( LPCTSTR sUsername, LPCTSTR sPassword );
	CString GetLastError();
	UINT GetPort();
	CString GetServerHostName();
	CString GetUsername();
	CString GetPassword();
	BOOL Disconnect();
	BOOL Connect();
	int GetNumMessages();
	BOOL GetMessage( UINT nMsg, CMailMessage* msg);
	BOOL DeleteMessage( UINT nMsg );

private:
	BOOL get_response( UINT executed_action );

	CString m_sError;
	CString m_sResponse;
	BOOL m_bConnected;
	UINT m_nPort;
	CString m_sPOP3ServerHostName;
	CString m_sUsername;
	CString m_sPassword;
	CSocket m_wsPOP3Server;

protected:
	enum eResponse
	{
		CONNECTION = 0,
		IDENTIFICATION,
		AUTHENTIFICATION,
		STATUS,
		RETRIEVE,
		DELE,
		QUIT,
		// Include any others here
		LAST_RESPONSE	// Do not add entries past this one
	};
	TCHAR response_buf[ RESPONSE_BUFFER_SIZE ];
	static TCHAR* error_table[];

};

#endif // !defined(AFX_POP3_H__CFFD2FBB_859A_11D2_A529_444553540000__INCLUDED_)
