// ParentDoc.cpp : implementation of the CParentDoc class
//

#include "stdafx.h"
#include "Parent.h"

#include "ParentDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParentDoc

IMPLEMENT_DYNCREATE(CParentDoc, CDocument)

BEGIN_MESSAGE_MAP(CParentDoc, CDocument)
	//{{AFX_MSG_MAP(CParentDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParentDoc construction/destruction

CParentDoc::CParentDoc()
{
	// TODO: add one-time construction code here

}

CParentDoc::~CParentDoc()
{
}

BOOL CParentDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CParentDoc serialization

void CParentDoc::Serialize(CArchive& ar)
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
// CParentDoc diagnostics

#ifdef _DEBUG
void CParentDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CParentDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParentDoc commands
