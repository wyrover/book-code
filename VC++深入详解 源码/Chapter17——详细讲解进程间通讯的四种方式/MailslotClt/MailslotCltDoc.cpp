// MailslotCltDoc.cpp : implementation of the CMailslotCltDoc class
//

#include "stdafx.h"
#include "MailslotClt.h"

#include "MailslotCltDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltDoc

IMPLEMENT_DYNCREATE(CMailslotCltDoc, CDocument)

BEGIN_MESSAGE_MAP(CMailslotCltDoc, CDocument)
	//{{AFX_MSG_MAP(CMailslotCltDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltDoc construction/destruction

CMailslotCltDoc::CMailslotCltDoc()
{
	// TODO: add one-time construction code here

}

CMailslotCltDoc::~CMailslotCltDoc()
{
}

BOOL CMailslotCltDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMailslotCltDoc serialization

void CMailslotCltDoc::Serialize(CArchive& ar)
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
// CMailslotCltDoc diagnostics

#ifdef _DEBUG
void CMailslotCltDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMailslotCltDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMailslotCltDoc commands
