// SnifferSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "SnifferSetting.h"
#include "afxdialogex.h"


// CSnifferSetting 对话框

IMPLEMENT_DYNAMIC(CSnifferSetting, CDialogEx)

CSnifferSetting::CSnifferSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSnifferSetting::IDD, pParent)
	, m_strData(_T(""))
	, m_strDataDescription(_T(""))
{

}

CSnifferSetting::~CSnifferSetting()
{
}

void CSnifferSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strData);
	DDX_Text(pDX, IDC_EDIT2, m_strDataDescription);
}


BEGIN_MESSAGE_MAP(CSnifferSetting, CDialogEx)
END_MESSAGE_MAP()


// CSnifferSetting 消息处理程序
