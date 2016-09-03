// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "CoolTabCtrl.h"
#include "PrevDlg.h"

class CMainFrame : public CFrameWnd
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CMainFrame)

// 重写
public:
	virtual ~CMainFrame();

	// 获得指定目录下所有的图像文件名
	static void GetAllFileNames(const CString& path, CStringArray& ary);
													
	/**************************************************************************
	*	作用:			更新状态栏的指示器
	*	参数:		
	*		nID			指示器的ID
	*		strShow		指示器显示的字符串
	**************************************************************************/
	void UpdateIndicator(int nID, const CString& strShow)
	{
		int nIndex = m_wndStatusBar.CommandToIndex(nID);
		m_wndStatusBar.SetPaneText(nIndex, strShow);

		int nLength = strShow.GetLength();		// 根据文本改变指示器大小
		m_wndStatusBar.SetPaneInfo(nIndex, nID, SBPS_NORMAL, nLength * 8);
	}

	// 消息响应函数
	afx_msg void OnViewControl();				// 显示/隐藏“控制窗口”
	afx_msg void OnUpdateViewControl(CCmdUI *pCmdUI);
												// “控制窗口”菜单项的命令更新函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
												// 展开树视图的某一项的响应函数
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
												// 树视图选择改变的响应函数
	afx_msg void OnSize(UINT nType, int cx, int cy);
												// 窗口大小改变的响应函数
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;					// 状态栏
	CToolBar    m_wndToolBar;					// 工具栏

	CMainFrame();

private:
	CCoolBar		m_wndCtrlBar;				// 浮动控制窗口
	CCoolTabCtrl	m_TabCtrl;					// 选项卡控件, 用于管理子窗口
	CTreeCtrl		m_TreeCtrl;					// 树型控件,用来显示目录树
	HTREEITEM		m_hRoot;					// 保存一个树型节点

	BOOL CreateCtrlBar(void);					// 创建控制窗口
	void AddSubDir(HTREEITEM hParent);			// 插入目录下的所有子项
	CString GetFullPath(HTREEITEM hCurrent);	// 获取指定节点的全路径
	void InsertDriveDir(HTREEITEM hParent);		// 插入驱动盘下的所有子项
	void InsertLogicalDrives(HTREEITEM hParen);	// 插入系统驱动器节点
};