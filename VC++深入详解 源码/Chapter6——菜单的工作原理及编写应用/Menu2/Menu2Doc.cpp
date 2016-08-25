// Menu2Doc.cpp : implementation of the CMenu2Doc class
//

#include "stdafx.h"
#include "Menu2.h"

#include "Menu2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenu2Doc

IMPLEMENT_DYNCREATE(CMenu2Doc, CDocument)

BEGIN_MESSAGE_MAP(CMenu2Doc, CDocument)
	//{{AFX_MSG_MAP(CMenu2Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenu2Doc construction/destruction

CMenu2Doc::CMenu2Doc()
{
	// TODO: add one-time construction code here

}

CMenu2Doc::~CMenu2Doc()
{
}

BOOL CMenu2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMenu2Doc serialization

void CMenu2Doc::Serialize(CArchive& ar)
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
// CMenu2Doc diagnostics

#ifdef _DEBUG
void CMenu2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMenu2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenu2Doc commands
