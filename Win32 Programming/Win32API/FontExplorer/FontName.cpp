// FontName.cpp : implementation file
//

#include "stdafx.h"
#include "FontExplorer.h"
#include "FontName.h"
#include "SaveDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontName

CFontName::CFontName()
{
}

CFontName::~CFontName()
{
}


BEGIN_MESSAGE_MAP(CFontName, CComboBox)
        //{{AFX_MSG_MAP(CFontName)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontName message handlers

void CFontName::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
        // TODO: Add your code to determine the size of specified item
        
}

/****************************************************************************
*                             CFontName::DrawItem
* Inputs:
*       LPDRAWITEMSTRUCT lpDrawItemStruct
* Result: void
*       
* Effect: 
*       Draws the font name, including the icon representing it.
* Notes:
*       Checks the DEVICE_FONTTYPE (D), RASTER_FONTTYPE (R) and 
*       TRUETYPE_FONTTYPE (T) bits and includes the bitmap according to
*       the following table:
*
*       D  R  T    icon         Bitmap
*       ===================================
*       0  0  0    Vector       IDB_VECFONT
*       0  0  1    TT           IDB_TTFONT
*       0  1  0    Raster       IDB_RASFONT
*       0  1  1    ---          none
*       1  0  0    Device       IDB_DEVFONT
*       1  0  1    ---          none
*       1  1  0    ---          none
*       1  1  1    ---          none
*
*       We don't have to delete the CDC * we obtain because it is a temporary
*       object and will be deleted during idle time
*
*       The control is an owner-draw control with CBS_HASSTRINGS; the itemdata
*       is the control ID
****************************************************************************/

void CFontName::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 CDC * dc = CDC::FromHandle(lpDrawItemStruct->hDC);
 CString fontname;

 if(lpDrawItemStruct->itemID == 0xFFFF )
    { /* only focus */
     dc->DrawFocusRect(&lpDrawItemStruct->rcItem);
     return; // see note
    } /* only focus */

  UINT bmpid;

  UINT id = (UINT)lpDrawItemStruct->itemData;

  switch(id & (TRUETYPE_FONTTYPE | RASTER_FONTTYPE | DEVICE_FONTTYPE))
     { /* id */
      case 0:                           // 0 0 0
              bmpid = IDB_VECFONT;
              break;
      case TRUETYPE_FONTTYPE:           // 0 0 1
              bmpid = IDB_TTFONT;
              break;
      case RASTER_FONTTYPE:             // 0 1 0
              bmpid = IDB_RASFONT;
              break;
      case DEVICE_FONTTYPE:             // 1 0 0
              bmpid = IDB_DEVFONT;
              break;
      default:                          // everything else
              bmpid = IDB_BLANKFONT;
              break;
     } /* id */
  
  GetLBText(lpDrawItemStruct->itemID, fontname);

  { /* draw entry */
   CSaveDC sdc(dc);
   CRect rctemp;
   
   rctemp = lpDrawItemStruct->rcItem;
   rctemp.left ++;
   rctemp.top ++;
   CBitmap bmp;
   BITMAP BitmapData;

   CBrush br;

   bmp.LoadBitmap(bmpid);

   bmp.GetObject(sizeof(BitmapData), &BitmapData);
   
   if(lpDrawItemStruct->itemState & ODS_SELECTED)
      { /* selected */
       sdc->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
       sdc->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
       br.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
      } /* selected */
   else
      { /* not selected */
       sdc->SetBkColor(::GetSysColor(COLOR_WINDOW));
       sdc->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
       br.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
      } /* not selected */

   sdc->SelectObject(&br);
   sdc->FillRect(&lpDrawItemStruct->rcItem, &br);

   sdc->SetBkMode(TRANSPARENT);

   {
    CDC memDC;
    memDC.CreateCompatibleDC(sdc);
    memDC.SelectObject(&bmp);
    sdc->BitBlt(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top,
               BitmapData.bmWidth, BitmapData.bmHeight, &memDC, 0, 0, SRCCOPY);
    rctemp.left += BitmapData.bmWidth + 6;
   }
   sdc->ExtTextOut(rctemp.left, rctemp.top, 0, &rctemp, fontname, fontname.GetLength(), NULL);

   if(lpDrawItemStruct->itemState & ODS_FOCUS && lpDrawItemStruct->itemAction != ODA_SELECT)
      sdc->DrawFocusRect(&lpDrawItemStruct->rcItem);
  } /* draw entry */

}

int CFontName::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
        // TODO: Add your code to determine the sorting order of the specified items
        // return -1 = item 1 sorts before item 2
        // return 0 = item 1 and item 2 sort the same
        // return 1 = item 1 sorts after item 2
        
        return 0;
}
