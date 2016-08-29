// GDI ExplorerView.cpp : implementation of the CGDIExplorerView class
//
#pragma warning(disable:4244)
#pragma warning(disable:4101) // no "unreferenced local variable" errors

#include "stdafx.h"
#include "stdafx.h"
#include "mainfrm.h"
#include "windowsx.h"

#include "GDIExplorer.h"

#include "math.h"
#include "ctype.h"
#include "stdlib.h"
#include "randstat.h"
#include "polypoly.h"

#include "GDIExplorerDoc.h"
#include "GDIExplorerView.h"

#include "select.h"
#include "uwm.h"
#include "wmf.h"
#include "MFH.h"
#include "copy.h"
#include "bb.h"

#include "polypt.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PI 3.141592654f
#define DEG_TO_RAD(n) ((float)(((float)(n)) / (360.0f/(2.0f * PI))))

#define DIM(x) (sizeof(x) / sizeof(x[0]))

static const TCHAR TimesFont[] = _T("Times New Roman");
static const TCHAR ArialFont[] = _T("Arial");

// This is really ugly, but is caused by our desire to keep the drawing
// code in "pure" C for illustrative purposes.  This should actually be
// a per-view member in each class.  It is only necessary because we are
// also writing metafiles for insertion into the book, and this code is
// in C++.

static RECT GlobalBB;

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerView

IMPLEMENT_DYNCREATE(CGDIExplorerView, CView)

BEGIN_MESSAGE_MAP(CGDIExplorerView, CView)
        //{{AFX_MSG_MAP(CGDIExplorerView)
        ON_COMMAND(IDM_SELECT, OnSelect)
        ON_COMMAND(ID_WRITE_METAFILE, OnWriteMetaFile)
        ON_COMMAND(ID_WRITE_ENHANCED, OnWriteEnhancedMetaFile)
        ON_COMMAND(IDM_SETCONTROLPOINTS, OnSetcontrolpoints)
        ON_COMMAND(IDM_SETPOLYFILL_ALTERNATE, OnSetpolyfillAlternate)
        ON_COMMAND(IDM_SETPOLYFILL_WINDING, OnSetpolyfillWinding)
        ON_WM_CONTEXTMENU()
        ON_WM_RBUTTONUP()
        ON_COMMAND(IDM_SETPOLYFILL_NONE, OnSetpolyfillNone)
        ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
        ON_COMMAND(ID_VIEW_BB, OnViewBb)
        ON_UPDATE_COMMAND_UI(ID_VIEW_BB, OnUpdateViewBb)
        //}}AFX_MSG_MAP
        // Standard printing commands
        ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
        ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerView construction/destruction

CGDIExplorerView::CGDIExplorerView()
{
 currentbb = NULL;
 viewbb = FALSE;
 style = IDC_ENDCAPS;
 // Initialize to identity matrix
 printscale.eM11 = 1.0f;  printscale.eM12 = 0.0f;
 printscale.eM21 = 0.0f;  printscale.eM22 = 1.0f;
 printscale.eDx  = 0.0f;  printscale.eDy  = 0.0f;
 SetParameters();
 rd.seed = 0;
 pd.fillmode = rd.fillmode = ALTERNATE; // default case
 rd.sides = 0;
 rd.RandomSides = TRUE;

 pd.count = 0;
 pd.points = NULL;
}

CGDIExplorerView::~CGDIExplorerView()
{
}

BOOL CGDIExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return CView::PreCreateWindow(cs);
}

typedef struct {
                int caption;    // string ID of caption
                BYTE style[4]; // coordinate style
                POINT pt[4];
               } bezier;

bezier b1 = { IDS_BEZIER_DOUBLE_CURVE,     
              { DT_RIGHT | DT_TOP,
                DT_CENTER | DT_BOTTOM,
                DT_CENTER | DT_TOP,
                DT_CENTER | DT_BOTTOM},
              { 30,  40,
                50,  20,
                70, 100,
                80,  40}
            };
bezier b2 = { IDS_BEZIER_CROSSOVER,
              {DT_RIGHT | DT_TOP,
               DT_LEFT | DT_VCENTER,
               DT_RIGHT | DT_VCENTER,
               DT_CENTER | DT_TOP},
              {120, 80,
               200, 50,
               110, 40,
               170, 80}
            };

bezier b3 = { IDS_BEZIER_SIMPLE_CURVE,
              {DT_CENTER | DT_TOP,
               DT_CENTER | DT_BOTTOM,
               DT_CENTER | DT_BOTTOM,
               DT_CENTER | DT_TOP},
              {220, 60,
               240, 40,
               260, 40,
               280, 60}
            };

bezier b4 = { IDS_BEZIER_COMPLEX_CURVE,
              {DT_CENTER | DT_TOP,
               DT_RIGHT | DT_BOTTOM,
               DT_LEFT | DT_BOTTOM,
               DT_CENTER | DT_TOP},
              {300, 40,
               330, 10,
               370,  5,
               360, 40}
            };

/****************************************************************************
*                               ids_MessageBox
* Inputs:
*       HWND hwnd: Parent window
*       int body: Body text string ID
*       int caption: Caption text string ID
*       int styles: Messagebox styles
* Result: int
*       Result of MessageBox call
* Effect: 
*       Loads the strings and issues the MessageBox call
****************************************************************************/

int ids_MessageBox(HWND hwnd, int body, int caption, int styles)
    {
     TCHAR s_body[256];
     TCHAR s_caption[256];
     LoadString(GetWindowInstance(hwnd), body, s_body, DIM(s_body));
     LoadString(GetWindowInstance(hwnd), caption, s_caption, DIM(s_caption));
     return MessageBox(hwnd, s_body, s_caption, styles);
    }

/****************************************************************************
*                              createWeightedFont
* Inputs:
*       int size: Height of font, positive for pixels, negative for points
*       LPCTSTR facecode: The face code name to use
*       int weight: Weight, one of the FW_ codes or an integer value
* Result: HFONT
*       Newly-created font
* Effect: 
*       Creates a font whose weight is specified explicitly
****************************************************************************/

HFONT createWeightedFont(int size, LPCTSTR facecode, int weight)
    {
     return CreateFont(size, 0,0,0, weight, FALSE, FALSE, FALSE, 
                        ANSI_CHARSET, 
                        OUT_DEFAULT_PRECIS,
                        CLIP_DEFAULT_PRECIS,
                        PROOF_QUALITY,
                        VARIABLE_PITCH,
                        facecode);
    }

/****************************************************************************
*                              createCaptionFont
* Inputs:
*       int size: Height of font, positive for pixels, negative for points
*       LPCTSTR facecode: The face code name to use
* Result: HFONT
*       Newly-created font
* Effect: 
*       Creates a font whose weight is FW_NORMAL
****************************************************************************/

HFONT createCaptionFont(int size, LPCTSTR facecode)
    {
     return createWeightedFont(size, facecode, FW_NORMAL);
    }

/****************************************************************************
*                                clearGlobalBB
* Result: void
*       
* Effect: 
*       Clears the global bounding box used by the metafile writer
****************************************************************************/

void clearGlobalBB()
    {
     GlobalBB.top = INT_MAX;
     GlobalBB.bottom = INT_MIN;
     GlobalBB.left = INT_MAX;
     GlobalBB.right = INT_MIN;
    }

/****************************************************************************
*                                 globalBBSet
* Result: BOOL
*       TRUE if the global BB has been set
*       FALSE if it is unmodified
****************************************************************************/

BOOL globalBBSet()
    {
     return GlobalBB.top != INT_MAX;
    }

/****************************************************************************
*                               updateGlobalBB
* Inputs:
*       LPRECT bb: New bounding box to use
* Result: void
*       
* Effect: 
*       Updates the global bounding box with the local bounding box
****************************************************************************/

void updateGlobalBB(LPRECT bb)
    {
     GlobalBB.top = min(bb->top, GlobalBB.top);
     GlobalBB.left = min(bb->left, GlobalBB.left);
     GlobalBB.bottom = max(bb->bottom, GlobalBB.bottom);
     GlobalBB.right = max(bb->right, GlobalBB.right);
    }


/****************************************************************************
*                                    scale
* Inputs:
*       float xscale: X-scaling factor
*       float yscale: Y-scaling factor
* Result: void
*       
* Effect: 
*       Scales the CTM by xscale, yscale
****************************************************************************/

static void scale(HDC hdc, float xscale, float yscale)
    {
     XFORM scalefactor = { 1.0f, 0.0f, 
                           0.0f, 1.0f, 
                           0.0f, 0.0f };  // identity
     
     scalefactor.eM11 = xscale;
     scalefactor.eM22 = yscale;

     if(!ModifyWorldTransform(hdc, &scalefactor, MWT_LEFTMULTIPLY))
        { /* scale hard way */
         SetWindowExtEx(hdc, 1000, 1000, NULL);
         SetViewportExtEx(hdc, (int)(xscale * 1000.0f), 
                               (int)(yscale * 1000.0f), NULL);
        } /* scale hard way */
    }

/****************************************************************************
*                                scaleToWindow
* Inputs:
*       HDC hdc: Display context
*       HWND hwnd: Window handle
*       int width: Width to scale
*       int height: Height to scale 
* Result: void
*       
* Effect: 
*       Scales the window min(hwnd.width()/width, hwnd.height/height)
****************************************************************************/

void scaleToWindow(HDC hdc, HWND hwnd, int width, int height)
    {
     RECT r;

     ::GetClientRect(hwnd, &r);

     float dx = (float)r.right / (float)width;
     float dy = (float)r.bottom / (float) height;
     
     float scale = min(dx, dy);

     SetMapMode(hdc, MM_ISOTROPIC);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, (int)(1000.0f * scale), (int)(1000.0f * scale),
                                        NULL);
    }

/****************************************************************************
*                                  translate
* Inputs:
*       float xdelta:
*       float ydelta:
* Result: void
*       
* Effect: 
*       Applies a translation to the world coordinate system
****************************************************************************/

static void translate(HDC hdc, float xdelta, float ydelta)
    {
     XFORM xlate = { 1.0f, 0.0f, 
                     0.0f, 1.0f, 
                     0.0f, 0.0f };  // identity
     
     xlate.eDx = xdelta;
     xlate.eDy = ydelta;

     if(!ModifyWorldTransform(hdc, &xlate, MWT_LEFTMULTIPLY))
        { /* compound existing translate  */
         POINT p;
         GetWindowOrgEx(hdc, &p);
         p.x -= (int)xdelta;
         p.y -= (int)ydelta;
         SetWindowOrgEx(hdc, p.x, p.y, NULL);
        } /* compound existing translate  */
    }


/****************************************************************************
*                                ids_DrawText
* Inputs:
*       HDC hdc: DC to draw into
*       int ids: String ID
*       LPRECT r: Rectangle
*       int styles: Styles to use
* Result: void
*       
* Effect: 
*       Loads the string and does a DrawText
****************************************************************************/

static void ids_DrawText(HINSTANCE hinst, HDC hdc, int ids, LPRECT r, int styles)
    {
     TCHAR text[256];
     LoadString(hinst, ids, text, DIM(text));
     DrawText(hdc, text, -1, r, styles);
    }

/****************************************************************************
*                          ids_GetTextExtentPoint32
* Inputs:
*       HINSTANCE hinst: Instance handle for string
*       HDC hdc: DC to use
*       int ids: String ID to use
*       LPSIZE size: Size object to modify
* Result: BOOL
*       TRUE if success, FALSE if failure
* Effect: 
*       Computes the size from a string in the resource file
****************************************************************************/

static BOOL
ids_GetTextExtentPoint32(HINSTANCE hinst, HDC hdc, int ids, LPSIZE size)
    {
     TCHAR text[256];
     LoadString(hinst, ids, text, DIM(text));

     return GetTextExtentPoint32(hdc, text, lstrlen(text), size);
    }

/****************************************************************************
*                                     nyi
* Result: void
*       
* Effect: 
*       Displays "Not yet implemented" message
****************************************************************************/

static void nyi(HWND hwnd, HDC hdc)
    {
     HFONT font = createCaptionFont(-30, ArialFont);
     RECT r;

     GetClientRect(hwnd, &r);
     ids_DrawText(GetWindowInstance(hwnd), hdc, IDS_NYI, &r, 
                                        DT_CENTER | DT_VCENTER | DT_WORDBREAK);
     DeleteFont(font);
    }

/****************************************************************************
*                                  DrawJoin
* Inputs:
*       HDC hdc: Display context
*       int y: Vertical coordinate for display
*       int style: Join style
*       int caption: ID of explanation string
* Result: void
*       
* Effect: 
*       Draws three examples of the line join
****************************************************************************/

#define JOIN_PEN_WIDTH 15
#define JOIN_SAMPLE_SIZE 100
#define JOIN_WIDTH  (JOIN_SAMPLE_SIZE + 2 * JOIN_SAMPLE_SIZE/3)
#define JOIN_HEIGHT (JOIN_SAMPLE_SIZE + JOIN_SAMPLE_SIZE/4)

#define XOFFSET (-30)
#define JOIN_IMAGE_WIDTH ((5 * JOIN_WIDTH) - XOFFSET)

static void drawJoin(HWND hwnd, HDC hdc, int y, int style, int caption, CBoundingBox * bbdata)
    {

     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0), 0 };
     HPEN pen = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT | style, 
                             JOIN_PEN_WIDTH, &blackbrush, 0, NULL);
     int restore;
     RECT r;
     POINT pts[3];
     HFONT font = createCaptionFont(-20, TimesFont);
     TCHAR text[256];

     LoadString(GetWindowInstance(hwnd),caption, text, DIM(text));

     restore = SaveDC(hdc);

     SelectFont(hdc, font);

     SetWindowOrgEx(hdc, XOFFSET, 0, NULL);

     r.top = y;
     r.bottom = r.top + JOIN_SAMPLE_SIZE;
     r.left = 0;
     r.right = r.left + JOIN_WIDTH;

     bbdata->Set(r);

     DrawText(hdc, text, -1, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

     SelectPen(hdc, pen);

     r.right = r.left + JOIN_SAMPLE_SIZE;

     bbdata->SetRight(r);

     if(Is95())
        BeginPath(hdc);

     OffsetRect(&r, JOIN_WIDTH, 0);
     pts[0].x = r.left;
     pts[0].y = r.bottom;
     pts[1].x = r.right;
     pts[1].y = r.bottom;
     pts[2].x = r.right;
     pts[2].y = r.top;
     Polyline(hdc, &pts[0], DIM(pts));

     OffsetRect(&r, JOIN_WIDTH, 0);
     pts[0].x = r.left;
     pts[0].y = r.bottom;
     pts[1].x = r.right;
     pts[1].y = r.bottom;
     pts[2].x = r.left;
     pts[2].y = r.top;
     Polyline(hdc, &pts[0], DIM(pts));

     OffsetRect(&r, JOIN_WIDTH, 0);
     pts[0].x = r.left;
     pts[0].y = r.bottom;
     pts[1].x = r.right;
     pts[1].y = r.bottom;
     pts[2].x = r.left;
     pts[2].y = r.bottom - JOIN_SAMPLE_SIZE/4;
     Polyline(hdc, &pts[0], DIM(pts));

     OffsetRect(&r, JOIN_WIDTH, 0);
     pts[0].x = r.left;
     pts[0].y = r.bottom;
     pts[1].x = r.right;
     pts[1].y = r.bottom;
     pts[2].x = r.left;
     pts[2].y = r.bottom - JOIN_SAMPLE_SIZE/6;
     Polyline(hdc, &pts[0], DIM(pts));

     if(Is95())
        { /* end path */
         EndPath(hdc);
         StrokePath(hdc);
        } /* end path */

     bbdata->LPtoDP(CDC::FromHandle(hdc));

     RestoreDC(hdc, restore);

     DeletePen(pen);
     DeleteFont(font);
    }

/****************************************************************************
*                                join_OnPaint
* Inputs:
*       HDC hdc: DC
* Result: void
*       
* Effect: 
*       Shows how line joins work
****************************************************************************/

static CBoundingBox
join_OnPaint(HWND hwnd, HDC hdc)
    {
     CBoundingBox bbdata(FALSE, TRUE) ;
#define YSTART 20     

     scaleToWindow(hdc, hwnd, JOIN_IMAGE_WIDTH, YSTART + 2 * JOIN_HEIGHT);

     drawJoin(hwnd, hdc, YSTART +               0, PS_JOIN_BEVEL, IDS_BEVEL_JOIN, &bbdata);
     drawJoin(hwnd, hdc, YSTART +     JOIN_HEIGHT, PS_JOIN_MITER, IDS_MITER_JOIN, &bbdata);
     drawJoin(hwnd, hdc, YSTART + 2 * JOIN_HEIGHT, PS_JOIN_ROUND, IDS_ROUND_JOIN, &bbdata);

     return bbdata;
    }

/****************************************************************************
*                                   drawDot
* Inputs:
*       HDC hdc: display context
*       LPPOINT pt: Point at which to draw dot
*       BYTE style: Style control
*       LPCTSTR label: Optional label, or NULL
* Result: void
*       
* Effect: 
*       Draws the dot and labels it according to the given text
****************************************************************************/

static void drawDot(HDC hdc, LPPOINT pt, BYTE style, LPCTSTR label)
    {
     RECT r;
     HFONT f;
     SIZE size;
     SIZE space;
     TEXTMETRIC tm;
     int height;

#define DOTSIZE 1
     Ellipse(hdc, pt->x - DOTSIZE, 
                  pt->y - DOTSIZE, 
                  pt->x + DOTSIZE, 
                  pt->y + DOTSIZE);


     
     if(label != NULL)
        { /* has text */
         // Create a nice small font 

         f = createCaptionFont(-5, ArialFont);

         SelectFont(hdc, f);

         // Determine where to place our text
         GetTextExtentPoint32(hdc, _T(" "), 1, &space);
         GetTextExtentPoint32(hdc, label, lstrlen(label), &size);

         GetTextMetrics(hdc, &tm);
         height = tm.tmHeight + tm.tmInternalLeading;

         r.left = pt->x;
         r.top  = pt->y;

         if(style & DT_LEFT)
            r.left = pt->x + space.cx;
         else
         if(style & DT_CENTER)
            r.left = pt->x - size.cx / 2 - space.cx;
         else
         if(style & DT_RIGHT)
            r.left = pt->x - size.cx - 2 * space.cx;

         if(style & DT_TOP)
            r.top = pt->y + 16 * DOTSIZE;
         else
         if(style & DT_VCENTER)
            r.top = pt->y - height;
         else
         if(style & DT_BOTTOM)
            r.top = pt->y - 2 * height;

         r.bottom = r.top + 2 * height;
         r.right = r.left + size.cx + 2 * space.cx;

         // FrameRect(hdc, &r, GetStockBrush(BLACK_BRUSH));

         DrawText(hdc, label, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

         DeleteFont(f);
        } /* has text */
    }

/****************************************************************************
*                                drawBezierDot
* Inputs:
*       HDC hdc: display context
*       bezier * b: Bezier descriptor
*       int i: Index of bezier point to describe
* Result: void
*       
* Effect: 
*       Draws an illustrative dot using the currently selected pen and
*       brush.  Draws the pen coordinates using the style parameters
****************************************************************************/

static void drawBezierDot(HDC hdc, bezier * b, int i)
    {
     TCHAR coords[30];
     wsprintf(coords,_T("%d,%d"), b->pt[i].x, b->pt[i].y);

     drawDot(hdc, &b->pt[i], b->style[i], coords);

    }

/****************************************************************************
*                                 drawBezier
* Inputs:
*       HDC hdc: DC to draw in
*       bezier * b: Bezier point description
* Result: void
*       
* Effect: 
*       Draws a bezier curve, showing all the control points
****************************************************************************/

static void drawBezier(HDC hdc, HWND hwnd, bezier * b)
    {
     HPEN showpen = CreatePen(PS_SOLID, 1, RGB(192,192,192)); 
     HPEN curvepen = CreatePen(PS_SOLID, 2, RGB(0,0,0));

     SelectPen(hdc, showpen);
     Polyline(hdc, &b->pt[0], 4);

     SelectPen(hdc, curvepen);
     if(!PolyBezier(hdc, &b->pt[0], 4))
        { /* no beziers */
         PostMessage(hwnd, UWM_ERROR, IDS_BEZIER_FAILED, IDS_NOT_SUPPORTED);
        } /* no beziers */
     else
        { /* finish drawing */
         SelectPen(hdc, GetStockPen(BLACK_PEN));
         SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
         drawBezierDot(hdc, b, 0);
         drawBezierDot(hdc, b, 1);
         drawBezierDot(hdc, b, 2);
         drawBezierDot(hdc, b, 3);
        } /* finish drawing */

     DeletePen(showpen);
     DeletePen(curvepen);
    }

/****************************************************************************
*                               bezier_OnPaint
* Inputs:
*       HDC hdc: Display context
* Result: void
*       
* Effect: 
*       Draws some Bezier curves
****************************************************************************/

static void bezier_OnPaint(HWND hwnd, HDC hdc)
    {
     int restore = SaveDC(hdc);

     SetGraphicsMode(hdc, GM_ADVANCED);
     translate(hdc, 0.0f, 20.0f);

     scaleToWindow(hdc, hwnd, 400, 120);

     drawBezier(hdc, hwnd, &b1);
     drawBezier(hdc, hwnd, &b2);
     drawBezier(hdc, hwnd, &b3);
     drawBezier(hdc, hwnd, &b4);

     RestoreDC(hdc, restore);

    }

/****************************************************************************
*                           endcaps_DrawPenExample
* Inputs:
*       HWND hwnd: Window handle
*       HDC hdc: DC to draw in
*       BOOL stroke: TRUE to draw with PolyLine and StrokePath
*                    FALSE to draw as separate lines
*       RECT r: Rectangle defining the drawing space
*       HPEN horz: Pen to use for horizontal line
*       HPEN vert: Pen to use for vertical line
*       LPCTSTR text: Text to display under the rectangle
* Result: void
*       
* Effect: 
*       Draws a horizontal pen, then draws a vertical pen, then draws
*       a 1-pixel white pen across the same endpoints.
****************************************************************************/
#define WIDTH 20

static void endcaps_DrawPenExample(HWND hwnd, HDC hdc, BOOL stroke,
                LPRECT r, HPEN horz, HPEN vert, int caption, BOOL angle)
    {
     HPEN whitepen = GetStockPen(WHITE_PEN);
     TEXTMETRIC tm;
     RECT trect;
     TCHAR text[50];

     LoadString(GetWindowInstance(hwnd), caption, text, DIM(text));


     if(stroke)
        BeginPath(hdc);

     SelectPen(hdc, horz);
     MoveToEx(hdc, r->left, r->top, NULL);
     LineTo(hdc, r->right, r->top);

     if(stroke)
        { /* end line 1 */
         EndPath(hdc);
         StrokePath(hdc);
         BeginPath(hdc);
        } /* end line 1 */

     SelectPen(hdc, vert);
     LineTo(hdc, (angle ? r->left : r->right), r->bottom);

     if(stroke)
        { /* end line 1 */
         EndPath(hdc);
         StrokePath(hdc);
         BeginPath(hdc);
        } /* end line 1 */

     SelectPen(hdc, whitepen);
     MoveToEx(hdc, r->left, r->top, NULL);
     LineTo(hdc, r->right, r->top);
     LineTo(hdc, (angle ? r->left : r->right), r->bottom);

     if(stroke)
        { /* end line 1 */
         EndPath(hdc);
         StrokePath(hdc);
        } /* end line 1 */


     // define a rectangle for the text
     trect.left = r->left;
     trect.right = r->right;
     trect.top = r->bottom + 2 * WIDTH;
     GetTextMetrics(hdc, &tm);
     trect.bottom = trect.top + 2 * (tm.tmHeight + tm.tmExternalLeading);
     DrawText(hdc, text, -1, &trect, DT_CENTER | DT_VCENTER);
    }

/****************************************************************************
*                             endcaps_DrawEndCaps
* Inputs:
*       HWND hwnd: Window handle
*       HDC hdc: Display context
*       BOOL stroke: TRUE to draw with Polyline and StrokePath
*                    FALSE to draw with as separate lines
* Result: void
*       
* Effect: 
*       Draws two lines.  On NT platforms, this can be done with separate
*       line draw operations; on 95 this must be done with Polyline and
*       StrokePath.
****************************************************************************/

void endcaps_DrawEndCaps(HWND hwnd, HDC hdc, BOOL stroke)
    {
     #define X1 (1 * WIDTH)
     #define Y1 (2 * WIDTH)
     #define Y2 (6 * WIDTH)
     #define R  (3 * WIDTH)
     #define Dx (5 * WIDTH)
     #define Dy (8 * WIDTH)
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0),       0 };
     LOGBRUSH graybrush  = {BS_SOLID, RGB(128,128,128), 0 };
     RECT r1 = {X1,          Y1,      X1 + R,          Y1 + R};
     RECT r2 = {X1 + Dx,     Y1,      X1 + R + Dx,     Y1 + R};
     RECT r3 = {X1 + 2 * Dx, Y1,      X1 + R + 2 * Dx, Y1 + R};
     RECT r4 = {X1,          Y1 + Dy, X1 + R,          Y1 + Dy + R};
     RECT r5 = {X1 + Dx,     Y1 + Dy, X1 + R + Dx,     Y1 + Dy + R};
     RECT r6 = {X1 + 2 * Dx, Y1 + Dy, X1 + R + 2 * Dx, Y1 + Dy + R};
     RECT rexplain = 
               {X1,              Y1 + Dy + 3 * R, 
                X1 + R + 2 * Dx, Y1 + Dy + 7 * R};

     HPEN roundblack = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_ROUND, 
                                WIDTH, &blackbrush, 0, NULL);
     HPEN squareblack = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_SQUARE,
                                WIDTH, &blackbrush, 0, NULL);
     HPEN flatblack = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT,
                                WIDTH, &blackbrush, 0, NULL);
     HPEN roundgray = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_ROUND, 
                                WIDTH, &graybrush, 0, NULL);
     HPEN squaregray = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_SQUARE,
                                WIDTH, &graybrush, 0, NULL);
     HPEN flatgray = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_FLAT,
                                WIDTH, &graybrush, 0, NULL);
     HFONT font = createCaptionFont(-20, TimesFont);
     
     int restore;

     int offset = -Dx;
     SetWindowOrgEx(hdc, offset, 0, NULL);
     if(!stroke && Is95())
        scaleToWindow(hdc, hwnd, -offset + 
                        max(r6.right, rexplain.right) + Dx, rexplain.bottom);
     else
        scaleToWindow(hdc, hwnd, -offset + r6.right + Dx, r6.bottom + Dy);

     if(roundblack == NULL)
        { /* no ExtCreatePen */
         PostMessage(hwnd, UWM_ERROR, IDS_EXTCREATEPEN_FAILED, IDS_NOT_SUPPORTED);
         DeleteFont(font);
         return;
        } /* no ExtCreatePen */

     restore = SaveDC(hdc);

     SelectFont(hdc, font);

     endcaps_DrawPenExample(hwnd, hdc, stroke, &r1, 
                                roundblack, roundgray, IDS_ROUND, FALSE);
     endcaps_DrawPenExample(hwnd, hdc, stroke, &r2, 
                                squareblack, squaregray, IDS_SQUARE, FALSE);
     endcaps_DrawPenExample(hwnd, hdc, stroke, &r3, 
                                flatblack, flatgray, IDS_FLAT, FALSE);

     endcaps_DrawPenExample(hwnd, hdc, stroke, &r4, 
                                roundblack, roundgray, IDS_ROUND, TRUE);
     endcaps_DrawPenExample(hwnd, hdc, stroke, &r5, 
                                squareblack, squaregray, IDS_SQUARE, TRUE);
     endcaps_DrawPenExample(hwnd, hdc, stroke, &r6, 
                                flatblack, flatgray, IDS_FLAT, TRUE);

     if(!stroke && Is95())
        { /* show explanation */
         CString s;
         s.LoadString(IDS_WIN95FAILURE);
         DrawText(hdc, s, -1, &rexplain, 
                        DT_CENTER | DT_WORDBREAK | DT_VCENTER);
        } /* show explanation */

     RestoreDC(hdc, restore);

     DeletePen(roundblack);
     DeletePen(squareblack);
     DeletePen(flatblack);
     DeletePen(roundgray);
     DeletePen(squaregray);
     DeletePen(flatgray);
     DeleteFont(font);
     
    }

/****************************************************************************
*                               endcaps_OnPaint
* Inputs:
*       HDC dc: Display context
* Result: void
*       
* Effect: 
*       Draws the endcaps example
****************************************************************************/

static void 
endcaps_OnPaint(HWND hwnd, HDC hdc)
    {
     endcaps_DrawEndCaps(hwnd, hdc, FALSE);
    }

/****************************************************************************
*                           endcaps_stroked_OnPaint
* Inputs:
*       HDC dc: Display context
* Result: void
*       
* Effect: 
*       Draws the endcaps example
****************************************************************************/

static void 
endcaps_stroked_OnPaint(HWND hwnd, HDC hdc)
    {
     endcaps_DrawEndCaps(hwnd, hdc, TRUE);
    }


/****************************************************************************
*                              textclip_MakePath
* Inputs:
*       HDC hdc: Display context
* Result: void
*       
* Effect: 
*       Creates a clipping path
****************************************************************************/

static void textclip_MakePath(HWND hwnd, HDC hdc)
    {
     RECT r;            // client rectangle
     HFONT clipfont;    // font used to define clipping region
     HFONT oldfont;
     TCHAR text[128];

     GetClientRect(hwnd, &r);

     // Scale the string so that it will fill the client area

     clipfont = createWeightedFont(r.bottom, TimesFont, FW_BOLD);
     
     SetGraphicsMode(hdc, GM_ADVANCED);

     oldfont = SelectFont(hdc, clipfont);

     LoadString(GetWindowInstance(hwnd), IDS_CLIPPER, text, DIM(text));
     // If we want the text itself to be the clipping region, we must set
     // transparent mode.  

     BeginPath(hdc);
     SetBkMode(hdc, TRANSPARENT);
     TextOut(hdc, 0, 0, text, lstrlen(text));
     EndPath(hdc);

     SelectFont(hdc, oldfont);
     DeleteFont(clipfont);
    }

/****************************************************************************
*                           textclip_DrawBackground
* Inputs:
*       HWND hwnd:
*       HDC hdc:
* Result: void
*       
* Effect: 
*       Draws the background text for the clipping example
****************************************************************************/

static void textclip_DrawBackground(HWND hwnd, HDC hdc)
    {
     RECT r;            // client rectangle
     HFONT bkgndfont;   // font used to draw background words
     HFONT oldfont;     // previous font
     POINT pt;          // current output position
     TEXTMETRIC tm;     // for determining font height
     int offset;        // used to produce nicer appearance of background
     int line;          // ...
     TCHAR text[128];
     int textlen;

     GetClientRect(hwnd, &r);

     // Create a background font so that the client area will contain
     // BKGND_ROWS lines of text

#define BKGND_ROWS 25
     bkgndfont = createCaptionFont(r.bottom / BKGND_ROWS, TimesFont);

     // We want text output to update the current position
     SetTextAlign(hdc, TA_UPDATECP);

     // Set up for the background text drawing loops
     oldfont = SelectFont(hdc, bkgndfont);
     GetTextMetrics(hdc, &tm);
     offset = 0;
     line = 0;

     MoveToEx(hdc, 0, 0, NULL);
     GetCurrentPositionEx(hdc, &pt);

     textlen = LoadString(GetWindowInstance(hwnd), IDS_BG, text, DIM(text));

     while(pt.y < r.bottom)
        { /* output one row */
         // Avoid boring columnar output: offset each line by one character
         // of the string

         offset = line;

         // Now draw the row
         while(pt.x < r.right)
            { /* output background word */
             TextOut(hdc, 0, 0, &text[offset], 
                                     textlen - 1 - offset);
             GetCurrentPositionEx(hdc, &pt);

             offset = 0;                // make sure full word draws next time
            } /* output background word */

         // Prepare for the next row

         line = (line + 1) % (textlen - 1);
         MoveToEx(hdc, 0, pt.y + tm.tmHeight, NULL);
         GetCurrentPositionEx(hdc, &pt);
        } /* output one row */
     
     SelectFont(hdc, oldfont);
     DeleteFont(bkgndfont);
    }

/****************************************************************************
*                                 textclip3_OnPaint
* Inputs:
*       HDC hdc: DC to draw in
* Result: void
*       
* Effect: 
*       Draws a large word "Cat" with lots of "mews" inside it
****************************************************************************/

static void
textclip3_OnPaint(HWND hwnd, HDC hdc)
    {
     int restore = SaveDC(hdc);
     textclip_MakePath(hwnd, hdc);
     
     // Now set the path defined by the text to be the clipping path

     SelectClipPath(hdc, RGN_COPY);

     textclip_DrawBackground(hwnd, hdc);


     // Clean up the DC and delete the resources created
     RestoreDC(hdc, restore);
    }

/****************************************************************************
*                              textclip1_OnPaint
* Inputs:
*       HDC hdc: DC to draw in
* Result: void
*       
* Effect: 
*       Draws a large word "Cat" and strokes it
****************************************************************************/

static void
textclip1_OnPaint(HWND hwnd, HDC hdc)
    {
     int restore = SaveDC(hdc);
     textclip_MakePath(hwnd, hdc);
     
     StrokePath(hdc);

     // Clean up the DC and delete the resources created
     RestoreDC(hdc, restore);
    }

/****************************************************************************
*                                 textclip2_OnPaint
* Inputs:
*       HDC hdc: DC to draw in
* Result: void
*       
* Effect: 
*       Draws a large word "Cat" with lots of "mews" inside it
****************************************************************************/

static void
textclip2_OnPaint(HWND hwnd, HDC hdc)
    {
     int restore = SaveDC(hdc);
     textclip_MakePath(hwnd, hdc);
     
     // Now set the path defined by the text to be the clipping path

     StrokePath(hdc);

     textclip_DrawBackground(hwnd, hdc);


     // Clean up the DC and delete the resources created
     RestoreDC(hdc, restore);
    }

typedef struct {
        int caption;
        XFORM x;
               } xforms;

xforms xf[] = {
{ IDS_XFORM_IDENTITY,
                 {1.0f, 0.0f,
                  0.0f, 1.0f,
                  0.0f, 0.0f}},

{ IDS_XFORM_TRANSLATION, 
                 { 1.0f,   0.0f, 
                   0.0f,   1.0f,
                   10.0f, 10.0f}},

{ IDS_XFORM_ROTATION30,
                { /* cos 30 */ 0.86602540f,  /* sin 30 */ 0.5f,
                  /*-sin 30 */-0.5f,         /* cos 30 */ 0.86602540f,
                               0.0f,                     0.0f}},

{ IDS_XFORM_SCALING_HALF, 
                   {0.5f,  0.0f,
                    0.0f,  0.5f,
                    0.0f,  0.0f}},

{ IDS_XFORM_SHEARV, 
                    {1.0f, 0.5f,
                     0.0f, 1.0f,
                     0.0f, 0.0f}},
                      
{ IDS_XFORM_SHEARH, 
                      {1.0f,   0.0f,
                       0.5f,   1.0f,
                       0.0f,   0.0f}},
                      
{ NULL /* END OF TABLE */ }
              };


/****************************************************************************
*                                drawTransform
* Inputs:
*       HDC hdc: DC in which transform is drawn
* Result: void
*       
* Effect: 
*       Draws the example
****************************************************************************/

#define XFORM_SIZE 80
#define XFORM_OVERSHOOT (XFORM_SIZE / 10)

static void
drawTransform(HWND hwnd, HDC hdc, xforms * xf)
    {
     int orgdc;
     TCHAR s_caption[256];
     HPEN axispen = CreatePen(PS_SOLID, 1, RGB(128,128,128)); 
     RECT r;
     int height;
     TEXTMETRIC tm;
     HFONT font;

     orgdc = SaveDC(hdc);
     
     SetMapMode(hdc, MM_LOENGLISH);

     SelectPen(hdc, axispen);

     MoveToEx(hdc, 0, -XFORM_OVERSHOOT, NULL);
     LineTo(hdc, 0, XFORM_SIZE + XFORM_OVERSHOOT);
     MoveToEx(hdc, -XFORM_OVERSHOOT, 0, NULL);
     LineTo(hdc, XFORM_SIZE + XFORM_OVERSHOOT, 0);

     SelectPen(hdc, GetStockPen(BLACK_PEN));
     SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));
     Rectangle(hdc, XFORM_SIZE, 0, 0, XFORM_SIZE/2);

     font = createCaptionFont(-14,ArialFont);
     SelectFont(hdc, font);

     GetTextMetrics(hdc, &tm);
     height = tm.tmHeight + tm.tmInternalLeading;

     SetRect(&r, -XFORM_OVERSHOOT, -2*XFORM_OVERSHOOT,
                 XFORM_SIZE + XFORM_OVERSHOOT,
                 2*height);

     LoadString(GetWindowInstance(hwnd), xf->caption, 
                                         s_caption, DIM(s_caption));
     DrawText(hdc, s_caption, -1, &r, DT_CENTER | DT_CALCRECT);
     DrawText(hdc, s_caption, -1, &r, DT_CENTER );

     FrameRect(hdc, &r, GetStockBrush(BLACK_BRUSH));

     if(!ModifyWorldTransform(hdc, &xf->x, MWT_LEFTMULTIPLY))
        { /* failed */
         PostMessage(hwnd, UWM_ERROR, xf->caption, IDS_TRANSFORM_FAILED);
        } /* failed */
     else
        { /* success */
         SelectBrush(hdc, GetStockBrush(DKGRAY_BRUSH));
         Rectangle(hdc, XFORM_SIZE, 0, 0, XFORM_SIZE/2);
        } /* success */


     RestoreDC(hdc, orgdc);

     DeletePen(axispen);
     DeleteFont(font);
    }

/****************************************************************************
*                                xform_OnPaint
* Inputs:
*       HDC dc: Display context
* Result: void
*       
* Effect: 
*       Shows graphical transforms
****************************************************************************/

static void
xform_OnPaint(HWND hwnd, HDC hdc)
    {
#define XFORM_OFFSET (1.5f * (float)XFORM_SIZE)
     int i;
     XFORM initialize = {1.2f,                         0.0f,
                         0.0f,                         1.2f,
                         (XFORM_OFFSET/2.0f), -XFORM_OFFSET};

     XFORM translate = { 1.0f,                  0.0f,
                         0.0f,                  1.0f,
                         (XFORM_OFFSET * 1.5f), 0.0f};

     int restore = SaveDC(hdc);


     SetGraphicsMode(hdc, GM_ADVANCED);

     if(!ModifyWorldTransform(hdc, &initialize, MWT_LEFTMULTIPLY))
        return;

     for(i = 0; xf[i].caption != NULL; i++)
        { /* show each */
         drawTransform(hwnd, hdc, &xf[i]);
         ModifyWorldTransform(hdc, &translate, MWT_RIGHTMULTIPLY);
        } /* show each */

     RestoreDC(hdc, restore);
     
    }

/****************************************************************************
*                              textxform_OnPaint
* Inputs:
*       HDC hdc: DC to draw in
* Result: void
*       
* Effect: 
*       Draws a shadowed text
****************************************************************************/

static void
textxform_OnPaint(HWND hwnd, HDC hdc)
    {
     int restore = SaveDC(hdc);
     int localrestore;
     SIZE size;
     XFORM shear = { 1.0f,   0.0f,
                    -1.6f,   1.3f,
                     0.0f,   0.0f};
     
#define SHEAR_X 5.0f
     XFORM initialize = { 1.0f, 0.0f,
                          0.0f, 1.0f,
                          SHEAR_X, 0.0f};

     HFONT font;
     TCHAR Message[100];

     font = createCaptionFont(-50, TimesFont);
     SelectFont(hdc, font);

     LoadString(GetWindowInstance(hwnd), IDS_SHADOW_MESSAGE, Message,
                                        DIM(Message));

     GetTextExtentPoint32(hdc, Message, lstrlen(Message), &size);

     initialize.eDy = 1.5f * (float)size.cy;

     scaleToWindow(hdc, hwnd,  size.cx, (int) (2.0f * (float)size.cy));

     SetGraphicsMode(hdc, GM_ADVANCED);
     if(!ModifyWorldTransform(hdc, &initialize, MWT_LEFTMULTIPLY))
        return;
         

     SetBkMode(hdc, TRANSPARENT);
     SetTextAlign(hdc, TA_BASELINE);

     //----------------------
     localrestore = SaveDC(hdc);


     SetTextColor(hdc, RGB(192,192,192));
     if(ModifyWorldTransform(hdc, &shear, MWT_LEFTMULTIPLY))
        { /* success */
         TextOut(hdc, 0, 0, Message, lstrlen(Message));
        } /* success */
     else
        { /* failure */
         PostMessage(hwnd, UWM_ERROR, IDS_SHEAR_TRANSFORMATION_FAILED,
                                        IDS_NOT_SUPPORTED);
        } /* failure */
     
     RestoreDC(hdc, localrestore);

     //----------------------
     localrestore = SaveDC(hdc);
     
     TextOut(hdc, 0, 0, Message, lstrlen(Message));

     RestoreDC(hdc, localrestore);
     //----------------------

     RestoreDC(hdc, restore);

     DeleteFont(font);
    }

/****************************************************************************
*                                 arc_OnPaint
* Inputs:
*       HDC hdc: DC to write into
* Result: void
*       
* Effect: 
*       Illustrates an arc drawn by the Arc or ArcTo functions
****************************************************************************/

#define MARKGAP 10
#define MARKLENG 50

// Note that we use English literals here instead of resources simply
// because these are the formal names used in the parameter names

static TCHAR xULLabel[]    = _T("xUL");
static TCHAR yULLabel[]    = _T("yUL");
static TCHAR xLRLabel[]    = _T("xLR");
static TCHAR yLRLabel[]    = _T("yLR");
static TCHAR xBeginLabel[] = _T("xBegin");
static TCHAR yBeginLabel[] = _T("yBegin");
static TCHAR xEndLabel[]   = _T("xEnd");
static TCHAR yEndLabel[]   = _T("yEnd");
static TCHAR xposLabel[]   = _T("xCur");
static TCHAR yposLabel[]   = _T("yCur");

static CBoundingBox
arc_OnPaint(HWND hwnd, HDC hdc, int id)
    {
     int xUL;
     int yUL;
     int xLR;
     int yLR;
     int xBegin;
     int yBegin;
     int xEnd;
     int yEnd;
     int arctox;
     int arctoy;

     RECT rect;
     int odc;
     HPEN hpenDot;
     HPEN hpenThin;
     HPEN hpenThick;
     SIZE size;
     HFONT font;

     CBoundingBox bbdata(FALSE, TRUE);

     // Initialize the parameters to the Arc function
     xUL    = -300;
     yUL    =  200;
     xLR    =  300;
     yLR    = -200;
     xBegin = -150;
     yBegin = -250;
     xEnd   = 0;
     yEnd   = 250;
     arctox = -100;
     arctoy = 50;



     // Get the device context
     // and establish the coordinate mapping

     // Save the display context
     
     odc = SaveDC(hdc);

     GetClientRect(hwnd, &rect);
     SetMapMode(hdc, MM_ISOTROPIC);
     SetViewportOrgEx(hdc, rect.right / 2, rect.bottom / 2, NULL);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, rect.right, - rect.bottom, NULL);

     // Create the drawing tools

     hpenDot = CreatePen(PS_DOT, 0, RGB(0,0,0));
     hpenThin = CreatePen(PS_SOLID, 0, RGB(0,0,0));
     hpenThick = CreatePen(PS_SOLID, 5, RGB(0,0,0));

     font = createCaptionFont(-30, ArialFont);

     SelectFont(hdc, font);

     // Use the dotted pen to...

     SelectPen(hdc, hpenDot);

     // ...draw the rectangle and inscribed ellipse.

     bbdata.Set(xUL, yUL, xLR, yLR);

     Rectangle(hdc, xUL, yUL, xLR, yLR);
     Ellipse  (hdc, xUL, yUL, xLR, yLR);

     // Draw the imaginary lines from the begin and end
     // points to the center of the rectangle

     MoveToEx(hdc, xBegin, yBegin, NULL);
     LineTo(hdc, 0, 0);
     LineTo(hdc, xEnd, yEnd);

     bbdata.Set(xBegin, yBegin);
     bbdata.Set(xEnd, yEnd);

     // Use the solid thin pen to...
     
     SelectPen(hdc, hpenThin);

     // Draw the point markers

     MoveToEx (hdc, xUL - MARKGAP,     yUL, NULL);
     bbdata.Set(    xUL - MARKGAP,     yUL);

     LineTo   (hdc, xUL - MARKLENG,    yUL);
     bbdata.Set(    xUL - MARKLENG,    yUL);

     MoveToEx (hdc, xUL,               yUL + MARKGAP, NULL);
     bbdata.Set(    xUL,               yUL + MARKGAP);

     LineTo   (hdc, xUL,               yUL + MARKLENG);
     bbdata.Set(    xUL,               yUL + MARKLENG);

     MoveToEx (hdc, xLR + MARKGAP,     yLR, NULL);
     bbdata.Set(    xLR + MARKGAP,     yLR);

     LineTo   (hdc, xLR + MARKLENG,    yLR);
     bbdata.Set(    xLR + MARKLENG,    yLR);

     MoveToEx (hdc, xLR,               yLR - MARKGAP, NULL);
     bbdata.Set(    xLR,               yLR - MARKGAP);

     LineTo   (hdc, xLR,               yLR - MARKLENG);
     bbdata.Set(    xLR,               yLR - MARKLENG);

     MoveToEx (hdc, xBegin - MARKGAP,  yBegin, NULL);
     bbdata.Set(    xBegin - MARKGAP,  yBegin);

     LineTo   (hdc, xBegin - MARKLENG, yBegin);
     bbdata.Set(    xBegin - MARKLENG, yBegin);

     MoveToEx (hdc, xBegin,            yBegin - MARKGAP, NULL);
     bbdata.Set(    xBegin,            yBegin - MARKGAP);

     LineTo   (hdc, xBegin,            yBegin - MARKLENG);
     bbdata.Set(    xBegin,            yBegin - MARKLENG);
     
     MoveToEx (hdc, xEnd + MARKGAP,    yEnd, NULL);
     bbdata.Set(    xEnd + MARKGAP,    yEnd);

     LineTo   (hdc, xEnd + MARKLENG,   yEnd);
     bbdata.Set(    xEnd + MARKLENG,   yEnd);

     MoveToEx (hdc, xEnd,              yEnd + MARKGAP, NULL);
     bbdata.Set(    xEnd,              yEnd + MARKGAP);

     LineTo   (hdc, xEnd,              yEnd + MARKLENG);
     bbdata.Set(    xEnd,              yEnd + MARKLENG);

     // We don't need to deal with bbdata in here because this is within
     // the space

     if(id == IDC_ARCTO)
        { /* arcto */
         MoveToEx(hdc, arctox - MARKGAP,arctoy, NULL);
         LineTo(hdc, arctox - MARKLENG,  arctoy);

         MoveToEx(hdc, arctox,          arctoy + MARKGAP, NULL);
         LineTo(hdc,   arctox,          arctoy + MARKLENG);

         GetTextExtentPoint32(hdc, yposLabel, lstrlen(yposLabel), &size);
         TextOut(hdc, 
                        arctox - 2 * MARKGAP - MARKLENG - size.cx, 
                        arctoy + size.cy / 2,
                        yposLabel, lstrlen(yposLabel));

         GetTextExtentPoint32(hdc, xposLabel, lstrlen(xposLabel), &size);
         TextOut(hdc,
                        arctox,
                        arctoy + MARKGAP + MARKLENG + size.cy,
                        xposLabel, lstrlen(xposLabel));

        } /* arcto */

     // Place labels by the markers

     // Put out the xUL label

     GetTextExtentPoint32(hdc, yULLabel, lstrlen(yULLabel), &size);

     TextOut(hdc,
                xUL - 2 * MARKGAP - MARKLENG - size.cx,
                yUL + size.cy / 2,
                yULLabel, lstrlen(yULLabel));

     bbdata.Set(xUL - 2 * MARKGAP - MARKLENG - size.cx, yUL - size.cy / 2);
     // Put out the yUL label

     GetTextExtentPoint32(hdc, xULLabel, lstrlen(xULLabel), &size);

     TextOut(hdc,
                xUL - size.cx / 2,
                yUL + MARKGAP + MARKLENG + size.cy,
                xULLabel, lstrlen(xULLabel));

     bbdata.Set(xUL - size.cx / 2, yUL + MARKGAP + MARKLENG + size.cy);

     // Put out the xLR label

     GetTextExtentPoint32(hdc, yLRLabel, lstrlen(yLRLabel), &size);

     TextOut(hdc,
                xLR + 2 * MARKGAP + MARKLENG,
                yLR + size.cy / 2,
                yLRLabel, lstrlen(yLRLabel));

     bbdata.Set(xLR + 2 * MARKGAP + MARKLENG + size.cx, yLR + size.cy / 2);

     // Put out the yLR label

     GetTextExtentPoint32(hdc, xLRLabel, lstrlen(xLRLabel), &size);

     TextOut(hdc,
                xLR - size.cx / 2,
                yLR - MARKGAP - MARKLENG,
                xLRLabel, lstrlen(xLRLabel));

     bbdata.Set(xLR - size.cx / 2, yLR - MARKGAP - MARKLENG);

     // Put out the yBegin label

     GetTextExtentPoint32(hdc, yBeginLabel, lstrlen(yBeginLabel), &size);

     TextOut(hdc,
                xBegin - 2 * MARKGAP - MARKLENG - size.cx,
                yBegin + size.cy / 2,
                yBeginLabel, lstrlen(yBeginLabel));
     bbdata.Set(xBegin - 2 * MARKGAP - MARKLENG - size.cx, yBegin + size.cy / 2);

     // Put out the xBegin label

     GetTextExtentPoint32(hdc, xBeginLabel, lstrlen(xBeginLabel), &size);

     TextOut(hdc,
                xBegin - size.cx / 2,
                yBegin - MARKGAP - MARKLENG,
                xBeginLabel, lstrlen(xBeginLabel));

     bbdata.Set(xBegin - size.cx / 2, yBegin - MARKGAP - MARKLENG - size.cy);

     // Put out the xEnd label

     GetTextExtentPoint32(hdc, yEndLabel, lstrlen(yEndLabel), &size);

     TextOut(hdc,
                xEnd + 2 * MARKGAP + MARKLENG,
                yEnd + size.cy / 2,
                yEndLabel, lstrlen(yEndLabel));

     bbdata.Set(xEnd + 2 * MARKGAP + MARKLENG, yEnd + size.cy / 2);

     // Put out the yEnd label

     GetTextExtentPoint32(hdc, xEndLabel, lstrlen(xEndLabel), &size);

     TextOut(hdc,
                xEnd - size.cx / 2,
                yEnd + MARKGAP + MARKLENG + size.cy,
                xEndLabel, lstrlen(xEndLabel));

     bbdata.Set(xEnd - size.cx / 2, yEnd + MARKGAP + MARKLENG + size.cy);


     // Use the solid thick pen to...

     SelectPen(hdc, hpenThick);

     // SetArcDirection(hdc, AD_CLOCKWISE);
     // SetArcDirection(hdc, AD_COUNTERCLOCKWISE);
     // draw the arc

     switch(id)
        { /* id */
         case IDC_ARC:
                 Arc(hdc, xUL, yUL, xLR, yLR, xBegin, yBegin, xEnd, yEnd);
                 break;
         case IDC_ARCTO:
                 MoveToEx(hdc, arctox, arctoy, NULL);
                 ArcTo(hdc, xUL, yUL, xLR, yLR, xBegin, yBegin, xEnd, yEnd);
                 break;
         case IDC_CHORD:
                 SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));
                 Chord(hdc, xUL, yUL, xLR, yLR, xBegin, yBegin, xEnd, yEnd);
                 break;
         case IDC_PIE:
                 SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));
                 Pie(hdc, xUL, yUL, xLR, yLR, xBegin, yBegin, xEnd, yEnd);
                 break;
        } /* id */

     bbdata.LPtoDP(CDC::FromHandle(hdc));

     RestoreDC(hdc, odc);

     DeletePen(hpenDot);
     DeletePen(hpenThin);
     DeletePen(hpenThick);
     DeleteFont(font);
     return bbdata;
    }

/****************************************************************************
*                                rect_OnPaint
* Inputs:
*       HWND hwnd: WIndow reference
*       HDC hdc: DC
*       int id: IDC_RECTANGLE | IDC_ELLIPSE
* Result: CBoundingBox
*       Bounding box of image
* Effect: 
*       Draws a rectangle or an ellipse
* Notes:
*       This computes a maximum bounding rectangle in a global variable.
****************************************************************************/

static CBoundingBox
rect_OnPaint(HWND hwnd, HDC hdc, int id)
    {
     int xUL;
     int yUL;
     int xLR;
     int yLR;

     RECT rect;
     int odc;
     HPEN hpenDot;
     HPEN hpenThin;
     HPEN hpenThick;
     SIZE size;
     HFONT font;
     CBoundingBox bbdata(FALSE, TRUE);
     
     // Initialize the parameters to the Rectangle/Ellipse function
     xUL    = -300;
     yUL    =  200;
     xLR    =  300;
     yLR    = -200;


     bbdata.Set(xUL, xLR, yUL, yLR);

     updateGlobalBB(&bbdata);

     // Save the display context
     
     odc = SaveDC(hdc);

     // Get the device context
     // and establish the coordinate mapping

     GetClientRect(hwnd, &rect);
     SetMapMode(hdc, MM_ISOTROPIC);
     SetViewportOrgEx(hdc, rect.right / 2, rect.bottom / 2, NULL);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, rect.right, - rect.bottom, NULL);

     // Create the drawing tools

     hpenDot = CreatePen(PS_DOT, 0, RGB(0,0,0));
     hpenThin = CreatePen(PS_SOLID, 0, RGB(0,0,0));
     hpenThick = CreatePen(PS_SOLID, 5, RGB(0,0,0));

     font = createCaptionFont(-14, ArialFont);

     // Use the dotted pen to...

     SelectPen(hdc, hpenDot);

     // ...draw the rectangle

     Rectangle(hdc, xUL, yUL, xLR, yLR);

     // Use the solid thin pen to...
     
     SelectPen(hdc, hpenThin);

     // Draw the point markers

     MoveToEx (hdc, xUL - MARKGAP,     yUL, NULL);
     bbdata.Set(    xUL - MARKGAP,     yUL);

     LineTo   (hdc, xUL - MARKLENG,    yUL);
     bbdata.Set(    xUL - MARKLENG,    yUL);

     MoveToEx (hdc, xUL,               yUL + MARKGAP, NULL);
     bbdata.Set(    xUL,               yUL + MARKGAP);

     LineTo   (hdc, xUL,               yUL + MARKLENG);
     bbdata.Set(    xUL,               yUL + MARKLENG);

     MoveToEx (hdc, xLR + MARKGAP,     yLR, NULL);
     bbdata.Set(    xLR + MARKGAP,     yLR);

     LineTo   (hdc, xLR + MARKLENG,    yLR);
     bbdata.Set(    xLR + MARKLENG,    yLR);

     MoveToEx (hdc, xLR,               yLR - MARKGAP, NULL);
     bbdata.Set(    xLR,               yLR - MARKGAP);

     LineTo   (hdc, xLR,               yLR - MARKLENG);
     bbdata.Set(    xLR,               yLR - MARKLENG);

     // Place labels by the markers

     // Put out the yUL label

     GetTextExtentPoint32(hdc, yULLabel, lstrlen(yULLabel), &size);


     bbdata.Set(xUL - 2 * MARKGAP - MARKLENG - size.cx,
                yUL + size.cy / 2,
                xUL - 2 * MARKGAP - MARKLENG - size.cx + size.cx,
                yUL + size.cy / 2 + size.cy);

     updateGlobalBB(&bbdata);

     TextOut(hdc, xUL - 2 * MARKGAP - MARKLENG - size.cx, yUL + size.cy / 2,
                yULLabel, lstrlen(yULLabel));

     // Put out the xUL label

     GetTextExtentPoint32(hdc, xULLabel, lstrlen(xULLabel), &size);

     bbdata.Set(xUL - size.cx / 2,
                yUL + MARKGAP + MARKLENG + size.cy,
                xUL - size.cx / 2 + size.cx,
                yUL + MARKGAP + MARKLENG + size.cy);

     updateGlobalBB(&bbdata);

     TextOut(hdc, xUL - size.cx / 2, yUL + MARKGAP + MARKLENG + size.cy,
                xULLabel, lstrlen(xULLabel));


     // Put out the yLR label

     GetTextExtentPoint32(hdc, yLRLabel, lstrlen(yLRLabel), &size);

     bbdata.Set(xLR + 2 * MARKGAP + MARKLENG,
                yLR + size.cy / 2,
                xLR + 2 * MARKGAP + MARKLENG + size.cx,
                yLR + size.cy / 2 + size.cy);

     updateGlobalBB(&bbdata);

     TextOut(hdc, xLR + 2 * MARKGAP + MARKLENG, yLR + size.cy / 2,
                yLRLabel, lstrlen(yLRLabel));

     // Put out the yLR label

     GetTextExtentPoint32(hdc, xLRLabel, lstrlen(xLRLabel), &size);

     bbdata.Set(xLR - size.cx / 2,
                yLR - MARKGAP - MARKLENG,
                xLR - size.cx / 2 + size.cx,
                yLR - MARKGAP - MARKLENG - size.cy);

     updateGlobalBB(&bbdata);

     TextOut(hdc, xLR - size.cx / 2, yLR - MARKGAP - MARKLENG,
                xLRLabel, lstrlen(xLRLabel));

     // Use the solid thick pen to...

     SelectPen(hdc, hpenThick);

     // draw the object

     switch(id)
        { /* id */
         case IDC_ELLIPSE:
                 SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));
                 Ellipse(hdc, xUL, yUL, xLR, yLR);
                 break;
         case IDC_RECTANGLE:
                 SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));
                 Rectangle(hdc, xUL, yUL, xLR, yLR);
                 break;
        } /* id */

     bbdata.LPtoDP(CDC::FromHandle(hdc));
     RestoreDC(hdc, odc);

     DeletePen(hpenDot);
     DeletePen(hpenThin);
     DeletePen(hpenThick);
     DeleteFont(font);
     return bbdata;
    }

/****************************************************************************
*                              roundrect_OnPaint
* Inputs:
*       HWND hwnd: WIndow reference
*       HDC hdc: DC
* Result: void
*       
* Effect: 
*       Draws a rounded rectangle showing the ellipses
****************************************************************************/

static TCHAR HeightLabel[50];
static TCHAR WidthLabel[50];

static CBoundingBox
roundrect_OnPaint(HWND hwnd, HDC hdc)
    {
     int xUL;
     int yUL;
     int xLR;
     int yLR;
     int Height;
     int Width;

     RECT rect;
     int odc;
     HPEN hpenDot;
     HPEN hpenThin;
     HPEN hpenThick;
     SIZE size;
     HFONT font;
     CBoundingBox bbdata(FALSE, TRUE);

     // Initialize the parameters to the Rectangle/Ellipse function
     xUL    = -300;
     yUL    =  200;
     xLR    =  300;
     yLR    = -200;
     Height =  100;
     Width  =  150;


     bbdata.Set(xUL, xLR, yUL, yLR);

     updateGlobalBB(&bbdata);

     LoadString(GetWindowInstance(hwnd), IDS_HEIGHT, HeightLabel, DIM(HeightLabel));
     LoadString(GetWindowInstance(hwnd), IDS_WIDTH, WidthLabel, DIM(WidthLabel));

     // Save the display context
     
     odc = SaveDC(hdc);

     // Get the device context
     // and establish the coordinate mapping

     GetClientRect(hwnd, &rect);
     SetMapMode(hdc, MM_ISOTROPIC);
     SetViewportOrgEx(hdc, rect.right / 2, rect.bottom / 2, NULL);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, rect.right, - rect.bottom, NULL);

     // Create the drawing tools

     hpenDot = CreatePen(PS_DOT, 0, RGB(0,0,0));
     hpenThin = CreatePen(PS_SOLID, 0, RGB(0,0,0));
     hpenThick = CreatePen(PS_SOLID, 5, RGB(0,0,0));

     font = createCaptionFont(-14, ArialFont);

     // Use the solid thin pen to...
     
     SelectPen(hdc, hpenThin);

     // Draw the point markers

     // xUL
     MoveToEx (hdc, xUL - MARKGAP,     yUL, NULL);
     bbdata.Set(    xUL - MARKGAP,     yUL);

     LineTo   (hdc, xUL - MARKLENG,    yUL);
     bbdata.Set(    xUL - MARKLENG,    yUL);

     // yUL
     MoveToEx (hdc, xUL,               yUL + MARKGAP, NULL);
     bbdata.Set(    xUL,               yUL + MARKGAP);

     LineTo   (hdc, xUL,               yUL + MARKLENG);
     bbdata.Set(    xUL,               yUL + MARKLENG);

     // xLR
     MoveToEx (hdc, xLR + MARKGAP,     yLR, NULL);
     bbdata.Set(    xLR + MARKGAP,     yLR);

     LineTo   (hdc, xLR + MARKLENG,    yLR);
     bbdata.Set(    xLR + MARKLENG,    yLR);

     // yLR
     MoveToEx (hdc, xLR,               yLR - MARKGAP, NULL);
     bbdata.Set(    xLR,               yLR - MARKGAP);

     LineTo   (hdc, xLR,               yLR - MARKLENG);
     bbdata.Set(    xLR,               yLR - MARKLENG);

     // nWidth left
     MoveToEx (hdc, xUL,               yLR - MARKGAP, NULL);
     bbdata.Set(    xUL,               yLR - MARKGAP);

     LineTo   (hdc, xUL,               yLR - MARKLENG);
     bbdata.Set(    xUL,               yLR - MARKLENG);

     // nWidth right
     MoveToEx (hdc, xUL + Width,       yLR - MARKGAP, NULL);
     bbdata.Set(    xUL + Width,       yLR - MARKGAP);

     LineTo   (hdc, xUL + Width,       yLR - MARKLENG);
     bbdata.Set(    xUL + Width,       yLR - MARKLENG);

     // nHeight bottom
     MoveToEx (hdc, xUL - MARKGAP,     yLR, NULL);
     bbdata.Set(    xUL - MARKGAP,     yLR);

     LineTo   (hdc, xUL - MARKLENG,    yLR);
     bbdata.Set(    xUL - MARKLENG,    yLR);

     // nHeight top
     MoveToEx (hdc, xUL - MARKGAP,     yLR + Height, NULL);
     bbdata.Set(    xUL - MARKGAP,     yLR + Height);

     LineTo   (hdc, xUL - MARKLENG,    yLR + Height);
     bbdata.Set(    xUL - MARKLENG,    yLR + Height);

     // Place labels by the markers

     // Put out the xUL label

     GetTextExtentPoint32(hdc, yULLabel, lstrlen(yULLabel), &size);

     bbdata.Set(xUL - 2 * MARKGAP - MARKLENG - size.cx,
                yUL + size.cy / 2,
                xUL - 2 * MARKGAP - MARKLENG,
                yUL + size.cy / 2 + size.cy);

     TextOut(hdc,
                xUL - 2 * MARKGAP - MARKLENG - size.cx,
                yUL + size.cy / 2,
                yULLabel, lstrlen(yULLabel));

     // Put out the yUL label

     GetTextExtentPoint32(hdc, xULLabel, lstrlen(xULLabel), &size);

     bbdata.Set(xUL - size.cx / 2,
                yUL + MARKGAP + MARKLENG + size.cy,
                xUL - size.cx / 2 + size.cx,
                yUL + MARKGAP + MARKLENG);

     TextOut(hdc,
                xUL - size.cx / 2,
                yUL + MARKGAP + MARKLENG + size.cy,
                xULLabel, lstrlen(xULLabel));


     // Put out the yLR label

     GetTextExtentPoint32(hdc, yLRLabel, lstrlen(yLRLabel), &size);

     bbdata.Set(xLR + 2 * MARKGAP + MARKLENG,
                yLR + size.cy / 2,
                xLR + 2 * MARKGAP + MARKLENG + size.cx,
                yLR + size.cy / 2 + size.cy);

     TextOut(hdc,
                xLR + 2 * MARKGAP + MARKLENG,
                yLR + size.cy / 2,
                yLRLabel, lstrlen(yLRLabel));

     // Put out the xLR label

     GetTextExtentPoint32(hdc, xLRLabel, lstrlen(xLRLabel), &size);

     bbdata.Set(xLR - size.cx / 2,
                yLR - MARKGAP - MARKLENG,
                xLR - size.cx / 2 + size.cx,
                yLR - MARKGAP - MARKLENG - size.cy);

     TextOut(hdc,
                xLR - size.cx / 2,
                yLR - MARKGAP - MARKLENG,
                xLRLabel, lstrlen(xLRLabel));

     // Put out the height label

     GetTextExtentPoint32(hdc, HeightLabel, lstrlen(HeightLabel), &size);

     bbdata.Set(xUL - size.cx - MARKGAP,
                yLR + Height / 2 + size.cy / 2,
                xUL - size.cx - MARKGAP + size.cx,
                yLR + Height / 2 + size.cy / 2 + size.cy);

     TextOut(hdc,
                xUL - size.cx - MARKGAP,
                yLR + Height / 2 + size.cy / 2,
                HeightLabel, lstrlen(HeightLabel));


      // Put out the width label

      GetTextExtentPoint32(hdc, WidthLabel, lstrlen(WidthLabel), &size);

      bbdata.Set(xUL + (Width - size.cx) / 2,
                 yLR - MARKGAP - MARKLENG,
                 xUL + (Width - size.cx) / 2 + size.cx,
                 yLR - MARKGAP - MARKLENG - size.cy);

      TextOut(hdc,
                xUL + (Width - size.cx) / 2,
                yLR - MARKGAP,
                WidthLabel, lstrlen(WidthLabel));

     // Use the dotted pen...

     SelectPen(hdc, hpenDot);

     // to draw the rectangle...
     Rectangle(hdc, xUL, yUL, xLR, yLR);

     // Use the solid thick pen to...

     SelectPen(hdc, hpenThick);

     // draw the object

     SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));
     RoundRect(hdc, xUL, yUL, xLR, yLR, Width, Height);

     // Use the dotted pen to...
     
     SelectPen(hdc, hpenDot);
     SetBkMode(hdc, TRANSPARENT); // fill with background

     // ...draw the ellipses

     Ellipse(hdc, xUL,         yUL,          xUL + Width, yUL - Height);
     Ellipse(hdc, xLR - Width, yUL,          xLR,         yUL - Height);
     Ellipse(hdc, xUL,         yLR + Height, xUL + Width, yLR);
     Ellipse(hdc, xLR - Width, yLR + Height, xLR,         yLR);

     // Now redraw the outline of the rounded rectangle
     SelectPen(hdc, hpenThick);
     SelectBrush(hdc, GetStockObject(NULL_BRUSH));
     RoundRect(hdc, xUL, yUL, xLR, yLR, Width, Height);

     bbdata.LPtoDP(CDC::FromHandle(hdc));

     RestoreDC(hdc, odc);

     DeletePen(hpenDot);
     DeletePen(hpenThin);
     DeletePen(hpenThick);
     DeleteFont(font);

     return bbdata;
    }

/****************************************************************************
*                                anglearc_draw
* Inputs:
*       HDC hdc: Display context
*       HPEN hpen: Pen to use for drawing arc
*       int radius: Radius of circle
*       FLOAT start: Start angle
*       FLOAT angle: Sweep angle
*       LPPOINT result: End position location, or NULL to ignore
* Result: void
*       
* Effect: 
*       Draws an arc using the pen selected.
****************************************************************************/

static void 
anglearc_draw(HDC hdc, HPEN hpen, LPPOINT center, int radius, 
                        FLOAT start, 
                        FLOAT angle,
                        LPPOINT result)
    {
     int saved = SaveDC(hdc);

         SelectObject(hdc, hpen);
         AngleArc(hdc, center->x, center->y, radius, start, angle);
         if(result != NULL)
            GetCurrentPositionEx(hdc, result);

     RestoreDC(hdc, saved);
    }

/****************************************************************************
*                              anglearc_OnPaint
* Inputs:
*       HWND hwnd:
*       HDC hdc:
* Result: void
*       
* Effect: 
*       Draws an AngleArc
****************************************************************************/

#define AA_X0 0
#define AA_Y0 0
#define AA_RADIUS 400
#define AA_SIZE  (AA_RADIUS + 40 + MARKLENG)

static TCHAR dwRadiusLabel[] = _T("dwRadius");
static TCHAR eSweepAngleLabel[] = _T("eSweepAngle");
static TCHAR eStartAngleLabel[] = _T("eStartAngle");

static void
anglearc_OnPaint(HWND hwnd, HDC hdc)
    {
     POINT pt;
     POINT endpt;
     RECT rect;
     POINT center;
     int saved;
     HPEN drawpen;
     POINT sweepcaption;
     POINT sstart; // start point for sweep line
     POINT send;   // end point for sweep line  
     SIZE size;    // string size
     RECT caption; // caption rectangle
     
         saved = SaveDC(hdc);
     GetClientRect (hwnd, &rect) ;
     SetViewportOrgEx (hdc, rect.right / 2, rect.bottom / 2, NULL) ;
     
     scaleToWindow(hdc, hwnd, 2 * AA_SIZE, AA_SIZE);

     MoveToEx(hdc, AA_X0, AA_Y0, NULL);

     drawpen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));

     GetCurrentPositionEx(hdc, &pt);
     center.x = pt.x - AA_RADIUS;
     center.y = pt.y;

     // Draw the arc

     anglearc_draw(hdc, drawpen, &center, AA_RADIUS, 0.0f, -30.0f, &endpt);

     DeletePen(drawpen);

     drawpen = CreatePen(PS_SOLID, 0, RGB(192, 192, 192));

     // Draw the indicator of the sweep angle

     MoveToEx(hdc, MARKLENG / 2, center.y, NULL);

     anglearc_draw(hdc, drawpen, &center, AA_RADIUS + MARKLENG / 2, 
                                                0.0f, -30.0f, NULL);

     // Draw the radius distance

     MoveToEx(hdc, center.x, center.y - MARKLENG / 2, NULL);
     LineTo(hdc,   center.x + AA_RADIUS, center.y - MARKLENG / 2);

     DeletePen(drawpen);

     GetTextExtentPoint32(hdc, dwRadiusLabel, lstrlen(dwRadiusLabel), &size);

     caption.left = center.x;
     caption.right = caption.left + AA_RADIUS;

     caption.bottom = center.y - MARKLENG;
     caption.top  = caption.bottom - size.cy;

     DrawText(hdc, dwRadiusLabel, -1, &caption, DT_CENTER | DT_VCENTER);

     // Draw the start angle caption

     GetTextExtentPoint32(hdc, eStartAngleLabel, lstrlen(eStartAngleLabel),
                                        &size);


     TextOut(hdc, center.x + AA_RADIUS + MARKLENG + MARKGAP,
                  center.y - size.cy / 2,
                  eStartAngleLabel,
                  lstrlen(eStartAngleLabel));

     // Get a point where we can put the "sweep" caption, and record
     // the points where we draw the sweep line

     MoveToEx(hdc, AA_X0 + MARKGAP, AA_Y0, NULL);

     anglearc_draw(hdc, GetStockPen(NULL_PEN), 
                        &center,
                        AA_RADIUS + MARKLENG, 
                        0.0f, -15.0f, &sweepcaption);

     TextOut(hdc, sweepcaption.x, sweepcaption.y, 
                        eSweepAngleLabel,
                        lstrlen(eSweepAngleLabel));

     MoveToEx(hdc, AA_X0 + MARKGAP, AA_Y0, NULL);

     anglearc_draw(hdc, GetStockPen(NULL_PEN),
                        &center,
                        AA_RADIUS + MARKGAP,
                        0.0f, -30.0f, &sstart);

     MoveToEx(hdc, AA_X0 + MARKLENG, AA_Y0, NULL);

     anglearc_draw(hdc, GetStockPen(NULL_PEN),
                        &center,
                        AA_RADIUS + MARKLENG,
                        0.0f, -30.0f, &send);

     // Draw the line to indicate the end angle

     MoveToEx(hdc, sstart.x, sstart.y, NULL);
     LineTo(hdc,   send.x, send.y);


     // Draw the lines to indicate the center of the circle

     MoveToEx(hdc, center.x - MARKGAP, center.y, NULL);
     LineTo(hdc,   center.x - MARKLENG, center.y);

     MoveToEx(hdc, center.x + MARKGAP, center.y, NULL);
     LineTo(hdc,   center.x + MARKLENG, center.y);

     MoveToEx(hdc, center.x, center.y - MARKGAP, NULL);
     LineTo(hdc,   center.x, center.y - MARKLENG);

     MoveToEx(hdc, center.x, center.y + MARKGAP, NULL);
     LineTo(hdc,   center.x, center.y + MARKLENG);

     // Draw the captions for the center of the circle

     GetTextExtentPoint32(hdc, _T("X"), 1, &size);

     TextOut(hdc, center.x - size.cx / 2,
                  center.y - MARKLENG - size.cy,
                  _T("X"), 1);

     GetTextExtentPoint32(hdc, _T("Y"), 1, &size);

     TextOut(hdc, center.x - MARKLENG - 2 * size.cx,
                  center.y - size.cy / 2,
                  _T("Y"), 1);

     // Draw the center of the circle

     MoveToEx(hdc, center.x, center.y, NULL);
     Ellipse(hdc, center.x - 2, center.y - 2, center.x + 2, center.y + 2);

     // Draw the mark to indicate the start angle
     MoveToEx(hdc, center.x + AA_RADIUS + MARKGAP, center.y, NULL);
     LineTo(hdc,   center.x + AA_RADIUS + MARKLENG, center.y);

     // Draw the mark to indicate the radius
     MoveToEx(hdc, center.x + AA_RADIUS, center.y - MARKGAP, NULL);
     LineTo(hdc,   center.x + AA_RADIUS, center.y - MARKLENG);

     RestoreDC(hdc, saved);
    }

/****************************************************************************
*                              penstyle_OnPaint
* Inputs:
*       HDC hdc: DC
* Result: void
*       
* Effect: 
*       Draws the pen styles
****************************************************************************/

#define XTEXT 40
#define YPEN  40
#define PENLENG 100
#define PENWIDTH 10  // for PS_INSIDEFRAME example

static TCHAR ps_dashdotdotsp[] = _T("PS_DASHDOTDOT  ");
static TCHAR ps_solid[] = _T("PS_SOLID");
static TCHAR ps_dash[] = _T("PS_DASH");
static TCHAR ps_dot[] = _T("PS_DOT");
static TCHAR ps_dashdot[] = _T("PS_DASHDOT");
static TCHAR ps_dashdotdot[] = _T("PS_DASHDOTDOT");
static TCHAR ps_null[] = _T("PS_NULL");
static TCHAR ps_insideframe[] = _T("PS_INSIDEFRAME");

static void 
penstyle_OnPaint(HDC hdc)
    {
     HPEN pen;
     SIZE size;
     HFONT font = createCaptionFont(-14, ArialFont);
     int yPen = YPEN;
     int xPen = XTEXT;
     int ifY;           // PS_INSIDEFRAME y-coordinate
     int restore = SaveDC(hdc);
     
     SelectFont(hdc, font);

     GetTextExtentPoint32(hdc, ps_dashdotdotsp, lstrlen(ps_dashdotdotsp), &size);

     xPen += size.cx;

     
     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_solid, lstrlen(ps_solid));
     pen = CreatePen(PS_SOLID, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     yPen += 2 * size.cy;

     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_dash, lstrlen(ps_dash));
     pen = CreatePen(PS_DASH, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     yPen += 2 * size.cy;

     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_dot, lstrlen(ps_dot));
     pen = CreatePen(PS_DOT, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     yPen += 2 * size.cy;

     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_dashdot, lstrlen(ps_dashdot));
     pen = CreatePen(PS_DASHDOT, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     yPen += 2 * size.cy;

     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_dashdotdot, lstrlen(ps_dashdotdot));
     pen = CreatePen(PS_DASHDOTDOT, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     yPen += 2 * size.cy;

     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_null, lstrlen(ps_null));
     pen = CreatePen(PS_NULL, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     yPen += 2 * size.cy;

     ifY = yPen;
     TextOut(hdc, XTEXT, yPen - size.cy / 2, ps_insideframe, lstrlen(ps_insideframe));
     pen = CreatePen(PS_INSIDEFRAME, 0, RGB(0,0,0));
     SelectPen(hdc, pen);
     MoveToEx(hdc, xPen, yPen, NULL);
     LineTo(hdc, xPen + PENLENG, yPen);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     DeletePen(pen);
     
     // Now show the PS_SOLID example

     SelectBrush(hdc, GetStockBrush(NULL_BRUSH));
     pen = CreatePen(PS_SOLID, PENWIDTH, RGB(192,192,192));
     SelectPen(hdc, pen);
     Rectangle(hdc, xPen + PENLENG + 2 * PENWIDTH, YPEN,
                    xPen + 2 * PENLENG + 2 * PENWIDTH, YPEN + PENLENG / 2);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     Rectangle(hdc, xPen + PENLENG + 2 * PENWIDTH, YPEN,
                    xPen + 2 * PENLENG + 2 * PENWIDTH, YPEN + PENLENG / 2);
     DeletePen(pen);
     
     // and the PS_INSIDEFRAME example
     pen = CreatePen(PS_INSIDEFRAME, PENWIDTH, RGB(192, 192, 192));
     SelectPen(hdc, pen);
     Rectangle(hdc, xPen + PENLENG + 2 * PENWIDTH, ifY,
                    xPen + 2 * PENLENG + 2 * PENWIDTH, ifY + PENLENG / 2);
     SelectPen(hdc, GetStockPen(BLACK_PEN));
     Rectangle(hdc, xPen + PENLENG + 2 * PENWIDTH, ifY,
                    xPen + 2 * PENLENG + 2 * PENWIDTH, ifY + PENLENG / 2);
     DeletePen(pen);

     RestoreDC(hdc, restore);
     DeleteFont(font);
    }

/****************************************************************************
*                                poly_OnPaint
* Inputs:
*       HDC hdc:
* Result: void
*       
* Effect: 
*       Polygon painter for example
****************************************************************************/

POINT points[] = {
        { 200, 400},
        { 200,   0},
        { 500,   0},
        { 500, 300},
        { 100, 300},
        { 100, 200},
        { 400, 200},
        { 400, 100},
        { 300, 100},
        { 300, 400}
                 };

static void
poly_OnPaint(HWND hwnd, HDC hdc)
    {
     RECT rect;
     int restore;
     
     restore = SaveDC(hdc);
     GetClientRect(hwnd, &rect);
     SetMapMode(hdc, MM_ISOTROPIC);
     SetViewportOrgEx(hdc, rect.right / 2, rect.bottom / 2, NULL);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, rect.right, -rect.bottom, NULL);

     SelectBrush(hdc, GetStockBrush(LTGRAY_BRUSH));

     // Draw the right-hand polygon
     Polygon(hdc, points, DIM(points));

     // Invert the coordinate system axes
     SetViewportExtEx(hdc, -rect.right, rect.bottom, NULL);
     
     // Change the polygon filling mode
     SetPolyFillMode(hdc, WINDING);

     // Draw the left-hand polygon
     Polygon(hdc, points, DIM(points));
     
     RestoreDC(hdc, restore);
    }

/****************************************************************************
*                              polypoly_OnPaint
* Inputs:
*       HWND hwnd: Window handle
*       HDC hdc: Display context
* Result: void
*       
* Effect: 
*       Draws the same polygons as poly_OnPaint but uses the PolyPolygon
*       function to do so
****************************************************************************/

POINT pp_points [] = {
        { 200, 400 },        /* Right polygon */
        { 200,   0 },
        { 500,   0 },
        { 500, 300 },
        { 100, 300 },
        { 100, 200 },
        { 400, 200 },
        { 400, 100 },
        { 300, 100 },
        { 300, 400 },

        { -200, -400 },        /* Left polygon */
        { -200,   0 },
        { -500,   0 },
        { -500, -300 },
        { -100, -300 },
        { -100, -200 },
        { -400, -200 },
        { -400, -100 },
        { -300, -100 },
        { -300, -400 }
                  } ;
int PolyCounts [] = { 10, 10 } ;

void polypoly_OnPaint(HWND hwnd, HDC hdc, PolyPolyData * pd)
    {
     RECT rect;
     HBRUSH hbr;
     int cxClient;
     int cyClient;

     /* Get the present size of the client area. */
     GetClientRect (hwnd, &rect) ;
     cxClient = rect.right ;
     cyClient = rect.bottom ;

     /* Get the device context and        */
     /* establish the coordinate mapping. */
     SetMapMode (hdc, MM_ISOTROPIC) ;
     SetViewportOrgEx (hdc, cxClient / 2, cyClient / 2, NULL) ;
     SetWindowExtEx (hdc, 1000, 1000, NULL) ;
     SetViewportExtEx (hdc, cxClient, -cyClient, NULL) ;

     hbr = GetStockBrush (LTGRAY_BRUSH) ;
     SelectBrush (hdc, hbr) ;

     /* Change the polygon-filling mode. */
     SetPolyFillMode (hdc, pd->fillmode) ;

     /* Draw the polygons. */
     PolyPolygon (hdc, pp_points, (pd->points == NULL ? PolyCounts
                                                      : pd->points), 
                                  (pd->points == NULL ? 2 : pd->count)) ;
    }

/****************************************************************************
*                              drawFocusExample
* Inputs:
*       HDC hdc: DC
*       LPRECT rect: Delimiting rectangle
*       BOOL withpen: TRUE to draw with pen, FALSE to draw with DrawFocusRect
* Result: void
*       
* Effect: 
*       Draws the example for the DrawFocusRect illustration
****************************************************************************/

static void
drawFocusExample(HDC hdc, LPRECT rect, BOOL withpen, HPEN hpenThin, HPEN hpenDot, CBoundingBox * bbdata)
    {
     SIZE size;

     // Use the solid thin pen to...
     
     SelectPen(hdc, hpenThin);

     // Draw the point markers

     MoveToEx (hdc, rect->left - MARKGAP,     rect->top, NULL);
     bbdata->Set(   rect->left - MARKGAP,     rect->top);

     LineTo   (hdc, rect->left - MARKLENG,    rect->top);
     bbdata->Set(   rect->left - MARKLENG,    rect->top);

     MoveToEx (hdc, rect->left,               rect->top + MARKGAP, NULL);
     bbdata->Set(   rect->left,               rect->top + MARKGAP);

     LineTo   (hdc, rect->left,               rect->top + MARKLENG);
     bbdata->Set(   rect->left,               rect->top + MARKLENG);

     MoveToEx (hdc, rect->right + MARKGAP,     rect->bottom, NULL);
     bbdata->Set(   rect->right + MARKGAP,     rect->bottom);

     LineTo   (hdc, rect->right + MARKLENG,    rect->bottom);
     bbdata->Set(   rect->right + MARKLENG,    rect->bottom);

     MoveToEx (hdc, rect->right,               rect->bottom - MARKGAP, NULL);
     bbdata->Set(   rect->right,               rect->bottom - MARKGAP);

     LineTo   (hdc, rect->right,               rect->bottom - MARKLENG);
     bbdata->Set(   rect->right,               rect->bottom - MARKLENG);

     // Place labels by the markers

     // Put out the rect->left label

     GetTextExtentPoint32(hdc, yULLabel, lstrlen(yULLabel), &size);

     TextOut(hdc,
                rect->left - 2 * MARKGAP - MARKLENG - size.cx,
                rect->top + size.cy / 2,
                yULLabel, lstrlen(yULLabel));

     bbdata->Set(rect->left - 2 * MARKGAP - MARKLENG - size.cx,
                 rect->top + size.cy / 2,
                 rect->left - 2 * MARKGAP - MARKLENG,
                 rect->top - size.cy / 2);

     // Put out the rect->top label

     GetTextExtentPoint32(hdc, xULLabel, lstrlen(xULLabel), &size);

     TextOut(hdc,
                rect->left - size.cx / 2,
                rect->top + MARKGAP + MARKLENG + size.cy,
                xULLabel, lstrlen(xULLabel));

     bbdata->Set(rect->left - size.cx / 2,
                 rect->top + MARKGAP + MARKLENG + size.cy,
                 rect->left + size.cx / 2,
                 rect->top + MARKGAP + MARKLENG - size.cy);

     // Put out the rect->right label

     GetTextExtentPoint32(hdc, yLRLabel, lstrlen(yLRLabel), &size);

     TextOut(hdc,
                rect->right + 2 * MARKGAP + MARKLENG,
                rect->bottom + size.cy / 2,
                yLRLabel, lstrlen(yLRLabel));

     bbdata->Set(rect->right + 2 * MARKGAP + MARKLENG,
                 rect->bottom + size.cy / 2,
                 rect->right + 2 * MARKGAP + MARKLENG + size.cx,
                 rect->bottom - size.cy / 2);

     // Put out the rect->bottom label

     GetTextExtentPoint32(hdc, xLRLabel, lstrlen(xLRLabel), &size);

     TextOut(hdc,
                rect->right - size.cx / 2,
                rect->bottom - MARKGAP - MARKLENG,
                xLRLabel, lstrlen(xLRLabel));
     bbdata->Set(rect->right - size.cx / 2,
                 rect->bottom - MARKGAP - MARKLENG,
                 rect->right - size.cx / 2 + size.cx,
                 rect->bottom - MARKGAP - MARKLENG - size.cy);

     // ...draw the rectangle

     if(withpen)
        { /* focus w/Rectangle */
         int odc = SaveDC(hdc);
         SelectPen(hdc, hpenDot);
         SelectBrush(hdc, GetStockBrush(NULL_BRUSH));
         SetROP2(hdc, R2_XORPEN);
         Rectangle(hdc, rect->left, rect->top, rect->right, rect->bottom);
         RestoreDC(hdc, odc);
        } /* focus w/Rectangle */
     else
        { /* focus rect */
         int odc = SaveDC(hdc);
         LPtoDP(hdc, (LPPOINT)rect, 2);
         SetMapMode(hdc, MM_TEXT);
         SetViewportOrgEx(hdc, 0, 0, NULL);
         DrawFocusRect(hdc, rect);
         RestoreDC(hdc, odc);
        } /* focus rect */
    }

/****************************************************************************
*                             draw_focus_OnPaint
* Inputs:
*       HWND hwnd: Window handle
*       HDC hdc: DC
* Result: CBoundingBox
*       Bounding box used for screen capture
* Effect: 
*       Draws a focus rectangle
****************************************************************************/

static CBoundingBox
drawfocus_OnPaint(HWND hwnd, HDC hdc)
    {
     int xUL;
     int yUL;
     int xLR;
     int yLR;

     RECT rect;
     CBoundingBox bbdata(FALSE, TRUE);

     HFONT font;
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0),       0 };
     int odc;
     HPEN hpenDot;
     HPEN hpenThin;
     int offset;

     xUL    = -300;
     yUL    =  200;
     xLR    =  300;
     yLR    = -200;
     
     // Save the display context
     
     odc = SaveDC(hdc);

     // Get the device context
     // and establish the coordinate mapping

     GetClientRect(hwnd, &rect);
     SetMapMode(hdc, MM_ISOTROPIC);
     SetViewportOrgEx(hdc, rect.right / 2, rect.bottom / 2, NULL);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, rect.right, - rect.bottom, NULL);

     // Create the drawing tools

     hpenDot = ExtCreatePen(PS_COSMETIC | PS_ALTERNATE, 1, &blackbrush, 0, NULL);
     hpenThin = CreatePen(PS_SOLID, 0, RGB(0,0,0));

     font = createCaptionFont(-30, ArialFont);


     SetRect(&rect, xUL, yUL, xLR, yLR);
     bbdata.Set( xUL, yUL, xLR, yLR);

     offset = 0;
     // offset = rect.right - rect.left;

     SelectFont(hdc, font);

     // Use the dotted pen to...

     SelectPen(hdc, hpenDot);

     // drawFocusExample(hdc, &rect, TRUE, hpenThin, hpenDot);

     offset += offset / 2;
     OffsetRect(&rect, offset, 0);

     drawFocusExample(hdc, &rect, FALSE, hpenThin, hpenDot, &bbdata);

     bbdata.LPtoDP(CDC::FromHandle(hdc));

     RestoreDC(hdc, odc);

     DeletePen(hpenDot);
     DeletePen(hpenThin);
     DeleteFont(font);

     return bbdata;
    }

/****************************************************************************
*                             hatchbrush_OnPaint
* Inputs:
*       HDC hdc: Display context
* Result: void
*       
* Effect: 
*       Draws samples of all the brushes
****************************************************************************/

static TCHAR hs_horizontalsp[] = _T("HS_HORIZONTAL  ");
static TCHAR hs_bdiagonal[]    = _T("HS_BDIAGONAL");
static TCHAR hs_cross[]        = _T("HS_CROSS");
static TCHAR hs_diagcross[]    = _T("HS_DIAGCROSS");
static TCHAR hs_fdiagonal[]    = _T("HS_FDIAGONAL");
static TCHAR hs_horizontal[]   = _T("HS_HORIZONTAL");
static TCHAR hs_vertical[]     = _T("HS_VERTICAL");

#define YBRUSH  40

static void
hatchbrush_OnPaint(HWND hwnd, HDC hdc)
    {
     HBRUSH brush;
     SIZE size;
     HFONT font = createCaptionFont(-14, ArialFont);
     int yBrush = YBRUSH;
     int xBrush = XTEXT;
     int restore = SaveDC(hdc);
     
     SelectFont(hdc, font);
     SelectPen(hdc, GetStockPen(NULL_PEN));

     GetTextExtentPoint32(hdc, hs_horizontalsp, lstrlen(hs_horizontalsp), &size);

     scaleToWindow(hdc, hwnd, xBrush + PENLENG, 14 * size.cy); 


     xBrush += size.cx;

     
     TextOut(hdc, XTEXT, yBrush, hs_bdiagonal, lstrlen(hs_bdiagonal));
     brush = CreateHatchBrush(HS_BDIAGONAL, RGB(0,0,0));
     SelectBrush(hdc, brush);
     Rectangle(hdc, xBrush, yBrush, xBrush + PENLENG, yBrush + size.cy);
     SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
     DeleteBrush(brush);
     
     yBrush += 2 * size.cy;

     TextOut(hdc, XTEXT, yBrush, hs_cross, lstrlen(hs_cross));
     brush = CreateHatchBrush(HS_CROSS, RGB(0,0,0));
     SelectBrush(hdc, brush);
     Rectangle(hdc, xBrush, yBrush, xBrush + PENLENG, yBrush + size.cy);
     SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
     DeleteBrush(brush);
     
     yBrush += 2 * size.cy;

     TextOut(hdc, XTEXT, yBrush, hs_diagcross, lstrlen(hs_diagcross));
     brush = CreateHatchBrush(HS_DIAGCROSS, RGB(0,0,0));
     SelectBrush(hdc, brush);
     Rectangle(hdc, xBrush, yBrush, xBrush + PENLENG, yBrush + size.cy);
     SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
     DeleteBrush(brush);
     
     yBrush += 2 * size.cy;

     TextOut(hdc, XTEXT, yBrush, hs_fdiagonal, lstrlen(hs_fdiagonal));
     brush = CreateHatchBrush(HS_FDIAGONAL, RGB(0,0,0));
     SelectBrush(hdc, brush);
     Rectangle(hdc, xBrush, yBrush, xBrush + PENLENG, yBrush + size.cy);
     SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
     DeleteBrush(brush);
     
     yBrush += 2 * size.cy;

     TextOut(hdc, XTEXT, yBrush, hs_horizontal, lstrlen(hs_horizontal));
     brush = CreateHatchBrush(HS_HORIZONTAL, RGB(0,0,0));
     SelectBrush(hdc, brush);
     Rectangle(hdc, xBrush, yBrush, xBrush + PENLENG, yBrush + size.cy);
     SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
     DeleteBrush(brush);
     
     yBrush += 2 * size.cy;

     TextOut(hdc, XTEXT, yBrush, hs_vertical, lstrlen(hs_vertical));
     brush = CreateHatchBrush(HS_VERTICAL, RGB(0,0,0));
     SelectBrush(hdc, brush);
     Rectangle(hdc, xBrush, yBrush, xBrush + PENLENG, yBrush + size.cy);
     SelectBrush(hdc, GetStockBrush(BLACK_BRUSH));
     DeleteBrush(brush);
     
     RestoreDC(hdc, restore);
     DeleteFont(font);

    }

#define STROKE_LENGTH 50
#define STROKE_X1 (2 * STROKE_LENGTH)
#define STROKE_Y1 (STROKE_LENGTH / 2)
#define STROKE_X2 (STROKE_X1 - STROKE_LENGTH)
#define STROKE_Y2 (STROKE_Y1 + STROKE_LENGTH)
#define STROKE_X3 (STROKE_X1 + STROKE_LENGTH)
#define STROKE_Y3 STROKE_Y2
#define STROKE_SPACING (2.7f * (float)STROKE_LENGTH)    

#define STROKE_VERT_GAP (STROKE_LENGTH / 5)

#define OPEN_PATH  0
#define CLOSE_PATH 1
#define DRAW_PATH  2

#define STROKE_PEN_WIDTH 15

/****************************************************************************
*                                  drawPath
* Inputs:
*       HDC hDC: Display context
*       int closed: Closing option, see Effect
*       HINSTANCE hinst: Instance of where to retrieve string
*       int caption: String ID of caption to display, or 0 for no caption
* Result: void
*       
* Effect: 
*       Draws a path given the stroke parameters.  There are three points
*       used, which form an isoceles triangle.  They are points p1, p2 and p3
*               OPEN_PATH:  p1->p2->p3
*               CLOSE_PATH: p1->p2->p3->p1  via CloseFigure
*               DRAW_PATH:  p1->p2->p3->p1  by explicit line drawing
****************************************************************************/

static void drawPath(HDC hdc, int closed, HINSTANCE hinst, int caption)
    {
     RECT r;

     BeginPath(hdc);
     MoveToEx(hdc, STROKE_X1, STROKE_Y1, NULL);
     LineTo(hdc, STROKE_X2, STROKE_Y2);
     LineTo(hdc, STROKE_X3, STROKE_Y3);

     // Based on the closed parameter, select the action following the
     // second path segment
     switch(closed)
        { /* what close option? */
         case OPEN_PATH:
                 // do nothing.  Path is open
                 break; 
         case CLOSE_PATH:
                 // Close the figure; let the GDI compute the path segment
                 // from p3 to p1
                 CloseFigure(hdc);
                 break;
         case DRAW_PATH:
                 // Draw an explicit line from p3 to p1
                 LineTo(hdc, STROKE_X1, STROKE_Y1);
                 break;
        } /* what close option? */

     EndPath(hdc);

     // If a caption is present, draw it.
     if(caption != 0)
        { /* has caption */
         TEXTMETRIC tm;
         HFONT font = createCaptionFont(-14, ArialFont);
         int saved = SaveDC(hdc);
         TCHAR text[256];

         LoadString(hinst, caption, text, DIM(text));

         SelectObject(hdc, font);
         GetTextMetrics(hdc, &tm);
         SetRect(&r, STROKE_X2, STROKE_Y2 + STROKE_VERT_GAP, STROKE_X3,
                            STROKE_Y2 + STROKE_VERT_GAP + 
                                2 *(tm.tmHeight + tm.tmExternalLeading));

         DrawText(hdc, text, -1, &r, DT_CENTER | DT_WORDBREAK);
         RestoreDC(hdc, saved);
         DeleteFont(font);
        } /* has caption */
    }

/****************************************************************************
*                                  saf_APath
* Inputs:
*       HDC hdc: DC to draw into
* Result: void
*       
* Effect: 
*       Draws a dotted path, p1->p2->p3
****************************************************************************/

static void saf_APath(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     HPEN pen;

     drawPath(hdc, OPEN_PATH, GetWindowInstance(hwnd), IDS_A_PATH);
     
     pen = CreatePen(PS_DOT, 0, RGB(0,0,0));
     SelectObject(hdc, pen);
     StrokePath(hdc);

     RestoreDC(hdc, horzsave);
     
     DeletePen(pen);
    }

/****************************************************************************
*                               saf_StrokedPath
* Inputs:
*       HDC hdc
* Result: void
*       
* Effect: 
*       Draws a stroked path, p1->p2->p3
****************************************************************************/

static void saf_StrokedPath(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     HPEN pen;
     
     drawPath(hdc, OPEN_PATH, GetWindowInstance(hwnd), IDS_STROKED_PATH);
     
     pen = CreatePen(PS_SOLID, 0, RGB(0,0,0));
     SelectObject(hdc, pen);
     StrokePath(hdc);

     RestoreDC(hdc, horzsave);
     DeletePen(pen);
    }

/****************************************************************************
*                             saf_StrokedPathWide
* Inputs:
*       HDC hdc: DC to draw into
* Result: void
*       
* Effect: 
*       Draws a stroked path with a wide brush, p1->p2->p3
****************************************************************************/

static void saf_StrokedPathWide(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     int pathsave;
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0), 0 };
     HPEN pen;
     HPEN whpen;
     
     drawPath(hdc, OPEN_PATH, GetWindowInstance(hwnd), IDS_WIDE_STROKED);
     
     pen = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_MITER,
                                STROKE_PEN_WIDTH, &blackbrush, 0, NULL);

     SelectObject(hdc, pen);

     pathsave = SaveDC(hdc);
     StrokePath(hdc);
     RestoreDC(hdc, pathsave);

     whpen = CreatePen(PS_DOT, 1, RGB(255,255,255));
     SelectObject(hdc, whpen);
     SetBkMode(hdc, TRANSPARENT);

     StrokePath(hdc);
     RestoreDC(hdc, horzsave);

     DeletePen(pen);
     DeletePen(whpen);
    }

/****************************************************************************
*                              saf_ClosedFigure
* Inputs:
*       HDC hdc: DC to draw into
* Result: void
*       
* Effect: 
*       Draws a closed figure p1->p2->p3->p1
****************************************************************************/

static void saf_ClosedFigure(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     HPEN pen;

     drawPath(hdc, CLOSE_PATH, GetWindowInstance(hwnd), IDS_CLOSED_FIGURE);

     pen = CreatePen(PS_SOLID, 0, RGB(0,0,0));

     SelectObject(hdc, pen);
     StrokePath(hdc);

     RestoreDC(hdc, horzsave);

     DeletePen(pen);
    }

/****************************************************************************
*                             saf_UnclosedFigure
* Inputs:
*       HDC hdc: DC to draw into
* Result: void
*       
* Effect: 
*       Draws an unclosed figure by drawing a third path segment back to the
*       origin, p1->p2->p3->p1
****************************************************************************/

static void saf_UnclosedFigure(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     HPEN pen;

     drawPath(hdc, DRAW_PATH, GetWindowInstance(hwnd), IDS_UNCLOSED_FIGURE);

     pen = CreatePen(PS_SOLID, 0, RGB(0,0,0));

     SelectObject(hdc, pen);
     StrokePath(hdc);

     RestoreDC(hdc, horzsave);

     DeletePen(pen);
     
    }

/****************************************************************************
*                            saf_ClosedFigureWide
* Inputs:
*       HDC hdc: DC to write into
* Result: void
*       
* Effect: 
*       Draws a closed figure with a wide pen,
*       p1->p2->p3->p1
****************************************************************************/

static void saf_ClosedFigureWide(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     int pathsave;
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0), 0 };
     HPEN pen;
     HPEN whpen;

     drawPath(hdc, CLOSE_PATH, GetWindowInstance(hwnd), IDS_CLOSED_WIDE);
     pen = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_MITER,
                                STROKE_PEN_WIDTH, &blackbrush, 0, NULL);

     SelectObject(hdc, pen);

     pathsave = SaveDC(hdc);
     StrokePath(hdc);
     RestoreDC(hdc, pathsave);

     whpen = CreatePen(PS_DOT, 1, RGB(255,255,255));
     SelectObject(hdc, whpen);
     SetBkMode(hdc, TRANSPARENT);

     StrokePath(hdc);
     RestoreDC(hdc, horzsave);
     DeletePen(pen);
     DeletePen(whpen);
     
    }

/****************************************************************************
*                               saf_FilledPath
* Inputs:
*       HDC hdc: DC to draw into
* Result: void
*       
* Effect: 
*       Fills the figure defined by the path
****************************************************************************/

static void saf_FilledPath(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);

     drawPath(hdc, CLOSE_PATH, GetWindowInstance(hwnd), IDS_FILLED_PATH);
     SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
     FillPath(hdc);
     RestoreDC(hdc, horzsave);
    }

/****************************************************************************
*                            saf_StrokedAndFilled
* Inputs:
*       HDC hdc: DC
* Result: void
*       
* Effect: 
*       Strokes and filles a path
****************************************************************************/

static void saf_StrokedAndFilled(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     int pathsave;
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0), 0 };
     HPEN pen;
     HPEN whpen;

     drawPath(hdc, CLOSE_PATH, GetWindowInstance(hwnd), IDS_STROKED_AND_FILLED);
     SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
     pen = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_MITER,
                                STROKE_PEN_WIDTH, &blackbrush, 0, NULL);
     SelectObject(hdc, pen);

     pathsave = SaveDC(hdc);
     StrokeAndFillPath(hdc);
     RestoreDC(hdc, pathsave);

     whpen = CreatePen(PS_DOT, 1, RGB(255,255,255));
     SelectObject(hdc, whpen);
     SetBkMode(hdc, TRANSPARENT);

     StrokePath(hdc);

     RestoreDC(hdc, horzsave);
     DeletePen(pen);
     DeletePen(whpen);
     
    }

/****************************************************************************
*                              saf_UnclosedWide
* Inputs:
*       HDC hdc: Display context to write to
* Result: void
*       
* Effect: 
*       Draws a wide figure with an unclosed figure path
****************************************************************************/

static void saf_UnclosedWide(HWND hwnd, HDC hdc)
    {
     int horzsave = SaveDC(hdc);
     int pathsave;
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0), 0 };
     HPEN pen;
     HPEN whpen;

     drawPath(hdc, DRAW_PATH, GetWindowInstance(hwnd), IDS_UNCLOSED_WIDE);

     pen = ExtCreatePen(PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_MITER,
                                STROKE_PEN_WIDTH, &blackbrush, 0, NULL);

     SelectPen(hdc, pen);

     pathsave = SaveDC(hdc);
     StrokePath(hdc);
     RestoreDC(hdc, pathsave);

     whpen = CreatePen(PS_DOT, 1, RGB(255,255,255));
     SelectObject(hdc, whpen);
     SetBkMode(hdc, TRANSPARENT);

     StrokePath(hdc);

     RestoreDC(hdc, horzsave);
     DeletePen(pen);
     DeletePen(whpen);
     
    }

/****************************************************************************
*                                   shiftBy
* Inputs:
*       HDC hdc: Display context
*       XFORM * shift: A transform containing only a significant vertical
*                      or horizontal linear motion.
* Result: void
*       
* Effect: 
*       On Win95, changes the origin; otherwise does a world transform
****************************************************************************/

void shiftBy(HDC hdc, XFORM * shift)
    {
     if(!Is95())
        ModifyWorldTransform(hdc, shift, MWT_RIGHTMULTIPLY);
     else
        { /* shift origin */
         ASSERT(shift->eM11 == 1.0f);
         ASSERT(shift->eM12 == 0.0f);
         ASSERT(shift->eM21 == 0.0f);
         ASSERT(shift->eM22 == 1.0f);

         int dx = (int)shift->eDx;
         int dy = (int)shift->eDy;
         
         POINT pt;
         GetWindowOrgEx(hdc, &pt);
         pt.x -= dx;
         pt.y -= dy;
         SetWindowOrgEx(hdc, pt.x, pt.y, NULL);
        } /* shift origin */
     
    }

/****************************************************************************
*                            strokeandfill_OnPaint
* Inputs:
*       HDC hDC: Display context
* Result: void
*       
* Effect: 
*       Shows the stroking and filling options
****************************************************************************/

static void strokeandfill_OnPaint(HWND hwnd, HDC hdc)
    {
     XFORM ShiftRight = { 1.0f,                               0.0f,
                          0.0f,                               1.0f,
                          STROKE_SPACING,                     0.0f};
     XFORM ShiftDown  = { 1.0f,                               0.0f,
                          0.0f,                               1.0f,
                          0.0f,                         STROKE_SPACING};
     int vertsave;

     SetGraphicsMode(hdc, GM_ADVANCED);

     vertsave = SaveDC(hdc);

     //================================================================-
     // ROW 1
     //================================================================-

     saf_APath(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     saf_StrokedPath(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     saf_StrokedPathWide(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     saf_ClosedFigure(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     saf_ClosedFigureWide(hwnd, hdc);

     //================================================================-
     // Move down to the next row
     //================================================================-
     RestoreDC(hdc, vertsave);

     shiftBy(hdc, &ShiftDown);
     vertsave = SaveDC(hdc);

     //================================================================-
     // ROW 2
     //================================================================-

     saf_UnclosedFigure(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     saf_UnclosedWide(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     saf_FilledPath(hwnd, hdc);
     
     shiftBy(hdc, &ShiftRight);

     saf_StrokedAndFilled(hwnd, hdc);

     shiftBy(hdc, &ShiftRight);

     //================================================================-
     RestoreDC(hdc, vertsave);
    }

/****************************************************************************
*                               complex_OnPaint
* Inputs:
*       HWND hwnd:
*       HDC hdc:
* Result: void
*       
* Effect: 
*       Draws an object that resembles a printer flowchart symbol
****************************************************************************/

#define COMPLEX_BASE 5

#define COMPLEX_X0 10
#define COMPLEX_Y0 10
#define COMPLEX_WIDTH (16 * COMPLEX_BASE)
#define COMPLEX_LEFT_HEIGHT (16 * COMPLEX_BASE)
#define COMPLEX_RIGHT_HEIGHT (12 * COMPLEX_BASE)
#define COMPLEX_X1 COMPLEX_X0
#define COMPLEX_Y1 (COMPLEX_Y0 + COMPLEX_LEFT_HEIGHT)
#define COMPLEX_X2 (COMPLEX_X0 + COMPLEX_WIDTH)
#define COMPLEX_Y2 (COMPLEX_Y0 + COMPLEX_RIGHT_HEIGHT)
#define COMPLEX_X3 (COMPLEX_X2)
#define COMPLEX_Y3 (COMPLEX_Y0)
#define COMPLEX_CAPTION_Y (COMPLEX_X0 + 3 * COMPLEX_BASE)
#define COMPLEX_OFFSET_Y1 (7 * COMPLEX_BASE)
#define COMPLEX_OFFSET_Y2 (5 * COMPLEX_BASE)
#define COMPLEX_FONTSIZE (-2 * COMPLEX_BASE)

#define COMPLEX_TOTAL_HEIGHT (1.5f * COMPLEX_Y1)
#define COMPLEX_DX 10.0f
#define COMPLEX_DY 10.0f

static void
complex_OnPaint(HWND hwnd, HDC hdc)
    {
     int save = SaveDC(hdc);
     POINT pt = {COMPLEX_X0, COMPLEX_CAPTION_Y };
     SIZE size;
     TCHAR caption[128];
     HFONT font = createCaptionFont(COMPLEX_FONTSIZE, ArialFont);
     POINT bottom[] = {
                        {COMPLEX_X1 + (COMPLEX_X2 - COMPLEX_X1) / 3, 
                                             COMPLEX_Y2 + COMPLEX_OFFSET_Y1},
                        {COMPLEX_X1 + 2 * (COMPLEX_X2 - COMPLEX_X1) / 3, 
                                             COMPLEX_Y2 - COMPLEX_OFFSET_Y2},
                        {COMPLEX_X2, COMPLEX_Y2}};


     SetGraphicsMode(hdc, GM_ADVANCED);

     // Compute the scaling so it is sized to the window

     scaleToWindow(hdc, hwnd, COMPLEX_WIDTH, (int)COMPLEX_TOTAL_HEIGHT);

     LoadString(GetWindowInstance(hwnd), IDS_COMPLEX_CAPTION, caption, 
                                                          DIM(caption));


     BeginPath(hdc);
     MoveToEx(hdc, COMPLEX_X0, COMPLEX_Y0, NULL);
     LineTo(hdc, COMPLEX_X1, COMPLEX_Y1);
     
     PolyBezierTo(hdc, bottom, 3);

     LineTo(hdc, COMPLEX_X3, COMPLEX_Y3);

     CloseFigure(hdc);

     // Compute a rectangle that centers the caption
     SelectFont(hdc, font);
     GetTextExtentPoint32(hdc, caption, lstrlen(caption), &size);

     // Now center the rectangle horizontally
     pt.x += ((COMPLEX_X2 - COMPLEX_X0) - size.cx) / 2;

     SetBkMode(hdc, TRANSPARENT);
     SetPolyFillMode(hdc, ALTERNATE);
     TextOut(hdc, pt.x, pt.y, caption, lstrlen(caption));
     
     EndPath(hdc);

     SelectBrush(hdc, GetStockObject(LTGRAY_BRUSH));
     StrokeAndFillPath(hdc);

#if 0
     {
      int i;
      for(i = 0; i < 3; i++)
         { /* each point */
          MoveToEx(hdc, bottom[i].x - 10, bottom[i].y, NULL);
          LineTo(hdc, bottom[i].x + 10, bottom[i].y);
          MoveToEx(hdc, bottom[i].x, bottom[i].y - 10, NULL);
          LineTo(hdc, bottom[i].x, bottom[i].y + 10);
         } /* each point */
     }
#endif

     RestoreDC(hdc, save);
     DeleteFont(font);
    }

/****************************************************************************
*                                  randomize
* Inputs:
*       int n: Range of random number: 0..n-1
* Result: int
*       Random integer in specified range
* Effect: 
*       Generates the next random number
****************************************************************************/

static int randomize(int n)
    {
     int s = rand();
     return s % n;
    }

/****************************************************************************
*                                   randpt
* Inputs:
*       LPPOINT pt: Point to randomize
*       LPPOINT limits: limit values
* Result: void
*       
* Effect: 
*       Creates two random points within the limits
****************************************************************************/

static void randpt(LPPOINT pt, LPPOINT limits)
    {
     pt->x = randomize(limits->x);
     pt->y = randomize(limits->y);
    }

/****************************************************************************
*                               random_OnPaint
* Inputs:
*       HWND hwnd:
*       HDC hdc:
* Result: void
*       
* Effect: 
*       Paints a random polygon on the screen
****************************************************************************/

static void random_OnPaint(HWND hwnd, HDC hdc, RandomData * rd)
    {
     RECT client;
     int sides;
     POINT pt;
     POINT limits;
     int i;
     int save = SaveDC(hdc);

     SetGraphicsMode(hdc, GM_ADVANCED);
     GetClientRect(hwnd, &client);

     InflateRect(&client, -5, -5);  // leave some margin

     limits.x = client.right;
     limits.y = client.bottom;
     srand((UINT)rd->seed);

     if(rd->RandomSides)
        while( (sides = randomize(40)) < 3) ;  // max number of sides
     else
        sides = rd->sides;

     BeginPath(hdc);

     randpt(&pt, &limits);
     MoveToEx(hdc, pt.x, pt.y, NULL);

     for(i = 0; i < sides - 1; i++) // use - 1 because CloseFigure gives side
        { /* generate each side */
         randpt(&pt, &limits);
         LineTo(hdc, pt.x, pt.y);
        } /* generate each side */

     CloseFigure(hdc);
     EndPath(hdc);

     switch(rd->fillmode)
        { /* decode */
         case WINDING:
         case ALTERNATE:
                 SetPolyFillMode(hdc, rd->fillmode);
                 SelectBrush(hdc, GetStockObject(LTGRAY_BRUSH));
                 break;
         default:
                 SetPolyFillMode(hdc, WINDING);
                 break;
        } /* decode */

     StrokeAndFillPath(hdc);

     RestoreDC(hdc, save);
    }

/****************************************************************************
*                              bmpbrush_OnPaint
* Inputs:
*       HWND hwnd:
*       HDC hdc:
* Result: void
*       
* Effect: 
*       Draws a couple areas and fills them with a bitmap brush
****************************************************************************/

static void
bmpbrush_OnPaint(HWND hwnd, HDC hdc)
    {
     int xUL    = -300;
     int yUL    =  200;
     int xLR    =  300;
     int yLR    = -200;
     int xBegin = -250;
     int yBegin = -100;
     int xEnd   = 0;
     int yEnd   = 250;
     RECT rect;
     RECT r;
#define BMP_CAPTION_OFFSET (-80)
     HFONT font = createCaptionFont(-(abs(BMP_CAPTION_OFFSET)), TimesFont);
     HBRUSH brush;
     HBITMAP bmp;
     int save = SaveDC(hdc);
     SIZE size;

     // Get the device context
     // and establish the coordinate mapping

     GetClientRect(hwnd, &rect);
     SetMapMode(hdc, MM_ISOTROPIC);
     SetViewportOrgEx(hdc, rect.right / 2, rect.bottom / 2, NULL);
     SetWindowExtEx(hdc, 1000, 1000, NULL);
     SetViewportExtEx(hdc, rect.right, - rect.bottom, NULL);

     bmp = LoadBitmap(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDB_MARBLE));
     brush = CreatePatternBrush(bmp);
     
     // Do a drop-shadow 3-D effect here
#define BMP_SHADOW (-20)
     SelectBrush(hdc, GetStockObject(BLACK_BRUSH));
     Ellipse(hdc, xUL, yUL+BMP_SHADOW, xLR, yLR+BMP_SHADOW);

     SelectBrush(hdc, GetStockObject(WHITE_BRUSH));
     Ellipse(hdc, xUL, yUL, xLR, yLR);


     SelectBrush(hdc, brush);
     Pie(hdc, xUL, yUL, xLR, yLR, xBegin, yBegin, xEnd, yEnd);

     r = rect;
     DPtoLP(hdc, (LPPOINT)&r, 2);
     r.top = yLR + BMP_CAPTION_OFFSET;
     r.bottom = yLR + 2 * BMP_CAPTION_OFFSET;
     SelectFont(hdc, font);


     ids_GetTextExtentPoint32(GetWindowInstance(hwnd), hdc, IDS_BMPBRUSH, &size);
     r.bottom = r.top - size.cy;

     ids_DrawText(GetWindowInstance(hwnd), hdc, IDS_BMPBRUSH, &r, 
                        DT_CENTER);

     RestoreDC(hdc, save);

     DeleteFont(font);
     DeleteBrush(brush);
     DeleteBitmap(bmp);
    }

/****************************************************************************
*                              geopen_drawsample
* Inputs:
*       HDC hdc: DC to draw in
*       int y:
*       int len: Length of line
*       HPEN pen: Pen to use
* Result: void
*       
* Effect: 
*       Draws a line
****************************************************************************/

static void geopen_drawsample(HDC hdc, int y, int len, HPEN pen)
    {
#define GEO_X 50     
     int saved = SaveDC(hdc);

     SelectPen(hdc, pen);
     MoveToEx(hdc, GEO_X, y, NULL);
     LineTo(hdc, GEO_X + len, y);
     RestoreDC(hdc, saved);
    }

/****************************************************************************
*                               geopen_OnPaint
* Inputs:
*       HWND hwnd: Window
*       HDC hdc: DC
* Result: void
*       
* Effect: 
*       Draws some geometric pen examples
****************************************************************************/

static void geopen_OnPaint(HWND hwnd, HDC hdc)
    {
     HPEN pen;
     LOGBRUSH blackbrush = {BS_SOLID, RGB(0,0,0), 0};
#define DASH 40
#define DOT  10
#define SP   10
     DWORD SOS[] = {DOT,  SP, DOT,  SP, DOT,  SP, SP,
                    DASH, SP, DASH, SP, DASH, SP, SP,
                    DOT,  SP, DOT,  SP, DOT,  SP, SP};

     pen = ExtCreatePen(PS_GEOMETRIC | PS_DASHDOTDOT | PS_ENDCAP_ROUND, 
                        20, &blackbrush, 0, NULL);

     geopen_drawsample(hdc, 50, 200, pen);
     DeletePen(pen);

     pen = ExtCreatePen(PS_GEOMETRIC | PS_DASHDOTDOT | PS_ENDCAP_ROUND, 
                        10, &blackbrush, 0, NULL);

     geopen_drawsample(hdc, 80, 200, pen);
     DeletePen(pen);

     pen = ExtCreatePen(PS_GEOMETRIC | PS_USERSTYLE | PS_ENDCAP_ROUND,
                        10, &blackbrush, 
                        (DWORD)DIM(SOS),
                        SOS);
     if(pen == NULL)
        { /* failed */
         DWORD err = GetLastError();
         // Report error here...
        } /* failed */
     else
        { /* success */
         geopen_drawsample(hdc, 100, 200, pen);
         DeletePen(pen);
        } /* success */

    }

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerView drawing

void CGDIExplorerView::OnDraw(CDC* pDC)
{
        CGDIExplorerDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        CString caption;

        int xMargin;
        int yMargin;

        xMargin = 20; // TODO: make this smarter
        yMargin = 20;


        caption.LoadString(description);
        GetParentFrame()->SetWindowText(caption);
        if(icon != 0)
           { /* has icon */
            SetIcon(AfxGetApp()->LoadIcon(icon), TRUE);
            SetIcon(AfxGetApp()->LoadIcon(icon), FALSE);
           } /* has icon */

        if(IsIconic() && !Is95())
           { /* draw icon */
            pDC->DrawIcon(0, 0, AfxGetApp()->LoadIcon(icon));
            return;
           } /* draw icon */

        currentbb = NULL;

        switch(style)
           { /* what style */
            case IDC_BMPBRUSH:
                    bmpbrush_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_BEZIER:
                    bezier_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_GEOPEN:
                    geopen_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_LINEJOIN:
                    join_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_STROKEANDFILL:
                    strokeandfill_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_TEXTCLIP1:
                    textclip1_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_TEXTCLIP2:
                    textclip2_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_TEXTCLIP3:
                    textclip3_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_ENDCAPS:
                    endcaps_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_ENDCAPS_STROKED:
                    endcaps_stroked_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_XFORM:
                    xform_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_TEXTXFORM:
                    textxform_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_ROUNDRECT:
                    bb = roundrect_OnPaint(this->m_hWnd, pDC->m_hDC);
                    if(viewbb)
                       pDC->DrawFocusRect(&bb);
                    bb.InflateRect(xMargin, yMargin);
                    currentbb = &bb;
                    break;
            case IDC_RECTANGLE:
            case IDC_ELLIPSE:
                    bb = rect_OnPaint(this->m_hWnd, pDC->m_hDC, style);
                    if(viewbb)
                       pDC->DrawFocusRect(&bb);
                    bb.InflateRect(xMargin, yMargin);
                    currentbb = &bb;
                    break;
            case IDC_ARCTO:
            case IDC_CHORD:
            case IDC_PIE:
            case IDC_ARC:
                    bb = arc_OnPaint(this->m_hWnd, pDC->m_hDC, style);
                    if(viewbb)
                       pDC->DrawFocusRect(&bb);

                    bb.InflateRect(xMargin, yMargin);
                    currentbb = &bb;
                    break;
            case IDC_ANGLEARC:
                    anglearc_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_PENSTYLES:
                    penstyle_OnPaint(pDC->m_hDC);
                    break;
            case IDC_HATCHBRUSH:
                    hatchbrush_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_POLYGON:
                    poly_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_POLYPOLYGON:
                    polypoly_OnPaint(this->m_hWnd, pDC->m_hDC, &pd);
                    break;
            case IDC_DRAWFOCUS:
                    bb = drawfocus_OnPaint(this->m_hWnd, pDC->m_hDC);
                    if(viewbb)
                       pDC->DrawFocusRect(&bb);

                    bb.InflateRect(xMargin, yMargin);
                    currentbb = &bb;
                    break;
            case IDC_COMPLEX_PATH:
                    complex_OnPaint(this->m_hWnd, pDC->m_hDC);
                    break;
            case IDC_RANDOM:
                    random_OnPaint(this->m_hWnd, pDC->m_hDC, &rd);
                    break;
           } /* what style */
}

//..|....1....|....2....|....3....|....4....|....5....|....6....|....7....|....
/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerView printing

BOOL CGDIExplorerView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // default preparation
        return DoPreparePrinting(pInfo);
}

void CGDIExplorerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add extra initialization before printing
}

void CGDIExplorerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
        // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerView diagnostics

#ifdef _DEBUG
void CGDIExplorerView::AssertValid() const
{
        CView::AssertValid();
}

void CGDIExplorerView::Dump(CDumpContext& dc) const
{
        CView::Dump(dc);
}

CGDIExplorerDoc* CGDIExplorerView::GetDocument() // non-debug version is inline
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIExplorerDoc)));
        return (CGDIExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerView message handlers

void CGDIExplorerView::OnSelect() 
{
 CSelect sel;

 sel.style = style;
 sel.rd = rd;

 sel.DoModal();
 style = sel.style;

 rd = sel.rd;

 InvalidateRect(NULL);
 SetParameters();
}

/****************************************************************************
*                         CGDIExplorerView::SetParameters
* Result: void
*       
* Effect: 
*       Sets the description string based on the style
****************************************************************************/

void CGDIExplorerView::SetParameters()
    {
     icon = 0;
     switch(style)
        { /* style */
         case IDC_CHORD:
                 description = IDS_DESC_CHORD;
                 icon = IDI_CHORD;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_PIE:
                 description = IDS_DESC_PIE;
                 icon = IDI_PIE;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_RECTANGLE:
                 description = IDS_DESC_RECT;
                 icon = IDI_RECTANGLE;
                 break;
         case IDC_ELLIPSE:
                 description = IDS_DESC_ELLIPSE;
                 icon = IDI_ELLIPSE;
                 break;
         case IDC_ROUNDRECT:
                 description = IDS_DESC_ROUNDRECT;
                 icon = IDI_ROUNDRECT;
                 break;
         case IDC_BMPBRUSH:
                 description = IDS_DESC_BITMAPFILL;
                 icon = IDI_BMPBRUSH;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_BEZIER:
                 description = IDS_DESC_BEZIER;
                 icon = IDI_BEZIER;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_GEOPEN:
                 description = IDS_DESC_GEOPEN;
                 icon = IDI_GEOPEN;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_LINEJOIN:
                 description = IDS_DESC_LINEJOIN;
                 icon = IDI_LINEJOIN;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_STROKEANDFILL:
                 description = IDS_DESC_STROKEANDFILL;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_TEXTCLIP1:
                 description = IDS_DESC_CLIPPING_REGION;
                 icon = IDI_CLIP1;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_TEXTCLIP2:
                 description = IDS_DESC_CLIPPING_BACKGROUND;
                 icon = IDI_CLIP2;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_TEXTCLIP3:
                 description = IDS_DESC_CLIPPING_FULL;
                 icon = IDI_CLIP3;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_ENDCAPS:
                 description = IDS_DESC_ENDCAPS;
                 icon = IDI_ENDCAPS;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_ENDCAPS_STROKED:
                 description = IDS_DESC_ENDCAPS_STROKED;
                 icon = IDI_ENDCAPS;
                 break;
         case IDC_XFORM:
                 description = IDS_DESC_XFORM;
                 icon = IDI_XFORM;
                 SetPrinterScale(1.0f);
                 break;
         case IDC_TEXTXFORM:
                 description = IDS_DESC_TEXTXFORM;
                 icon = IDI_TEXTXFORM;
                 SetPrinterScale(4.0f);
                 break;
         case IDC_ARCTO:
                 description = IDS_DESC_ARCTO;
                 icon = IDI_ARCTO;
                 SetPrinterScale(3.0f);
                 SetPrinterOffset(500.0f, -300.0f);
                 break;
         case IDC_ARC:
                 description = IDS_DESC_ARC;
                 icon = IDI_ARC;
                 SetPrinterScale(3.0f);
                 SetPrinterOffset(500.0f, -300.0f);
                 break;
         case IDC_ANGLEARC:
                 description = IDS_DESC_ANGLEARC;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_PENSTYLES:
                 description = IDS_DESC_PENSTYLES;
                 icon = IDI_PENSTYLE;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_HATCHBRUSH:
                 description = IDS_DESC_HATCHBRUSH;
                 icon = IDI_HATCHBRUSH;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_POLYGON:
                 description = IDS_DESC_POLYGON;
                 icon = IDI_POLYGON;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_POLYPOLYGON:
                 description = IDS_DESC_POLYPOLYGON;
                 icon = IDI_POLYPOLYGON;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_DRAWFOCUS:
                 description = IDS_DESC_DRAWFOCUS;
                 icon = IDI_FOCUS;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_COMPLEX_PATH:
                 description = IDS_DESC_COMPLEX_PATH;
                 icon = IDI_COMPLEX;
                 SetPrinterScale(3.0f);
                 break;
         case IDC_RANDOM:
                 description = IDS_DESC_RANDOM;
                 icon = IDI_RANDOM;
                 break;
         } /* style */  
    }

/****************************************************************************
*                           CGDIExplorerView::GetFileName
* Inputs:
*       CString & filename
*       UINT nIDSTitle: Title style ID
*       DWORD lFlags: flags for dialog
*       BOOL bOpenFileDialog: Open or Save? TRUE for open
*       BOOL enhanced: Use enhanced dialog
* Result: BOOL
*       TRUE if successful (fileName is change)
*       FALSE if error (fileName is unchanged)
* Effect: 
*       Prompts user for a file name
****************************************************************************/

BOOL CGDIExplorerView::GetFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
        BOOL bOpenFileDialog, BOOL enhanced)
    {
     CFileDialog dlgFile(bOpenFileDialog);

     CString title;

     dlgFile.m_ofn.Flags |= lFlags;

     CString strFilter;
     CString strDefault;
     CString ext;


     strFilter.LoadString(enhanced ? IDS_EMF_FILES : IDS_WMF_FILES);
     ext.LoadString(enhanced ? IDS_EMF_EXT : IDS_WMF_EXT);


     strFilter += _T('\0');
     strFilter += _T("*.") + ext;
     dlgFile.m_ofn.nMaxCustFilter++;
     strFilter += _T('\0');   // next string please

     dlgFile.m_ofn.lpstrDefExt = ext;


     // append the "*.*" all files filter
     CString allFilter;
     VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
     strFilter += allFilter;
     strFilter += _T('\0');   // next string please
#ifndef _MAC
     strFilter += _T("*.*");
#else
     strFilter += _T("****");
#endif
     strFilter += _T('\0');   // last string

     dlgFile.m_ofn.nMaxCustFilter++;

     dlgFile.m_ofn.lpstrFilter = strFilter;
#ifndef _MAC
     dlgFile.m_ofn.lpstrTitle = title;
#else
     dlgFile.m_ofn.lpstrPrompt = title;
#endif

     dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
     
     BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
     fileName.ReleaseBuffer();
     return bResult;
     
    }

/****************************************************************************
*                              metafile_checksum
* Inputs:
*       METAFILEHEADER * mfh: Metafile header
* Result: WORD
*       The XOR checksum of the first 10 words of the header
****************************************************************************/

WORD metafile_checksum(METAFILEHEADER * mfh)
    {
     WORD sum = 0;
     LPWORD p = (LPWORD)mfh;
     for(int i = 0; i < 10; i++)
        sum ^= p[i];
     return sum;
    }

/****************************************************************************
*                                WriteMetaFile
* Inputs:
*       BOOL enhanced: TRUE for enhanced meteafile, FALSE for normal
* Result: void
*       
* Effect: 
*       Writes an enhanced metafile (.emf) or windows metafile (.wmf)
*       based upon the input parameter
****************************************************************************/

void CGDIExplorerView::WriteMetaFile(BOOL enhanced)
    {
     CString name;
     CString fulldescription;
     CMetaFileDC metadc;
     CDC * refDC = GetDC();
     CRect rect;
     BOOL failed = FALSE;
     int PelX;  // conversion factors for enhanced metafile header
     int PelY;
     CMFH dlg;


     PelX = refDC->GetDeviceCaps(HORZSIZE) * 100 / refDC->GetDeviceCaps(HORZRES);
     PelY = refDC->GetDeviceCaps(VERTSIZE) * 100 / refDC->GetDeviceCaps(VERTRES);

     // Clear the global bounding box used for metafile computations
     clearGlobalBB();

     // Draw the object once so we have computed its bounding box

     {
      CClientDC dc(this);
      OnDraw(&dc);
     }
     

     GetClientRect(&rect);

     // If the global BB has been set, use it instead
     if(globalBBSet())
        rect = GlobalBB;

     // rect.left *= PelX;
     // rect.top *= PelY;
     // rect.right *= PelX;
     // rect.bottom *= PelY;

     if(!GetFileName(name, AFX_IDS_SAVEFILE,
                            OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
                            FALSE,
                            enhanced))
                    return;

     // Create the placeable metafile header (we will write it later)

     METAFILEHEADER mfh;
     mfh.key = 0x9AC6CDD7;                      // magic header key
     mfh.hmf = 0;                               // unused, must be 0

     // Note we do truncating assignment to short rectangle structure!
     mfh.bbox.Left   = (SHORT)rect.left;
     mfh.bbox.Top    = (SHORT)rect.top;
     mfh.bbox.Right  = (SHORT)rect.right;
     mfh.bbox.Bottom = (SHORT)rect.bottom;

     mfh.inch = 1000;                           // logical units per inch
     mfh.reserved = 0;                          // reserved, must be 0


     { /* tweak it */
      static oldinch;
      static SMALL_RECT oldbbox; // retained for hints
      static int oldPelX;        // ...
      static int oldPelY;        // ...
      static BOOL oldsetorigin;  // ...

      dlg.m_left = mfh.bbox.Left;
      dlg.m_top  = mfh.bbox.Top;
      dlg.m_right = mfh.bbox.Right;
      dlg.m_bottom = mfh.bbox.Bottom;
      dlg.m_inch   = mfh.inch;
      dlg.m_lastleft = oldbbox.Left;
      dlg.m_lasttop  = oldbbox.Top;
      dlg.m_lastright = oldbbox.Right;
      dlg.m_lastbottom = oldbbox.Bottom;
      dlg.m_lastinch   = oldinch;
      dlg.m_SetOrigin = oldsetorigin;
      dlg.m_PelX = PelX;
      dlg.m_PelY = PelY;
      dlg.m_OldPelX = oldPelX;
      dlg.m_OldPelY = oldPelY;

      dlg.DoModal();

      mfh.bbox.Left = dlg.m_left;
      mfh.bbox.Top  = dlg.m_top;
      mfh.bbox.Right = dlg.m_right;
      mfh.bbox.Bottom = dlg.m_bottom;
      mfh.inch   = dlg.m_inch;
      PelX = dlg.m_PelX;
      PelY = dlg.m_PelY;
      oldsetorigin = dlg.m_SetOrigin;

      oldPelX = PelX;
      oldPelY = PelY;
      oldbbox = mfh.bbox;
      oldinch = mfh.inch;
     } /* tweak it */

     mfh.checksum = metafile_checksum(&mfh);    // checksum


     BeginWaitCursor();

     fulldescription.LoadString(IDS_METAFILE_DESC);
     fulldescription += _T('\0');  // get first NULL between strings
     fulldescription += description;
     fulldescription += _T('\0');        // get second NULL at end

     CString desc;

     desc.LoadString(description);

     if(enhanced)
        metadc.CreateEnhanced(refDC, _T("$$$.tmp"), &rect, desc);
     else
        metadc.Create(_T("$$$.tmp"));

     OnDraw(&metadc);

     // Close the metafile and delete its file handle (but not the file)
     if(enhanced)
        ::DeleteEnhMetaFile(metadc.CloseEnhanced());
     else
        ::DeleteMetaFile(metadc.Close());


     HANDLE f = CreateFile(name, GENERIC_WRITE, 0, NULL, 
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
     HANDLE s = CreateFile(_T("$$$.tmp"), GENERIC_READ, 0, NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

     if(f != NULL && s != NULL)
        { /* copy */
         DWORD written;
         WriteFile(f, &mfh, sizeof(mfh), &written, NULL);
         
         while(TRUE)
            { /* copy metafile contents */
             TCHAR buffer[1024];
             DWORD read;
             ReadFile(s, buffer, DIM(buffer), &read, NULL);
             if(read == 0)
                break;  // EOF detected
             WriteFile(f, buffer, read, &written, NULL);
            } /* copy metafile contents */
        } /* copy */
     else
        { /* error */
         // NYI: report problem
        } /* error */

     if(f != NULL)
        CloseHandle(f);
     if(s != NULL)
        CloseHandle(s);
     EndWaitCursor();   
     
    }

void CGDIExplorerView::OnWriteMetaFile() 
{
 WriteMetaFile(FALSE);
        
}

void CGDIExplorerView::OnWriteEnhancedMetaFile() 
{
 WriteMetaFile(TRUE);
}

void CGDIExplorerView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
 if(pInfo != NULL)
    { /* printing DC */
#if 0
     if(!::SetGraphicsMode(pDC->m_hDC, GM_ADVANCED))
        return;
     if(!::ModifyWorldTransform(pDC->m_hDC, &printscale, MWT_LEFTMULTIPLY))
        return;
#else
 
#endif
     
    } /* printing DC */
        
  CView::OnPrepareDC(pDC, pInfo);
}


void CGDIExplorerView::OnSetcontrolpoints() 
{
 CPolyPoints dlg;

 // This creates a copy of the default the first time we do it
 if(pd.points == NULL)
    { /* initialize */
     pd.count = DIM(PolyCounts);
     pd.points = new int[pd.count];
     if(pd.points == NULL)
        { /* alloc failed */
         // NYI: error reporting
         return;
        } /* alloc failed */

     memcpy(pd.points, PolyCounts, DIM(PolyCounts));
     pd.data = pp_points; // fixed, does not change
     pd.valuecount = DIM(pp_points);
    } /* initialize */

 dlg.pd = pd;  // copy entire structure

 dlg.DoModal();

 pd = dlg.pd;
        
 InvalidateRect(NULL);
}

void CGDIExplorerView::OnSetpolyfillAlternate() 
{
 rd.fillmode = pd.fillmode = ALTERNATE;
 InvalidateRect(NULL);
}

void CGDIExplorerView::OnSetpolyfillWinding() 
{
 
 rd.fillmode = pd.fillmode = WINDING;
 InvalidateRect(NULL);
}


/****************************************************************************
*                          CGDIExplorerView::DoContextMenu
* Inputs:
*       CPoint point: Mouse point, in client units
* Result: BOOL
*       TRUE if menu was found; FALSE if menu not found
* Effect: 
*       If there is a menu defined for the current window, pop it up and
*       do something with it.
* Notes:
*       This is a "generic" handler for any draw class (as designated by
*       its class code, which must be unique for each draw class, and 
*       designates its icon and popup menu).  Therefore, it will attempt
*       to set state even for menu items which may not exist in a given
*       class   
****************************************************************************/

BOOL CGDIExplorerView::DoContextMenu(CPoint point)
    {
     CMenu menu;
     if(!menu.LoadMenu(icon))
        return FALSE;

     ClientToScreen(&point);

     // For any given menu, the following functions may or may not
     // find their state in that menu.  However, for those menus that
     // have the state, the items will be checked...


     // If we had more time we would create classes to handle this...

     switch(icon)
        { /* icon */
         case IDI_POLYPOLYGON:
                 menu.CheckMenuItem(IDM_SETPOLYFILL_ALTERNATE, 
                        MF_BYCOMMAND | (pd.fillmode == ALTERNATE ? MF_CHECKED 
                                                          : MF_UNCHECKED));
                 menu.CheckMenuItem(IDM_SETPOLYFILL_WINDING, 
                        MF_BYCOMMAND | (pd.fillmode == WINDING ? MF_CHECKED 
                                                        : MF_UNCHECKED));
                 break;
         case IDI_RANDOM:
                 menu.CheckMenuItem(IDM_SETPOLYFILL_ALTERNATE, 
                        MF_BYCOMMAND | (rd.fillmode == ALTERNATE ? MF_CHECKED 
                                                          : MF_UNCHECKED));
                 menu.CheckMenuItem(IDM_SETPOLYFILL_WINDING, 
                        MF_BYCOMMAND | (rd.fillmode == WINDING ? MF_CHECKED 
                                                        : MF_UNCHECKED));
                 menu.CheckMenuItem(IDM_SETPOLYFILL_NONE, 
                        MF_BYCOMMAND | (rd.fillmode != WINDING &&
                                        rd.fillmode != ALTERNATE
                                                        ? MF_CHECKED 
                                                        : MF_UNCHECKED));
                 break;
        } /* icon */

     menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                                point.x, point.y, this, NULL);

     menu.DestroyMenu();

     return TRUE;
    }

void CGDIExplorerView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
 if(!DoContextMenu(point))
    CView::OnContextMenu(pWnd, point);
        
}

void CGDIExplorerView::OnRButtonUp(UINT nFlags, CPoint point) 
{
 if(!CGDIExplorerView::DoContextMenu(point))
    CView::OnRButtonUp(nFlags, point);
}

void CGDIExplorerView::OnSetpolyfillNone() 
{
 rd.fillmode = ALTERNATE + WINDING; // illegal value means "none"
 InvalidateRect(NULL);
}

void CGDIExplorerView::OnEditCopy() 
{
 toClipboard(this, currentbb);  
}

void CGDIExplorerView::OnViewBb() 
{
 viewbb = !viewbb;
 InvalidateRect(NULL);
}

void CGDIExplorerView::OnUpdateViewBb(CCmdUI* pCmdUI) 
{
 pCmdUI->SetCheck(viewbb);
        
}
