// SegmentDoc.cpp : CSegmentDoc 类的实现
//

#include "stdafx.h"
#include "Segment.h"

#include "SegmentDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSegmentDoc

IMPLEMENT_DYNCREATE(CSegmentDoc, CDocument)

BEGIN_MESSAGE_MAP(CSegmentDoc, CDocument)
END_MESSAGE_MAP()


// CSegmentDoc 构造/析构

CSegmentDoc::CSegmentDoc()
{
	// TODO: 在此添加一次性构造代码

}

CSegmentDoc::~CSegmentDoc()
{
}

BOOL CSegmentDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CSegmentDoc 序列化

void CSegmentDoc::Serialize(CArchive& ar)
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


// CSegmentDoc 诊断

#ifdef _DEBUG
void CSegmentDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSegmentDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSegmentDoc 命令

CDib* CSegmentDoc::GetDib(void)
{
    return &dib;
}

BOOL CSegmentDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  在此添加您专用的创建代码
    if(!dib.LoadFromFile(lpszPathName))
    {
        AfxMessageBox("加载位图操作失败！");
        return FALSE;
    }

    return TRUE;
}
