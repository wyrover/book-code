// StretchBlt.cpp : implementation file
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
#include "strmode.h"
#include "bmw.h"
#include "brdemo.h"

#include "extrops.h"

#include "orgext.h"
#include "blt.h"
#include "genblt.h"
#include "StrBlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStretchBlt property page

IMPLEMENT_DYNCREATE(CStretchBlt, CGenBlt)

CStretchBlt::CStretchBlt() : CGenBlt(IDS_STRETCHBLT)
{
        //{{AFX_DATA_INIT(CStretchBlt)
        //}}AFX_DATA_INIT
}

CStretchBlt::~CStretchBlt()
{
}

void CStretchBlt::DoDataExchange(CDataExchange* pDX)
{
        CGenBlt::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CStretchBlt)
        DDX_Control(pDX, IDC_ROP, c_Rop);
        DDX_Control(pDX, IDC_STRETCH_CAPTION, c_c_StretchBltMode);
        DDX_Control(pDX, IDC_STRETCHBLT, c_StretchBlt);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStretchBlt, CGenBlt)
        //{{AFX_MSG_MAP(CStretchBlt)
        ON_CBN_SELENDOK(IDC_ROP, OnSelendokRop)
        ON_EN_CHANGE(IDC_PLGX, OnChangePlgx)
        ON_EN_CHANGE(IDC_PLGY, OnChangePlgy)
        ON_CBN_SELENDOK(IDC_STRETCHBLT, OnSelendokStretchblt)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStretchBlt message handlers

/****************************************************************************
*                            CStretchBlt::getParms
* Inputs:
*       strparms * p: Pointer to parameter block
* Result: void
*       
* Effect: 
*       Fills in the parameter block
* Notes:
*       This stuff was so complex to write I only wanted to write it once
*       and share it with the formatting and action routines
****************************************************************************/

void CStretchBlt::getParms(strparms * p)
    {
     p->dst.org.x = c_ulX.GetVal();
     p->dst.org.y = c_ulY.GetVal();

     p->dst.ext.cx = c_lrX.GetVal();
     p->dst.ext.cy = c_lrY.GetVal();

     p->src.org.x = c_SrcX.GetVal();
     p->src.org.y = c_SrcY.GetVal();

     p->src.ext.cx = c_SrclrX.GetVal();
     p->src.ext.cy = c_SrclrY.GetVal();
    }

/****************************************************************************
*                                  enableOp
* Result: void
*       
* Effect: 
*       Enables the op button
****************************************************************************/

void CStretchBlt::enableOp()
    {
     c_DoBlt.EnableWindow(c_Rop.GetCurSel() != CB_ERR &&
                   (!hasSource() || c_BitmapSelect.GetCurSel() != CB_ERR) &&
                   (!hasPat() || brush.m_hObject != NULL));     
    }

/****************************************************************************
*                             CStretchBlt::makeOp
* Result: void
*       
* Effect: 
*       Formats the BitBlt operation
*
* Notes:
*       BitBlt(hdest, ulx, uly, width, height, hsrc, xsrc, ysrc, rop)
****************************************************************************/

void CStretchBlt::makeOp()
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

     CString fmt;
     fmt.LoadString(IDS_STRETCHBLT_FMT);
     // StretchBlt(hdest, xdst: %d, ydst: %d, dstwid: %d, dstht: %d
     //            hsrc,  xsrc: %d, ysrc: %d, srcwid: %d, srcht: %d, %s)

     strparms p;

     getParms(&p);


     s.Format(fmt,
                p.dst.org.x, p.dst.org.y,
                p.dst.ext.cx, p.dst.ext.cy,
                p.src.org.x, p.src.org.y,
                p.src.ext.cx, p.src.ext.cy,
                rop);

     c_Op.SetWindowText(s);

     enableOp();
    }

BOOL CStretchBlt::OnInitDialog() 
{
        opid = IDS_STRETCHBLT_OP;

        CGenBlt::OnInitDialog();
        
        c_Rop.Load(FALSE);
        c_Rop.Select(SRCCOPY);

        // strShow(TRUE); // we are a StretchBlt page


        createBrush(&brush);

        c_BrDemo.brush = &brush;

        // Enable the StretchBltMode selector

        c_c_StretchBltMode.ShowWindow(SW_SHOW);
        c_StretchBlt.ShowWindow(SW_SHOW);
        c_StretchBlt.Load();
        c_StretchBlt.Select(STRETCH_ANDSCANS);

        int lb;
        int ub;
        c_lrXSpin.GetRange(lb, ub);
        c_lrXSpin.SetRange(-ub, ub);  // allow for mirror-imaging
        c_lrXSpin.SetPos(ub);         // initialize to max width

        c_lrYSpin.GetRange(lb, ub);
        c_lrYSpin.SetRange(-ub, ub);

        c_SrclrXSpin.ShowWindow(SW_SHOW);
        c_SrclrXSpin.GetRange(lb, ub);
        c_SrclrXSpin.SetRange(-ub, ub);

        c_SrclrYSpin.ShowWindow(SW_SHOW);
        c_SrclrYSpin.GetRange(lb, ub);
        c_SrclrYSpin.SetRange(-ub, ub);


        CString s;
        s.LoadString(IDS_WIDTH);
        c_c_pt2x.SetWindowText(s);
        c_c_pt2x.ShowWindow(SW_SHOW);

        s.LoadString(IDS_HEIGHT);
        c_c_pt2y.SetWindowText(s);
        c_c_pt2y.ShowWindow(SW_SHOW);

        c_SrclrX.EnableWindow(TRUE);
        c_SrclrY.EnableWindow(TRUE);

        createBrush(&brush);

        c_BrDemo.brush = &brush;

        endInitDialog();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************************************
*                              CStretchBlt::doBLT
* Result: void
*       
* Effect: 
*       Performs the StretchBlt desired.  This works by doing the Blt from
*       the offscreen bitmap in source to the offscreen bitmap in dest,
*       then forcing dest to redraw.
****************************************************************************/

void CStretchBlt::doBLT() 
{
 CDC source;
 CDC dest;

 source.CreateCompatibleDC(NULL);
 dest.CreateCompatibleDC(NULL);

 source.SelectObject(c_Source.getBitmap());
 dest.SelectObject(c_Dest.getBitmap());

 strparms p;
 getParms(&p);

 int rop = c_Rop.GetCurItem();
 int mode = c_StretchBlt.GetCurItem();

 dest.SetStretchBltMode(mode);

 dest.SelectObject(&brush);

 DWORD oldlimit = ::GdiSetBatchLimit(1);  // force accurate reporting

 if(!dest.StretchBlt(p.dst.org.x, p.dst.org.y, p.dst.ext.cx, p.dst.ext.cy,
            &source, p.src.org.x, p.src.org.y, p.src.ext.cx, p.src.ext.cy, 
                                                                       rop))
     lasterror = ::GetLastError();
 else
     lasterror = 0;

 setLastError();
        
 ::GdiSetBatchLimit(oldlimit);
        
 c_Dest.InvalidateRect(NULL);
}

/****************************************************************************
*                             CStretchBlt::hasSource
* Result: BOOL
*       TRUE, always 
****************************************************************************/

BOOL CStretchBlt::hasSource()
    {
     return isSourceRop(c_Rop.GetCurItem());
    }

/****************************************************************************
*                             CStretchBlt::hasPat
* Result: BOOL
*       TRUE, always 
****************************************************************************/

BOOL CStretchBlt::hasPat()
    {
     return isPatRop(c_Rop.GetCurItem());
    }

/****************************************************************************
*                           CStretchBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CStretchBlt::updateControls()
    {
     if(!initialized)
         return;

     CGenBlt::updateControls();
    }


/****************************************************************************
*                           CStretchBlt::getStrMode
* Result: int
*       Current SetStretchBltMode setting
****************************************************************************/

int CStretchBlt::getStrMode()
    {
     return c_StretchBlt.GetCurItem();
    }

/****************************************************************************
*                             CPlgBlt::getStrModeStr
* Result: CString
*       The StretchBltMode print value to use
****************************************************************************/

CString CStretchBlt::getStrModeStr()
    {
     int index = c_StretchBlt.GetCurSel();
     if(index == CB_ERR)
        return CString(_T("?")); // should never happen
     CString s;
     c_StretchBlt.GetLBText(index, s);
     return s;
    }

BOOL CStretchBlt::OnSetActive() 
{

 createBrush(&brush);
        
 updateControls();

 return CGenBlt::OnSetActive();
}

/****************************************************************************
*                              CStretchBlt::loadRop
* Inputs:
*       BOOL ext: TRUE to load extended ops, FALSE for normal ops
* Result: void
*       
* Effect: 
*       Loads the normal or extended ternary ROP opcodes into the box
****************************************************************************/

void CStretchBlt::loadRop(BOOL ext)
    {
     CWaitCursor w; // 

     DWORD sel = c_Rop.GetCurItem();
     c_Rop.Load(ext);
     if(c_Rop.Select(sel) == CB_ERR)  // select last if p0ssible
        c_Rop.Select(SRCCOPY);          // default to SRCCOPY
    }

void CStretchBlt::OnSelendokRop() 
{
        makeOp();
        updateControls();
}

void CStretchBlt::OnSelendokStretchblt() 
{
 makeOp();
 updateControls();
        
}
