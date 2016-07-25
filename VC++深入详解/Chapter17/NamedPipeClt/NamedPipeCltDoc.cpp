// NamedPipeCltDoc.cpp : implementation of the CNamedPipeCltDoc class
//

#include "stdafx.h"
#include "NamedPipeClt.h"

#include "NamedPipeCltDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltDoc

IMPLEMENT_DYNCREATE(CNamedPipeCltDoc, CDocument)

BEGIN_MESSAGE_MAP(CNamedPipeCltDoc, CDocument)
	//{{AFX_MSG_MAP(CNamedPipeCltDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltDoc construction/destruction

CNamedPipeCltDoc::CNamedPipeCltDoc()
{
	// TODO: add one-time construction code here

}

CNamedPipeCltDoc::~CNamedPipeCltDoc()
{
}

BOOL CNamedPipeCltDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltDoc serialization

void CNamedPipeCltDoc::Serialize(CArchive& ar)
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
// CNamedPipeCltDoc diagnostics

#ifdef _DEBUG
void CNamedPipeCltDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNamedPipeCltDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeCltDoc commands
