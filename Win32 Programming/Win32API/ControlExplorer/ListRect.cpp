// ListRect.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "loggable.h"
#include "TinyButt.h"
#include "listops.h"
#include "IntEdit.h"

#include "ListRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListboxRect property page

IMPLEMENT_DYNCREATE(CListboxRect, CListOps)

CListboxRect::CListboxRect() : CListOps(CListboxRect::IDD)
{
        //{{AFX_DATA_INIT(CListboxRect)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CListboxRect::~CListboxRect()
{
}

void CListboxRect::DoDataExchange(CDataExchange* pDX)
{
        CListOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListboxRect)
        DDX_Control(pDX, IDC_SETITEMHEIGHT, c_SetitemHeight);
        DDX_Control(pDX, IDC_ULY, c_uly);
        DDX_Control(pDX, IDC_ULX, c_ulx);
        DDX_Control(pDX, IDC_SPIN_ULY, c_Spinuly);
        DDX_Control(pDX, IDC_SPIN_ULX, c_Spinulx);
        DDX_Control(pDX, IDC_SPIN_LRY, c_Spinlry);
        DDX_Control(pDX, IDC_SPIN_LRX, c_Spinlrx);
        DDX_Control(pDX, IDC_SPIN_INDEX, c_SpinIndex);
        DDX_Control(pDX, IDC_LRY, c_lry);
        DDX_Control(pDX, IDC_LRX, c_lrx);
        DDX_Control(pDX, IDC_ITEMHEIGHT, c_ItemHeight);
        DDX_Control(pDX, IDC_INDEX, c_Index);
        DDX_Control(pDX, IDC_SPIN_ITEMHEIGHT, c_SpinItemHeight);
        DDX_Control(pDX, IDC_GETITEMRECT, c_GetItemRect);
        DDX_Control(pDX, IDC_GETITEMHEIGHT, c_GetItemHeight);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListboxRect, CListOps)
        //{{AFX_MSG_MAP(CListboxRect)
        ON_BN_CLICKED(IDC_GETITEMHEIGHT, OnGetitemheight)
        ON_BN_CLICKED(IDC_GETITEMRECT, OnGetitemrect)
        ON_BN_CLICKED(IDC_SETITEMHEIGHT, OnSetitemheight)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListboxRect message handlers

BOOL CListboxRect::OnInitDialog() 
{
        CListOps::OnInitDialog();
        
        c_SpinIndex.SetRange(-1, 50);
        c_SpinIndex.SetPos(0);

        
        c_Spinulx.SetRange(0, 200); // TODO: get more meaningful numbers here
        c_Spinuly.SetRange(0, 200); // TODO: get more meaningful numbers here
        c_Spinlrx.SetRange(0, 200); // TODO: get more meaningful numbers here
        c_Spinlry.SetRange(0, 200); // TODO: get more meaningful numbers here
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CListboxRect::OnGetitemheight() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_GETITEMHEIGHT, index);
 showResult_d_N_d(IDS_LB_GETITEMHEIGHT, index, result);

 c_ItemHeight.SetVal(result);
}

void CListboxRect::OnGetitemrect() 
{
 CRect r;
 CListBox * box = getListBox();
 int index = c_Index.GetVal();

 int result = box->SendMessage(LB_GETITEMRECT, index, (LPARAM)&r);
 if(result != LB_ERR)
    { /* ok */
     c_ulx.SetVal(r.left);
     c_uly.SetVal(r.top);
     c_lrx.SetVal(r.right);
     c_lry.SetVal(r.bottom);
     showResult_d_R_LBERR(IDS_LB_GETITEMRECT, index, &r, result);
    } /* ok */
 else
    { /* error */
     showResult_d_N_LBERR(IDS_LB_GETITEMRECT, index, result);
    } /* error */
}

void CListboxRect::OnSetitemheight() 
{
 CListBox * box = getListBox();
 int height = c_ItemHeight.GetVal();
 int index = c_Index.GetVal();
 int result = box->SetItemHeight(index, height);
 showResult_d_d_LBERR(IDS_LB_SETITEMHEIGHT, index, height, result);
}
