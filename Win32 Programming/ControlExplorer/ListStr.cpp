// ListStr.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "listops.h"
#include "TinyButt.h"
#include "IntEdit.h"
#include "ListStr.h"
#include "xform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListStrings property page

IMPLEMENT_DYNCREATE(CListStrings, CListOps)

CListStrings::CListStrings() : CListOps(CListStrings::IDD)
{
        //{{AFX_DATA_INIT(CListStrings)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CListStrings::~CListStrings()
{
}

void CListStrings::DoDataExchange(CDataExchange* pDX)
{
        CListOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListStrings)
        DDX_Control(pDX, IDC_SPIN_FIND_START, c_SpinFindStart);
        DDX_Control(pDX, IDC_FIND_START, c_FindStart);
        DDX_Control(pDX, IDC_GETTEXTLENGTH, c_GetTextLength);
        DDX_Control(pDX, IDC_GETTEXT, c_GetText);
        DDX_Control(pDX, IDC_GETSEL, c_GetSel);
        DDX_Control(pDX, IDC_GETITEMHEIGHT, c_GetItemHeight);
        DDX_Control(pDX, IDC_GETITEMDATA, c_GetItemData);
        DDX_Control(pDX, IDC_FINDSTRINGEXACT, c_FindStringExact);
        DDX_Control(pDX, IDC_FINDSTRING, c_FindString);
        DDX_Control(pDX, IDC_DELETESTRING, c_DeleteString);
        DDX_Control(pDX, IDC_ADDSTRING, c_AddString);
        DDX_Control(pDX, IDC_STRING, c_String);
        DDX_Control(pDX, IDC_SPIN_INDEX, c_SpinIndex);
        DDX_Control(pDX, IDC_SETSEL, c_SetSel);
        DDX_Control(pDX, IDC_SETITEMDATA, c_SetItemData);
        DDX_Control(pDX, IDC_SETCURSEL, c_SetCurSel);
        DDX_Control(pDX, IDC_SELECTSTRING, c_SelectString);
        DDX_Control(pDX, IDC_INDEX, c_Index);
        DDX_Control(pDX, IDC_SETSELVAL, c_SetSelVal);
        DDX_Control(pDX, IDC_ITEMDATAVAL, c_ItemDataVal);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListStrings, CListOps)
        //{{AFX_MSG_MAP(CListStrings)
        ON_BN_CLICKED(IDC_ADDSTRING, OnAddstring)
        ON_BN_CLICKED(IDC_DELETESTRING, OnDeletestring)
        ON_BN_CLICKED(IDC_FINDSTRING, OnFindstring)
        ON_BN_CLICKED(IDC_FINDSTRINGEXACT, OnFindstringexact)
        ON_BN_CLICKED(IDC_GETITEMDATA, OnGetitemdata)
        ON_BN_CLICKED(IDC_GETITEMHEIGHT, OnGetitemheight)
        ON_BN_CLICKED(IDC_GETSEL, OnGetsel)
        ON_BN_CLICKED(IDC_GETTEXT, OnGettext)
        ON_BN_CLICKED(IDC_GETTEXTLENGTH, OnGettextlength)
        ON_BN_CLICKED(IDC_SELECTSTRING, OnSelectstring)
        ON_BN_CLICKED(IDC_SETCURSEL, OnSetcursel)
        ON_BN_CLICKED(IDC_SETITEMDATA, OnSetitemdata)
        ON_BN_CLICKED(IDC_SETSEL, OnSetsel)
        ON_BN_CLICKED(IDC_INSERTSTRING, OnInsertstring)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListStrings message handlers

BOOL CListStrings::OnSetActive() 
{
        return CListOps::OnSetActive();
}

BOOL CListStrings::OnInitDialog() 
{
        CListOps::OnInitDialog();
        
        c_SpinIndex.SetRange(-1, 50);
        c_SpinIndex.SetPos(0);
        
        c_SpinFindStart.SetRange(-1, 50);
        c_SpinFindStart.SetPos(-1);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CListStrings::OnAddstring() 
{
 CString s;
 c_String.GetWindowText(s);
 s = transform(s);

 CListBox * box = getListBox();
 int result = box->AddString(s);
 showResult_N_s_LBERR(IDS_LB_ADDSTRING, _T("\"...\""), result);
        
}

void CListStrings::OnDeletestring() 
{
 CListBox * box = getListBox();
 int item = c_Index.GetVal();
 int result = box->DeleteString(item);
 showResult_d_N_LBERR(IDS_LB_DELETESTRING, item, result);
}

void CListStrings::doFind(int msg, int id)
{
 CString s;
 c_String.GetWindowText(s);
 s = transform(s);

 CListBox * box = getListBox();

 int start = c_FindStart.GetVal();

 int result = box->SendMessage(msg, start, (LPARAM)(LPCTSTR)s);
 showResult_d_s_LBERR(id, start, _T("\"...\""), result);
}

void CListStrings::OnFindstring() 
{
 doFind(LB_FINDSTRING, IDS_LB_FINDSTRING);
}

void CListStrings::OnFindstringexact() 
{
 doFind(LB_FINDSTRINGEXACT, IDS_LB_FINDSTRINGEXACT);
        
}

void CListStrings::OnGetitemdata() 
{
 CListBox * box = getListBox();

 int index = c_Index.GetVal();
 
 DWORD result = box->GetItemData(index);

 showResult_d_N_d(IDS_LB_GETITEMDATA, index, result);

 c_ItemDataVal.SetVal(result);
        
}

void CListStrings::OnGetitemheight() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_GETITEMHEIGHT, index);
 showResult_d_N_d(IDS_LB_GETITEMHEIGHT, index, result);
}


void CListStrings::OnGetsel() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_GETSEL, index);

 showResult_d_N_d(IDS_LB_GETSEL, index, result);
        
}

void CListStrings::OnGettext() 
{
 CListBox * box = getListBox();
 CString s;
 int index = c_Index.GetVal();
 box->GetText(index, s);

 // Note that escape character such as " are not properly quoted.  There
 // was no strong motivation to do this.
 CString text;
 text.Format(_T("\"%s\""), s);

 showResult_d_B_s(IDS_LB_GETTEXT, index, text);
 c_String.SetWindowText(s);
}

void CListStrings::OnGettextlength() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_GETTEXTLEN, index);
 showResult_d_N_d(IDS_LB_GETTEXTLEN, index, result);
}


void CListStrings::OnSelectstring() 
{
 doFind(LB_SELECTSTRING, IDS_LB_SELECTSTRING);
}

void CListStrings::OnSetcursel() 
{
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_SETCURSEL, index);
 showResult_d_N_d(IDS_LB_SETCURSEL, index, result);
        
}

void CListStrings::OnSetitemdata() 
{
 CListBox * box = getListBox();

 int index = c_Index.GetVal();
 
 DWORD itemdata = c_ItemDataVal.GetVal();

 DWORD result = box->SetItemData(index, itemdata);

 showResult_d_d_LBERR(IDS_LB_SETITEMDATA, index, itemdata, result);

}


void CListStrings::OnSetsel() 
{
 BOOL val = c_SetSelVal.GetCheck();
 CListBox * box = getListBox();
 int index = c_Index.GetVal();
 int result = box->SendMessage(LB_SETSEL, val, index);

 showResult_d_d_LBERR(IDS_LB_SETSEL, val, index, result);
        
}

void CListStrings::OnInsertstring() 
{
 CString s;
 c_String.GetWindowText(s);
 s = transform(s);

 int index = c_Index.GetVal();

 CListBox * box = getListBox();
 int result = box->InsertString(index, s);
 showResult_d_s_LBERR(IDS_LB_INSERTSTRING, index, _T("\"...\""), result);
        
}
