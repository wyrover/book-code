#pragma once
#include "afxcmn.h"
#include "MyList.h"


// CCProcessSearch 对话框

class CCProcessSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CCProcessSearch)

public:
	CCProcessSearch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCProcessSearch();

// 对话框数据
	enum { IDD = IDD_DLG_PROCESSSEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFindInfo;
	CMyList m_listFindInfo;
	int idFrom;//该id用来区分是从哪里调用的该对话框，对来进行相应的初始化工作。
	virtual BOOL OnInitDialog();
	afx_msg void SearchDllModuleInfo(CMyList *m_listInfo,int Type);
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedStopSearch();
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
};
