// PropDoc.cpp : implementation of the CPropDoc class
//

#include "stdafx.h"
#include "Prop.h"

#include "PropDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropDoc

IMPLEMENT_DYNCREATE(CPropDoc, CDocument)

BEGIN_MESSAGE_MAP(CPropDoc, CDocument)
	//{{AFX_MSG_MAP(CPropDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDoc construction/destruction

CPropDoc::CPropDoc()
{
	// TODO: add one-time construction code here

}

CPropDoc::~CPropDoc()
{
}

BOOL CPropDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPropDoc serialization

void CPropDoc::Serialize(CArchive& ar)
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
// CPropDoc diagnostics

#ifdef _DEBUG
void CPropDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPropDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPropDoc commands
