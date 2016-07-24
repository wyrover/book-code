// test_audio_loopDoc.cpp : implementation of the CTest_audio_loopDoc class
//

#include "stdafx.h"
#include "test_audio_loop.h"

#include "test_audio_loopDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopDoc

IMPLEMENT_DYNCREATE(CTest_audio_loopDoc, CDocument)

BEGIN_MESSAGE_MAP(CTest_audio_loopDoc, CDocument)
	//{{AFX_MSG_MAP(CTest_audio_loopDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopDoc construction/destruction

CTest_audio_loopDoc::CTest_audio_loopDoc()
{
	// TODO: add one-time construction code here

}

CTest_audio_loopDoc::~CTest_audio_loopDoc()
{
}

BOOL CTest_audio_loopDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopDoc serialization

void CTest_audio_loopDoc::Serialize(CArchive& ar)
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
// CTest_audio_loopDoc diagnostics

#ifdef _DEBUG
void CTest_audio_loopDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTest_audio_loopDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTest_audio_loopDoc commands
