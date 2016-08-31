// MFH.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "MFH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFH dialog


CMFH::CMFH(CWnd* pParent /*=NULL*/)
        : CDialog(CMFH::IDD, pParent)
{
        //{{AFX_DATA_INIT(CMFH)
        m_inch = 0;
        m_bottom = 0;
        m_left = 0;
        m_right = 0;
        m_top = 0;
        m_lastbottom = 0;
        m_lastinch = 0;
        m_lastleft = 0;
        m_lastright = 0;
        m_lasttop = 0;
        m_OldPelX = 0;
        m_OldPelY = 0;
        m_PelX = 0;
        m_PelY = 0;
        m_SetOrigin = FALSE;
        //}}AFX_DATA_INIT
}


void CMFH::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMFH)
        DDX_Text(pDX, IDC_INCH, m_inch);
        DDX_Text(pDX, IDC_BOTTOM, m_bottom);
        DDX_Text(pDX, IDC_LEFT, m_left);
        DDX_Text(pDX, IDC_RIGHT, m_right);
        DDX_Text(pDX, IDC_TOP, m_top);
        DDX_Text(pDX, IDC_OLDBOTTOM, m_lastbottom);
        DDX_Text(pDX, IDC_OLDINCH, m_lastinch);
        DDX_Text(pDX, IDC_OLDLEFT, m_lastleft);
        DDX_Text(pDX, IDC_OLDRIGHT, m_lastright);
        DDX_Text(pDX, IDC_OLDTOP, m_lasttop);
        DDX_Text(pDX, IDC_OLDPELX, m_OldPelX);
        DDX_Text(pDX, IDC_OLDPELY, m_OldPelY);
        DDX_Text(pDX, IDC_PELX, m_PelX);
        DDX_Text(pDX, IDC_PELY, m_PelY);
        DDX_Check(pDX, IDC_SETORIGIN, m_SetOrigin);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMFH, CDialog)
        //{{AFX_MSG_MAP(CMFH)
        ON_BN_CLICKED(IDC_USEOLD, OnUseold)
        ON_BN_CLICKED(IDC_USEOLDPELXY, OnUseoldpelxy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFH message handlers

void CMFH::OnUseold() 
{
 m_left = m_lastleft;
 m_right = m_lastright;
 m_top = m_lasttop;
 m_bottom = m_lastbottom;
 m_inch = m_lastinch;
 UpdateData(FALSE);
        
}

void CMFH::OnUseoldpelxy() 
{
 m_PelX = m_OldPelX;
 m_PelY = m_OldPelY;
        
}
