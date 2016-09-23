
#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessMemory.h"
#include "afxdialogex.h"

#define MAX  0x1000

IMPLEMENT_DYNAMIC(CProcessMemory, CDialog)

CProcessMemory::CProcessMemory(CWnd* pParent)
	: CDialog(CProcessMemory::IDD, pParent)
{
	m_ProcessManagerDlg = (CProcessManager*)pParent;
}


void CProcessMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMORY, m_List);
}


BEGIN_MESSAGE_MAP(CProcessMemory, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MEMORY, &CProcessMemory::OnNMRClickListMemory)
	ON_COMMAND(ID_PROCESS_PROTECT_NOACCESS, &CProcessMemory::OnProcessProtectNoaccess)
	ON_COMMAND(ID_PROCESS_PROTECT_READ, &CProcessMemory::OnProcessProtectRead)
	ON_COMMAND(ID_PROCESS_PROTECT_READWRITE, &CProcessMemory::OnProcessProtectReadwrite)
	ON_COMMAND(ID_PROCESS_PROTECT_READWRITE_GUARD, &CProcessMemory::OnProcessProtectReadwriteGuard)
	ON_COMMAND(ID_PROCESS_PROTECT_READ_EXECUTE, &CProcessMemory::OnProcessProtectReadExecute)
	ON_COMMAND(ID_PROCESS_PROTECT_WRITE_COPY, &CProcessMemory::OnProcessProtectWriteCopy)
	ON_COMMAND(ID_PROCESS_MEMORY_READ, &CProcessMemory::OnProcessMemoryRead)
	ON_COMMAND(ID_PROCESS_MEMORY_WRITE, &CProcessMemory::OnProcessMemoryWrite)
END_MESSAGE_MAP()


BOOL CProcessMemory::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0, L"内存基地址", LVCFMT_LEFT,100);
	m_List.InsertColumn(1, L"范围", LVCFMT_LEFT, 100);
	m_List.InsertColumn(2, L"保护", LVCFMT_CENTER, 110);
	m_List.InsertColumn(3, L"状态", LVCFMT_CENTER, 80);
	m_List.InsertColumn(4, L"类型", LVCFMT_CENTER, 100);

	ULONG_PTR  ulControlCode = ENUM_MEMORY;
	SendIoControlCode(ulControlCode);
	return TRUE; 
}



BOOL CProcessMemory::SendIoControlCode(ULONG_PTR ulControlCode)
{

	if (ulControlCode==ENUM_MEMORY)
	{
		EnumMemoryInfor();


		if (m_Vector.empty())
		{
			return FALSE;
		}
		for (vector <PROCESS_MEMORY_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )
		{
			AddItemToControlList(*Iter);
		}

		CString str;
		str.Format(L"进程内存: %d ",m_Vector.size());
		str +=L"(";
		str +=m_ProcessManagerDlg->m_ProcessName;
		str +=L")";

		SetWindowText(str);

	}

	else if (ulControlCode==MODIFY_MEMORY)
	{
		ModifyMemoryProtect();

		EnumMemoryInfor();

		if (m_Vector.empty())
		{
			return FALSE;
		}


		for (vector <PROCESS_MEMORY_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )
		{
			AddItemToControlList(*Iter);
		}

		CString str;
		str.Format(L"进程内存: %d ",m_Vector.size());
		str +=L"(";
		str +=m_ProcessManagerDlg->m_ProcessName;
		str +=L")";

		SetWindowText(str);

	}

	return TRUE;
}



VOID CProcessMemory::AddItemToControlList(PROCESS_MEMORY_ENTRY_INFOR ProcessMemoryEntryInfor)
{
	CString strBase, strSize, strProtect,strState,strType;

	strBase.Format(L"0x%04p", ProcessMemoryEntryInfor.ulBase);
	strSize.Format(L"0x%08p", ProcessMemoryEntryInfor.ulSize);

	strProtect.Format(L"%d", ProcessMemoryEntryInfor.ulProtect);
	strState.Format(L"%p", ProcessMemoryEntryInfor.ulState);
	strType.Format(L"%p", ProcessMemoryEntryInfor.ulType);


	switch(ProcessMemoryEntryInfor.ulType)
	{
	case 0x1000000:
		{
			strType = L"Image";
			break;
		}
	case 0x40000:
		{
			strType = L"Map";
			break;
		}

	case 0x20000:
		{

			strType = L"Private";

			break;
		}

	default:
		{
			strType = L"";
			break;
		}
	}


	switch(ProcessMemoryEntryInfor.ulState)
	{
	case 0x10000:
		{
			strState = L"Free";
			break;
		}

	case 0x2000:
		{
			strState = L"Reserve";
			break;
		}

	case 0x1000:
		{
			strState = L"Commit";
			break;
		}

	default:
		{
			strState = L"";
			break;
		}
	}

	switch(ProcessMemoryEntryInfor.ulProtect)
	{
	case 1:
		{
			strProtect = L"No Access";
			break;
		}

	case 2:
		{
			strProtect = L"Read"; 
			break;
		}
	case 4:
		{
			strProtect = L"ReadWrite";
			break;
		}
	case 260:
		{
			strProtect = L"ReadWrite GUARD";
			break;
		}
	case 32:
		{
			strProtect = L"ReadExecute";
			break;
		}
	case 8:
		{
			strProtect = L"WriteCopy";
			break;
		}

	default:
		{
			strProtect = L"";
			break;
		}
	}


	int n = m_List.InsertItem(m_List.GetItemCount(), strBase);
	m_List.SetItemText(n, 1, strSize);
	m_List.SetItemText(n, 2, strProtect);
	m_List.SetItemText(n, 3, strState);
	m_List.SetItemText(n, 4, strType);
}

BOOL CProcessMemory::EnumMemoryInfor()
{
	BOOL bOk = FALSE;

	m_Vector.clear();

	m_List.DeleteAllItems();

	ULONG_PTR ulCount = 0x1000;
	PPROCESS_MEMORY_INFORMATION ProcessMemroyInformation = NULL;
	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;

	do 
	{
		ULONG_PTR ulSize = 0;

		if (ProcessMemroyInformation)
		{
			free(ProcessMemroyInformation);
			ProcessMemroyInformation = NULL;
		}

		ulSize = sizeof(PROCESS_MEMORY_INFORMATION) + ulCount * sizeof(PROCESS_MEMORY_ENTRY_INFOR);

		ProcessMemroyInformation = (PPROCESS_MEMORY_INFORMATION)malloc(ulSize);
		if (!ProcessMemroyInformation)
		{
			break;
		}

		memset(ProcessMemroyInformation,0,ulSize);


		bRet = DeviceIoControl(g_hDevice,CTL_ENUMPROCESSMEMORY,
			&m_ProcessManagerDlg->m_ProcessID,
			sizeof(ULONG_PTR),
			ProcessMemroyInformation,
			ulSize,
			&ulReturnSize,
			NULL);


		ulCount = ProcessMemroyInformation->NumberOfMemorys + 1000;


		int a = GetLastError();

		int b = 0;


	} while (bRet == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	if (bRet && ProcessMemroyInformation)
	{
		for (ULONG i = 0; i < ProcessMemroyInformation->NumberOfMemorys; i++)
		{
			m_Vector.push_back(ProcessMemroyInformation->Memorys[i]);
		}

		bOk = TRUE;
	}

	if (ProcessMemroyInformation)
	{
		free(ProcessMemroyInformation);
		ProcessMemroyInformation = NULL;
	}

	return bOk;
}

BOOL CProcessMemory::ModifyMemoryProtect()
{
	struct _MODIFY_MEMORY 
	{
		ULONG       ulProcessID;
		ULONG_PTR   ulBase;
		ULONG_PTR   ulSize;
		ULONG       ulNewProtect;
	}Data;

	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;

	memset(&Data,0,sizeof(_MODIFY_MEMORY));

	Data.ulProcessID  = m_ProcessManagerDlg->m_ProcessID;
	Data.ulBase       = m_Base;
	Data.ulNewProtect = m_NewProtect;
	Data.ulSize       = m_Size;

	return  DeviceIoControl(g_hDevice,CTL_MODIFYROCESSMEMORYPROTECT,
		&Data,
		sizeof(_MODIFY_MEMORY),
		NULL,
		0,
		&ulReturnSize,
		NULL);
}

void CProcessMemory::OnNMRClickListMemory(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_PROCESS_MEMORY);               
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


void CProcessMemory::OnProcessProtectNoaccess()
{

	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strBase = m_List.GetItemText(iItem,0);  

	swscanf(strBase.GetBuffer()+2,L"%p",&m_Base);


	strSize = m_List.GetItemText(iItem,1);  

	swscanf(strSize.GetBuffer()+2,L"%p",&m_Size);

	m_NewProtect = 2;

	ULONG_PTR  ulControlCode = MODIFY_MEMORY;
	SendIoControlCode(ulControlCode);	
}


void CProcessMemory::OnProcessProtectRead()
{
	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strBase = m_List.GetItemText(iItem,0);  

	swscanf(strBase.GetBuffer()+2,L"%p",&m_Base);


	strSize = m_List.GetItemText(iItem,1);  

	swscanf(strSize.GetBuffer()+2,L"%p",&m_Size);

	m_NewProtect = 1;

	ULONG_PTR  ulControlCode = MODIFY_MEMORY;
	SendIoControlCode(ulControlCode);}


void CProcessMemory::OnProcessProtectReadwrite()
{
	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strBase = m_List.GetItemText(iItem,0);  

	swscanf(strBase.GetBuffer()+2,L"%p",&m_Base);


	strSize = m_List.GetItemText(iItem,1);  

	swscanf(strSize.GetBuffer()+2,L"%p",&m_Size);

	m_NewProtect = 4;

	ULONG_PTR  ulControlCode = MODIFY_MEMORY;
	SendIoControlCode(ulControlCode);
	
}


void CProcessMemory::OnProcessProtectReadwriteGuard()
{
	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strBase = m_List.GetItemText(iItem,0);  

	swscanf(strBase.GetBuffer()+2,L"%p",&m_Base);


	strSize = m_List.GetItemText(iItem,1);  

	swscanf(strSize.GetBuffer()+2,L"%p",&m_Size);

	m_NewProtect = 260;

	ULONG_PTR  ulControlCode = MODIFY_MEMORY;
	SendIoControlCode(ulControlCode);
	
}


void CProcessMemory::OnProcessProtectReadExecute()
{
	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strBase = m_List.GetItemText(iItem,0);  

	swscanf(strBase.GetBuffer()+2,L"%p",&m_Base);


	strSize = m_List.GetItemText(iItem,1);  

	swscanf(strSize.GetBuffer()+2,L"%p",&m_Size);

	m_NewProtect = 32;

	ULONG_PTR  ulControlCode = MODIFY_MEMORY;
	SendIoControlCode(ulControlCode);
	
}


void CProcessMemory::OnProcessProtectWriteCopy()
{
	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos); 

	if (iItem==-1)
	{
		return;
	}

	strBase = m_List.GetItemText(iItem,0);  

	swscanf(strBase.GetBuffer()+2,L"%p",&m_Base);


	strSize = m_List.GetItemText(iItem,1);  

	swscanf(strSize.GetBuffer()+2,L"%p",&m_Size);

	m_NewProtect = 8;

	ULONG_PTR  ulControlCode = MODIFY_MEMORY;
	SendIoControlCode(ulControlCode);
}




void CProcessMemory::OnProcessMemoryRead()
{
	CString  strBase;
	CString  strSize;
	POSITION Pos = m_List.GetFirstSelectedItemPosition(); 
	int iItem = m_List.GetNextSelectedItem(Pos);

	strBase = m_List.GetItemText(iItem,0); 

	CReadProcessMemory Dlg;
	Dlg.m_ProcessID = m_ProcessManagerDlg->m_ProcessID;
	Dlg.m_Address = strBase;
	Dlg.m_Length  =  MAX;
	Dlg.DoModal();

}


void CProcessMemory::OnProcessMemoryWrite()
{
	
}
