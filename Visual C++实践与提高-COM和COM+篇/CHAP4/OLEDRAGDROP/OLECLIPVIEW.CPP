// OleClipView.cpp : implementation of the COleClipView class
//

#include "stdafx.h"
#include "OleClip.h"

#include "cdib.h"

#include "OleClipDoc.h"
#include "OleClipView.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleClipView

IMPLEMENT_DYNCREATE(COleClipView, CView)

BEGIN_MESSAGE_MAP(COleClipView, CView)
    //{{AFX_MSG_MAP(COleClipView)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_WM_LBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_MESSAGE(WM_VIEWPALETTECHANGED, OnViewPaletteChanged)
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_EDIT_PASTEFROM, OnEditPastefrom)
    ON_COMMAND(ID_EDIT_COPYTO, OnEditCopyto)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPYTO, OnUpdateEditCopy)
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleClipView construction/destruction

COleClipView::COleClipView() :
    m_rectTracker(50, 50, 250, 250),
    m_dragOffset(0, 0),
    m_rectTrackerEnter(50, 50, 250, 250)
{
    // TODO: add construction code here
}

COleClipView::~COleClipView()
{
}

BOOL COleClipView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COleClipView drawing

void COleClipView::OnDraw(CDC* pDC)
{
    CDib& dib = GetDocument()->m_dib;
    m_tracker.m_rect = m_rectTracker;
    pDC->LPtoDP(m_tracker.m_rect); // tracker wants device coordinates
    m_tracker.Draw(pDC);
    dib.Draw(pDC, m_rectTracker.TopLeft(), m_rectTracker.Size());
}

/////////////////////////////////////////////////////////////////////////////
// COleClipView printing

BOOL COleClipView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void COleClipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void COleClipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COleClipView diagnostics

#ifdef _DEBUG
void COleClipView::AssertValid() const
{
    CView::AssertValid();
}

void COleClipView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

COleClipDoc* COleClipView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COleClipDoc)));
    return (COleClipDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COleClipView message handlers

void COleClipView::OnEditCut()
{
    OnEditCopy();
    GetDocument()->OnEditClearAll();
}

void COleClipView::OnEditCopy()
{
    COleDataSource* pSource = SaveDib();

    if (pSource) {
        pSource->SetClipboard(); // OLE deletes data source
    }
}

void COleClipView::OnEditPaste()
{
    COleClipDoc* pDoc = GetDocument();
    COleDataObject dataObject;
    VERIFY(dataObject.AttachClipboard());
    DoPasteDib(&dataObject);
    CClientDC dc(this);
    pDoc->m_dib.UsePalette(&dc);
    pDoc->SetModifiedFlag();
    pDoc->UpdateAllViews(NULL);
}

void COleClipView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    CDib& dib = GetDocument()->m_dib;
    pCmdUI->Enable(dib.GetSizeImage() > 0L);
}

void COleClipView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    COleDataObject dataObject;
    BOOL bAvail = dataObject.AttachClipboard() &&
                  dataObject.IsDataAvailable(CF_DIB);
    pCmdUI->Enable(bAvail);
}

BOOL COleClipView::DoPasteDib(COleDataObject *pDataObject)
{
    // update command user interface should keep us out of
    //  here if not CF_DIB
    if (!pDataObject->IsDataAvailable(CF_DIB)) {
        TRACE("CF_DIB format is unavailable\n");
        return FALSE;
    }

    COleClipDoc* pDoc = GetDocument();
    // Seems to be MOVEABLE memory, so we must use GlobalLock!
    //  (hDib != lpDib) GetGlobalData copies the memory, so we can
    //  hang onto it until we delete the CDib.
    HGLOBAL hDib = pDataObject->GetGlobalData(CF_DIB);
    ASSERT(hDib != NULL);
    LPVOID lpDib = ::GlobalLock(hDib);
    ASSERT(lpDib != NULL);
    pDoc->m_dib.AttachMemory(lpDib, TRUE, hDib);
    pDoc->SetModifiedFlag();
    pDoc->UpdateAllViews(NULL);
    return TRUE;
}

COleDataSource* COleClipView::SaveDib()
{
    CDib& dib = GetDocument()->m_dib;

    if (dib.GetSizeImage() > 0) {
        COleDataSource* pSource = new COleDataSource();
        int nHeaderSize = dib.GetSizeHeader();
        int nImageSize = dib.GetSizeImage();
        HGLOBAL hHeader = ::GlobalAlloc(GMEM_SHARE,
                                        nHeaderSize + nImageSize);
        LPVOID pHeader = ::GlobalLock(hHeader);
        ASSERT(pHeader != NULL);
        LPVOID pImage = (LPBYTE) pHeader + nHeaderSize;
        memcpy(pHeader, dib.m_lpBMIH, nHeaderSize);
        memcpy(pImage, dib.m_lpImage, nImageSize);
        // Receiver is supposed to free the global memory
        ::GlobalUnlock(hHeader);
        pSource->CacheGlobalData(CF_DIB, hHeader);
        return pSource;
    }

    return NULL;
}

void COleClipView::OnLButtonDown(UINT nFlags, CPoint point)
{
    COleClipDoc* pDoc = GetDocument();

    if (m_tracker.HitTest(point) == CRectTracker::hitMiddle) {
        COleDataSource* pSource = SaveDib();

        if (pSource) {
            // DoDragDrop returns only after drop is complete
            CClientDC dc(this);
            OnPrepareDC(&dc);
            CPoint topleft = m_rectTracker.TopLeft();
            dc.LPtoDP(&topleft);
            // 'point' here is not the same as the point parameter in
            //  OnDragEnter, so we use this one to compute the offset
            m_dragOffset = point - topleft;  // device coordinates
            pDoc->m_bDragHere = TRUE;
            DROPEFFECT dropEffect = pSource->DoDragDrop(
                                        DROPEFFECT_MOVE | DROPEFFECT_COPY, CRect(0, 0, 0, 0));
            TRACE("after DoDragDrop -- dropEffect = %ld\n", dropEffect);

            if (dropEffect == DROPEFFECT_MOVE && pDoc->m_bDragHere)
                pDoc->OnEditClearAll();

            pDoc->m_bDragHere = FALSE;
            delete pSource;
        }
    } else {
        if (m_tracker.Track(this, point, FALSE, NULL)) {
            CClientDC dc(this);
            OnPrepareDC(&dc);
            // should have some way to prevent it going out of bounds
            m_rectTracker = m_tracker.m_rect;
            dc.DPtoLP(m_rectTracker); // Update logical coords
        }
    }

    Invalidate();
}

BOOL COleClipView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_tracker.SetCursor(pWnd, nHitTest)) {
        return TRUE;
    } else {
        return CView::OnSetCursor(pWnd, nHitTest, message);
    }
}
LONG COleClipView::OnViewPaletteChanged(UINT wParam, LONG lParam)
{
    TRACE("COleClipView::OnViewPaletteChanged, HWND = %x, code = %d\n", GetSafeHwnd(), wParam);
    CClientDC dc(this);
    GetDocument()->m_dib.UsePalette(&dc, wParam);
    Invalidate();
    return 0;
}

void COleClipView::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);
    AfxGetApp()->m_pMainWnd->SendMessage(WM_PALETTECHANGED,
                                         (UINT) GetSafeHwnd());
}

void COleClipView::OnInitialUpdate()
{
    m_dropTarget.Register(this); // IMPORTANT
    m_tracker.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeOutside;
    CView::OnInitialUpdate();
}

void COleClipView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
// custom MM_LOENGLISH; positive y is down
    if (pDC->IsPrinting()) {
        int nHsize = pDC->GetDeviceCaps(HORZSIZE) * 1000 / 254;
        int nVsize = pDC->GetDeviceCaps(VERTSIZE) * 1000 / 254;
        pDC->SetMapMode(MM_ANISOTROPIC);
        pDC->SetWindowExt(nHsize, nVsize);
        pDC->SetViewportExt(pDC->GetDeviceCaps(HORZRES),
                            pDC->GetDeviceCaps(VERTRES));
    } else {
        CView::OnPrepareDC(pDC, pInfo);
    }
}

DROPEFFECT COleClipView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    TRACE("Entering COleClipView::OnDragEnter, point = (%d, %d)\n",
          point.x, point.y);
    m_rectTrackerEnter = m_rectTracker; // Save original coordinates
    //  for cursor leaving
    //  rectangle
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DrawFocusRect(m_rectTracker); // will be erased in OnDragOver
    return OnDragOver(pDataObject, dwKeyState, point);
}

DROPEFFECT COleClipView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
    if (!pDataObject->IsDataAvailable(CF_DIB)) {
        return DROPEFFECT_NONE;
    }

    MoveTrackRect(point);

    if ((dwKeyState & MK_CONTROL) == MK_CONTROL) {
        return DROPEFFECT_COPY;
    }

    // Check for force move
    if ((dwKeyState & MK_ALT) == MK_ALT) {
        return DROPEFFECT_MOVE;
    }

    // default -- recommended action is move
    return DROPEFFECT_MOVE;
}

void COleClipView::OnDragLeave()
{
    TRACE("Entering COleClipView::OnDragLeave\n");
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DrawFocusRect(m_rectTracker);
    m_rectTracker = m_rectTrackerEnter; // Forget it ever happened
}

BOOL COleClipView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    TRACE("Entering COleClipView::OnDrop -- dropEffect = %d\n", dropEffect);
    BOOL bRet;
    COleClipDoc* pDoc = GetDocument();
    MoveTrackRect(point);

    if (pDoc->m_bDragHere) {
        pDoc->m_bDragHere = FALSE;
        bRet = TRUE;
    } else {
        bRet = DoPasteDib(pDataObject);
    }

    return bRet;
}

void COleClipView::MoveTrackRect(CPoint point)
{
    CClientDC dc(this);
    OnPrepareDC(&dc);
    dc.DrawFocusRect(m_rectTracker);
    dc.LPtoDP(m_rectTracker);
    CSize sizeTrack = m_rectTracker.Size();
    CPoint newTopleft = point - m_dragOffset;  // still device
    m_rectTracker = CRect(newTopleft, sizeTrack);
    m_tracker.m_rect = m_rectTracker;
    dc.DPtoLP(m_rectTracker);
    dc.DrawFocusRect(m_rectTracker);
}

void COleClipView::OnEditPastefrom()
{
    COleClipDoc* pDoc = GetDocument();
    CFileDialog dlg(TRUE, "bmp", "*.bmp");

    if (dlg.DoModal() != IDOK) return;

    if (pDoc->m_dib.AttachMapFile(dlg.GetPathName(), TRUE)) { // share
        CClientDC dc(this);
        pDoc->m_dib.SetSystemPalette(&dc);
        pDoc->SetModifiedFlag();
        pDoc->UpdateAllViews(NULL);
    }
}

void COleClipView::OnEditCopyto()
{
    CDib& dib = GetDocument()->m_dib;
    CFileDialog dlg(FALSE, "bmp", "*.bmp");

    if (dlg.DoModal() != IDOK) return;

    BeginWaitCursor();
    dib.CopyToMapFile(dlg.GetPathName());
    EndWaitCursor();
}
