// CntrItem.cpp : implementation of the CContainerItem class
//

#include "stdafx.h"
#include "Contain.h"

#include "ContrDoc.h"
#include "CntrItem.h"
#include "ContrVw.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContainerItem implementation

IMPLEMENT_SERIAL(CContainerItem, COleClientItem, 0)

CContainerItem::CContainerItem(CContainerDoc* pContainer)
    : COleClientItem(pContainer)
{
    // TODO: add one-time construction code here
    m_rect.SetRect(10, 10, 50, 50);
}

CContainerItem::~CContainerItem()
{
    // TODO: add cleanup code here
}


void CContainerItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
    ASSERT_VALID(this);
    COleClientItem::OnChange(nCode, dwParam);
    // When an item is being edited (either in-place or fully open)
    //  it sends OnChange notifications for changes in the state of the
    //  item or visual appearance of its content.
    // TODO: invalidate the item by calling UpdateAllViews
    //  (with hints appropriate to your application)
    GetDocument()->UpdateAllViews(NULL);
    // for now just update ALL views/no hints
}

BOOL CContainerItem::OnChangeItemPosition(const CRect& rectPos)
{
    ASSERT_VALID(this);

    // During in-place activation CContainerItem::OnChangeItemPosition
    //  is called by the server to change the position of the in-place
    //  window.  Usually, this is a result of the data in the server
    //  document changing such that the extent has changed or as a result
    //  of in-place resizing.
    //
    // The default here is to call the base class, which will call
    //  COleClientItem::SetItemRects to move the item
    //  to the new position.

    if (!COleClientItem::OnChangeItemPosition(rectPos))
        return FALSE;

    GetDocument()->UpdateAllViews(NULL);
    m_rect = rectPos;
    // mark document as dirty
    GetDocument()->SetModifiedFlag();
    return TRUE;
}

void CContainerItem::OnGetItemPosition(CRect& rPosition)
{
    ASSERT_VALID(this);
    // return rect relative to client area of view
    rPosition = m_rect;
}

void CContainerItem::OnActivate()
{
    // allow only one inplace active item per frame
    CView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);

    if (pItem != NULL && pItem != this)
        pItem->Close();

    COleClientItem::OnActivate();
}

void CContainerItem::OnDeactivateUI(BOOL bUndoable)
{
    COleClientItem::OnDeactivateUI(bUndoable);
    // hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);

    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CContainerItem::Serialize(CArchive& ar)
{
    ASSERT_VALID(this);
    // Call base class first to read in COleClientItem data.
    // Since this sets up the m_pDocument pointer returned from
    //  CContainerItem::GetDocument, it is a good idea to call
    //  the base class Serialize first.
    COleClientItem::Serialize(ar);

    // now store/retrieve data specific to CContainerItem
    if (ar.IsStoring()) {
        // TODO: add storing code here
        ar << m_rect;
    } else {
        // TODO: add loading code here
        ar >> m_rect;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CContainerItem diagnostics

#ifdef _DEBUG
void CContainerItem::AssertValid() const
{
    COleClientItem::AssertValid();
}

void CContainerItem::Dump(CDumpContext& dc) const
{
    COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
