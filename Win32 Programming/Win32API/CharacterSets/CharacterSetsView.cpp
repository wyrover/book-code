// CharacterSetsView.cpp : implementation of the CCharacterSetsView class
//

#include "stdafx.h"
#include "CharacterSets.h"

#include "CharacterSetsDoc.h"
#include "CharacterSetsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsView

IMPLEMENT_DYNCREATE(CCharacterSetsView, CScrollView)

BEGIN_MESSAGE_MAP(CCharacterSetsView, CScrollView)
        //{{AFX_MSG_MAP(CCharacterSetsView)
        ON_WM_SIZE()
        ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsView construction/destruction

CCharacterSetsView::CCharacterSetsView()
{
        // TODO: add construction code here

}

CCharacterSetsView::~CCharacterSetsView()
{
}

BOOL CCharacterSetsView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsView drawing

void CCharacterSetsView::OnDraw(CDC* pDC)
{
        CCharacterSetsDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);

        TEXTMETRIC tm;
        pDC->SelectObject(&font);

        pDC->GetTextMetrics(&tm);

        //     lm
        //     |
        //------------------------------------------------
        //     00  01  02  03  04  05  ... 0C  0D  0E  0F
        // 00  xx  xx  xx  xx  xx  xx      xx  xx  xx  xx  
        // 10
        // 20


        CString s = _T(" 00 ");
        CSize lm = pDC->GetTextExtent(s);
        s = _T("00 ");
        CSize sep = pDC->GetTextExtent(s);
        s = _T("00");
        CSize zz = pDC->GetTextExtent(s);

        int half = zz.cx / 4;

        pDC->MoveTo(lm.cx, 0);
        for(int x = 0; x < 0x10; x++)
           { /* show each */
            CString n;
            n.Format(_T("%02x "), x);
            pDC->TextOut(lm.cx + x * sep.cx, 0, n, lstrlen(n));
           } /* show each */

        for(int y = 0; y < 0xFF; y+= 0x10)
           { /* each row */
            CString n;
            int dy = ((y / 0x10) + 1) * sep.cy;

            n.Format(_T(" %02x "), y);
            pDC->TextOut(0, dy, n, lstrlen(n));

            for(int x = 0; x < 0x10; x++)
               { /* each item */
                n.Format(_T("%c"), y + x);
                pDC->TextOut(lm.cx + sep.cx * x + half, dy, n, lstrlen(n));
               } /* each item */
           } /* each row */

}

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsView printing

BOOL CCharacterSetsView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CCharacterSetsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CCharacterSetsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsView diagnostics

#ifdef _DEBUG
void CCharacterSetsView::AssertValid() const
{
        CScrollView::AssertValid();
}

void CCharacterSetsView::Dump(CDumpContext& dc) const
{
        CScrollView::Dump(dc);
}

CCharacterSetsDoc* CCharacterSetsView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCharacterSetsDoc)));
        return (CCharacterSetsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCharacterSetsView message handlers

/****************************************************************************
*                           CChap7View::setScaling
* Result: void
*       
* Effect: 
*       Changes the scaling size for the drawing
****************************************************************************/

void CCharacterSetsView::setScaling()
    {
        CSize sz;
        CClientDC dc(this);
        dc.SelectObject(&font);
        
        TEXTMETRIC tm;
        dc.GetTextMetrics(&tm);

        // Here we "know" how many characters are actually used in the layout
        //
        sz.cy = (tm.tmHeight + tm.tmExternalLeading) * 17;
        sz.cx = (tm.tmAveCharWidth * 53);
        
        CSize page(4 * (tm.tmHeight + tm.tmExternalLeading));
        CSize line(tm.tmHeight + tm.tmExternalLeading);

        SetScrollSizes(MM_TEXT, sz, page, line);
     
    }

void CCharacterSetsView::OnInitialUpdate() 
{
        CScrollView::OnInitialUpdate();

        CString s;
        CMultiDocTemplate * tmp = (CMultiDocTemplate *)GetDocument()->GetDocTemplate();
        tmp->GetDocString(s, CDocTemplate::docName);

        GetParent()->SetWindowText(s);

        setScaling();

        
}

void CCharacterSetsView::OnSize(UINT nType, int cx, int cy) 
{
        CScrollView::OnSize(nType, cx, cy);
        
        setScaling();
        
}

void CCharacterSetsView::OnEditCopy() 
{
 // This creates a Window DC for the entire window, then uses it to grab
 // the bitmap from the window
        
 CWindowDC wdc(this->GetParent());

 CDC memDC;

 memDC.CreateCompatibleDC(&wdc);

 CBitmap bm;
 CRect r;
 
 this->GetParent()->GetWindowRect(&r);

 CSize sz(r.Width(), r.Height());

 bm.CreateCompatibleBitmap(&wdc, sz.cx, sz.cy);
 
 CBitmap * oldbm = memDC.SelectObject(&bm);
 
 memDC.BitBlt(0, 0, sz.cx, sz.cy, &wdc, 0, 0, SRCCOPY);

 OpenClipboard();
 ::EmptyClipboard();
 ::SetClipboardData(CF_BITMAP, bm.m_hObject);
 CloseClipboard();

 memDC.SelectObject(oldbm);
 bm.Detach();  // make sure bitmap not deleted with CBitmap object
 
}
