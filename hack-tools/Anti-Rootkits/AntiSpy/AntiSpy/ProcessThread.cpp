

#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessThread.h"
#include "afxdialogex.h"



extern HANDLE g_hDevice;

IMPLEMENT_DYNAMIC(CProcessThread, CDialog)

CProcessThread::CProcessThread(CWnd* pParent )
	: CDialog(CProcessThread::IDD, pParent)
{
	m_ProcessManagerDlg = (CProcessManager*)pParent;

	m_bThreadSuspend = -1;
}

CProcessThread::~CProcessThread()
{
	m_Vector.clear();
}

void CProcessThread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREAD, m_List);
}


BEGIN_MESSAGE_MAP(CProcessThread, CDialog)
	
	ON_NOTIFY(NM_RCLICK, IDC_LIST_THREAD, &CProcessThread::OnNMRClickListThread)
	ON_COMMAND(ID_PROCESS_THREAD_REFRESH, &CProcessThread::OnThreadRefresh)
	ON_COMMAND(ID_PROCESS_THREAD_TERMINATE, &CProcessThread::OnThreadTerminate)
	ON_COMMAND(ID_PROCESS_THREAD_FORCE_TERMINATE, &CProcessThread::OnThreadForceTerminate)
	ON_COMMAND(ID_PROCESS_THREAD_SUSPEND, &CProcessThread::OnThreadSuspend)
	ON_COMMAND(ID_PROCESS_THREAD_RESUME, &CProcessThread::OnThreadResume)
END_MESSAGE_MAP()




BOOL CProcessThread::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	ULONG ulControlCode = ENUM_THREAD;
	SendIoControlCode(ulControlCode);

	return TRUE;  
}

VOID CProcessThread::InitControl()
{
	

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0, L"线程ID", LVCFMT_LEFT, 50);
	m_List.InsertColumn(1, L"ETHREAD", LVCFMT_CENTER, 80);
	m_List.InsertColumn(2, L"Teb", LVCFMT_CENTER, 80);
	m_List.InsertColumn(3, L"优先级", LVCFMT_CENTER, 70);
	m_List.InsertColumn(4, L"线程入口", LVCFMT_CENTER, 80);
	m_List.InsertColumn(5, L"切换次数", LVCFMT_CENTER, 80);
	m_List.InsertColumn(6, L"线程状态", LVCFMT_CENTER, 80);
	m_List.InsertColumn(7, L"模块", LVCFMT_LEFT, 220);

	SetWindowLong(m_List.m_hWnd ,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);                   			  
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
}



BOOL CProcessThread::SendIoControlCode(ULONG_PTR ulControlCode)
{
	if (ulControlCode==ENUM_THREAD||ulControlCode==REFRESH)
	{
		EnumThreadInfor();

		if (m_Vector.empty())
		{
			return FALSE;
		}
		for (vector <PROCESS_THREAD_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )
		{
			AddItemToControlList(*Iter);
		}

		m_EditThreadCount = m_Vector.size();

		CString str;
		str.Format(L"进程线程: %d ",m_EditThreadCount);
		str +=L"(";
		str +=m_ProcessManagerDlg->m_ProcessName;
		str +=L")";

		SetWindowText(str);
	}

	else if (ulControlCode==KILL_THREAD)
	{

		KillThread();
		Sleep(1);
		SendIoControlCode(REFRESH);
		
	}

	else if(ulControlCode == FORCE_KILL_THREAD)
	{
		ForceKillThread();
		Sleep(1);
		SendIoControlCode(REFRESH);


	}

	else if(ulControlCode == SUSPEND_THREAD)
	{
		MySuspendThread();
		

	}

	else if(ulControlCode == RESUME_THREAD)
	{
		MyResumeThread();
		
	}

	else if(ulControlCode == JUDGE_THREADSUSPEND)
	{
		JudgeThreadIsSuspend();
	}
	


	return TRUE;
}


BOOL CProcessThread::EnumThreadInfor()
{
	BOOL bOk = FALSE;

	m_Vector.clear();

	m_List.DeleteAllItems();
	UpdateData(TRUE);

	ULONG_PTR ulCount = 0x1000;
	PPROCESS_THREAD_INFORMATION ProcessThreadInformation = NULL;
	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;
	do 
	{
		ULONG_PTR ulSize = 0;

		if (ProcessThreadInformation)
		{
			free(ProcessThreadInformation);
			ProcessThreadInformation = NULL;
		}

		ulSize = sizeof(PROCESS_THREAD_INFORMATION) + ulCount * sizeof(PROCESS_THREAD_INFORMATION);

		ProcessThreadInformation = (PPROCESS_THREAD_INFORMATION)malloc(ulSize);
		if (!ProcessThreadInformation)
		{
			break;
		}

		memset(ProcessThreadInformation,0,ulSize);


		bRet = DeviceIoControl(g_hDevice,CTL_ENUMPROCESSTHREAD,
			&m_ProcessManagerDlg->m_ProcessID,
			sizeof(ULONG),
			ProcessThreadInformation,
			ulSize,
			&ulReturnSize,
			NULL);


		ulCount = ProcessThreadInformation->NumberOfThreads + 1000;

	} while (bRet == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	if (bRet && ProcessThreadInformation)
	{
		for (ULONG i = 0; i < ProcessThreadInformation->NumberOfThreads; i++)
		{
			m_Vector.push_back(ProcessThreadInformation->Threads[i]);
		}

		bOk = TRUE;
	}

	if (ProcessThreadInformation)
	{
		free(ProcessThreadInformation);
		ProcessThreadInformation = NULL;
	}

	return bOk;
}

BOOL CProcessThread::KillThread()
{
	DWORD ulReturnSize = 0;

	return  DeviceIoControl(g_hDevice,CTL_KILLPROCESSTHREAD,
		&m_ulEThreadOfKill,
		sizeof(ULONG_PTR),
		NULL,
		0,
		&ulReturnSize,
		NULL);
}
BOOL CProcessThread::ForceKillThread()
{
	DWORD ulReturnSize = 0;

	return  DeviceIoControl(g_hDevice,CTL_FORCEKILLPROCESSTHREAD,
		&m_ulEThreadOfForceKill,
		sizeof(ULONG_PTR),
		NULL,
		0,
		&ulReturnSize,
		NULL);
}
BOOL CProcessThread::MySuspendThread()
{
	DWORD ulReturnSize = 0;

	return  DeviceIoControl(g_hDevice,CTL_SUSPENDPROCESSTHREAD,
		&m_ulEThreadOfSuspend,
		sizeof(ULONG_PTR),
		NULL,
		0,
		&ulReturnSize,
		NULL);

	
}
BOOL CProcessThread::MyResumeThread()
{
	DWORD ulReturnSize = 0;

	return  DeviceIoControl(g_hDevice,CTL_RESUMEPROCESSTHREAD,
		&m_ulEThreadOfResume,
		sizeof(ULONG_PTR),
		NULL,
		0,
		&ulReturnSize,
		NULL);
}
BOOL CProcessThread::JudgeThreadIsSuspend()
{
	DWORD ulReturnSize = 0;

	return  DeviceIoControl(g_hDevice,CTL_JUDGETHREADSUSPEND,
		&m_ulEThreadofJudge,
		sizeof(ULONG_PTR),
		&m_bThreadSuspend,
		sizeof(ULONG_PTR),
		&ulReturnSize,
		NULL);
}


VOID CProcessThread::AddItemToControlList(PROCESS_THREAD_ENTRY_INFOR ProcessThreadEntryInfor)
{
	
	CString strTid, strObject, strReferenceCount,strWin32StartAddress,strTebAddress,strState,strPriority,strModuleName;

	m_ProcessManagerDlg->FixPath(ProcessThreadEntryInfor.Module);
	

	strTid.Format(L"%d", ProcessThreadEntryInfor.Tid);
	strObject.Format(L"0x%08p", ProcessThreadEntryInfor.Thread);

	strWin32StartAddress.Format(L"0x%p",ProcessThreadEntryInfor.Win32StartAddress);
	strTebAddress.Format(L"0x%p",ProcessThreadEntryInfor.Teb);
	strReferenceCount.Format(L"%d", ProcessThreadEntryInfor.ContextSwitches);

	strState.Format(L"%d",ProcessThreadEntryInfor.State);
	strPriority.Format(L"%d",ProcessThreadEntryInfor.Priority);

	

	int n = m_List.InsertItem(m_List.GetItemCount(), strTid);
	m_List.SetItemText(n, 1, strObject);
	m_List.SetItemText(n, 2, strTebAddress);
	m_List.SetItemText(n,3,strPriority);
	m_List.SetItemText(n, 4, strWin32StartAddress);

	m_List.SetItemText(n, 5, strReferenceCount);
	m_List.SetItemText(n,6,strState);
	m_List.SetItemText(n,7,ProcessThreadEntryInfor.Module);
	
}




void CProcessThread::OnNMRClickListThread(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString strEThread;

	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}
	strEThread = m_List.GetItemText(iItem,1);  

	swscanf(strEThread.GetBuffer()+2,L"%p",&m_ulEThreadofJudge); 



	ULONG_PTR  ulControlCode = JUDGE_THREADSUSPEND;
	SendIoControlCode(ulControlCode);


	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CMenu	Menu;
	Menu.LoadMenu(IDR_MENU_PROCESS_THREAD);               //加载菜单资源
	CMenu*	pM = Menu.GetSubMenu(0);                 //获得菜单的子项

	CPoint	p;
	GetCursorPos(&p);
	int count = pM->GetMenuItemCount();
	if (m_List.GetSelectedCount() == 0)         //如果没有选中
	{ 
		for (int i = 0;i<count;i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          //菜单全部变灰
		}

	}
	else
	{


		if(m_bThreadSuspend == 0)   //线程正常运行
		{
			pM->EnableMenuItem(6, MF_BYPOSITION | MF_DISABLED | MF_GRAYED); 
		}
		else						//线程已被挂起
		{
			pM->EnableMenuItem(5, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}

	}

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}


void CProcessThread::OnThreadRefresh()
{
	ULONG_PTR  ulControlCode = REFRESH;
	SendIoControlCode(ulControlCode);
}


void CProcessThread::OnThreadTerminate()
{
	CString strEThread;

	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}
	strEThread = m_List.GetItemText(iItem,1);  

	swscanf(strEThread.GetBuffer()+2,L"%p",&m_ulEThreadOfKill); 



	ULONG_PTR  ulControlCode = KILL_THREAD;
	SendIoControlCode(ulControlCode);

}


void CProcessThread::OnThreadForceTerminate()
{
	CString strEThread;

	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}
	strEThread = m_List.GetItemText(iItem,1);  

	swscanf(strEThread.GetBuffer()+2,L"%p",&m_ulEThreadOfForceKill); 



	ULONG_PTR  ulControlCode = FORCE_KILL_THREAD;
	SendIoControlCode(ulControlCode);
}


void CProcessThread::OnThreadSuspend()
{
	CString strEThread;

	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}
	strEThread = m_List.GetItemText(iItem,1);  

	swscanf(strEThread.GetBuffer()+2,L"%p",&m_ulEThreadOfSuspend);


	ULONG_PTR  ulControlCode = SUSPEND_THREAD;
	SendIoControlCode(ulControlCode);
}


void CProcessThread::OnThreadResume()
{
	CString strEThread;

	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}
	strEThread = m_List.GetItemText(iItem,1);  

	swscanf(strEThread.GetBuffer()+2,L"%p",&m_ulEThreadOfResume); 



	ULONG_PTR  ulControlCode = RESUME_THREAD;
	SendIoControlCode(ulControlCode);
}

