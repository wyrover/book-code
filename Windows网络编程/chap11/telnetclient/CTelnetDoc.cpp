// CTelnetDoc.cpp : implementation of the CTelnetDoc class
//

#include "stdafx.h"
#include "CTelnet.h"

#include "CTelnetDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelnetDoc

IMPLEMENT_DYNCREATE(CTelnetDoc, CDocument)

BEGIN_MESSAGE_MAP(CTelnetDoc, CDocument)
	//{{AFX_MSG_MAP(CTelnetDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelnetDoc construction/destruction

CTelnetDoc::CTelnetDoc()
{
	// TODO: add one-time construction code here

}

CTelnetDoc::~CTelnetDoc()
{
}

BOOL CTelnetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTelnetDoc serialization

void CTelnetDoc::Serialize(CArchive& ar)
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
// CTelnetDoc diagnostics

#ifdef _DEBUG
void CTelnetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTelnetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTelnetDoc commands
