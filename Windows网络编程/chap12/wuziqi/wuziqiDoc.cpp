// wuziqiDoc.cpp : implementation of the CWuziqiDoc class
//

#include "stdafx.h"
#include "wuziqi.h"

#include "wuziqiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWuziqiDoc

IMPLEMENT_DYNCREATE(CWuziqiDoc, CDocument)

BEGIN_MESSAGE_MAP(CWuziqiDoc, CDocument)
	//{{AFX_MSG_MAP(CWuziqiDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWuziqiDoc construction/destruction

CWuziqiDoc::CWuziqiDoc()
{
	// TODO: add one-time construction code here

}

CWuziqiDoc::~CWuziqiDoc()
{
}

BOOL CWuziqiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWuziqiDoc serialization

void CWuziqiDoc::Serialize(CArchive& ar)
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
// CWuziqiDoc diagnostics

#ifdef _DEBUG
void CWuziqiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWuziqiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWuziqiDoc commands
