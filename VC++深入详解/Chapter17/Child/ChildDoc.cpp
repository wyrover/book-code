// ChildDoc.cpp : implementation of the CChildDoc class
//

#include "stdafx.h"
#include "Child.h"

#include "ChildDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildDoc

IMPLEMENT_DYNCREATE(CChildDoc, CDocument)

BEGIN_MESSAGE_MAP(CChildDoc, CDocument)
	//{{AFX_MSG_MAP(CChildDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildDoc construction/destruction

CChildDoc::CChildDoc()
{
	// TODO: add one-time construction code here

}

CChildDoc::~CChildDoc()
{
}

BOOL CChildDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildDoc serialization

void CChildDoc::Serialize(CArchive& ar)
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
// CChildDoc diagnostics

#ifdef _DEBUG
void CChildDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChildDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildDoc commands
