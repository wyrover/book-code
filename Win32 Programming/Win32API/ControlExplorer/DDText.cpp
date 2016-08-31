// DDText.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "loggable.h"
#include "intedit.h"
#include "tinybutt.h"
#include "comboOps.h"
#include "DDText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))

/////////////////////////////////////////////////////////////////////////////
// CDDText property page

IMPLEMENT_DYNCREATE(CDDText, CComboOps)

CDDText::CDDText() : CComboOps(CDDText::IDD)
{
        //{{AFX_DATA_INIT(CDDText)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CDDText::~CDDText()
{
}

void CDDText::DoDataExchange(CDataExchange* pDX)
{
        CComboOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDDText)
        DDX_Control(pDX, IDC_STRING, c_String);
        DDX_Control(pDX, IDC_ITEMDATA, c_ItemData);
        DDX_Control(pDX, IDC_ADDSTRING, c_AddString);
        DDX_Control(pDX, IDC_INSERTSTRING, c_InsertString);
        DDX_Control(pDX, IDC_GETLBTEXTLENGTH, c_GetLBTextLength);
        DDX_Control(pDX, IDC_GETLBTEXT, c_GetLBText);
        DDX_Control(pDX, IDC_GETITEMHEIGHT, c_GetItemHeight);
        DDX_Control(pDX, IDC_GETITEMDATA, c_GetItemData);
        DDX_Control(pDX, IDC_GETEDITSEL, c_GetEditSel);
        DDX_Control(pDX, IDC_FINDSTRINGEXACT, c_FindStringExact);
        DDX_Control(pDX, IDC_FINDSTRING, c_FindString);
        DDX_Control(pDX, IDC_SPINSELSTART, c_SpinSelStart);
        DDX_Control(pDX, IDC_SPINSELEND, c_SpinSelEnd);
        DDX_Control(pDX, IDC_SPINSELECTION, c_SpinSelection);
        DDX_Control(pDX, IDC_SPINITEMHEIGHT, c_SpinItemHeight);
        DDX_Control(pDX, IDC_SETITEMHEIGHT, c_SetItemHeight);
        DDX_Control(pDX, IDC_SETITEMDATA, c_SetItemData);
        DDX_Control(pDX, IDC_SETEDITSEL, c_SetEditSel);
        DDX_Control(pDX, IDC_SELSTART, c_SelStart);
        DDX_Control(pDX, IDC_SELEND, c_SelEnd);
        DDX_Control(pDX, IDC_SELECTION, c_Selection);
        DDX_Control(pDX, IDC_ITEMHEIGHT, c_ItemHeight);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDDText, CComboOps)
        //{{AFX_MSG_MAP(CDDText)
        ON_BN_CLICKED(IDC_FINDSTRING, OnFindstring)
        ON_BN_CLICKED(IDC_FINDSTRINGEXACT, OnFindstringexact)
        ON_BN_CLICKED(IDC_GETEDITSEL, OnGeteditsel)
        ON_BN_CLICKED(IDC_GETITEMDATA, OnGetitemdata)
        ON_BN_CLICKED(IDC_GETITEMHEIGHT, OnGetitemheight)
        ON_BN_CLICKED(IDC_GETLBTEXT, OnGetlbtext)
        ON_BN_CLICKED(IDC_GETLBTEXTLENGTH, OnGetlbtextlength)
        ON_BN_CLICKED(IDC_INSERTSTRING, OnInsertstring)
        ON_BN_CLICKED(IDC_SETEDITSEL, OnSeteditsel)
        ON_BN_CLICKED(IDC_SETITEMDATA, OnSetitemdata)
        ON_BN_CLICKED(IDC_SETITEMHEIGHT, OnSetitemheight)
        ON_BN_CLICKED(IDC_ADDSTRING, OnAddstring)
        ON_BN_CLICKED(IDC_DELETESTRING, OnDeletestring)
        ON_BN_CLICKED(IDC_GETWINDOWTEXT, OnGetwindowtext)
        ON_BN_CLICKED(IDC_SELECTSTRING, OnSelectstring)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDText message handlers

BOOL CDDText::OnSetActive() 
{
        updateControls();
        
        return CComboOps::OnSetActive();
}

BOOL CDDText::OnInitDialog() 
{
        CComboOps::OnInitDialog();
        
        c_SpinItemHeight.SetRange(0, 100);
        c_SpinSelStart.SetRange(-1, 200);
        c_SpinSelEnd.SetRange(-1, 200);
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CDDText::OnFindstring() 
{
 CString s;
 c_String.GetWindowText(s);  // get search string

 int start = c_Selection.GetVal();
 int result = combo->FindString(start, s);
 showResult_d_s_CBERR(IDS_CB_FINDSTRING, start, s, result);
 c_Selection.SetVal(result);
}

void CDDText::OnFindstringexact() 
{
 CString s;
 c_String.GetWindowText(s);  // get search string

 int start = c_Selection.GetVal();
 int result = combo->FindStringExact(start, s);
 showResult_d_s_CBERR(IDS_CB_FINDSTRINGEXACT, start, s, result);
 c_Selection.SetVal(result);
}

void CDDText::OnGeteditsel() 
{
 DWORD sel = combo->GetEditSel();
 int start = LOWORD(sel);
 int end   = HIWORD(sel);

 c_SelStart.SetVal(start);
 c_SelEnd.SetVal(end);

 showResult_N_N_DW(IDS_CB_GETEDITSEL, sel);
}

void CDDText::OnGetitemdata() 
{
 int index = c_Selection.GetVal();
 int result = combo->GetItemData(index);
 showResult_d_N_d(IDS_CB_GETITEMDATA, index, result);
 c_ItemData.SetVal(result);
}

void CDDText::OnGetitemheight() 
{
 int index = c_Selection.GetVal();
 int result = combo->GetItemHeight(index);
 showResult_d_N_CBERR(IDS_CB_GETITEMHEIGHT, index, result);
 c_ItemHeight.SetVal(result);
}

void CDDText::OnGetlbtext()
{
 CString s;
 int index = c_Selection.GetVal();
 int result = combo->SendMessage(CB_GETLBTEXT, index, (LPARAM)(LPCTSTR)s);
 showResult_d_B_CBERR(IDS_CB_GETLBTEXT, index, result);
 c_String.SetWindowText(s);
}

void CDDText::OnGetlbtextlength() 
{
 int index = c_Selection.GetVal();
 int result = combo->GetLBTextLen(index);
 showResult_d_N_CBERR(IDS_CB_GETLBTEXTLEN, index, result);
}

void CDDText::OnInsertstring() 
{
 CString s;
 c_String.GetWindowText(s);

 int index = c_Selection.GetVal();

 int result = combo->InsertString(index, s);

 c_Selection.SetVal(result);
 showResult_d_s_CBERR(IDS_CB_INSERTSTRING, index, s, result);
 updateControls();
}

void CDDText::OnSeteditsel() 
{
 DWORD sel = MAKELONG(c_SelStart.GetVal(), c_SelEnd.GetVal());
 int result = combo->SendMessage(CB_SETEDITSEL, 0, sel);


 showResult_N_DW_CBERR(IDS_CB_SETEDITSEL, sel, result);
}

void CDDText::OnSetitemdata() 
{
 int data = c_ItemData.GetVal();
 int index = c_Selection.GetVal();
 int result = combo->SetItemData(index, data);
 showResult_d_d_CBERR(IDS_CB_SETITEMDATA, index, data, result);
}

void CDDText::OnSetitemheight() 
{
 int h = c_ItemHeight.GetVal();
 int index = c_Selection.GetVal();

 int result = combo->SetItemHeight(index, h);
 showResult_d_d_CBERR(IDS_CB_SETITEMHEIGHT, index, h, result);
}

void CDDText::updateControls()
{
 c_SpinSelection.SetRange(-1, combo->GetCount());
}

void CDDText::OnAddstring() 
{
 CString s;
 c_String.GetWindowText(s);
 int result = combo->AddString(s);
 c_Selection.SetVal(result);
 showResult_N_s_CBERR(IDS_CB_ADDSTRING, s, result);
 updateControls();
}

void CDDText::OnDeletestring() 
{
 int index = c_Selection.GetVal();
 int result = combo->DeleteString(index);
 showResult_d_N_CBERR(IDS_CB_DELETESTRING, index, result);
 updateControls();
}

void CDDText::OnGetwindowtext() 
{
 TCHAR buffer[256];
 int result = combo->SendMessage(WM_GETTEXT, DIM(buffer), (LPARAM)&buffer);
 c_String.SetWindowText(buffer);
 showResult_d_B_CBERR(IDS_WM_GETTEXT, DIM(buffer), result);
}

void CDDText::OnSelectstring() 
{
 CString s;
 c_String.GetWindowText(s);  // get search string

 int start = c_Selection.GetVal();
 int result = combo->SelectString(start, s);
 showResult_d_s_CBERR(IDS_CB_SELECTSTRING, start, s, result);
 c_Selection.SetVal(result);
}
