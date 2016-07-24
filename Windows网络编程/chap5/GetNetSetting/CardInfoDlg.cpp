// CardInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetNetSetting.h"
#include "CardInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardInfoDlg dialog


CCardInfoDlg::CCardInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardInfoDlg)
	//}}AFX_DATA_INIT
}


void CCardInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardInfoDlg)
	DDX_Control(pDX, IDC_IPADDRESS, m_staticIPAddress);
	DDX_Control(pDX, IDC_MACADDR, m_addrMAC);
	DDX_Control(pDX, IDC_NETGATE, m_listNetGate);
	DDX_Control(pDX, IDC_ISROUTE, m_staticIsRoute);
	DDX_Control(pDX, IDC_ISDNS, m_staticIsDns);
	DDX_Control(pDX, IDC_IP, m_listctrlIP);
	DDX_Control(pDX, IDC_DOMAIN, m_staticDomain);
	DDX_Control(pDX, IDC_DNSSERIAL, m_listDNS);
	DDX_Control(pDX, IDC_HOSTNAME, m_staticHostName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCardInfoDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardInfoDlg message handlers
