// Exp_13_4_2Dlg.h : 头文件
//

#pragma once


// CExp_13_4_2Dlg 对话框
class CExp_13_4_2Dlg : public CDialog
{
// 构造
public:
	CExp_13_4_2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXP_13_4_2_DIALOG };

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
    afx_msg void OnBnClickedBtnOpenfile();
public:
    afx_msg void OnBnClickedBtnExecute();
public:
    afx_msg void OnBnClickedBtnExit();
public:
    afx_msg void OnDestroy();
public:
    CString m_strFile;
};
