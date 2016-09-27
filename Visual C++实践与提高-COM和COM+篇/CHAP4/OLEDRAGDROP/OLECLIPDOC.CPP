// OleClipDoc.cpp : implementation of the COleClipDoc class
//

#include "stdafx.h"
#include "OleClip.h"

#include "cdib.h"

#include "OleClipDoc.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleClipDoc

IMPLEMENT_DYNCREATE(COleClipDoc, CDocument)

BEGIN_MESSAGE_MAP(COleClipDoc, CDocument)
    //{{AFX_MSG_MAP(COleClipDoc)
    ON_COMMAND(ID_EDIT_CLEAR_ALL, OnEditClearAll)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleClipDoc construction/destruction

COleClipDoc::COleClipDoc()
{
    m_bDragHere = FALSE;
}

COleClipDoc::~COleClipDoc()
{
}

BOOL COleClipDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: add reinitialization code here
    // (SDI documents will reuse this document)
    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COleClipDoc serialization

void COleClipDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// COleClipDoc diagnostics

#ifdef _DEBUG
void COleClipDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void COleClipDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COleClipDoc commands

void COleClipDoc::OnEditClearAll()
{
    DeleteContents();
    UpdateAllViews(NULL);
    SetModifiedFlag();
}

void COleClipDoc::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_dib.GetSizeImage() > 0);
}

void COleClipDoc::DeleteContents()
{
    m_dib.Empty();
}
