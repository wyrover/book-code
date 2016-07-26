// POP3.cpp: Implementation of CPOP3.
// Copyright (C) 1998 Michael Krebs
// In Adaption of Wes Clyburn's CSMTP class
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "POP3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TCHAR* CPOP3::error_table[] =
{
	"Server didn't connect",
	"Bad user name",							// IDENTIFICATION
	"Invalid username/password combination",	// AUTHENTIFICATION
	"STATus couldn't be retrieved",				// STATUS
	"RETRieve failed",							// RETRIEVE
	"Could not DELEte message",					// DELE
	"Error while QUITting"						// QUIT
};
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CPOP3::CPOP3( LPCTSTR szPOP3ServerName, UINT nPort, LPCTSTR sUsername, LPCTSTR sPassword)
{
	ASSERT( szPOP3ServerName != NULL );
	AfxSocketInit();
	m_sPOP3ServerHostName = szPOP3ServerName;
	m_nPort = nPort;
	m_sUsername = sUsername;
	m_sPassword = sPassword;
	m_bConnected = FALSE;
	m_sError = _T( "OK" );

}

CPOP3::~CPOP3()
{
	if( m_bConnected )
		Disconnect();
}


BOOL CPOP3::Connect()
{
	CString sUser;
	CString sPass;

	if( m_bConnected )
		return TRUE;

	if( !m_wsPOP3Server.Create() )
	{
		m_sError = _T( "Unable to create the socket." );
		return FALSE;
	}
	if( !m_wsPOP3Server.Connect( GetServerHostName(), GetPort() ) )
	{
		m_sError = _T( "Unable to connect to server" );
		m_wsPOP3Server.Close();
		return FALSE;
	}
	if( !get_response( CONNECTION ) )
	{
		m_wsPOP3Server.Close();
		return FALSE;
	}
	sUser.Format( "USER %s\r\n", GetUsername());
	m_wsPOP3Server.Send( (LPCTSTR)sUser, sUser.GetLength() );
	if( !get_response( IDENTIFICATION ) )
	{
		m_wsPOP3Server.Close();
		return FALSE;
	}
	sPass.Format( "PASS %s\r\n", GetPassword());
	m_wsPOP3Server.Send( (LPCTSTR)sPass, sPass.GetLength() );
	if( !get_response( AUTHENTIFICATION ) )
	{
		m_wsPOP3Server.Close();
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;
}


BOOL CPOP3::get_response( UINT executed_action )
{
	int nChars = m_wsPOP3Server.Receive( response_buf, RESPONSE_BUFFER_SIZE );
	if( nChars == SOCKET_ERROR )
	{
		m_sError = _T( "Socket Error" );
		return FALSE;
	}

	m_sResponse = response_buf;
	m_sResponse = m_sResponse.Left(nChars);

	if (m_sResponse.Left(4)=="-ERR")
	{
		m_sError=error_table[executed_action];
		return FALSE; 
	}
	return TRUE;
}


UINT CPOP3::GetPort()
{
	return m_nPort;
}


CString CPOP3::GetUsername()
{
	return m_sUsername;
}


CString CPOP3::GetPassword()
{
	return m_sPassword;
}


CString CPOP3::GetLastError()
{
	return m_sError;
}


CString CPOP3::GetServerHostName()
{
	return m_sPOP3ServerHostName;
}


void CPOP3::SetServerProperties( LPCTSTR sServerHostName, UINT nPort)
{
	ASSERT( sServerHostName != NULL );
	// Needs to be safe in non-debug too
	if( sServerHostName == NULL )
		return;
	m_sPOP3ServerHostName = sServerHostName;
	m_nPort = nPort;
}


void CPOP3::SetUserProperties( LPCTSTR sUsername, LPCTSTR sPassword )
{
	ASSERT( sUsername != NULL );
	ASSERT( sPassword != NULL );
	
	if( sUsername == NULL )
		return;
	if( sPassword == NULL )
		return;
	
	m_sUsername = sUsername;
	m_sPassword = sPassword;
}


BOOL CPOP3::Disconnect()
{
	BOOL ret;
	if( !m_bConnected )
		return TRUE;
	// Disconnect gracefully from the server and close the socket
	CString sQuit = _T( "QUIT\r\n" );
	m_wsPOP3Server.Send( (LPCTSTR)sQuit, sQuit.GetLength() );

	// No need to check return value here.
	// If it fails, the message is available with GetLastError
	ret = get_response( QUIT );
	m_wsPOP3Server.Close();

	m_bConnected = FALSE;
	return ret;
}


int CPOP3::GetNumMessages()
{
	CString sStat = _T( "STAT\r\n" );
	m_wsPOP3Server.Send( (LPCTSTR)sStat, sStat.GetLength() );

	if( !get_response( STATUS ) ) return -1;

	int pos=m_sResponse.FindOneOf("0123456789");
	if (pos<0) return -1;
	return atoi(m_sResponse.Mid(pos));

}


BOOL CPOP3::GetMessage( UINT nMsg, CMailMessage* msg)
{
	CString sMsg;
	CString sRetr;
	sRetr.Format("RETR %d\r\n",nMsg);
	m_wsPOP3Server.Send( (LPCTSTR)sRetr, sRetr.GetLength() );

	if( !get_response( RETRIEVE ) ) return FALSE;

	sMsg=m_sResponse;
	while ( sMsg.Find("\r\n.\r\n")<0 ) 
	{
		// nChars = number of bytes read
		int nChars = m_wsPOP3Server.Receive( response_buf, RESPONSE_BUFFER_SIZE );
		if ( nChars == SOCKET_ERROR ) return FALSE;
		m_sResponse=response_buf;
		sMsg+=m_sResponse.Left( nChars ); //only the first nChars bytes of response_buf are valid !
	}

	sMsg=sMsg.Mid(sMsg.Find("\r\n")+2); //first line of output is +OK
	sMsg=sMsg.Left(sMsg.GetLength()-3); //last line is always .\r\n

	int br=sMsg.Find("\r\n\r\n"); //breakpoint between header and body
	msg->m_sHeader=sMsg.Left(br);
	msg->m_sBody=sMsg.Mid(br+4);
	msg->DecodeHeader();
	msg->DecodeBody();
	return TRUE;
}

BOOL CPOP3::DeleteMessage( UINT nMsg )
{
	CString sDele = _T( "STAT\r\n" );
	sDele.Format("DELE %d\r\n",nMsg);
	m_wsPOP3Server.Send( (LPCTSTR)sDele, sDele.GetLength() );

	return get_response( DELE );
}

