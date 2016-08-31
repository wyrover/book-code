// GDI ExplorerDoc.cpp : implementation of the CGDIExplorerDoc class
//

#include "stdafx.h"
#include "GDIExplorer.h"

#include "GDIExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerDoc

IMPLEMENT_DYNCREATE(CGDIExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGDIExplorerDoc, CDocument)
        //{{AFX_MSG_MAP(CGDIExplorerDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerDoc construction/destruction

CGDIExplorerDoc::CGDIExplorerDoc()
{
        // TODO: add one-time construction code here

}

CGDIExplorerDoc::~CGDIExplorerDoc()
{
}

BOOL CGDIExplorerDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerDoc serialization

void CGDIExplorerDoc::Serialize(CArchive& ar)
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
// CGDIExplorerDoc diagnostics

#ifdef _DEBUG
void CGDIExplorerDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CGDIExplorerDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerDoc commands
