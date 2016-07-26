// SMTP.cpp: implementation of the CSMTP class.
// Copyright (c) 1998, Wes Clyburn
//
// Adapted to modified CMailMessage class
// Copyright (c) 1998 Michael Krebs
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMTP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Static member initializers
//

// Note: the order of the entries is important.
//       They must be synchronized with eResponse entries. 
CSMTP::response_code CSMTP::response_table[] =
{
	{ 250, "SMTP server error" },						// GENERIC_SUCCESS
	{ 220, "SMTP server not available" },				// CONNECT_SUCCESS
	{ 354, "SMTP server not ready for data" },			// DATA_SUCCESS
	{ 221, "SMTP server didn't terminate session" }  	// QUIT_SUCCESS
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSMTP::CSMTP( LPCTSTR szSMTPServerName, UINT nPort )
{
	ASSERT( szSMTPServerName != NULL );
	AfxSocketInit();
	m_sMailerName = _T( "WC Mail" );
	m_sSMTPServerHostName = szSMTPServerName;
	m_nPort = nPort;
	m_bConnected = FALSE;
	m_sError = _T( "OK" );
}

CSMTP::~CSMTP()
{
	if( m_bConnected )
		Disconnect();
}

CString CSMTP::GetServerHostName()
{
	return m_sSMTPServerHostName;
}

BOOL CSMTP::Connect()
{
	CString sHello;
	TCHAR local_host[ 80 ];	// Warning: arbitrary size
	if( m_bConnected )
		return TRUE;

	if( !m_wsSMTPServer.Create() )
	{
		m_sError = _T( "Unable to create the socket." );
		return FALSE;
	}
	if( !m_wsSMTPServer.Connect( GetServerHostName(), GetPort() ) )
	{
		m_sError = _T( "Unable to connect to server" );
		m_wsSMTPServer.Close();
		return FALSE;
	}
	if( !get_response( CONNECT_SUCCESS ) )
	{
		m_sError = _T( "Server didn't respond." );
		m_wsSMTPServer.Close();
		return FALSE;
	}
	gethostname( local_host, 80 );
	sHello.Format( "HELO %s\r\n", local_host );
	m_wsSMTPServer.Send( (LPCTSTR)sHello, sHello.GetLength() );
	if( !get_response( GENERIC_SUCCESS ) )
	{
		m_wsSMTPServer.Close();
		return FALSE;
	}
	m_bConnected = TRUE;
	return TRUE;
}

BOOL CSMTP::Disconnect()
{
	BOOL ret;
	if( !m_bConnected )
		return TRUE;
	// Disconnect gracefully from the server and close the socket
	CString sQuit = _T( "QUIT\r\n" );
	m_wsSMTPServer.Send( (LPCTSTR)sQuit, sQuit.GetLength() );

	// No need to check return value here.
	// If it fails, the message is available with GetLastError
	ret = get_response( QUIT_SUCCESS );
	m_wsSMTPServer.Close();

	m_bConnected = FALSE;
	return ret;
}

UINT CSMTP::GetPort()
{
	return m_nPort;
}

CString CSMTP::GetMailerName()
{
	return m_sMailerName;
}

CString CSMTP::GetLastError()
{
	return m_sError;
}

BOOL CSMTP::SendMessage(CMailMessage * msg)
{
	ASSERT( msg != NULL );
	if( !m_bConnected )
	{
		m_sError = _T( "Must be connected" );
		return FALSE;
	}
	if( FormatMailMessage( msg ) == FALSE )
	{
		return FALSE;
	}
	if( transmit_message( msg ) == FALSE )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSMTP::FormatMailMessage( CMailMessage* msg )
{
	ASSERT( msg != NULL );
	if( msg->EncodeHeader() == FALSE )
	{
		return FALSE;
	}

	msg->EncodeBody();

	// Append a CR/LF to body if necessary.
	if( msg->m_sBody.Right( 2 ) != "\r\n" )
		msg->m_sBody += "\r\n";
	return TRUE;
}

void CSMTP::SetServerProperties( LPCTSTR sServerHostName, UINT nPort)
{
	ASSERT( sServerHostName != NULL );
	// Needs to be safe in non-debug too
	if( sServerHostName == NULL )
		return;
	m_sSMTPServerHostName = sServerHostName;
	m_nPort = nPort;
}


BOOL CSMTP::transmit_message(CMailMessage * msg)
{
	CString sFrom;
	CString sTo;
	CString sTemp;
	CString sEmail;

	ASSERT( msg != NULL );
	if( !m_bConnected )
	{
		m_sError = _T( "Must be connected" );
		return FALSE;
	}

	// Send the MAIL command
	//
	sFrom.Format( "MAIL From: <%s>\r\n", (LPCTSTR)msg->m_sFrom );
	m_wsSMTPServer.Send( (LPCTSTR)sFrom, sFrom.GetLength() );
	if( !get_response( GENERIC_SUCCESS ) )
		return FALSE;
	
	// Send RCPT commands (one for each recipient)
	//
	for( int i = 0; i < msg->GetNumRecipients(); i++ )
	{
		msg->GetRecipient( sEmail, sTemp, i );
		sTo.Format( "RCPT TO: <%s>\r\n", (LPCTSTR)sEmail );
		m_wsSMTPServer.Send( (LPCTSTR)sTo, sTo.GetLength() );
		get_response( GENERIC_SUCCESS );
	}

	// Send the DATA command
	sTemp = "DATA\r\n";
	m_wsSMTPServer.Send( (LPCTSTR)sTemp, sTemp.GetLength() );
	if( !get_response( DATA_SUCCESS ) )
	{
		return FALSE;
	}
	// Send the header
	//
	m_wsSMTPServer.Send( (LPCTSTR)msg->m_sHeader, msg->m_sHeader.GetLength() );

	//Insert additional headers here !
	sTemp="X-Mailer: CSMTP class for MFC\r\n";
	m_wsSMTPServer.Send( (LPCTSTR)sTemp, sTemp.GetLength() );

	//Empty line
	sTemp="\r\n";
	m_wsSMTPServer.Send( (LPCTSTR)sTemp, sTemp.GetLength() );

	// Send the body
	//
	m_wsSMTPServer.Send( (LPCTSTR)msg->m_sBody, msg->m_sBody.GetLength() );

	// Signal end of data
	//
	sTemp = "\r\n.\r\n";
	m_wsSMTPServer.Send( (LPCTSTR)sTemp, sTemp.GetLength() );
	if( !get_response( GENERIC_SUCCESS ) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSMTP::get_response( UINT response_expected )
{
	ASSERT( response_expected >= GENERIC_SUCCESS );
	ASSERT( response_expected < LAST_RESPONSE );

	CString sResponse;
	UINT response;
	response_code* pResp;	// Shorthand

	if( m_wsSMTPServer.Receive( response_buf, RESPONSE_BUFFER_SIZE ) == SOCKET_ERROR )
	{
		m_sError = _T( "Socket Error" );
		return FALSE;
	}
	sResponse = response_buf;
	sscanf( (LPCTSTR)sResponse.Left( 3 ), "%d", &response );
	pResp = &response_table[ response_expected ];
	if( response != pResp->nResponse )
	{
		m_sError.Format( "%d:%s", response, (LPCTSTR)pResp->sMessage );
		return FALSE;
	}
	return TRUE;
}


