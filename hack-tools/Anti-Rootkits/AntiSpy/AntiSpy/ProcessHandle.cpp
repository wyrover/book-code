
#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessHandle.h"
#include "afxdialogex.h"


extern HANDLE g_hDevice;

IMPLEMENT_DYNAMIC(CProcessHandle, CDialog)

CProcessHandle::CProcessHandle(CWnd* pParent )
	: CDialog(CProcessHandle::IDD, pParent)
{
	m_ProcessManagerDlg = (CProcessManager*)pParent;
}

CProcessHandle::~CProcessHandle()
{
	m_Vector.clear();
}

void CProcessHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CProcessHandle, CDialog)
	ON_COMMAND(ID_PROCESS_HANDLE_REFRESH, &CProcessHandle::OnProcessHandleRefresh)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CProcessHandle::OnNMRClickList)
	ON_COMMAND(ID_PROCESS_HANDLE_CLOSE, &CProcessHandle::OnProcessHandleClose)
END_MESSAGE_MAP()




BOOL CProcessHandle::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitControl();

	ULONG ulControlCode = ENUM_HANDLE;
	SendIoControlCode(ulControlCode);


	return TRUE;  
}


VOID CProcessHandle::InitControl()
{
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,L"句柄", LVCFMT_LEFT,80);
	m_List.InsertColumn(1,L"句柄对象", LVCFMT_LEFT, 80);
	m_List.InsertColumn(2,L"句柄类型", LVCFMT_LEFT, 80);
	m_List.InsertColumn(3,L"引用计数", LVCFMT_CENTER, 70);
	m_List.InsertColumn(4,L"句柄名称", LVCFMT_LEFT, 378);
	

	SetWindowLong(m_List.m_hWnd ,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);                   			  
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
}


VOID CProcessHandle::AddItemToControlList(PROCESS_HANDLE_ENTRY_INFOR ProcessHandleEntryInfor)
{
	CString strHandle, strObject, strReferenceCount;

	strHandle.Format(L"0x%04p", ProcessHandleEntryInfor.Handle);
	strObject.Format(L"0x%08p", ProcessHandleEntryInfor.Object);
	strReferenceCount.Format(L"%d", ProcessHandleEntryInfor.ReferenceCount);

	int n = m_List.InsertItem(m_List.GetItemCount(), strHandle);
	m_List.SetItemText(n, 1, strObject);
	m_List.SetItemText(n, 2, ProcessHandleEntryInfor.wzHandleType);
	m_List.SetItemText(n, 3, strReferenceCount);
	m_List.SetItemText(n, 4, ProcessHandleEntryInfor.wzHandleName);
}


BOOL CProcessHandle::SendIoControlCode(ULONG_PTR ulControlCode)
{
	if (ulControlCode==ENUM_HANDLE)
	{
		EnumInfor();

		if (m_Vector.empty())
		{
			return FALSE;
		}
		for (vector <PROCESS_HANDLE_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )
		{
			AddItemToControlList(*Iter);
		}

		m_EditHandleCount = m_Vector.size();

		CString str;
		str.Format(L"进程句柄: %d ",m_EditHandleCount);
		str +=L"(";
		str +=m_ProcessManagerDlg->m_ProcessName;
		str +=L")";

		SetWindowText(str);
	}

	else if (ulControlCode==KILL_HANDLE)
	{

		if(KillHandle()==TRUE)
		{
			EnumInfor();

			if (m_Vector.empty())
			{
				return FALSE;
			}


			for (vector <PROCESS_HANDLE_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
				Iter != m_Vector.end( ); 
				Iter++ )
			{
				AddItemToControlList(*Iter);
			}

			m_EditHandleCount = m_Vector.size();

			CString str;
			str.Format(L"进程句柄: %d ",m_EditHandleCount);
			str +=L"(";
			str +=m_ProcessManagerDlg->m_ProcessName;
			str +=L")";

			SetWindowText(str);
		}
	}

	return TRUE;
}



BOOL CProcessHandle::EnumInfor()
{
	BOOL bOk = FALSE;

	m_Vector.clear();

	m_List.DeleteAllItems();
	UpdateData(TRUE);
	
	ULONG_PTR ulCount = 0x1000;
	PPROCESS_HANDLE_INFORMATION ProcessHandleInformation = NULL;
	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;
	do 
	{
		ULONG_PTR ulSize = 0;

		if (ProcessHandleInformation)
		{
			free(ProcessHandleInformation);
			ProcessHandleInformation = NULL;
		}

		ulSize = sizeof(PROCESS_HANDLE_INFORMATION) + ulCount * sizeof(PROCESS_HANDLE_ENTRY_INFOR);

		ProcessHandleInformation = (PPROCESS_HANDLE_INFORMATION)malloc(ulSize);
		if (!ProcessHandleInformation)
		{
			break;
		}

		memset(ProcessHandleInformation,0,ulSize);


		bRet = DeviceIoControl(g_hDevice,CTL_ENUMPROCESSHANDLE,
			&m_ProcessManagerDlg->m_ProcessID,
			sizeof(ULONG),
			ProcessHandleInformation,
			ulSize,
			&ulReturnSize,
			NULL);


		ulCount = ProcessHandleInformation->NumberOfHandles + 1000;

	} while (bRet == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	if (bRet && ProcessHandleInformation)
	{
		for (ULONG i = 0; i < ProcessHandleInformation->NumberOfHandles; i++)
		{
			m_Vector.push_back(ProcessHandleInformation->Handles[i]);
		}

		bOk = TRUE;
	}

	if (ProcessHandleInformation)
	{
		free(ProcessHandleInformation);
		ProcessHandleInformation = NULL;
	}

	return bOk;
}


BOOL CProcessHandle::KillHandle()
{
	struct _DATA_ 
	{
		ULONG       ulProcessID;
		ULONG_PTR   ulHandle; 
	}Data;

	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;

	memset(&Data,0,sizeof(_DATA_));

	Data.ulProcessID = m_ProcessManagerDlg->m_ProcessID;
	Data.ulHandle    = m_Handle;

	return  DeviceIoControl(g_hDevice,CTL_KILLPROCESSHANDLE,
		&Data,
		sizeof(_DATA_),
		NULL,
		0,
		&ulReturnSize,
		NULL);
}


void CProcessHandle::OnProcessHandleRefresh()
{
	ULONG_PTR ulControlCode = ENUM_HANDLE;

	SendIoControlCode(ulControlCode);
}


void CProcessHandle::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu	popup;
	popup.LoadMenu(IDR_PROCESS_HANDLE_MENU);               
	CMenu*	pM = popup.GetSubMenu(0);                 
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();

	if (m_List.GetSelectedCount() == 0)         
	{ 
		for (int i = 0;i<count;i++)
		{
			if(i!=0)
			{
				pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          
			}
		}
	}

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}


void CProcessHandle::OnProcessHandleClose()
{
	CString strHandle;

	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem    = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strHandle = m_List.GetItemText(iItem,0);  

	swscanf(strHandle.GetBuffer()+2,L"%p",&m_Handle);

	ULONG_PTR  ulControlCode = KILL_HANDLE;
	SendIoControlCode(ulControlCode);
}
