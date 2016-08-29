// FormatMessageBoxDoc.cpp : implementation of the CFormatMessageBoxDoc class
//

#include "stdafx.h"
#include "FormatMessageBox.h"

#include "FormatMessageBoxDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxDoc

IMPLEMENT_DYNCREATE(CFormatMessageBoxDoc, CDocument)

BEGIN_MESSAGE_MAP(CFormatMessageBoxDoc, CDocument)
	//{{AFX_MSG_MAP(CFormatMessageBoxDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxDoc construction/destruction

CFormatMessageBoxDoc::CFormatMessageBoxDoc()
{
	// TODO: add one-time construction code here

}

CFormatMessageBoxDoc::~CFormatMessageBoxDoc()
{
}

BOOL CFormatMessageBoxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxDoc serialization

void CFormatMessageBoxDoc::Serialize(CArchive& ar)
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
// CFormatMessageBoxDoc diagnostics

#ifdef _DEBUG
void CFormatMessageBoxDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFormatMessageBoxDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormatMessageBoxDoc commands
