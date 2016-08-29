// Abort.cpp : implementation file
//

#include "stdafx.h"
#include "PrintExplorer.h"
#include "Abort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbort dialog


CAbort::CAbort(CWnd* pParent /*=NULL*/)
        : CDialog(CAbort::IDD, pParent)
{
        //{{AFX_DATA_INIT(CAbort)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}


void CAbort::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CAbort)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbort, CDialog)
        //{{AFX_MSG_MAP(CAbort)
                // NOTE: the ClassWizard will add message map macros here
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbort message handlers
