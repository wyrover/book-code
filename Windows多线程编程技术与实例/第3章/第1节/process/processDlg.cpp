 // processDlg.cpp : implementation file
//

#include "stdafx.h"
#include "process.h"
#include "processDlg.h"

#include<Tlhelp32.h>
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_TRAY WM_USER+12

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg dialog

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessDlg)
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(ID_MENU_INFO, OnMenuInfo)
	ON_BN_CLICKED(IDC_KILL, OnKill)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessDlg message handlers

BOOL CProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_ListCtrl.m_hWnd, 
		LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT  );
	//在列表控件中添加图标
	m_imageList.Create(IDB_BITMAP1,16,1,RGB(255,0,255));
	m_ListCtrl.SetImageList(&m_imageList,LVSIL_SMALL);
	//初始化列表控件
	m_ListCtrl.InsertColumn(0,"Image Name",LVCFMT_LEFT,120,0);
	m_ListCtrl.InsertColumn(1,"ProID",LVCFMT_CENTER,88,1);
	m_ListCtrl.InsertColumn(2,"Parent ProID",LVCFMT_CENTER,88,2);

	

	SetTimer(1,20000,NULL);
	refresh();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProcessDlg::OnPaint() 
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
HCURSOR CProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CProcessDlg::refresh()
{
	//获取系统中的所有进程信息
	m_ListCtrl.DeleteAllItems();
	HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(SnapShot==NULL)
	{
		MessageBox("Fail to take a snapshot of the processes!!");
		return FALSE;
	}

	int nIndex;
	CString str;
	PROCESSENTRY32 ProcessInfo;//定义进程信息变量

	ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小
	//获得系统中第一个进程的信息
	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	int m_nProcess=0;
	while(Status)
	{
		m_nProcess++;

		//在列表控件中添加进程名称
		nIndex=m_ListCtrl.InsertItem(m_nProcess,ProcessInfo.szExeFile);
		//在列表控件中添加进程ID
		str.Format("%04x",ProcessInfo.th32ProcessID);
		m_ListCtrl.SetItemText(nIndex,1,str);
		//在列表控制件中添加进程的父进程
		str.Format("%04x",ProcessInfo.th32ParentProcessID);
		m_ListCtrl.SetItemText(nIndex,2,str);
		
		//获取下一个进程的信息
		Status=Process32Next(SnapShot,&ProcessInfo);
	}
	CloseHandle(SnapShot);
	
	
	UpdateData(false);
	return TRUE;
}

void CProcessDlg::addTray()
{
		NOTIFYICONDATA tnd;
		tnd.cbSize=sizeof(NOTIFYICONDATA);
		tnd.hWnd=this->m_hWnd;
		tnd.uID=IDI_ICON1;
		tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
		tnd.uCallbackMessage=WM_TRAY;// 
		tnd.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1));
		strcpy(tnd.szTip,"屏蔽系统鼠标右健程序，双击图标弹出窗口");//图标提示为"测试程序"
		Shell_NotifyIcon(NIM_ADD,&tnd);//向任务栏添加图标

}

void CProcessDlg::delTray()
{
		NOTIFYICONDATA tnd;
		tnd.cbSize=sizeof(NOTIFYICONDATA);
		tnd.hWnd=this->m_hWnd;
		tnd.uID=IDI_ICON1;
		tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
		tnd.uCallbackMessage=NULL;// 
		tnd.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1));
		strcpy(tnd.szTip,"屏蔽系统鼠标右健程序，双击图标弹出窗口");//图标提示为"测试程序"
		Shell_NotifyIcon(NIM_DELETE,&tnd);//向任务栏添加图标
}

LRESULT CProcessDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
		{
			case WM_SYSCOMMAND:
				if(wParam==SC_MINIMIZE)
				{
					addTray();
					ShowWindow(SW_HIDE);
					return 0;
				}
				break;
			case WM_TRAY:
				if(lParam==WM_RBUTTONUP||lParam==WM_LBUTTONUP)
				{
					ShowWindow(SW_SHOW);
			 		delTray();
				}
				break;
			}		
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CProcessDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	CMenu menu;CPoint pt;
	menu.LoadMenu(IDR_MENU1);
	GetCursorPos(&pt);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON
			   |TPM_RIGHTBUTTON, pt.x, pt.y,this, NULL);
	*pResult = 0;

}

void CProcessDlg::OnMenuInfo() 
{
	// TODO: Add your command handler code here
	CInfoDlg dlg;
	CString str,strTemp;
	int selItem=m_ListCtrl.GetNextItem(-1,LVIS_SELECTED);
	
	if(selItem>-1)
	{
		strTemp=m_ListCtrl.GetItemText(selItem,1);
		
		HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		
		PROCESSENTRY32 ProcessInfo;//声明进程信息变量
		ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小
		//返回系统中第一个进程的信息
		BOOL Status=Process32First(SnapShot,&ProcessInfo);
		int m_nProcess=0;
		while(Status)
		{
			m_nProcess++;
			
			str.Format("%04x",ProcessInfo.th32ProcessID);
			if(str==strTemp)
			{
				dlg.dwSize=ProcessInfo.dwSize;
				dlg.cntUsage=ProcessInfo.cntUsage;
				dlg.processID=ProcessInfo.th32ProcessID;
				dlg.defaultHeapID=ProcessInfo.th32DefaultHeapID;
				dlg.moduleID=ProcessInfo.th32ModuleID;
				dlg.cntThreads=ProcessInfo.cntThreads;
				dlg.parentProcessID=ProcessInfo.th32ParentProcessID;
				dlg.classBase=ProcessInfo.pcPriClassBase;
				dlg.dwFlags=ProcessInfo.dwFlags;
				dlg.m_strExeFile=ProcessInfo.szExeFile;
				TRACE("--------------%s\n",dlg.m_strExeFile);
				break;
			}
			//获取下一个进程的信息
			Status=Process32Next(SnapShot,&ProcessInfo);
		}
		CloseHandle(SnapShot);
		dlg.DoModal();
	}
	else
	{
		MessageBox("Please select the list item");
	}
	
}



void CProcessDlg::OnKill() 
{
	// TODO: Add your control notification handler code here
 
	CString str,strTemp;
	int CurItem=m_ListCtrl.GetNextItem(-1,LVIS_SELECTED);
	strTemp=m_ListCtrl.GetItemText(CurItem,1);
	HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 ProcessInfo;//声明进程信息变量
	ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小
	//返回系统中第一个进程的信息
	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	int m_nProcess=0;
	while(Status)
	{
		m_nProcess++;
		str.Format("%04x",ProcessInfo.th32ProcessID);
		if(str==strTemp)
		{
			if((AfxMessageBox("Do you really want to kill the process?",MB_OKCANCEL|MB_DEFBUTTON2))==IDOK)
			{
				HANDLE hProcess= OpenProcess( PROCESS_ALL_ACCESS, FALSE,ProcessInfo.th32ProcessID);
				if(::TerminateProcess(hProcess,1))
				{
					AfxMessageBox(" The process has been successfully killed");
					refresh() ;
				}
				else
					AfxMessageBox("Sorry, The process is not killed");
				CloseHandle(hProcess);
			}
			break;
		}
		//获取下一个进程的信息
		Status=Process32Next(SnapShot,&ProcessInfo);
	}	
	CloseHandle(SnapShot);
	
}

void CProcessDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	refresh();	
	CDialog::OnTimer(nIDEvent);
}

void CProcessDlg::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	refresh();
}

HBRUSH CProcessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(160,180,220));
		HBRUSH br=CreateSolidBrush(RGB(160,180,235));
		return br;
	}
	else if(nCtlColor==CTLCOLOR_DLG)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(160,180,220));
		HBRUSH br=CreateSolidBrush(RGB(160,180,235));
		return br;
	}
	return hbr;
}
