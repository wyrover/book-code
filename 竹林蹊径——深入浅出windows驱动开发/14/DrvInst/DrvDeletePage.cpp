// DrvDeletePage.cpp : implementation file
//

#include "stdafx.h"
#include "DrvInst.h"
#include "DrvDeletePage.h"
#include "setup.h"

// DrvDeletePage dialog

IMPLEMENT_DYNAMIC(DrvDeletePage, CPropertyPage)

DrvDeletePage::DrvDeletePage()
	: CPropertyPage(DrvDeletePage::IDD)
	, m_strHID(_T(""))
{

}

DrvDeletePage::~DrvDeletePage()
{
}

void DrvDeletePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstctl_devices);
	DDX_Text(pDX, IDC_EDIT1, m_strHID);
}


BEGIN_MESSAGE_MAP(DrvDeletePage, CPropertyPage)
	ON_BN_CLICKED(IDBTN_OK, &DrvDeletePage::OnBnClickedOk)
	ON_BN_CLICKED(IDBTN_DELETE, &DrvDeletePage::OnBnClickedDelete)
END_MESSAGE_MAP()


BOOL DrvDeletePage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);

	return CPropertyPageEx::OnSetActive();
}

void DrvDeletePage::OnBnClickedOk()
{
	UpdateData();	
	if(0 == m_strHID.GetLength())return;

	m_strHID.Trim();
	//if(m_strHIDPre == m_strHID)
	//	return;

	m_strHIDPre = m_strHID;	

	// 找到所有设备
	FindAllDevices(m_strHID.GetBuffer());

	// 显示到ListCtl控件中
	m_lstctl_devices.DeleteAllItems();

	for(int i = 0; i < g_HIDDevices.nCount; i++)
	{
		m_lstctl_devices.InsertItem(i, g_HIDDevices.asDeviceInstanceID[i]);
		m_lstctl_devices.SetItemText(i, 1, g_HIDDevices.bLink[i] ? "连接":"未连");
		m_lstctl_devices.SetItemState(i, LVIS_SELECTED,   LVIS_SELECTED);
	}
}

void DrvDeletePage::OnBnClickedDelete()
{
	int nItem;
	int nConnect = 0;
	int nNotConnect = 0;

	// 是否还有未删除的设备，如果都已经删除了，就放弃操作。
	for(nItem = 0; nItem < g_HIDDevices.nCount; nItem++)
		if(g_HIDDevices.bSuccess[nItem] == false)
			break;

	if(nItem >= g_HIDDevices.nCount)
	{
		MessageBox("已完成", "报告", MB_OK);
		return;
	}

	// 收集信息
	POSITION pos = m_lstctl_devices.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		MessageBox("选择要删除的设备", "无效操作", MB_OK);
		return;
	}

	for(int i = 0; i < g_HIDDevices.nCount; i++)
		g_HIDDevices.bWillBeDelete[i] = false;

	while (pos)
	{
		nItem = m_lstctl_devices.GetNextSelectedItem(pos);

		if(g_HIDDevices.bLink[nItem])
			nConnect++;
		else
			nNotConnect++;

		g_HIDDevices.bWillBeDelete[nItem] = true;
		//g_HIDDevices.bSuccess[nItem] = true;
	}

	// 确认
	CString str;
	str.Format("%d个设备将被删除:\n%d个连接，%d个未连接。", nConnect+nNotConnect, nConnect, nNotConnect);
	if(IDCANCEL == MessageBox(str, "确认", MB_OKCANCEL))
		return;

	// 设备删除
	RemoveDevices();

	int nFail = 0;
	int nSuc = 0;

	for(int i = 0; i < g_HIDDevices.nCount; i++)
	{
		if(g_HIDDevices.bWillBeDelete[i])
		{
			if(g_HIDDevices.bSuccess[i] == false)
				nFail++;
			else
				nSuc++;
		}
	}

	if(nFail == 0)
		MessageBox("全部成功", "报告", MB_OK);
	else{
		CString str;
		str.Format("成功：%d，失败：%d", nSuc, nFail);
		MessageBox(str, "报告", MB_OK);
	}

	if(g_HIDDevices.bNeedReboot)
	{
		if(IDOK == MessageBox("部分设备需要重启后才能完全删除", "提示", MB_OKCANCEL))
			Reboot();
	}
}

BOOL DrvDeletePage::OnWizardFinish()
{
	return CPropertyPage::OnWizardFinish();
}

BOOL DrvDeletePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_lstctl_devices.InsertColumn(0, "SymName", LVCFMT_LEFT, 195);
	m_lstctl_devices.InsertColumn(1, "Status", LVCFMT_LEFT, 75);

	GetDlgItem(IDC_EDIT1)->SetWindowText("USB");
	OnBnClickedOk();
	return TRUE;
}

BOOL DrvDeletePage::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd == GetDlgItem(IDC_EDIT1)->GetSafeHwnd() &&
		pMsg->message == WM_KEYDOWN &&
		pMsg->wParam == VK_RETURN)
	{
		OnBnClickedOk();
		return 1;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

LRESULT DrvDeletePage::OnWizardBack()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetActivePage(1);

	return CPropertyPage::OnWizardBack();
}
