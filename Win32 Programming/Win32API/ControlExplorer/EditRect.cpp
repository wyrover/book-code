// EditRect.cpp : implementation file
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
#include "EditRect.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditRect property page

IMPLEMENT_DYNCREATE(CEditRect, CEditOps)

CEditRect::CEditRect() : CEditOps(CEditRect::IDD)
{
        //{{AFX_DATA_INIT(CEditRect)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CEditRect::~CEditRect()
{
}

void CEditRect::DoDataExchange(CDataExchange* pDX)
{
        CEditOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditRect)
        DDX_Control(pDX, IDC_EC_USEFONTINFO, c_ECUseFontInfo);
        DDX_Control(pDX, IDC_EC_RIGHTMARGIN, c_ECRightMargin);
        DDX_Control(pDX, IDC_EC_LEFTMARGIN, c_ECLeftMargin);
        DDX_Control(pDX, IDC_ULY, c_uly);
        DDX_Control(pDX, IDC_ULX, c_ulx);
        DDX_Control(pDX, IDC_SPIN_ULY, c_Spinuly);
        DDX_Control(pDX, IDC_SPIN_ULX, c_Spinulx);
        DDX_Control(pDX, IDC_SPIN_RIGHT, c_SpinRight);
        DDX_Control(pDX, IDC_SPIN_LRY, c_Spinlry);
        DDX_Control(pDX, IDC_SPIN_LRX, c_Spinlrx);
        DDX_Control(pDX, IDC_SPIN_LEFT, c_SpinLeft);
        DDX_Control(pDX, IDC_SETRECTNP, c_SetRectNP);
        DDX_Control(pDX, IDC_SETRECT, c_SetRect);
        DDX_Control(pDX, IDC_SETMARGINS, c_SetMargins);
        DDX_Control(pDX, IDC_RIGHT, c_Right);
        DDX_Control(pDX, IDC_MARGIN_CAPTION, c_MarginCaption);
        DDX_Control(pDX, IDC_LRY, c_lry);
        DDX_Control(pDX, IDC_LRX, c_lrx);
        DDX_Control(pDX, IDC_LEFT, c_Left);
        DDX_Control(pDX, IDC_GETRECT, c_GetRect);
        DDX_Control(pDX, IDC_GETMARGINS, c_GetMargins);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditRect, CEditOps)
        //{{AFX_MSG_MAP(CEditRect)
        ON_BN_CLICKED(IDC_GETRECT, OnGetrect)
        ON_BN_CLICKED(IDC_GETMARGINS, OnGetmargins)
        ON_BN_CLICKED(IDC_SETMARGINS, OnSetmargins)
        ON_BN_CLICKED(IDC_SETRECT, OnSetrect)
        ON_BN_CLICKED(IDC_SETRECTNP, OnSetrectnp)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditRect message handlers

void CEditRect::OnGetrect() 
{
 CRect r;
 edit->SendMessage(EM_GETRECT, 0, (LPARAM)&r);

 CString t;
 t.LoadString(IDS_EM_GETRECT);
 
 CString s;
 s.Format(_T("%s(0, &r) => {%d, %d, %d, %d}"), 
                        t, r.left, r.top, r.right, r.bottom);
 addMessage(s);
        
 c_ulx.SetVal(r.left);
 c_uly.SetVal(r.top);
 c_lrx.SetVal(r.right);
 c_lry.SetVal(r.bottom);
}

void CEditRect::OnGetmargins() 
{
 DWORD margins = edit->SendMessage(EM_GETMARGINS);

 showResult_N_N_DW(IDS_EM_GETMARGINS, margins);
 c_Left.SetVal(LOWORD(margins));
 c_Right.SetVal(HIWORD(margins));
}

void CEditRect::OnSetmargins() 
{
 DWORD margins = MAKELONG(c_Left.GetVal(), c_Right.GetVal());

 CString t;
 t.LoadString(IDS_EM_SETMARGINS);
 
 WPARAM flags = 0;

 if(c_ECLeftMargin.GetCheck())
    flags |= EC_LEFTMARGIN;
 if(c_ECRightMargin.GetCheck())
    flags |= EC_RIGHTMARGIN;
 if(c_ECUseFontInfo.GetCheck())
    flags |= EC_USEFONTINFO;

 CString s;
 s.Format(_T("%s(%d, 0x%08x=(%d, %d))"), t, flags, margins, LOWORD(margins),
                                                        HIWORD(margins));
 addMessage(s);
        
}

/****************************************************************************
*                              CEditRect::doRect
* Inputs:
*       int msg: message to send
*       int id: ID to display
* Result: void
*       
* Effect: 
*       Does the appropriate EM_SETRECT[NP] command
****************************************************************************/

void CEditRect::doRect(int msg, int id)
    {
     CRect r;
     r.left = c_ulx.GetVal();
     r.top  = c_uly.GetVal();
     r.right = c_lrx.GetVal();
     r.bottom = c_lry.GetVal();

     edit->SendMessage(msg, 0, (LPARAM)&r);
     
     CString t;
     t.LoadString(id);

     CString s;
     s.Format(_T("%s(0, {%d, %d, %d, %d})"), t, r.left, r.top, r.right, r.bottom);
     addMessage(s);
    }

void CEditRect::OnSetrect() 
{
 doRect(EM_SETRECT, IDS_EM_SETRECT);
        
}

void CEditRect::OnSetrectnp() 
{
 doRect(EM_SETRECTNP, IDS_EM_SETRECTNP);
        
}

BOOL CEditRect::OnInitDialog() 
{
        CEditOps::OnInitDialog();
        
        if(Is3())
           { /* no margins */
            c_Right.EnableWindow(FALSE);
            c_MarginCaption.EnableWindow(FALSE);
            c_Left.EnableWindow(FALSE);
            c_SpinLeft.EnableWindow(FALSE);
            c_SpinRight.EnableWindow(FALSE);
            c_SetMargins.EnableWindow(FALSE);
            c_GetMargins.EnableWindow(FALSE);
            c_ECLeftMargin.EnableWindow(FALSE);
            c_ECRightMargin.EnableWindow(FALSE);
            c_ECUseFontInfo.EnableWindow(FALSE);
           } /* no margins */
        else
           { /* has margins */
            c_ECLeftMargin.SetCheck(TRUE);
            c_ECRightMargin.SetCheck(TRUE);
           } /* has margins */

        c_SpinLeft.SetRange(-50, 50);
        c_SpinRight.SetRange(-50, 50);

        CRect r;
        edit->GetClientRect(&r);

#define slop 20 
        c_Spinulx.SetRange(-slop, r.Width() + slop);
        c_Spinuly.SetRange(-slop, r.Height() + slop);
        c_Spinlrx.SetRange(-slop, r.Width() + slop);
        c_Spinlry.SetRange(-slop, r.Height() + slop);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
