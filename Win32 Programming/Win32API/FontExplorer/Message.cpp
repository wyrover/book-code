// Message.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CBrush NonemptyBrush;
static CBrush EmptyBrush;

/////////////////////////////////////////////////////////////////////////////
// CMessage

CMessage::CMessage()
{
 if(EmptyBrush.m_hObject == NULL)
    EmptyBrush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
 if(NonemptyBrush.m_hObject == NULL)
    NonemptyBrush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

CMessage::~CMessage()
{
}


BEGIN_MESSAGE_MAP(CMessage, CEdit)
        //{{AFX_MSG_MAP(CMessage)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessage message handlers

/****************************************************************************
*                          CMessage::SetWindowText
* Inputs:
*       LPCTSTR text: Message to display, or NULL, or MAKEINTRESOURCE of msg
* Result: void
*       
* Effect: 
*       Sets the text in the window.  If the text is of 0 length or NULL,
*       displays the background color
****************************************************************************/

void CMessage::SetWindowText(LPCTSTR text)
    {
     CString s;
     if(text == NULL)
        CEdit::SetWindowText(_T(""));
     else
     if(HIWORD(text) == 0)
        { /* MAKEINTRESOURCE */
         s.LoadString(LOWORD(text));
         CEdit::SetWindowText(s);
        } /* MAKEINTRESOURCE */
     else
        CEdit::SetWindowText(text);
    }

/****************************************************************************
*                            CMessage::OnCtlColor
* Inputs:
*       CDC * pDC: DC to modify
* Result: HBRUSH
*       Brush to use
* Effect: 
*       Handles WM_CTLCOLOREDIT routed from parent
****************************************************************************/

HBRUSH CMessage::OnCtlColor(CDC * pDC)
    {
     if(GetWindowTextLength() != 0)
        { /* nonempty */
         pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
         pDC->SetBkMode(TRANSPARENT);
         return (HBRUSH)NonemptyBrush.m_hObject;
        } /* nonempty */
     else
        return (HBRUSH)EmptyBrush.m_hObject;
     
    }
