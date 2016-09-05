// MirTranDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "MirTranDlg.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"
#include "GeoOperator.h"


// CMirTranDlg 对话框

IMPLEMENT_DYNAMIC(CMirTranDlg, CDialog)

CMirTranDlg::CMirTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMirTranDlg::IDD, pParent)
	, m_nType(0)
{

}

CMirTranDlg::~CMirTranDlg()
{
	SAFE_DELETE (dlg);
}

void CMirTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_MIRROR_H, m_nType);
}


BEGIN_MESSAGE_MAP(CMirTranDlg, CDialog)
	ON_BN_CLICKED(IDC_MIRROR_H, &CMirTranDlg::OnBnClickedMirrorH)
	ON_BN_CLICKED(IDC_MIRROR_V, &CMirTranDlg::OnBnClickedMirrorV)
	ON_BN_CLICKED(IDC_SAVE, &CMirTranDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CMirTranDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMirTranDlg 消息处理程序

BOOL CMirTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/******************************************************************************
*	作用:		重新进行镜像变换并刷新预览窗口
******************************************************************************/
void CMirTranDlg::Refresh()
{
	UpdateData(TRUE);

	// 更新预览对话框
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	SAFE_DELETE_ARRAY (pView->m_pImageTempBuffer);

	if (m_nType == 0)
		HMirrorTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight);
	else
		VMirrorTrans(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight);

	dlg->Refresh();
}

/******************************************************************************
*	作用:		选择“水平镜像”的命令响应函数
******************************************************************************/
void CMirTranDlg::OnBnClickedMirrorH()
{
	Refresh();
}

/******************************************************************************
*	作用:		选择“垂直镜像”的命令响应函数
******************************************************************************/
void CMirTranDlg::OnBnClickedMirrorV()
{
	Refresh();
}

void CMirTranDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}

void CMirTranDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}

	OnOK();
}
