// HandleEdit.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "HandleEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHandleEdit

CHandleEdit::CHandleEdit()
{
}

CHandleEdit::~CHandleEdit()
{
}


BEGIN_MESSAGE_MAP(CHandleEdit, CEdit)
        //{{AFX_MSG_MAP(CHandleEdit)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHandleEdit message handlers

/****************************************************************************
*                          CHandleEdit.SetWindowText
* Inputs:
*       HANDLE h: Handle to be set
* Result: void
*       
* Effect: 
*       Sets a representation of the handle to the window
****************************************************************************/

void CHandleEdit::SetWindowText(HANDLE h)
    {
     if(h == NULL)
        CEdit::SetWindowText(_T("NULL"));
     else
        { /* hex handle */
         CString s;
         s.Format(_T("%08x"), h);
         CEdit::SetWindowText(s);
        } /* hex handle */
    }

/////////////////////////////////////////////////////////////////////////////
// CTextHandleEdit

CTextHandleEdit::CTextHandleEdit()
{
}

CTextHandleEdit::~CTextHandleEdit()
{
}


BEGIN_MESSAGE_MAP(CTextHandleEdit, CHandleEdit)
        //{{AFX_MSG_MAP(CTextHandleEdit)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextHandleEdit message handlers

/****************************************************************************
*                          CTextHandleEdit.SetWindowText
* Inputs:
*       HANDLE h: Handle to be set
* Result: void
*       
* Effect: 
*       Sets a representation of the handle to the window
****************************************************************************/

void CTextHandleEdit::SetWindowText(HANDLE h)
    {
     TCHAR * p;

     if(h == NULL)
        CEdit::SetWindowText(_T("NULL"));
     else
        { /* hex handle */
         CString s;
         p = (TCHAR *)::GlobalLock(h);

         s.Format(_T("%08x: \"%s\""), h, p);

         ::GlobalUnlock(h);
         CEdit::SetWindowText(s);
        } /* hex handle */
    }

/****************************************************************************
*                           CHandleEdit::Blank
* Result: void
*       
* Effect: 
*       Blanks contents
****************************************************************************/

void CHandleEdit::Blank()
    {
     CEdit::SetWindowText(_T(""));
    }
