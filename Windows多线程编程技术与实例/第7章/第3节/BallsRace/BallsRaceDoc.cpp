// BallsRaceDoc.cpp : implementation of the CBallsRaceDoc class
//

#include "stdafx.h"
#include "BallsRace.h"

#include "BallsRaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceDoc

IMPLEMENT_DYNCREATE(CBallsRaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CBallsRaceDoc, CDocument)
	//{{AFX_MSG_MAP(CBallsRaceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceDoc construction/destruction

CBallsRaceDoc::CBallsRaceDoc()
{
	// TODO: add one-time construction code here

}

CBallsRaceDoc::~CBallsRaceDoc()
{
}

BOOL CBallsRaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBallsRaceDoc serialization

void CBallsRaceDoc::Serialize(CArchive& ar)
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
// CBallsRaceDoc diagnostics

#ifdef _DEBUG
void CBallsRaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBallsRaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceDoc commands
