#pragma once
#include "Afxcmn.h"
#include "MyList.h"

// CProcessModule 对话框


class CSubModule : public CDialogEx
{
	DECLARE_DYNAMIC(CSubModule)

public:
	CSubModule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSubModule();

// 对话框数据
	enum { IDD = IDD_DLG_PROMODULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyList m_SubList;//Process Module ListView
	afx_msg void OnBnClickedBtnClose();
	virtual BOOL OnInitDialog();
	int m_Subcase;//区分listview的信息。
	int m_SubItem;//选定的行号
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnUnLoadDLLModule();
	afx_msg void OnUnLoadDLLModuleAndDelete();
	afx_msg void OnCopyDLLDataToClipboard();
	afx_msg void OnLookupDLLInServices();
	afx_msg void OnTrustDLLModule();
	afx_msg void OnProcessThreadList();
	afx_msg void OnKillProcessThread();
	afx_msg void OnSuspendProcessThread();
	afx_msg void OnResumeProcessThread();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
