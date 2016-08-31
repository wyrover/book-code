// PlgBlt.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "source.h"
#include "dest.h"
#include "IntEdit.h"
#include "idcombo.h"
#include "gcombo.h"
#include "image.h"
#include "bmselect.h"
#include "strmode.h"
#include "brdemo.h"
#include "bmw.h"

#include "blt.h"
#include "genblt.h"
#include "PlgBlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlgBlt property page

IMPLEMENT_DYNCREATE(CPlgBlt, CGenBlt)

CPlgBlt::CPlgBlt() : CGenBlt(IDS_PLGBLT)
{
        //{{AFX_DATA_INIT(CPlgBlt)
        //}}AFX_DATA_INIT
}

CPlgBlt::~CPlgBlt()
{
}

void CPlgBlt::DoDataExchange(CDataExchange* pDX)
{
        CGenBlt::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPlgBlt)
        DDX_Control(pDX, IDC_ROP, c_Rop);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlgBlt, CGenBlt)
        //{{AFX_MSG_MAP(CPlgBlt)
        ON_CBN_SELENDOK(IDC_ROP, OnSelendokRop)
        ON_BN_CLICKED(IDC_SETMASK, OnSetmask)
        ON_BN_CLICKED(IDC_CLEARMASK, OnClearmask)
        ON_EN_CHANGE(IDC_XMASK, OnChangeXmask)
        ON_EN_CHANGE(IDC_YMASK, OnChangeYmask)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlgBlt message handlers

/****************************************************************************
*                            CPlgBlt::getParms
* Inputs:
*       plgparms * p: Pointer to parameter block
* Result: void
*       
* Effect: 
*       Fills in the parameter block
* Notes:
*       This stuff was so complex to write I only wanted to write it once
*       and share it with the formatting and action routines
****************************************************************************/

void CPlgBlt::getParms(plgparms * p)
    {
     p->plg[0].x = c_ulX.GetVal();
     p->plg[0].y = c_ulY.GetVal();
     p->plg[1].x = c_PlgX.GetVal();
     p->plg[1].y = c_PlgY.GetVal();
     p->plg[2].x = c_lrX.GetVal();
     p->plg[2].y = c_lrY.GetVal();

     p->src.x = c_SrcX.GetVal();
     p->src.y = c_SrcY.GetVal();

     p->size.cx = c_SrclrX.GetVal();
     p->size.cy = c_SrclrY.GetVal();

     p->mask.x = c_XMask.GetVal();
     p->mask.y = c_YMask.GetVal();
    }


/****************************************************************************
*                              CPlgBlt::enableOp
* Result: void
*       
* Effect: 
*       Enables the op button
****************************************************************************/

void CPlgBlt::enableOp()
    {
     c_DoBlt.EnableWindow(c_Rop.GetCurSel() != CB_ERR &&
                   (!hasSource() || c_BitmapSelect.GetCurSel() != CB_ERR));
     
    }

/****************************************************************************
*                             CPlgBlt::makeOp
* Result: void
*       
* Effect: 
*       Formats the BitBlt operation
*
* Notes:
*       BitBlt(hdest, ulx, uly, width, height, hsrc, xsrc, ysrc, rop)
****************************************************************************/

void CPlgBlt::makeOp()
    {
     if(!initialized)
        return;

     CString s;
     CString rop;
     CString hmask;

     int sel = c_Rop.GetCurSel();
     if(sel == CB_ERR)
        rop = _T("?");
     else
        { /* has rop */
         c_Rop.GetLBText(sel, rop);
         rop = getRopName(rop);
        } /* has rop */

     if((HBITMAP)mask == NULL)
        hmask = _T("NULL");
     else
        hmask = _T("hmask");

     CString fmt;
     fmt.LoadString(IDS_PLGBLT_FMT);
     // PlgBlt(hdest, [{%d,%d}, {%d,%d}, {%d, %d}], hsrc, xsrc: %d, ysrc: %d,
     //        width: %d, height: %d, mask: %s, xmask: %d, ymask: %d)

     plgparms p;

     getParms(&p);


     s.Format(fmt,
                p.plg[0].x, p.plg[0].y,
                p.plg[1].x, p.plg[1].y,
                p.plg[2].x, p.plg[2].y,
                p.src.x,    p.src.y,
                p.size.cx,  p.size.cy,
                hmask,
                p.mask.x,   p.mask.y,
                rop);

     c_Op.SetWindowText(s);

     enableOp();

    }

BOOL CPlgBlt::OnInitDialog() 
{
        opid = IDS_PLGBLT_OP;

        CGenBlt::OnInitDialog();
        
        c_Rop.Load();
        c_Rop.Select(STRETCH_ANDSCANS); // default for DC

        plgShow(TRUE); // we are a plgblt page

        CString s;
        s.LoadString(IDS_STRETCH_CAPTION);
        c_c_Rop.SetWindowText(s);

        c_Extended.ShowWindow(SW_HIDE);

        c_c_Brush.ShowWindow(SW_HIDE);
        c_BrDemo.ShowWindow(SW_HIDE);

        s.LoadString(IDS_PT0X);
        c_c_pt0x.SetWindowText(s);
        s.LoadString(IDS_PT0Y);
        c_c_pt0y.SetWindowText(s);
        s.LoadString(IDS_PT2X);
        c_c_pt2x.SetWindowText(s);
        s.LoadString(IDS_PT2Y);
        c_c_pt2y.SetWindowText(s);

        c_Mask.setBorder(0); // use full display size

        endInitDialog();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                              CPlgBlt::doBLT
* Result: void
*       
* Effect: 
*       Performs the PlgBlt desired.  This works by doing the Blt from
*       the offscreen bitmap in source to the offscreen bitmap in dest,
*       then forcing dest to redraw.
****************************************************************************/

void CPlgBlt::doBLT() 
{
 CDC source;
 CDC dest;

 source.CreateCompatibleDC(NULL);
 dest.CreateCompatibleDC(NULL);

 source.SelectObject(c_Source.getBitmap());
 dest.SelectObject(c_Dest.getBitmap());

 plgparms p;
 getParms(&p);

 int rop = c_Rop.GetCurItem();

 dest.SetStretchBltMode(rop);

 DWORD oldlimit = ::GdiSetBatchLimit(1);  // force accurate reporting

 if(!dest.PlgBlt(p.plg, &source, p.src.x, p.src.y, p.size.cx, p.size.cy, 
                mask, p.mask.x, p.mask.y))
     lasterror = ::GetLastError();
 else
     lasterror = 0;

 setLastError();
        
 ::GdiSetBatchLimit(oldlimit);

 c_Dest.InvalidateRect(NULL);
}

/****************************************************************************
*                             CPlgBlt::hasSource
* Result: BOOL
*       TRUE, always 
****************************************************************************/

BOOL CPlgBlt::hasSource()
    {
     return TRUE;
    }

/****************************************************************************
*                            CPlgBlt::isMonochrome
* Result: BOOL
*       TRUE if the currently selected bitmap image is monochrome
*       FALSE if it is not monochrome
****************************************************************************/

BOOL CPlgBlt::isMonochrome()
    {
     CBitmap bm;
     CClientDC dc(&c_Source);
     bm.LoadBitmap(c_BitmapSelect.GetCurItem());
     BOOL result = ::isMonochrome(&dc, bm);

     bm.DeleteObject();

     return result;
    }

/****************************************************************************
*                           CPlgBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CPlgBlt::updateControls()
    {
     // If there is no source object, hide the source selection controls

     if(!initialized)
         return;

     CGenBlt::updateControls();

     // Enable the set/clear mask options

     c_ClearMask.EnableWindow((HBITMAP)mask != NULL);

     c_SetMask.EnableWindow(isMonochrome());

     // Enable mask.x,y values
     BOOL enable = ((HBITMAP)mask != NULL);
     c_XMaskSpin.EnableWindow(enable);
     c_YMaskSpin.EnableWindow(enable);
     c_XMask.EnableWindow(enable);
     c_YMask.EnableWindow(enable);

    }


void CPlgBlt::OnSetmask() 
{
 if((HBITMAP)mask != NULL)
    mask.DeleteObject();

 mask.LoadBitmap(c_BitmapSelect.GetCurItem());
 c_Mask.setBitmap(c_BitmapSelect.GetCurItem());

 c_XMask.SetVal(0);
 c_YMask.SetVal(0);

 CClientDC dc(&c_Source);

 CSize size = getBitmapSize(&dc, mask);

 c_XMaskSpin.SetRange(0, size.cx);
 c_YMaskSpin.SetRange(0, size.cy);

 updateControls();
 makeOp();
}

void CPlgBlt::OnClearmask() 
{
 mask.DeleteObject();
 c_Mask.setBitmap(NULL);
 updateControls();
 makeOp();
        
}

void CPlgBlt::OnChangeXmask() 
{
 makeOp();

}

void CPlgBlt::OnChangeYmask() 
{
 makeOp();
        
}

/****************************************************************************
*                             CPlgBlt::getStrMode
* Result: int
*       The StretchBltMode value to use
****************************************************************************/

int CPlgBlt::getStrMode()
    {
     return c_Rop.GetCurItem();
    }

/****************************************************************************
*                             CPlgBlt::getStrModeStr
* Result: CString
*       The StretchBltMode print value to use
****************************************************************************/

CString CPlgBlt::getStrModeStr()
    {
     int index = c_Rop.GetCurSel();
     if(index == CB_ERR)
        return CString(_T("?")); // should never happen
     CString s;
     c_Rop.GetLBText(index, s);
     return s;
    }

void CPlgBlt::OnSelendokRop() 
{
        makeOp();
        updateControls();

}

/****************************************************************************
*                              CPlgBlt::getCopyOrg
* Result: CPoint
*       The point at which a blt copy is supposed to start
* Notes: 
*       This is the default operation.  More complex operations, such as
*       PlgBlt, may override this
****************************************************************************/

CPoint CPlgBlt::getCopyOrg()
    {
     CRect r;
     c_Dest.getObjectRect(&r);
     return CPoint(r.left, r.top);
    }

/****************************************************************************
*                              CPlgBlt::getCopyExt
* Result: CSize
*       
* Effect: 
*       Returns the size of the copy bitmap.  This adjusts for the various
*       destination points.
****************************************************************************/

CSize CPlgBlt::getCopyExt()
    {
     return CSize(c_lrX.GetVal() + max(c_ulX.GetVal(), 
                                       max(c_lrX.GetVal(), c_PlgX.GetVal())),
                  c_lrY.GetVal() + max(c_ulY.GetVal(),
                                       max(c_lrY.GetVal(), c_PlgY.GetVal())));
    }
