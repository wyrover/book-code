// Pop.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Pop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// CPop Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPop::CPop()
{
	m_PopServer.Create();
}
//{4EEC1C91-6BE1-11d1-8824-00001C302581}

CPop::~CPop()
{
	m_PopServer.Close();
}

BOOL CPop::Connect(CString & Host, CString & User, CString & Password)
{
	char	buf [512];

	if (!m_PopServer.Connect(Host,110)) // 110 Pop3 Port
	{
		m_ErrorMessage = _T("Server cannot be connected");
		return FALSE;
	}
	else
	{
		if(CheckResponse(CONNECTION_CHECK)==FALSE)
			return FALSE;

		wsprintf (buf, "USER %s\r\n", (LPCSTR) User);
		m_PopServer.Send(buf, strlen (buf));
		if(CheckResponse(USER_CHECK)==FALSE)
			return FALSE;

		wsprintf (buf, "PASS %s\r\n", (LPCSTR) Password);
		m_PopServer.Send(buf, strlen (buf)); 
		if (CheckResponse(PASSWORD_CHECK)==FALSE)
			return FALSE;

		return TRUE;
	}

}

BOOL CPop::Delete(int & MsgNumber)
{
	char	buf [512];

	wsprintf (buf, "DELE %d\r\n",MsgNumber );
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(DELETE_CHECK)==FALSE)
		return FALSE;
	else
		return TRUE;
}

BOOL CPop::Disconnect()
{
	char	buf [512];

	wsprintf (buf, "QUIT \r\n");
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(QUIT_CHECK)==FALSE)
		return FALSE;
	else
		return TRUE;
}

BOOL CPop::Noop()
{
	char	buf [512];

	wsprintf (buf, "NOOP  \r\n");
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(NOOP_CHECK)==FALSE)
		return FALSE;
	else
		return TRUE;
}

// Return the Msg Size for given msg number
int CPop::GetMessageSize(int MsgNumber)
{
	if(m_SizeOfMsg.GetSize() < MsgNumber+1)
		return 0;
	else
		return m_SizeOfMsg[MsgNumber+1];
	
}

BOOL CPop::Reset()
{
	char	buf [512];

	wsprintf (buf, "RSET \r\n");
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(RSET_CHECK)==FALSE)
		return FALSE;
	else
		return TRUE;
}

// MsgContents will hold the msg body
BOOL CPop::Retrieve(int  MsgNumber)
{
	char	buf [512];

	wsprintf (buf, "RETR %d\r\n",MsgNumber );	
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(RETR_CHECK)==FALSE)
		return FALSE;
	else
		return TRUE;
}

BOOL CPop::Statistics()
{
	char	buf [512];

	wsprintf (buf, "STAT \r\n");
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(STAT_CHECK)==FALSE)
		return FALSE;
	else			
		return TRUE;	
}

CString CPop::GetMsgContents()
{
	return m_MsgContents;
}

int CPop::GetNumberOfMails()
{
	return m_NumberMail;
}

int CPop::GetTotalMailSize()
{
	return m_TotalSize;
}

BOOL CPop::Connect()
{
	Connect(m_Host, m_User, m_Password);
	return TRUE;
}

void CPop::SetHost(CString & Host)
{
	m_Host = Host;
}

CString CPop::GetHost()
{
	return m_Host;
}

void CPop::SetUser(CString & User)
{
	m_User = User;
}

CString CPop::GetUser()
{
	return m_User;
}

void CPop::SetPassword(CString & Password)
{
	m_Password = Password;
}

CString CPop::GetPassword()
{
	return m_Password;
}

BOOL CPop::CheckResponse(int ResponseType)
{
	char	buf [1000];

	for (int i=0;i<512;i++)
		buf[i]='\0';

	m_PopServer.Receive(buf, sizeof(buf));

	switch (ResponseType)
	{
		case CONNECTION_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Bad Connection");
				return FALSE;
			} 
			break;

		case USER_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Bad User Name");
				return FALSE;
			} 
			break;
		case PASSWORD_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Bad Password Name");
				return FALSE;
			}
			break;
		case QUIT_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during QUIT");
				return FALSE;
			}
			break;
		case DELETE_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during DELE");
				return FALSE;
			}
			break;
		case RSET_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during RSET");
				return FALSE;
			}
			break;
		case STAT_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during STAT");
				return FALSE;
			}
			else
			{
				BOOL EmailNumber = TRUE;
				for (char *p = buf; *p != '\0'; p++)
				{
					if (*p == '\t' || *p == ' ')
					{						
						if(EmailNumber == TRUE)
						{
							m_NumberMail = atoi(p);
							EmailNumber = FALSE;
						}
						else
						{
							m_TotalSize = atoi(p);
							return TRUE;
						}
						
						
					}
				}

			}
			break;
		case NOOP_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during NOOP");
				return FALSE;
			}
			break;

		case LIST_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during LIST");
				return FALSE;
			}
			else
			{
				m_PopServer.Receive(buf, sizeof(buf));

				for (char *p = buf; *p != '.'; p++)
					if (*p == '\t' || *p == ' ')
						m_SizeOfMsg.Add(atoi(p));
			}
			break;	
		case RETR_CHECK:
			if (strnicmp(buf,"-ERR", 4) == 0)
			{
				m_ErrorMessage = _T("Error occured during RETR");
				return FALSE;
			}
			else
			{
				char temp[9000];
				m_PopServer.Receive(temp, sizeof(temp));
				m_MsgContents = temp;
			}
			break;	
	}
	return TRUE;
}

CString CPop::GetErrorMessage()
{
	return m_ErrorMessage;
}

BOOL CPop::List()
{
	char	buf [512];

	wsprintf (buf, "LIST  \r\n");
	m_PopServer.Send(buf, strlen (buf)); 
	if (CheckResponse(LIST_CHECK)==FALSE)
		return FALSE;
	else
		return TRUE;

}
