// FontExplorerDoc.cpp : implementation of the CFontExplorerDoc class
//

#include "stdafx.h"
#include "FontExplorer.h"

#include "FontExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerDoc

IMPLEMENT_DYNCREATE(CFontExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFontExplorerDoc, CDocument)
        //{{AFX_MSG_MAP(CFontExplorerDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerDoc construction/destruction

CFontExplorerDoc::CFontExplorerDoc()
{
        // TODO: add one-time construction code here

}

CFontExplorerDoc::~CFontExplorerDoc()
{
}

BOOL CFontExplorerDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerDoc serialization

void CFontExplorerDoc::Serialize(CArchive& ar)
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
// CFontExplorerDoc diagnostics

#ifdef _DEBUG
void CFontExplorerDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CFontExplorerDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerDoc commands
