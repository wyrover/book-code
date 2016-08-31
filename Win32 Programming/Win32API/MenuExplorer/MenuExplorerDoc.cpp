// MenuExplorerDoc.cpp : implementation of the CMenuExplorerDoc class
//

#include "stdafx.h"
#include "MenuExplorer.h"

#include "MenuExplorerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDoc

IMPLEMENT_DYNCREATE(CMenuExplorerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMenuExplorerDoc, CDocument)
        //{{AFX_MSG_MAP(CMenuExplorerDoc)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDoc construction/destruction

CMenuExplorerDoc::CMenuExplorerDoc()
{
        // TODO: add one-time construction code here

}

CMenuExplorerDoc::~CMenuExplorerDoc()
{
}

BOOL CMenuExplorerDoc::OnNewDocument()
{
        if (!CDocument::OnNewDocument())
                return FALSE;

        // TODO: add reinitialization code here
        // (SDI documents will reuse this document)

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDoc serialization

void CMenuExplorerDoc::Serialize(CArchive& ar)
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
// CMenuExplorerDoc diagnostics

#ifdef _DEBUG
void CMenuExplorerDoc::AssertValid() const
{
        CDocument::AssertValid();
}

void CMenuExplorerDoc::Dump(CDumpContext& dc) const
{
        CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerDoc commands
