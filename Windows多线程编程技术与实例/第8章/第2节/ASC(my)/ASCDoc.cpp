// ASCDoc.cpp : implementation of the CASCDoc class
//

#include "stdafx.h"
#include "ASC.h"

#include "ASCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CASCDoc

IMPLEMENT_DYNCREATE(CASCDoc, CDocument)

BEGIN_MESSAGE_MAP(CASCDoc, CDocument)
	//{{AFX_MSG_MAP(CASCDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CASCDoc construction/destruction

CASCDoc::CASCDoc()
{
	// TODO: add one-time construction code here

}

CASCDoc::~CASCDoc()
{
}

BOOL CASCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CASCDoc serialization

void CASCDoc::Serialize(CArchive& ar)
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
// CASCDoc diagnostics

#ifdef _DEBUG
void CASCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CASCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CASCDoc commands
