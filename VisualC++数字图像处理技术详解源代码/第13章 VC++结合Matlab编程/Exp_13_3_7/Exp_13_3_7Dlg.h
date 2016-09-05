// Exp_13_3_7Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "dib.h"


// CExp_13_3_7Dlg 对话框
class CExp_13_3_7Dlg : public CDialog
{
// 构造
public:
	CExp_13_3_7Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXP_13_3_7_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnOpen();
public:
    afx_msg void OnBnClickedBtnCvt();
public:
    afx_msg void OnBnClickedBtnSave();
public:
    afx_msg void OnBnClickedBtnExit();
public:
    // 应用程序所在路径
    CString m_strAppPath;
public:
    // 图像文件名称
    CString m_strFile;
public:
    // 图像控件
    CStatic m_stcPic;
public:
    // 位图操作对象
    CDib m_dib;
public:
    afx_msg void OnDestroy();
public:
    // 显示位图
    void showPic(void);
};
