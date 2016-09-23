// ProcessWndChangDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessWndChangDataDlg.h"
#include "afxdialogex.h"


// CProcessWndChangDataDlg 对话框

IMPLEMENT_DYNAMIC(CProcessWndChangDataDlg, CDialog)

CProcessWndChangDataDlg::CProcessWndChangDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessWndChangDataDlg::IDD, pParent)
	, m_Edit(_T(""))
{

}

CProcessWndChangDataDlg::~CProcessWndChangDataDlg()
{
}

void CProcessWndChangDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA, m_Edit);
}


BEGIN_MESSAGE_MAP(CProcessWndChangDataDlg, CDialog)
END_MESSAGE_MAP()


// CProcessWndChangDataDlg 消息处理程序
