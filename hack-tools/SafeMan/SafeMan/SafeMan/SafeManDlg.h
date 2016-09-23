// SafeManDlg.h : 头文件
//

#pragma once

#include "CDialogSK.h"
#include "HoverButton.h"
#include "TrayIcon.h"
#include "ProcessManager.h"
#include "SSDTManager.h"
#include "MonitorManager.h"
#include "afxwin.h"

#define WM_ICON_NOTIFY WM_USER+1  // 自定义托盘消息


// CSafeManDlg 对话框
class CSafeManDlg : public CDialogSK
{
// 构造
public:
	CSafeManDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSafeManDlg();	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SAFEMAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonProcessmanager();
	afx_msg void OnBnClickedButtonSsdtmanager();
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedExit();
	afx_msg void OnMenuItemSafeman();
	afx_msg void OnMenuItemQuit();
	LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);  // 托盘消息
	afx_msg void OnBnClickedTray();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);  // 设置鼠标样式

private:
	CHoverButton m_ProcessManager;
	CProcessManager* m_pProcessManager; 
	CHoverButton m_SSDTManager;
	CSSDTManager* m_pSSDTManager; 
	//
	CHoverButton m_MonitorManager;
	CMonitorManager* m_pMonitorManager;
	//
	CHoverButton m_Min;
	CHoverButton m_Exit;
	CHoverButton m_Tray;
	CTrayIcon m_TrayIcon;  // 托盘变量
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonProcessmonitormanager();
};
