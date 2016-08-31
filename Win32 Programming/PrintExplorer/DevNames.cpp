// DevNames.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "DevNames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHDevNames

CHDevNames::CHDevNames()
{
}

CHDevNames::~CHDevNames()
{
}


BEGIN_MESSAGE_MAP(CHDevNames, CEdit)
        //{{AFX_MSG_MAP(CHDevNames)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHDevNames message handlers

/****************************************************************************
*                          CHDevNames::SetWindowText
* Inputs:
*       HANDLE hDevNames: 
* Result: void
*       
* Effect: 
*       Formats the DEVNAMES structure into the control
****************************************************************************/

void CHDevNames::SetWindowText(HANDLE hDevNames)
    {
     if(hDevNames != NULL)
        { /* has devnames */
         CString s;
         LPDEVNAMES dn;
         dn = (LPDEVNAMES)::GlobalLock(hDevNames);
         s.Format(_T("[%s] [%s] [%s] %s"), 
                                (LPCTSTR)&((LPCSTR)dn)[dn->wDriverOffset],
                                (LPCTSTR)&((LPCSTR)dn)[dn->wDeviceOffset],
                                (LPCTSTR)&((LPCSTR)dn)[dn->wOutputOffset],
                                dn->wDefault & DN_DEFAULTPRN ? _T("Default")
                                                            : _T(""));
         ::GlobalUnlock(hDevNames);
         CEdit::SetWindowText(s);
        } /* has devnames */
     else
        { /* no devnames */
         CEdit::SetWindowText(_T("NULL"));
        } /* no devnames */
     
    }
