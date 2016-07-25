// drawLineDoc.cpp : implementation of the CDrawLineDoc class
//

#include "stdafx.h"
#include "drawLine.h"

#include "drawLineDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawLineDoc

IMPLEMENT_DYNCREATE(CDrawLineDoc, CDocument)

BEGIN_MESSAGE_MAP(CDrawLineDoc, CDocument)
	//{{AFX_MSG_MAP(CDrawLineDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawLineDoc construction/destruction

CDrawLineDoc::CDrawLineDoc()
{
	// TODO: add one-time construction code here

}

CDrawLineDoc::~CDrawLineDoc()
{
}

BOOL CDrawLineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDrawLineDoc serialization

void CDrawLineDoc::Serialize(CArchive& ar)
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
// CDrawLineDoc diagnostics

#ifdef _DEBUG
void CDrawLineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDrawLineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawLineDoc commands
