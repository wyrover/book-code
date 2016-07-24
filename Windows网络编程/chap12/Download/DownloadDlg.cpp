// DownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Download.h"
#include "DownloadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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


DWORD WINAPI DownloadThread(LPVOID);
DWORD WINAPI Notify(LPVOID);

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg dialog

CDownloadDlg::CDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bFinished = FALSE;
}

void CDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CDownloadDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg message handlers

BOOL CDownloadDlg::OnInitDialog()
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
	
	CRect rect;
	GetWindowRect(&rect);
	SetWindowPos(&wndTopMost, rect.left, rect.top, 
			rect.Width(), rect.Height(), SWP_NOMOVE); 

	for(int i = 0; i < 4; i++){
		m_hThread[i] = NULL;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDownloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDownloadDlg::OnPaint() 
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
HCURSOR CDownloadDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CDownloadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_ctrGetList.Create(
		LVS_REPORT | WS_VISIBLE | WS_CHILD | WS_BORDER,
			CRect( 10, 10 , 260, 125),	
			this, IDC_GET_LIST);

	m_ctrGetList.SetTextColor(RGB(0,0,0));
	m_ctrGetList.SetTextBkColor(RGB(220,235,220));
	m_ctrGetList.SetBkColor(RGB(220,235,220));

	// List Control Header Initializing.
	LV_COLUMN Lvc;

	Lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	Lvc.fmt = LVCFMT_LEFT;
	
	// First Column.
	Lvc.pszText = (LPTSTR)(LPCSTR)"Internet Link";
	Lvc.cx = 500;
	Lvc.iSubItem = 0;
	m_ctrGetList.InsertColumn(0, &Lvc);

	// Set Extended style.
	m_ctrGetList.SetExtendedStyle( 
		LVS_EX_ONECLICKACTIVATE |
		LVS_EX_FULLROWSELECT | 
		LVS_EX_UNDERLINEHOT | 
		LVS_EX_INFOTIP |
		LVS_EX_FLATSB  |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES 
		);
	
	return 0;
}

void CDownloadDlg::OnStart() 
{
	char s[1024];
	m_ctrGetList.GetItemText(0, 0, s, 1024);
	CString str;//("http://localhost/test/siguo.exe");
	str.Format(s);

	int ndx = str.ReverseFind('/');
	if(ndx == -1){
		AfxMessageBox("Error URL");
		return;
	}
	CString temp = str.Mid(ndx + 1);
	m_localfile.Empty();
	CFileDialog dlg(FALSE, NULL, temp);
	if(dlg.DoModal() == IDOK){
		m_localfile = dlg.m_ofn.lpstrFile;
		ndx = m_localfile.ReverseFind('\\');
		if(ndx == -1){
			AfxMessageBox("Error URL");
			return;
		}
		m_localfile = m_localfile.Left(ndx + 1);
		m_localfile +=temp;
		http.StartTask(str, m_localfile);
		CreateThread();
	}
}

void CDownloadDlg::CreateThread()
{
	if(http.m_bSupportResume){
		TRACE("We will create four thread to download!\n");
		DWORD dwThread;
		http.m_index = 0;
		for(int i = 0; i < 4; i++){
			http.m_bTerminate[i] = FALSE;
			m_hThread[i] = ::CreateThread(NULL, 0, DownloadThread, 
				(LPVOID)&http, 0, &dwThread);
			if(m_hThread[i] == NULL){
				TRACE("Create Thread Error!\n");
				return;
			}
		}
	}
	else{
		TRACE("We will create only one thread to download!\n");
		DWORD dwThread;
		http.m_index = 0;
		http.m_bTerminate[0] = FALSE;
		m_hThread[0] = ::CreateThread(NULL, 0, DownloadThread, 
			(LPVOID)&http, 0, &dwThread);
		if(m_hThread[0] == NULL){
			TRACE("Create Thread Error!\n");
			return;
		}
	}
	DWORD dwNotify;
	m_hNotify = ::CreateThread(NULL, 0, Notify, 
			(LPVOID)this, 0, &dwNotify);
	if(m_hNotify == NULL){
		TRACE("Create Thread Error!\n");
		return;
	}
}

void CDownloadDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for(int i = 0; i < 4; i++){
		if(m_hThread[i] != NULL){
			CloseHandle(m_hThread[i]);
			m_hThread[i] = NULL;
		}
	}
}

DWORD CDownloadDlg::Finish()
{
	char* lpData = NULL;
	if(http.m_bSupportResume){
		HRESULT ret = WaitForMultipleObjects(4, m_hThread, TRUE, INFINITE);
		if(http.m_bTerminate[0]){
			return -1;
		}
		if(ret == 0){
			m_bFinished = TRUE;
			AfxMessageBox("Download OK!\n");
			CFile file, t[4];
			CString name;
			
			file.Open(m_localfile, CFile::modeCreate | CFile::modeWrite);
			for(int i = 0; i < 4; i++){
				name.Empty();
				name.Format(".down%d", i);
				name = m_localfile + name;
				t[i].Open(name, CFile::modeRead);
				DWORD len = t[i].GetLength();
				if(lpData){
					delete [] lpData;
					lpData = NULL;
				}
				if(lpData == NULL){
					lpData = new char[len];
				}
				t[i].Read(lpData, len);
				file.Write(lpData, len);
				t[i].Close();
				DeleteFile(name);
			}
			if(lpData){
				delete [] lpData;
				lpData = NULL;
			}
			file.Close();
			name.Empty();
			name = m_localfile + ".down";
			DeleteFile(name);
		}
	}
	else{
		HRESULT ret = WaitForSingleObject(m_hNotify, INFINITE);
		if(http.m_bTerminate[0]){
			return -1;
		}
		if(ret == 0){
			m_bFinished = TRUE;
			AfxMessageBox("Download OK!\n");
			CFile file, t;
			CString name;
			
			file.Open(m_localfile, CFile::modeCreate | CFile::modeWrite);
			name = m_localfile + ".down0";
			t.Open(name, CFile::modeRead);
			DWORD len = t.GetLength();
			if(lpData){
				delete [] lpData;
				lpData = NULL;
			}
			if(lpData == NULL){
				lpData = new char[len];
			}
			t.Read(lpData, len);
			file.Write(lpData, len);
			t.Close();
			DeleteFile(name);
		
			if(lpData){
				delete [] lpData;
				lpData = NULL;
			}
			file.Close();
		}
	}
	if(lpData){
		delete [] lpData;
		lpData = NULL;
	}
	return 0;
}


DWORD WINAPI DownloadThread(LPVOID lpParam)
{
	CHTTPDownload* pThis = (CHTTPDownload*)lpParam;

	int index;
	index = pThis->m_index;
	InterlockedIncrement(&pThis->m_index);

	int ret = pThis->ThreadFunc(index);
	TRACE("Thread %d has successfully finsihed!%d\n", index, ret);
	return 0L;
}

DWORD WINAPI Notify(LPVOID lpParam)
{
	CDownloadDlg* pThis = (CDownloadDlg*)lpParam;

	return pThis->Finish();
}

void CDownloadDlg::OnCancel() 
{
	for(int i = 0; i < 4; i++)
		http.m_bTerminate[i] = TRUE;

	HRESULT ret = WaitForSingleObject(m_hNotify, INFINITE);
	if(ret != WAIT_OBJECT_0)
		return;
	if(m_bFinished)
		return;
	FILE* fp = NULL;
	CString name;
	name = m_localfile + ".down";
	if((fp = fopen(name, "w+")) == NULL){
		TRACE("Create Down file Error!\n");
		return ;
	}

	CString str = http.m_state.url + "\r\n";
	fputs(str, fp);
	str.Empty();
	str = http.m_state.localfile + "\r\n";
	fputs(str, fp);
	str.Empty();
	fwrite(&http.m_state.length, sizeof(LONG), 1, fp);
	fwrite(&http.m_state.time, sizeof(CTime), 1, fp);
	fwrite(http.m_state.range, sizeof(LONG), 8, fp);
	fclose(fp);
}
