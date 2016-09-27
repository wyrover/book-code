// UserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "timesheet.h"
#include "UserDialog.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserDialog dialog


CUserDialog::CUserDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CUserDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUserDialog)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CUserDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUserDialog)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserDialog, CDialog)
    //{{AFX_MSG_MAP(CUserDialog)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserDialog message handlers
