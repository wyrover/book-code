// PhotoProcessDoc.cpp : CPhotoProcessDoc 类的实现
//

#include "stdafx.h"
#include "PhotoProcess.h"

#include "PhotoProcessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoProcessDoc

IMPLEMENT_DYNCREATE(CPhotoProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CPhotoProcessDoc, CDocument)
END_MESSAGE_MAP()


// CPhotoProcessDoc 构造/析构

CPhotoProcessDoc::CPhotoProcessDoc()
{
	// TODO: 在此添加一次性构造代码

}

CPhotoProcessDoc::~CPhotoProcessDoc()
{
}

BOOL CPhotoProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CPhotoProcessDoc 序列化

void CPhotoProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CPhotoProcessDoc 诊断

#ifdef _DEBUG
void CPhotoProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhotoProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPhotoProcessDoc 命令
