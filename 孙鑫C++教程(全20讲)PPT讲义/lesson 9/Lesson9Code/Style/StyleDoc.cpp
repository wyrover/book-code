// StyleDoc.cpp : implementation of the CStyleDoc class
//

#include "stdafx.h"
#include "Style.h"

#include "StyleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDoc

IMPLEMENT_DYNCREATE(CStyleDoc, CDocument)

BEGIN_MESSAGE_MAP(CStyleDoc, CDocument)
	//{{AFX_MSG_MAP(CStyleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDoc construction/destruction

CStyleDoc::CStyleDoc()
{
	// TODO: add one-time construction code here

}

CStyleDoc::~CStyleDoc()
{
}

BOOL CStyleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CStyleDoc serialization

void CStyleDoc::Serialize(CArchive& ar)
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
// CStyleDoc diagnostics

#ifdef _DEBUG
void CStyleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStyleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStyleDoc commands
