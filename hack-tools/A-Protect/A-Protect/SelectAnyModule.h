#pragma once
#include "afxcmn.h"


// CSelectAnyModule 对话框

class CSelectAnyModule : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectAnyModule)

public:
	CSelectAnyModule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectAnyModule();

// 对话框数据
	enum { IDD = IDD_DLG_SELECTANYMODELE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_SelectAnyModuleList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSelectall();
	afx_msg void OnBnClickedBtnCancelSelectall();
	afx_msg void OnBnClickedBtnScan();
};
