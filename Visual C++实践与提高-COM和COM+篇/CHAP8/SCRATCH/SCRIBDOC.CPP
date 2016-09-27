// ScribDoc.cpp : implementation of the CScribbleDoc class
//

#include "stdafx.h"
#include "Scribble.h"

#include "ScribDoc.h"
#include "ScribItm.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScribbleDoc

IMPLEMENT_DYNCREATE(CScribbleDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CScribbleDoc, COleServerDoc)
    //{{AFX_MSG_MAP(CScribbleDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScribbleDoc construction/destruction

CScribbleDoc::CScribbleDoc()
{
    // Use OLE compound files
    EnableCompoundFile();
    // TODO: add one-time construction code here
}

CScribbleDoc::~CScribbleDoc()
{
}

BOOL CScribbleDoc::OnNewDocument()
{
    if (!COleServerDoc::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CScribbleDoc server implementation

COleServerItem* CScribbleDoc::OnGetEmbeddedItem()
{
    // OnGetEmbeddedItem is called by the framework to get the COleServerItem
    //  that is associated with the document.  It is only called when necessary.
    CScribbleItem* pItem = new CScribbleItem(this);
    ASSERT_VALID(pItem);
    return pItem;
}



/////////////////////////////////////////////////////////////////////////////
// CScribbleDoc serialization

void CScribbleDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CScribbleDoc diagnostics

#ifdef _DEBUG
void CScribbleDoc::AssertValid() const
{
    COleServerDoc::AssertValid();
}

void CScribbleDoc::Dump(CDumpContext& dc) const
{
    COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScribbleDoc commands
