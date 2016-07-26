// IME.h:IME 应用程序的主头文件
//

#if !defined(AFX_IME_H__8E60BE06_2516_11D4_8ABF_00B0D02D1522__INCLUDED_)
#define AFX_IME_H__8E60BE06_2516_11D4_8ABF_00B0D02D1522__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error 在包含此文件之前包含“stdafx.h”以生成 PCH
#endif

#include "resource.h"		// 主符号

/////////////////////////////////////////////////////////////////////////////
// CIMEApp:
// 有关此类的实现，请参见 IME.cpp
//

class CIMEApp : public CWinApp
{
public:
	CIMEApp();

// 重写
	// ClassWizard 生成的虚函数重写
	//{{AFX_VIRTUAL(CIMEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// 实现

	//{{AFX_MSG(CIMEApp)
		// 注意 - ClassWizard 将在此处添加和移除成员函数。
		//    不要编辑这些生成的代码块中的内容!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ 将紧挨上一行之前插入附加声明。

#endif // !defined(AFX_IME_H__8E60BE06_2516_11D4_8ABF_00B0D02D1522__INCLUDED_)
