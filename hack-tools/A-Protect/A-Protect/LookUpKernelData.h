#pragma once
#include "afxwin.h"


// CLookUpKernelData 对话框

class CLookUpKernelData : public CDialogEx
{
	DECLARE_DYNAMIC(CLookUpKernelData)

public:
	CLookUpKernelData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLookUpKernelData();

// 对话框数据
	enum { IDD = IDD_DLG_KERNALDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ccbKernelBase;
	CString m_strLookUpSize;
	CString m_strKernelData;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnok();
};
