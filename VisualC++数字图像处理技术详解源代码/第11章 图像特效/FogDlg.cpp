// FogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicHouse.h"
#include "FogDlg.h"
#include "PreviewDlg.h"
#include "Filter.h"
#include "Mainfrm.h"
#include "MagicHouseDoc.h"
#include "MagicHouseView.h"


// CFogDlg 对话框

IMPLEMENT_DYNAMIC(CFogDlg, CDialog)

CFogDlg::CFogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFogDlg::IDD, pParent)
	, m_nType(0)
	, m_nValue(1)
	, m_nPos(0)
{
	m_bModify = false;
}

CFogDlg::~CFogDlg()
{
}

void CFogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_HFOG, m_nType);
	DDX_Text(pDX, IDC_VALUE, m_nValue);
	DDX_Slider(pDX, IDC_SLIDER, m_nPos);
}


BEGIN_MESSAGE_MAP(CFogDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV, &CFogDlg::OnBnClickedPrev)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SAVE, &CFogDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CFogDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/******************************************************************************
*	作用:		初始化对话框
******************************************************************************/
BOOL CFogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置滑块的范围
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetRange(1, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/******************************************************************************
*	作用:		滑块的响应函数
******************************************************************************/
void CFogDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	m_nValue = m_nPos;
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/******************************************************************************
*	作用:		重新进行图像的雾化处理
******************************************************************************/
void CFogDlg::Refresh()
{
	UpdateData(TRUE);

	// 更新预览对话框
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	if (m_nType == 0)
		HorFog(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight, m_nValue);
	else if (m_nType == 1)
		VerFog(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight, m_nValue);
	else
		ComFog(pView->m_pImageBuffer, pView->m_pImageTempBuffer, 
		pView->m_nPicWidth, pView->m_nPicHeight, m_nValue);
	
	m_bModify = true;
}

/******************************************************************************
*	作用:		“预览”按钮的响应函数
******************************************************************************/
void CFogDlg::OnBnClickedPrev()
{
	Refresh();

	CPreviewDlg dlg;
	dlg.m_nType = 1;
	dlg.DoModal();
}

/******************************************************************************
*	作用:		“保存”按钮的响应函数
******************************************************************************/
void CFogDlg::OnBnClickedSave()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CMagicHouseView* pView = (CMagicHouseView*)pMain->GetActiveView();

	Refresh();
	pView->OnFileSave();
	m_bModify = false;
}

/******************************************************************************
*	作用:		“确定”按钮的响应函数
******************************************************************************/
void CFogDlg::OnBnClickedOk()
{
	if (m_bModify)
	{
		if (IDYES == MessageBox(L"图像已经更改，是否保存？", L"Magic House", MB_YESNO))
			OnBnClickedSave();
	}
	OnOK();
}