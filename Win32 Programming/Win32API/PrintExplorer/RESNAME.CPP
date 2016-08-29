// resname.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "resname.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceName

CResourceName::CResourceName()
{
}

CResourceName::~CResourceName()
{
}


BEGIN_MESSAGE_MAP(CResourceName, CEdit)
        //{{AFX_MSG_MAP(CResourceName)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceName message handlers

/****************************************************************************
*                          CResourceName.SetWindowText
* Inputs:
*       LPCTSTR name: Name to store, or MAKEINTRESOURCE of its ID
* Result: void
*       
* Effect: 
*       Sets a representation of the resource name to the window
****************************************************************************/

void CResourceName::SetWindowText(LPCTSTR name)
    {
     if(name == NULL)
        CEdit::SetWindowText(_T("NULL"));
     else
     if(HIWORD(name) == 0)
        { /* MAKEINTRESOURCE */
         CString s;
         s.Format(_T("%d"), LOWORD(name));
         CEdit::SetWindowText(s);
        } /* MAKEINTRESOURCE */
     else
        { /* string */
         CString s;
         s.Format(_T("\"%s\""), name);
         CEdit::SetWindowText(s);
        } /* string */
    }
