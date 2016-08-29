// EditStat.cpp : implementation file
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
#include "EditStat.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditState property page

IMPLEMENT_DYNCREATE(CEditState, CEditOps)

CEditState::CEditState() : CEditOps(CEditState::IDD)
{
        //{{AFX_DATA_INIT(CEditState)
        //}}AFX_DATA_INIT
}

CEditState::~CEditState()
{
}

void CEditState::DoDataExchange(CDataExchange* pDX)
{
        CEditOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CEditState)
        DDX_Control(pDX, IDC_LIMITVAL, c_LimitVal);
        DDX_Control(pDX, IDC_SPINLIMIT, c_SpinLimit);
        DDX_Control(pDX, IDC_EMPTYUNDOBUFFER, c_EmptyUndoBuffer);
        DDX_Control(pDX, IDC_SETPASSWORDCHAR, c_SetPasswordChar);
        DDX_Control(pDX, IDC_PASSWORD_CHAR, c_PasswordChar);
        DDX_Control(pDX, IDC_READONLY_FLAG, c_ReadOnlyFlag);
        DDX_Control(pDX, IDC_MODIFY_FLAG, c_ModifyFlag);
        DDX_Control(pDX, IDC_GETLIMITTEXT, c_GetLimitText);
        DDX_Control(pDX, IDC_SETLIMITTEXT, c_SetLimitText);
        DDX_Control(pDX, IDC_GETTHUMB, c_GetThumb);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditState, CEditOps)
        //{{AFX_MSG_MAP(CEditState)
        ON_BN_CLICKED(IDC_GETTHUMB, OnGetthumb)
        ON_BN_CLICKED(IDC_CANUNDO, OnCanundo)
        ON_BN_CLICKED(IDC_EMPTYUNDOBUFFER, OnEmptyundobuffer)
        ON_BN_CLICKED(IDC_GETMODIFY, OnGetmodify)
        ON_BN_CLICKED(IDC_GETPASSWORDCHAR, OnGetpasswordchar)
        ON_BN_CLICKED(IDC_SETREADONLY, OnSetreadonly)
        ON_BN_CLICKED(IDC_SETPASSWORDCHAR, OnSetpasswordchar)
        ON_BN_CLICKED(IDC_SCROLLCARET, OnScrollcaret)
        ON_EN_CHANGE(IDC_PASSWORD_CHAR, OnChangePasswordChar)
        ON_BN_CLICKED(IDC_SETMODIFY, OnSetmodify)
        ON_BN_CLICKED(IDC_SETLIMITTEXT, OnSetlimittext)
        ON_BN_CLICKED(IDC_GETLIMITTEXT, OnGetlimittext)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditState message handlers

/****************************************************************************
*                           CEditState::OnSetActive
* Result: BOOL
*       
* Effect: 
*       ²
****************************************************************************/

BOOL CEditState::OnSetActive()
    {
     c_SpinLimit.SetPos(edit->GetLimitText());
     c_LimitVal.SetVal(edit->GetLimitText());
     return CEditOps::OnSetActive();
    }

BOOL CEditState::OnInitDialog() 
{
        CEditOps::OnInitDialog();
        
        c_PasswordChar.SetLimitText(1);
        
        c_SpinLimit.SetRange(0, 100);
        c_SpinLimit.SetPos(edit->GetLimitText());
        c_LimitVal.SetVal(edit->GetLimitText());
        c_GetThumb.EnableWindow(!Is3());

        // We have to disable the GetLimitText if we are in API3.  If
        // we don't have GetLimitText, we can't properly set the control
        // in the parent window, so we also disable the SetLimitText as
        // well
        c_GetLimitText.EnableWindow(!Is3());
        c_SetLimitText.EnableWindow(!Is3());
        c_LimitVal.EnableWindow(!Is3());
        c_SpinLimit.EnableWindow(!Is3());

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
