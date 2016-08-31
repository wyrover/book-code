// DC ExplorerDoc.cpp : implementation of the CDCExplorerDoc class
//

#include "stdafx.h"
#include "DCExplorer.h"

#include "DCExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerDoc

IMPLEMENT_DYNCREATE(CDCExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CDCExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CDCExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerDoc construction/destruction

CDCExplorerDoc::CDCExplorerDoc()
{
	// TODO: add one-time construction code here

}

CDCExplorerDoc::~CDCExplorerDoc()
{
}

BOOL CDCExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerDoc serialization

void CDCExplorerDoc::Serialize(CArchive& ar)
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
// CDCExplorerDoc diagnostics

#ifdef _DEBUG
void CDCExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerDoc commands
