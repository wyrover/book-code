// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "GraphShower.h"

#include "MainFrm.h"

#include "GraphShowerDoc.h"
#include "GraphShowerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_MYTREECTRL, OnItemexpandedTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MYTREECTRL, OnSelchangedTree)
	ON_COMMAND(ID_VIEW_CONTROL, &CMainFrame::OnViewControl)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CONTROL, &CMainFrame::OnUpdateViewControl)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_PICNUM,	// 图片数量
	ID_INDICATOR_PICNAME,	// 当前显示的图片文件名
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CreateCtrlBar();

	return 0;
}

/******************************************************************************
*	作用:		创建控制窗口
*	备注:		控制窗口包括文件目录和位图浏览两个窗口,通过
******************************************************************************/
BOOL CMainFrame::CreateCtrlBar(void)
{
	if ( !m_wndCtrlBar.Create(L"控制窗口", this, 100) ) 
	{ 
		TRACE0("未能创建控制窗口\n"); 
		return -1; 
	}

	m_wndCtrlBar.SetBarStyle(m_wndCtrlBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC); 

	m_wndCtrlBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndCtrlBar, AFX_IDW_DOCKBAR_LEFT);	// 停靠在左边

	// 创建一个选项卡控件
	m_TabCtrl.Create(TCS_DOWN | WS_CHILD | WS_VISIBLE, CRect(0,0,100,100), &m_wndCtrlBar, 125);

	// 创建树型控件
	if (!m_TreeCtrl.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_TabCtrl, IDC_MYTREECTRL))
	{
		TRACE0("创建失败！\n");
		return -1;
	}

	m_TreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	DWORD dwStyle = GetWindowLong(m_TreeCtrl.m_hWnd, GWL_STYLE);

	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_TreeCtrl.m_hWnd, GWL_STYLE, dwStyle);

	m_hRoot = m_TreeCtrl.InsertItem(L"我的电脑");
	InsertLogicalDrives(m_hRoot);
	InsertDriveDir(m_hRoot);
	m_TreeCtrl.Expand(m_hRoot,TVE_EXPAND);

	// 将树型控件加入到TabCtrl中
	m_TabCtrl.AddPage(&m_TreeCtrl, L"文件目录", IDI_DIR);
	m_TabCtrl.AddPage(RUNTIME_CLASS(CPrevDlg), IDD_PREV, L"位图浏览", IDI_DIR);
	m_TabCtrl.UpdateWindow();

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style &= ~FWS_ADDTOTITLE;	// 取消MFC自动添加标题的风格

	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序

/******************************************************************************
*	作用:		在指定父节点下插入系统驱动器节点
******************************************************************************/
void CMainFrame::InsertLogicalDrives(HTREEITEM hParent)
{
	DWORD	szAllDriveStrings = GetLogicalDriveStrings(0,NULL);
	LPWSTR	pDriveStrings = new WCHAR[szAllDriveStrings + sizeof(_T(""))];
	LPWSTR	pDel = pDriveStrings;
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = wcslen(pDriveStrings);
	while(szDriveString > 0)
	{
		m_TreeCtrl.InsertItem(pDriveStrings, hParent);
		pDriveStrings += szDriveString + 1;
		szDriveString = wcslen(pDriveStrings);
	}

	SAFE_DELETE_ARRAY (pDel);
}

/******************************************************************************
*	作用:		在指定父节点下插入驱动盘下的所有子项
******************************************************************************/
void CMainFrame::InsertDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hParent);
	while(hChild)
	{
		CString strText = m_TreeCtrl.GetItemText(hChild);
		if(strText.Right(1) != L"\\")
			strText += L"\\";
		strText += L"*.*";
		CFileFind file;
		BOOL bContinue = file.FindFile(strText);
		while(bContinue)
		{
			bContinue = file.FindNextFile();
			if(!file.IsDots())
				m_TreeCtrl.InsertItem(file.GetFileName(), hChild);
		}
		InsertDriveDir(hChild);
		hChild = m_TreeCtrl.GetNextItem(hChild, TVGN_NEXT);
		file.Close();
	}	
}

/******************************************************************************
*	作用:		在指定父节点下插入目录下的所有子项
******************************************************************************/
void CMainFrame::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);
	if(strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += L"*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);
	while(bContinue)
	{
		bContinue = file.FindNextFile();
		if( !file.IsDots())
			m_TreeCtrl.InsertItem(file.GetFileName(), hParent);
	}
}

/******************************************************************************
*	作用:		获取指定节点的全路径
******************************************************************************/
CString CMainFrame::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = L"";
	while(hCurrent != m_hRoot)
	{
		strTemp = m_TreeCtrl.GetItemText(hCurrent);
		if(strTemp.Right(1) != "\\")
			strTemp += L"\\";
		strReturn = strTemp  + strReturn;
		hCurrent = m_TreeCtrl.GetParentItem(hCurrent);
	}

	return strReturn;
}

/******************************************************************************
*	作用:		展开树视图的某一项的响应函数
******************************************************************************/
void CMainFrame::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(item.hItem);
	while(hChild)
	{
		AddSubDir(hChild);
		hChild = m_TreeCtrl.GetNextItem(hChild,TVGN_NEXT);
	}
	*pResult = 0;
}

/******************************************************************************
*	作用:		获得指定目录下所有的图像文件名
*	参数:		
*		path	指定目录路径
*		ary		保存图像文件名的字符串容器
******************************************************************************/
void CMainFrame::GetAllFileNames(const CString& path, CStringArray& ary)
{
	ary.RemoveAll();

	CFileFind fileFind;
	CString filesPath = path + L"\\*.*";

	if (fileFind.FindFile(filesPath))
	{

		CString strPicPath;
		CString strExt;

		// 如果该目录还有文件
		while (fileFind.FindNextFile())
		{
			strPicPath = fileFind.GetFileName();
			// 获取后缀名
			strExt = strPicPath.Right(3);
			strExt.MakeLower();

			// 如果后缀名为bmp,jpg则判断是图片文件
			// 此处可以扩充功能
			if (strExt == "bmp" || strExt == "jpg")
				ary.Add(strPicPath);
		}

		strPicPath = fileFind.GetFileName();
		// 获取后缀名
		strExt = strPicPath.Right(3);
		strExt.MakeLower();

		// 如果后缀名为bmp,jpg则判断是图片文件
			// 此处可以扩充功能
		if (strExt == "bmp" || strExt == "jpg")
			ary.Add(strPicPath);
	}

	fileFind.Close();
}

/******************************************************************************
*	作用:		树视图选择改变的响应函数
******************************************************************************/
void CMainFrame::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
		return;
	CString str = GetFullPath(item.hItem);
	str = str.Left(str.GetLength() - 1);
	CFileFind fileFile;

	if( fileFile.FindFile(str) )
	{
		CString strExt;

		fileFile.FindNextFile();
		if(!fileFile.IsDirectory() && !fileFile.IsDots())
		{
			strExt = str.Right(3);
			strExt.MakeLower();
		
			// 如果后缀名为bmp,jpg则判断是图片文件
			// 此处可以扩充功能
			if (strExt == "bmp" || strExt == "jpg")
			{
				CGraphShowerView* pView = (CGraphShowerView*)GetActiveView();

				pView->ClearOffset();
				pView->OpenFile(str);
				pView->FixOffset();
				pView->RefreshIndicator();
			}
		}
	}
	fileFile.Close();

	*pResult = 0;
}

/******************************************************************************
*	作用:		窗口大小改变的响应函数
******************************************************************************/
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	CGraphShowerView* pView = (CGraphShowerView*)GetActiveView();

	if (pView != NULL)
	{
		pView->ClearOffset();
		pView->FixOffset();
	}
}


/******************************************************************************
*	作用:		显示/隐藏“控制窗口”
******************************************************************************/
void CMainFrame::OnViewControl()
{
	BOOL bVisible = ((m_wndCtrlBar.IsWindowVisible()) != 0);

	ShowControlBar(&m_wndCtrlBar, !bVisible, FALSE);
	RecalcLayout();
}

/******************************************************************************
*	作用:		“控制窗口”菜单项的命令更新函数
******************************************************************************/
void CMainFrame::OnUpdateViewControl(CCmdUI *pCmdUI)
{
	BOOL bVisible = ((m_wndCtrlBar.IsWindowVisible()) != 0);
	pCmdUI->SetCheck(bVisible);
}
