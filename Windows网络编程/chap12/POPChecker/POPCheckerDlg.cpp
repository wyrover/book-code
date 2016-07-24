// POPCheckerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "POPChecker.h"
#include "POPCheckerDlg.h"
//#include "Pop.h"
#include "TrayIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_ICON_NOTIFY  WM_USER+10

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOPCheckerDlg dialog

CPOPCheckerDlg::CPOPCheckerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPOPCheckerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPOPCheckerDlg)
	m_Duration = 0;
	m_NoMail = 0;
	m_Password = _T("");
	m_POPServer = _T("");
	m_UserName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPOPCheckerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOPCheckerDlg)
	DDX_Text(pDX, IDC_DURATION, m_Duration);
	DDX_Text(pDX, IDC_NO_MAIL, m_NoMail);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_POPSERVER, m_POPServer);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPOPCheckerDlg, CDialog)
	//{{AFX_MSG_MAP(CPOPCheckerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START, OnStart)
	ON_WM_TIMER()
	ON_COMMAND(ID_CONFIGURATION, OnConfiguration)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(ID_EXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOPCheckerDlg message handlers

BOOL CPOPCheckerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Error = 0;	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPOPCheckerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPOPCheckerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPOPCheckerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPOPCheckerDlg::OnStart() 
{
	m_Error = 0;
	UpdateData();
	if(m_UserName.IsEmpty()!=0)
		MessageBox("User Name is not Entered ....");
	else  
		if(m_Password.IsEmpty()!=0)
			MessageBox("Password is not Entered ....");
		else
			if(m_POPServer.IsEmpty()!=0)
				MessageBox("POPServer is not Entered ....");
			else
			{
				HICON hIcon;
				
				hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
				m_TrayIcon.Create(this, WM_ICON_NOTIFY, "Mail Checker", 
									  hIcon, IDR_POPUP_MENU);
				ShowWindow(SW_HIDE);
				SetTimer(1, m_Duration*1000, NULL);
			}
}

void CPOPCheckerDlg::OnTimer(UINT nIDEvent) 
{
	CPop PopServer;

	switch(nIDEvent)
	{
		case 0:	
			KillTimer(0);
			Beep(800*1,500);	// produce Beep sound 
			Beep(800*2,500);	// produce Beep sound 
			SetTimer(0, m_Duration*1000, NULL);
			break;	
		case 1:		
			KillTimer(1);
			if(PopServer.Connect(m_POPServer,m_UserName,m_Password)==FALSE)
			{
				Beep(800*1,500);	// produce Beep sound 
				Beep(800*2,500);	// produce Beep sound 
				SetTimer(0, m_Duration*1000, NULL);
				m_TrayIcon.SetTooltipText("Mail Checker: POP server cannot be connected ...");
				m_Error = 1;
			}
			else
				if(PopServer.Statistics()==FALSE)
				{
					Beep(32767,1000);	// produce Beep sound 
					SetTimer(0, m_Duration*1000, NULL);
					m_Error = 2;
				}
				else
				{
					m_NoMail = PopServer.GetNumberOfMails();
					UpdateData(FALSE);
					CString temp;
					temp.Format("Mail Checker: You have Mail = %d ",m_NoMail);
					m_TrayIcon.SetTooltipText((LPCTSTR)temp);
					SetTimer(1, m_Duration*1000, NULL);
				}	
		break;
	}
}

LRESULT CPOPCheckerDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
    // Delegate all the work back to the default implementation in CTrayIcon.
    return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CPOPCheckerDlg::OnConfiguration() 
{
	ShowWindow(SW_SHOWNORMAL);
	m_TrayIcon.RemoveIcon();	

	if(m_Error>0)
	{
		KillTimer(0);
		switch (m_Error)
		{
		case 1: MessageBox("POP Server cannot be connected ...");break;
		case 2: MessageBox("Mail information cannot be retrived ...");break;
		}
	}
	else
		KillTimer(1);
}

void CPOPCheckerDlg::OnExit() 
{
	KillTimer(0);
	m_TrayIcon.RemoveIcon();	
	CDialog::OnCancel();
}

BOOL CPOPCheckerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
