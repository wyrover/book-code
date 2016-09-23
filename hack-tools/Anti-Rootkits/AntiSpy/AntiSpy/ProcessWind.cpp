#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessWind.h"
#include "ProcessWndChangDataDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(CProcessWind, CDialog)

CProcessWind::CProcessWind(CWnd* pParent)
	: CDialog(CProcessWind::IDD, pParent)
{
	m_ProcessManagerDlg = (CProcessManager*)pParent;
}

CProcessWind::~CProcessWind()
{
	m_Vector.clear();
}

void CProcessWind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WND, m_List);
}


BEGIN_MESSAGE_MAP(CProcessWind, CDialog)
	ON_COMMAND(ID_PROCESS_WND_REFRESH, &CProcessWind::OnProcessWndRefresh)
	ON_COMMAND(ID_PROCESS_WND_CHAGD_DATA, &CProcessWind::OnProcessWndChagdData)
	ON_COMMAND(ID_PROCESS_WND_SHOW, &CProcessWind::OnProcessWndShow)
	ON_COMMAND(ID_PROCESS_WND_HIDE, &CProcessWind::OnProcessWndHide)
	ON_COMMAND(ID_PROCESS_WND_ENABLE, &CProcessWind::OnProcessWndEnable)
	ON_COMMAND(ID_PROCESS_WND_DISABLED, &CProcessWind::OnProcessWndDisabled)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WND, &CProcessWind::OnNMRClickListWnd)
END_MESSAGE_MAP()


BOOL CProcessWind::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	EnumWndInfor();

	for (vector <PROCESS_WND_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
		Iter != m_Vector.end( ); 
		Iter++ )
	{
		AddItemToControlList(*Iter);
	}

	CString str;
	str.Format(L"窗口个数: %d ",m_Vector.size());
	str +=L"(";
	str +=m_ProcessManagerDlg->m_ProcessName;
	str +=L")";

	SetWindowText(str);
	return TRUE; 
}


VOID CProcessWind::InitControl()
{
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0, L"窗口句柄括", LVCFMT_LEFT, 150);
	m_List.InsertColumn(1, L"窗口标题", LVCFMT_LEFT, 130);
	m_List.InsertColumn(2, L"窗口类名", LVCFMT_LEFT, 130);
	m_List.InsertColumn(3, L"窗口可见性", LVCFMT_LEFT, 100);
	m_List.InsertColumn(4, L"线程ID", LVCFMT_LEFT, 80);
	m_List.InsertColumn(5, L"进程ID", LVCFMT_LEFT, 80);

	SetWindowLong(m_List.m_hWnd ,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);                   			  
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
}

ULONG_PTR CProcessWind::EnumWndInfor()
{
	PROCESS_WND_ENTRY_INFOR ProcessWndInformation = {0};
	DWORD dwThreadID,dwProcessID;
	ULONG_PTR i,ulCout = 0;

	m_Vector.clear();

	for(i=0; i<100000000; i+=4)
	{
		dwThreadID = GetWindowThreadProcessId((HWND)i,&dwProcessID);

		if(dwThreadID!=0&&dwProcessID!=0)
		{
			if(dwProcessID==m_ProcessManagerDlg->m_ProcessID)
			{
				ProcessWndInformation.hWnd = (HWND)i;
				ProcessWndInformation.ulPid  = dwProcessID;
				ProcessWndInformation.ulTid = dwThreadID;

				m_Vector.push_back(ProcessWndInformation);

				ulCout++;
			}
		}
	}
	return ulCout;
}

VOID CProcessWind::AddItemToControlList(PROCESS_WND_ENTRY_INFOR ProcessWndEntryInfor)
{
	WCHAR wzClassName[MAX_PATH] = {0};
	WCHAR wzWindowsText[MAX_PATH] = {0};

	CString  strhWnd;
	CString  strTid;
	CString  strPid;
	CString  strVisable;

	::GetClassName(ProcessWndEntryInfor.hWnd,  wzClassName, MAX_PATH);
	::GetWindowText(ProcessWndEntryInfor.hWnd, wzWindowsText, MAX_PATH);


	if (::IsWindowVisible(ProcessWndEntryInfor.hWnd))
	{
		strVisable = L"可见";
	}
	else
	{
		strVisable = L"-";
	}

	strhWnd.Format(L"0x%p",ProcessWndEntryInfor.hWnd);
	strTid.Format(L"%d",ProcessWndEntryInfor.ulTid);
	strPid.Format(L"%d",ProcessWndEntryInfor.ulPid);
	int iRow = m_List.GetItemCount();
	int iItem = m_List.InsertItem(iRow,strhWnd);
	m_List.SetItemText(iItem, 1, wzWindowsText);
	m_List.SetItemText(iItem, 2, wzClassName);
	m_List.SetItemText(iItem, 3, strVisable);
	m_List.SetItemText(iItem, 4, strTid);
	m_List.SetItemText(iItem, 5, strPid);

}


void CProcessWind::OnProcessWndRefresh()
{
	
	EnumWndInfor();

	m_List.DeleteAllItems();

	for (vector <PROCESS_WND_ENTRY_INFOR>::iterator Iter = m_Vector.begin( ); 
		Iter != m_Vector.end( ); 
		Iter++ )
	{
		AddItemToControlList(*Iter);
	}

	CString str;
	str.Format(L"窗口个数: %d ",m_Vector.size());
	str +=L"(";
	str +=m_ProcessManagerDlg->m_ProcessName;
	str +=L")";

	SetWindowText(str);
}


void CProcessWind::OnProcessWndChagdData()
{
	CProcessWndChangDataDlg  Dlg;

	if(Dlg.DoModal()==IDOK)
	{
		UpdateData(FALSE);

		CString strFix = Dlg.m_Edit;
		HWND  hWnd = NULL;
		int iItem = m_List.GetSelectionMark();

		if (iItem<0)
		{
			return;
		}

		CString strWnd = m_List.GetItemText(iItem,0);

		swscanf(strWnd.GetBuffer()+2,L"%p",&hWnd);

		::SetWindowText(hWnd,strFix);

		OnProcessWndRefresh();
	}
}


void CProcessWind::OnProcessWndShow()
{
	HWND  hWnd = NULL;
	int iItem = m_List.GetSelectionMark();

	if (iItem < 0)
	{
		return;
	}

	CString strWnd = m_List.GetItemText(iItem,0);

	swscanf(strWnd.GetBuffer()+2,L"%p",&hWnd);
	::ShowWindow(hWnd,SW_SHOW);

	OnProcessWndRefresh();
}


void CProcessWind::OnProcessWndHide()
{
	HWND  hWnd = NULL;
	int iItem = m_List.GetSelectionMark();

	if (iItem < 0)
	{
		return;
	}

	CString strWnd = m_List.GetItemText(iItem,0);

	swscanf(strWnd.GetBuffer()+2,L"%p",&hWnd);
	::ShowWindow(hWnd,SW_HIDE);

	OnProcessWndRefresh();
}


void CProcessWind::OnProcessWndEnable()
{
	HWND  hWnd = NULL;
	int iItem = m_List.GetSelectionMark();

	if (iItem < 0)
	{
		return;
	}

	CString strWnd = m_List.GetItemText(iItem,0);

	swscanf(strWnd.GetBuffer()+2,L"%p",&hWnd);
	::EnableWindow(hWnd,TRUE);

	OnProcessWndRefresh();
}


void CProcessWind::OnProcessWndDisabled()
{
	HWND  hWnd = NULL;
	int iItem = m_List.GetSelectionMark();

	if (iItem < 0)
	{
		return;
	}

	CString strWnd = m_List.GetItemText(iItem,0);

	swscanf(strWnd.GetBuffer()+2,L"%p",&hWnd);
	::EnableWindow(hWnd,FALSE);

	OnProcessWndRefresh();
}


void CProcessWind::OnNMRClickListWnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CMenu	popup;
	popup.LoadMenu(IDR_MENU_PROCESS_WND);               
	CMenu*	pM = popup.GetSubMenu(0);                 
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();

	if (m_List.GetSelectedCount() == 0)         
	{ 
		for (int i = 0;i<count;i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          
		}
	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}
