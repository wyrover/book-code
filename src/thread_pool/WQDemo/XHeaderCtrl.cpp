// XHeaderCtrl.cpp
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This code is based on "Outlook 98-Style FlatHeader Control"
// by Maarten Hoeben.
//
// See http://www.codeguru.com/listview/FlatHeader.shtml
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XHeaderCtrl.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl

IMPLEMENT_DYNCREATE(CXHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CXHeaderCtrl, CHeaderCtrl)
    //{{AFX_MSG_MAP(CXHeaderCtrl)
    ON_MESSAGE(HDM_INSERTITEMA, OnInsertItem)
    ON_MESSAGE(HDM_INSERTITEMW, OnInsertItem)
    ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
    ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
    ON_MESSAGE(HDM_LAYOUT, OnLayout)
    ON_WM_PAINT()
    ON_WM_SYSCOLORCHANGE()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXHeaderCtrl::CXHeaderCtrl()
{
    m_bDoubleBuffer = TRUE;
    m_iSpacing = 6;
    m_sizeArrow.cx = 8;
    m_sizeArrow.cy = 8;
    m_sizeImage.cx = 0;
    m_sizeImage.cy = 0;
    m_bStaticBorder = FALSE;
    m_nDontDropCursor = 0;
    m_bResizing = FALSE;
    m_nClickFlags = 0;
    m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
    m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
    m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
    m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXHeaderCtrl::~CXHeaderCtrl()
{
}

///////////////////////////////////////////////////////////////////////////////
// ModifyProperty
BOOL CXHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
{
    switch (wParam) {
    case FH_PROPERTY_SPACING:
        m_iSpacing = (int)lParam;
        break;

    case FH_PROPERTY_ARROW:
        m_sizeArrow.cx = LOWORD(lParam);
        m_sizeArrow.cy = HIWORD(lParam);
        break;

    case FH_PROPERTY_STATICBORDER:
        m_bStaticBorder = (BOOL)lParam;
        break;

    case FH_PROPERTY_DONTDROPCURSOR:
        m_nDontDropCursor = (UINT)lParam;
        break;

    default:
        return FALSE;
    }

    Invalidate();
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DrawCtrl
void CXHeaderCtrl::DrawCtrl(CDC* pDC)
{
    CRect rectClip;

    if (pDC->GetClipBox(&rectClip) == ERROR)
        return;

    CRect rectClient, rectItem;
    GetClientRect(&rectClient);
    pDC->FillSolidRect(rectClip, m_cr3DFace);
    int iItems = GetItemCount();
    ASSERT(iItems >= 0);
    CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
    CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
    CPen* pPen = pDC->GetCurrentPen();
    CFont* pFont = pDC->SelectObject(GetFont());
    pDC->SetBkColor(m_cr3DFace);
    pDC->SetTextColor(m_crBtnText);
    int iWidth = 0;

    for (int i = 0; i < iItems; i++) {
        int iItem = OrderToIndex(i);
        TCHAR szText[FLATHEADER_TEXT_MAX];
        HDITEM hditem;
        hditem.mask = HDI_WIDTH | HDI_FORMAT | HDI_TEXT | HDI_IMAGE | HDI_BITMAP;
        hditem.pszText = szText;
        hditem.cchTextMax = sizeof(szText);
        VERIFY(GetItem(iItem, &hditem));
        VERIFY(GetItemRect(iItem, rectItem));

        if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right) {
            if (hditem.fmt & HDF_OWNERDRAW) {
                DRAWITEMSTRUCT disItem;
                disItem.CtlType = ODT_BUTTON;
                disItem.CtlID = GetDlgCtrlID();
                disItem.itemID = iItem;
                disItem.itemAction = ODA_DRAWENTIRE;
                disItem.itemState = 0;
                disItem.hwndItem = m_hWnd;
                disItem.hDC = pDC->m_hDC;
                disItem.rcItem = rectItem;
                disItem.itemData = 0;
                DrawItem(&disItem);
            } else {
                rectItem.DeflateRect(m_iSpacing, 0);
                DrawItem(pDC, rectItem, &hditem);
                rectItem.InflateRect(m_iSpacing, 0);
                //if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
                //  pDC->InvertRect(rectItem);
            }

            if (i < iItems - 1) {
                pDC->SelectObject(&penShadow);
                pDC->MoveTo(rectItem.right - 1, rectItem.top + 2);
                pDC->LineTo(rectItem.right - 1, rectItem.bottom - 2);
                pDC->SelectObject(&penHighLight);
                pDC->MoveTo(rectItem.right, rectItem.top + 2);
                pDC->LineTo(rectItem.right, rectItem.bottom - 2);
            }
        }

        iWidth += hditem.cxy;
    }

    if (iWidth > 0) {
        rectClient.right = rectClient.left + iWidth;
        pDC->Draw3dRect(rectClient, m_cr3DHighLight, m_cr3DShadow);
    }

    pDC->SelectObject(pFont);
    pDC->SelectObject(pPen);
    penHighLight.DeleteObject();
    penShadow.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
    ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
    ASSERT(lphdi->mask & HDI_FORMAT);
    int iWidth = 0;
    CBitmap* pBitmap = NULL;
    BITMAP BitmapInfo;

    if (lphdi->fmt & HDF_BITMAP) {
        ASSERT(lphdi->mask & HDI_BITMAP);
        ASSERT(lphdi->hbm);
        pBitmap = CBitmap::FromHandle(lphdi->hbm);

        if (pBitmap)
            VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
    }

    rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? iWidth + m_iSpacing : 0;
    rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ?
                  iWidth + m_iSpacing : 0;
    DrawText(pDC, rect, lphdi);
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CXHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
    CImageList* pImageList = GetImageList();
    int iWidth = 0;

    if (lphdi->iImage != XHEADERCTRL_NO_IMAGE) {
        if (pImageList) {
            if (rect.Width() > 0) {
                POINT point;
                point.y = rect.CenterPoint().y - (m_sizeImage.cy >> 1);

                if (bRight)
                    point.x = rect.right - m_sizeImage.cx;
                else
                    point.x = rect.left;

                SIZE size;
                size.cx = rect.Width() < m_sizeImage.cx ? rect.Width() : m_sizeImage.cx;
                size.cy = m_sizeImage.cy;
                // save image list background color
                COLORREF rgb = pImageList->GetBkColor();
                // set image list background color to same as header control
                pImageList->SetBkColor(pDC->GetBkColor());
                pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
                pImageList->SetBkColor(rgb);
                iWidth = m_sizeImage.cx;
            }
        }
    }

    return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CXHeaderCtrl::DrawBitmap(CDC* pDC,
                             CRect rect,
                             LPHDITEM lphdi,
                             CBitmap* pBitmap,
                             BITMAP* pBitmapInfo,
                             BOOL bRight)
{
    UNUSED_ALWAYS(lphdi);
    int iWidth = 0;

    if (pBitmap) {
        iWidth = pBitmapInfo->bmWidth;

        if (iWidth <= rect.Width() && rect.Width() > 0) {
            POINT point;
            point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight >> 1);

            if (bRight)
                point.x = rect.right - iWidth;
            else
                point.x = rect.left;

            CDC dc;

            if (dc.CreateCompatibleDC(pDC) == TRUE) {
                VERIFY(dc.SelectObject(pBitmap));
                iWidth = pDC->BitBlt(
                             point.x, point.y,
                             pBitmapInfo->bmWidth, pBitmapInfo->bmHeight,
                             &dc,
                             0, 0,
                             SRCCOPY
                         ) ? iWidth : 0;
            } else
                iWidth = 0;
        } else
            iWidth = 0;
    }

    return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CXHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
    CSize size;
    pDC->SetTextColor(RGB(0, 0, 255));

    if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING) {
        size = pDC->GetTextExtent(lphdi->pszText);
        // always center column headers
        pDC->DrawText(lphdi->pszText, -1, rect,
                      DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER);
    }

    size.cx = rect.Width() > size.cx ? size.cx : rect.Width();
    return size.cx > 0 ? size.cx : 0;
}

///////////////////////////////////////////////////////////////////////////////
// OnInsertItem
LRESULT CXHeaderCtrl::OnInsertItem(WPARAM, LPARAM)
{
    return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnDeleteItem
LRESULT CXHeaderCtrl::OnDeleteItem(WPARAM, LPARAM)
{
    return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnSetImageList
LRESULT CXHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
{
    CImageList* pImageList;
    pImageList = CImageList::FromHandle((HIMAGELIST)lParam);
    IMAGEINFO info;

    if (pImageList->GetImageInfo(0, &info)) {
        m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
        m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
    }

    return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnLayout
LRESULT CXHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
    LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

    if (m_bStaticBorder)
        lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER) * 2;

    return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CXHeaderCtrl::OnSysColorChange()
{
    TRACE(_T("in CXHeaderCtrl::OnSysColorChange\n"));
    CHeaderCtrl::OnSysColorChange();
    m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
    m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
    m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
    m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CXHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
    UNUSED_ALWAYS(pDC);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXHeaderCtrl::OnPaint()
{
    CPaintDC dc(this);

    if (m_bDoubleBuffer) {
        CMemDC MemDC(&dc);
        DrawCtrl(&MemDC);
    } else
        DrawCtrl(&dc);
}
