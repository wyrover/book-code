#pragma once
#include "afxcmn.h"
#include "MyList.h"

// CStackThread 对话框

class CStackThread : public CDialogEx
{
	DECLARE_DYNAMIC(CStackThread)

public:
	CStackThread(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStackThread();

// 对话框数据
	enum { IDD = IDD_DLG_STACKTHREAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strStackThread;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnThreadStackByPDB();

	CListCtrl	m_ListCtrl;
};
