// EditCont.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "IntEdit.h"
#include "TinyButt.h"
#include "hexdisp.h"
#include "ixable.h"
#include "loggable.h"
#include "EditOps.h"
#include "EditCont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditContents property page

IMPLEMENT_DYNCREATE(CEditContents, CEditOps)

CEditContents::CEditContents() : CEditOps(CEditContents::IDD)
{
        //{{AFX_DATA_INIT(CEditContents)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CEditContents::~CEditContents()
{
}

void CEditContents::DoDataExchange(CDataExchange* pDX)
{
        CEditOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditContents)
        DDX_Control(pDX, IDC_HEX, c_Hex);
        DDX_Control(pDX, IDC_GETLINECOUNT, c_GetLineCount);
        DDX_Control(pDX, IDC_WM_SETTEXT, c_SetText);
        DDX_Control(pDX, IDC_FMTLINES, c_FmtLines);
        DDX_Control(pDX, IDC_CANUNDO, c_CanUndo);
        DDX_Control(pDX, IDC_EMPTYUNDOBUFFER, c_EmptyUndoBuffer);
        DDX_Control(pDX, IDC_EOL_FLAG, c_EOLFlag);
        DDX_Control(pDX, IDC_UNDOABLE, c_Undoable);
        DDX_Control(pDX, IDC_GETLINE_VAL, c_GetlineVal);
        DDX_Control(pDX, IDC_SPIN_GETLINE_VAL, c_SpinGetlineVal);
        DDX_Control(pDX, IDC_WM_UNDO, c_WM_Undo);
        DDX_Control(pDX, IDC_WM_PASTE, c_WM_Paste);
        DDX_Control(pDX, IDC_WM_CUT, c_WM_Cut);
        DDX_Control(pDX, IDC_WM_COPY, c_WM_COPY);
        DDX_Control(pDX, IDC_EDIT, c_Edit);
        DDX_Control(pDX, IDC_LOWSEL, c_LowSel);
        DDX_Control(pDX, IDC_SPIN_LOWSEL, c_SpinLowSel);
        DDX_Control(pDX, IDC_HIGHSEL, c_HighSel);
        DDX_Control(pDX, IDC_SPIN_HIGHSEL, c_SpinHighSel);
        DDX_Control(pDX, IDC_EM_UNDO, c_EM_Undo);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditContents, CEditOps)
        //{{AFX_MSG_MAP(CEditContents)
        ON_BN_CLICKED(IDC_HEX, OnHex)
        ON_BN_CLICKED(IDC_CANUNDO, OnCanundo)
        ON_BN_CLICKED(IDC_EMPTYUNDOBUFFER, OnEmptyundobuffer)
        ON_BN_CLICKED(IDC_EM_UNDO, OnEmUndo)
        ON_BN_CLICKED(IDC_FMTLINES, OnFmtlines)
        ON_BN_CLICKED(IDC_GETLINE, OnGetline)
        ON_BN_CLICKED(IDC_GETLINECOUNT, OnGetlinecount)
        ON_BN_CLICKED(IDC_GETSEL, OnGetsel)
        ON_BN_CLICKED(IDC_REPLACESEL, OnReplacesel)
        ON_BN_CLICKED(IDC_SETSEL, OnSetsel)
        ON_BN_CLICKED(IDC_LINELENGTH, OnLinelength)
        ON_BN_CLICKED(IDC_WM_UNDO, OnWmUndo)
        ON_BN_CLICKED(IDC_WM_SETTEXT, OnWmSettext)
        ON_BN_CLICKED(IDC_WM_PASTE, OnWmPaste)
        ON_BN_CLICKED(IDC_WM_GETTEXTLENGTH, OnWmGettextlength)
        ON_BN_CLICKED(IDC_WM_GETTEXT, OnWmGettext)
        ON_BN_CLICKED(IDC_WM_CUT, OnWmCut)
        ON_BN_CLICKED(IDC_WM_COPY, OnWmCopy)
        ON_BN_CLICKED(IDC_WM_CLEAR, OnWmClear)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditContents message handlers

BOOL CEditContents::OnInitDialog() 
{
        CEditOps::OnInitDialog();
        
        c_SpinGetlineVal.SetRange(0, 50);
        c_SpinGetlineVal.SetPos(0);
        
        enableControls();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                         CEditContents::OnSetActive
* Result: BOOL
*       
* Effect: 
*       Initializes the selection
****************************************************************************/

BOOL CEditContents::OnSetActive()
    {
     getSel();  // force selection to be updated

     enableControls();

     return CEditOps::OnSetActive();
    }
