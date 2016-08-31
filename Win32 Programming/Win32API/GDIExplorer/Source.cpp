// Source.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "Source.h"
#include "bmw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSource

CSource::CSource()
{
 factor = -1;  // set border compensation size to use default
}

CSource::~CSource()
{
}


BEGIN_MESSAGE_MAP(CSource, CStatic)
        //{{AFX_MSG_MAP(CSource)
        ON_WM_DESTROY()
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSource message handlers

/****************************************************************************
*                           CSource::getObjectRect
* Inputs:
*       CRect * r: Rectangle value to be filled in
* Result: void
*       
* Effect: 
*       Sets the rectangle to be the "object" rectangle contained in the
*       source space.
* Notes:
*       The rectangle returned is in client coordinates of the CSource
*       object.  Use GetObjectWindowRect to get the coordinates in 
*       screen units
*
*           .left
*           |
*       +-------------------------+
*.top __|                         |
*       |   XXXXXXXXXXXXXXXXXXX   |
*       |   XXXXXXXXXXXXXXXXXXX   |
*       |   XXXXXXXXXXXXXXXXXXX   |___.bottom
*       |                         |
*       +-------------------------+
*                              |
*                              .right
****************************************************************************/

void CSource::getObjectRect(CRect * r)
    {
     CRect cr;

     GetClientRect(&cr);  // get parent rectangle
     
#define FACTOR 6
     int lb;
     int tb; 

     if(factor == -1)
        { /* default factor */
         lb = cr.Width() / FACTOR;  // left border
         tb = cr.Height() / FACTOR; // top border
        } /* default factor */
     else
     if(factor == 0)
        { /* no border */
         lb = tb = 0;
        } /* no border */
     else
        { /* use factor */
         lb = cr.Width() / factor;  // left border
         tb = cr.Height() / factor; // top border
        } /* use factor */
     r->left = cr.left + lb;
     r->top  = cr.top  + tb;
     r->right = cr.right - lb;
     r->bottom = cr.bottom - tb;
    }

/****************************************************************************
*                        CSource::getObjectWindowRect
* Inputs:
*       CRect * r: Rectangle value to be filled in
* Result: void
*       
* Effect: 
*       Sets the rectangle to the screen coordinates of the "object"
*       rectangle contained in the source space.
****************************************************************************/

void CSource::getObjectWindowRect(CRect * r)
    {
     getObjectRect(r);  // get object rect in client coordinates
     ClientToScreen(r); // convert to screen coordinates
    }

/****************************************************************************
*                             CSource::setBitmap
* Inputs:
*       int bmid: Bitmap id, or 0 to clear bitmap
* Result: BOOL
*       TRUE if bitmap load worked
*       FALSE if bitmap load failed
* Effect: 
*       Loads the bitmap.  If a previous bitmap was loaded, it is deleted
****************************************************************************/

BOOL CSource::setBitmap(int bmid)
    {
     CBitmap bm;

     if(bmid == 0)
        { /* clear */
         setBitmap((CBitmap *)NULL);
         return TRUE; // successfully cleared bitmap
        } /* clear */

     if(!bm.LoadBitmap(bmid))
        return FALSE;

     if(!setBitmap(&bm))
        return FALSE;

     return TRUE;
    }

/****************************************************************************
*                             CSource::setBitmap
* Inputs:
*       CBitmap * bm: Bitmap to set, or NULL to remove the bitmap
* Result: void
*       
* Effect: 
*       Sets the bitmap into the object.  The bitmap will be detached from
*       its input object
****************************************************************************/

BOOL CSource::setBitmap(CBitmap * bm)
    {
     if((HBITMAP)bitmap != NULL)
        { /* delete old bitmap */
         bitmap.DeleteObject();
        } /* delete old bitmap */

     if(bm != NULL)
        bitmap.Attach(bm->Detach());
     else
        bitmap.Detach();

     InvalidateRect(NULL);  // force new bitmap to draw
     return TRUE;
    }

/****************************************************************************
*                             CSource::createBitmap
* Result: void
*       
* Effect: 
*       Creates a bitmap which is the off-screen (and definitive) 
*       representation of what is in the image area of the object
****************************************************************************/

void CSource::createBitmap()
    {
     CRect r;
     getObjectRect(&r);   // gets the rectangle into which the drawing is made
     
     CClientDC dc(this);
     bitmap.CreateCompatibleBitmap(&dc, r.Width(), r.Height());

     clearBitmap();
     InvalidateRect(NULL);
    }

/****************************************************************************
*                             CSource::OnDestroy
* Result: void
*       
* Effect: 
*       Deletes any bitmap object that was created.
****************************************************************************/

void CSource::OnDestroy() 
{
        CStatic::OnDestroy();
        
        bitmap.DeleteObject();
        
}

/****************************************************************************
*                               CSource::OnPaint
* Result: void
*       
* Effect: 
*       Does a BitBlt of the bitmap which represents the image area
*       onto the screen.  All "real" work on the bitmap is done to the
*       memory bitmap which is stored with the object
****************************************************************************/

void CSource::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        CRect r;
        CRect client;

        GetClientRect(&client);

        CBrush fillbr;

        fillbr.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

        dc.FillRect(&client, &fillbr);

        getObjectRect(&r);
        
        if((HBITMAP)bitmap != NULL)
           { /* paint bitmap */
            CDC bm;
            bm.CreateCompatibleDC(NULL);

            int save = bm.SaveDC();
            
            bm.SelectObject(bitmap);

            dc.BitBlt(r.left, r.top, r.Width(), r.Height(), &bm, 0, 0, SRCCOPY);
            bm.RestoreDC(save);

            r.InflateRect(2,2);
            dc.SelectStockObject(NULL_BRUSH);

            dc.Rectangle(&r);

           } /* paint bitmap */
        else
           { /* no bitmap */
            int save = dc.SaveDC();
            dc.SelectStockObject(NULL_BRUSH);
            dc.Rectangle(&r);
            dc.MoveTo(r.left, r.top);
            dc.LineTo(r.right, r.bottom);
            dc.MoveTo(r.right, r.top);
            dc.LineTo(r.left, r.bottom);
            dc.RestoreDC(save);

            save = dc.SaveDC();
            r.InflateRect(2,2);
            dc.SelectStockObject(NULL_BRUSH);
            dc.Rectangle(&r);

            dc.RestoreDC(save);

           } /* no bitmap */
            

        // Do not call CSource::OnPaint() for painting messages
}

/****************************************************************************
*                            CSource::clearBitmap
* Inputs:
*       int mode: 0 for black, 1 for white
* Result: void
*       
* Effect: 
*       Clears the bitmap
****************************************************************************/

void CSource::clearBitmap(int mode)
    {
     CDC dc;

     dc.CreateCompatibleDC(NULL);

     dc.SelectObject(bitmap);

     CBrush br;
     if(mode == 1)
        br.CreateStockObject(WHITE_BRUSH);
     else
        br.CreateStockObject(BLACK_BRUSH);

     CSize sz = getBitmapSize(&dc, bitmap);

     CRect r(0, 0, sz.cx, sz.cy);
     dc.FillRect(&r, &br);

     // Having changed the contents of the bitmap, force it to redraw
     InvalidateRect(NULL);
    }

/****************************************************************************
*                             CSource::getBitmap
* Result: CBitmap *
*       Handle to bitmap, or NULL
****************************************************************************/

CBitmap * CSource::getBitmap()
    {
     return &bitmap;
    }

/****************************************************************************
*                             CSource::setBorder
* Inputs:
*       int border: Border factor, an integer expressing the ratio of the
*                   border size to the box size (e.g., 5 gives a margin
*                   of 1/5 the box size on each side).
*                   A value of 0 means that there is no border
*                   A value of -1 means to use the default border size
* Result: int
*       Previous border value
* Effect: 
*       Changes the border value.  
* Notes:
*       This does not explicitly invalidate the rectangle after a change;
*       the change will take effect the next time the object is redrawn.
*       You should do an InvalidateRect(NULL) operation to force a full redraw.
****************************************************************************/

int CSource::setBorder(int border)
    {
     int result = factor;
     factor = border;
     return result;
    }
