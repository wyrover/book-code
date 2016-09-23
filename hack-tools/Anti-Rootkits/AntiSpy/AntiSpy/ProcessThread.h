#pragma once
#include "afxcmn.h"
#include "ProcessManager.h"


#define ENUM_THREAD          100
#define REFRESH              101
#define KILL_THREAD          102
#define FORCE_KILL_THREAD    103
#define SUSPEND_THREAD       104
#define RESUME_THREAD        105
#define JUDGE_THREADSUSPEND  106


typedef struct _PROCESS_THREAD_ENTRY_INFOR_
{
	ULONG_PTR Thread;
	ULONG_PTR Tid;
	ULONG_PTR Teb;
	UCHAR Priority;
	ULONG_PTR Win32StartAddress;
	ULONG ContextSwitches;
	UCHAR State;
	WCHAR Module[MAX_PATH];
}PROCESS_THREAD_ENTRY_INFOR, *PPROCESS_THREAD_ENTRY_INFOR;

typedef struct _PROCESS_THREAD_INFORMATION
{
	ULONG_PTR    NumberOfThreads;
	PROCESS_THREAD_ENTRY_INFOR Threads[1];
}PROCESS_THREAD_INFORMATION, *PPROCESS_THREAD_INFORMATION;


class CProcessThread : public CDialog
{
	DECLARE_DYNAMIC(CProcessThread)

public:
	CProcessThread(CWnd* pParent = NULL);   
	virtual ~CProcessThread();
	vector<PROCESS_THREAD_ENTRY_INFOR> m_Vector;
	CProcessManager* m_ProcessManagerDlg;
	ULONG     m_EditThreadCount;

	ULONG_PTR m_ulEThreadOfKill;
	ULONG_PTR m_ulEThreadOfForceKill;
	ULONG_PTR m_ulEThreadOfSuspend;
	ULONG_PTR m_ulEThreadOfResume;
	ULONG_PTR m_ulEThreadofJudge;
	int       m_nSuspendListItem;
	ULONG_PTR m_bThreadSuspend;

	enum { IDD = IDD_DLG_PROCESS_THREAD };

	VOID CProcessThread::InitControl();
	BOOL CProcessThread::SendIoControlCode(ULONG_PTR ulControlCode);
	BOOL CProcessThread::EnumThreadInfor();
	VOID CProcessThread::AddItemToControlList(PROCESS_THREAD_ENTRY_INFOR ProcessThreadEntryInfor);
	//WCHAR* CProcessThread::FixPath(WCHAR* wzSour);
	BOOL CProcessThread::KillThread();
	BOOL CProcessThread::ForceKillThread();
	BOOL CProcessThread::MySuspendThread();
	BOOL CProcessThread::MyResumeThread();
	BOOL CProcessThread::JudgeThreadIsSuspend();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickListThread(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnThreadRefresh();
	afx_msg void OnThreadTerminate();
	afx_msg void OnThreadForceTerminate();
	afx_msg void OnThreadSuspend();
	afx_msg void OnThreadResume();
};
