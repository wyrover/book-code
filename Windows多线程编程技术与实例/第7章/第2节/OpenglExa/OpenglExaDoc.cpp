// OpenglExaDoc.cpp : implementation of the COpenglExaDoc class
//

#include "stdafx.h"
#include "OpenglExa.h"

#include "OpenglExaDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenglExaDoc

IMPLEMENT_DYNCREATE(COpenglExaDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenglExaDoc, CDocument)
	//{{AFX_MSG_MAP(COpenglExaDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenglExaDoc construction/destruction

COpenglExaDoc::COpenglExaDoc()
{
	// TODO: add one-time construction code here

}

COpenglExaDoc::~COpenglExaDoc()
{
}

BOOL COpenglExaDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COpenglExaDoc serialization

void COpenglExaDoc::Serialize(CArchive& ar)
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
// COpenglExaDoc diagnostics

#ifdef _DEBUG
void COpenglExaDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenglExaDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenglExaDoc commands
