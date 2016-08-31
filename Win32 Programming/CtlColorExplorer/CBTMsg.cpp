// CBTMsg.cpp : implementation file
//

#include "stdafx.h"
#include "CtlColorExplorer.h"
#include "CBTMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCBTMsgBox

CCBTMsgBox::CCBTMsgBox()
{
 subclass   = FALSE;
 hwnd       = NULL;
 MsgBoxProc = NULL;
 hhook      = NULL;
 hbrush     = NULL;
 bkcolor    = ::GetSysColor(COLOR_3DFACE);
 textcolor  = ::GetSysColor(COLOR_WINDOWTEXT);
 bkmode     = OPAQUE;
 hfont      = NULL;
}

CCBTMsgBox::~CCBTMsgBox()
{
}


BEGIN_MESSAGE_MAP(CCBTMsgBox, CWnd)
        //{{AFX_MSG_MAP(CCBTMsgBox)
        ON_WM_CTLCOLOR()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCBTMsgBox message handlers

HBRUSH CCBTMsgBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
        pDC->SetBkColor(bkcolor);
        pDC->SetTextColor(textcolor);
        pDC->SetBkMode(bkmode);
        if(hfont != NULL)
           pWnd->SetFont(CFont::FromHandle(hfont));
        return hbrush;
}
