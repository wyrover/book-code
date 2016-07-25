// MailslotSrvDoc.cpp : implementation of the CMailslotSrvDoc class
//

#include "stdafx.h"
#include "MailslotSrv.h"

#include "MailslotSrvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvDoc

IMPLEMENT_DYNCREATE(CMailslotSrvDoc, CDocument)

BEGIN_MESSAGE_MAP(CMailslotSrvDoc, CDocument)
	//{{AFX_MSG_MAP(CMailslotSrvDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvDoc construction/destruction

CMailslotSrvDoc::CMailslotSrvDoc()
{
	// TODO: add one-time construction code here

}

CMailslotSrvDoc::~CMailslotSrvDoc()
{
}

BOOL CMailslotSrvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvDoc serialization

void CMailslotSrvDoc::Serialize(CArchive& ar)
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
// CMailslotSrvDoc diagnostics

#ifdef _DEBUG
void CMailslotSrvDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMailslotSrvDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMailslotSrvDoc commands
