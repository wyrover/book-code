// XiaoChuHeiDian.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "XiaoChuHeiDian.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// XiaoChuHeiDian dialog


XiaoChuHeiDian::XiaoChuHeiDian(CWnd* pParent /*=NULL*/)
	: CDialog(XiaoChuHeiDian::IDD, pParent)
{
	//{{AFX_DATA_INIT(XiaoChuHeiDian)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void XiaoChuHeiDian::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XiaoChuHeiDian)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XiaoChuHeiDian, CDialog)
	//{{AFX_MSG_MAP(XiaoChuHeiDian)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XiaoChuHeiDian message handlers

void XiaoChuHeiDian::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	n=4;
}

void XiaoChuHeiDian::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	n=8;
}
