// SmtpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmtpPop.h"
#include "SmtpDlg.h"
#include "SMTP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmtpDlg dialog


CSmtpDlg::CSmtpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmtpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmtpDlg)
	m_smtp = _T("");
	m_username = _T("");
	m_password = _T("");
	m_soursemail = _T("");
	m_desmail = _T("");
	m_subject = _T("");
	m_emailbody = _T("");
	//}}AFX_DATA_INIT
}


void CSmtpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmtpDlg)
	DDX_Text(pDX, IDC_EDIT1, m_smtp);
	DDX_Text(pDX, IDC_EDIT2, m_username);
	DDX_Text(pDX, IDC_EDIT3, m_password);
	DDX_Text(pDX, IDC_EDIT4, m_soursemail);
	DDX_Text(pDX, IDC_EDIT5, m_desmail);
	DDX_Text(pDX, IDC_EDIT6, m_subject);
	DDX_Text(pDX, IDC_EDIT7, m_emailbody);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmtpDlg, CDialog)
	//{{AFX_MSG_MAP(CSmtpDlg)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmtpDlg message handlers

void CSmtpDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData( TRUE );
	CSMTP smtp( m_smtp );
	CMailMessage msg;
	msg.m_sFrom = m_soursemail;
	msg.AddMultipleRecipients(m_desmail );
	msg.m_sSubject = m_subject;
	msg.m_sBody = m_emailbody;
	if( !smtp.Connect() )
	{
		AfxMessageBox( smtp.GetLastError() );
		return;
	}
	if( !smtp.SendMessage( &msg ) )
	{
		AfxMessageBox( smtp.GetLastError() );
		return;
	}
	if( !smtp.Disconnect() )
	{
		AfxMessageBox( smtp.GetLastError() );
		return;
	}
	AfxMessageBox( _T( "Message Sent Successfully") );
	
}

void CSmtpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
