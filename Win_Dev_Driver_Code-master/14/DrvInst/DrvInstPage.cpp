// DrvInstPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrvInst.h"
#include "DrvInstPage.h"
#include "setup.h"
#include <strsafe.h>

// DrvInstPage dialog

IMPLEMENT_DYNAMIC(DrvInstPage, CPropertyPage)

DrvInstPage::DrvInstPage()
	: CPropertyPage(DrvInstPage::IDD)
{

}

DrvInstPage::~DrvInstPage()
{
}

void DrvInstPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDLST_INF_FILE, m_lstbox_inf);
	DDX_Control(pDX, IDC_EDIT1, m_edt_curdir);
}


BEGIN_MESSAGE_MAP(DrvInstPage, CPropertyPage)
	ON_BN_CLICKED(IDBTN_SEL_DIR, &DrvInstPage::OnBnClickedSelDir)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL DrvInstPage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);

	return CPropertyPageEx::OnSetActive();
}

LRESULT DrvInstPage::OnWizardNext()
{	
	int nItem;
	POSITION pos = m_lstbox_inf.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		MessageBox("选择安装文件", "无效操作", MB_OK);
		return -1;
	}

	for(int i = 0; i < g_nHIDCount; i++)
		g_HIDInfo[i]->bNeedUpdate = false;

	for(int i = 0; i < g_nInfCount; i++)
		g_InfList[i]->bWillBeInstall = false;

	while (pos)
	{
		nItem = m_lstbox_inf.GetNextSelectedItem(pos);
		g_InfList[nItem]->bWillBeInstall = true;
	}

	InstallDriver();

	return CPropertyPage::OnWizardNext();
}

BOOL DrvInstPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_lstbox_inf.InsertColumn(0, "Name", LVCFMT_LEFT, 80);
	m_lstbox_inf.InsertColumn(1, "Version", LVCFMT_LEFT, 140);
	m_lstbox_inf.InsertColumn(2, "Provider", LVCFMT_LEFT, 80);
	m_lstbox_inf.InsertColumn(3, "Sign", LVCFMT_LEFT, 75);

	char strCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurDir);
	m_edt_curdir.SetWindowText(strCurDir);
	StringCchCopy(m_asPath, MAX_PATH, strCurDir);

	RefreshListCtl();
	return TRUE; 
}


void DrvInstPage::OnBnClickedSelDir()
{
	char szPath[MAX_PATH];     //存放选择的目录路径
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = this->m_hWnd;   
	bi.pidlRoot = NULL; 
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "选择搜索目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		m_edt_curdir.SetWindowText(szPath);
		StringCchCopy(m_asPath, MAX_PATH, szPath);
		FreeAllInfFiles();
		FindAllInfFiles(szPath);
		RefreshListCtl();
	}
}

void DrvInstPage::RefreshListCtl()
{	
	m_lstbox_inf.DeleteAllItems();
	
	for (int i = 0; i < g_nInfCount; i++)
	{
		m_lstbox_inf.InsertItem(i, g_InfList[i]->asInfName);

		m_lstbox_inf.SetItemText(i, 1, g_InfList[i]->asVersion);
		m_lstbox_inf.SetItemText(i, 2, g_InfList[i]->asProvider);
		m_lstbox_inf.SetItemText(i, 3, g_InfList[i]->asDSign);
	}
}

void DrvInstPage::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CPropertyPage::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL DrvInstPage::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd == m_edt_curdir.GetSafeHwnd() &&
		pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_RETURN)
	{
		CString str;
		char* ptr;
		m_edt_curdir.GetWindowText(str);
		ptr = str.GetBuffer();

		if(0 != strcmp(ptr, m_asPath))
		{
			m_edt_curdir.SetWindowText(ptr);
			StringCchCopy(m_asPath, MAX_PATH, ptr);
			FreeAllInfFiles();
			FindAllInfFiles(ptr);
			RefreshListCtl();
		}

		str.ReleaseBuffer();
		return 1;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}
