// CellDetectionDoc.cpp : CCellDetectionDoc 类的实现
//

#include "stdafx.h"
#include "CellDetection.h"

#include "CellDetectionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCellDetectionDoc

IMPLEMENT_DYNCREATE(CCellDetectionDoc, CDocument)

BEGIN_MESSAGE_MAP(CCellDetectionDoc, CDocument)
END_MESSAGE_MAP()


// CCellDetectionDoc 构造/析构

CCellDetectionDoc::CCellDetectionDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCellDetectionDoc::~CCellDetectionDoc()
{
}

BOOL CCellDetectionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CCellDetectionDoc 序列化

void CCellDetectionDoc::Serialize(CArchive& ar)
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


// CCellDetectionDoc 诊断

#ifdef _DEBUG
void CCellDetectionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCellDetectionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCellDetectionDoc 命令
