// UDZ.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "UDZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZ

CZ::CZ()
{
 wnd = NULL;
}

CZ::~CZ()
{
}


BEGIN_MESSAGE_MAP(CZ, CStatic)
        //{{AFX_MSG_MAP(CZ)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZ message handlers

/****************************************************************************
*                                  CZ::setZ
* Inputs:
*       CWnd * hwnd: Window to be saved
*       CWnd * hcaption: Its caption window
* Result: void
*       
* Effect: 
*       Initializes the data structure appropriately
****************************************************************************/

void CZ::setZ(CWnd * hwnd, CWnd * hcaption)
    {
     wnd = hwnd;
     cap = hcaption;
     hwnd->GetWindowRect(&rect);
     hwnd->GetParent()->ScreenToClient(&rect);
     hcaption->GetWindowText(caption);
    }

/****************************************************************************
*                                 CZ::enable
* Inputs:
*       HWND desired: Desired window
* Result: void
*       
* Effect: 
*       If the current window is the desired window, enable it and its caption
*       else disable it.
****************************************************************************/

void CZ::enable(HWND desired)
    {
     BOOL enable = (desired != NULL && wnd->m_hWnd == desired);
     cap->EnableWindow(enable);
     wnd->EnableWindow(enable);
    }

/****************************************************************************
*                                CZ::operator=
* Inputs:
*       const CZ& src: Source to copy
* Result: CZ&
*       Reference to 'this'
* Effect: 
*       Copies the member variables required to shift the contents without
*       shifting the entire object, in particular, we leave all the CWnd
*       members alone.  
****************************************************************************/

CZ& CZ::operator=(const CZ& src)
    {
     wnd = src.wnd;
     cap = src.cap;
     caption = src.caption;
     SetWindowText(caption);
     rect = src.rect;
     
     return *this;
    }
