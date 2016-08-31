// KernOpt.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "dispopts.h"
#include "KernOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKernOptions dialog


CKernOptions::CKernOptions(CWnd* pParent /*=NULL*/)
        : CDialog(CKernOptions::IDD, pParent)
{
        //{{AFX_DATA_INIT(CKernOptions)
        m_KernedOnly = FALSE;
        //}}AFX_DATA_INIT
        opts.debug = FALSE;
        opts.hrule = FALSE;
        opts.vrule = FALSE;
        opts.chrule = FALSE;
        opts.colors = TRUE;
        opts.white = FALSE;
        opts.track = FALSE;
        opts.trackval = 0;
        opts.kerning = FALSE;
        opts.basal = FALSE;
}


void CKernOptions::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CKernOptions)
        DDX_Control(pDX, IDC_BASAL, c_Basal);
        DDX_Control(pDX, IDC_PAIR, c_Pair);
        DDX_Check(pDX, IDC_KERNED_ONLY, m_KernedOnly);
        //}}AFX_DATA_MAP
        DDX_Check(pDX, IDC_BASAL, opts.basal);
        DDX_Check(pDX, IDC_CHRULE, opts.chrule);
        DDX_Check(pDX, IDC_HRULE, opts.hrule);
        DDX_Check(pDX, IDC_VRULE, opts.vrule);
        DDX_Check(pDX, IDC_DEBUG, opts.debug);
        DDX_Check(pDX, IDC_COLORS, opts.colors);
        DDX_Check(pDX, IDC_WHITE, opts.white);
        DDX_Check(pDX, IDC_PAIR, opts.kerning);
        DDX_Check(pDX, IDC_TRACK, opts.track);
}


BEGIN_MESSAGE_MAP(CKernOptions, CDialog)
        //{{AFX_MSG_MAP(CKernOptions)
        ON_BN_CLICKED(IDC_BASAL, OnBasal)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKernOptions message handlers

void CKernOptions::OnBasal() 
{
 enableControls();
        
}

/****************************************************************************
*                        CKernOptions::EnableControls
* Result: void
*       
* Effect: 
*       Enables/disables appropriate controls
****************************************************************************/

void CKernOptions::enableControls()
    {
     c_Pair.EnableWindow(!c_Basal.GetCheck());
    }


BOOL CKernOptions::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        enableControls();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}
