// FontDisp.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "poplist.h"
#include "legendi.h"
#include "legend.h"
#include "dispopts.h"
#include "fontdesc.h"
#include "FontDisp.h"
#include "savedc.h"
#include "kern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define fontCOLOR_INTERNAL_LEADING RGB(255, 255,   0)
#define fontCOLOR_EXTERNAL_LEADING RGB(  0, 255, 255)
#define penCOLOR_BASELINE          RGB(  0, 255,   0)
#define fontCOLOR_ASCENT           RGB(192, 255, 192)   // 010
#define fontCOLOR_DESCENT          RGB(255, 192, 192)   // 100
#define fontCOLOR_ABC1             RGB(192, 192, 255)   // 001
#define fontCOLOR_ABC2             RGB(255, 192, 255)   // 101

#define penCOLOR_ABC               RGB(127, 127, 127)

CBrush internalLeadingBrush(fontCOLOR_INTERNAL_LEADING);
CPen   internalLeadingPen(PS_SOLID, 0, fontCOLOR_INTERNAL_LEADING);

CBrush externalLeadingBrush(fontCOLOR_EXTERNAL_LEADING);
CPen   externalLeadingPen(PS_SOLID, 0, fontCOLOR_EXTERNAL_LEADING);

CBrush ascentBrush(fontCOLOR_ASCENT);
CPen   ascentPen(PS_SOLID, 0, fontCOLOR_ASCENT);

CBrush descentBrush(fontCOLOR_DESCENT);
CPen   descentPen(PS_SOLID, 0, fontCOLOR_DESCENT);

CPen   baselinePen(PS_SOLID, 0, penCOLOR_BASELINE);

CBrush abc1Brush(fontCOLOR_ABC1);
CPen   abc1Pen(PS_SOLID, 0, fontCOLOR_ABC1);
CBrush abc2Brush(fontCOLOR_ABC2);
CPen   abc2Pen(PS_SOLID, 0, fontCOLOR_ABC2);

CPen   abcPen(PS_SOLID, 0, penCOLOR_ABC);

#define BLINE  1
#define BLINEc 0
#define ACLINE 2
#define ACLINEc 2
#define LINEOFFSET 3

#define ABCextra 5

typedef struct {float sx; float sy; } scaling;
static LegendInfo Legend_internalLeading(&internalLeadingBrush, &internalLeadingPen, IDS_INTERNAL_LEADING);
static LegendInfo Legend_externalLeading(&externalLeadingBrush, &externalLeadingPen, IDS_EXTERNAL_LEADING);
static LegendInfo Legend_ascent(&ascentBrush, &ascentPen, IDS_ASCENT);
static LegendInfo Legend_descent(&descentBrush, &descentPen, IDS_DESCENT);
static LegendInfo Legend_ABC1(&abc1Brush, &abc1Pen, IDS_ABC1);
static LegendInfo Legend_ABC2(&abc2Brush, &abc2Pen, IDS_ABC2);

static LegendInfo * legend[] = {
        &Legend_internalLeading,
        &Legend_ascent,
        &Legend_descent,
        &Legend_externalLeading,
        &Legend_ABC1,
        &Legend_ABC2,
        NULL                   };

/****************************************************************************
*                           CFontDisplay::GetLegend
* Result: LegendInfo **
*       Pointer to legend table
****************************************************************************/

LegendInfo ** CFontDisplay::GetLegend()
    {
     return &legend[0];
    }

/****************************************************************************
*                         CFontDisplay::CFontDisplay
* Inputs:
*       BOOL dokerning: Establishes initial kerning value
* Effect: 
*       Constructor
****************************************************************************/


CFontDisplay::CFontDisplay(BOOL dokerning /* = FALSE */ )
{
 opts.debug = FALSE;

 opts.hrule = TRUE;
 opts.vrule = TRUE;
 opts.chrule = TRUE;

 opts.colors = TRUE;
 opts.white = FALSE; // stock background

 opts.track = FALSE;  // no track kerning
 opts.trackval = 0;

 opts.kerning = dokerning;

 opts.basal = FALSE;
#ifdef _DEBUG
 pinfo = NULL;
#endif
}

/****************************************************************************
*                         CFontDisplay::~CFontDisplay
* Effect: 
*       Frees up any pinfo storage
****************************************************************************/

CFontDisplay::~CFontDisplay()
{
#ifdef _DEBUG
 if(pinfo != NULL)
    delete [] pinfo;
#endif
}


BEGIN_MESSAGE_MAP(CFontDisplay, CStatic)
        //{{AFX_MSG_MAP(CFontDisplay)
        ON_WM_PAINT()
        ON_WM_ERASEBKGND()
        ON_WM_RBUTTONDOWN()
        ON_WM_RBUTTONUP()
        ON_WM_LBUTTONDOWN()
        ON_WM_NCHITTEST()
        ON_WM_LBUTTONUP()
        ON_WM_LBUTTONDBLCLK()
        ON_WM_CREATE()
        ON_WM_DESTROY()
        ON_WM_CANCELMODE()
        ON_WM_SIZE()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontDisplay message handlers

/****************************************************************************
*                                   getABC
* Inputs:
*       CDC & dc: Display context
*       TCHAR ch: Character whose ABC value is to be obtained
*       ABC& abc: ABC structure
* Result: void
*       
* Effect: 
*       Loads up ABC values.  If the font is not a TrueType font, sets
*       A = 0, C = 0, B = text extent
****************************************************************************/

void getABC(CDC & dc, TCHAR ch, ABC& abc)
    {
      if(!dc.GetCharABCWidths(ch, ch, &abc))
         { /* not TrueType */
          abc.abcA = 0;
          abc.abcC = 0;
          SIZE sz = dc.GetTextExtent(&ch, 1);
          abc.abcB = sz.cx;
         } /* not TrueType */
    }

/****************************************************************************
*                               CFontDisplay::normalizeScaling
* Inputs:
*       float sx: Scaling factor, expressed as floating point number
* Result: iscale
*       .win is window extent
*       .view is viewport extent
* Notes: 
*       This computes the two integer values needed to set the viewport extent
*       and the window extent.  These integers are ideally in the ratio 
*       expressed by the floating point number, but may be off by one unit
*       because of the need to dumb-down to the 16-bit Win95 GDI.
****************************************************************************/

iscale CFontDisplay::normalizeScaling(float sx)
    {
     iscale sc;
     sc.win = 1000;
     sc.view = (int)(1000.0f * sx);
     while(sc.view > 32767)
        { /* dumb down */
         sc.view /= 2;
         sc.win /= 2;
        } /* dumb down */
     return sc;
    }

/****************************************************************************
*                               computeScaling
* Inputs:
*       CRect r: Rectangle defining container window
*       SIZE sz: width of string including overhangs
* Result: scaling
*       Floating point values giving scaling for x and y
* Effect: 
*       ²
****************************************************************************/

scaling computeScaling(CRect r, SIZE sz)
    {
     scaling sc;
     sc.sx = (float)r.Width() / (float)sz.cx;
     sc.sy = (float)r.Height() / (float)sz.cy;
     return sc;
    }

/****************************************************************************
*                     CFontDisplay::getOverhangTextExtent
* Inputs:
*       CDC & dc: DC to use for values.  Should be untransformed and use
*                       MM_TEXT mapping mode
*       CString s: String to compute width of
* Result: SIZE
*       Size of string
* Effect: 
*       ²
****************************************************************************/

SIZE CFontDisplay::getOverhangTextExtent(CDC & dc, CString s)
    {
     SIZE sz;
     CSaveDC pdc(&dc);
     pdc->SetMapMode(MM_TEXT);

     sz = pdc->GetTextExtent(s);

     if(s.GetLength() == 0)
        return sz;

     // If we have overhang on the far left or far right
     // factor the overhang into the values
     // Add the overhang only for TrueType fonts

     ABC abc;
     if(pdc->GetCharABCWidths(s[0], s[0], &abc))
        if(abc.abcA < 0)
           sz.cx += -abc.abcA;

      if(pdc->GetCharABCWidths(s[s.GetLength()- 1], s[s.GetLength() - 1], &abc))
         if(abc.abcC < 0)
            sz.cx += -abc.abcC;

      if(opts.track & s.GetLength() > 0)
         sz.cx += opts.trackval * (s.GetLength() - 1);

      return sz;
    }

/****************************************************************************
*                          CFontDisplay::writeRulerV
* Inputs:
*       CDC & dc: Display context
*       int left: Left edge
*       int top: Starting position
*       int bottom: Ending position
* Result: void
*       
* Effect: 
*       Draws a vertical ruler
****************************************************************************/

#define RULE5  2
#define RULE10 3
#define RULE1  1

void CFontDisplay::writeRulerV(CDC & idc, int left, int top, int bottom)
    {
     CSaveDC dc(&idc);
     dc->SetROP2(R2_NOTXORPEN);
     for(int i = top; i <= bottom; i++)
        { /* write rule */
         int width;

         if(i % 10 == 0)
            width = RULE10;
         else
         if(i % 5 == 0)
            width = RULE5;
         else
            width = RULE1;

         dc->MoveTo(left, i);
         dc->LineTo(left + width, i);
        } /* write rule */
    }

/****************************************************************************
*                          CFontDisplay::writeRulerH
* Inputs:
*       CDC & dc: DC
*       int left: 
* Result: void
*       
* Effect: 
*       Draws a horizontal rule along the bottom edge
* Notes:
*       Need to supply a sign eventually so we can have a scale going up
*       or down.  
****************************************************************************/

void CFontDisplay::writeRulerH(CDC & idc, int pos, int left, int right)
    {
     CSaveDC dc(&idc);
     dc->SetROP2(opts.white ? R2_COPYPEN : R2_NOTXORPEN);
     for(int i = left; i <= right; i++)
        { /* write rule */
         int height;

         if(i % 10 == 0)
            height = RULE10;
         else
         if(i % 5 == 0)
            height = RULE5;
         else
            height = RULE1;

         dc->MoveTo(i, pos);
         dc->LineTo(i, pos - height);
        } /* write rule */
     
    }

/****************************************************************************
*                             CFontDisplay::DrawV
* Inputs:
*       CDC & dc: DC
*       int x: Horizontal position
*       int top: Top of line
*       int bottom: Bottom of line
* Result: void
*       
* Effect: 
*       Draws a fiducial line
****************************************************************************/

void CFontDisplay::drawV(CDC & dc, int x, int top, int bottom)
    {
     CSaveDC pdc(&dc);
     pdc->SelectObject(&abcPen);
     pdc->MoveTo(x, top);
     pdc->LineTo(x, bottom);
    }

/****************************************************************************
*                         CFontDisplay::drawDistance
* Inputs:
*       CDC & dc: DC
*       int from: Horizontal starting position
*       int to: Horizontal ending position
*       int ypos: Vertical position of line
*       LPCTSTR legend: Legend to display
*       int ylegend: Position to display legend
* Result: void
*       
* Effect: 
*       Draws a horizontal line with a legend above or below it, centered
*       over it.
* Notes:
*
****************************************************************************/

void CFontDisplay::drawDistance(CDC & dc, int from, int to, int ypos, LPCTSTR legend, int ylegend)
    {
     CSaveDC pdc(&dc);

     pdc->MoveTo(from, ypos);
     pdc->LineTo(to, ypos);
     

#if 0 // debug later
     SIZE sz;
     pdc->SelectObject(&legendFont);
     sz = pdc->GetTextExtent(legend, lstrlen(legend));

     pdc->TextOut(from + (to - from - sz.cx) / 2, ylegend, legend, 
                                                        lstrlen(legend) );
#endif

    }

/****************************************************************************
*                        CFontDisplay::GetDisplayCode
* Inputs:
*       TCHAR ch: Character code
* Result: int
*       Display code
****************************************************************************/

int CFontDisplay::GetDisplayCode(CDC & dc, TCHAR ch)
    {
     ABC abc;

     {
      CSaveDC pdc(&dc);
      pdc->SetMapMode(MM_TEXT);
      getABC(pdc, ch, abc);
     }

     return (f(abc.abcA) + 3 * f(abc.abcC));

    }

/****************************************************************************
*                            CFontDisplay::drawABC
* Inputs:
*       CDC & dc: Display context
*       int pos: Character starting position
*       TCHAR ch: Character code
* Result: void
*       
* Effect: 
*       Draws the ABC fiducials and annotations
* Notes:
*       We encode the cases in a simple-minded way; we use the
*       encodings 0, 1, 2 to indicate the relationship to 0, and
*       a dense polynomial encoding according to the formula
*               f(A) + 3 * f(c) 
*
*       f(x) is defined as
*               x == 0 then 0
*               x <  0 then 1
*               x >  0 then 2
*               
*
*          0                              1  <= fiducial
*          |               B              |
*          |------------------------------|  A == 0, C == 0     0     
*          |                              |  
*          |                              |
*
*          0     1                        2  <= fiducial
*          |     |            B           |
*          |------------------------------|  A < 0, C == 0      
*          |-----|                        |    1    3 * 0       1
*          |  A  |                        |
*
*          0     1                        2  <= fiducial
*          |     |            B           |
*          |     |------------------------|  A > 0, C == 0      
*          |-----|                        |    2    3 * 0       2
*          |  A  |                        |
*
*          0                        1     2  <= fiducial
*          |           B            |     |
*          |------------------------------|  A == 0, C < 0
*          |                        |-----|    0     3 * 1      3
*          |                        |  C  |
*
*          0     1                  2     3  <= fiducial
*          |     |        B         |     |
*          |------------------------------|  A < 0, C < 0       
*          |-----|                  |-----|    1    3 * 1       4
*          |  A  |                  |  C  |
*
*          0     1                  2     3  <= fiducial
*          |     |            B     |     |
*          |     |------------------------|  A > 0, C < 0       
*          |-----|                  |-----|    2    3 * 1       5
*          |  A  |                  |  C  |     
*
*
*          0                        1     2  <= fiducial
*          |           B            |     |
*          |------------------------|  C  |  A == 0, C > 0
*          |                        |-----|    0     3 * 2      6
*          |                        |     |
*
*          0     1                  2     3  <= fiducial
*          |     |                  |     |
*          |-----------B------------|     |  A < 0, C > 0       
*          |--A--|                  |--C--|    1    3 * 2       7
*          |     |                  |     |
*
*          0     1                  2     3  <= fiducial        
*          |     |        B         |     |
*          |     |------------------|     |  A > 0, C > 0       
*          |-----|                  |-----|    2    3 * 2       8
*          |  A  |                  |  C  |
*
*
*
****************************************************************************/

void CFontDisplay::drawABCs(CDC & dc, int pos, TCHAR ch, int top, int bottom, int parity)
    {
     ABC abc;
     int code;
     int botf = bottom + ABCextra - 2;
     
     {
      CSaveDC pdc(&dc);
      pdc->SetMapMode(MM_TEXT);
      getABC(pdc, ch, abc);
     }

     code = GetDisplayCode(dc, ch); 

     // Draw the rectangles (in alternating colors) that highlight the ABC
     // rulers
     if(opts.colors)
         { /* show colors */
          CSaveDC pdc(&dc);

          pdc->SelectObject((parity ? abc1Pen   : abc2Pen));
          pdc->SelectObject((parity ? abc1Brush : abc2Brush));
          CRect r;
          r.left = pos;
          r.top = bottom;
          r.right = r.left + abc.abcB + (abc.abcA < 0 ? 0 : abc.abcA) +
                                        (abc.abcC < 0 ? 0 : abc.abcC);
          r.bottom = r.top + LINEOFFSET;
          pdc->Rectangle(&r);
         } /* show colors */
     
     switch(code)
        { /* code */
         case 0:  // A == 0, C == 0
                 drawV(dc, pos, top, botf);             // 0
                 drawV(dc, pos + abc.abcB, top, botf);  // 1
                 drawDistance(dc, pos, pos+abc.abcB, bottom + BLINE, _T("B"), 
                                                bottom + BLINEc);
                 break;
         case 1: // A < 0, C == 0
                 drawV(dc, pos, top, botf);             // 0
                 drawV(dc, pos - abc.abcA, top, botf);  // 1
                 drawV(dc, pos + abc.abcB, top, botf);  // 2
                 drawDistance(dc, pos, pos+ abc.abcB, bottom +BLINE, _T("B"),
                                                bottom + BLINEc);
                 drawDistance(dc, pos, pos - abc.abcA, bottom+ACLINE, _T("A"),
                                                bottom + ACLINEc);
                 break;
         case 2: // A > 0, C == 0
                 drawV(dc, pos, top, botf);                             // 0
                 drawV(dc, pos + abc.abcA, top, botf);                  // 1
                 drawV(dc, pos + abc.abcA + abc.abcB, top, botf);       // 2 
                 drawDistance(dc, pos + abc.abcA, pos + abc.abcA + abc.abcB,
                                        bottom + BLINE,
                                        _T("B"), bottom + BLINEc);
                 drawDistance(dc, pos, pos + abc.abcA, bottom + ACLINE, 
                                                _T("A"),
                                                bottom + ACLINEc);
                 break;
         case 3: // A == 0, C < 0
                 drawV(dc, pos, top, botf);
                 drawV(dc, pos + abc.abcB, top, botf);
                 drawV(dc, pos + abc.abcB + abc.abcC, top, botf);
                 drawDistance(dc, pos, pos + abc.abcB, bottom + BLINE,
                                        _T("B"), bottom);
                 drawDistance(dc, pos + abc.abcB + abc.abcC,
                                  pos + abc.abcB,
                                  bottom + ACLINE,
                                  _T("C"), bottom + ACLINEc);
                 break;
         case 4: // A < 0, C < 0
                 drawV(dc, pos, top, botf);
                 drawV(dc, pos - abc.abcA, top, botf);
                 drawV(dc, pos + abc.abcB, top, botf);
                 drawV(dc, pos + abc.abcB + abc.abcC, top, botf);
                 drawDistance(dc, pos, pos + abc.abcB, 
                                  bottom + 1,
                                  _T("B"), bottom);
                 drawDistance(dc, pos, pos - abc.abcA,
                                  bottom + 2,
                                  _T("A"), bottom + 2);
                 drawDistance(dc, pos + abc.abcB + abc.abcC, pos + abc.abcB,
                                  bottom + 2,
                                  _T("C"), bottom + 2);
                 break;
         case 5: // A > 0, C < 0
                 drawV(dc, pos, top, botf);
                 drawV(dc, pos + abc.abcA, top, botf);
                 drawV(dc, pos + abc.abcA + abc.abcB, top, botf);
                 drawV(dc, pos + abc.abcA + abc.abcB + abc.abcC, top, botf);
                 drawDistance(dc, pos + abc.abcA, pos + abc.abcA + abc.abcB,
                                   bottom + 1,
                                   _T("B"), bottom);
                 drawDistance(dc, pos, pos + abc.abcA, bottom + 2,
                                   _T("A"), bottom + 2);
                 drawDistance(dc, pos + abc.abcA + abc.abcB + abc.abcC,
                                  pos + abc.abcA + abc.abcB,
                                  bottom + 2,
                                  _T("C"), bottom + 2);
                 break;
         case 6: // A == 0, C > 0
                 drawV(dc, pos, top, botf);
                 drawV(dc, pos + abc.abcB, top, botf);
                 drawV(dc, pos + abc.abcB + abc.abcC, top, botf);
                 drawDistance(dc, pos, pos + abc.abcB, bottom + 1, 
                                        _T("B"), bottom);
                 drawDistance(dc, pos + abc.abcB, pos + abc.abcB + abc.abcC,
                                        bottom + 2,
                                        _T("C"), bottom + 2);
                 break;
         case 7: // A < 0, C > 0
                 drawV(dc, pos, top, botf);
                 drawV(dc, pos - abc.abcA, top, botf);
                 drawV(dc, pos + abc.abcB, top, botf);
                 drawV(dc, pos + abc.abcB + abc.abcC, top, botf);
                 drawDistance(dc, pos, pos + abc.abcB, bottom + 1,
                                        _T("B"), bottom);
                 drawDistance(dc, pos, pos - abc.abcA, bottom + 2,
                                        _T("A"), bottom + 2);
                 drawDistance(dc, pos + abc.abcB, pos + abc.abcB + abc.abcC,
                                        bottom + 2,
                                        _T("C"), bottom + 2);
                 break;
         case 8: // A > 0, C > 0
                 drawV(dc, pos, top, botf);
                 drawV(dc, pos + abc.abcA, top, botf);
                 drawV(dc, pos + abc.abcA + abc.abcB, top, botf);
                 drawV(dc, pos + abc.abcA + abc.abcB + abc.abcC, top, botf);
                 drawDistance(dc, pos + abc.abcA, pos + abc.abcA + abc.abcB,
                                bottom + 1,
                                _T("B"), bottom);
                 drawDistance(dc, pos, pos + abc.abcA, bottom + 2, 
                                _T("A"), bottom + 2);
                 drawDistance(dc, pos + abc.abcA + abc.abcB, 
                                  pos + abc.abcA + abc.abcB + abc.abcC,
                                  bottom + 2,
                                  _T("C"), bottom + 2);
                 break;
        } /* code */

    }

/****************************************************************************
*                        CFontDisplay::requiredHeight
* Inputs:
*       CFontDescriptor & desc
* Result: int
*       Required height for display, including rulers and the space around
*       them
****************************************************************************/

int CFontDisplay::requiredHeight(CFontDescriptor & desc)
    {
     return (desc.tm.tmHeight + desc.tm.tmExternalLeading) + (opts.chrule ? ABCextra : 0) + 1;
    }

/****************************************************************************
*                         CFontDisplay::limitScaling
* Inputs:
*       TEXTMETRIC & tm: Allows us to determine if raster font
*       int scale: Current scaling factor
* Result: float
*       if raster font, min(scale, 5)
*       if TrueType font, scale
****************************************************************************/

float CFontDisplay::limitScaling(TEXTMETRIC & tm, float scale)
    {
     if( (tm.tmPitchAndFamily & (TMPF_VECTOR | TMPF_TRUETYPE)) == 0)
        return min(5.0f, scale);
     else
        return scale;
    }

/****************************************************************************
*                            CFontDisplay::OnPaint
* Result: void
*       
* Effect: 
*       Handles the painting
****************************************************************************/

void CFontDisplay::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        
        CFontDescriptor desc(this, this->GetParent());

        CRect r;
        GetClientRect(&r);

#ifdef _DEBUG
        if(pinfo != NULL)
           { /* release info structure */
            delete [] pinfo;
            pinfo = NULL;
           } /* release info structure */
#endif
        // We have to set the clipping region because static controls
        // inherit the CS_PARENTDC style, which we don't want.  This
        // style sets the clipping region to be the region of the parent,
        // which seems intrinsically nonsensical, but that's how it is.
        // It does NOT, as the documentation states, share a DC with the
        // parent.  This is incorrect, and the error is pointed out in the
        // KB article Q111005.

        CRgn clip;
        clip.CreateRectRgnIndirect(&r);
        dc.SelectObject(clip);

        CString s;
        GetWindowText(s);

#ifdef _DEBUG
        if(s.GetLength() > 0)
           pinfo = new PosInfo[s.GetLength()];
#endif

        dc.SelectObject(desc.font);

        SIZE stringwidth = getOverhangTextExtent(dc, s);

        stringwidth.cx += 1; // fudge factor to keep edge on screen

        // Compute ratios of horizontal and vertical, and scale the the
        // smaller one


        scaling sc;

        stringwidth.cy = requiredHeight(desc);
        sc = computeScaling(r, stringwidth);

        // We need to convert the scaling factor from floating point to
        // integer for the SetWindowExtEx and SetViewPortExtEx calls.
        // We want the smallest scaling so that we don't exceed either the
        // width or the height

        float sx = min(sc.sx, sc.sy);

        // Note that a raster font can be scaled at most 5X horizontally and
        // 8X vertically, so if the font is a raster font we limit it
        // to 5X

        sx = limitScaling(desc.tm, sx);

        iscale scale = normalizeScaling(sx);

        dc.SetMapMode(MM_ISOTROPIC);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(desc.font);

        // Note: MUST set WindowExt before ViewportExt
        // Scaling factor is ViewportExtent / WindowExtent
        dc.SetWindowExt(scale.win, scale.win);
        dc.SetViewportExt(scale.view, scale.view);

        int top = 0;
        int bottom = desc.tm.tmHeight + desc.tm.tmExternalLeading;
        int right = stringwidth.cx;

        if(opts.track && opts.trackval > 0 && s.GetLength() > 0)
           right += opts.trackval * (s.GetLength() - 1);

        if(opts.colors)
            { /* show colors */
             CSaveDC pdc(&dc);
             /*
                +------------------------------------------+
                |     internal leading                     |
                +------------------------------------------+
                |   *****                                  |
                |  *** ***                                 |
                | ***   ***                                |
                |***     ***  ** ****                      | ascent
                |***********  ********                     |
                |***********  ***   ***                    |
                |***     ***  ***    ***                   |
                |***     ***  ***   ***                    |
                |***     ***  ********                     |
                +------------------------------------------+ baseline
                |             ***                          |
                |             ***                          | descent
                |            ****                          |
                +------------------------------------------+
                |    external leading                      |
                +------------------------------------------+
                |-----------| 
                |-|       |-| 
                             | |-----| |
                             |-|     |-|
             */

             // Draw the Internal Leading dimension
             pdc->SelectObject(&internalLeadingBrush);
             pdc->SelectObject(&internalLeadingPen);
             pdc->Rectangle(0, top, right, top+desc.tm.tmInternalLeading);


             pdc->SelectObject(&ascentBrush);
             pdc->SelectObject(&ascentPen);
             pdc->Rectangle(0, top + desc.tm.tmInternalLeading,
                                    right, 
                                    top + desc.tm.tmHeight - desc.tm.tmDescent);

             pdc->SelectObject(&descentBrush);
             pdc->SelectObject(&descentPen);
             pdc->Rectangle(0, top + desc.tm.tmHeight - desc.tm.tmDescent,
                               right,
                               top + desc.tm.tmHeight);

             pdc->SelectObject(&externalLeadingBrush);
             pdc->SelectObject(&externalLeadingPen);
             pdc->Rectangle(0, top + desc.tm.tmHeight, right, 
                            top+desc.tm.tmHeight + desc.tm.tmExternalLeading);

            } /* show colors */

        KERNINFO ki(&dc);


        int pos = 0;
        int lead = 0;
        int baseoffset = 0;
        if(s.GetLength() > 0)
           { /* compute base offset */
            CSaveDC pdc(&dc);
            pdc->SetMapMode(MM_TEXT);
            ABC abc;
            if(dc.GetCharABCWidths(s[0], s[0], &abc))
               { /* TrueType */
                if(abc.abcA < 0)
                   baseoffset = -abc.abcA;
               } /* TrueType */
           } /* compute lead */

        for(int i = 0; i < s.GetLength(); i++)
           { /* draw ABCs */
            TCHAR c[2];
            ABC abc;
            int offset = 0;

            { 
             CSaveDC pdc(&dc);
             pdc->SetMapMode(MM_TEXT);
             if(!dc.GetCharABCWidths(s[i], s[i], &abc))
                { /* TrueType */
                 abc.abcA = abc.abcC = 0;
                 c[0] = s[i];
                 CSize sz = dc.GetTextExtent(c, 1);
                 abc.abcB = sz.cx;
                } /* TrueType */
             if(abc.abcA < 0)
                offset = abc.abcA;
            }

            if(opts.chrule)
               drawABCs(dc, pos + baseoffset + offset, s[i], top, bottom + (i % 2) * LINEOFFSET, i % 2);
            if(!opts.basal)
               { /* hand-layout */

                c[0] = s[i];

                dc.SetTextColor(color);

                int outpos = pos + baseoffset + offset;
                dc.TextOut(outpos, 0, c, 1);
#ifdef _DEBUG
                pinfo[i].ch = s[i];
                pinfo[i].pos = outpos;
#endif          

                if(opts.kerning && ki.count > 0)
                   { /* modify by pair kerning */
                    if(i < s.GetLength() - 1) // no pair at end of string
                       { /* still in range */
                        LPKERNINGPAIR pair;
                        pair = ki.findPair((UINT)s[i], (UINT)s[i+1]);
                        if(pair != NULL)
                           { /* kerned */
                            pos += pair->iKernAmount;
                           } /* kerned */
                       } /* still in range */
                   } /* modify by pair kerning */
               } /* hand-layout */

            // Modify by track kerning
            if(opts.track)
               pos += opts.trackval;

            pos += abc.abcA + abc.abcB + abc.abcC ;
            } /* draw ABCs */

        if(opts.basal)
           { /* use base textout */
            if(opts.track)
               dc.SetTextCharacterExtra(opts.trackval);
            dc.SetTextColor(color);
            dc.TextOut(baseoffset, 0, s, s.GetLength());
#ifdef _DEBUG
            int p = baseoffset;

            for(int i = 0; i < s.GetLength(); i++)
               { /* record data */
                pinfo[i].ch = s[i];
                pinfo[i].pos = p;
                SIZE sz = dc.GetTextExtent(&pinfo[i].ch, 1);
                p += sz.cx;
               } /* record data */
#endif
           } /* use base textout */

        {
         CSaveDC sdc(&dc);
         int y = desc.tm.tmHeight - desc.tm.tmDescent;
         sdc->MoveTo(0, y);
         sdc->LineTo(right, y);
        }

        if(opts.vrule)
           writeRulerV(dc, 0, top, 
                       top + desc.tm.tmHeight + desc.tm.tmExternalLeading);

        if(opts.hrule)
           writeRulerH(dc, top + desc.tm.tmHeight + desc.tm.tmExternalLeading, 
                        0, right);
}

/****************************************************************************
*                         CFontDisplay::OnEraseBkgnd
* Inputs:
*       CDC * pDC: Display context
* Result: BOOL
*       CStatic::OnEraseBkgnd
* Effect: 
*       Erases the background
****************************************************************************/

BOOL CFontDisplay::OnEraseBkgnd(CDC* pDC) 
{
 CBrush bkg;
 CRect r;

 CSaveDC dc(pDC);

 bkg.CreateSolidBrush(opts.white ? RGB(255, 255, 255) 
                            : ::GetSysColor(COLOR_BTNFACE));
 GetClientRect(&r);

 dc->SelectObject(&bkg);
 dc->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);

 return CStatic::OnEraseBkgnd(pDC);
}

/****************************************************************************
*                         CFontDisplay::OnRButtonDown
* Inputs:
*       UINT nFlags: ignored, except as passed to parent
*       CPoint point: Place where mouse click occurred
* Result: void
*       
* Effect: 
*       If not already popped up, pops up a window showing the parameters
*       used to compute the font display
****************************************************************************/

void CFontDisplay::OnRButtonDown(UINT nFlags, CPoint point) 
{
 if(GetCapture() == this)
    { /* already captured */
     dismissPopup(point); // if rdown but outside window, dismiss it
     return;   
    } /* already captured */

 CFontDescriptor desc(this, GetParent());

 popup = new CPopupList();
 SetCapture();

 CRect r;
 GetClientRect(&r);
 popup->Create(WS_VISIBLE | WS_BORDER | WS_CHILD, r, this, 0);

 CString t;
 GetWindowText(t);

 SIZE sz;
 
 CString s;
 CString fmt;
 CClientDC dc(this);

 dc.SelectObject(desc.font);

 GetClientRect(&r);

 if(opts.debug)
    { /* debug only */
     fmt.LoadString(IDS_RECT_EXTENT);
     s.Format(fmt, r.Width(), r.Height());
     popup->AddString(s);

     fmt.LoadString(IDS_TEXT_EXTENT);
     sz = dc.GetTextExtent(t);
     s.Format(fmt, sz.cx, sz.cy);
     popup->AddString(s);
 
     fmt.LoadString(IDS_TEXT_OVERHANG);
     sz = getOverhangTextExtent(dc, t);
     sz.cy = requiredHeight(desc);
     s.Format(fmt, sz.cx, sz.cy);
     popup->AddString(s);

     scaling sc = computeScaling(r, sz);
     fmt.LoadString(IDS_FSCALE);
     s.Format(fmt, sc.sx, sc.sy);
     popup->AddString(s);
 

     // Note that a raster font can be scaled at most 5X horizontally and
     // 8X vertically, so if the font is a raster font we limit it
     // to 5X

     iscale scale = normalizeScaling(limitScaling(desc.tm, min(sc.sx, sc.sy)));

     fmt.LoadString(IDS_ISCALE);
     s.Format(fmt, scale.win, scale.view);
     popup->AddString(s);

     fmt.LoadString(IDS_SCALED_EXTENT);
     s.Format(fmt, (sz.cx * scale.view) / scale.win, (sz.cy * scale.view) / scale.win);
     popup->AddString(s);

     fmt.LoadString(IDS_DRAWCODE);

     for(int i = 0; i < t.GetLength(); i++)
        { /* list widths */
         TCHAR ch = t[i];
         
         int code = GetDisplayCode(dc, ch);

         s.Format(fmt, ch, ch, ch, code);
         popup->AddString(s);
        } /* list widths */
    } /* debug only */
 else
    { /* normal user mode */
     { /* protect DC */
      CSaveDC pdc(&dc);
      pdc->SetMapMode(MM_TEXT);

      CString s;
      fmt.LoadString(IDS_TEXT_EXTENT);
      CSize sz = pdc->GetTextExtent(t);
      s.Format(fmt, sz.cx, sz.cy);

      fmt.LoadString(IDS_ABC_WIDTHS);
      for(int i = 0; i < t.GetLength(); i++)
         { /* format each ABC */
          TCHAR ch = t[i];
          ABC abc;

          getABC(pdc, ch, abc);
          s.Format(fmt, ch, ch, ch, abc.abcA, abc.abcB, abc.abcC);

#ifdef _DEBUG
          {
           CString p;
           p.Format(_T(" @ %d"), pinfo[i].pos);
           s += p;
          }
#endif

          popup->AddString(s);
         } /* format each ABC */

      KERNINFO ki(pdc);

      if(ki.count > 0)
         { /* has kerning */

          for(int i = 0; i < t.GetLength() - 1; i++)
             { /* format kerning pairs */
              LPKERNINGPAIR pair = ki.findPair((UINT)t[i], (UINT)t[i+1]);
              if(pair != NULL)
                 { /* show it */
                  s.Format(_T("\"%c\"-\"%c\": %d"), t[i], t[i+1], 
                                                    pair->iKernAmount);
                  popup->AddString(s);
                 } /* show it */
             } /* format kerning pairs */
         } /* has kerning */
     } /* protect DC */     
    } /* normal user mode */

 //============================================================================
 // Having filled the box, we now shrink it to fit
 //============================================================================

 int n = popup->GetCount();  // get number of lines
 int ht = popup->GetItemHeight(0); // get a typical item height (fixed)
 CRect w;
 popup->GetWindowRect(&w);
 // If the list box is smaller than the area, anchor it to the bottom but
 // make it no higher than needed
 if(n * ht < w.Height())
    { /* shrink it */
     if(opts.debug)
        { /* show at bottom */
         w.top = w.bottom - (n * ht);
        } /* show at bottom */
     else
        { /* show at top */
         w.bottom = w.top + (n * ht);
        } /* show at top */
     ScreenToClient(&w);
     popup->MoveWindow(&w, FALSE);
    } /* shrink it */
}

/****************************************************************************
*                          CFontDisplay::OnRButtonUp
* Inputs:
*       UINT nFlags: ignored, except as passed on
*       CPoint point: Place where mouse button click occurred
* Result: void
*       
* Effect: 
*       If the button goes up when we have capture, we pass it into the
*       popup.  We reestablish capture because it gets dismissed.
****************************************************************************/

void CFontDisplay::OnRButtonUp(UINT nFlags, CPoint point) 
{
 if(GetCapture() == this)
    { /* pass it thru */
     MapWindowPoints(popup, &point, 1);
     popup->DoRButtonUp(nFlags, point);
     if(GetCapture() != this)
        SetCapture();  // make sure we restore capture
     return;
    } /* pass it thru */
        
 CStatic::OnRButtonUp(nFlags, point);
}

/****************************************************************************
*                                dismissPopup
* Inputs:
*       CPoint point: Point of mouse click
* Result: BOOL
*       TRUE if we have dismissed popup window, or there was none
*       FALSE if click was within popup
* Effect: 
*       If mouse click is outside popup window, dismisses the popup window
****************************************************************************/

BOOL CFontDisplay::dismissPopup(CPoint point)
    {
     if(GetCapture() != this)
        return TRUE;

     CRect r;
     popup->GetWindowRect(&r);
     popup->ScreenToClient(&r);
     if(r.PtInRect(point))
        return FALSE;  // ignore mouse clicks inside popup window

     // mouse click was outside window
     popup->DestroyWindow();  // get rid of popup
     ReleaseCapture();
     return TRUE;
     
    }


/****************************************************************************
*                         CFontDisplay::OnLButtonDown
* Inputs:
*       UINT nFlags:
*       CPoint point:
* Result: void
*       
* Effect: 
*       If the button goes down outside the popup, the popup is dismissed
*       If inside, it is passed to the popup.
*       We have to re-establish capture because it is dismissed somehow
****************************************************************************/

void CFontDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
 if(!dismissPopup(point))
    { /* pass it in */
     MapWindowPoints(popup, &point, 1);
     popup->DoLButtonDown(nFlags, point);
     if(GetCapture() != this)
        SetCapture();  // make sure we restore capture
     return;
    } /* pass it in */

 CStatic::OnLButtonDown(nFlags, point);
}

/****************************************************************************
*                          CFontDisplay::OnNcHitTest
* Inputs:
*       CPoint point: Mouse position, expressed in screen coordinates (NC-
*               style handlers use screen coordinates)
* Result: UINT
*       HTCLIENT if mouse is in client area of window
*       CStatic::OnNcHitTest() otherwise
* Notes: 
*       The default appears to be HTTRANSPARENT, so if we expect to handle
*       any of the events such as mouse down, etc. we have to return HTCLIENT
****************************************************************************/

UINT CFontDisplay::OnNcHitTest(CPoint point) 
{
 CRect r;
 GetClientRect(&r);
 ClientToScreen(&r);
 
 if(r.PtInRect(point))
    return HTCLIENT;
        
        return CStatic::OnNcHitTest(point);
}

void CFontDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
 if(!dismissPopup(point))
    { /* pass it thru */
     MapWindowPoints(popup, &point, 1);
     popup->DoLButtonUp(nFlags, point);
     if(GetCapture() != this)
        SetCapture();  // make sure we restore capture
     return;
    } /* pass it thru */
        
 CStatic::OnLButtonUp(nFlags, point);
}

/****************************************************************************
*                         CFontDisplayOnLButtonDblClk
* Inputs:
*       UINT nFlags: ignored except to pass forward or upward
*       CPoint point: Place of mouse click
* Result: void
*       
* Effect: 
*       If the double click is outside the popup space, dismisses the
*       popup.  Otherwise passes the click into the popup.  Upon completion,
*       reasserts capture, which for some reason has been killed by the
*       window into which we pass the mouse click.  
****************************************************************************/

void CFontDisplay::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
 if(!dismissPopup(point))
    { /* pass it thru */
     MapWindowPoints(popup, &point, 1);
     popup->DoLButtonDblClk(nFlags, point);
     if(GetCapture() != this)
        SetCapture();  // make sure we restore capture
     return;
    } /* pass it thru */
        
        CStatic::OnLButtonDblClk(nFlags, point);
}

/****************************************************************************
*                        CFontDisplay::PreCreateWindow
* Inputs:
*       CREATESTRUCT & cs: creation structure
* Result: BOOL
*       Result of CStatic::PreCreateWindow
* Effect: 
*       Adds the WS_CLIPCHILDREN flag.  This is because we pop up a
*       display window on top of the window, and if we do anything that 
*       causes the app to redisplay (e.g., uncovering it) while the info
*       box is being displayed, the underlying window "shows through" the
*       text of the list box.
****************************************************************************/

BOOL CFontDisplay::PreCreateWindow(CREATESTRUCT& cs) 
{
 cs.style |= WS_CLIPCHILDREN;
        
        return CStatic::PreCreateWindow(cs);
}

/****************************************************************************
*                           CFontDisplay::OnCreate
* Inputs:
*       LPCREATESTRUCT lpCreateStruct: ignored except as passed on
* Result: int
*       -1 if error
*       0 if success
* Effect: 
*       ²
****************************************************************************/

int CFontDisplay::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
        if (CStatic::OnCreate(lpCreateStruct) == -1)
                return -1;
        
        legendFont.CreateFont(-1, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
                                ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                                CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                FF_DONTCARE,
                                _T("Arial"));
        
        return 0;
}

void CFontDisplay::OnDestroy() 
{
        CStatic::OnDestroy();
        
        legendFont.DeleteObject();
        
}


/****************************************************************************
*                         CFontDisplay::OnCancelMode
* Result: void
*       
* Effect: 
*       This is necessary because we can lose focus in ways outside our
*       control, and when we return, mouse clicking outside the popup
*       won't dismiss it because we don't have capture.  WM_CANCELMODE is
*       used, among other objectives, to cancel capture, so this is our
*       best intercept point to destroy the popup.
****************************************************************************/

void CFontDisplay::OnCancelMode() 
{
        if(GetCapture() == this)
           { /* has capture */
            ReleaseCapture();
            popup->DestroyWindow();
           } /* has capture */

        CStatic::OnCancelMode();
        
        
}

void CFontDisplay::OnSize(UINT nType, int cx, int cy) 
{
        CStatic::OnSize(nType, cx, cy);
        
        InvalidateRect(NULL);
        
}
