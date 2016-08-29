// ControlExplorerDoc.cpp : implementation of the CControlExplorerDoc class
//

#include "stdafx.h"
#include "ControlExplorer.h"

#include "ControlExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerDoc

IMPLEMENT_DYNCREATE(CControlExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CControlExplorerDoc, CDocument)
	//{{AFX_MSG_MAP(CControlExplorerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerDoc construction/destruction

CControlExplorerDoc::CControlExplorerDoc()
{
	// TODO: add one-time construction code here

}

CControlExplorerDoc::~CControlExplorerDoc()
{
}

BOOL CControlExplorerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerDoc serialization

void CControlExplorerDoc::Serialize(CArchive& ar)
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
// CControlExplorerDoc diagnostics

#ifdef _DEBUG
void CControlExplorerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CControlExplorerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CControlExplorerDoc commands
