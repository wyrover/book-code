// MaskBlt.cpp : implementation file
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
#include "BitRop.h"
#include "bmw.h"
#include "brdemo.h"

#include "orgext.h"
#include "blt.h"
#include "genblt.h"
#include "MskBlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskBlt property page

IMPLEMENT_DYNCREATE(CMaskBlt, CGenBlt)

CMaskBlt::CMaskBlt() : CGenBlt(IDS_MASKBLT)
{
        //{{AFX_DATA_INIT(CMaskBlt)
        //}}AFX_DATA_INIT
}

CMaskBlt::~CMaskBlt()
{
}

void CMaskBlt::DoDataExchange(CDataExchange* pDX)
{
        CGenBlt::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMaskBlt)
        DDX_Control(pDX, IDC_ROP, c_Rop);
        DDX_Control(pDX, IDC_STRETCHBLT, c_Rop2);
        DDX_Control(pDX, IDC_STRETCH_CAPTION, c_c_Rop2);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaskBlt, CGenBlt)
        //{{AFX_MSG_MAP(CMaskBlt)
        ON_CBN_SELENDOK(IDC_ROP, OnSelendokRop)
        ON_EN_CHANGE(IDC_PLGX, OnChangePlgx)
        ON_EN_CHANGE(IDC_PLGY, OnChangePlgy)
        ON_CBN_SELENDOK(IDC_STRETCHBLT, OnSelendokRop2)
        ON_BN_CLICKED(IDC_SETMASK, OnSetmask)
        ON_BN_CLICKED(IDC_CLEARMASK, OnClearmask)
        ON_EN_CHANGE(IDC_XMASK, OnChangeXmask)
        ON_EN_CHANGE(IDC_YMASK, OnChangeYmask)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskBlt message handlers

/****************************************************************************
*                            CMaskBlt::getParms
* Inputs:
*       mskparms * p: Pointer to parameter block
* Result: void
*       
* Effect: 
*       Fills in the parameter block
* Notes:
*       This stuff was so complex to write I only wanted to write it once
*       and share it with the formatting and action routines
****************************************************************************/

void CMaskBlt::getParms(mskparms * p)
    {
     p->dst.org.x = c_ulX.GetVal();
     p->dst.org.y = c_ulY.GetVal();
     p->dst.ext.cx = c_lrX.GetVal();
     p->dst.ext.cy = c_lrY.GetVal();

     p->mask.x = c_XMask.GetVal();
     p->mask.y = c_YMask.GetVal();

     p->src.x = c_SrcX.GetVal();
     p->src.y = c_SrcY.GetVal();

    }

/****************************************************************************
*                             CMaskBlt::enableOp
* Result: void
*       
* Effect: 
*       Enables the op button
****************************************************************************/

void CMaskBlt::enableOp()
    {
     c_DoBlt.EnableWindow(c_Rop.GetCurSel() != CB_ERR &&
                   (!hasSource() || c_BitmapSelect.GetCurSel() != CB_ERR) &&
                   (!hasPat() || brush.m_hObject != NULL));
     
    }

/****************************************************************************
*                             CMaskBlt::makeOp
* Result: void
*       
* Effect: 
*       Formats the BitBlt operation
*
* Notes:
*       BitBlt(hdest, ulx, uly, width, height, hsrc, xsrc, ysrc, rop)
****************************************************************************/

void CMaskBlt::makeOp()
    {
     if(!initialized)
        return;

     CString s;
     CString rop1;
     CString rop2;
     CString hmask;
     int rop1v;
     int rop2v;

     int sel = c_Rop.GetCurSel();
     if(sel == CB_ERR)
        rop1 = _T("?");
     else
        { /* has rop1 */
         c_Rop.GetLBText(sel, rop1);
         rop1 = getRopName(rop1);
         rop1v = c_Rop.GetCurItem();
        } /* has rop1 */

     sel = c_Rop2.GetCurSel();
     if(sel == CB_ERR)
        rop2 = _T("?");
     else
        { /* has rop2 */
         c_Rop2.GetLBText(sel, rop2);
         rop2 = getRopName(rop2);
         rop2v = c_Rop2.GetCurItem();
        } /* has rop2 */

     if(mask.m_hObject == NULL)
        hmask = _T(": NULL");

     CString fmt;
     fmt.LoadString(IDS_MASKBLT_FMT);
     // MaskBlt(hdest, xdst: %d, ydst: %d, dstwid: %d, dstht: %d
     //            hsrc,  xsrc: %d, ysrc: %d, hmask: %s, maskx: %d, masky: %d, 
     //            MAKEROP4(%s, %s))

     mskparms p;

     getParms(&p);


     s.Format(fmt,
                p.dst.org.x, p.dst.org.y,
                p.dst.ext.cx, p.dst.ext.cy,
                p.src.x, p.src.y,
                hmask,
                p.mask.x, p.mask.y,
                rop1, rop2, MAKEROP4(rop1v, rop2v));

     c_Op.SetWindowText(s);

     enableOp();

    }

BOOL CMaskBlt::OnInitDialog() 
{
        opid = IDS_MASKBLT_OP;

        CGenBlt::OnInitDialog();
        
        c_Rop.Load(FALSE);
        c_Rop.Select(SRCCOPY);

        c_Rop2.Load(FALSE);
        c_Rop2.Select(SRCCOPY);

        c_Rop2.ShowWindow(SW_SHOW);

        // strShow(TRUE); // we are a MaskBlt page

        createBrush(&brush);

        c_BrDemo.brush = &brush;

        // Disable the source width and height controls

        c_SrclrX.SetReadOnly(TRUE);
        c_SrclrY.SetReadOnly(TRUE);
        c_SrclrXSpin.EnableWindow(FALSE);
        c_SrclrYSpin.EnableWindow(FALSE);
        c_SrclrXSpin.ShowWindow(SW_HIDE);
        c_SrclrYSpin.ShowWindow(SW_HIDE);

        // Enable the MaskBltMode selector

        CString s;
        s.LoadString(IDS_ROP2);
        c_c_Rop2.SetWindowText(s);
        c_c_Rop2.ShowWindow(SW_SHOW);

        c_lrX.SetVal(c_SrclrX.GetVal());
        c_lrXSpin.SetPos(c_lrX.GetVal());

        s.LoadString(IDS_WIDTH);
        c_c_pt2x.SetWindowText(s);
        c_c_pt2x.ShowWindow(SW_SHOW);

        s.LoadString(IDS_HEIGHT);
        c_c_pt2y.SetWindowText(s);
        c_c_pt2y.ShowWindow(SW_SHOW);

        createBrush(&brush);

        c_BrDemo.brush = &brush;

        maskShow(TRUE);

        c_Mask.setBorder(0); // use full display size

        s.LoadString(IDS_ROP_FG);
        c_Rop_Caption.SetWindowText(s);

        s.LoadString(IDS_ROP_BG);
        c_c_Rop2.SetWindowText(s);

        endInitDialog();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                              CMaskBlt::doBLT
* Result: void
*       
* Effect: 
*       Performs the MaskBlt desired.  This works by doing the Blt from
*       the offscreen bitmap in source to the offscreen bitmap in dest,
*       then forcing dest to redraw.
****************************************************************************/

void CMaskBlt::doBLT() 
{
 CDC source;
 CDC dest;

 source.CreateCompatibleDC(NULL);
 dest.CreateCompatibleDC(NULL);

 source.SelectObject(c_Source.getBitmap());
 dest.SelectObject(c_Dest.getBitmap());

 mskparms p;
 getParms(&p);

 int rop1 = c_Rop.GetCurItem();
 int rop2 = c_Rop2.GetCurItem();

 dest.SelectObject(&brush);

 DWORD oldlimit = ::GdiSetBatchLimit(1);  // force accurate reporting

 if(!dest.MaskBlt(p.dst.org.x, p.dst.org.y, p.dst.ext.cx, p.dst.ext.cy,
            &source, p.src.x, p.src.y, mask, p.mask.x, p.mask.y, 
                MAKEROP4(rop1, rop2)))
     lasterror = ::GetLastError();
 else
     lasterror = 0;

 setLastError();
        
 ::GdiSetBatchLimit(oldlimit);

 c_Dest.InvalidateRect(NULL);
}

/****************************************************************************
*                             CMaskBlt::hasSource
* Result: BOOL
*       TRUE, always 
****************************************************************************/

BOOL CMaskBlt::hasSource()
    {
     return CBlt::isSourceRop(c_Rop.GetCurItem()) ||
            CBlt::isSourceRop(c_Rop2.GetCurItem());
    }

/****************************************************************************
*                             CMaskBlt::hasPat
* Result: BOOL
*       TRUE, always 
****************************************************************************/

BOOL CMaskBlt::hasPat()
    {
     return CBlt::isPatRop(c_Rop.GetCurItem()) ||
            CBlt::isPatRop(c_Rop2.GetCurItem());
    }

/****************************************************************************
*                           CMaskBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CMaskBlt::updateControls()
    {
     if(!initialized)
         return;

     CGenBlt::updateControls();

     c_ClearMask.EnableWindow((HBITMAP)mask != NULL);

     c_SetMask.EnableWindow(isMonochrome());

     // Enable mask.x,y values
     BOOL enable = ((HBITMAP)mask != NULL);
     c_XMaskSpin.EnableWindow(enable);
     c_YMaskSpin.EnableWindow(enable);
     c_XMask.EnableWindow(enable);
     c_YMask.EnableWindow(enable);

     enableOp();

    }


BOOL CMaskBlt::OnSetActive() 
{
 createBrush(&brush);
        
 updateControls();

 return CGenBlt::OnSetActive();
}

/****************************************************************************
*                              CMaskBlt::loadRop
* Inputs:
*       BOOL ext: TRUE to load extended ops, FALSE for normal ops
* Result: void
*       
* Effect: 
*       Loads the normal or extended ternary ROP opcodes into the box
****************************************************************************/

void CMaskBlt::loadRop(BOOL ext)
    {
     CWaitCursor w; // 

     DWORD sel = c_Rop.GetCurItem();
     c_Rop.Load(ext);
     if(c_Rop.Select(sel) == CB_ERR)  // select last if p0ssible
        c_Rop.Select(SRCCOPY);          // default to SRCCOPY

     sel = c_Rop2.GetCurItem();
     c_Rop2.Load(ext);
     if(c_Rop2.Select(sel) == CB_ERR)
        c_Rop2.Select(SRCCOPY);

    }

void CMaskBlt::OnSelendokRop() 
{
        makeOp();
        updateControls();
}

void CMaskBlt::OnSelendokRop2() 
{
        makeOp();
        updateControls();
}

void CMaskBlt::OnSetmask() 
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

void CMaskBlt::OnClearmask() 
{
 mask.DeleteObject();
 c_Mask.setBitmap(NULL);
 updateControls();
 makeOp();
        
}

void CMaskBlt::OnChangeXmask() 
{
 makeOp();

}

void CMaskBlt::OnChangeYmask() 
{
 makeOp();
        
}

/****************************************************************************
*                            CMaskBlt::isMonochrome
* Result: BOOL
*       TRUE if the currently selected bitmap image is monochrome
*       FALSE if it is not monochrome
****************************************************************************/

BOOL CMaskBlt::isMonochrome()
    {
     CBitmap bm;
     CClientDC dc(&c_Source);
     bm.LoadBitmap(c_BitmapSelect.GetCurItem());
     BOOL result = ::isMonochrome(&dc, bm);

     bm.DeleteObject();

     return result;
    }

/****************************************************************************
*                            CMaskBlt::getStrMode
* Result: int
*       0, always (not a legal StretchBltMode value)
****************************************************************************/

int CMaskBlt::getStrMode()
    {
     return 0;
    }

/****************************************************************************
*                            CMaskBlt::getStrModeStr
* Result: CString
*       "", always (not a legal StretchBltMode value)
****************************************************************************/

CString CMaskBlt::getStrModeStr()
    {
     return CString(_T(""));
    }
