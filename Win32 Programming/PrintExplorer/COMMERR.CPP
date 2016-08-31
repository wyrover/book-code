!// commerr.cpp : implementation file
//

#include "stdafx.h"
#include "stdlib.h"
#include "PrintExplorer.h"
#include "CommErr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CBrush ErrorBrush(RGB(255, 0, 0));
static CBrush OKBrush;


/////////////////////////////////////////////////////////////////////////////
// CCommErr

CCommErr::CCommErr()
{
 deffmt = NULL;
 if(OKBrush.m_hObject == NULL)
    OKBrush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
}

CCommErr::~CCommErr()
{
}



BEGIN_MESSAGE_MAP(CCommErr, CEdit)
        //{{AFX_MSG_MAP(CCommErr)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NumericEdit message handlers

/****************************************************************************
*                          CCommErr::SetWindowText
* Inputs:
*       int val: Value to set
* Result: void
*       
* Effect: 
*       Sets a representation of the numeric value to the window
****************************************************************************/

void CCommErr::SetWindowText(int val)
    {
     CString s;
     if(val == 0)
         s = _T("");
     else
        { /* real error */
         
         if(s.LoadString(val) == 0)
            { /* PDERR */
             if(s.LoadString(IDS_CDERR_GENERALCODES + val) == 0)
                { /* CDERR */
                 s.Format(_T("0x%08x"), val);
                } /* CDERR */
            } /* PDERR */
        } /* real error */
     CEdit::SetWindowText(s);
     ::SetWindowLong(m_hWnd, GWL_USERDATA, val);
    }

/****************************************************************************
*                            CCommErr::OnCtlColor
* Inputs:
*       CDC * pDC: DC to modify
* Result: HBRUSH
*       Brush to use
* Effect: 
*       Handles WM_CTLCOLOREDIT routed from parent
****************************************************************************/

HBRUSH CCommErr::OnCtlColor(CDC * pDC)
    {
     if(::GetWindowLong(m_hWnd, GWL_USERDATA) != 0)
        { /* error */
         pDC->SetTextColor(RGB(255, 255, 0));
         pDC->SetBkMode(TRANSPARENT);
         return (HBRUSH)ErrorBrush.m_hObject;
        } /* error */
     else
        return (HBRUSH)OKBrush.m_hObject;
     
    }
