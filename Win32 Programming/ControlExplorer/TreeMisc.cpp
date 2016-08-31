// TreeMisc.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"

#include "NumericEdit.h"
#include "TinyButt.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "treeOps.h"
#include "tvitem.h"

#include "TreeMisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeMisc property page

IMPLEMENT_DYNCREATE(CTreeMisc, CTreeOps)

CTreeMisc::CTreeMisc() : CTreeOps(CTreeMisc::IDD)
{
        //{{AFX_DATA_INIT(CTreeMisc)
        //}}AFX_DATA_INIT
}

CTreeMisc::~CTreeMisc()
{
}

void CTreeMisc::DoDataExchange(CDataExchange* pDX)
{
        CTreeOps::DoDataExchange(pDX);

        //{{AFX_DATA_MAP(CTreeMisc)
        DDX_Control(pDX, IDC_TEXTONLY, c_TextOnly);
        DDX_Control(pDX, IDC_TOP_CAPTION, c_c_Top);
        DDX_Control(pDX, IDC_TOP, c_Top);
        DDX_Control(pDX, IDC_RIGHT_CAPTION, c_c_Right);
        DDX_Control(pDX, IDC_RIGHT, c_Right);
        DDX_Control(pDX, IDC_RECT_CAPTION, c_c_Rect);
        DDX_Control(pDX, IDC_LEFT_CAPTION, c_c_Left);
        DDX_Control(pDX, IDC_LEFT, c_Left);
        DDX_Control(pDX, IDC_GETITEMRECT, c_GetItemRect);
        DDX_Control(pDX, IDC_BOTTOM_CAPTION, c_c_Bottom);
        DDX_Control(pDX, IDC_BOTTOM, c_Bottom);
        DDX_Control(pDX, IDC_COUNT, c_Count);
        DDX_Control(pDX, IDC_CANCEL, c_Cancel);
        DDX_Control(pDX, IDC_FLAG_CAPTION, c_c_Flag);
        DDX_Control(pDX, IDC_TVGN_CARET, c_TVGN_CARET);
        DDX_Control(pDX, IDC_TVGN_DROPHILITE, c_TVGN_DROPHILITE);
        DDX_Control(pDX, IDC_TVGN_FIRSTVISIBLE, c_TVGN_FIRSTVISIBLE);
        DDX_Control(pDX, IDC_HITEM, c_hItem);
        DDX_Control(pDX, IDC_INDENT, c_Indent);
        DDX_Control(pDX, IDC_SPININDENT, c_SpinIndent);
        DDX_Control(pDX, IDC_DELETEITEM, c_DeleteItem);
        DDX_Control(pDX, IDC_EDITLABEL, c_EditLabel);
        DDX_Control(pDX, IDC_ENDEDITLABELNOW, c_EndEditLabelNow);
        DDX_Control(pDX, IDC_GETCOUNT, c_GetCount);
        DDX_Control(pDX, IDC_GETINDENT, c_GetIndent);
        DDX_Control(pDX, IDC_SELECTITEM, c_SelectItem);
        DDX_Control(pDX, IDC_SETINDENT, c_SetIndent);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreeMisc, CTreeOps)
        //{{AFX_MSG_MAP(CTreeMisc)
        ON_BN_CLICKED(IDC_DELETEITEM, OnDeleteitem)
        ON_BN_CLICKED(IDC_EDITLABEL, OnEditlabel)
        ON_BN_CLICKED(IDC_ENDEDITLABELNOW, OnEndeditlabelnow)
        ON_CBN_SELENDOK(IDC_HITEM, OnSelendokHitem)
        ON_BN_CLICKED(IDC_GETCOUNT, OnGetcount)
        ON_BN_CLICKED(IDC_GETINDENT, OnGetindent)
        ON_BN_CLICKED(IDC_SELECTITEM, OnSelectitem)
        ON_BN_CLICKED(IDC_SETINDENT, OnSetindent)
        ON_BN_CLICKED(IDC_GETITEMRECT, OnGetitemrect)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeMisc message handlers

void CTreeMisc::OnDeleteitem() 
{
 int index = c_hItem.GetCurSel();
 if(index != CB_ERR)
    { /* delete it */
     HTREEITEM item = (HTREEITEM)c_hItem.GetItemData(index);

     CString is;
     is = c_Tree->GetItemText(item);
     CString s;
     s.Format(_T("%08x: \"%s\""), item, is);

     BOOL result = c_Tree->SendMessage(TVM_DELETEITEM, 0, (LPARAM)item);

     if(result)
        loadComboFromTree(&c_hItem);

     showResult_N_s_b(IDS_TVM_DELETEITEM, s, result);
    } /* delete it */
}

/****************************************************************************
*                        CTreeMisc::loadComboFromTree
* Inputs:
*       CComboBox * box: Combo box to load
* Result: void
*       
* Effect: 
*       Calls superclass loadComboBoxFromTree, but then inserts the NULL
*       entry at the front
****************************************************************************/

void CTreeMisc::loadComboFromTree(CComboBox * box)
    {
     CTreeOps::loadComboFromTree(box, TRUE);
     CString s;
     s.LoadString(IDS_NULL);
     box->InsertString(0, s);
    }

BOOL CTreeMisc::OnSetActive() 
{
 loadComboFromTree(&c_hItem);
 return CTreeOps::OnSetActive();
}

BOOL CTreeMisc::OnInitDialog() 
{
        CTreeOps::OnInitDialog();
        
        enableControls();
        
        CRect r;
        c_Tree->GetWindowRect(&r);
        c_SpinIndent.SetRange(0, r.Width() / 4);  // random choice, max 1/4 window
        CheckRadioButton(IDC_TVGN_CARET, IDC_TVGN_FIRSTVISIBLE, IDC_TVGN_CARET);
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeMisc::OnEditlabel() 
{
 int index = c_hItem.GetCurSel();
 if(index != CB_ERR)
    { /* edit it */
     HTREEITEM item = (HTREEITEM)c_hItem.GetItemData(index);

     CString s = itemString(item);

     HWND result = (HWND)c_Tree->SendMessage(TVM_EDITLABEL, 0, (LPARAM)item);
     showResult_N_s_x(IDS_TVM_EDITLABEL, s, (DWORD)result);
    } /* edit it */
        
}

void CTreeMisc::OnEndeditlabelnow() 
{
 BOOL cancel = c_Cancel.GetCheck() == BST_CHECKED;
 BOOL result = c_Tree->SendMessage(TVM_ENDEDITLABELNOW, cancel, 0);
 showResult_b_N_b(IDS_TVM_ENDEDITLABELNOW, cancel, result);     
}

void CTreeMisc::enableControls()
{
 int index = c_hItem.GetCurSel();
 BOOL enable = (index != CB_ERR);
 BOOL root = enable && (HTREEITEM)c_hItem.GetItemData(index) == TVI_ROOT;
 BOOL isnull = c_hItem.GetItemData(index) == NULL;
 BOOL editable = c_Tree->GetStyle() & TVS_EDITLABELS;

 c_DeleteItem.EnableWindow(enable);

 c_c_Rect.EnableWindow(enable && !root && !isnull);
 c_c_Left.EnableWindow(!root && !isnull);
 c_c_Right.EnableWindow(!root && !isnull);
 c_c_Top.EnableWindow(!root && !isnull);
 c_c_Bottom.EnableWindow(!root && !isnull);
 c_Left.EnableWindow(!root && !isnull);
 c_Right.EnableWindow(!root && !isnull);
 c_Bottom.EnableWindow(!root && !isnull);
 c_Top.EnableWindow(!root && !isnull);
 c_GetItemRect.EnableWindow(enable && !root && !isnull);

 c_Cancel.EnableWindow(enable && !root && !isnull && editable);
 c_EditLabel.EnableWindow(enable && !root && !isnull && editable);
 c_EndEditLabelNow.EnableWindow(enable && !root && !isnull && editable);// TODO: make this smarter...let it detect an edit in progress...

 c_GetCount.EnableWindow(TRUE);
 c_GetIndent.EnableWindow(TRUE);

 c_SelectItem.EnableWindow(enable && !root);
 c_c_Flag.EnableWindow(enable && !root);
 c_TVGN_CARET.EnableWindow(enable && !root);
 c_TVGN_DROPHILITE.EnableWindow(enable && !root);
 c_TVGN_FIRSTVISIBLE.EnableWindow(enable && !root);

 c_SetIndent.EnableWindow(TRUE);
 c_SpinIndent.EnableWindow(TRUE);
}

void CTreeMisc::OnSelendokHitem() 
{
 enableControls();
}

void CTreeMisc::OnGetcount() 
{
 int result = c_Tree->SendMessage(TVM_GETCOUNT);
 c_Count.SetWindowText(result);
 showResult_N_N_d(IDS_TVM_GETCOUNT, result);    
}

void CTreeMisc::OnGetindent() 
{
 int result = c_Tree->SendMessage(TVM_GETINDENT);
 c_Indent.SetWindowText(result);
 showResult_N_N_d(IDS_TVM_GETINDENT, result);
}

void CTreeMisc::OnSelectitem() 
{
 int index = c_hItem.GetCurSel();
 CString flagstr;

 if(index != CB_ERR)
    { /* valid selection */
     int flag;
     if(c_TVGN_CARET.GetCheck())
        { /* caret */
         flag = TVGN_CARET;
         flagstr.LoadString(IDS_TVGN_CARET);
        } /* caret */
     else
     if(c_TVGN_DROPHILITE.GetCheck())
        { /* drop */
         flag = TVGN_DROPHILITE;
         flagstr.LoadString(IDS_TVGN_DROPHILITE);
        } /* drop */
     else
     if(c_TVGN_FIRSTVISIBLE.GetCheck())
        { /* first visible */
         flag = TVGN_FIRSTVISIBLE;
         flagstr.LoadString(IDS_TVGN_FIRSTVISIBLE);
        } /* first visible */
     else
        return;  // should never happen...

     HTREEITEM item = (HTREEITEM)c_hItem.GetItemData(index);

     BOOL result = c_Tree->SendMessage(TVM_SELECTITEM, flag, (LPARAM)item);
     
     showResult_s_s_b(IDS_TVM_SELECTITEM, flagstr, itemString(item), result);
    } /* valid selection */
}

void CTreeMisc::OnSetindent() 
{
 int indent = c_Indent.GetWindowInt();
 c_Tree->SendMessage(TVM_SETINDENT, indent, 0);
 showResult_d_N_N(IDS_TVM_SETINDENT, indent);
}

void CTreeMisc::OnGetitemrect() 
{
 CRect rc;
 BOOL textonly = c_TextOnly.GetCheck() == BST_CHECKED;

 BOOL result = c_Tree->SendMessage(TVM_GETITEMRECT, textonly, (LPARAM)&rc);

 c_Top.SetWindowText(rc.top);
 c_Left.SetWindowText(rc.left);
 c_Right.SetWindowText(rc.right);
 c_Bottom.SetWindowText(rc.bottom);

 showResult_b_R_b(IDS_TVM_GETITEMRECT, textonly, &rc, result); 
        
}
