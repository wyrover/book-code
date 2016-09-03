// FngprntDoc.cpp : CFngprntDoc 类的实现
//

#include "stdafx.h"
#include "Fngprnt.h"

#include "FngprntDoc.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFngprntDoc

IMPLEMENT_DYNCREATE(CFngprntDoc, CDocument)

BEGIN_MESSAGE_MAP(CFngprntDoc, CDocument)
END_MESSAGE_MAP()


// CFngprntDoc 构造/析构

CFngprntDoc::CFngprntDoc()
{
	// TODO: 在此添加一次性构造代码

}

CFngprntDoc::~CFngprntDoc()
{
}

BOOL CFngprntDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CFngprntDoc 序列化

void CFngprntDoc::Serialize(CArchive& ar)
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


// CFngprntDoc 诊断

#ifdef _DEBUG
void CFngprntDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFngprntDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFngprntDoc 命令


CDib* CFngprntDoc::GetDib(void)
{
    return &dib;
}


BOOL CFngprntDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  在此添加您专用的创建代码

    // 如果加载位图失败
    if(!dib.LoadFromFile(lpszPathName))
    {
        // 显示对话框，提示用户位图加载失败
        AfxMessageBox("加载位图操作失败！");
        return FALSE;
    }

    return TRUE;
}
