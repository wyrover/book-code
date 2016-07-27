// CDCStep3Doc.cpp : implementation of the CCDCStep3Doc class
//

#include "stdafx.h"
#include "CDCStep3.h"

#include "CDCStep3Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3Doc

IMPLEMENT_DYNCREATE(CCDCStep3Doc, CDocument)

BEGIN_MESSAGE_MAP(CCDCStep3Doc, CDocument)
	//{{AFX_MSG_MAP(CCDCStep3Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3Doc construction/destruction

CCDCStep3Doc::CCDCStep3Doc()
{
	// TODO: add one-time construction code here

}

CCDCStep3Doc::~CCDCStep3Doc()
{
}

BOOL CCDCStep3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
    //AfxGetMainWnd()->SetWindowText("CDC step3: with original MFC");

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCDCStep3Doc serialization

void CCDCStep3Doc::Serialize(CArchive& ar)
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
// CCDCStep3Doc diagnostics

#ifdef _DEBUG
void CCDCStep3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCDCStep3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCDCStep3Doc commands
