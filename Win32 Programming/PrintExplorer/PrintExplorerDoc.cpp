// PrintExplorerDoc.cpp : implementation of the CPrintExplorerDoc class
//

#include "stdafx.h"
#include "PrintExplorer.h"

#include "PrintExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerDoc

IMPLEMENT_DYNCREATE(CPrintExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CPrintExplorerDoc, CDocument)
        //{{AFX_MSG_MAP(CPrintExplorerDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerDoc construction/destruction

CPrintExplorerDoc::CPrintExplorerDoc()
{
        // TODO: add one-time construction code here

}

CPrintExplorerDoc::~CPrintExplorerDoc()
{
}

BOOL CPrintExplorerDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerDoc serialization

void CPrintExplorerDoc::Serialize(CArchive& ar)
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
// CPrintExplorerDoc diagnostics

#ifdef _DEBUG
void CPrintExplorerDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CPrintExplorerDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerDoc commands
