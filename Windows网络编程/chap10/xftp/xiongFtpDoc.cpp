// xiongFtpDoc.cpp : implementation of the CXiongFtpDoc class
//

#include "stdafx.h"
#include "xiongFtp.h"

#include "xiongFtpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpDoc

IMPLEMENT_DYNCREATE(CXiongFtpDoc, CDocument)

BEGIN_MESSAGE_MAP(CXiongFtpDoc, CDocument)
	//{{AFX_MSG_MAP(CXiongFtpDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpDoc construction/destruction

CXiongFtpDoc::CXiongFtpDoc()
{
	// TODO: add one-time construction code here

}

CXiongFtpDoc::~CXiongFtpDoc()
{
}

BOOL CXiongFtpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CXiongFtpDoc serialization

void CXiongFtpDoc::Serialize(CArchive& ar)
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
// CXiongFtpDoc diagnostics

#ifdef _DEBUG
void CXiongFtpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXiongFtpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXiongFtpDoc commands
