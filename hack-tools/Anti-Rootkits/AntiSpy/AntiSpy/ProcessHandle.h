#pragma once
#include "afxcmn.h"
#include <vector>
#include "IOControlCode.h"
#include "ProcessManager.h"
using namespace std;

#define ENUM_HANDLE   20
#define KILL_HANDLE  10

typedef struct _PROCESS_HANDLE_ENTRY_INFOR_
{
	ULONG_PTR      Handle;
	ULONG_PTR      Object;
	ULONG_PTR      ReferenceCount;
	WCHAR          wzHandleType[MAX_PATH];
	WCHAR          wzHandleName[512];
}PROCESS_HANDLE_ENTRY_INFOR, *PPROCESS_HANDLE_ENTRY_INFOR;

typedef struct _PROCESS_HANDLE_INFORMATION
{
	ULONG_PTR NumberOfHandles;
	PROCESS_HANDLE_ENTRY_INFOR Handles[1];
}PROCESS_HANDLE_INFORMATION, *PPROCESS_HANDLE_INFORMATION;



class CProcessHandle : public CDialog
{
	DECLARE_DYNAMIC(CProcessHandle)

public:
	VOID InitControl();
	BOOL SendIoControlCode(ULONG_PTR ulControlCode);
	BOOL EnumInfor();
	BOOL KillHandle();
	VOID AddItemToControlList(PROCESS_HANDLE_ENTRY_INFOR ProcessHandleEntryInfor);

public:
	vector<PROCESS_HANDLE_ENTRY_INFOR> m_Vector;
	CProcessManager* m_ProcessManagerDlg;
public:
	CProcessHandle(CWnd* pParent = NULL);   
	virtual ~CProcessHandle();
	enum { IDD = IDD_DLG_PROCESS_HANDLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	ULONG     m_EditHandleCount;
	ULONG_PTR m_Handle;
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	afx_msg void OnProcessHandleRefresh();
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProcessHandleClose();
};
