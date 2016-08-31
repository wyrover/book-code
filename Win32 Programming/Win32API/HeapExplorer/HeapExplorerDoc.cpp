// HeapExplorerDoc.cpp : implementation of the CHeapExplorerDoc class
//

#include "stdafx.h"
#include "HeapExplorer.h"

#include "HeapExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerDoc

IMPLEMENT_DYNCREATE(CHeapExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CHeapExplorerDoc, CDocument)
        //{{AFX_MSG_MAP(CHeapExplorerDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerDoc construction/destruction

CHeapExplorerDoc::CHeapExplorerDoc()
{
        // TODO: add one-time construction code here

}

CHeapExplorerDoc::~CHeapExplorerDoc()
{
}

BOOL CHeapExplorerDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerDoc serialization

void CHeapExplorerDoc::Serialize(CArchive& ar)
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
// CHeapExplorerDoc diagnostics

#ifdef _DEBUG
void CHeapExplorerDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CHeapExplorerDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHeapExplorerDoc commands
