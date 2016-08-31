// CtlColorExplorerDoc.cpp : implementation of the CCtlColorExplorerDoc class
//

#include "stdafx.h"
#include "CtlColorExplorer.h"

#include "CtlColorExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerDoc

IMPLEMENT_DYNCREATE(CCtlColorExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCtlColorExplorerDoc, CDocument)
        //{{AFX_MSG_MAP(CCtlColorExplorerDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerDoc construction/destruction

CCtlColorExplorerDoc::CCtlColorExplorerDoc()
{
        // TODO: add one-time construction code here

}

CCtlColorExplorerDoc::~CCtlColorExplorerDoc()
{
}

BOOL CCtlColorExplorerDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerDoc serialization

void CCtlColorExplorerDoc::Serialize(CArchive& ar)
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
// CCtlColorExplorerDoc diagnostics

#ifdef _DEBUG
void CCtlColorExplorerDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CCtlColorExplorerDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerDoc commands
