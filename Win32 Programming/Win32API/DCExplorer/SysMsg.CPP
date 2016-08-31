// SysMsg.cpp : implementation file
//

#include "stdafx.h"
#include "Message.h"
#include "SysMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemMessage

CSystemMessage::CSystemMessage()
{
}

CSystemMessage::~CSystemMessage()
{
}


BEGIN_MESSAGE_MAP(CSystemMessage, CMessage)
	//{{AFX_MSG_MAP(CSystemMessage)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemMessage message handlers

/****************************************************************************
*                          CSystemMessage::SetWindowText
* Inputs:
*       DWORD error: Error code, or 0 for no error
* Result: void
*       
* Effect: 
*       Sets the text of the error in the window.  If the error code is 0,
*	or ::FormatSystemMessage fails, displays the background color
****************************************************************************/

void CSystemMessage::SetWindowText(DWORD error)
    {
     if(error == 0)
        { /* blank out */
	 CMessage::SetWindowText(NULL);
	 return;
	} /* blank out */
     
     LPTSTR s;

     if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		 FORMAT_MESSAGE_FROM_SYSTEM,
		 NULL, 		// source ignored for FROM_SYSTEM
		 error,		// error code
		 0, 		// default language ID
		 (LPTSTR)&s, 	// pointer to pointer to buffer
		 0,		// Minimum buffer length
		 NULL) 		// arg array
		     == 0)
	{ /* failed */
	 // NYI: handle recursive error condition
	 CMessage::SetWindowText(NULL); // for now, clear it
	} /* failed */
     else
	{ /* success */
	 // Convert any control characters (such as \r or \n) to spaces
	 for(int i = 0; s[i] != _T('\0'); i++)
	    if(s[i] < _T(' '))
	       s[i] = _T(' ');

	 CMessage::SetWindowText(s);
	 LocalFree(s);
	} /* success */
    }
