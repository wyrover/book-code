// DialogSelectIP.cpp : implementation file
//

#include "stdafx.h"
#include "Sniffer.h"
#include "DialogSelectIP.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectIP dialog


CDialogSelectIP::CDialogSelectIP(CWnd* pParent /*=NULL*/)
    : CDialog(CDialogSelectIP::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDialogSelectIP)
    //}}AFX_DATA_INIT
}


void CDialogSelectIP::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDialogSelectIP)
    DDX_Control(pDX, IDC_LIST_IP, m_ctlListIP);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSelectIP, CDialog)
    //{{AFX_MSG_MAP(CDialogSelectIP)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectIP message handlers

BOOL CDialogSelectIP::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    for (int i = 0; i < m_iTotalIP; i++) {
        m_ctlListIP.AddString(m_strIP[i]);
    }

    m_iCurSel = 0;
    m_ctlListIP.SetCurSel(m_iCurSel);
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSelectIP::OnOK()
{
    // TODO: Add extra validation here
    m_iCurSel = m_ctlListIP.GetCurSel();

    if (m_iCurSel == LB_ERR) {
        MessageBox("没有选择要监听的IP地址。");
        return;
    }

    CDialog::OnOK();
}
