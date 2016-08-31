// PropBut.cpp : implementation file
//

#include "stdafx.h"
#include "HeapExplorer.h"
#include "PropBut.h"
#include "infomsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropButton

CPropButton::CPropButton()
{
}

CPropButton::~CPropButton()
{
}


BEGIN_MESSAGE_MAP(CPropButton, CButton)
        //{{AFX_MSG_MAP(CPropButton)
        ON_WM_RBUTTONDOWN()
        ON_WM_RBUTTONUP()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropButton message handlers

void CPropButton::OnRButtonDown(UINT nFlags, CPoint point) 
{
        SetCapture();
        GetParent()->SendMessage(UWM_SHOW_INFO, GetDlgCtrlID());
        
        CButton::OnRButtonDown(nFlags, point);
}

void CPropButton::OnRButtonUp(UINT nFlags, CPoint point) 
{
        if(GetCapture() == this)
           { /* had it */
            ReleaseCapture();
            GetParent()->SendMessage(UWM_RELEASE_INFO);
           } /* had it */
        
        CButton::OnRButtonUp(nFlags, point);
}
