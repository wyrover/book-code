#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// 记录进程信息的结构体
typedef struct tagProcessInfo     
{
	ULONG address;  // 进程地址           
	LONG pid;  // 进程ID
	UCHAR name[16];  // 进程名
	struct tagProcessInfo *next;  // 单向链表指针
}ProcessInfo, *PProcessInfo;


// CProcessManager 对话框

class CProcessManager : public CDialog
{
	DECLARE_DYNAMIC(CProcessManager)

public:
	CProcessManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessManager();

// 对话框数据
	enum { IDD = IDD_PROCESSMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();  // 初始化对话框
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  // 改变对话框和控件背景色
	afx_msg void OnBnClickedButtonListprocessRing3();  // Ring3列举进程按钮事件
	afx_msg void OnBnClickedButtonPspcidtable();  // 通过Pspcidtable列举进程
	afx_msg void OnNMRclickListProcess(NMHDR *pNMHDR, LRESULT *pResult);  // 列表框单击右键事件
	afx_msg void OnMenuTerminateProcessRing0();  // Ring0结束进程
	afx_msg void OnNMCustomdrawListProcess(NMHDR *pNMHDR, LRESULT *pResult);  // 绘制列表
	afx_msg void OnBnClickedButtonActiveProcessLinks();  // 通过ActiveProcessLinks列举进程
	afx_msg void OnBnClickedButtonObjectTable();  // 通过ObjectTable列举进程
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);  // 设置鼠标样式
private:
	BOOL AdjustPurview();  // 调整权限，主要是为了OpenProcess成功返回Handle

private:
	CBrush m_brush;  // 自定义画刷
	CListCtrl m_ListProcess;  // 进程列举列表框	
};
