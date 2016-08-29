// DC ExplorerView.cpp : implementation of the CDCExplorerView class
//

#include "stdafx.h"
#include "DCExplorer.h"
#include <math.h>

#include "DCExplorerDoc.h"
#include "is95.h"

#include "fltedit.h"
#include "intedit.h"
#include "idcombo.h"
#include "gcombo.h"
#include "stock.h"
#include "dcvals.h"
#include "poly.h"
#include "color.h"
#include "hatch.h"
#include "hatchsmp.h"
#include "PenSamp.h"
#include "PenStyle.h"
#include "BrStyle.h"
#include "LineJoin.h"
#include "endcaps.h"

#include "DCExplorerView.h"

#include "Message.h"
#include "MatSamp.h"
#include "MatText.h"
#include "MatGraf.h"
#include "AlgnSamp.h"
#include "mapsamp.h"
#include "SysMsg.h"
#include "ropcodes.h"
#include "bitcombo.h"
#include "numericedit.h"
#include "enumedit.h"
#include "SysColor.h"
#include "pcombo.h"

#include "DCProps.h"
#include "polydraw.h"

#include "bkg.h"
#include "mapping.h"
#include "pensel.h"
#include "text.h"
#include "xform.h"
#include "align.h"
#include "devcaps.h"
#include "palette.h"

#include "degrad.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerView

IMPLEMENT_DYNCREATE(CDCExplorerView, CView)

BEGIN_MESSAGE_MAP(CDCExplorerView, CView)
	//{{AFX_MSG_MAP(CDCExplorerView)
	ON_COMMAND(IDM_DC_EXP, OnDcExp)
	ON_COMMAND(ID_POLYGON, OnPolygon)
	ON_COMMAND(ID_LINES, OnLines)
	ON_COMMAND(ID_FILLEDFIG, OnFilledfig)
	ON_COMMAND(ID_TEXT, OnText)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerView construction/destruction

CDCExplorerView::CDCExplorerView()
{
 page = 0; // default to first property page
 sampleid = 0; // no drawing
}

CDCExplorerView::~CDCExplorerView()
{
}

BOOL CDCExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerView drawing

/****************************************************************************
*                          CDCExplorerView::getScaledFont
* Inputs:
*       CDC * dc: Display context
*	CString & s: String to display
*	CRect & bb: Bounding box
* Result: CFont *
*       A font which allows the text to be displayed entirely within the
*	bounding box specified.  The properties of the font are based
*	on the facecode and other properties (in fact, everything except
*	height) of the font selected for the DC
* Effect: 
*       Creates a font
* Notes:
*	This actually has to create two fonts.  One font is a dummy value
*	used to compute the aspect ratio of the text, and hence determine
*	its maximum scaling in x and y that allow it to fit in the bounding
*	box bb.  The maximum scaling is the min scaling that keeps the
*	text in the box; for a very short box, we can't use full width and
*	for a tall box we can't use full height, and that is accounted for.
****************************************************************************/

CFont * CDCExplorerView::getScaledFont(CDC * dc, CString & s, CRect & bb)
    {
#define fontht 100     

     CFont * font = new CFont;

     LOGFONT lf;

     if(dcv.font != NULL)
        { /* has font */
	 dcv.font->GetObject(sizeof(lf), &lf);
	 lf.lfHeight = fontht;  // canonical font height
	 font->CreateFontIndirect(&lf);
	} /* has font */
     else
        { /* no font */
	 // If there was no font defined, default to creating one
	 // of Times New Roman
         font->CreateFont(fontht,
     		     0,			   // nWidth, 
		     0,			   // nEscapement, 
		     0,			   // nOrientation, 
		     FW_BOLD,		   // nWeight, 
		     FALSE,		   // bItalic, 
		     FALSE,		   // bUnderline, 
		     FALSE,		   // cStrikeOut, 
		     ANSI_CHARSET,	   // nCharSet, 
		     OUT_DEFAULT_PRECIS,   // nOutPrecision, 
		     CLIP_DEFAULT_PRECIS,  // nClipPrecision, 
		     DEFAULT_QUALITY,	   // nQuality, 
		     VARIABLE_PITCH,	   // nPitchAndFamily, 
		     _T("Times New Roman") // lpszFacename 
		     );
	 font->GetObject(sizeof(lf), &lf);
	} /* no font */

     int save = dc->SaveDC();

     dc->SelectObject(font);

     CSize sz = dc->GetTextExtent(s);

     float ratio = (float)min(bb.Width(), bb.Height()) / (float)sz.cx ;

     int newheight = (int) (ratio * (float)fontht);

     dc->RestoreDC(save);

     // Destroy the temporary working font so we can create a new one
     font->DeleteObject();

     lf.lfHeight = newheight;
     font->CreateFontIndirect(&lf);
     return font;
    }

/****************************************************************************
*                           CDCExplorerView::drawPolygon
* Inputs:
*       CDC * dc: Display context
* Result: void
*       
* Effect: 
*       Draws a couple polygons on the surface
****************************************************************************/

void CDCExplorerView::drawPolygon(CDC * dc)
    {
     CRect r;
     GetClientRect(&r);

     int save = dc->SaveDC();
#define polycount 3

     int width = r.Width() / polycount;
     int height = r.Height();
     int vmargin = width / 6;
     int hmargin = height / 6;

     // Draw the first (classic!) object

     CRect p;
     p.left = r.left + vmargin;
     p.top  = r.top + hmargin;
     p.right = p.left + width - vmargin;
     p.bottom = p.top + height - vmargin;

     polyDraw(dc, p);

     // Draw the 5-pointed star

     p.left += width;
     p.right += width;

     // Draw a 5-pointed star with the word "Star" imposed on it

#define sides 5
     CPoint sp[sides]; // vertices of the star

     // We want a circle, so we generate a circle that fits the aspect
     // ratio
     int rd = min(p.Width() / 2, p.Height() / 2);

     CPoint center(p.left + p.Width() / 2, p.top + p.Height() / 2);
     
     for(int i = 0; i < sides; i++)
        { /* compute points */
	 // Note that there are 2*PI radians in a full circle

	 double theta =  ((float)i * (2.0f * 3.141592f) / 5.0f);
	 sp[i].x = center.x + (int)((float)rd * sin(theta));
	 sp[i].y = center.y + (int)((float)rd * cos(theta));
	} /* compute points */

     dc->BeginPath();
     dc->MoveTo(sp[0].x, sp[0].y);
     for(i = 0; i < sides - 1; i++)
        { /* draw to alternate sides */
	 int newside = (2 * i + 2) % sides;
	 dc->LineTo(sp[newside].x, sp[newside].y);
	} /* draw to alternate sides */

     dc->CloseFigure();

     // What we're going to do here is get a generic font of some number
     // of units in height, compute the width and height of the star 
     // caption string which is IDS_STAR_CAPTION, and using the aspect
     // ratio figure out how high the font really has to be to fit in the
     // bounding box of the star.  We then create a new font with the right
     // height to accomplish this.

     CString caption;
     caption.LoadString(IDS_STAR_CAPTION);

     CFont * font = getScaledFont(dc, caption, p);

     // This will work better if the background mode is set to TRANSPARENT
     // but we don't do that for the user.

     // Get the new string size so we can nominally center it...

     dc->SelectObject(font);
     CSize sz = dc->GetTextExtent(caption);

     // This will work strangely if TA_UPDATECP mode is set...so we force
     // a MoveTo first just in case.  However, this still works oddly
     // if the SetAlign is other than TA_TOP, but that's what we want the
     // user to see...

     dc->MoveTo(center.x - sz.cx / 2, center.y - sz.cy / 2);

     dc->TextOut(center.x - sz.cx / 2, center.y - sz.cy / 2, caption);

     dc->EndPath();

     dc->StrokeAndFillPath();

     dc->RestoreDC(save);

     delete font;
    }

/****************************************************************************
*			     CDCExplorerView::drawLines
* Inputs:
*       CDC * dc: Display context
* Result: void
*       
* Effect: 
*       Draws some lines using current DC 
* Notes:	
*	We use paths instead of just drawing lines so that geometric pens
*	work properly under Win95
****************************************************************************/

void CDCExplorerView::drawLines(CDC * dc)
    {
     // The first line we draw is from the current position to 
     // a position 2/3 across and 2/3 down

     CRect r;
     GetClientRect(&r);

     int yaxis = r.Width() / 3;
     int xaxis = (3 * r.Height()) / 4;


     dc->BeginPath();
     CPoint to(yaxis, xaxis);

     dc->LineTo(to.x, to.y);
     dc->EndPath();

     dc->StrokePath();

     dc->BeginPath();
     dc->MoveTo(yaxis, r.top);
     dc->LineTo(r.Width() / 3, r.bottom);

     dc->MoveTo(r.left, xaxis);
     dc->LineTo(r.right, xaxis);


     dc->MoveTo(r.left, xaxis);

     POINT curve[] = {
			yaxis, xaxis - r.Height() / 2,
			yaxis + r.Width() / 4, xaxis + r.Height() / 3,
			r.right, r.top + r.Height() / 5};

			
     dc->PolyBezierTo(curve, 3);

     dc->EndPath();
     dc->StrokePath();

     dc->BeginPath();

     //----------------
     // miter limit example
     //----------------

     int radius = min(r.Height() / 4, r.Width() / 4);
     CPoint center(yaxis + r.Height() / 4, xaxis - r.Height() / 4);

     // Draw a small-angle /_ image.  Note that we use the MiterAngle we
     // set in the setup screen although in fact this is not a DC property.

     dc->MoveTo(center.x + radius, center.y);
     dc->LineTo(center.x, center.y);
     dc->LineTo(center.x + (int)((float)radius
     				      * (float)cos(deg_to_rad(dcv.angle))),
     	        center.y - (int)((float)radius
				      * (float)sin(deg_to_rad(dcv.angle))));
     
     dc->EndPath();
     dc->StrokePath();

    }

/****************************************************************************
*                            CDCExplorerView::drawText
* Inputs:
*       CDC * dc: Display context
* Result: void
*       
* Effect: 
*       Draws some text
****************************************************************************/

void CDCExplorerView::drawText(CDC * dc)
    {
     int save = dc->SaveDC();
     
     TEXTMETRIC tm;

     if(dcv.font != NULL)
	dc->SelectObject(dcv.font);

     dc->GetTextMetrics(&tm);
     int height = tm.tmHeight + tm.tmExternalLeading;

     CString s;

     for(int i = IDS_TEXT_BASE + 1; i < IDS_TEXT_LIMIT; i++)
        { /* show samples */
	 s.LoadString(i);
	 dc->TextOut(0, (i - IDS_TEXT_BASE) * height, s);
	} /* show samples */
	 
     
    }

/****************************************************************************
*                          CDCExplorerView::drawFilledFig
* Inputs:
*       CDC * dc: Display context
* Result: void
*       
* Effect: 
*       Draws some filled figures using current DC settings
****************************************************************************/

void CDCExplorerView::drawFilledFig(CDC * dc)
    {
     CRect r;
     int save = dc->SaveDC();

     GetClientRect(&r);

#define filledcount 4

     int width = r.Width() / filledcount;
     int height = r.Height();
     int vmargin = width / 6;
     int hmargin = height / 6;
     
     CRect rect;
     rect.left = r.left + vmargin;
     rect.right = r.left + width - vmargin;
     rect.top = r.top + hmargin;
     rect.bottom = r.bottom - hmargin;


     dc->Rectangle(&rect);

     // Something like this:
     //
     //   +-----------+
     //   |***********|
     //   |***********|
     //   |****/ \****|
     //   |***/   \***|
     //   |**/     \**|
     //   |*/       \*|
     //   |/         \|

     rect.left += width;
     rect.right += width;

     dc->BeginPath();
     dc->MoveTo(rect.left, rect.top);
     dc->LineTo(rect.right, rect.top);
     dc->LineTo(rect.right, rect.bottom);
     dc->LineTo(rect.left + rect.Width() / 2, rect.top + rect.Height() / 2);
     dc->LineTo(rect.left, rect.bottom);
     dc->CloseFigure();
     dc->EndPath();

     dc->StrokeAndFillPath();

     // Something like this:
     //
     // +-----------+
     //  \         /
     //   \       /
     //    \     /
     //     \   /
     //     /   \
     //    /     \
     //   /       \
     //  /         \
     // +-----------+

     rect.left += width;
     rect.right += width;

     dc->BeginPath();
     dc->MoveTo(rect.left, rect.top);
     dc->LineTo(rect.right, rect.top);
     dc->LineTo(rect.right - rect.Width() / 3, rect.top + rect.Height() / 2);
     dc->LineTo(rect.right, rect.bottom);
     dc->LineTo(rect.left, rect.bottom);
     dc->LineTo(rect.left + rect.Width() / 3,  rect.top + rect.Height() / 2);
     dc->CloseFigure();
     dc->EndPath();

     dc->FillPath();

     rect.left += width;
     rect.right += width;

     CString A = _T("A");

     CFont * font = getScaledFont(dc, A, rect);

     dc->SelectObject(font);

     dc->BeginPath();
     dc->MoveTo(rect.left, rect.top); // in case TA_NOUPDATECP mode
     dc->TextOut(rect.left, rect.top, A);
     dc->EndPath();

     dc->StrokeAndFillPath();

     dc->RestoreDC(save);

     delete font;

    }

void CDCExplorerView::OnDraw(CDC* pDC)
{
	CDCExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	dcv.setDC(pDC);
	switch(sampleid)
	   { /* which ID */
	    case ID_LINES:
		    drawLines(pDC);
		    break;
	    case ID_TEXT:
		    drawText(pDC);
		    break;
	    case ID_FILLEDFIG:
		    drawFilledFig(pDC);
		    break;
	    case ID_POLYGON:
		    drawPolygon(pDC);
		    break;
	   } /* which ID */
}

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerView printing

BOOL CDCExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDCExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDCExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerView diagnostics

#ifdef _DEBUG
void CDCExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CDCExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDCExplorerDoc* CDCExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDCExplorerDoc)));
	return (CDCExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCExplorerView message handlers

void CDCExplorerView::OnDcExp() 
{
 CPropertySheet props;

 CText text;
 CBkg  bkg;
 CMapping mapping;
 CXForm   xform;
 CPenSel  pen;
 CAlign   align;
 CDevCaps devcaps;
 CPaletteDisplay palette;

 text.dcv  = &dcv;
 bkg.dcv   = &dcv;
 pen.dcv   = &dcv;
 mapping.dcv = &dcv;
 xform.dcv = &dcv;
 align.dcv = &dcv;
 devcaps.dcv = &dcv;
 palette.dcv = &dcv;

 props.AddPage(&text);
 props.AddPage(&align);
 props.AddPage(&bkg);
 props.AddPage(&pen);
 props.AddPage(&mapping);
 props.AddPage(&palette);
 if(!Is95())
    props.AddPage(&xform);
 props.AddPage(&devcaps);
	

 props.SetActivePage(dcv.lastpage);

 // There really ought to be a good way to do this, but it looks like
 // it is something in the initialization of CPropertySheet that should
 // handle this, and that is very badly documented.  Consider these two
 // lines a very inelegant solution (disgusting, actually).

 dcv.current_bkmode = dcv.BkMode;
 dcv.current_bkcolor = dcv.BkColor;

 props.DoModal();

 InvalidateRect(NULL);  // redraw with new parameters
}

void CDCExplorerView::OnPolygon() 
{
 sampleid = ID_POLYGON;
 InvalidateRect(NULL);
	
}

void CDCExplorerView::OnLines() 
{
 sampleid = ID_LINES;
 InvalidateRect(NULL);
	
}

void CDCExplorerView::OnFilledfig() 
{
 sampleid = ID_FILLEDFIG;
 InvalidateRect(NULL);
	
}

void CDCExplorerView::OnText() 
{
 sampleid = ID_TEXT;
 InvalidateRect(NULL);
	
}

/****************************************************************************
*                          CDCExplorerView::doContextMenu
* Inputs:
*	CPoint point: Ignored for now
* Result: void
*       
* Effect: 
*       Pops up the modal dialog
****************************************************************************/

void CDCExplorerView::doContextMenu(CPoint point)
    {
     OnDcExp();
    }

void CDCExplorerView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
 	doContextMenu(point);
	
}

void CDCExplorerView::OnRButtonUp(UINT nFlags, CPoint point) 
{
 	doContextMenu(point);
}

void CDCExplorerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
 // If there is more than one non-iconic CDCExplorerView window, we can
 // do a drag-and-drop of properties, otherwise we don't try

 int count = 0;

 // Here's the structure
 //	MDIFramwWnd     (A)
 //	  |
 //	  +-- MDIClient (B)
 //	  |     |
 //	  |	+-- CMDIChildWnd	(C)
 //	  |	|     |
 //       |     |     +--CDCExplorerView	(D)
 //       |     |
 //       |     +-- CMDIChildWnd	(C)
 //       |     |     |
 //       |     |     +--CDCExplorerView   	(D)   <= You are here
 //       |     :			 :
 //       |     :			 :
 //       |     other child windows      :
 //       :
 //       :
 //       toolbars, status bars, etc.

 CMDIFrameWnd * main = (CMDIFrameWnd *)AfxGetMainWnd();   // (A)
 CWnd * parent = CWnd::FromHandle(main->m_hWndMDIClient); // (B)
 CWnd * childFrame;
 for(childFrame = parent->GetWindow(GW_CHILD); 		  // (C)
     childFrame != NULL; 
     childFrame = childFrame->GetWindow(GW_HWNDNEXT))
    { /* count windows */
     CWnd * view = childFrame->GetWindow(GW_CHILD);	  // (D)
     if(view == NULL)
	continue;
     if(!view->IsKindOf(RUNTIME_CLASS(CDCExplorerView)))
	continue; // skip non-this-view windows
     if(view->IsIconic())
	continue;  // ignore iconized windows
     count++;
    } /* count windows */

 if(count > 1)
    { /* drag it */
     SetCapture();
     dump_seq = -1;
     ::SetCursor(AfxGetApp()->LoadCursor(IDC_SELF));
    } /* drag it */

	CView::OnLButtonDown(nFlags, point);
}

/****************************************************************************
*                             CDCExplorerView::dropit
* Inputs:
*       CPoint point: Pint, in screen coordinates
* Result: BOOL
*       TRUE if dropped
*	FALSE if wasn't dropped
* Effect: 
*       Drops the DC parameters on the target window
****************************************************************************/

BOOL CDCExplorerView::dropit(CPoint point)
    {
     CWnd * dropee = WindowFromPoint(point);
     if(dropee != NULL)
	{ /* maybe drop it */
	 // We cannot drop it on ourself, or any other window that
	 // is not a chap5view, and it must not be iconic

         if(dropee->m_hWnd == m_hWnd)
	    return FALSE;  // can't drop on ourself
	 if(dropee->IsIconic())
	    return FALSE;  // ignore iconic windows
	 if(!dropee->IsKindOf(RUNTIME_CLASS(CDCExplorerView)))
	    return FALSE;  // not a view class

	 // Copy the contents of the source dcv (us) into the target
	 dcv.copyTo( &((CDCExplorerView *) dropee)->dcv);

	 dropee->InvalidateRect(NULL);  // force redraw after drop

	 // Note that it does NOT activate the dropee

	 // Make sure we restrict the cursor to the window during
	 // the animation sequence (looks tacky if we drag it
	 // into another window while it is dumping)
	 CRect r;
	 dropee->GetWindowRect(&r);
	 ClipCursor(&r);
	} /* maybe drop it */

     return TRUE;
    }

// The animation sequence for dumping

static int dumping[] = {
		IDC_DUMPING1,
		IDC_DUMPING2,
		IDC_DUMPING3,
		IDC_DUMPING4,
		IDC_DUMPED,
		IDC_DUMPED,
		0 // EOT
		       };

void CDCExplorerView::OnTimer(UINT nIDEvent) 
{
 dump_seq++;
 if(dumping[dump_seq] == 0)
    { /* end of sequence */
     KillTimer(nIDEvent);
     ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
     ReleaseCapture();
     ClipCursor(NULL);
    } /* end of sequence */
 else
    { /* found it */
     ::SetCursor(AfxGetApp()->LoadCursor(dumping[dump_seq]));
    } /* found it */

 CView::OnTimer(nIDEvent);
}

#define interval 200

void CDCExplorerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
 CWnd * capture = GetCapture();
 	if(capture != NULL && capture->m_hWnd == m_hWnd)
	   { /* we have capture */
	    ClientToScreen(&point);
	    if(dropit(point))
	       { /* do drop sequence */
		::SetCursor(AfxGetApp()->LoadCursor(dumping[0]));
		dump_seq = 0;
		SetTimer(1, interval, NULL);
		
	       } /* do drop sequence */
	    else
	       { /* no drop */
	        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		ReleaseCapture();
	       } /* no drop */
	   } /* we have capture */
	
	CView::OnLButtonUp(nFlags, point);
}

void CDCExplorerView::OnMouseMove(UINT nFlags, CPoint point) 
{
 CWnd * capture = GetCapture();

 if(capture != NULL && capture->m_hWnd == m_hWnd)
    { /* have capture */
     ClientToScreen(&point);
     CWnd * dropee = WindowFromPoint(point);
     HCURSOR dragee = NULL;

     // If the dump sequence is active, we don't want to change the
     // cursor as it is under control of the dump sequence timer

     if(dump_seq >= 0)
        { /* animating dump */
	 return;
	} /* animating dump */

     // We could have:
     //		Some window which is not even in our app (IDC_NO)
     //		ourself (IDC_SELF)
     //		An iconic window (IDC_NO)
     //		A CDCExplorerView window which is not ourself (IDC_CANDUMP)

     if(dropee == NULL)
	dragee = AfxGetApp()->LoadStandardCursor(IDC_NO);
     else
     if(dropee->m_hWnd == m_hWnd)
	dragee = AfxGetApp()->LoadCursor(IDC_SELF);
     else
     if(dropee->IsIconic())
	dragee = AfxGetApp()->LoadStandardCursor(IDC_NO);
     else
     if(dropee->IsKindOf(RUNTIME_CLASS(CDCExplorerView)))
	dragee = AfxGetApp()->LoadCursor(IDC_CANDUMP);
     else
        { /* check border */
	 // This avoids that annoying change to the "IDC_NO" icon as we
	 // pass over window borders.  What we do is see if the first
	 // child of the current window is a CDCExplorerView
	 CWnd * child = dropee->GetWindow(GW_CHILD);
	 if(child != NULL)
	    { /* check child */
	     if(child->m_hWnd == m_hWnd)
		dragee = AfxGetApp()->LoadCursor(IDC_SELF);
             else
	     if(child->IsKindOf(RUNTIME_CLASS(CDCExplorerView)))
		dragee = AfxGetApp()->LoadCursor(IDC_CANDUMP);
	    } /* check child */
	} /* check border */

     if(dragee != NULL)
	::SetCursor(dragee);
     else // assume not possible...
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
     return;
    } /* have capture */

 CView::OnMouseMove(nFlags, point);
}


BOOL CDCExplorerView::OnEraseBkgnd(CDC* pDC) 
{
 CRect r;
 GetClientRect(&r);

 // Note that we only support solid brush colors here.  We could extend
 // this to support hatched, bitmap, etc. types of background painting

 CBrush br(dcv.wcolor);
 pDC->FillRect(&r, &br);
 return TRUE;

 // return CView::OnEraseBkgnd(pDC);
}

