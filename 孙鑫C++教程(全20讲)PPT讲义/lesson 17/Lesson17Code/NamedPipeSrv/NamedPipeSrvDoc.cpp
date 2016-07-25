// NamedPipeSrvDoc.cpp : implementation of the CNamedPipeSrvDoc class
//

#include "stdafx.h"
#include "NamedPipeSrv.h"

#include "NamedPipeSrvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvDoc

IMPLEMENT_DYNCREATE(CNamedPipeSrvDoc, CDocument)

BEGIN_MESSAGE_MAP(CNamedPipeSrvDoc, CDocument)
	//{{AFX_MSG_MAP(CNamedPipeSrvDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvDoc construction/destruction

CNamedPipeSrvDoc::CNamedPipeSrvDoc()
{
	// TODO: add one-time construction code here

}

CNamedPipeSrvDoc::~CNamedPipeSrvDoc()
{
}

BOOL CNamedPipeSrvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvDoc serialization

void CNamedPipeSrvDoc::Serialize(CArchive& ar)
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
// CNamedPipeSrvDoc diagnostics

#ifdef _DEBUG
void CNamedPipeSrvDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNamedPipeSrvDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeSrvDoc commands
