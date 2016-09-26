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
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_WM_LBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_MESSAGE(WM_VIEWPALETTECHANGED, OnViewPaletteChanged)
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_EDIT_PASTEFROM, OnEditPastefrom)
    ON_COMMAND(ID_EDIT_COPYTO, OnEditCopyto)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPYTO, OnUpdateEditCopy)
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleClipView construction/destruction

COleClipView::COleClipView() : m_rectTracker(20, 20, 300, 300)
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
    if (m_tracker.Track(this, point, FALSE, NULL)) {
        CClientDC dc(this);
        OnPrepareDC(&dc);
        m_rectTracker = m_tracker.m_rect;
        dc.DPtoLP(m_rectTracker); // Update logical coordinates
        Invalidate();
    }
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
    TRACE("CEx26aView::OnViewPaletteChanged, HWND = %x, code = %d\n", GetSafeHwnd(), wParam);
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
    m_tracker.m_nStyle = CRectTracker::solidLine |
                         CRectTracker::resizeOutside;
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
