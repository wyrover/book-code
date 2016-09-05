// MoveTranDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "MoveTranDlg.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"
#include "GeoOperator.h"


// CMoveTranDlg 对话框

IMPLEMENT_DYNAMIC(CMoveTranDlg, CDialog)

CMoveTranDlg::CMoveTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveTranDlg::IDD, pParent)
	, m_nType(0)
	, m_nX(0)
	, m_nY(0)
{

}

CMoveTranDlg::~CMoveTranDlg()
{
	SAFE_DELETE (dlg);
}

void CMoveTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NORMAL, m_nType);
	DDX_Text(pDX, IDC_EDIT_X, m_nX);
	DDX_Text(pDX, IDC_EDIT_Y, m_nY);
}

BEGIN_MESSAGE_MAP(CMoveTranDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_NORMAL, &CMoveTranDlg::OnBnClickedRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_SIZE, &CMoveTranDlg::OnBnClickedRadioSize)
	ON_EN_CHANGE(IDC_EDIT_X, &CMoveTranDlg::OnEnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, &CMoveTranDlg::OnEnChangeEditY)
	ON_BN_CLICKED(IDC_SAVE, &CMoveTranDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CMoveTranDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMoveTranDlg 消息处理程序

/******************************************************************************
*	作用:		对话框初始化时创建预览窗口
******************************************************************************/
BOOL CMoveTranDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	dlg = new CPreviewDlg(this);

	dlg->m_nType = 1;
	dlg->m_bEnableClose = false;
	dlg->Create(IDD_PREVIEW);
	dlg->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/******************************************************************************
*	作用:		重新进行平移变换并刷新预览窗口
******************************************************************************/
void CMoveTranDlg::Refresh()
{
	UpdateData(TRUE);

	// 更新预览对话框
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	SAFE_DELETE_ARRAY (pView->m_pImageTempBuffer);
	pView->m_nTempWidth = pView->m_nPicWidth;
	pView->m_nTempHeight = pView->m_nPicHeight;

	if (m_nType == 0)
		MoveTransNormal(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
						pView->m_nPicWidth, pView->m_nPicHeight, m_nX, m_nY);
	else
		MoveTransSize(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
					  pView->m_nPicWidth, pView->m_nPicHeight, m_nX, m_nY, 
					  pView->m_nTempWidth, pView->m_nTempHeight);

	dlg->Refresh();
}

void CMoveTranDlg::OnBnClickedRadioNormal()
{
	Refresh();
}

void CMoveTranDlg::OnBnClickedRadioSize()
{
	Refresh();
}

void CMoveTranDlg::OnEnChangeEditX()
{
	Refresh();
}

void CMoveTranDlg::OnEnChangeEditY()
{
	Refresh();
}

void CMoveTranDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	pView->OnFileSave();
	m_bModify = false;
}

void CMoveTranDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}

	OnOK();
}