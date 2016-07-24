// IPAddress.cpp : implementation file
//

#include "stdafx.h"
#include "Phone.h"
#include "IPAddress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIPAddress dialog
CIPAddress::~CIPAddress()
{
	if(ip!=NULL)
		delete ip;
}

CIPAddress::CIPAddress(CWnd* pParent /*=NULL*/)
	: CDialog(CIPAddress::IDD, pParent)
{
	ip=NULL;
	//{{AFX_DATA_INIT(CIPAddress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIPAddress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPAddress)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIPAddress, CDialog)
	//{{AFX_MSG_MAP(CIPAddress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPAddress message handlers

void CIPAddress::OnOK() 
{
	UpdateData();
	BYTE s[4];
	CIPAddressCtrl* ipWnd=(CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS);
	int i=ipWnd->GetAddress(s[0],s[1],s[2],s[3]);
	ip=new char[64];
	wsprintf(ip,"%d.%d.%d.%d",s[0],s[1],s[2],s[3]);
	if(i==0)
		MessageBox("You must enter an address,or click Cancel!");
	else
		CDialog::OnOK();
}
