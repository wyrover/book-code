// EditScrl.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "IntEdit.h"
#include "TinyButt.h"
#include "HexDisp.h"
#include "ixable.h"
#include "loggable.h"
#include "editops.h"
#include "EditScrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditScroll property page

IMPLEMENT_DYNCREATE(CEditScroll, CEditOps)

CEditScroll::CEditScroll() : CEditOps(CEditScroll::IDD)
{
        //{{AFX_DATA_INIT(CEditScroll)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CEditScroll::~CEditScroll()
{
}

void CEditScroll::DoDataExchange(CDataExchange* pDX)
{
        CEditOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditScroll)
        DDX_Control(pDX, IDC_SB_PAGEUP, c_SBPageUp);
        DDX_Control(pDX, IDC_SB_PAGEDOWN, c_SBPageDown);
        DDX_Control(pDX, IDC_SB_LINEUP, c_SBLineUp);
        DDX_Control(pDX, IDC_SB_LINEDOWN, c_SBLineDown);
        DDX_Control(pDX, IDC_POSY, c_PosY);
        DDX_Control(pDX, IDC_POSX, c_PosX);
        DDX_Control(pDX, IDC_SPIN_POSX, c_SpinPosX);
        DDX_Control(pDX, IDC_SPIN_POSY, c_SpinPosY);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditScroll, CEditOps)
        //{{AFX_MSG_MAP(CEditScroll)
        ON_BN_CLICKED(IDC_SB_LINEDOWN, OnSbLinedown)
        ON_BN_CLICKED(IDC_SB_LINEUP, OnSbLineup)
        ON_BN_CLICKED(IDC_SB_PAGEDOWN, OnSbPagedown)
        ON_BN_CLICKED(IDC_SB_PAGEUP, OnSbPageup)
        ON_BN_CLICKED(IDC_SCROLL, OnScroll)
        ON_BN_CLICKED(IDC_SCROLLCARET, OnScrollcaret)
        ON_BN_CLICKED(IDC_LINESCROLL, OnLinescroll)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditScroll message handlers

BOOL CEditScroll::OnInitDialog() 
{
        CEditOps::OnInitDialog();
        
        c_SpinPosX.SetRange(-50, 50);
        c_SpinPosY.SetRange(-50, 50);
        CheckRadioButton(IDC_SB_LINEDOWN, IDC_SB_PAGEUP, IDC_SB_LINEDOWN);
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditScroll::OnSbLinedown() 
{
 // Nothing for now
}

void CEditScroll::OnSbLineup() 
{
 // Nothing for now
}

void CEditScroll::OnSbPagedown() 
{
 // Nothing for now
}

void CEditScroll::OnSbPageup() 
{
 // Nothing for now
}

void CEditScroll::OnScroll() 
{
 int wParam;
 int id;
 if(c_SBLineDown.GetCheck())
    { /* line down */
     wParam = SB_LINEDOWN;
     id = IDS_SB_LINEDOWN;
    } /* line down */
 else
 if(c_SBLineUp.GetCheck())
    { /* line up */
     wParam = SB_LINEUP;
     id = IDS_SB_LINEUP;
    } /* line up */
 else
 if(c_SBPageDown.GetCheck())
    { /* page down */
     wParam = SB_PAGEDOWN;
     id = IDS_SB_PAGEDOWN;
    } /* page down */
 else
 if(c_SBPageUp.GetCheck())
    { /* page up */
     wParam = SB_PAGEUP;
     id = IDS_SB_PAGEUP;
    } /* page up */

 edit->SendMessage(EM_SCROLL, wParam);
 
 CString t;
 t.LoadString(IDS_EM_SCROLL);

 CString ids;
 ids.LoadString(id);

 CString s;
 s.Format(_T("%s(%s, 0)"), t, ids);
 addMessage(s);
}

void CEditScroll::OnScrollcaret() 
{
 edit->SendMessage(EM_SCROLLCARET);
 showResult_N_N_N(IDS_EM_SCROLLCARET);
}

void CEditScroll::OnLinescroll() 
{
 int x = c_PosX.GetVal();
 int y = c_PosY.GetVal();

 int result = edit->SendMessage(EM_LINESCROLL, x, y);
 showResult_d_d_d(IDS_EM_LINESCROLL, x, y, result);
}
