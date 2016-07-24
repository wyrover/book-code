// MyBrowserDoc.cpp : implementation of the CMyBrowserDoc class
//

#include "stdafx.h"
#include "MyBrowser.h"

#include "MyBrowserDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserDoc

IMPLEMENT_DYNCREATE(CMyBrowserDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyBrowserDoc, CDocument)
	//{{AFX_MSG_MAP(CMyBrowserDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserDoc construction/destruction

CMyBrowserDoc::CMyBrowserDoc()
{
	// TODO: add one-time construction code here

}

CMyBrowserDoc::~CMyBrowserDoc()
{
}

BOOL CMyBrowserDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyBrowserDoc serialization

void CMyBrowserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserDoc diagnostics

#ifdef _DEBUG
void CMyBrowserDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyBrowserDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyBrowserDoc commands
