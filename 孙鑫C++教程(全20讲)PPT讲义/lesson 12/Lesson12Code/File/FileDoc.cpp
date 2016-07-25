// FileDoc.cpp : implementation of the CFileDoc class
//

#include "stdafx.h"
#include "File.h"

#include "FileDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDoc

IMPLEMENT_DYNCREATE(CFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CFileDoc, CDocument)
	//{{AFX_MSG_MAP(CFileDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDoc construction/destruction

CFileDoc::CFileDoc()
{
	// TODO: add one-time construction code here

}

CFileDoc::~CFileDoc()
{
}

BOOL CFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFileDoc serialization

void CFileDoc::Serialize(CArchive& ar)
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
// CFileDoc diagnostics

#ifdef _DEBUG
void CFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileDoc commands
