// SMTP.h: interface for the CSMTP class.
// Copyright (c) 1998, Wes Clyburn
//
// Adapted to modified CMailMessage class
// Copyright (c) 1998 Michael Krebs
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMTP_H__55DE48CB_BEA4_11D1_870E_444553540000__INCLUDED_)
#define AFX_SMTP_H__55DE48CB_BEA4_11D1_870E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxsock.h>
#include "MailMessage.h"

#define SMTP_PORT 25		// Standard port for SMTP servers
#define RESPONSE_BUFFER_SIZE 1024

class CSMTP  
{
public:
	CSMTP( LPCTSTR szSMTPServerName, UINT nPort = SMTP_PORT );
	virtual ~CSMTP();

	void SetServerProperties( LPCTSTR sServerHostName, UINT nPort = SMTP_PORT );
	CString GetLastError();
	CString GetMailerName();
	UINT GetPort();
	BOOL Disconnect();
	BOOL Connect();
	virtual BOOL FormatMailMessage( CMailMessage* msg );
	BOOL SendMessage( CMailMessage* msg );
	CString GetServerHostName();

private:
	BOOL get_response( UINT response_expected );
	BOOL transmit_message( CMailMessage* msg );

	CString m_sError;
	BOOL m_bConnected;
	UINT m_nPort;
	CString m_sMailerName;
	CString m_sSMTPServerHostName;
	CSocket m_wsSMTPServer;
protected:

//
// Helper Code
//
	struct response_code
	{
		UINT nResponse;		// Response we're looking for
		TCHAR* sMessage;	// Error message if we don't get it
	};

	enum eResponse
	{
		GENERIC_SUCCESS = 0,
		CONNECT_SUCCESS,
		DATA_SUCCESS,
		QUIT_SUCCESS,
		// Include any others here
		LAST_RESPONSE	// Do not add entries past this one
	};
	TCHAR response_buf[ RESPONSE_BUFFER_SIZE ];
	static response_code response_table[];
};

#endif // !defined(AFX_SMTP_H__55DE48CB_BEA4_11D1_870E_444553540000__INCLUDED_)
