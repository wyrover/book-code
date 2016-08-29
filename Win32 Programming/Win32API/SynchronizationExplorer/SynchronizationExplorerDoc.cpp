// SynchronizationExplorerDoc.cpp : implementation of the CSynchronizationExplorerDoc class
//

#include "stdafx.h"
#include "SynchronizationExplorer.h"

#include "SynchronizationExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerDoc

IMPLEMENT_DYNCREATE(CSynchronizationExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CSynchronizationExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CSynchronizationExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerDoc construction/destruction

CSynchronizationExplorerDoc::CSynchronizationExplorerDoc()
{
	// TODO: add one-time construction code here

}

CSynchronizationExplorerDoc::~CSynchronizationExplorerDoc()
{
}

BOOL CSynchronizationExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerDoc serialization

void CSynchronizationExplorerDoc::Serialize(CArchive& ar)
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
// CSynchronizationExplorerDoc diagnostics

#ifdef _DEBUG
void CSynchronizationExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSynchronizationExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerDoc commands
