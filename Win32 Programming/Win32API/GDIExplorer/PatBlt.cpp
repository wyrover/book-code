// PatBlt.cpp : implementation file
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
#include "patrop.h"
#include "brdemo.h"
#include "strmode.h"

#include "blt.h"
#include "PatBlt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatBlt property page

IMPLEMENT_DYNCREATE(CPatBlt, CBlt)

CPatBlt::CPatBlt() : CBlt(IDS_PATBLT)
{
        //{{AFX_DATA_INIT(CPatBlt)
        //}}AFX_DATA_INIT
}

CPatBlt::~CPatBlt()
{
}

void CPatBlt::DoDataExchange(CDataExchange* pDX)
{
        CBlt::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CPatBlt)
        DDX_Control(pDX, IDC_ROP, c_Rop);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatBlt, CBlt)
        //{{AFX_MSG_MAP(CPatBlt)
        ON_CBN_SELENDOK(IDC_ROP, OnSelendokRop)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatBlt message handlers


/****************************************************************************
*                              CPatBlt::enableOp
* Result: void
*       
* Effect: 
*       Enables the operation button
****************************************************************************/

void CPatBlt::enableOp()
    {
     c_DoBlt.EnableWindow(c_Rop.GetCurSel() != CB_ERR &&
                          c_BitmapSelect.GetCurSel() != CB_ERR &&
                          brush.m_hObject != NULL);
     
    }

/****************************************************************************
*                             CPatBlt::makeOp
* Result: void
*       
* Effect: 
*       Formats the PatBlt operation
*
* Notes:
*       PatBlt(hdest, ulx, uly, width, height, rop)
****************************************************************************/

void CPatBlt::makeOp()
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

     CString fmt;
     fmt.LoadString(IDS_PATBLT_FMT);
     s.Format(fmt,
                c_ulX.GetVal(),
                c_ulY.GetVal(),
                c_lrX.GetVal(),
                c_lrY.GetVal(),
                rop);

     c_Op.SetWindowText(s);

     enableOp();
    }

BOOL CPatBlt::OnInitDialog() 
{
        opid = IDS_PATBLT_OP;

        CBlt::OnInitDialog();
        
        c_Rop.Load(FALSE);
        c_Rop.Select(PATCOPY);

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

void CPatBlt::OnSelendokRop() 
{
        makeOp();
        updateControls();
}

/****************************************************************************
*                              CPatBlt::doBLT
* Result: void
*       
* Effect: 
*       Performs the PatBlt desired.  This works by doing the PatBlt from
*       the offscreen bitmap in source to the offscreen bitmap in dest,
*       then forcing dest to redraw.
****************************************************************************/

void CPatBlt::doBLT() 
{
 CDC dest;

 dest.CreateCompatibleDC(NULL);

 dest.SelectObject(c_Dest.getBitmap());

 int ulX = c_ulX.GetVal();
 int ulY = c_ulY.GetVal();
 int width = c_lrX.GetVal();
 int height = c_lrY.GetVal();

 int rop = c_Rop.GetCurItem();

 dest.SelectObject(&brush);

 DWORD oldlimit = ::GdiSetBatchLimit(1);  // force accurate reporting

 if(!dest.PatBlt(ulX, ulY, width, height, rop))
     lasterror = ::GetLastError();
 else
     lasterror = 0;

 setLastError();
        
 ::GdiSetBatchLimit(oldlimit);

 c_Dest.InvalidateRect(NULL);
}

/****************************************************************************
*                             CPatBlt::hasSource
* Result: BOOL
*       FALSE, always (PatBlt takes no source operands)
****************************************************************************/

BOOL CPatBlt::hasSource()
    {
     return FALSE;
    }

/****************************************************************************
*                           CPatBlt::updateControls
* Result: void
*       
* Effect: 
*       Adjusts values of the controls.  This includes:
*               - Setting the limits for the source bitmap controls
****************************************************************************/

void CPatBlt::updateControls()
    {
     CBlt::updateControls();
    }


/****************************************************************************
*                              CPatBlt::loadRop
* Inputs:
*       BOOL ext: TRUE to load extended ops, FALSE for normal ops
* Result: void
*       
* Effect: 
*       Loads the normal or extended ternary ROP opcodes into the box
****************************************************************************/

void CPatBlt::loadRop(BOOL ext)
    {
     DWORD sel = c_Rop.GetCurItem();
     c_Rop.Load(ext);
     if(c_Rop.Select(sel) == CB_ERR)  // select last if p0ssible
        c_Rop.Select(PATCOPY);          // default to PATCOPY
    }


BOOL CPatBlt::OnSetActive() 
{
 createBrush(&brush);
        
 updateControls();

 return CBlt::OnSetActive();
}
