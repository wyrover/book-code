#pragma once
#include "afxcmn.h"
#include "IOControlCode.h"
#include <vector>
#include<WINDOWS.H>
using namespace std;

#define  FORCE_ENUM_PROCESS  10
#define  KILL_PROCESS        20
#define  KILL_PROCESS_FORCED 30
#define  KILL_PROCESS_AND_DELETE_FILE 40

typedef struct _PROCESS_INFO_
{
	ULONG ulProcessID;
	ULONG ulProcessParentID;
	ULONG_PTR ulEProcess;
	WCHAR wzCompanyName[MAX_PATH];
	WCHAR wzProcessFileName[MAX_PATH];
	WCHAR wzProcessName[MAX_PATH];
	BOOL  bHide;
}PROCESS_INFO,*PPROCESS_INFO;

typedef struct _PROCESS_INFORMATION_
{
	ULONG_PTR    NumberOfProcess;
	PROCESS_INFO Process[1];
}RTL_PROCESS_INFORMATION,*PRTL_PROCESS_INFORMATION;

int CALLBACK SortFuncByIncrease(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK SortFuncByDecrease(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

class CProcessManager : public CDialog
{
	DECLARE_DYNAMIC(CProcessManager)
public:
	VOID    InitControl();
	VOID    AddItemToControlList(PROCESS_INFO ProcessInfor,ULONG ulIndex);
	BOOL    SendIoControlCode(ULONG_PTR ulControlCode);
	BOOL    EnableDebugPri();
	VOID    EnumProcess();
	VOID    FixPath(WCHAR* wzImagePathName);
	VOID    FindTheHideProcess();
	void    CreatStatusBar(void);
	CString GetFileCompanyName(CString strPath);
	HICON   GetProcessIcon(WCHAR* wzProcessPath);
	VOID    LocationExplorer(CString strPath);
	void    CopyData(CString strhSelectData);
	VOID    KillProcess();
	VOID    KillProcessByForce();
	VOID    KillProcessAndDeleteFile();

public:
	CProcessManager(CWnd* pParent = NULL);  
	virtual ~CProcessManager();
	enum { IDD = IDD_DLG_PROCESS_MANAGER };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	CImageList   m_ControlImageList;
	CBitmap      m_BmpLocation;
	vector<PROCESS_INFO> m_Vector;
	ULONG_PTR    m_ProcessID;
	CString      m_ProcessName;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProcessModule();
    afx_msg void OnCustomdrawMyList ( NMHDR* pNMHDR, LRESULT* pResult );//ªÊ÷∆List¡¥±Ì
	afx_msg void OnProcessRefresh();
	afx_msg void OnProcessLocationFile();
	afx_msg void OnProcessCopyProcessname();
	afx_msg void OnProcessCopyProcessImageName();
	afx_msg void OnProcessCopyEprocess();
	afx_msg void OnProcessHandle();
	afx_msg void OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProcessCloseProcess();
	afx_msg void OnProcessCloseForced();
	afx_msg void OnProcessCloseDelete();
	afx_msg void OnProcessWind();
	afx_msg void OnProcessThread();
	afx_msg void OnProcessMemory();
};
