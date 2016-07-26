// Pop3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmtpPop.h"
#include "Pop3Dlg.h"
#include "POP3.h"
#include "MailMessage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPop3Dlg dialog


CPop3Dlg::CPop3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPop3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPop3Dlg)
	m_POP3 = _T("");
	m_password = _T("");
	m_subject = _T("");
	m_body = _T("");
	m_username = _T("");
	m_num = 0;
	m_soursemail = _T("");
	m_desmail = _T("");
	//}}AFX_DATA_INIT
}


void CPop3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPop3Dlg)
	DDX_Text(pDX, IDC_EDIT1, m_POP3);
	DDX_Text(pDX, IDC_EDIT3, m_password);
	DDX_Text(pDX, IDC_EDIT4, m_subject);
	DDX_Text(pDX, IDC_EDIT5, m_body);
	DDX_Text(pDX, IDC_EDIT2, m_username);
	DDX_Text(pDX, IDC_EDIT6, m_num);
	DDX_Text(pDX, IDC_EDIT7, m_soursemail);
	DDX_Text(pDX, IDC_EDIT8, m_desmail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPop3Dlg, CDialog)
	//{{AFX_MSG_MAP(CPop3Dlg)
	ON_BN_CLICKED(IDC_CANCAL, OnCancal)
	ON_BN_CLICKED(IDC_REC, OnReceive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPop3Dlg message handlers

void CPop3Dlg::OnCancal() 
{
	// TODO: Add your control notification handler code here
	
}

void CPop3Dlg::OnReceive() 
{
	// TODO: Add your control notification handler code here
		UpdateData( TRUE );
	CPOP3 pop3( m_POP3 );
	pop3.SetUserProperties(m_username,m_password);
	if (!pop3.Connect())
	{
		AfxMessageBox( pop3.GetLastError() );
		return;
	}
	CMailMessage msg;
	if (!pop3.GetMessage(m_num,&msg))
	{
		AfxMessageBox( pop3.GetLastError() );
		return;
	}

	m_body=msg.m_sBody;
	m_subject=msg.m_sSubject;
	m_soursemail=msg.m_sFrom;
	m_desmail="";
	for (int a=0; a<msg.GetNumRecipients(); a++)
	{
		CString sEmail;
		CString sFriendly;
		msg.GetRecipient(sEmail,sFriendly,a);
		m_desmail+=sEmail;
		m_desmail+=" ";
	}
	m_desmail.TrimRight();

	if( !pop3.Disconnect() )
	{
		AfxMessageBox( pop3.GetLastError() );
		return;
	}
	AfxMessageBox( _T( "Successfully disconnected" ) );

	UpdateData(FALSE);
	
}
