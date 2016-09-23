#pragma once
#include "afxcmn.h"
#include "MyList.h"


// CSelectKernelModuleHook 对话框

class CSelectKernelModuleHook : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectKernelModuleHook)

public:
	CSelectKernelModuleHook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectKernelModuleHook();

// 对话框数据
	enum { IDD = IDD_DLG_SKMHOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyList m_SKMHOOKList;
	virtual BOOL OnInitDialog();
};
