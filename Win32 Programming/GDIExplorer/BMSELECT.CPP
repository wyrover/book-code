// BitmapSelect.cpp : implementation file
//
#define OEMRESOURCE

#include "stdafx.h"
#include "GDIExplorer.h"
#include "image.h"
#include "bmSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Note that we have carefully chosen the IDs so that the integer ID
// of the bitmap resource is the same name as the integer ID of the 
// string which describes it.
static int bitmaps[] = {
         IDB_RGB1,
         IDB_RGB2,
         IDB_RGB3,
         IDB_RGB4,
         IDB_RGB5,
         IDB_RGB6,
         IDB_DESERT,
         IDB_GRADED,
         IDB_PLAID,
         IDB_REDBRICK,
         IDB_STAR,
         IDB_MASK1,
         IDB_MASK2,
         IDB_MASK3,
         IDB_MASK4,
         IDB_VSTRIPES,
         IDB_HSTRIPES,
         IDB_MARBLE,
         0  // EOT
                          };

/////////////////////////////////////////////////////////////////////////////
// CBitmapSelect

CBitmapSelect::CBitmapSelect()
{
 maxlen = -1;  // force to stay on screen
}

CBitmapSelect::~CBitmapSelect()
{
}


BEGIN_MESSAGE_MAP(CBitmapSelect, CImageCombo)
        //{{AFX_MSG_MAP(CBitmapSelect)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapSelect message handlers

void CBitmapSelect::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 if(dis->itemID == -1)
    return;  // nothing to draw

 DWORD cmd = ::GetMenuCheckMarkDimensions();
 CSize cmsize(LOWORD(cmd), HIWORD(cmd));
 
 CDC * dc = CDC::FromHandle(dis->hDC);

 CDC source;
 source.CreateCompatibleDC(dc);

 CBitmap bi;
 BITMAPINFO bmi;



// if(dis->itemID == (UINT)GetCurSel())
 if(!(dis->itemAction & ODA_SELECT || dis->itemAction & ODA_DRAWENTIRE))
    { /* selected */
     bi.m_hObject = ::LoadBitmap(0, MAKEINTRESOURCE(OBM_CHECK));
     bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
     bmi.bmiHeader.biBitCount = 0; // want size returned
     if(::GetDIBits(dc->m_hDC, bi, 0, 0, NULL, &bmi, DIB_RGB_COLORS) != 0)
        { /* got checkmark */
         source.SelectObject(bi);
         dc->BitBlt(dis->rcItem.left, dis->rcItem.top,
                    bmi.bmiHeader.biWidth,
                    bmi.bmiHeader.biHeight,
                    &source, 0, 0, SRCCOPY);
        } /* got checkmark */
     bi.DeleteObject();
    } /* selected */
 else
    { /* not selected */
     // This is necessary so Windows95 will erase the checkmark it drew
     // when the selection moves on to another element.  Without this, the
     // code works correctly in WIN/NT3.x, but for the 4.x GUI we must
     // erase the checkmark

     CRect r(dis->rcItem.left, dis->rcItem.top, 
             dis->rcItem.left + LOWORD(cmd),
             dis->rcItem.top + HIWORD(cmd));
     CBrush fill(::GetSysColor(COLOR_WINDOW));
     dc->FillRect(&r, &fill);
    } /* not selected */
 

 bi.LoadBitmap(dis->itemData);
 source.SelectObject(bi);
 
 CSize sz;

 bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
 bmi.bmiHeader.biBitCount = 0; // want size returned
 if(::GetDIBits(dc->m_hDC, bi, 0, 0, NULL, &bmi, DIB_RGB_COLORS) != 0)
    { /* got bitmap */
     dc->BitBlt(cmsize.cx + dis->rcItem.left, dis->rcItem.top, 
                min(bmi.bmiHeader.biWidth,
                    dis->rcItem.right - dis->rcItem.left - cmsize.cx),
                bmi.bmiHeader.biHeight,
                &source,
                0, 0,
                SRCCOPY);
    } /* got bitmap */
 
        
 bi.DeleteObject();
}

void CBitmapSelect::MeasureItem(LPMEASUREITEMSTRUCT mis) 
{
 if(mis->itemID == -1)
    return;  // no can do...no item there

 int item;
 BITMAPINFO bmi;
 CSize sz;
 CPoint pt;
 CClientDC dc(this);
 CBitmap bi;

 item = GetItemData(mis->itemID);

 if(!bi.LoadBitmap(item))
    return;  // load failed

 bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
 bmi.bmiHeader.biBitCount = 0; // want size returned
 if(::GetDIBits(dc.m_hDC, bi, 0, 0, NULL, &bmi, DIB_RGB_COLORS) != 0)
    { /* got bitmap */
     mis->itemHeight = bmi.bmiHeader.biHeight + 2;
    } /* got bitmap */
 
        
 bi.DeleteObject();
}

/****************************************************************************
*                             CMitmapSelect::Load
* Inputs:
*       int sel: Selection to make
* Result: int
*       Index of selection made
* Effect: 
*       Loads the bitmap elements into the combo box
****************************************************************************/

int CBitmapSelect::Load(int sel)
    {
     return CImageCombo::AddStrings(bitmaps, sel);
    }
