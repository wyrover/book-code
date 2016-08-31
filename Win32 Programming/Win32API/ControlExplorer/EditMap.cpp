// EditMap.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "IntEdit.h"
#include "TinyButt.h"
#include "hexdisp.h"
#include "ixable.h"
#include "loggable.h"
#include "editops.h"
#include "EditMap.h"
#include "Is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditMapping property page

IMPLEMENT_DYNCREATE(CEditMapping, CEditOps)

CEditMapping::CEditMapping() : CEditOps(CEditMapping::IDD)
{
        //{{AFX_DATA_INIT(CEditMapping)
        //}}AFX_DATA_INIT
}

CEditMapping::~CEditMapping()
{
}

void CEditMapping::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditMapping)
        DDX_Control(pDX, IDC_POS_EXPLANATION, c_e_Pos);
        DDX_Control(pDX, IDC_POS_CAPTION, c_c_Pos);
        DDX_Control(pDX, IDC_CHARFROMPOS, c_CharFromPos);
        DDX_Control(pDX, IDC_POSFROMCHAR, c_PosFromChar);
        DDX_Control(pDX, IDC_LINE_VAL, c_LineVal);
        DDX_Control(pDX, IDC_SETSEL, c_SetSel);
        DDX_Control(pDX, IDC_SPIN_COLUMN_VAL, c_SpinColumnVal);
        DDX_Control(pDX, IDC_SPIN_LINE_VAL, c_SpinLineVal);
        DDX_Control(pDX, IDC_SPIN_POSX, c_SpinPosX);
        DDX_Control(pDX, IDC_SPIN_POSY, c_SpinPosY);
        DDX_Control(pDX, IDC_COLUMN_VAL, c_Column);
        DDX_Control(pDX, IDC_LOWSEL, c_LowSel);
        DDX_Control(pDX, IDC_POSX, c_PosX);
        DDX_Control(pDX, IDC_POSY, c_PosY);
        DDX_Control(pDX, IDC_SPIN_HIGHSEL, c_SpinHighSel);
        DDX_Control(pDX, IDC_SPIN_LOWSEL, c_SpinLowSel);
        DDX_Control(pDX, IDC_HIGHSEL, c_HighSel);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMapping, CEditOps)
        //{{AFX_MSG_MAP(CEditMapping)
        ON_BN_CLICKED(IDC_CHARFROMPOS, OnCharfrompos)
        ON_BN_CLICKED(IDC_COLUMN, OnColumn)
        ON_BN_CLICKED(IDC_GETFIRSTVISIBLELINE, OnGetfirstvisibleline)
        ON_BN_CLICKED(IDC_GETSEL, OnGetsel)
        ON_BN_CLICKED(IDC_LINEFROMCHAR, OnLinefromchar)
        ON_BN_CLICKED(IDC_LINEINDEX, OnLineindex)
        ON_BN_CLICKED(IDC_POSFROMCHAR, OnPosfromchar)
        ON_BN_CLICKED(IDC_SETSEL, OnSetsel)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMapping message handlers

BOOL CEditMapping::OnInitDialog() 
{
        CEditOps::OnInitDialog();
        
        c_PosFromChar.EnableWindow(!Is3());
        c_CharFromPos.EnableWindow(!Is3());
        c_PosX.EnableWindow(!Is3());
        c_PosY.EnableWindow(!Is3());
        c_SpinPosX.EnableWindow(!Is3());
        c_SpinPosY.EnableWindow(!Is3());
        c_c_Pos.EnableWindow(!Is3());
        c_e_Pos.EnableWindow(!Is3());
        
        c_SpinLineVal.SetRange(0, 50);
        c_SpinLineVal.SetPos(0);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditMapping::OnGetfirstvisibleline() 
{
 DWORD result = CEditOps::getFirstVisibleLine();
 c_LineVal.SetVal(result);
}

void CEditMapping::OnLinefromchar() 
{
 int result = getLineFromChar(c_LowSel.GetVal());
 c_LineVal.SetVal(result);
}

void CEditMapping::OnLineindex() 
{
 int result = edit->SendMessage(EM_LINEINDEX, c_LineVal.GetVal());

 showResult_d_N_d(IDS_EM_LINEINDEX, c_LineVal.GetVal(), result);

 c_LowSel.SetVal(result);
 c_HighSel.SetVal(result);
}

void CEditMapping::OnPosfromchar() 
{
 CPoint pos = edit->PosFromChar(c_LowSel.GetVal());

 c_PosX.SetVal(pos.x);
 c_PosY.SetVal(pos.y);

 CString t;
 t.LoadString(IDS_EM_POSFROMCHAR);

 CString s;
 s.Format(_T("%s(%d) => 0x%08x=(%d, %d)"), t, c_LowSel.GetVal(),
                        MAKELONG(pos.x, pos.y), pos.x, pos.y);
 addMessage(s);

}


/****************************************************************************
*                          CEditMapping::OnSetActive
* Result: BOOL
*       
* Effect: 
*       Sets up any parameters needed as the page becomes active
****************************************************************************/

BOOL CEditMapping::OnSetActive()
    {

     getSel();

     enableControls();


     return CEditOps::OnSetActive();
    }

/****************************************************************************
*                           CEditMapping::OnCharfrompos
* Result: void
*       
* Effect: 
*       Given the position x,y, returns the character position
****************************************************************************/

void CEditMapping::OnCharfrompos() 
{
 CPoint pt(c_PosX.GetVal(), c_PosY.GetVal());

 int pos = edit->SendMessage(EM_CHARFROMPOS, 0, MAKELONG(pt.x, pt.y));

 // LOWORD(pos) is character position
 // HIWORD(pos) is line index

 CString t;
 t.LoadString(IDS_EM_CHARFROMPOS);
 
 CString s;

 s.Format(_T("%s(0, {%d, %d})=> 0x%08x=(ch:%d, ln:%d)"),
                        t, pt.x, pt.y, pos, LOWORD(pos), HIWORD(pos));

 addMessage(s);

 c_LowSel.SetVal(LOWORD(pos));
 c_HighSel.SetVal(LOWORD(pos));
 c_LineVal.SetVal(HIWORD(pos));
}

void CEditMapping::OnColumn() 
{
 int result = getLineFromChar(c_LowSel.GetVal());
 int linestart = edit->SendMessage(EM_LINEINDEX, result);

 showResult_d_N_d(IDS_EM_LINEINDEX, c_LineVal.GetVal(), linestart);
        
 int column = c_LowSel.GetVal() - linestart;

 CString s;
 s.Format(_T("%d - %d => %d"), c_LineVal.GetVal(), linestart, column);
 addMessage(s);

 c_Column.SetVal(column);
}
