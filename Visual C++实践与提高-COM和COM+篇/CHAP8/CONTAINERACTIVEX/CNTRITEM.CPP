// CntrItem.cpp : implementation of the CContainerActiveXCntrItem class
//

#include "stdafx.h"
#include "ContainerActiveX.h"

#include "ContainerActiveXDoc.h"
#include "ContainerActiveXView.h"
#include "CntrItem.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContainerActiveXCntrItem implementation

IMPLEMENT_SERIAL(CContainerActiveXCntrItem, COleDocObjectItem, 0)

CContainerActiveXCntrItem::CContainerActiveXCntrItem(CContainerActiveXDoc* pContainer)
    : COleDocObjectItem(pContainer)
{
    // TODO: add one-time construction code here
}

CContainerActiveXCntrItem::~CContainerActiveXCntrItem()
{
    // TODO: add cleanup code here
}

void CContainerActiveXCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
    ASSERT_VALID(this);
    COleDocObjectItem::OnChange(nCode, dwParam);
    // When an item is being edited (either in-place or fully open)
    //  it sends OnChange notifications for changes in the state of the
    //  item or visual appearance of its content.
    // TODO: invalidate the item by calling UpdateAllViews
    //  (with hints appropriate to your application)
    GetDocument()->UpdateAllViews(NULL);
    // for now just update ALL views/no hints
}

BOOL CContainerActiveXCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
    ASSERT_VALID(this);

    // During in-place activation CContainerActiveXCntrItem::OnChangeItemPosition
    //  is called by the server to change the position of the in-place
    //  window.  Usually, this is a result of the data in the server
    //  document changing such that the extent has changed or as a result
    //  of in-place resizing.
    //
    // The default here is to call the base class, which will call
    //  COleDocObjectItem::SetItemRects to move the item
    //  to the new position.

    if (!COleDocObjectItem::OnChangeItemPosition(rectPos))
        return FALSE;

    // TODO: update any cache you may have of the item's rectangle/extent
    return TRUE;
}


void CContainerActiveXCntrItem::OnActivate()
{
}

void CContainerActiveXCntrItem::OnDeactivateUI(BOOL bUndoable)
{
    COleDocObjectItem::OnDeactivateUI(bUndoable);
    // Hide the object if it is not an outside-in object
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);

    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CContainerActiveXCntrItem::Serialize(CArchive& ar)
{
    ASSERT_VALID(this);
    // Call base class first to read in COleDocObjectItem data.
    // Since this sets up the m_pDocument pointer returned from
    //  CContainerActiveXCntrItem::GetDocument, it is a good idea to call
    //  the base class Serialize first.
    COleDocObjectItem::Serialize(ar);

    // now store/retrieve data specific to CContainerActiveXCntrItem
    if (ar.IsStoring()) {
        // TODO: add storing code here
    } else {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CContainerActiveXCntrItem diagnostics

#ifdef _DEBUG
void CContainerActiveXCntrItem::AssertValid() const
{
    COleDocObjectItem::AssertValid();
}

void CContainerActiveXCntrItem::Dump(CDumpContext& dc) const
{
    COleDocObjectItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
