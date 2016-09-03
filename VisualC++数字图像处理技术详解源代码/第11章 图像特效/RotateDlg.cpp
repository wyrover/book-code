// RotateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "RotateDlg.h"
#include "GeoOperator.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"


// CRotateDlg 对话框

#define PI 3.1415926535

IMPLEMENT_DYNAMIC(CRotateDlg, CDialog)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateDlg::IDD, pParent)
	, m_nAngle(0)
	, m_nPos(0)
	, m_nType(0)
{
}

CRotateDlg::~CRotateDlg()
{
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ANGLE, m_nAngle);
	DDX_Slider(pDX, IDC_SLIDER, m_nPos);
	DDX_CBIndex(pDX, IDC_MODE, m_nType);
}

BEGIN_MESSAGE_MAP(CRotateDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV, &CRotateDlg::OnBnClickedPrev)
	ON_EN_CHANGE(IDC_ANGLE, &CRotateDlg::OnEnChangeAngle)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SAVE, &CRotateDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CRotateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRotateDlg 消息处理程序

/******************************************************************************
*	作用:		对话框初始化
******************************************************************************/
BOOL CRotateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 修改“旋转角度”的范围
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetRange(0, 360);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/******************************************************************************
*	作用:		“预览效果”按钮的响应函数
******************************************************************************/
void CRotateDlg::OnBnClickedPrev()
{
	Refresh();

	CPreviewDlg dlg;
	dlg.m_nType = 1;
	dlg.DoModal();
}

/******************************************************************************
*	作用:		重新进行图像旋转
******************************************************************************/
void CRotateDlg::Refresh()
{
	UpdateData(TRUE);

	// 更新预览对话框
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	SAFE_DELETE_ARRAY (pView->m_pImageTempBuffer);

	if (m_nType == 0)
		RotateNormal(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
					 pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nTempWidth, 
					 pView->m_nTempHeight, m_nAngle / 180.0 * PI);
	else
		RotateInterpolation(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
					 pView->m_nPicWidth, pView->m_nPicHeight, pView->m_nTempWidth, 
					 pView->m_nTempHeight, m_nAngle / 180.0 * PI);
}

/******************************************************************************
*	作用:		“角度”改变时的响应函数
******************************************************************************/
void CRotateDlg::OnEnChangeAngle()
{
	UpdateData(TRUE);

	if (m_nAngle < 0)
		m_nAngle = 0;
	if (m_nAngle > 360)
		m_nAngle = 360;

	m_nPos = m_nAngle;

	UpdateData(FALSE);
}

/******************************************************************************
*	作用:		水平滑块的响应函数
******************************************************************************/
void CRotateDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	m_nAngle = m_nPos;
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRotateDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	Refresh();
	pView->OnFileSave();
	m_bModify = false;
}

void CRotateDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}

	OnOK();
}
