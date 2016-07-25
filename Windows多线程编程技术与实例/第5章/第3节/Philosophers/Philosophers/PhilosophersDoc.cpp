// PhilosophersDoc.cpp : implementation of the CPhilosophersDoc class
//

#include "stdafx.h"
#include "Philosophers.h"

#include "PhilosophersDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersDoc

IMPLEMENT_DYNCREATE(CPhilosophersDoc, CDocument)

BEGIN_MESSAGE_MAP(CPhilosophersDoc, CDocument)
	//{{AFX_MSG_MAP(CPhilosophersDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersDoc construction/destruction

CPhilosophersDoc::CPhilosophersDoc()
{
	// TODO: add one-time construction code here

}

CPhilosophersDoc::~CPhilosophersDoc()
{
}

BOOL CPhilosophersDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPhilosophersDoc serialization

void CPhilosophersDoc::Serialize(CArchive& ar)
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
// CPhilosophersDoc diagnostics

#ifdef _DEBUG
void CPhilosophersDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhilosophersDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhilosophersDoc commands
