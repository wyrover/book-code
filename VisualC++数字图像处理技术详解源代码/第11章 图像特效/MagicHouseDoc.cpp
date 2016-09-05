// MagicHouseDoc.cpp : CMagicHouseDoc 类的实现
//

#include "stdafx.h"
#include "MagicHouse.h"

#include "MagicHouseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicHouseDoc

IMPLEMENT_DYNCREATE(CMagicHouseDoc, CDocument)

BEGIN_MESSAGE_MAP(CMagicHouseDoc, CDocument)
END_MESSAGE_MAP()


// CMagicHouseDoc 构造/析构

CMagicHouseDoc::CMagicHouseDoc()
{
	// TODO: 在此添加一次性构造代码

}

CMagicHouseDoc::~CMagicHouseDoc()
{
}

BOOL CMagicHouseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMagicHouseDoc 序列化

void CMagicHouseDoc::Serialize(CArchive& ar)
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


// CMagicHouseDoc 诊断

#ifdef _DEBUG
void CMagicHouseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMagicHouseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMagicHouseDoc 命令
