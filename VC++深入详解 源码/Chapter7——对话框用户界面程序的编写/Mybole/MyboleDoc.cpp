// MyboleDoc.cpp : implementation of the CMyboleDoc class
//

#include "stdafx.h"
#include "Mybole.h"

#include "MyboleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyboleDoc

IMPLEMENT_DYNCREATE(CMyboleDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyboleDoc, CDocument)
	//{{AFX_MSG_MAP(CMyboleDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyboleDoc construction/destruction

CMyboleDoc::CMyboleDoc()
{
	// TODO: add one-time construction code here

}

CMyboleDoc::~CMyboleDoc()
{
}

BOOL CMyboleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMyboleDoc serialization

void CMyboleDoc::Serialize(CArchive& ar)
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
// CMyboleDoc diagnostics

#ifdef _DEBUG
void CMyboleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyboleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyboleDoc commands
