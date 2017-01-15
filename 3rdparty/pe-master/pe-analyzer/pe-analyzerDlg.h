/*
 * Copyright 2010 JiJie Shi
 *
 * This file is part of PEMaster.
 *
 * PEMaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PEMaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PEMaster.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

// pe-analyzerDlg.h : 头文件
//

#pragma once

#define WM_START_FILE_ANALYZE ( WM_USER + 0x101 )
#define WM_MAIN_TREE_ITEM_SELED ( WM_USER + 0x102 )
#define WM_LIST_DETAIL_ITEM_SELED ( WM_USER + 0x103 )
#define WM_MAIN_TREE_ITEM_RCLICK ( WM_USER + 0x104 )
#define MENU_ITEM_ID_DUMP_FILE	( 0x1001 )
#define WM_DO_UI_WORK ( WM_USER + 0x105 )

typedef struct __analyze_context
{
	char file_path[ MAX_PATH ];
	HWND main_wnd;
	HWND tree_main;
	HWND list_detail;
	file_analyzer analyzer;
	HANDLE start_event;
	HANDLE analyze_thread;
	dword thread_id;
} analyze_context;

// CpeanalyzerDlg 对话框
class CpeanalyzerDlg : public CDialog
{
// 构造
public:
	CpeanalyzerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PEANALYZER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	analyze_context analyzing_context;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg VOID OnBnClickedButtonSelFile();
	afx_msg void OnBnClickedStopAnalyze();
	afx_msg void OnTvnSelchangedTreeMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClientTreeMain( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg LRESULT OnDoUIWork( WPARAM wParam, LPARAM lParam );
	afx_msg void OnLvnItemchangedListDetail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles( HDROP hDropInfo );
};
