// GraphShowerDoc.cpp : CGraphShowerDoc 类的实现
//

#include "stdafx.h"
#include "GraphShower.h"

#include "GraphShowerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphShowerDoc

IMPLEMENT_DYNCREATE(CGraphShowerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphShowerDoc, CDocument)
END_MESSAGE_MAP()


// CGraphShowerDoc 构造/析构

CGraphShowerDoc::CGraphShowerDoc()
{
	// TODO: 在此添加一次性构造代码

}

CGraphShowerDoc::~CGraphShowerDoc()
{
}

BOOL CGraphShowerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CGraphShowerDoc 序列化

void CGraphShowerDoc::Serialize(CArchive& ar)
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


// CGraphShowerDoc 诊断

#ifdef _DEBUG
void CGraphShowerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphShowerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGraphShowerDoc 命令
