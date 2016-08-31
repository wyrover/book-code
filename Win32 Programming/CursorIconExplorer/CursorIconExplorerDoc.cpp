// CursorIconExplorerDoc.cpp : implementation of the CCursorIconExplorerDoc class
//

#include "stdafx.h"
#include "CursorIconExplorer.h"

#include "CursorIconExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerDoc

IMPLEMENT_DYNCREATE(CCursorIconExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCursorIconExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CCursorIconExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerDoc construction/destruction

CCursorIconExplorerDoc::CCursorIconExplorerDoc()
{
	// TODO: add one-time construction code here

}

CCursorIconExplorerDoc::~CCursorIconExplorerDoc()
{
}

BOOL CCursorIconExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerDoc serialization

void CCursorIconExplorerDoc::Serialize(CArchive& ar)
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
// CCursorIconExplorerDoc diagnostics

#ifdef _DEBUG
void CCursorIconExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCursorIconExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCursorIconExplorerDoc commands
