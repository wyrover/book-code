// CopyFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CopyFile.h"
#include "CopyFileDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////

struct myPra
{
	char* pSourse;
	char* pDest;
};

unsigned long __stdcall copy(void* pthePra);
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
// CCopyFileDlg dialog

CCopyFileDlg::CCopyFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyFileDlg)
	m_strSourse = _T("");
	m_strDest = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCopyFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyFileDlg)
	DDX_Text(pDX, IDC_EDIT_SOURSE, m_strSourse);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDest);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCopyFileDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyFileDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELFILE, OnButtonSelfile)
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyFileDlg message handlers

BOOL CCopyFileDlg::OnInitDialog()
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


//	m_wndAnimate.Open(_T("FILECOPY.avi"));


	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCopyFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCopyFileDlg::OnPaint() 
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
HCURSOR CCopyFileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCopyFileDlg::OnButtonSelfile() 
{
	// TODO: Add your control notification handler code here
	  	//设置对话框参数并显示


	CFileDialog dlg(TRUE,
					NULL, 
					NULL, 
					OFN_HIDEREADONLY	| 
					OFN_OVERWRITEPROMPT | 
					OFN_FILEMUSTEXIST,
					_T("所有文件(*.*)|*.*||"),
					0);

	//把选择文件的全路径和文件名分别传给变量m_strSourse和m_strFileName
	if (dlg.DoModal() == IDOK) {	
	 	m_strSourse = dlg.GetPathName();			
		m_strFileName=dlg.GetFileName();
	    UpdateData(FALSE);
	}
	//将选择文件拷贝的目的地的“浏览”按钮设为可用状态
	GetDlgItem(IDC_BUTTON_DEST)->EnableWindow(TRUE);



	
}

void CCopyFileDlg::OnButtonDest() 
{
// TODO: Add your control notification handler code here
	//CDirDialog是自定义类，实现选择目录的功能，类似于CFileDialog,只不过CFileDialog是用于选择文件
	CDirDialog	dlg;
	if (dlg.DoBrowse(this) == IDOK) {
		//得到目标路径传给m_strDest
		//并显示它到对话框的相应路径框中　
		m_strDest = dlg.m_strPath;
		int len = m_strDest.GetLength();
		if (m_strDest[len-1] != '\\') {			
			m_strDest = m_strDest +"\\" ;	
		}
		
		m_strDest = m_strDest+m_strFileName;
		UpdateData(FALSE);
	}
	GetDlgItem(IDC_BUTTON_COPY)->EnableWindow(TRUE);
}

void CCopyFileDlg::OnButtonCopy() 
{
	struct  myPra pra;
// 	char *pdata1,*pdata2;
	UINT len1,len2;
	len1 = m_strDest.GetLength();
	len2 = m_strSourse.GetLength();
//	pdata1 = new char[len1];
//	pdata2 = new char[len2];
//	lstrcpy(pdata1,m_strDest);
//	lstrcpy(pdata2,m_strSourse);

	pra.pDest = new char[len1+1];
 	pra.pSourse = new char[len2+1];
	lstrcpyn(pra.pDest,m_strDest,len1+1);
 	lstrcpyn(pra.pSourse,m_strSourse,len2+1);





	unsigned long TID1;
	//创建文件拷贝线程
	CreateThread(NULL, 0, &copy, &pra, NULL, &TID1);		
}



unsigned long __stdcall copy(void* pthePra)
{
	
	struct myPra* pStrPra = (myPra*)pthePra;
	CFile sourceFile;
	CFile destFile;

  

   // we'll use a CFileException object to get error information

   CFileException ex;

   // open the source file for reading

   if (!sourceFile.Open(pStrPra->pSourse,
      CFile::modeRead | CFile::shareDenyWrite, &ex))
   {
      // complain if an error happened
      // no need to delete the ex object

      TCHAR szError[1024];
      ex.GetErrorMessage(szError, 1024);
      return 1;
   }
   else
   {
      if (!destFile.Open(pStrPra->pDest, CFile::modeWrite |
            CFile::shareExclusive | CFile::modeCreate, &ex))
      {
         TCHAR szError[1024];
         ex.GetErrorMessage(szError, 1024);
         sourceFile.Close();
         return 1;
      }

      BYTE buffer[4096];
      DWORD dwRead;


      // Read in 4096-byte blocks,
      // remember how many bytes were actually read,
      // and try to write that many out. This loop ends
      // when there are no more bytes to read.

      do
      {
         dwRead = sourceFile.Read(buffer, 4096);
         destFile.Write(buffer, dwRead);
	
	
      }
      while (dwRead > 0);

    
      destFile.Close();
      sourceFile.Close();
   }
	return 0;

}


