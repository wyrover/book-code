
#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessModule.h"
#include "IOControlCode.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CProcessModule, CDialog)

CProcessModule::CProcessModule(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessModule::IDD, pParent)
{
	m_ProcessManagerDlg = (CProcessManager*)pParent;
}

CProcessModule::~CProcessModule()
{
	m_Vector.clear();
}

void CProcessModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}


BEGIN_MESSAGE_MAP(CProcessModule, CDialog)
END_MESSAGE_MAP()



BOOL CProcessModule::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	m_List.InsertColumn(0,L"基地址",LVCFMT_LEFT,120);
	m_List.InsertColumn(1,L"路径",LVCFMT_LEFT,200);	
	m_List.InsertColumn(2,L"大小",LVCFMT_LEFT,120); 	
	m_List.InsertColumn(3,L"文件厂商",LVCFMT_CENTER,180); 	

	SetWindowLong(m_List.m_hWnd ,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);                   			  
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	GetProcessModuleInfo();

	return TRUE;  
}

VOID  CProcessModule::GetProcessModuleInfo()
{
	ULONG_PTR  ulControlCode = ENUM_MODULE;
	SendIoControlCode(ulControlCode);
}

BOOL CProcessModule::SendIoControlCode(ULONG_PTR ulControlCode)
{
	if (ulControlCode==ENUM_MODULE)
	{
		EnumModuleInfor();

		if (m_Vector.empty())
		{
			return FALSE;
		}

		for (vector <PROCESS_MODULE_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )
		{
			m_ProcessManagerDlg->FixPath(Iter->ModulePath);
			
			AddItemToControlList(*Iter);
		}

		CString str;
		str.Format(L"进程模块: %d ",m_Vector.size());
		str +=L"(";
		str +=m_ProcessManagerDlg->m_ProcessName;
		str +=L")";
		SetWindowText(str);
	}

	else if (ulControlCode==UNLOAD_MODULE)
	{

	}

	return TRUE;
}

VOID CProcessModule::AddItemToControlList(PROCESS_MODULE_ENTRY_INFOR ProcessModuleEntryInfor)
{
	CString strBase, strSize, strModulePath ; 
	strBase.Format(L"0x%08p", ProcessModuleEntryInfor.ModuleBase);
	strSize.Format(L"0x%p",ProcessModuleEntryInfor.ModuleSize);

	
	int n = m_List.InsertItem(m_List.GetItemCount(), strBase);
	m_List.SetItemText(n, 1, ProcessModuleEntryInfor.ModulePath);
	m_List.SetItemText(n, 2, strSize);
	m_List.SetItemText(n,3,m_ProcessManagerDlg->GetFileCompanyName(ProcessModuleEntryInfor.ModulePath));
}


BOOL CProcessModule::EnumModuleInfor()
{
	BOOL bOk = FALSE;

	m_Vector.clear();

	m_List.DeleteAllItems();

	ULONG_PTR ulCount = 0x1000;

	PPROCESS_MODULE_INFORMATION ProcessModuleInformation = NULL;

	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;

	do 
	{
		ULONG_PTR ulSize = 0;

		if (ProcessModuleInformation)
		{
			free(ProcessModuleInformation);
			ProcessModuleInformation = NULL;
		}

		ulSize = sizeof(PROCESS_MODULE_INFORMATION) + ulCount * sizeof(PROCESS_MODULE_ENTRY_INFOR);

		ProcessModuleInformation = (PPROCESS_MODULE_INFORMATION)malloc(ulSize);

		if (!ProcessModuleInformation)
		{
			break;
		}

		memset(ProcessModuleInformation,0,ulSize);


		bRet = DeviceIoControl(g_hDevice,CTL_ENUMPROCESSMODULE,
			&m_ProcessManagerDlg->m_ProcessID,
			sizeof(ULONG_PTR),
			ProcessModuleInformation,
			ulSize,
			&ulReturnSize,
			NULL);

		ulCount = ProcessModuleInformation->NumberOfModule + 1000;

	} while (bRet == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	if (bRet && ProcessModuleInformation)
	{
		for (ULONG i = 0; i < ProcessModuleInformation->NumberOfModule; i++)
		{
			m_Vector.push_back(ProcessModuleInformation->Module[i]);
		}

		bOk = TRUE;
	}

	if (ProcessModuleInformation)
	{
		free(ProcessModuleInformation);
		ProcessModuleInformation = NULL;
	}
	return bOk;
}


