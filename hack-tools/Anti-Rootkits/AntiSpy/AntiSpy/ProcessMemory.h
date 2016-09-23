#pragma once
#include "afxcmn.h"
#include <vector>
#include "ProcessManager.h"
#include "ReadProcessMemory.h"
using namespace std;

#define  ENUM_MEMORY 10
#define  MODIFY_MEMORY 20 


typedef struct _PROCESS_MEMORY_ENTRY_INFOR_
{
	ULONG_PTR ulBase;
	ULONG_PTR ulSize;
	ULONG ulProtect;   
	ULONG ulState;
	ULONG ulType;
}PROCESS_MEMORY_ENTRY_INFOR, *PPROCESS_MEMORY_ENTRY_INFOR;

typedef struct _PROCESS_MEMORY_INFORMATION_
{
	ULONG_PTR             NumberOfMemorys;
	PROCESS_MEMORY_ENTRY_INFOR Memorys[1];
}PROCESS_MEMORY_INFORMATION, *PPROCESS_MEMORY_INFORMATION;


class CProcessMemory : public CDialog
{
	DECLARE_DYNAMIC(CProcessMemory)

public:
	BOOL SendIoControlCode(ULONG_PTR ulControlCode);
	BOOL EnumMemoryInfor();
	VOID AddItemToControlList(PROCESS_MEMORY_ENTRY_INFOR ProcessMemoryEntryInfor);
	BOOL ModifyMemoryProtect();
public:
	ULONG_PTR  m_Base;
	ULONG_PTR  m_Size;
	ULONG      m_NewProtect;
	CProcessManager* m_ProcessManagerDlg;
	vector<PROCESS_MEMORY_ENTRY_INFOR> m_Vector;

public:
	CProcessMemory(CWnd* pParent = NULL);
	~CProcessMemory()
	{
		m_Vector.clear();
	}

	enum { IDD = IDD_DLG_PROCESS_MEMORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickListMemory(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProcessProtectNoaccess();
	afx_msg void OnProcessProtectRead();
	afx_msg void OnProcessProtectReadwrite();
	afx_msg void OnProcessProtectReadwriteGuard();
	afx_msg void OnProcessProtectReadExecute();
	afx_msg void OnProcessProtectWriteCopy();
	afx_msg void OnProcessMemoryRead();
	afx_msg void OnProcessMemoryWrite();
};
