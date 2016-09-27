// ScribItm.cpp : implementation of the CScribbleItem class
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
// CScribbleItem implementation

IMPLEMENT_DYNAMIC(CScribbleItem, COleServerItem)

CScribbleItem::CScribbleItem(CScribbleDoc* pContainerDoc)
    : COleServerItem(pContainerDoc, TRUE)
{
    // TODO: add one-time construction code here
    //  (eg, adding additional clipboard formats to the item's data source)
}

CScribbleItem::~CScribbleItem()
{
    // TODO: add cleanup code here
}

void CScribbleItem::Serialize(CArchive& ar)
{
    // CScribbleItem::Serialize will be called by the framework if
    //  the item is copied to the clipboard.  This can happen automatically
    //  through the OLE callback OnGetClipboardData.  A good default for
    //  the embedded item is simply to delegate to the document's Serialize
    //  function.  If you support links, then you will want to serialize
    //  just a portion of the document.
    if (!IsLinkedItem()) {
        CScribbleDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        pDoc->Serialize(ar);
    }
}

BOOL CScribbleItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
    // Most applications, like this one, only handle drawing the content
    //  aspect of the item.  If you wish to support other aspects, such
    //  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
    //  implementation of OnGetExtent should be modified to handle the
    //  additional aspect(s).
    if (dwDrawAspect != DVASPECT_CONTENT)
        return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

    // CScribbleItem::OnGetExtent is called to get the extent in
    //  HIMETRIC units of the entire item.  The default implementation
    //  here simply returns a hard-coded number of units.
    CScribbleDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: replace this arbitrary size
    rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC units
    return TRUE;
}

BOOL CScribbleItem::OnDraw(CDC* pDC, CSize& rSize)
{
    // Remove this if you use rSize
    UNREFERENCED_PARAMETER(rSize);
    CScribbleDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    // TODO: set mapping mode and extent
    //  (The extent is usually the same as the size returned from OnGetExtent)
    pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowOrg(0, 0);
    pDC->SetWindowExt(3000, 3000);
    // TODO: add drawing code here.  Optionally, fill in the HIMETRIC extent.
    //  All drawing takes place in the metafile device context (pDC).
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CScribbleItem diagnostics

#ifdef _DEBUG
void CScribbleItem::AssertValid() const
{
    COleServerItem::AssertValid();
}

void CScribbleItem::Dump(CDumpContext& dc) const
{
    COleServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
