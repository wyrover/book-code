// Blt.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "source.h"
#include "dest.h"
#include "IntEdit.h"
#include "image.h"
#include "idcombo.h"
#include "bmselect.h"
#include "brdemo.h"
#include "bmw.h"
#include "strmode.h"

#include "extrops.h"

#include "Blt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlt property page

IMPLEMENT_DYNCREATE(CBlt, CPropertyPage)

CBlt::CBlt() : CPropertyPage(CBlt::IDD)
{
 lasterror = 0;
}
            

CBlt::CBlt(int caption) : CPropertyPage(CBlt::IDD, caption)
{
 initialized = FALSE;
 lasterror = 0;

        //{{AFX_DATA_INIT(CBlt)
        //}}AFX_DATA_INIT
}

CBlt::~CBlt()
{
}

void CBlt::DoDataExchange(CDataExchange* pDX)
{
        CPropertyPage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CBlt)
        DDX_Control(pDX, IDC_XSRC_CAPTION, c_c_SrcX);
        DDX_Control(pDX, IDC_YSRC_CAPTION, c_c_SrcY);
        DDX_Control(pDX, IDC_ROP_BORDER, c_Rop_Caption);
        DDX_Control(pDX, IDC_YMASKSPIN, c_YMaskSpin);
        DDX_Control(pDX, IDC_YMASK_CAPTION, c_c_YMask);
        DDX_Control(pDX, IDC_YMASK, c_YMask);
        DDX_Control(pDX, IDC_XMASKSPIN, c_XMaskSpin);
        DDX_Control(pDX, IDC_XMASK_CAPTION, c_c_XMask);
        DDX_Control(pDX, IDC_XMASK, c_XMask);
        DDX_Control(pDX, IDC_WIDTH, c_c_SourceWidth);
        DDX_Control(pDX, IDC_HEIGHT, c_c_SourceHeight);
        DDX_Control(pDX, IDC_PT2_Y, c_c_pt2y);
        DDX_Control(pDX, IDC_PT2_X, c_c_pt2x);
        DDX_Control(pDX, IDC_PT1_Y, c_c_pt1y);
        DDX_Control(pDX, IDC_PT1_X, c_c_pt1x);
        DDX_Control(pDX, IDC_PT0_Y, c_c_pt0y);
        DDX_Control(pDX, IDC_PT0_X, c_c_pt0x);
        DDX_Control(pDX, IDC_ROP_CAPTION, c_c_Rop);
        DDX_Control(pDX, IDC_MASK_CTL_CAPTION, c_c_MaskCtl);
        DDX_Control(pDX, IDC_CLEARMASK, c_ClearMask);
        DDX_Control(pDX, IDC_SETMASK, c_SetMask);
        DDX_Control(pDX, IDC_EXTENDED, c_Extended);
        DDX_Control(pDX, IDC_SRCLRYSPIN, c_SrclrYSpin);
        DDX_Control(pDX, IDC_SRCLRXSPIN, c_SrclrXSpin);
        DDX_Control(pDX, IDC_PLGYSPIN, c_PlgYSpin);
        DDX_Control(pDX, IDC_PLGY, c_PlgY);
        DDX_Control(pDX, IDC_PLGXSPIN, c_PlgXSpin);
        DDX_Control(pDX, IDC_PLGX, c_PlgX);
        DDX_Control(pDX, IDC_BITMAP_CAPTION, c_c_Bitmap);
        DDX_Control(pDX, IDC_SRCLRY, c_SrclrY);
        DDX_Control(pDX, IDC_SRCLRX, c_SrclrX);
        DDX_Control(pDX, IDC_BRUSH_CAPTION, c_c_Brush);
        DDX_Control(pDX, IDC_ERROR, c_Error);
        DDX_Control(pDX, IDC_CLEARW, c_ClearW);
        DDX_Control(pDX, IDC_CLEARB, c_ClearB);
        DDX_Control(pDX, IDC_DEST, c_Dest);
        DDX_Control(pDX, IDC_SOURCE, c_Source);
        DDX_Control(pDX, IDC_ULYSPIN, c_ulYSpin);
        DDX_Control(pDX, IDC_ULXSPIN, c_ulXSpin);
        DDX_Control(pDX, IDC_LRYSPIN, c_lrYSpin);
        DDX_Control(pDX, IDC_LRY, c_lrY);
        DDX_Control(pDX, IDC_LRXSPIN, c_lrXSpin);
        DDX_Control(pDX, IDC_LRX, c_lrX);
        DDX_Control(pDX, IDC_SRCYSPIN, c_SrcYSpin);
        DDX_Control(pDX, IDC_SRCY, c_SrcY);
        DDX_Control(pDX, IDC_SRCXSPIN, c_SrcXSpin);
        DDX_Control(pDX, IDC_SRCX, c_SrcX);
        DDX_Control(pDX, IDC_ULY, c_ulY);
        DDX_Control(pDX, IDC_ULX, c_ulX);
        DDX_Control(pDX, IDC_DOBLT, c_DoBlt);
        DDX_Control(pDX, IDC_BITMAP, c_BitmapSelect);
        DDX_Control(pDX, IDC_OP, c_Op);
        DDX_Control(pDX, IDC_MASK, c_Mask);
        DDX_Control(pDX, IDC_MASK_CAPTION, c_c_Mask);
        DDX_Control(pDX, IDC_BRUSH, c_BrDemo);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlt, CPropertyPage)
        //{{AFX_MSG_MAP(CBlt)
        ON_EN_CHANGE(IDC_SRCX, OnChangeSrcx)
        ON_EN_CHANGE(IDC_SRCY, OnChangeSrcy)
        ON_EN_CHANGE(IDC_LRX, OnChangeLrx)
        ON_EN_CHANGE(IDC_LRY, OnChangeLry)
        ON_EN_CHANGE(IDC_ULX, OnChangeUlx)
        ON_EN_CHANGE(IDC_ULY, OnChangeUly)
        ON_BN_CLICKED(IDC_DOBLT, OnDoblt)
        ON_BN_CLICKED(IDC_CLEARW, OnClearW)
        ON_BN_CLICKED(IDC_CLEARB, OnClearB)
        ON_CBN_SELENDOK(IDC_BITMAP, OnSelendokBitmap)
        ON_BN_CLICKED(IDC_EXTENDED, OnExtended)
        ON_EN_CHANGE(IDC_SRCLRX, OnChangeSrclrx)
        ON_EN_CHANGE(IDC_SRCLRY, OnChangeSrclry)
        ON_BN_CLICKED(ID_EDIT_COPY, OnCopy)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlt message handlers

/****************************************************************************
*                             CBlt::endInitDialog
* Result: void
*       
* Effect: 
*       Finishes up dialog initialization
****************************************************************************/

void CBlt::endInitDialog()
    {
    //================================================================
    initialized = TRUE;
    //================================================================

    makeOp();

    updateSource();

    updateControls();

     
    }

/****************************************************************************
*                               CBlt::maskShow
* Inputs:
*       BOOL show: TRUE to show, FALSE to hide
* Result: void
*       
* Effect: 
*       Shows all the mask-related controls
****************************************************************************/

void CBlt::maskShow(BOOL show)
    {
     int sw = (show ? SW_SHOW : SW_HIDE);
     int nsw= (show ? SW_HIDE : SW_SHOW);

     c_SetMask.ShowWindow(sw);
     c_ClearMask.ShowWindow(sw);
     c_c_MaskCtl.ShowWindow(sw);
     c_c_Mask.ShowWindow(sw);
     c_Mask.ShowWindow(sw);
     c_XMask.ShowWindow(sw);
     c_c_XMask.ShowWindow(sw);
     c_XMaskSpin.ShowWindow(sw);
     c_YMask.ShowWindow(sw);
     c_c_YMask.ShowWindow(sw);
     c_YMaskSpin.ShowWindow(sw);
     
    }


/****************************************************************************
*                                CBlt::plgShow
* Inputs:
*       BOOL show: TRUE to show, FALSE to hide
* Result: void
*       
* Effect: 
*       Shows or hides the PlgBlt coordinate controls
****************************************************************************/

void CBlt::plgShow(BOOL show)
    {
     int sw = (show ? SW_SHOW : SW_HIDE);
     int nsw= (show ? SW_HIDE : SW_SHOW);

     c_PlgX.ShowWindow(sw);
     c_PlgXSpin.ShowWindow(sw);
     c_PlgY.ShowWindow(sw);
     c_PlgYSpin.ShowWindow(sw);
     c_SrclrYSpin.ShowWindow(sw);
     c_SrclrXSpin.ShowWindow(sw);
     c_SrclrX.SetReadOnly(!show);
     c_SrclrY.SetReadOnly(!show);
     c_c_pt1x.ShowWindow(sw);
     c_c_pt1y.ShowWindow(sw);
     c_c_pt2x.ShowWindow(sw);
     c_c_pt2y.ShowWindow(sw);

     c_SrclrXSpin.ShowWindow(sw);
     c_SrclrYSpin.ShowWindow(sw);
     c_SrclrXSpin.EnableWindow(show);
     c_SrclrYSpin.EnableWindow(show);

     c_SrclrX.SetReadOnly(!show);
     c_SrclrY.SetReadOnly(!show);

     maskShow(show);

     c_Rop_Caption.ShowWindow(nsw);  // HIDE this for plg
     c_c_Rop.ShowWindow(sw);         // replace with this caption
    }

/****************************************************************************
*                             CBlt::makeOp
* Result: void
*       
* Effect: 
*       Formats the operation in printable form
*
* Notes:
*       This is not implemented in this class but by each of the subclasses
****************************************************************************/

void CBlt::makeOp()
    {
     // Does nothing
    }

BOOL CBlt::OnInitDialog() 
{
        CPropertyPage::OnInitDialog();
        
        CRect r;

        c_Dest.getObjectRect(&r);
        
        c_ulX.SetVal(0);
        c_ulY.SetVal(0);
        c_lrX.SetVal(r.Width());
        c_lrY.SetVal(r.Height());
        c_ulXSpin.SetRange(0,r.Width());
        c_ulYSpin.SetRange(0,r.Height());
        c_lrXSpin.SetRange(0,r.Width() );
        c_lrYSpin.SetRange(0,r.Height());


        c_Source.getObjectRect(&r);
        c_SrcX.SetVal(0);
        c_SrcY.SetVal(0);
        c_SrcXSpin.SetRange(0, r.Width() - 1);
        c_SrcYSpin.SetRange(0, r.Height() - 1);

        c_Error.SetWindowText(_T(""));

        c_BitmapSelect.Load(); // no selection by ID
        c_BitmapSelect.SetCurSel(0); // select by position, position 0

        c_Dest.createBitmap();

        // For now, assume that we are *not* a PlgBlt window
        plgShow(FALSE);

        // Place the correct opcode in the button
        CString s;
        s.LoadString(opid);
        c_DoBlt.SetWindowText(s);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CBlt::OnChangeSrcx() 
{
 
        makeOp();
        
}

void CBlt::OnChangeSrcy() 
{
        makeOp();
        
}

void CBlt::OnChangeLrx() 
{
        makeOp();
        
}

void CBlt::OnChangeLry() 
{
        makeOp();
        
}

void CBlt::OnChangeUlx() 
{
        makeOp();
        
}

void CBlt::OnChangeUly() 
{
        makeOp();
        
}


/****************************************************************************
*                                 CBlt::doBLT
* Result: void
*       
* Effect: 
*       Does nothing.  This must be implemented in derived classes from this
*       abstract base class
****************************************************************************/

void CBlt::doBLT()
    {
     // Does nothing.  Subclasses must implement this
     ASSERT(FALSE); // this should never happen
    }

/****************************************************************************
*                              CBlt::OnDoblt
* Result: void
*       
* Effect: 
*       Performs the BitBlt desired.  This works by doing the BitBlt from
*       the offscreen bitmap in source to the offscreen bitmap in dest,
*       then forcing dest to redraw.
****************************************************************************/

void CBlt::OnDoblt() 
{
 doBLT();
}

void CBlt::OnClearW() 
{
 c_Dest.clearBitmap();
}

void CBlt::OnClearB()
{
 c_Dest.clearBitmap(0);
}

/****************************************************************************
*                               CBlt::isSourceRop
* Inputs:
*       int rop: ROP code
* Result: BOOL
*       TRUE if ROP is a source-using ROP 
*       FALSE if ROP is not a source-using ROP
****************************************************************************/

BOOL CBlt::isSourceRop(int rop)
    {
     // These come from the extended ROP codes...

     switch(rop)
        { /* extended rop code */
         case extrop_0:                 // 00: BLACKNESS
         case extrop_DPon:              // 05: --
         case extrop_DPna:              // 0A: --
         case extrop_Pn:                // 0F: --
         case extrop_PDna:              // 50: --
         case extrop_Dn:                // 55: DSTINVERT
         case extrop_DPx:               // 5A: PATINVERT
         case extrop_DPan:              // 5F: --
         case extrop_DPa:               // A0: --
         case extrop_PDxn:              // A5: --
         case extrop_D:                 // AA: --
         case extrop_DPno:              // AF: --
         case extrop_P:                 // F0: PATCOPY
         case extrop_PDno:              // F5: --
         case extrop_DPo:               // FA: --
         case extrop_1:                 // FF: WHITENESS
                 return FALSE;
        } /* extended rop code */
         
     return TRUE;
     
    }

/****************************************************************************
*                               CBlt::isPatRop
* Inputs:
*       int rop: ROP code
* Result: BOOL
*       TRUE if the rop requires a pattern (brush)
*       FALSE if the rop does not require a pattern
****************************************************************************/

BOOL CBlt::isPatRop(int rop)
    {
     switch(rop)
        { /* has pattern? */
         case extrop_0:    // BLACKNESS
         case extrop_DSon: // NOTSRCERASE
         case extrop_DSna:
         case extrop_Sn:   // NOTSRCCOPY
         case extrop_SDna:
         case extrop_Dn:   // DSTINVERT
         case extrop_DSx:  // SRCINVERT
         case extrop_DSan:
         case extrop_DSa:  // SRCAND
         case extrop_DSxn:
         case extrop_D:
         case extrop_DSno: // MERGEPAINT
         case extrop_S:    // SRCCOPY
         case extrop_SDno:
         case extrop_DSo:  // SRCPAINT
         case extrop_1:    // WHITENESS
                 return FALSE;
        } /* has pattern? */
     return TRUE;
    }
            

/****************************************************************************
*                             CBlt::hasSource
* Result: BOOL
*       TRUE if the rop operator uses a source operand
*       FALSE if it does not use a source operand
* Notes:
*       The base class always returns true; the derived classes may return
*       false as well
****************************************************************************/

BOOL CBlt::hasSource()
    {
     return TRUE;
    }

/****************************************************************************
*                         CBlt::showSourceControls
* Inputs:
*       BOOL show: TRUE to show, FALSE to hide
* Result: void
*       
* Effect: 
*       Shows or hides all the source controls
****************************************************************************/

void CBlt::showSourceControls(BOOL show)
    {
     int sw = (show ? SW_SHOW : SW_HIDE);

     if(!initialized)
        return;

     c_SrcXSpin.ShowWindow(sw);
     c_SrcYSpin.ShowWindow(sw);
     c_SrcX.ShowWindow(sw);
     c_SrcY.ShowWindow(sw);
     c_SrclrX.ShowWindow(sw);
     c_SrclrY.ShowWindow(sw);
     c_BitmapSelect.ShowWindow(sw);
     c_Source.ShowWindow(sw);
     c_c_Bitmap.ShowWindow(sw);
     c_c_SrcX.ShowWindow(sw);
     c_c_SrcY.ShowWindow(sw);
     c_c_SourceWidth.ShowWindow(sw);
     c_c_SourceHeight.ShowWindow(sw);
     
     // We only show the spin controls if they are enabled and we are
     // in show mode; otherwise they are always hidden

     int sw1 = show && c_SrclrXSpin.IsWindowEnabled() ? SW_SHOW : SW_HIDE;
     c_SrclrXSpin.ShowWindow(sw1);
     c_SrclrYSpin.ShowWindow(sw1);
    }

/****************************************************************************
*                           CBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CBlt::updateControls()
    {
     if(!initialized)
         return;

     showSourceControls(hasSource());
         
     if(hasSource())
        { /* has source */
         CBitmap * bitmap = c_Source.getBitmap();
         CClientDC dc(&c_Source);
         if(bitmap != NULL)
            { /* has source bitmap */
             CSize newsz = getBitmapSize(&dc, *bitmap);
             CSize oldsz(c_SrcX.GetVal(), c_SrcY.GetVal());

             // Constrain the starting position to be within the size

             if(oldsz.cx >= newsz.cx)
                { /* make smaller x */
                 c_SrcX.SetVal(newsz.cx - 1);
                 c_SrcXSpin.SetRange(0, newsz.cx - 1);
                } /* make smaller x */

             if(oldsz.cy >= newsz.cy)
                { /* make smaller y */
                 c_SrcY.SetVal(newsz.cy - 1);
                 c_SrcYSpin.SetRange(0, newsz.cy - 1);
                } /* make smaller y */

             c_SrcXSpin.SetRange(0, newsz.cx - 1);
             c_SrcYSpin.SetRange(0, newsz.cy - 1);

             // Constrain the height and width of the destination to not exceed
             // the source dimensions.  

             // TODO: implement this

             // Set size parameters for display.  Do test to avoid recursive
             // loop caused by this set triggering another EN_CHANGE 
             // notification

             if(c_SrclrX.GetVal() != newsz.cx)
                c_SrclrX.SetVal(newsz.cx);

             if(c_SrclrY.GetVal() != newsz.cy)
                c_SrclrY.SetVal(newsz.cy);

             // Set the spin range in case we actually use it

             c_SrclrXSpin.SetRange(0, newsz.cx);
             c_SrclrYSpin.SetRange(0, newsz.cy);

             // Now enable the controls for setting the x,y source position

             c_SrcXSpin.EnableWindow(TRUE);
             c_SrcYSpin.EnableWindow(TRUE);
             c_SrcX.EnableWindow(TRUE);
             c_SrcY.EnableWindow(TRUE);
             c_SrclrX.ShowWindow(SW_SHOW);
             c_SrclrY.ShowWindow(SW_SHOW);

             // Show the spin controls only if they are enabled...
             if(c_SrclrXSpin.IsWindowEnabled())
                c_SrclrXSpin.ShowWindow(SW_SHOW);
             if(c_SrclrYSpin.IsWindowEnabled())
                c_SrclrYSpin.ShowWindow(SW_SHOW);
            } /* has source bitmap */
         else
            { /* no source bitmap */
             c_SrcXSpin.EnableWindow(FALSE);
             c_SrcYSpin.EnableWindow(FALSE);
             c_SrcX.EnableWindow(FALSE);
             c_SrcY.EnableWindow(FALSE);
             c_SrclrX.ShowWindow(SW_HIDE);
             c_SrclrY.ShowWindow(SW_HIDE);
             c_SrclrXSpin.ShowWindow(SW_HIDE);
             c_SrclrYSpin.ShowWindow(SW_HIDE);
            } /* no source bitmap */
        } /* has source */

     enableOp();
    }

/****************************************************************************
*                          CBlt::getSourceBitmapSize
* Result: CSize
*       Size of bitmap, or 0,0 if not loaded
****************************************************************************/

CSize CBlt::getSourceBitmapSize()
    {
     CBitmap * bm = c_Source.getBitmap();
     CClientDC dc(&c_Source);
     return getBitmapSize(&dc, *bm);
    }


/****************************************************************************
*                            CBlt::updateSource
* Inputs:
*       
* Result: void
*       
* Effect: 
*       Updates the source bitmap
****************************************************************************/

void CBlt::updateSource()
    {
     int item = c_BitmapSelect.GetCurItem();
     c_Source.setBitmap(item);
    }

void CBlt::OnSelendokBitmap() 
{
 updateSource();
        
 updateControls();

}

/****************************************************************************
*                              CBlt::getRopName
* Inputs:
*       CString &s: Pointer to a string of the form "NAME: description"
* Result: CString
*       A string which contains only "NAME"
* Effect: 
*       Creates a new temporary string object
****************************************************************************/

CString CBlt::getRopName(CString &s)
    {
     return s.SpanExcluding(_T(":"));
    }

/****************************************************************************
*                                CBlt::loadRop
* Inputs:
*       BOOL ext: TRUE to load extended opcodes, FALSE to load normal
* Result: void
*       
* Effect: 
*       This has no effect; it must be defined in the derived classes to
*       have an implementation
****************************************************************************/

void CBlt::loadRop(BOOL ext)
    {
     // Does nothing.  Implement in subclasses
    }

void CBlt::OnExtended() 
{
 loadRop(c_Extended.GetCheck());
        
}


/****************************************************************************
*                           CBlt::OnChangeSrclr[xy]
* Result: void
*       
* Effect: 
*       Updates the set of controls including updating the opcode display.
*       If we have controls that are readonly or disabled, then any changes
*       were made by other parts of the program, not by user action, so
*       we avoid doing a redundant update by checking for these conditions.
****************************************************************************/

void CBlt::OnChangeSrclrx() 
{
    if(::IsWindow(c_SrclrX.m_hWnd) && c_SrclrX.IsWindowEnabled() && !c_SrclrX.GetReadOnly())
    makeOp();
        
}

void CBlt::OnChangeSrclry() 
{
    if(::IsWindow(c_SrclrY.m_hWnd) && c_SrclrY.IsWindowEnabled() && !c_SrclrY.GetReadOnly())
    makeOp();
        
}


/****************************************************************************
*                              CBlt::queryBrush
* Inputs:
*       LOGBRUSH * lb: Pointer to brush structure
* Result: BOOL
*       TRUE if a brush was returned
*       FALSE if no brush was returned
* Effect: 
*       Sends a query to all siblings.  If a sibling fills in the brush
*       structure, it returns TRUE having filled in the brush structure.
*       If the brush dialog is not active, no other dialog is supposed to
*       return a nonzero value (as from DefWindowProc).
****************************************************************************/

BOOL CBlt::queryBrush(LOGBRUSH * lb)
    {
     return QuerySiblings(0, (LPARAM)lb);
    }

BOOL CBlt::OnSetActive() 
{
        // TODO: Add your specialized code here and/or call the base class
        
        return CPropertyPage::OnSetActive();
}

/****************************************************************************
*                               CBlt::createBrush
* Inputs:
*       CBrush * br: Brush object to have real brush attached
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       If the brush selector is active, will create a real brush
****************************************************************************/

BOOL CBlt::createBrush(CBrush * br)
    {
     LOGBRUSH lbr;
     
     if(!queryBrush(&lbr))
        return FALSE;   // brush selector page not active

     if(br->m_hObject != NULL)
        br->DeleteObject();

     if(lbr.lbStyle == (UINT)-1)
        { /* stock brush */
         return br->CreateStockObject(lbr.lbColor);
        } /* stock brush */

     return br->CreateBrushIndirect(&lbr);
    }


/****************************************************************************
*                             CBlt::setLastError
* Result: void
*       
* Effect: 
*       Sets the lasterror code to the c_Error box
****************************************************************************/

void CBlt::setLastError()
    {
     if(lasterror == 0)
        { /* no error */
         c_Error.SetWindowText(_T(""));
         return;
        } /* no error */

     LPTSTR s;
     if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                     FORMAT_MESSAGE_FROM_SYSTEM,
                     NULL,
                     lasterror,
                     0,
                     (LPTSTR)&s,
                     0,
                     NULL) == 0)
        { /* failed */
         CString t;
         t.Format(_T("%08lx"), lasterror);
         c_Error.SetWindowText(t);
        } /* failed */
     else
        { /* success */
         c_Error.SetWindowText(s);
         LocalFree(s);
        } /* success */
     MessageBeep(MB_ICONERROR); // warn user that error occurred
    }

/****************************************************************************
*                               CBlt::enableOp
* Result: void
*       
* Effect: 
*       None.  Implement in subclass
****************************************************************************/

void CBlt::enableOp()
    {
     ASSERT(FALSE);
    }

/****************************************************************************
*                                CBlt::OnCopy
* Result: void
*       
* Effect: 
*       Creates a new bitmap, copies the contents of the target bitmap to it,
*       and adds the resulting bitmap to the clipboard.
* Notes:
*       The coordinates transferred are based on the origin and width
*       parameters for the destintation bitmap.
****************************************************************************/


void CBlt::OnCopy() 
    {
     CClientDC destDC(&c_Dest);

     // Create a bitmap into which we copy the current dest bitmap
     CDC memDC;
     memDC.CreateCompatibleDC(&destDC);

     CBitmap bm;
     CRect r;

     c_Dest.getObjectRect(&r);

     CPoint org = getCopyOrg();
     CSize ext = getCopyExt();

     bm.CreateCompatibleBitmap(&destDC, ext.cx, ext.cy);

     CBitmap * oldbm = memDC.SelectObject(&bm);

     memDC.BitBlt(0, 0, ext.cx, ext.cy, &destDC, org.x, org.y, SRCCOPY);
 
     OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();

     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
        
}


/****************************************************************************
*                              CBlt::getCopyOrg
* Result: CPoint
*       The point at which a blt copy is supposed to start
* Notes: 
*       This is the default operation.  More complex operations, such as
*       PlgBlt, may override this
****************************************************************************/

CPoint CBlt::getCopyOrg()
    {
     CRect r;
     c_Dest.getObjectRect(&r);
     return CPoint(r.left + c_ulX.GetVal(), r.top + c_ulY.GetVal());
    }

/****************************************************************************
*                              CBlt::getCopyExt
* Result: CSize
*       
* Effect: 
*       Returns the size of the copy bitmap.  More complex operations may
*       override this virtual method
****************************************************************************/

CSize CBlt::getCopyExt()
    {
     return CSize(c_lrX.GetVal(), c_lrY.GetVal());
    }
