#pragma once
#include "afxcmn.h"
#include<vector>
#include "ProcessManager.h"
using namespace std;

typedef struct _PROCESS_WND_ENTRY_INFOR_
{
	HWND  hWnd;
	ULONG ulPid;
	ULONG ulTid;
}PROCESS_WND_ENTRY_INFOR, *PPROCESS_WND_ENTRY_INFOR;

class CProcessWind : public CDialog
{
	DECLARE_DYNAMIC(CProcessWind)

public:
	ULONG_PTR EnumWndInfor();
	VOID InitControl();
	VOID AddItemToControlList(PROCESS_WND_ENTRY_INFOR ProcessWndEntryInfor);

public:
	CProcessManager* m_ProcessManagerDlg;
	vector<PROCESS_WND_ENTRY_INFOR> m_Vector;

public:
	CProcessWind(CWnd* pParent = NULL);  
	virtual ~CProcessWind();
	enum { IDD = IDD_DLG_PROCESS_WND };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnProcessWndRefresh();
	afx_msg void OnProcessWndChagdData();
	afx_msg void OnProcessWndShow();
	afx_msg void OnProcessWndHide();
	afx_msg void OnProcessWndEnable();
	afx_msg void OnProcessWndDisabled();
	afx_msg void OnNMRClickListWnd(NMHDR *pNMHDR, LRESULT *pResult);
};
