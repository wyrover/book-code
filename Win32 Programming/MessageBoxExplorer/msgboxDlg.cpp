// msgboxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "msgbox.h"
#include "msgboxDlg.h"
#include "about.h"
#include "is95.h"
#include "xform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMsgboxDlg dialog

CMsgboxDlg::CMsgboxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgboxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgboxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMsgboxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgboxDlg)
	DDX_Control(pDX, IDC_NULLPARENT, c_NullParent);
	DDX_Control(pDX, IDC_FLAGS, c_Flags);
	DDX_Control(pDX, IDC_ICONWARNING_ICON, c_IconWarningIcon);
	DDX_Control(pDX, IDC_ICONQUESTION_ICON, c_IconQuestionIcon);
	DDX_Control(pDX, IDC_ICONINFORMATION_ICON, c_IconInformationIcon);
	DDX_Control(pDX, IDC_ICONERROR_ICON, c_IconErrorIcon);
	DDX_Control(pDX, IDC_NO_ICON, c_NoIcon);
	DDX_Control(pDX, IDC_RESULT, c_Result);
	DDX_Control(pDX, IDC_MSGBOX, c_MsgBox);
	DDX_Control(pDX, IDC_MB_YESNOCANCEL, c_YesNoCancel);
	DDX_Control(pDX, IDC_MB_YESNO, c_YesNo);
	DDX_Control(pDX, IDC_MB_TASKMODAL, c_TaskModal);
	DDX_Control(pDX, IDC_MB_SYSTEMMODAL, c_SystemModal);
	DDX_Control(pDX, IDC_MB_SETFOREGROUND, c_SetForeground);
	DDX_Control(pDX, IDC_MB_SERVICE_NOTIFICATION, c_ServiceNotification);
	DDX_Control(pDX, IDC_MB_RTLREADING, c_RTLReading);
	DDX_Control(pDX, IDC_MB_RIGHT, c_Right);
	DDX_Control(pDX, IDC_MB_RETRYCANCEL, c_RetryCancel);
	DDX_Control(pDX, IDC_MB_OKCANCEL, c_OKCancel);
	DDX_Control(pDX, IDC_MB_OK, c_OK);
	DDX_Control(pDX, IDC_MB_ICONWARNING, c_IconWarning);
	DDX_Control(pDX, IDC_MB_ICONQUESTION, c_IconQuestion);
	DDX_Control(pDX, IDC_MB_ICONINFORMATION, c_IconInformation);
	DDX_Control(pDX, IDC_MB_ICONERROR, c_IconError);
	DDX_Control(pDX, IDC_MB_HELP, c_Help);
	DDX_Control(pDX, IDC_MB_DEFBUTTON4, c_DefButton4);
	DDX_Control(pDX, IDC_MB_DEFBUTTON3, c_DefButton3);
	DDX_Control(pDX, IDC_MB_DEFBUTTON2, c_DefButton2);
	DDX_Control(pDX, IDC_MB_DEFBUTTON1, c_DefButton1);
	DDX_Control(pDX, IDC_MB_DEFAULT_DESKTOP_ONLY, c_DefaultDesktopOnly);
	DDX_Control(pDX, IDC_MB_APPLMODAL, c_ApplModal);
	DDX_Control(pDX, IDC_MB_ABORTRETRYIGNORE, c_AbortRetryIgnore);
	DDX_Control(pDX, IDC_CAPTION, c_Caption);
	DDX_Control(pDX, IDC_BODY, c_Body);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgboxDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgboxDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MSGBOX, OnMsgbox)
	ON_BN_CLICKED(IDC_MB_YESNOCANCEL, OnMbYesnocancel)
	ON_BN_CLICKED(IDC_MB_YESNO, OnMbYesno)
	ON_BN_CLICKED(IDC_MB_TASKMODAL, OnMbTaskmodal)
	ON_BN_CLICKED(IDC_MB_SYSTEMMODAL, OnMbSystemmodal)
	ON_BN_CLICKED(IDC_MB_SETFOREGROUND, OnMbSetforeground)
	ON_BN_CLICKED(IDC_MB_SERVICE_NOTIFICATION, OnMbServiceNotification)
	ON_BN_CLICKED(IDC_MB_RTLREADING, OnMbRtlreading)
	ON_BN_CLICKED(IDC_MB_RIGHT, OnMbRight)
	ON_BN_CLICKED(IDC_MB_RETRYCANCEL, OnMbRetrycancel)
	ON_BN_CLICKED(IDC_MB_OKCANCEL, OnMbOkcancel)
	ON_BN_CLICKED(IDC_MB_OK, OnMbOk)
	ON_BN_CLICKED(IDC_MB_ICONWARNING, OnMbIconwarning)
	ON_BN_CLICKED(IDC_MB_ICONQUESTION, OnMbIconquestion)
	ON_BN_CLICKED(IDC_MB_ICONINFORMATION, OnMbIconinformation)
	ON_BN_CLICKED(IDC_MB_ICONERROR, OnMbIconerror)
	ON_BN_CLICKED(IDC_MB_HELP, OnMbHelp)
	ON_BN_CLICKED(IDC_MB_DEFBUTTON4, OnMbDefbutton4)
	ON_BN_CLICKED(IDC_MB_DEFBUTTON3, OnMbDefbutton3)
	ON_BN_CLICKED(IDC_MB_DEFBUTTON2, OnMbDefbutton2)
	ON_BN_CLICKED(IDC_MB_DEFBUTTON1, OnMbDefbutton1)
	ON_BN_CLICKED(IDC_MB_DEFAULT_DESKTOP_ONLY, OnMbDefaultDesktopOnly)
	ON_BN_CLICKED(IDC_MB_APPLMODAL, OnMbApplmodal)
	ON_BN_CLICKED(IDC_MB_ABORTRETRYIGNORE, OnMbAbortretryignore)
	ON_BN_CLICKED(IDC_NO_ICON, OnNoIcon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgboxDlg message handlers

BOOL CMsgboxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CheckRadioButton(IDC_MB_APPLMODAL, IDC_MB_SYSTEMMODAL, IDC_MB_APPLMODAL);
	modality = MB_APPLMODAL;

	CheckRadioButton(IDC_NO_ICON, IDC_MB_ICONWARNING, IDC_NO_ICON);
	icon = 0;

	CheckRadioButton(IDC_MB_DEFBUTTON1, IDC_MB_DEFBUTTON4, IDC_MB_DEFBUTTON1);
	def = MB_DEFBUTTON1;

	CheckRadioButton(IDC_MB_ABORTRETRYIGNORE, IDC_MB_YESNOCANCEL, IDC_MB_OK);
	buttons = MB_OK;

	misc = 0;

	if(Is95())
	   { /* not NT */
	    c_ServiceNotification.EnableWindow(FALSE);  // NT only
	    c_DefaultDesktopOnly.EnableWindow(FALSE);   // NT only
	   } /* not NT */

	if(Is3())
	   { /* API 3 */
	    c_Right.EnableWindow(FALSE);
	    c_RTLReading.EnableWindow(FALSE);
	    c_DefButton4.EnableWindow(FALSE);
	    c_Help.EnableWindow(FALSE);
	   } /* API 3 */
	
	CString s;
	s.LoadString(IDS_SAMPLE_CAPTION);
	c_Caption.SetWindowText(s);

	s.LoadString(IDS_SAMPLE_BODY);
	c_Body.SetWindowText(s);

	HICON ic;

	ic = AfxGetApp()->LoadStandardIcon(IDI_ERROR);
	c_IconErrorIcon.SetIcon(ic);

	ic = AfxGetApp()->LoadStandardIcon(IDI_INFORMATION);
	c_IconInformationIcon.SetIcon(ic);

	ic = AfxGetApp()->LoadStandardIcon(IDI_QUESTION);
	c_IconQuestionIcon.SetIcon(ic);

	ic = AfxGetApp()->LoadStandardIcon(IDI_WARNING);
	c_IconWarningIcon.SetIcon(ic);

	c_Flags.SetWindowText(_T(""));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMsgboxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAbout dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CMsgboxDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMsgboxDlg::OnPaint() 
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
HCURSOR CMsgboxDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMsgboxDlg::OnMsgbox() 
{
 CString body;
 CString caption;

 c_Body.GetWindowText(body);
 c_Caption.GetWindowText(caption);
 c_Result.SetWindowText(_T(""));

 body = transform(body);

 int flags = buttons | icon | modality | def | misc;

 CString s;
 s.Format(_T("0x%08x"), flags);
 c_Flags.SetWindowText(s);

 int result = ::MessageBox((c_NullParent.GetCheck() ? NULL : m_hWnd), 
 						    body, caption,  flags);

	
 int id;

 switch(result)
    { /* result */
     case 0:
	     id = IDS_0;
	     break;
     case IDABORT:
	     id = IDS_IDABORT;
	     break;
     case IDCANCEL:
	     id = IDS_IDCANCEL;
	     break;
     case IDIGNORE:
	     id = IDS_IDIGNORE;
	     break;
     case IDNO:
	     id = IDS_IDNO;
	     break;
     case IDOK:
	     id = IDS_IDOK;
	     break;
     case IDRETRY:
	     id = IDS_IDRETRY;
	     break;
     case IDYES:
	     id = IDS_IDYES;
	     break;
    } /* result */

 s.LoadString(id);
 c_Result.SetWindowText(s);
}

void CMsgboxDlg::OnMbYesnocancel() 
{
 buttons = MB_YESNOCANCEL;
}

void CMsgboxDlg::OnMbYesno() 
{
 buttons = MB_YESNO;
}

void CMsgboxDlg::OnMbTaskmodal() 
{
 modality = MB_TASKMODAL;
}

void CMsgboxDlg::OnMbSystemmodal() 
{
 modality = MB_SYSTEMMODAL;
}

void CMsgboxDlg::OnMbSetforeground() 
{
 if(c_Right.GetCheck())
    misc |= MB_SETFOREGROUND;
 else
    misc &= ~MB_SETFOREGROUND;
	
}

void CMsgboxDlg::OnMbServiceNotification() 
{
 if(c_Right.GetCheck())
    misc |= MB_SERVICE_NOTIFICATION;
 else
    misc &= ~MB_SERVICE_NOTIFICATION;
}

void CMsgboxDlg::OnMbRtlreading() 
{
 if(c_Right.GetCheck())
    misc |= MB_RTLREADING;
 else
    misc &= ~MB_RTLREADING;
}

void CMsgboxDlg::OnMbRight() 
{
 if(c_Right.GetCheck())
    misc |= MB_RIGHT;
 else
    misc &= ~MB_RIGHT;
}

void CMsgboxDlg::OnMbRetrycancel() 
{
 buttons = MB_RETRYCANCEL;
}

void CMsgboxDlg::OnMbOkcancel() 
{
 buttons = MB_OKCANCEL;
}

void CMsgboxDlg::OnMbOk() 
{
 buttons = MB_OK;
	
}

void CMsgboxDlg::OnMbIconwarning() 
{
 icon = MB_ICONWARNING;
}

void CMsgboxDlg::OnMbIconquestion() 
{
 icon = MB_ICONQUESTION;
}

void CMsgboxDlg::OnMbIconinformation() 
{
 icon = MB_ICONINFORMATION;
}

void CMsgboxDlg::OnMbIconerror() 
{
 icon = MB_ICONERROR;
}

void CMsgboxDlg::OnMbHelp() 
{
 if(c_Help.GetCheck())
    misc |= MB_HELP;
 else
    misc &= ~MB_HELP;
}

void CMsgboxDlg::OnMbDefbutton4() 
{
 def = MB_DEFBUTTON4;
}

void CMsgboxDlg::OnMbDefbutton3() 
{
 def = MB_DEFBUTTON3;
}

void CMsgboxDlg::OnMbDefbutton2() 
{
 def = MB_DEFBUTTON2;
}

void CMsgboxDlg::OnMbDefbutton1() 
{
 def = MB_DEFBUTTON1;
}

void CMsgboxDlg::OnMbDefaultDesktopOnly() 
{
 if(c_DefaultDesktopOnly.GetCheck())
    misc |= MB_DEFAULT_DESKTOP_ONLY;
 else
    misc &= ~MB_DEFAULT_DESKTOP_ONLY;
}

void CMsgboxDlg::OnMbApplmodal() 
{
 modality = MB_APPLMODAL;
}

void CMsgboxDlg::OnMbAbortretryignore() 
{
 buttons = MB_ABORTRETRYIGNORE;
}

void CMsgboxDlg::OnNoIcon() 
{
 icon = 0;
}
