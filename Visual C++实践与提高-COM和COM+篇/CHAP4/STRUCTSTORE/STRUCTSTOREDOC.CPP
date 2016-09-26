// StructStoreDoc.cpp : implementation of the CStructStoreDoc class
//

#include "stdafx.h"
#include "StructStore.h"

#include "StructStoreDoc.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructStoreDoc

IMPLEMENT_DYNCREATE(CStructStoreDoc, CDocument)

BEGIN_MESSAGE_MAP(CStructStoreDoc, CDocument)
    //{{AFX_MSG_MAP(CStructStoreDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructStoreDoc construction/destruction

CStructStoreDoc::CStructStoreDoc()
{
    // TODO: add one-time construction code here
}

CStructStoreDoc::~CStructStoreDoc()
{
}

BOOL CStructStoreDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CStructStoreDoc serialization

void CStructStoreDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CStructStoreDoc diagnostics

#ifdef _DEBUG
void CStructStoreDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CStructStoreDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructStoreDoc commands
