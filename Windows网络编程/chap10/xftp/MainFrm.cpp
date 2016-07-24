// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "xiongFtp.h"
#include "XiongFtpView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
BOOL IsFree, m_bConnected;
int nThreadCount;
CFtpInfoView* pFtpInfoView;
CLocFileView* pLocView;
CServFileView* pServView;
HINTERNET hInetSession;
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND_EX(CG_ID_VIEW_MYDIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_MYDIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_QUICKCONNECT, OnQuickconnect)
	ON_COMMAND(ID_FILE_DISCONNECT, OnFileDisconnect)
	ON_UPDATE_COMMAND_UI(ID_FILE_DISCONNECT, OnUpdateFileDisconnect)
	ON_COMMAND(IDC_FILE_RECONNECT, OnFileReconnect)
	ON_COMMAND(ID_FILE_CONNECT, OnFileConnect)
	ON_UPDATE_COMMAND_UI(ID_FILE_CONNECT, OnUpdateFileConnect)
	ON_UPDATE_COMMAND_UI(IDC_FILE_RECONNECT, OnUpdateFileReconnect)
	ON_COMMAND(ID_FILE_STOP, OnFileStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOP, OnUpdateFileStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND, OnUpdateFileSend)
	ON_UPDATE_COMMAND_UI(ID_FILE_FTP_DOWNLOAD, OnUpdateFileFtpDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bConnected=FALSE;
	
}

CMainFrame::~CMainFrame()
{
	InternetCloseHandle(hInetSession);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CImageList imageList;
	CBitmap bitmap;
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "My Dialog Bar":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CMainFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_MYDIALOGBAR
	//   5. Assign the item a Caption: My Dialog Bar

	// TODO: Change the value of CG_ID_VIEW_MYDIALOGBAR to an appropriate value:
	//   1. Open the file resource.h
	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_wndMyDialogBar
		if (!m_wndMyDialogBar.Create(this, CG_IDD_MYDIALOGBAR,
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_ID_VIEW_MYDIALOGBAR))
		{
			TRACE0("Failed to create dialog bar m_wndMyDialogBar\n");
			return -1;		// fail to create
		}

		m_wndMyDialogBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndMyDialogBar);
		m_wndMyDialogBar.SetDlgItemText(IDC_FTPPORT,"21");
		m_wndMyDialogBar.SetDlgItemText(IDC_FTPNAME,"169.254.46.12");
		m_wndMyDialogBar.SetDlgItemText(IDC_FTPUSER,"Administrator");
		m_wndMyDialogBar.SetDlgItemText(IDC_FTPPASSWORD,"xionglkj");
		m_wndMyDialogBar.GetDlgItem(IDC_QUICKCONNECT)->ShowWindow(SW_HIDE);

	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

CString CMainFrame::GetFtpName()
{
	CString str;
	m_wndMyDialogBar.GetDlgItemText(IDC_FTPNAME,str);
	return str;

}
int CMainFrame::GetFtpPort()
{
	CString str;
	m_wndMyDialogBar.GetDlgItemText(IDC_FTPPORT,str);
	int i=atoi(str);
	return i;
}
CString CMainFrame::GetFtpUser()
{
	CString str;
	m_wndMyDialogBar.GetDlgItemText(IDC_FTPUSER,str);
	return str;
}
CString CMainFrame::GetFtpPassword()
{
	CString str;
	m_wndMyDialogBar.GetDlgItemText(IDC_FTPPASSWORD,str);
	return str;
}

void CMainFrame::OnQuickconnect() 
{
	if(!hInetSession)return;
	pServView->OnQuickConnect();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


//////////////////////////////////////////////////////////////////////
// CFtpParaClass Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpParaClass::CFtpParaClass()
{
	HIMAGELIST hImageList;
	SHFILEINFO shFi;
	hImageList=(HIMAGELIST)SHGetFileInfo("C:\\",0,&shFi,sizeof(shFi),
		SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	if(!hImageList)
		return ;
	m_ctImageList.m_hImageList=hImageList;
	m_hDragCursor=AfxGetApp()->LoadIcon(IDI_CURSOR);
	m_bDragging=FALSE;
}

CFtpParaClass::~CFtpParaClass()
{
	if(m_ctImageList.m_hImageList)
		m_ctImageList.Detach();

}

void CFtpParaClass::SetFileColumns(CListCtrl* pListCtrl)
{
	static struct
	{
		LPSTR pszText;
		int uiFormat;
	}columns[]={
		_T("文件名"),LVCFMT_LEFT,
		_T("日期"),LVCFMT_CENTER,
		_T("字节数"),LVCFMT_CENTER
	};
	for(int i=0,j=120;i<sizeof(columns)/sizeof(columns[0]);i++,j-=10)
		pListCtrl->InsertColumn(i,columns[i].pszText,columns[i].uiFormat,j);
}

void CFtpParaClass::SetPopMenu(int nIndex)
{
	CMenu dMenu;
	if(!dMenu.LoadMenu(IDR_LOC_SERV_MENU))
		AfxThrowResourceException();
	CMenu* pPopMenu=dMenu.GetSubMenu(nIndex);
	ASSERT(pPopMenu!=NULL);
	POINT pt;
	::GetCursorPos(&pt);
	pPopMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,AfxGetMainWnd());
}

BOOL CFtpParaClass::SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo)
{
	pListCtrl->SetImageList(&m_ctImageList,LVSIL_SMALL);
	if(!pCombo)
		return TRUE;
	pCombo->SetImageList(&m_ctImageList);
	return TRUE;
}

void CFtpParaClass::GetFileIcon(CString& fileName,int* iIcon,int* iIconSel)
{
	CString str=fileName;
	if(str.Right(1)!="\\")
		str+="\\";
	SHFILEINFO shFi;
	SHGetFileInfo(str,0,&shFi,sizeof(shFi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
	*iIcon=shFi.iIcon;
	DestroyIcon(shFi.hIcon);
	if(iIconSel)
	{
		SHGetFileInfo(str,0,&shFi,sizeof(shFi),SHGFI_ICON|SHGFI_SMALLICON|SHGFI_OPENICON|SHGFI_DISPLAYNAME);
		*iIconSel=shFi.iIcon;
		DestroyIcon(shFi.hIcon);
	}
//	fileName=shFi.szDisplayName;
	return ;
}

void CFtpParaClass::OnInsertFile(CListCtrl* pListCtrl,LPVOID pIn,LPARAM lParam)
{
	FILE_FTP_INFO*pInfo=(FILE_FTP_INFO*)pIn;
	LVITEM	lvIt;
	int iIcon,nItem;
	if(pInfo->nType==DIRECTORYICON)
		nItem=0;
	else
		nItem=pListCtrl->GetItemCount();
	CString fileName=pInfo->szFilePath;
	if(fileName.Right(1)=="\\")
		fileName+=pInfo->szFileName;
	else
	{
		fileName+="\\";
		fileName+=pInfo->szFileName;
	}
	if((int)lParam==LOCFILE)
		GetFileIcon(fileName,&iIcon);
	else
		iIcon=pInfo->nType;
	lvIt.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	lvIt.iImage=iIcon;
	lvIt.lParam=pInfo->nType;
	lvIt.pszText=pInfo->szFileName;
	lvIt.iSubItem=0;
	lvIt.iItem=nItem;
	int iPos=pListCtrl->InsertItem(&lvIt);
	lvIt.mask=LVIF_TEXT;
	lvIt.iItem=iPos;
	lvIt.pszText=pInfo->szFileDate;
	lvIt.iSubItem=1;
	pListCtrl->SetItem(&lvIt);
	lvIt.pszText=pInfo->szFileSize;
	lvIt.iSubItem=2;
	pListCtrl->SetItem(&lvIt);
	IsFree=TRUE;
}

int CFtpParaClass::OnInsertDir(CComboBoxEx* pCombo,CString& str,int iIcon,int iIconSel)
{
	int nItem=pCombo->GetCount();
	COMBOBOXEXITEM Combo;
	Combo.mask=CBEIF_IMAGE|CBEIF_TEXT|CBEIF_SELECTEDIMAGE;
	Combo.iImage=iIcon;
	Combo.iSelectedImage=iIconSel;
	Combo.pszText=(LPTSTR)(LPCTSTR)str;
    Combo.iItem=nItem;
	pCombo->InsertItem(&Combo);
	return nItem;
}

void CFtpParaClass::OnFileName(CListCtrl* pListCtrl)
{
	POSITION iPos=pListCtrl->GetFirstSelectedItemPosition();
	CString str;
	int nItem=pListCtrl->GetNextSelectedItem(iPos);
	if(nItem==-1)return;
	str=pListCtrl->GetItemText(nItem,0);
	CInputEdit* pEdit=new CInputEdit(nItem,0,str,pListCtrl);
	CRect rect;
	pListCtrl->GetItemRect(nItem,&rect,LVIR_LABEL);
	rect.left+=1;
	rect.right-=10;
	rect.top+=26;
	rect.bottom+=26;
	pEdit->Create(WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,rect,pListCtrl->GetParent(),IDD_INPUT_EDIT);
}


/////////////////////////////////////////////////////////////////////////////
// CLocFileView


IMPLEMENT_DYNCREATE(CLocFileView, CFormView)

CLocFileView::CLocFileView()
	: CFormView(CLocFileView::IDD)
{
	m_bHaveDotFlag=FALSE;
	m_pSendFileThread=NULL;
	//{{AFX_DATA_INIT(CLocFileView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CLocFileView::~CLocFileView()
{
	if(m_pFindLocFileThread)
	{
		m_pFindLocFileThread->ResumeThread();
		::WaitForSingleObject(m_pFindLocFileThread->m_hThread,100);
		delete m_pFindLocFileThread;
	}
	if(m_pSendFileThread)
	{   m_pSendFileThread->ResumeThread();
		::WaitForSingleObject(m_pSendFileThread->m_hThread,100);
		delete m_pSendFileThread;
	}
}

void CLocFileView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocFileView)
	DDX_Control(pDX, IDC_LOC_FILE, m_ctLocFile);
	DDX_Control(pDX, IDC_LOC_DIR, m_ctLocDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocFileView, CFormView)
	//{{AFX_MSG_MAP(CLocFileView)
	ON_NOTIFY(NM_DBLCLK, IDC_LOC_FILE, OnDblclkLocFile)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_LOC_DIR, OnSelchangeLocDir)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LOC_FILE, OnBegindragLocFile)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_RCLICK, IDC_LOC_FILE, OnRclickLocFile)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LOC_FILE, OnBeginlabeleditLocFile)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LOC_FILE, OnEndlabeleditLocFile)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFILE,CLocFileView::OnInsertLocFile)
	ON_MESSAGE(WM_SETREDRAWFLAG,CLocFileView::OnSetLocRedrawFlag)
	ON_MESSAGE(WM_SETDIRECTORYDISPLAY,CLocFileView::OnSetLocDirectoryDisplay)
	ON_MESSAGE(WM_REDISPLAYFILE,CLocFileView::OnReDisplayLocFile)
	ON_MESSAGE(WM_SETDIR,CLocFileView::OnSetLocDir)
	ON_MESSAGE(WM_PRESENDFILE,CLocFileView::OnPreSendFile)
	ON_COMMAND(ID_FILE_DELETE,CLocFileView::OnLocFileDelete)
	ON_COMMAND(ID_FILE_SEND,CLocFileView::OnLocFileSend)
	ON_COMMAND(ID_FILE_RENAME,CLocFileView::OnLocFileReName)
	ON_COMMAND(ID_FILE_EXECUTE,CLocFileView::OnLocFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocFileView diagnostics

#ifdef _DEBUG
void CLocFileView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLocFileView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLocFileView message handlers
/////////////////////////////////////////////////////////////////////////////
// CServFileView

IMPLEMENT_DYNCREATE(CServFileView, CFormView)

CServFileView::CServFileView()
	: CFormView(CServFileView::IDD)
{
	m_bHaveDotFlag=FALSE;
	m_pRenameFileThread=NULL;
	m_pReceiveFileThread=NULL;
	m_pFindServFileThread=NULL;
	m_pDeleteFileThread=NULL;
	//{{AFX_DATA_INIT(CServFileView)
	//}}AFX_DATA_INIT
}

CServFileView::~CServFileView()
{
	if(m_ctServImageList.m_hImageList)
		m_ctServImageList.Detach();
	if(m_pFindServFileThread)
	{
		::WaitForSingleObject(m_pFindServFileThread->m_hThread,100);
		delete m_pFindServFileThread;
	}
	if(m_pReceiveFileThread)
	{
		::WaitForSingleObject(m_pReceiveFileThread->m_hThread,100);
		delete m_pReceiveFileThread;
	}
	if(m_pDeleteFileThread)
	{
		::WaitForSingleObject(m_pDeleteFileThread->m_hThread,100);
		delete m_pDeleteFileThread;
	}
	if(m_pRenameFileThread)
	{
		::WaitForSingleObject(m_pRenameFileThread->m_hThread,100);
		delete m_pRenameFileThread;
	}
	DeleteObject(hBitmap);

}

void CServFileView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServFileView)
	DDX_Control(pDX, IDC_BACK, m_ctBack);
	DDX_Control(pDX, IDC_SERV_FILE, m_ctServFile);
	DDX_Control(pDX, IDC_SERV_DIR, m_ctServDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServFileView, CFormView)
	//{{AFX_MSG_MAP(CServFileView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_SERV_FILE, OnDblclkServFile)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_SERV_FILE, OnBegindragServFile)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(NM_RCLICK, IDC_SERV_FILE, OnRclickServFile)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_SERV_FILE, OnBeginlabeleditServFile)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_SERV_FILE, OnEndlabeleditServFile)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_CBN_SELCHANGE(IDC_SERV_DIR, OnSelchangeServDir)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETFILE,CServFileView::OnInsertServFile)
	ON_MESSAGE(WM_SETREDRAWFLAG,CServFileView::OnSetServRedrawFlag)
	ON_MESSAGE(WM_SETDIRECTORYDISPLAY,CServFileView::OnSetServDirectoryDisplay)
	ON_MESSAGE(WM_REDISPLAYFILE,CServFileView::OnReDisplayServFile)
	ON_MESSAGE(WM_SETDIR,CServFileView::OnSetServDir)
	ON_MESSAGE(WM_PREDELETEFILE,CServFileView::OnPreDeleteFile)
	ON_MESSAGE(WM_PRERECEIVEFILE,CServFileView::OnPreReceiveFile)
	ON_COMMAND(ID_FILE_DELETE,CServFileView::OnServFileDelete)
	ON_COMMAND(ID_FILE_FTP_DOWNLOAD,CServFileView::OnServFileDownload)
	ON_COMMAND(ID_FILE_RENAME,CServFileView::OnServFileReName)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServFileView diagnostics

#ifdef _DEBUG
void CServFileView::AssertValid() const
{
	CFormView::AssertValid();
}

void CServFileView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServFileView message handlers
/////////////////////////////////////////////////////////////////////////////
// CFtpInfoView

IMPLEMENT_DYNCREATE(CFtpInfoView, CEditView)

CFtpInfoView::CFtpInfoView()
{
	
}

CFtpInfoView::~CFtpInfoView()
{
	m_NewFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CFtpInfoView, CEditView)
	//{{AFX_MSG_MAP(CFtpInfoView)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECORDFTPINFO,CFtpInfoView::OnRecordFtpInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpInfoView drawing

void CFtpInfoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CFtpInfoView diagnostics

#ifdef _DEBUG
void CFtpInfoView::AssertValid() const
{
	CEditView::AssertValid();
}

void CFtpInfoView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFtpInfoView message handlers

LRESULT CFtpInfoView::OnRecordFtpInfo(WPARAM wParam,LPARAM lParam)
{

	CEdit& edit=GetEditCtrl();
	CString str=(LPCTSTR)lParam;
	int nLine=edit.GetLineCount();
	int nStart=edit.LineIndex(nLine);
	edit.SetSel(nStart,str.GetLength()+nStart);
	edit.ReplaceSel("\r\n"+str);
	return 0L;

}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// CG: The following block was added by the Splitter Bar component.
	{
		if(!m_wndSplitter1.CreateStatic(this,3,1))
			return FALSE;
        if(!m_wndSplitter1.CreateView(0,0,RUNTIME_CLASS(CFtpInfoView),CSize(375,100),pContext)||
			!m_wndSplitter1.CreateView(2,0,RUNTIME_CLASS(CXiongFtpView),CSize(375,100),pContext))
			return FALSE;
		m_wndSplitter1.SetRowInfo(1,180,0);
		if(m_wndSplitter2.CreateStatic(&m_wndSplitter1,1,2,WS_CHILD|WS_VISIBLE,m_wndSplitter1.IdFromRowCol(1,0))==NULL)
			return FALSE;
		if(!m_wndSplitter2.CreateView(0,0,RUNTIME_CLASS(CLocFileView),CSize(375,180),pContext)||
			!m_wndSplitter2.CreateView(0,1,RUNTIME_CLASS(CServFileView),CSize(375,180),pContext))
			return FALSE;
		return TRUE;
	}
}

void CLocFileView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	nThreadCount=0;
	pLocView=this;
	m_pFindLocFileThread=NULL;
    SetFileColumns(&m_ctLocFile);
	SetImageList(&m_ctLocFile,&m_ctLocDir);
	m_ctLocFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	GetLocDir();
	int i=m_ctLocDir.FindStringExact(-1,"C:\\");
	m_ctLocDir.SetCurSel(i);
	::SetCurrentDirectory("C:\\");
	CMyAppThread* pThread=new CMyAppThread("C:\\");
	if(!BeginFindLocFile(pThread))
		AfxMessageBox("起动查找线程失败");
}

int CLocFileView::InsertLocDir(CString& str,int iIcon,int iIconSel)
{	
	return OnInsertDir(&m_ctLocDir,str,iIcon,iIconSel);
}

void CServFileView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_ctServFile.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_pMainFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	pServView=this;
	hBitmap=::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BACKBITMAP));
	m_ctBack.SetBitmap(hBitmap);
	SetFileColumns(&m_ctServFile);	
	SetServImageList(IDB_FILE_BITMAP);
}

LRESULT CServFileView::OnInsertServFile(WPARAM wParam,LPARAM lParam)
{
	FILE_FTP_INFO* pInfo=(FILE_FTP_INFO*)wParam;
	CString str=pInfo->szFileName;
	if(str=="."||str=="..")
	{
		m_bHaveDotFlag=TRUE;
		return 0L;
	}
	OnInsertFile(&m_ctServFile,(LPVOID)pInfo,lParam);
    return 0L;

}

LRESULT CServFileView::OnSetServRedrawFlag(WPARAM wParam,LPARAM lParam)
{
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	if(m_bHaveDotFlag)
	{
		CString str="..";
		int iIcon=DIRECTORYICON;
		m_ctServFile.InsertItem(0,str,iIcon);
		m_ctServFile.SetItemData(0,DIRECTORYICON);
		m_bHaveDotFlag=FALSE;
	}
	m_ctServFile.Invalidate();
	m_ctServFile.SetRedraw();
	return 0L;	
}

LRESULT CServFileView::OnSetServDirectoryDisplay(WPARAM wParam,LPARAM lParam)
{
	CString str=(LPCTSTR)lParam;
	int nItem=m_ctServDir.FindStringExact(-1,str);
	if(nItem==CB_ERR)
		nItem=OnInsertDir(&m_ctServDir,str,DIRECTORYICON,DIRECTORYSEL);
	m_ctServDir.SetCurSel(nItem);
	return 0L;
}

LRESULT CServFileView::OnReDisplayServFile(WPARAM wParam,LPARAM lParam)
{
	CString strFtp=(LPCTSTR)lParam;
	OnSetServDirectoryDisplay(0,lParam);
	CMyAppThread* pThread=new CMyAppThread(NULL,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE);
	if(!BeginFindServFile(pThread))
		AfxMessageBox("建立网络线程失败");
	return 0L;
}

LRESULT CServFileView::OnSetServDir(WPARAM wParam,LPARAM lParam)
{
    CString str=(LPCTSTR)lParam;
	int nItem=m_ctServDir.FindStringExact(-1,str);
	if(nItem==CB_ERR)
		nItem=OnInsertDir(&m_ctServDir,str,DIRECTORYICON,DIRECTORYSEL);
	m_ctServDir.SetCurSel(nItem);
	m_ctServFile.SetRedraw(FALSE);
	m_ctServFile.DeleteAllItems();
	m_ctServFile.SetRedraw();
	return 0L;

}

void CServFileView::OnServFileDelete()
{
	if(!m_bConnected)return;
	CString strFtp;
	int i=0;
	POSITION iPos;
	int nItem=m_ctServDir.GetCurSel();
	m_ctServDir.GetLBText(nItem,strFtp);
	i=m_ctServFile.GetSelectedCount();
	FILE_COUNT_INFO* pFileCount=new FILE_COUNT_INFO[i];
	SEND_RECEIVE_INFO* pSeRe=new SEND_RECEIVE_INFO;
	iPos=m_ctServFile.GetFirstSelectedItemPosition();
	for(int j=0;j<i;j++)
	{
		int nItem=m_ctServFile.GetNextSelectedItem(iPos);
		m_ctServFile.GetItemText(nItem,0,pFileCount[j].fileName,256);
		pFileCount[j].ufileFlag=m_ctServFile.GetItemData(nItem);
		
	}
	CMyAppThread* pThread=new CMyAppThread(NULL,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE,i);
	pSeRe->pCount=pFileCount;
	pSeRe->pThread=pThread;
	if(!BeginDeleteFile(pSeRe))
		AfxMessageBox("建立文件删除线程出错");
}


void CServFileView::OnServFileDownload()
{
	if(!m_bConnected)return;
	CString str,strFtp;
	int i=0;
	POSITION iPos;
	int nItem=pLocView->m_ctLocDir.GetCurSel();
	pLocView->m_ctLocDir.GetLBText(nItem,str);
	nItem=m_ctServDir.GetCurSel();
	m_ctServDir.GetLBText(nItem,strFtp);
	i=m_ctServFile.GetSelectedCount();
	FILE_COUNT_INFO* pFileCount=new FILE_COUNT_INFO[i];
	SEND_RECEIVE_INFO* pSeRe=new SEND_RECEIVE_INFO;
	iPos=m_ctServFile.GetFirstSelectedItemPosition();
	for(int j=0;j<i;j++)
	{
		int nItem=m_ctServFile.GetNextSelectedItem(iPos);
		m_ctServFile.GetItemText(nItem,0,pFileCount[j].fileName,256);
		pFileCount[j].ufileFlag=m_ctServFile.GetItemData(nItem);
		
	}
	CMyAppThread* pThread=new CMyAppThread(str,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE,i);
	pSeRe->pCount=pFileCount;
	pSeRe->pThread=pThread;
	if(!BeginReceiveFile(pSeRe))
		AfxMessageBox("建立文件接收线程出错");
}

void CServFileView::OnServFileReName()
{
	OnFileName(&m_ctServFile);
}

BOOL CServFileView::BeginDeleteFile(LPVOID pInfo)
{
	SEND_RECEIVE_INFO* pInf=(SEND_RECEIVE_INFO*)pInfo;
	CMyAppThread* pThread=(CMyAppThread*)pInf->pThread;
	if(m_pDeleteFileThread)
	{
		::WaitForSingleObject(m_pDeleteFileThread->m_hThread,INFINITE);
		delete m_pDeleteFileThread;
		m_pDeleteFileThread=NULL;
	}
	if(m_pDeleteFileThread=AfxBeginThread(pThread->StartDeleteFileThread,pInfo,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pDeleteFileThread->m_bAutoDelete=FALSE;
		nThreadCount++;
		m_pDeleteFileThread->ResumeThread();
		return TRUE;
	}
		return FALSE;
}

BOOL CServFileView::BeginRenameFile(LPVOID pInfo)
{
	FILE_CHANGE_INFO* pInf=(FILE_CHANGE_INFO*)pInfo;
	CMyAppThread* pThread=(CMyAppThread*)pInf->pThread;
	if(m_pRenameFileThread)
	{
		::WaitForSingleObject(m_pRenameFileThread->m_hThread,100);
		delete m_pRenameFileThread;
		m_pRenameFileThread=NULL;
	}
	if(m_pRenameFileThread=AfxBeginThread(pThread->StartRenameFileThread,pInfo,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pRenameFileThread->m_bAutoDelete=FALSE;
		m_pRenameFileThread->ResumeThread();
		return TRUE;
	}
	return FALSE;
}
BOOL CServFileView::BeginFindServFile(LPVOID lParam)
{
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	if(m_pFindServFileThread)
	{
		::WaitForSingleObject(m_pFindServFileThread->m_hThread,INFINITE);
		delete m_pFindServFileThread;
		m_pFindServFileThread=NULL;
	}
	if(m_pFindServFileThread=AfxBeginThread(pThread->StartFindServFileThread,lParam,THREAD_PRIORITY_BELOW_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pFindServFileThread->m_bAutoDelete=FALSE;
		m_ctServFile.SetRedraw(FALSE);
		m_ctServFile.DeleteAllItems();		
		m_pFindServFileThread->ResumeThread();
		return TRUE;
	}
	return FALSE;
}


void CLocFileView::GetLocDir()
{
	int iIcon,iIconSel;
	char lpBuffer[MAX_PATH];
	char* ptr;
	CString str;
	m_iDirCount=0;
	::GetLogicalDriveStrings(MAX_PATH,lpBuffer);
	ptr=lpBuffer;
	while(*ptr)
	{
		m_iDirCount++;
		str=ptr;
		GetFileIcon(str,&iIcon,&iIconSel);
		InsertLocDir(str,iIcon,iIconSel);
		ptr+=strlen(ptr)+1;
	}
}

BOOL CLocFileView::BeginFindLocFile(LPVOID pInfo)
{
	
	CMyAppThread* pThread=(CMyAppThread*)pInfo;
	if(m_pFindLocFileThread)
	{
		::WaitForSingleObject(m_pFindLocFileThread->m_hThread,100);
		delete m_pFindLocFileThread;
		m_pFindLocFileThread=NULL;
	}
	if(m_pFindLocFileThread=AfxBeginThread(pThread->StartFindLocFileThread,pInfo,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pFindLocFileThread->m_bAutoDelete=FALSE;
		m_ctLocFile.SetRedraw(FALSE);
		m_ctLocFile.DeleteAllItems();
		m_pFindLocFileThread->ResumeThread();
		return TRUE;
	}
	return FALSE;
}

BOOL CLocFileView::BeginSendFile(LPVOID pInfo)
{
	SEND_RECEIVE_INFO* pInf=(SEND_RECEIVE_INFO*)pInfo;
	CMyAppThread* pThread=(CMyAppThread*)pInf->pThread;
	if(m_pSendFileThread)
	{
		::WaitForSingleObject(m_pSendFileThread->m_hThread,10);
		delete m_pSendFileThread;
		m_pSendFileThread=NULL;
	}
	if(m_pSendFileThread=AfxBeginThread(pThread->StartSendFileThread,pInfo,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pSendFileThread->m_bAutoDelete=FALSE;
		nThreadCount++;
		m_pSendFileThread->ResumeThread();
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLocFileView::BeginPreSendFile(LPVOID pInfo)
{
	CMyAppThread* pThread=(CMyAppThread*)pInfo;
	CWinThread* pPreSendFile=NULL;
	if(pPreSendFile=AfxBeginThread(pThread->StartPreSendFileThread,pInfo,THREAD_PRIORITY_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		nThreadCount++;
		pPreSendFile->ResumeThread();
		return TRUE;
	}
	return FALSE;
}

LRESULT CLocFileView::OnSetLocDirectoryDisplay(WPARAM wParam,LPARAM lParam)
{
	int iIcon,iIconSel,nItem;
	CString str=(LPCTSTR)lParam;
	if(str.Right(1)=="\\")
		m_ctLocDir.DeleteItem(m_iDirCount);
	nItem=m_ctLocDir.FindStringExact(-1,str);
	if(nItem==CB_ERR)
	{
		GetFileIcon(str,&iIcon,&iIconSel);
		int iCount=m_ctLocDir.GetCount();
		if(iCount==m_iDirCount)
			nItem=InsertLocDir(str,iIcon,iIconSel);
		else
		{
			m_ctLocDir.DeleteItem(m_iDirCount);
			nItem=InsertLocDir(str,iIcon,iIconSel);
		}
	}
	m_ctLocDir.SetCurSel(nItem);
	return 0L;
}

LRESULT CLocFileView::OnPreSendFile(WPARAM wParam,LPARAM lParam)
{
	CString str=(LPCTSTR)wParam;
	CString strFtp=(LPCTSTR)lParam;
	CMyAppThread* pThread=new CMyAppThread(str,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE);
	if(!BeginPreSendFile(pThread))
		AfxMessageBox("起动发送子线程失败");
	return 0L;

}
LRESULT CLocFileView::OnSetLocRedrawFlag(WPARAM wParam,LPARAM lParam)
{
	if(m_bHaveDotFlag)
	{
		CString str="..";
		int iIcon=DIRECTORYICON;
		m_ctLocFile.InsertItem(0,str,iIcon);
		m_ctLocFile.SetItemData(0,DIRECTORYICON);
		m_bHaveDotFlag=FALSE;
	}
	m_ctLocFile.Invalidate();
	m_ctLocFile.SetRedraw();
	return 0L;	
}

LRESULT CLocFileView::OnReDisplayLocFile(WPARAM wParam,LPARAM lParam)
{
	CString str=(LPCTSTR)lParam;
	OnSetLocDirectoryDisplay(0,lParam);
	CMyAppThread* pThread=new CMyAppThread(str);
	if(!BeginFindLocFile(pThread))
		AfxMessageBox("建立网络线程失败");
	return 0L;
}

LRESULT CLocFileView::OnInsertLocFile(WPARAM wParam,LPARAM lParam)
{
	FILE_FTP_INFO* pInfo=(FILE_FTP_INFO*)wParam;
	CString str=pInfo->szFileName;
	if(str=="."||str=="..")
	{
		m_bHaveDotFlag=TRUE;
		return 0L;
	}
	OnInsertFile(&m_ctLocFile,pInfo,lParam);
	return 0L;
}
/////////////////////////////////////////////////////////////////////////////
// CMyAppThread

IMPLEMENT_DYNCREATE(CMyAppThread, CWinThread)

CMyAppThread::CMyAppThread()
{
	
}


CMyAppThread::~CMyAppThread()
{
	if(hFtpConn)
		InternetCloseHandle(hFtpConn);	
}



BOOL CMyAppThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMyAppThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyAppThread, CWinThread)
	//{{AFX_MSG_MAP(CMyAppThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyAppThread message handlers

UINT CMyAppThread::StartRenameFileThread(LPVOID lParam)
{
	CFtpParaClass::FILE_CHANGE_INFO* pInfo=(CFtpParaClass::FILE_CHANGE_INFO*)lParam;
	CMyAppThread* pThis=(CMyAppThread*)pInfo->pThread;
	return pThis->RenameFileThread(lParam);


}
UINT CMyAppThread::StartDeleteFileThread(LPVOID lParam)
{
	CFtpParaClass::SEND_RECEIVE_INFO* pInfo=(CFtpParaClass::SEND_RECEIVE_INFO*)lParam;
	CMyAppThread* pThis=(CMyAppThread*)pInfo->pThread;
	return pThis->DeleteFileThread(lParam);
}

UINT CMyAppThread::StartReceiveFileThread(LPVOID lParam)
{
	CFtpParaClass::SEND_RECEIVE_INFO* pInfo=(CFtpParaClass::SEND_RECEIVE_INFO*)lParam;
	CMyAppThread* pThis=(CMyAppThread*)pInfo->pThread;
	return pThis->ReceiveFileThread(lParam);
}

UINT CMyAppThread::StartSendFileThread(LPVOID lParam)
{
	CFtpParaClass::SEND_RECEIVE_INFO* pInfo=(CFtpParaClass::SEND_RECEIVE_INFO*)lParam;
	CMyAppThread* pThis=(CMyAppThread*)pInfo->pThread;
	return pThis->SendFileThread(lParam);
}

UINT CMyAppThread::StartPreSendFileThread(LPVOID lParam)
{
	CMyAppThread* pThis=(CMyAppThread*)lParam;
	return pThis->PreSendFileThread(lParam);
}

UINT CMyAppThread::StartPreReceiveFileThread(LPVOID lParam)
{
	CMyAppThread* pThis=(CMyAppThread*)lParam;
	return pThis->PreRecieveFileThread(lParam);
}

UINT CMyAppThread::StartPreDeleteFileThread(LPVOID lParam)
{
	CMyAppThread* pThis=(CMyAppThread*)lParam;
	return pThis->PreDeleteFileThread(lParam);
}
	
UINT CMyAppThread::StartFindLocFileThread(LPVOID lParam)
{
	CMyAppThread* pThis=(CMyAppThread*)lParam;
	return pThis->FindLocFileThread(lParam);
}

UINT CMyAppThread::StartFindServFileThread(LPVOID lParam)
{
	CMyAppThread* pThis=(CMyAppThread*)lParam;
	return pThis->FindServFileThread(lParam);
}

UINT CMyAppThread::FindLocFileThread(LPVOID lParam)
{
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	CFtpParaClass::FILE_FTP_INFO* pInfo=new (CFtpParaClass::FILE_FTP_INFO);
	CFileFind fileFind;
	CString str;
	UINT i=0;
	CTime time;
	BOOL bContinue;
	::SetCurrentDirectory(m_szLocDirectory);
	::GetCurrentDirectory(MAX_PATH,(LPTSTR)(LPCTSTR)m_szLocDirectory);
	strcpy(pInfo->szFilePath,m_szLocDirectory);
	::PostMessage(pLocView->m_hWnd,WM_SETDIRECTORYDISPLAY,0,(LPARAM)(LPCSTR)m_szLocDirectory);
	bContinue=fileFind.FindFile("*.*");
	if(!bContinue)
		goto end;
	while(bContinue)
	{
		bContinue=fileFind.FindNextFile();
		if(fileFind.IsHidden())continue;
		str=fileFind.GetFileName();
		strcpy(pInfo->szFileName,str);
		fileFind.GetLastWriteTime(time);
		str=time.Format("%x");
		strcpy(pInfo->szFileDate,str);
		if(fileFind.IsDirectory())
		{
			strcpy(pInfo->szFileSize,"<DIR>");
			pInfo->nType=DIRECTORYICON;
			
		}
		else
		{
			int i=fileFind.GetLength();
			if(i>1024)
			{
				str.Format("%d",i/1024);
				str+="KB";
			}
			else
				str.Format("%d",i);
			strcpy(pInfo->szFileSize,str);
			pInfo->nType=FILEICON;
		}
		::SendMessage(pLocView->m_hWnd,WM_SETFILE,(WPARAM)pInfo,(LPARAM)LOCFILE);
	}
end:
	::PostMessage(pLocView->m_hWnd,WM_SETREDRAWFLAG,0,0);
	fileFind.Close();
	delete pInfo;
	delete pThread;
	return 0L;
}

UINT CMyAppThread::FindServFileThread(LPVOID lParam)
{
	CString str,szFile,szFtpInfo;
	if(!hFtpConn)
	{
		szFtpInfo=m_szFtpName+"服务器未接上";
		pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
		return 0L;
	}
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	CFtpParaClass::FILE_FTP_INFO* pInfo=new(CFtpParaClass::FILE_FTP_INFO);
	FILETIME fileTime;
	WIN32_FIND_DATA findData;
	HINTERNET hFind;
	szFtpInfo="正在连接"+m_szFtpName+"服务器";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	szFtpInfo=m_szFtpName+"服务器已连上，用户"+m_szFtpUser+"登录成功";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	DWORD dwLength=MAX_PATH;
	if(m_szFtpDirectory!=" ")
		FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	FtpGetCurrentDirectory(hFtpConn,(LPTSTR)(LPCTSTR)m_szFtpDirectory,&dwLength);
	::SendMessage(pServView->m_hWnd,WM_SETDIRECTORYDISPLAY,0,(LPARAM)(LPCTSTR)m_szFtpDirectory);
	szFtpInfo="正在查找文件";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	if(!(hFind=FtpFindFirstFile(hFtpConn,_T("*"),&findData,0,0)))
	{
		if (GetLastError()  == ERROR_NO_MORE_FILES) 
		{
			AfxMessageBox("目录为空，没有多余的内容");
			goto end;
		}
		else
		{
			szFtpInfo="文件查找出错，退回，请查明原因";
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			goto end;
		}
	}
	do{
		szFile=findData.cFileName;
		strcpy(pInfo->szFileName,szFile);
		fileTime=findData.ftLastWriteTime;
		CTime time=CTime(fileTime);
		CString str=time.Format("%x");
		strcpy(pInfo->szFileDate,str);
		if(findData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
		{
			strcpy(pInfo->szFileSize,"<DIR>");
			pInfo->nType=DIRECTORYICON;
		}
		else
		{
			DWORD i=findData.nFileSizeLow;
			if(i>1024)
			{
				str.Format("%ld",i/1024);
				str+="KB";
			}
			else
				str.Format("%ld",i);
			strcpy(pInfo->szFileSize,str);
			pInfo->nType=FILEICON;
		}
		::SendMessage(pServView->m_hWnd,WM_SETFILE,(WPARAM)pInfo,(LPARAM)SERVFILE);
		
	}while(InternetFindNextFile(hFind,&findData));
	
	::SendMessage(pServView->m_hWnd,WM_SETREDRAWFLAG,0,(LPARAM)lParam);
	InternetCloseHandle(hFind);
end:
	szFtpInfo="查找文件结束";
	::Sleep(0);
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	delete pInfo;
	delete pThread;
	return 0L;	
}

UINT CMyAppThread::SendFileThread(LPVOID lParam)
{
	IsFree=TRUE;
	int i=0;
	CFtpParaClass:: FILE_FTP_INFO* pInf=new (CFtpParaClass::FILE_FTP_INFO);
	CFtpParaClass::SEND_RECEIVE_INFO* pInf1=(CFtpParaClass::SEND_RECEIVE_INFO*)lParam;
	CMyAppThread* pThread=(CMyAppThread*)pInf1->pThread;
	CFtpParaClass::FILE_COUNT_INFO* pInfo=pInf1->pCount;
    CString str,strFtp,szFtpInfo,DirName;
	if(!hFtpConn)
		goto end;
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	::SetCurrentDirectory(m_szLocDirectory);
	for(;i<m_nCount;i++)
	{
		str=m_szLocDirectory;
		strFtp=m_szFtpDirectory;
		DirName=pInfo[i].fileName;
		if(DirName=="..")continue;
		if(m_szLocDirectory.Right(1)=="\\")
			str+=DirName;
		else
			str+="\\"+DirName;
		if(m_szFtpDirectory.Right(1)=='/')
			strFtp+=DirName;
		else
			strFtp+="/"+DirName;
		if(pInfo[i].ufileFlag==FILEICON)
		{
			szFtpInfo="发送";
			szFtpInfo+=DirName;
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			if(FtpPutFile(hFtpConn,str,strFtp,INTERNET_FLAG_NO_CACHE_WRITE,0))
			{
				if(IsFree)
				{
					IsFree=FALSE;
					strcpy(pInf->szFileName,DirName);
					strcpy(pInf->szFileDate,"");
					strcpy(pInf->szFileSize,"");
					pInf->nType=FILEICON;
					::PostMessage(pServView->m_hWnd,WM_SETFILE,(WPARAM)pInf,SERVFILE);
				}
				continue;
			}
			else
			{
				szFtpInfo="发送"+DirName+"出错";
				pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				break;
			}
		}
		else
			pLocView->SendMessage(WM_PRESENDFILE,(WPARAM)(LPCTSTR)str,(LPARAM)(LPCTSTR)strFtp);
	}
end:
	nThreadCount--;
	::Sleep(10);
	while(1)
	{
		if(nThreadCount==0)
		{
			szFtpInfo="文件发送结束,退回";
			szFtpInfo+=m_szLocDirectory;
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			::SendMessage(pServView->m_hWnd,WM_REDISPLAYFILE,0,(LPARAM)(LPCTSTR)m_szFtpDirectory);
			break;
		}
		::Sleep(100);
	}
	delete[] pInfo;
	delete pInf;
	delete pThread;
	delete pInf1;
	return 0L;
}


UINT CMyAppThread::PreSendFileThread(LPVOID lParam)
{
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	CFtpParaClass:: FILE_FTP_INFO* pInfo=new CFtpParaClass::FILE_FTP_INFO;
	CFileFind fileFind;
	CString szFtpInfo,str,strFtp;
	szFtpInfo="创建"+m_szFtpDirectory+"目录";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	FtpCreateDirectory(hFtpConn,m_szFtpDirectory);
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	::SetCurrentDirectory(m_szLocDirectory);
	szFtpInfo="进入"+m_szFtpDirectory+"目录";
	pServView->SendMessage(WM_SETDIR,0,(LPARAM)(LPCTSTR)m_szFtpDirectory);
	if(IsFree)
		pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	UINT bContinue=fileFind.FindFile("*");
	CString tFile;
	if(bContinue)
	{
		do
		{
			str=m_szLocDirectory;
			strFtp=m_szFtpDirectory;
			bContinue=fileFind.FindNextFile();
			tFile=fileFind.GetFileName();
			if(tFile=="."||tFile==".."||fileFind.IsHidden())continue;
			if(m_szLocDirectory.Right(1)=='\\')
					str+=tFile;
				else
					str+="\\"+tFile;
				if(m_szFtpDirectory.Right(1)=='/')
					strFtp+=tFile;
				else
					strFtp+="/"+tFile;
			if(fileFind.IsDirectory())
				pLocView->SendMessage(WM_PRESENDFILE,(WPARAM)(LPCTSTR)str,(LPARAM)(LPCTSTR)strFtp);
			else
			{
				szFtpInfo="发送"+tFile;
				pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				FtpPutFile(hFtpConn,str,strFtp,INTERNET_FLAG_NO_CACHE_WRITE,0)?TRUE:FALSE;
				if(IsFree)
				{
					IsFree=FALSE;
					strcpy(pInfo->szFileName,tFile);
					strcpy(pInfo->szFileDate,"");
					strcpy(pInfo->szFileSize,"");
					pInfo->nType=FILEICON;
					::PostMessage(pServView->m_hWnd,WM_SETFILE,(WPARAM)pInfo,SERVFILE);
				}
			}
		}while(bContinue);
		fileFind.Close();
	}
		delete pInfo;
		delete pThread;
		nThreadCount--;
		return 0L;
}

UINT CMyAppThread::ReceiveFileThread(LPVOID lParam)
{
	IsFree=TRUE;
	int i=0;
	CFtpParaClass:: FILE_FTP_INFO* pInf=new (CFtpParaClass::FILE_FTP_INFO);
	CFtpParaClass::SEND_RECEIVE_INFO* pInf1=(CFtpParaClass::SEND_RECEIVE_INFO*)lParam;
	CMyAppThread* pThread=(CMyAppThread*)pInf1->pThread;
	CFtpParaClass::FILE_COUNT_INFO* pInfo=pInf1->pCount;
    CString str,strFtp,szFtpInfo,DirName;
	if(!hFtpConn)
		goto end;
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	::SetCurrentDirectory(m_szLocDirectory);
	for(;i<m_nCount;i++)
	{
		str=m_szLocDirectory;
		strFtp=m_szFtpDirectory;
		DirName=pInfo[i].fileName;
		if(DirName=="..")continue;
		if(m_szLocDirectory.Right(1)=="\\")
			str+=DirName;
		else
			str+="\\"+DirName;
		if(m_szFtpDirectory.Right(1)=='/')
			strFtp+=DirName;
		else
			strFtp+="/"+DirName;
		if(pInfo[i].ufileFlag==FILEICON)
		{
			szFtpInfo="接收"+DirName;
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			if(FtpGetFile(hFtpConn,strFtp,str,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY |
				INTERNET_FLAG_NO_CACHE_WRITE,0))
			{
				if(IsFree)
				{
					IsFree=FALSE;
					strcpy(pInf->szFileName,DirName);
					strcpy(pInf->szFileDate,"");
					strcpy(pInf->szFileSize,"");
					pInf->nType=FILEICON;
					::PostMessage(pLocView->m_hWnd,WM_SETFILE,(WPARAM)pInf,SERVFILE);
				}
				continue;
			}
			else
			{
				szFtpInfo="接收"+DirName+"出错";
				pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				break;
			}
		}
		else
			pServView->SendMessage(WM_PRERECEIVEFILE,(WPARAM)(LPCTSTR)str,(LPARAM)(LPCTSTR)strFtp);
	}
	
end:
	nThreadCount--;
	::Sleep(10);
	while(1)
	{
		if(nThreadCount==0)
		{
			szFtpInfo="文件接收结束,退回"+m_szLocDirectory;
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			::SendMessage(pLocView->m_hWnd,WM_REDISPLAYFILE,0,(LPARAM)(LPCTSTR)m_szLocDirectory);
			break;
		}
		::Sleep(100);
	}
	delete[] pInfo;
	delete pInf;
	delete pThread;
	delete pInf1;
	return 0L;
}

UINT CMyAppThread::PreRecieveFileThread(LPVOID lParam)
{
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	CFtpParaClass:: FILE_FTP_INFO* pInfo=new CFtpParaClass::FILE_FTP_INFO;
	WIN32_FIND_DATA  fileData;
	HINTERNET hFind;
	CString szFtpInfo,str,strFtp;
	szFtpInfo="创建"+m_szLocDirectory+"目录";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	CreateDirectory(m_szLocDirectory,NULL);
	::SetCurrentDirectory(m_szLocDirectory);
	if(IsFree)
		pLocView->SendMessage(WM_SETDIR,0,(LPARAM)(LPCTSTR)m_szLocDirectory);
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	szFtpInfo="进入"+m_szFtpDirectory+"目录成功";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	CString tFile;
	if(!(hFind=FtpFindFirstFile(hFtpConn,_T("*"),&fileData,0,0)))
		goto end;
	do
	{
		tFile=fileData.cFileName;
		if(tFile=="."||tFile==".."||fileData.dwFileAttributes==FILE_ATTRIBUTE_HIDDEN)continue;
		str=m_szLocDirectory;
		strFtp=m_szFtpDirectory;
		if(m_szLocDirectory.Right(1)=='\\')
			str+=tFile;
		else
			str+="\\"+tFile;
		if(m_szFtpDirectory.Right(1)=='/')
			strFtp+=tFile;
		else
			strFtp+="/"+tFile;
		
		if(fileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			pServView->SendMessage(WM_PRERECEIVEFILE,(WPARAM)(LPCTSTR)str,(LPARAM)(LPCTSTR)strFtp);
		else
		{
			szFtpInfo="正在接收"+tFile;
			pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			if(!FtpGetFile(hFtpConn,strFtp,str,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY |
				INTERNET_FLAG_NO_CACHE_WRITE,0))
			{			
				szFtpInfo="接收"+strFtp+"出错";
				pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				break;
			}
			if(IsFree)
			{
				IsFree=FALSE;
				strcpy(pInfo->szFileName,tFile);
				strcpy(pInfo->szFileDate,"");
				strcpy(pInfo->szFileSize,"");
				pInfo->nType=FILEICON;
				::PostMessage(pLocView->m_hWnd,WM_SETFILE,(WPARAM)pInfo,SERVFILE);
			}
		}
	}while(InternetFindNextFile(hFind,&fileData));
	InternetCloseHandle(hFind);
end:
	delete pInfo;
	delete pThread;
	nThreadCount--;
	return 0L;
}

UINT CMyAppThread::PreDeleteFileThread(LPVOID lParam)
{
	CMyAppThread* pThread=(CMyAppThread*)lParam;
	CFtpParaClass:: FILE_FTP_INFO* pInfo=new CFtpParaClass::FILE_FTP_INFO;
	WIN32_FIND_DATA  fileData;
	HINTERNET hFind;
	CString szFtpInfo,strFtp;
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	szFtpInfo="进入"+m_szFtpDirectory+"目录成功";
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	CString tFile;
	if(!(hFind=FtpFindFirstFile(hFtpConn,_T("*"),&fileData,0,0)))
		goto end;
	do
	{
		tFile=fileData.cFileName;
		if(tFile=="."||tFile==".."||fileData.dwFileAttributes==FILE_ATTRIBUTE_HIDDEN)continue;
		strFtp=m_szFtpDirectory;
		if(m_szFtpDirectory.Right(1)=='/')
			strFtp+=tFile;
		else
			strFtp+="/"+tFile;
		
		if(fileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			pServView->SendMessage(WM_PREDELETEFILE,0,(LPARAM)(LPCTSTR)strFtp);
		else
		{
			szFtpInfo="正在删除"+tFile;
			pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			if(!FtpDeleteFile(hFtpConn,strFtp))
			{			
				szFtpInfo="删除"+strFtp+"出错";
				pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				break;
			}
		}
	}while(InternetFindNextFile(hFind,&fileData));
	InternetCloseHandle(hFind);
end:
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory+"/"+"..");
	szFtpInfo="正在删除"+m_szFtpDirectory+"目录";
	pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	while(1)
	{
		if(FtpRemoveDirectory(hFtpConn,m_szFtpDirectory))
			//必须在此添加修改m_ctServDir的功能；
			break;
		::Sleep(100);
	}
	delete pInfo;
	delete pThread;
	nThreadCount--;
	return 0L;
}

UINT CMyAppThread::DeleteFileThread(LPVOID lParam)
{
	IsFree=TRUE;
	int i=0;
	CFtpParaClass:: FILE_FTP_INFO* pInf=new (CFtpParaClass::FILE_FTP_INFO);
	CFtpParaClass::SEND_RECEIVE_INFO* pInf1=(CFtpParaClass::SEND_RECEIVE_INFO*)lParam;
	CMyAppThread* pThread=(CMyAppThread*)pInf1->pThread;
	CFtpParaClass::FILE_COUNT_INFO* pInfo=pInf1->pCount;
    CString strFtp,szFtpInfo,DirName;
	if(!hFtpConn)
		goto end;
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	::SetCurrentDirectory(m_szLocDirectory);
	for(;i<m_nCount;i++)
	{
		strFtp=m_szFtpDirectory;
		DirName=pInfo[i].fileName;
		if(DirName=="..")continue;
		if(m_szFtpDirectory.Right(1)=='/')
			strFtp+=DirName;
		else
			strFtp+="/"+DirName;
		if(pInfo[i].ufileFlag==FILEICON)
		{
			szFtpInfo="删除"+DirName;
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			if(FtpDeleteFile(hFtpConn,strFtp))
			{
				continue;
			}
			else
			{
				szFtpInfo="删除"+DirName+"出错";
				pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				break;
			}
		}
		else;
			pServView->SendMessage(WM_PREDELETEFILE,0,(LPARAM)(LPCTSTR)strFtp);
	}
	
end:
	nThreadCount--;
	::Sleep(10);
	while(1)
	{
		if(nThreadCount==0)
		{
			szFtpInfo="文件删除结束,退回"+m_szLocDirectory;
			pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			::SendMessage(pServView->m_hWnd,WM_REDISPLAYFILE,0,(LPARAM)(LPCTSTR)m_szFtpDirectory);
			break;
		}
		::Sleep(100);
	}
	delete[] pInfo;
	delete pInf;
	delete pThread;
	delete pInf1;
	return 0L;
}

UINT CMyAppThread::RenameFileThread(LPVOID lParam)
{
	CString szFtpInfo;
	CFtpParaClass::FILE_CHANGE_INFO* pInfo=(CFtpParaClass::FILE_CHANGE_INFO*)lParam;
	CMyAppThread* pThread=(CMyAppThread*)pInfo->pThread;
	szFtpInfo="正在连接"+m_szFtpName+"服务器";
	pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	if(!hFtpConn)
	{
		szFtpInfo="未连上"+m_szFtpName+"服务器，请查明原因,目前只能返回";
		pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
		::Sleep(10);
		goto end;
	}
	szFtpInfo=m_szFtpName+"服务器连接成功,现进行改名操作";
	pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	FtpSetCurrentDirectory(hFtpConn,m_szFtpDirectory);
	szFtpInfo="正在把";
	szFtpInfo+=pInfo->OldName;
	szFtpInfo+="改为";
	szFtpInfo+=pInfo->NewName;
	pFtpInfoView->PostMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
    if(FtpRenameFile(hFtpConn,pInfo->OldName,pInfo->NewName))
		szFtpInfo="改名成功";
	else
	{
		szFtpInfo="改名失败，有可能有重名文件";
		pServView->PostMessage(WM_REDISPLAYFILE,0,(LPARAM)(LPCTSTR)m_szFtpDirectory);
	}
	pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
end:
	::Sleep(0);
	delete pThread;
	delete pInfo;
	return 0L;

}


void CLocFileView::OnDblclkLocFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NMLVDISPINFO* pLocInfo=(NMLVDISPINFO*)pNMHDR;
	CString str,buf;
	int iIcon,iIconSel;
	str.Format("%d",pLocInfo->item);
	if(str=="-1")return;
	int nItem=atoi(str);
	DWORD dwFlag=m_ctLocFile.GetItemData(nItem);
	if(dwFlag==FILEICON)
	{
		OnLocFileOpen();
		return;
	}
	buf=m_ctLocFile.GetItemText(nItem,0);
	nItem=m_ctLocDir.GetCurSel();
	m_ctLocDir.GetLBText(nItem,str);
	if(str.Right(1)!="\\")
		str+="\\";
	str+=buf;
	GetFileIcon(str,&iIcon,&iIconSel);
	int i=m_ctLocDir.FindStringExact(-1,str);
	if(i==CB_ERR)
	{
		int iCount=m_ctLocDir.GetCount();
		if(iCount==m_iDirCount)
		{
			i=InsertLocDir(str,iIcon,iIconSel);
		}
		else
		{
			m_ctLocDir.DeleteItem(m_iDirCount);
			i=InsertLocDir(str,iIcon,iIconSel);
		}
	}
	m_ctLocDir.SetCurSel(i);
	CMyAppThread* pThread=new CMyAppThread(str);
	BeginFindLocFile(pThread);
	*pResult = 0;
}

void CLocFileView::OnSize(UINT nType, int cx, int cy) 
{
//	CFormView::OnSize(nType, cx, cy);
	if(m_ctLocDir.m_hWnd)
		m_ctLocDir.SetWindowPos(this,0,0,cx-3,0,SWP_NOZORDER|SWP_NOMOVE);
	if(m_ctLocFile.m_hWnd)
	{
		m_ctLocFile.ShowWindow(SW_HIDE);
		m_ctLocFile.SetWindowPos(this,0,0,cx-1,cy-22,SWP_NOZORDER|SWP_NOMOVE);
		m_ctLocFile.ShowWindow(SW_SHOW);
	}
	
}

void CServFileView::OnSize(UINT nType, int cx, int cy) 
{
//	CFormView::OnSize(nType, cx, cy);
    if(m_ctServDir.m_hWnd&&m_ctBack.m_hWnd)
	{
		m_ctServDir.SetWindowPos(this,0,0,cx-30,0,SWP_NOZORDER|SWP_NOMOVE);
		m_ctBack.SetWindowPos(this,cx-28,0,0,0,SWP_NOZORDER|SWP_NOSIZE);
	}
	if(m_ctServFile.m_hWnd)
	{
		m_ctServFile.ShowWindow(SW_HIDE);
		m_ctServFile.SetWindowPos(this,0,0,cx,cy-22,SWP_NOZORDER|SWP_NOMOVE);
		m_ctServFile.ShowWindow(SW_SHOW);
	}	
}

BOOL CServFileView::SetServImageList(UINT nBitmapID)
{
	if(m_ctServImageList.Create(nBitmapID,16,1,RGB(0,255,0)))
	{
		m_ctServFile.SetImageList(&m_ctServImageList,LVSIL_SMALL);
		m_ctServDir.SetImageList(&m_ctServImageList);
	}
	else
		return FALSE;
	
	return TRUE;
}

void CLocFileView::OnSelchangeLocDir() 
{
	CString str;
	char lpBuffer[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,lpBuffer);
	int i=m_ctLocDir.GetCurSel();
	m_ctLocDir.GetLBText(i,str);
	if(str==lpBuffer)return;
	i=0;
	while(1)
	{
		if(!SetCurrentDirectory(str))
		{
			i++;
			if(i<2)
			{
				AfxMessageBox("请插入磁盘再试");
				continue;
			}
			str=lpBuffer;
			if((i=m_ctLocDir.FindStringExact(-1,str))!=-1)
				m_ctLocDir.SetCurSel(i);
		}
		else
		{
			CMyAppThread* pThread=new CMyAppThread(str);
			BeginFindLocFile(pThread);
		}
		break;
	}	
	
}

void CServFileView::OnDblclkServFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVDISPINFO* pLocInfo=(NMLVDISPINFO*)pNMHDR;
	CString str,buf;
	str.Format("%d",pLocInfo->item);
	if(str=="-1")
		return;
	int nItem=atoi(str);
	buf=m_ctServFile.GetItemText(nItem,0);
	DWORD dwFlag=m_ctServFile.GetItemData(nItem);
	if(dwFlag==FILEICON)
		return;
	nItem=m_ctServDir.GetCurSel();
	m_ctServDir.GetLBText(nItem,str);
	if(str!="/")
		str+="/";
	str+=buf;
	nItem=m_ctServDir.FindStringExact(-1,str);
	if(nItem==CB_ERR)
		nItem=OnInsertDir(&m_ctServDir,str,DIRECTORYICON,DIRECTORYSEL);
	m_ctServDir.SetCurSel(nItem);
	OnQuickConnect();
	if(buf=="..")
		m_ctServDir.DeleteItem(nItem);
	*pResult = 0;
}

void CLocFileView::OnBegindragLocFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bDragging)return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;    	
	*pResult = 0;
	int nItem=pNMListView->iItem;
	POINT pt=pNMListView->ptAction;
	m_pDragImageList=m_ctLocFile.CreateDragImage(nItem,&pt);
	m_bDragging=TRUE;
	m_pDragImageList->BeginDrag(0,CPoint(0,0));
	m_pDragImageList->DragEnter(NULL,pt);
	SetCapture();
	SetCursor(m_hDragCursor);
	
	
	*pResult = 0;
}

void CLocFileView::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(m_bDragging)
	{
		CRect rect;
		pServView->m_ctServFile.GetWindowRect(&rect);
		ClientToScreen(&point);
		if(rect.PtInRect(point)&&m_bConnected)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else
			SetCursor(m_hDragCursor);
		m_pDragImageList->DragMove(point);
	}	
	CFormView::OnMouseMove(nFlags, point);
}

void CLocFileView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(m_bDragging)
	{
		m_bDragging=FALSE;
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImageList;
		m_pDragImageList=NULL;
		CRect rect;
		pServView->m_ctServFile.GetWindowRect(&rect);
		ClientToScreen(&point);
		if(rect.PtInRect(point)&&m_bConnected)
			OnLocFileSend();
	}	
	CFormView::OnLButtonUp(nFlags, point);
}

void CLocFileView::OnLocFileSend()
{
	if(!m_bConnected)return;
	CString str,strFtp;
	int i=0;
	POSITION iPos;
	int nItem=pServView->m_ctServDir.GetCurSel();
	pServView->m_ctServDir.GetLBText(nItem,strFtp);
	nItem=m_ctLocDir.GetCurSel();
	m_ctLocDir.GetLBText(nItem,str);
	i=m_ctLocFile.GetSelectedCount();
	FILE_COUNT_INFO* pFileCount=new FILE_COUNT_INFO[i];
	SEND_RECEIVE_INFO* pSeRe=new SEND_RECEIVE_INFO;
	iPos=m_ctLocFile.GetFirstSelectedItemPosition();
	for(int j=0;j<i;j++)
	{
		int nItem=m_ctLocFile.GetNextSelectedItem(iPos);
		m_ctLocFile.GetItemText(nItem,0,pFileCount[j].fileName,256);
		pFileCount[j].ufileFlag=m_ctLocFile.GetItemData(nItem);
		
	}
	CMyAppThread* pThread=new CMyAppThread(str,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE,i);
	pSeRe->pCount=pFileCount;
	pSeRe->pThread=pThread;
	if(!BeginSendFile(pSeRe))
		AfxMessageBox("建立文件发送线程出错");
}

void CLocFileView::OnLocFileOpen()
{
	CString str,szFile;
	int nItem,i;
	nItem=m_ctLocDir.GetCurSel();
	if(nItem==CB_ERR)return;
	m_ctLocDir.GetLBText(nItem,str);
	szFile=str;
	POSITION iPos=m_ctLocFile.GetFirstSelectedItemPosition();
	nItem=m_ctLocFile.GetNextSelectedItem(iPos);
	i=m_ctLocFile.GetItemData(nItem);
	if(str.Right(1)=="\\")
		szFile+=m_ctLocFile.GetItemText(nItem,0);
	else
		szFile+="\\"+m_ctLocFile.GetItemText(nItem,0);
	if(i==FILEICON)
		::ShellExecute(m_hWnd,NULL,szFile,"open",str,SW_SHOWDEFAULT);
	else
	{
		CMyAppThread* pThread=new CMyAppThread(szFile);
		BeginFindLocFile(pThread);
	}

}
void CLocFileView::OnLocFileDelete()
{
	POSITION iPos=m_ctLocFile.GetFirstSelectedItemPosition();
	if(!iPos)return;
	SHFILEOPSTRUCT lpsh;
	CString str;
	int nItem;
	int i=m_ctLocFile.GetSelectedCount();
	ZeroMemory(&lpsh,sizeof(lpsh));
	lpsh.hwnd= HWND_DESKTOP;
	lpsh.fFlags=FOF_NOCONFIRMATION;
	lpsh.wFunc=FO_DELETE;
	for(int j=0;j<i;j++)
	{
		nItem=m_ctLocFile.GetNextSelectedItem(iPos);
		str=m_ctLocFile.GetItemText(nItem,0);
		char buf[MAX_PATH];
		strcpy(buf,str);
		buf[str.GetLength()+1]='\0';
		lpsh.pFrom=buf;
		if( 0 != SHFileOperation(&lpsh))
		{
			AfxMessageBox("删除文件出错,请检查");
			return ;
		}
	}
	nItem=m_ctLocDir.GetCurSel();
	if(nItem==CB_ERR)
		str="C:\\";
	else
		m_ctLocDir.GetLBText(nItem,str);
	CMyAppThread* pThread=new CMyAppThread(str);
	BeginFindLocFile(pThread);
	
}

void CLocFileView::OnLocFileReName()
{
	OnFileName(&m_ctLocFile);
}


void CLocFileView::OnRclickLocFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem=pNMListView->iItem;
	if(nItem==-1)return;
	SetPopMenu(0);
	*pResult = 0;
}

void CLocFileView::OnBeginlabeleditLocFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CLocFileView::OnEndlabeleditLocFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem;
	CString str,buf;
	nItem=m_ctLocDir.GetCurSel();
    m_ctLocDir.GetLBText(nItem,str);
	::SetCurrentDirectory(str);
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM * pItem=&pDispInfo->item;
	nItem=pItem->iItem;
	buf=m_ctLocFile.GetItemText(nItem,0);
	if(pItem->pszText&&buf!=pItem->iItem)
	{
		m_ctLocFile.SetItemText(pItem->iItem,pItem->iSubItem,pItem->pszText);
		::rename(buf,pItem->pszText);
		CMyAppThread* pThread=new CMyAppThread(str);
		BeginFindLocFile(pThread);
	}
	*pResult = 0;
}

LRESULT CLocFileView::OnSetLocDir(WPARAM wParam,LPARAM lParam)
{
	OnSetLocDirectoryDisplay(wParam,lParam);
	m_ctLocFile.SetRedraw(FALSE);
	m_ctLocFile.DeleteAllItems();
	m_ctLocFile.SetRedraw();
	return 0L;

}
/////////////////////////////////////////////////////////////////////////////
// CInputEdit

CInputEdit::CInputEdit(int iItem,int iSubItem,CString& szInitText,CListCtrl* pListCtrl)
{
	m_pListCtrl=pListCtrl;
	m_iItem=iItem;
	m_iSubItem=iSubItem;
	m_szInitText=szInitText;
	m_bEsc=FALSE;
}

CInputEdit::~CInputEdit()
{
}


BEGIN_MESSAGE_MAP(CInputEdit, CEdit)
	//{{AFX_MSG_MAP(CInputEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputEdit message handlers

void CInputEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_ESCAPE||nChar==VK_RETURN)
	{
		if(nChar==VK_ESCAPE)
			m_bEsc=TRUE;
		GetParent()->SetFocus();
		return;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
	CString str;
	GetWindowText(str);
	CWindowDC dc(this);
	CFont* pFont=GetParent()->GetFont();
	CFont* pFontDC=dc.SelectObject(pFont);
	CSize size=dc.GetTextExtent(str);
	dc.SelectObject(pFontDC);
	size.cx+=5;
	CRect rect,parentrect;
	GetClientRect(&rect);
	GetParent()->GetClientRect(&parentrect);
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);
	if(size.cx>rect.Width())
	{
		if(size.cx+rect.left<parentrect.right)
			rect.right=rect.left+size.cx;
		else
			rect.right=parentrect.right;
		MoveWindow(&rect);
	}
}

void CInputEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	CString str;
	GetWindowText(str);
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom=m_pListCtrl->m_hWnd;
	dispinfo.hdr.idFrom=GetDlgCtrlID();
	dispinfo.hdr.code=LVN_ENDLABELEDIT;
	dispinfo.item.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	dispinfo.item.iItem=m_iItem;
	dispinfo.item.iSubItem=m_iSubItem;
	dispinfo.item.pszText=m_bEsc?NULL:(LPTSTR)(LPCTSTR)str;
	dispinfo.item.cchTextMax=str.GetLength();
	GetParent()->SendMessage(WM_NOTIFY,GetParent()->GetDlgCtrlID(),(LPARAM)&dispinfo);	
	DestroyWindow();	
}

BOOL CInputEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam==VK_RETURN||pMsg->wParam==VK_DELETE||pMsg->wParam==VK_ESCAPE||
		GetKeyState(VK_CONTROL))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

int CInputEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	CFont* font=GetParent()->GetFont();
	SetFont(font);
	SetWindowText(m_szInitText);
	SetFocus();
	SetSel(0,-1);
	return 0;
}

void CInputEdit::OnNcDestroy() 
{
	CEdit::OnNcDestroy();
	delete this;	
}

BOOL CLocFileView::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE&&m_bDragging)
	{
		m_bDragging=FALSE;
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImageList;
		return TRUE;
	}	
	return CFormView::PreTranslateMessage(pMsg);
}

void CServFileView::OnBegindragServFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bDragging)return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;    	
	*pResult = 0;
	int nItem=pNMListView->iItem;
	POINT pt=pNMListView->ptAction;
	m_pDragImageList=m_ctServFile.CreateDragImage(nItem,&pt);
	m_bDragging=TRUE;
	m_pDragImageList->BeginDrag(0,CPoint(0,0));
	m_pDragImageList->DragEnter(NULL,pt);
	SetCapture();
}

void CServFileView::OnMouseMove(UINT nFlags, CPoint point) 
{
    if(m_bDragging)
	{
		CRect rect;
		pLocView->m_ctLocFile.GetWindowRect(&rect);
		ClientToScreen(&point);
		if(rect.PtInRect(point)&&m_bConnected)
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else
			SetCursor(m_hDragCursor);
		m_pDragImageList->DragMove(point);
	}	
	CFormView::OnMouseMove(nFlags, point);
}

BOOL CServFileView::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE&&m_bDragging)
	{
		m_bDragging=FALSE;
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImageList;
		return TRUE;
	}	
	return CFormView::PreTranslateMessage(pMsg);
}

void CServFileView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(m_bDragging)
	{
		m_bDragging=FALSE;
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImageList;
		m_pDragImageList=NULL;
		CRect rect;
		pLocView->m_ctLocFile.GetWindowRect(&rect);
		ClientToScreen(&point);
		if(rect.PtInRect(point)&&m_bConnected)
			OnServFileDownload();
	}
	CFormView::OnLButtonUp(nFlags, point);
}

BOOL CServFileView::BeginReceiveFile(LPVOID pInfo)
{
	SEND_RECEIVE_INFO* pInf=(SEND_RECEIVE_INFO*)pInfo;
	CMyAppThread* pThread=(CMyAppThread*)pInf->pThread;
	if(m_pReceiveFileThread)
	{
		::WaitForSingleObject(m_pReceiveFileThread->m_hThread,INFINITE);
		delete m_pReceiveFileThread;
		m_pReceiveFileThread=NULL;
	}
	if(m_pReceiveFileThread=AfxBeginThread(pThread->StartReceiveFileThread,pInfo,THREAD_PRIORITY_ABOVE_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		m_pReceiveFileThread->m_bAutoDelete=FALSE;
		nThreadCount++;
		m_pReceiveFileThread->ResumeThread();
		return TRUE;
	}
		return FALSE;
}

BOOL CServFileView::BeginPreReceiveFile(LPVOID pInfo)
{
	CMyAppThread* pThread=(CMyAppThread*)pInfo;
	CWinThread* pPreReceiveFile=NULL;
	if(pPreReceiveFile=AfxBeginThread(pThread->StartPreReceiveFileThread,pInfo,THREAD_PRIORITY_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		nThreadCount++;
		pPreReceiveFile->ResumeThread();
		return TRUE;
	}
	return FALSE;
}

BOOL CServFileView::BeginPreDeleteFile(LPVOID pInfo)
{
	CMyAppThread* pThread=(CMyAppThread*)pInfo;
	CWinThread* pPreDeleteFile=NULL;
	if(pPreDeleteFile=AfxBeginThread(pThread->StartPreDeleteFileThread,pInfo,THREAD_PRIORITY_NORMAL,
		0,CREATE_SUSPENDED,NULL))
	{
		nThreadCount++;
		pPreDeleteFile->ResumeThread();
		return TRUE;
	}
	return FALSE;
}

LRESULT CServFileView::OnPreReceiveFile(WPARAM wParam,LPARAM lParam)
{
	CString str=(LPCTSTR)wParam;
	CString strFtp=(LPCTSTR)lParam;
	CMyAppThread* pThread=new CMyAppThread(str,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE);
	if(!BeginPreReceiveFile(pThread))
		AfxMessageBox("起动发送子线程失败");
	return 0L;

}

LRESULT CServFileView::OnPreDeleteFile(WPARAM wParam,LPARAM lParam)
{
	CString strFtp=(LPCTSTR)lParam;
	CMyAppThread* pThread=new CMyAppThread(NULL,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE);
	if(!BeginPreDeleteFile(pThread))
		AfxMessageBox("起动发送子线程失败");
	return 0L;

}

void CServFileView::OnRclickServFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	int nItem=pNMListView->iItem;
	if(nItem==-1||!m_bConnected)return;
	SetPopMenu(1);
	
}

void CServFileView::OnBeginlabeleditServFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	*pResult = 0;
}

void CServFileView::OnEndlabeleditServFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem;
	CString str,strFtp;
	nItem=m_ctServDir.GetCurSel();
	if(nItem==CB_ERR)return;
	m_ctServDir.GetLBText(nItem,strFtp);
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM * pItem=&pDispInfo->item;
	nItem=pItem->iItem;
	FILE_CHANGE_INFO* pInfo=new FILE_CHANGE_INFO;
	str=m_ctServFile.GetItemText(nItem,0);
	if(pItem->pszText&&str!=pItem->pszText)
	{
		CMyAppThread* pThread=new CMyAppThread(NULL,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
			m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),strFtp,TRUE);
		pInfo->pThread=pThread;
		strcpy(pInfo->OldName,str);
		strcpy(pInfo->NewName,pItem->pszText);
		if(!BeginRenameFile(pInfo))
			AfxMessageBox("建立文件重命名线程出错，返回");
		m_ctServFile.SetItemText(pItem->iItem,pItem->iSubItem,pItem->pszText);
	}
	*pResult = 0;
}

void CServFileView::OnBack() 
{
	CString str;
	int nItem=m_ctServDir.GetCurSel();
	if(nItem==CB_ERR)return;
	m_ctServDir.GetLBText(nItem,str);
	if(str=="/")return;
	if(str.Right(1)!="/")
		str+="/";
	str+="..";
	nItem=OnInsertDir(&m_ctServDir,str,DIRECTORYICON,DIRECTORYSEL);
	m_ctServDir.SetCurSel(nItem);
	OnQuickConnect();
	m_ctServDir.DeleteItem(nItem);
}

void CServFileView::OnQuickConnect()
{
	CString str;
	int nItem=m_ctServDir.GetCurSel();
	if(nItem!=CB_ERR)
		m_ctServDir.GetLBText(nItem,str);
	else
		str=" ";
	CMyAppThread* pThread=new CMyAppThread(NULL,m_pMainFrame->GetFtpName(),m_pMainFrame->GetFtpUser(),
		m_pMainFrame->GetFtpPassword(),m_pMainFrame->GetFtpPort(),str,TRUE);
	if(!BeginFindServFile(pThread))
		AfxMessageBox("建立网络线程失败");

}


int CFtpInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
    pFtpInfoView=this;
	m_NewFont.CreatePointFont(70,"Arial",NULL);
	GetEditCtrl().SetFont(&m_NewFont,true);
	return 0;
}

void CFtpInfoView::OnChange() 
{
	return;
}



void CFtpInfoView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
// 	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnFileDisconnect() 
{
	CString str,szFtpInfo;
    m_wndMyDialogBar.GetDlgItem(IDC_FTPNAME)->GetWindowText(str);
	szFtpInfo="已中断与"+str+"的连接";
		pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	InternetCloseHandle(hInetSession);
	m_wndMyDialogBar.GetDlgItem(IDC_QUICKCONNECT)->ShowWindow(SW_HIDE);
	m_bConnected=FALSE;
}


void CMainFrame::OnUpdateFileDisconnect(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_bConnected);
   
}
void CMainFrame::OnFileReconnect()
{
	OnQuickconnect();
}


void CMainFrame::OnFileConnect() 
{
	CString szFtpInfo;
	if(hInetSession=InternetOpen(AfxGetAppName(),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0))
	{
		szFtpInfo="已建立连接会话，请选择《快速连接》按钮连接至你所需要的文件服务器";
		m_bConnected=TRUE;
		m_wndMyDialogBar.GetDlgItem(IDC_QUICKCONNECT)->ShowWindow(SW_SHOW);
		pFtpInfoView->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	}
//	OnQuickconnect();
}

void CMainFrame::OnUpdateFileConnect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bConnected);
	
}

void CMainFrame::OnUpdateFileReconnect(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_bConnected);	
}

void CMainFrame::OnFileStop() 
{
	m_bConnected=FALSE;
	
}

void CMainFrame::OnUpdateFileStop(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_bConnected);	
}

void CMainFrame::OnUpdateFileSend(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_bConnected);	
}

void CMainFrame::OnUpdateFileFtpDownload(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_bConnected);	
}
/////////////////////////////////////////////////////////////////////////////
// CDialDlg dialog


CDialDlg::CDialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialDlg)
	m_szPassword = _T("");
	m_szUserID = _T("");
	//}}AFX_DATA_INIT
}


void CDialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialDlg)
	DDX_Control(pDX, IDC_INETNUMBER, m_ctEntryName);
	DDX_Text(pDX, IDC_INETPASSWORD, m_szPassword);
	DDX_Text(pDX, IDC_INETUSER, m_szUserID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialDlg, CDialog)
	//{{AFX_MSG_MAP(CDialDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialDlg message handlers

BOOL CDialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServFileView::OnSelchangeServDir() 
{
	OnQuickConnect();	
}

