// HIDPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrvInst.h"
#include "HIDPage.h"
#include "setup.h"
#include "strsafe.h"


// CHIDPage dialog

IMPLEMENT_DYNAMIC(CHIDPage, CPropertyPage)

CHIDPage::CHIDPage()
	: CPropertyPage(CHIDPage::IDD)
{

}

CHIDPage::~CHIDPage()
{
}

void CHIDPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDLST_INF_FILE, m_lstctl_drv_status);
	DDX_Control(pDX, IDLST_INF_FILE2, m_lstctl_HID);
	DDX_Control(pDX, IDBTN_UPDATE, m_btn_update);
}


BEGIN_MESSAGE_MAP(CHIDPage, CPropertyPage)
	ON_BN_CLICKED(IDBTN_UPDATE, &CHIDPage::OnBnClickedUpdate)
END_MESSAGE_MAP()


BOOL CHIDPage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);

	char strNum[12];
	CString str;
	int nNumDev = 0;

	m_lstctl_drv_status.DeleteAllItems();
	m_lstctl_HID.DeleteAllItems();

	for (int i = 0; i < g_nInfCount; i++)
	{
		if(g_InfList[i]->bWillBeInstall)
		{
			m_lstctl_drv_status.InsertItem(nNumDev, g_InfList[i]->asInfName);
			m_lstctl_drv_status.SetItemText(nNumDev, 1, g_InfList[i]->asDesName);

			if(g_InfList[i]->nResultInstall == 1)
				m_lstctl_drv_status.SetItemText(nNumDev, 2, "系统相同");
			else if(g_InfList[i]->nResultInstall == 0 &&  g_InfList[i]->asDesPath[0])
				m_lstctl_drv_status.SetItemText(nNumDev, 2, "成功");
			else
				m_lstctl_drv_status.SetItemText(nNumDev, 2, "失败");

			nNumDev++;
		}
	}

	nNumDev = 0;
	for(int i = 0; i < g_nHIDCount; i++)
	{
		if(g_HIDInfo[i]->bNeedUpdate)
		{
			m_lstctl_HID.InsertItem(nNumDev, g_HIDInfo[i]->asHID);
			m_lstctl_HID.SetItemState(nNumDev, LVIS_SELECTED, LVIS_SELECTED);

			sprintf(strNum, "%d", g_HIDInfo[i]->nNumCone);
			m_lstctl_HID.SetItemText(nNumDev, 1, strNum);

			sprintf(strNum, "%d", g_HIDInfo[i]->nNumNotCone);
			m_lstctl_HID.SetItemText(nNumDev, 2, strNum);

			m_btn_update.ShowWindow(SW_SHOW);
			m_btn_update.EnableWindow(true);

			nNumDev++;
		}
	}

	if(0 == nNumDev)
	{
		m_lstctl_HID.InsertItem(0, "没有设备需要更新");
	}

	return CPropertyPageEx::OnSetActive();
}

// DrvStartPage message handlers

BOOL CHIDPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_lstctl_drv_status.InsertColumn(0, "SrcName", LVCFMT_LEFT, 85);
	m_lstctl_drv_status.InsertColumn(1, "DesName", LVCFMT_LEFT, 85);
	m_lstctl_drv_status.InsertColumn(2, "Status", LVCFMT_LEFT, 65);

	m_lstctl_HID.InsertColumn(0, "Name", LVCFMT_LEFT, 180);
	m_lstctl_HID.InsertColumn(1, "连接设备数", LVCFMT_LEFT, 60);
	m_lstctl_HID.InsertColumn(2, "未连接设备数", LVCFMT_LEFT, 60);

	return TRUE; 
}

void CHIDPage::OnBnClickedUpdate()
{
	int nItem;
	POSITION pos = m_lstctl_HID.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		MessageBox("选择设备ID", "无效操作", MB_OK);
		return;
	}

	for(int i = 0; i < g_nHIDCount; i++)
		g_HIDInfo[i]->bWillBeUpdate = false;

	while (pos)
	{
		nItem = m_lstctl_HID.GetNextSelectedItem(pos);
		g_HIDInfo[nItem]->bWillBeUpdate = true;		
		g_HIDInfo[nItem]->bSuccess = true;		
	}

	UpdateDriver();

	int nSuc = 0;
	int nFail = 0;
	for(int i = 0; i < g_nHIDCount; i++)
	{
		if(g_HIDInfo[i]->bWillBeUpdate)
		{
			if(g_HIDInfo[nItem]->bSuccess)nSuc++;
			else nFail++;
		}
	}

	if(nFail == 0)
	{
		MessageBox("全部成功", "结果", MB_OK);
	}else
	{
		CString str;
		str.Format("成功：%d；失败：%d", nSuc, nFail);
		MessageBox(str, "结果", MB_OK);
	}

	m_btn_update.EnableWindow(false);
}
