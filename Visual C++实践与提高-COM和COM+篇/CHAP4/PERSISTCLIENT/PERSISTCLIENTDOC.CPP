// PersistClientDoc.cpp : implementation of the CPersistClientDoc class
//

#include "stdafx.h"
#include "PersistClient.h"

#include "PersistClientDoc.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersistClientDoc

IMPLEMENT_DYNCREATE(CPersistClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CPersistClientDoc, CDocument)
    //{{AFX_MSG_MAP(CPersistClientDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersistClientDoc construction/destruction

CPersistClientDoc::CPersistClientDoc()
{
    // TODO: add one-time construction code here
}

CPersistClientDoc::~CPersistClientDoc()
{
}

BOOL CPersistClientDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPersistClientDoc serialization

void CPersistClientDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CPersistClientDoc diagnostics

#ifdef _DEBUG
void CPersistClientDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CPersistClientDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPersistClientDoc commands
