#include <stdafx.h>

#include "dcvals.h"

/****************************************************************************
*                            CDCValues::CDCValues
* Effect: 
*       Initializes the structure
****************************************************************************/

CDCValues::CDCValues()
    {
     POINT zero = { 0, 0};
     POINT one  = { 1, 1};
     XFORM identity = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

     BkColor = RGB(255, 255, 255);
     BkMode = OPAQUE;
     current_bkmode = 0;
     align = TA_TOP | TA_LEFT | TA_NOUPDATECP;

     // 
     brush = new CBrush;
     brush->CreateStockObject(BLACK_BRUSH);

     brushorg = zero;
     penposition = zero;
     rop = R2_COPYPEN;

     font = NULL; 	       // no custom font to start
     textcharextra = 0;

     graphicsmode = GM_COMPATIBLE;

     mapmode = MM_TEXT;
     pen = new CPen;  
     pen->CreateStockObject(BLACK_PEN);

     fillmode = ALTERNATE;
     textcolor = RGB(0, 0, 0);
     miterlimit = 10.0f;

     matrix = identity;
     vext = one;
     wext = one;
     vorg = zero;
     worg = zero;

     // Window background brush color
     
     wcolor = GetSysColor(COLOR_WINDOW);
     // Initialize internal dialog state variables
     //
     // ----- Pen -----
     pensel = SEL_STOCK;
     stockpen = BLACK_PEN;
     pensolid = COLORREF(RGB(0, 0, 0));
     endcaps  = PS_ENDCAP_ROUND;
     linejoin = PS_JOIN_ROUND;
     lbstyle  = MAKELONG(BS_SOLID, 0);
     penwidth = 0;
     penstyle = PS_SOLID;

     // ----- Font -----
     // fontsel indicates if we should use stock or custom font
     stockfont = SYSTEM_FONT;  // the stock font to use
     fontsel = SEL_STOCK;      // default to stock font

     // ----- Brush -----
     brushsel = SEL_STOCK;
     stockbrush = WHITE_BRUSH;
     hatchsel = HS_FDIAGONAL;
     stocksel = BLACK_BRUSH;
     solidsel = RGB(0, 0, 0);

     // ----- Mapping mode -----
     ticks = 10;

     // ----- Last page -----
     lastpage = 0;

     // ----- miter angle -----
     angle = 12;  // initial miter example angle

    }

/****************************************************************************
*                            CDCValues::~CDCValues
* Effect: 
*       Frees up any resources being held
****************************************************************************/

CDCValues::~CDCValues()
    {
     if(brush != NULL)
	brush->DeleteObject();
     if(font != NULL)
	font->DeleteObject();
     if(pen != NULL)
	pen->DeleteObject();
    }

/****************************************************************************
*                              CDCValues::setDC
* Result: void
*       
* Effect: 
*       Sets the DC values.  Will not report failure if GM_ADVANCED cannot
*	be set, as it will assume it is running on Win95
****************************************************************************/

void CDCValues::setDC(CDC * dc)
    {

     dc->SetBkColor(BkColor);
     dc->SetBkMode(BkMode);

     dc->SetBrushOrg(brushorg);  // note that we must set brush org before
     				 // selecting the brush

     if(brush != NULL)
	dc->SelectObject(brush);

     dc->MoveTo(penposition.x, penposition.y);
     dc->SetROP2(rop);
     if(font != NULL)
	dc->SelectObject(font);
     dc->SetTextCharacterExtra(textcharextra);
     dc->SetMapMode(mapmode);
     if(pen != NULL)
	dc->SelectObject(pen);
     dc->SetPolyFillMode(fillmode);
     dc->SetTextAlign(align);
     dc->SetTextColor(textcolor);
     dc->SetMiterLimit(miterlimit);

     if(::SetGraphicsMode(dc->m_hDC, graphicsmode))
        { /* can set */
	 ::SetWorldTransform(dc->m_hDC, &matrix);
	} /* can set */

     // Note: SetWindowExt must be called *before* SetViewportExt
     // in case we are in MM_ISOTROPIC mode
     dc->SetWindowExt(wext.x, wext.y);
     dc->SetViewportExt(vext.x, vext.y);

     dc->SetViewportOrg(vorg.x, vorg.y);
     dc->SetWindowOrg(worg.x, worg.y);
    }

/****************************************************************************
*                              CDCValues::copyTo
* Inputs:
*       CDCValues * target: Place to copy the current DC to
* Result: BOOL
*       TRUE if successful
*	FALSE if an error
* Effect: 
*       Copies the DC values.  For GDI objects, we don't want sharing,
*	so we have to clone the existing objects
****************************************************************************/

BOOL CDCValues::copyTo(CDCValues * target)
    {
     *target = *this;  // copy all values across

     // Now that we have copied all the values, we have to clone the
     // GDI objects

     if(brush != NULL)
        { /* clone brush */
	 if(brush->m_hObject == NULL)
	    target->brush = NULL;
	 else
	    { /* clone it */
	     LOGBRUSH lb;
	     
	     brush->GetObject(sizeof(lb), &lb);
	     
	     target->brush = new CBrush;
	     target->brush->CreateBrushIndirect(&lb);
	    } /* clone it */
	} /* clone brush */

     if(font != NULL)
        { /* clone font */
	 if(font->m_hObject == NULL)
	    target->font = NULL;
         else
	    { /* clone it */
	     LOGFONT lf;
	     font->GetObject(sizeof(lf), &lf);
	     target->font = new CFont;
	     target->font->CreateFontIndirect(&lf);
	    } /* clone it */
	} /* clone font */

     if(pen != NULL)
        { /* clone pen */
	 if(pen->m_hObject == NULL)
	    target->pen = NULL;
         else
	    { /* clone it */
	     EXTLOGPEN lp;
	     LOGBRUSH lb;

	     pen->GetObject(sizeof(lp), &lp);
	     // This looks clumsy, and is, because of missing functionality
	     // in MFC 4.1; there is no direct way to create a pen from
	     // an EXTLOGPEN

	     lb.lbStyle = lp.elpBrushStyle;
	     lb.lbColor = lp.elpColor;
	     lb.lbHatch = lp.elpHatch;

	     target->pen = new CPen;
	     // Note that this re-create of the pen does not support
	     // user-defined dotted pens; the last two parameters are
	     // implicitly defaulted to 0, NULL

	     target->pen->CreatePen(lp.elpPenStyle, lp.elpWidth, &lb);
	    } /* clone it */
	} /* clone pen */

     return TRUE;
    }
