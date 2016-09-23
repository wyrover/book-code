

#include "stdafx.h"
#include "AntiSpy.h"
#include "DriverModule.h"
#include "afxdialogex.h"



IMPLEMENT_DYNAMIC(CDriverModule, CDialog)

CDriverModule::CDriverModule(CWnd* pParent )
	: CDialog(CDriverModule::IDD, pParent)
{

}

CDriverModule::~CDriverModule()
{
}

void CDriverModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DRIVER_MODULE, m_List);
}


BEGIN_MESSAGE_MAP(CDriverModule, CDialog)
END_MESSAGE_MAP()


VOID CDriverModule::InitControl()
{
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,L"驱动名",LVCFMT_CENTER,85);
	m_List.InsertColumn(1,L"基地址",LVCFMT_CENTER,120);
	m_List.InsertColumn(2,L"大小",LVCFMT_CENTER,70);
	m_List.InsertColumn(3,L"驱动对象",LVCFMT_CENTER,80);
	m_List.InsertColumn(4,L"驱动路径",LVCFMT_CENTER,260);
	m_List.InsertColumn(5,L"服务名",LVCFMT_CENTER,120);
	m_List.InsertColumn(6,L"文件厂商",LVCFMT_CENTER,120);

	SetWindowLong(m_List.m_hWnd ,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);                   			  
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
}

BOOL CDriverModule::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControl();

	return TRUE;  
}


