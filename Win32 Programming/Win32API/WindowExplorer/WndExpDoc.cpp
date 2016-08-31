// Window ExplorerDoc.cpp : implementation of the CWindowExplorerDoc class
//

#include "stdafx.h"
#include "WndExp.h"

#include "WndExpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndExpDoc

IMPLEMENT_DYNCREATE(CWndExpDoc, CDocument)

BEGIN_MESSAGE_MAP(CWndExpDoc, CDocument)
	//{{AFX_MSG_MAP(CWndExpDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndExpDoc construction/destruction

CWndExpDoc::CWndExpDoc()
{
	// TODO: add one-time construction code here

}

CWndExpDoc::~CWndExpDoc()
{
}

BOOL CWndExpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWndExpDoc serialization

void CWndExpDoc::Serialize(CArchive& ar)
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
// CWndExpDoc diagnostics

#ifdef _DEBUG
void CWndExpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWndExpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWndExpDoc commands
