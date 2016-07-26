/******************************************************************************\
*	IME.cpp: 定义应用程序的类行为。
*
*	这是 Microsoft 源代码示例的一部分。
*	版权所有(C) Microsoft Corporation。
*	保留所有权利。
*
*	此源代码仅用作 Microsoft 
*	开发工具和/或 WinHelp 文档的补充。
*	有关 Microsoft 示例程序的详细信息，
*	请参见这些来源。
\******************************************************************************/


#include "stdafx.h"
#include "IMM.h"
#include "IMEEdit.h"
#include "IME.h"
#include "IMEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIMEApp

BEGIN_MESSAGE_MAP(CIMEApp, CWinApp)
	//{{AFX_MSG_MAP(CIMEApp)
		// 注意 - ClassWizard 将在此处添加和移除映射宏。
		//    不要编辑这些生成的代码块中的内容!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMEApp 构造

CIMEApp::CIMEApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

/////////////////////////////////////////////////////////////////////////////
// 唯一的 CIMEApp 对象

CIMEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIMEApp 初始化

BOOL CIMEApp::InitInstance()
{
	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	//  最终可执行文件的大小，则应从下列内容中移除
	//  不需要的特定初始化例程。

	CIMEDlg dlg;
	dlg.DoModal();

	return FALSE;
}
