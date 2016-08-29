// BitBlt.cpp : implementation file
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
#include "brdemo.h"
#include "strmode.h"

#include "blt.h"
#include "BitBlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitBlt property page

IMPLEMENT_DYNCREATE(CBitBlt, CBlt)

CBitBlt::CBitBlt() : CBlt(IDS_BITBLT)
{
        //{{AFX_DATA_INIT(CBitBlt)
        //}}AFX_DATA_INIT
}

CBitBlt::~CBitBlt()
{
}

void CBitBlt::DoDataExchange(CDataExchange* pDX)
{
        CBlt::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CBitBlt)
        DDX_Control(pDX, IDC_ROP, c_Rop);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBitBlt, CBlt)
        //{{AFX_MSG_MAP(CBitBlt)
        ON_CBN_SELENDOK(IDC_ROP, OnSelendokRop)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitBlt message handlers


/****************************************************************************
*                              CBitBlt::enableOp
* Result: void
*       
* Effect: 
*       Enables the op button
****************************************************************************/

void CBitBlt::enableOp()
    {
     c_DoBlt.EnableWindow(c_Rop.GetCurSel() != CB_ERR &&
                   (!hasSource() || c_BitmapSelect.GetCurSel() != CB_ERR) &&
                   (!hasPat() || brush.m_hObject != NULL));
     
    }

/****************************************************************************
*                             CBitBlt::makeOp
* Result: void
*       
* Effect: 
*       Formats the BitBlt operation
*
* Notes:
*       BitBlt(hdest, ulx, uly, width, height, hsrc, xsrc, ysrc, rop)
****************************************************************************/

void CBitBlt::makeOp()
    {
     if(!initialized)
        return;

     CString s;
     CString rop;
     CString hsrc;

     int sel = c_Rop.GetCurSel();
     if(sel == CB_ERR)
        rop = _T("?");
     else
        { /* has rop */
         c_Rop.GetLBText(sel, rop);
         rop = getRopName(rop);
        } /* has rop */

     hsrc = (hasSource() ? _T("hsrc") : _T("NULL"));

     CString fmt;
     fmt.LoadString(IDS_BITBLT_FMT);
     s.Format(fmt,
                c_ulX.GetVal(),
                c_ulY.GetVal(),
                c_lrX.GetVal(),
                c_lrY.GetVal(),
                hsrc,
                (hasSource() ? c_SrcX.GetVal() : 0),
                (hasSource() ? c_SrcY.GetVal() : 0),
                rop);

     c_Op.SetWindowText(s);

     enableOp();
    }

BOOL CBitBlt::OnInitDialog() 
{
        opid = IDS_BITBLT_OP;

        CBlt::OnInitDialog();
        
        c_Rop.Load(FALSE);
        c_Rop.Select(SRCCOPY);

        createBrush(&brush);

        c_BrDemo.brush = &brush;

        CString s;

        s.LoadString(IDS_WIDTH);
        c_c_pt2x.SetWindowText(s);
        c_c_pt2x.ShowWindow(SW_SHOW);

        s.LoadString(IDS_HEIGHT);
        c_c_pt2y.SetWindowText(s);
        c_c_pt2y.ShowWindow(SW_SHOW);

        endInitDialog();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CBitBlt::OnSelendokRop() 
{
        makeOp();
        updateControls();
}

/****************************************************************************
*                              CBitBlt::doBLT
* Result: void
*       
* Effect: 
*       Performs the BitBlt desired.  This works by doing the BitBlt from
*       the offscreen bitmap in source to the offscreen bitmap in dest,
*       then forcing dest to redraw.
****************************************************************************/

void CBitBlt::doBLT() 
{
 CDC source;
 CDC dest;

 source.CreateCompatibleDC(NULL);
 dest.CreateCompatibleDC(NULL);

 source.SelectObject(c_Source.getBitmap());
 dest.SelectObject(c_Dest.getBitmap());

 int ulX = c_ulX.GetVal();
 int ulY = c_ulY.GetVal();
 int width = c_lrX.GetVal();
 int height = c_lrY.GetVal();
 int srcX = c_SrcX.GetVal();
 int srcY = c_SrcY.GetVal();

 int rop = c_Rop.GetCurItem();

 CDC * hsrc;

 hsrc = (hasSource() ? &source : NULL);

 dest.SelectObject(&brush);

 DWORD oldlimit = ::GdiSetBatchLimit(1);  // force accurate reporting

 if(!dest.BitBlt(ulX, ulY, width, height, hsrc, srcX, srcY, rop))
    { /* has error */
     lasterror = ::GetLastError();
     c_Error.SetWindowText(_T("?")); // TODO: use FormatMessage class here
    } /* has error */
 else
    { /* success */
     lasterror = 0;
     c_Error.SetWindowText(_T(""));
    } /* success */

 ::GdiSetBatchLimit(oldlimit);
        
 c_Dest.InvalidateRect(NULL);
}

/****************************************************************************
*                             CBitBlt::hasSource
* Result: BOOL
*       TRUE if the rop operator uses a source operand
*       FALSE if it does not use a source operand
****************************************************************************/

BOOL CBitBlt::hasSource()
    {
     return CBlt::isSourceRop(c_Rop.GetCurItem());
    }

/****************************************************************************
*                               CBitBlt::hasPat
* Result: BOOL
*       TRUE if the current rop requires a pattern
*       FALSE if it does not require a pattern
****************************************************************************/

BOOL CBitBlt::hasPat()
    {
     return CBlt::isPatRop(c_Rop.GetCurItem());
    }

/****************************************************************************
*                           CBitBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CBitBlt::updateControls()
    {
     CBlt::updateControls();
    }


/****************************************************************************
*                              CBitBlt::loadRop
* Inputs:
*       BOOL ext: TRUE to load extended ops, FALSE for normal ops
* Result: void
*       
* Effect: 
*       Loads the normal or extended ternary ROP opcodes into the box
****************************************************************************/

void CBitBlt::loadRop(BOOL ext)
    {
     CWaitCursor w; // 

     DWORD sel = c_Rop.GetCurItem();
     c_Rop.Load(ext);
     if(c_Rop.Select(sel) == CB_ERR)  // select last if p0ssible
        c_Rop.Select(SRCCOPY);          // default to SRCCOPY
    }

BOOL CBitBlt::OnSetActive() 
{

 createBrush(&brush);
        
 updateControls();

 return CBlt::OnSetActive();
}
