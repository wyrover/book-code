// OpensshScpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpensshScp.h"
#include "OpensshScpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////
//ThreadPara* m_pThreadPara ;

UINT MyThreadProc(LPVOID pPara);

int WinExecuteProc(CString& FileName, BOOL bVisible);

/////////////////////////////////////////////////////////////////////////////
// COpensshScpDlg dialog

COpensshScpDlg::COpensshScpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpensshScpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpensshScpDlg)
	m_strSource = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	m_strFileName = _T("");

	m_pThread = NULL;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpensshScpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpensshScpDlg)
	DDX_Control(pDX, IDC_IPADDRESS_DES, m_IPAddrDesCtrl);
	DDX_Text(pDX, IDC_EDIT_SOURSE, m_strSource);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpensshScpDlg, CDialog)
	//{{AFX_MSG_MAP(COpensshScpDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_SCP, OnButScp)
	ON_BN_CLICKED(IDC_BUTTON_SELFILE, OnButtonSelfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpensshScpDlg message handlers


/////////////////////////////
BOOL COpensshScpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpensshScpDlg::OnPaint() 
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
HCURSOR COpensshScpDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COpensshScpDlg::OnButScp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	BYTE nField0,nField1,nField2,nField3;
	int iRet =  m_IPAddrDesCtrl.GetAddress( nField0, nField1, nField2,  nField3 );
	if (iRet != 4) {
		MessageBox("目的IP不合法，请重新输入IP地址");
		return;
	}
	if (m_strFileName.IsEmpty()) {
		MessageBox("请选择要传输的文件！");
		return;
	}
	if (m_strUser.IsEmpty()) {
		MessageBox("请填写目的用户名！");
		return;
	}
	CString strIP = "";
	strIP.Format("%d.%d.%d.%d",nField0, nField1, nField2,  nField3);

	ThreadPara* pPara = new ThreadPara();
	pPara->hWnd = GetSafeHwnd();
//	pPara->strPara = "c:\\pscp.exe -1 -i c:\\keyPriv.ppk c:\\aa.txt root@192.168.0.66:bbb.txt";

	pPara->strPara = "c:\\pscp.exe -1 -i c:\\keyPriv.ppk " + m_strSource + " " + m_strUser + "@" + strIP + ":" +m_strFileName;

	MessageBox(pPara->strPara );
	m_pThread = AfxBeginThread(MyThreadProc, pPara, THREAD_PRIORITY_NORMAL, 0, 0,NULL );
}

int WinExecuteProc(CString& FileName, BOOL bVisible)
{
	AfxMessageBox(FileName );
	DWORD exitCode;
	STARTUPINFO StartUpInfo;
	PROCESS_INFORMATION ProcessInfo;
	int Visible;

	ZeroMemory(&StartUpInfo,sizeof(StartUpInfo));
	StartUpInfo.cb = sizeof(StartUpInfo);
	StartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
	if(bVisible == TRUE)
		Visible = 1;
	else
		Visible = 0;
	StartUpInfo.wShowWindow = Visible;
	
	ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));

	LPTSTR strFileName = new TCHAR[FileName.GetLength() + 1];
	_tcscpy(strFileName,FileName);

	if(!CreateProcess(NULL,strFileName,NULL,NULL,FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE ,NULL,NULL,
			&StartUpInfo,&ProcessInfo))// | CREATE_SUSPENDED
	{
		delete strFileName;
		return -1;
	}
	else
	{//如用WaitForInputIdle则子程序是非模态的	
		WaitForSingleObject(ProcessInfo.hProcess,9000);// INFINITE
		GetExitCodeProcess(ProcessInfo.hProcess,&exitCode);

		TRACE("\n----------------------%d--------------\n",exitCode);

		CloseHandle(ProcessInfo.hProcess);
		CloseHandle(ProcessInfo.hThread);
		delete strFileName;
		return exitCode;


	
	}	

}


UINT MyThreadProc(LPVOID pPara)
{//没有要传输的文件,会很快返回1,如网络不通(指对方的IP不存在,对方没有SSH服务器等情况)会耗一段时间才返回1,传输成功时返回0 
//找不到pscp.exe,很快返回-1,找不到keypriv.ppk,会很慢的返回259,公钥认证无法通过,很慢才会返回259 



	ThreadPara* pParat = (ThreadPara*)pPara;
		AfxMessageBox(pParat->strPara );
	int retCode = 0;
	retCode = WinExecuteProc(pParat->strPara, FALSE);
	CString str = "";
	str.Format("%d",retCode);

	AfxMessageBox(str);
	

	CString strRet = "";
	switch(retCode) {
	case -1:
		strRet = "找不到pscp.exe";
		break;
	case 0:
		strRet = "文件传输成功";
		break;
	case 1:
		strRet = "网络不通或对方没有SSH1服务器或没找到要传输的文件！";
		break;
	case 259:
		strRet = "公钥认证失败！";
		break;
	default:
		strRet = "因网络不通或认证失败,文件传输失败!";
	}
	
	MessageBox(pParat->hWnd,strRet,NULL,MB_OK);
	delete pParat;

//	AfxEndThread(0,TRUE);
	return 0;
}

//DEL void COpensshScpDlg::OnOK() 
//DEL {
//DEL 	// TODO: Add extra validation here
//DEL 	DWORD dwForOK = 0;
//DEL 	dwForOK = WaitForSingleObject(m_pThread->m_hThread,1000);
//DEL 	if (dwForOK == WAIT_OBJECT_0) {
//DEL 			CDialog::OnOK();
//DEL 	}
//DEL 	else{
//DEL 		int iTemp = MessageBox("文件还没传送完,要放弃文件传送?",NULL,MB_OK|MB_ICONQUESTION);
//DEL 		TRACE("------------------------------------i-----------%d---",iTemp);
//DEL 	}
//DEL 	CDialog::OnOK();
//DEL }

//DEL void COpensshScpDlg::OnSsh() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL //	ThreadPara* pPara = new ThreadPara();
//DEL //	pPara->hWnd = GetSafeHwnd();
//DEL //	pPara->strPara = "c:\\putty.exe -1 -i c:\\keyPriv.ppk root@192.168.0.66";
//DEL //	
//DEL //	m_pThread = AfxBeginThread(MyThreadProc, pPara, THREAD_PRIORITY_NORMAL, 0, 0,NULL );
//DEL 
//DEL 	HINSTANCE hInstance ;
//DEL 	hInstance = ShellExecute(this->m_hWnd,"open","putty.exe","-1 -i keyPriv.ppk root@192.168.0.66","c:\\",SW_SHOW );
//DEL 	long retInt = (long)(hInstance);
//DEL 	CString strtemp = "";
//DEL 	strtemp.Format("%d",retInt);
//DEL 	MessageBox(strtemp);
//DEL 
//DEL 
//DEL 	
//DEL 	
//DEL }

void COpensshScpDlg::OnButtonSelfile() 
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
	 	m_strSource = dlg.GetPathName();			
		m_strFileName=dlg.GetFileName();
	    UpdateData(FALSE);
	}

}

void COpensshScpDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	if (m_pThread != NULL) {
//		
//		DWORD dwForOK = 0;
//		dwForOK = WaitForSingleObject(m_pThread->m_hThread,0);
//		
//		if (dwForOK ==  WAIT_OBJECT_0){// 
//			CDialog::OnCancel();
//		}
//		else{
//			int iTemp = MessageBox("文件还没传送完,要放弃文件传送?",NULL,MB_OKCANCEL);
//			TRACE("------------------------------------i-----------%d---",iTemp);
//		}
//	}
	CDialog::OnCancel();
}
