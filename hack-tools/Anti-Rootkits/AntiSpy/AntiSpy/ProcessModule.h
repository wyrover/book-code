#pragma once
#include "afxcmn.h"
#include <vector>
#include "ProcessManager.h"

using namespace std;

#define ENUM_MODULE 10
#define UNLOAD_MODULE 20

typedef struct _PROCESS_MODULE_ENTRY_INFOR_
{
	ULONG_PTR ModuleBase;
	ULONG     ModuleSize;
	WCHAR     ModulePath[MAX_PATH];
}PROCESS_MODULE_ENTRY_INFOR, *PPROCESS_MODULE_ENTRY_INFOR;

typedef struct _PROCESS_MODULE_INFORMATION
{
	ULONG_PTR    NumberOfModule;
	PROCESS_MODULE_ENTRY_INFOR Module[1];
}PROCESS_MODULE_INFORMATION, *PPROCESS_MODULE_INFORMATION;


class CProcessModule : public CDialog
{
	DECLARE_DYNAMIC(CProcessModule)

public:
	CProcessManager* m_ProcessManagerDlg;
public:
	CProcessModule(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessModule();

	VOID GetProcessModuleInfo();

	BOOL EnumModuleInfor();
//	BOOL UnloadModule();

	BOOL SendIoControlCode(ULONG_PTR ulControlCode);
	VOID AddItemToControlList(PROCESS_MODULE_ENTRY_INFOR ProcessModuleEntryInfor);


	enum { IDD = IDD_PROCESS_MODULE_DIALOG };

	vector<PROCESS_MODULE_ENTRY_INFOR> m_Vector;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
};
