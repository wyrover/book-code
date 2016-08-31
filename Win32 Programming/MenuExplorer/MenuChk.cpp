// MenuChk.cpp : implementation file
//

#include "stdafx.h"
#include "MenuExplorer.h"
#include "MenuChk.h"
#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuCheck

CMenuCheck::CMenuCheck()
{
 maxlen = 0;
}

CMenuCheck::~CMenuCheck()
{
}


BEGIN_MESSAGE_MAP(CMenuCheck, CComboBox)
        //{{AFX_MSG_MAP(CMenuCheck)
        ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuCheck message handlers


/****************************************************************************
*                              CMenuCheck::load
* Result: void
*       
* Effect: 
*       Loads the stock bitmaps
****************************************************************************/

static int bmps[] = { IDB_CHECK1, IDB_CHECK2, IDB_CHECK3, IDB_CHECK4,
                      IDB_CHECK5, IDB_CHECK6, IDB_CHECK7, IDB_CHECK8,
                      0 // EOT
                    };

void CMenuCheck::load()
    {
     for(int i = 0; bmps[i] != 0; i++)
        { /* load each */
         CBitmap bm;
         bm.LoadBitmap(bmps[i]);
         int index = CComboBox::AddString((LPCTSTR)(HBITMAP)bm);
         bm.Detach();  // avoid deletion by destructor
        } /* load each */
    }

void CMenuCheck::DeleteItem(LPDELETEITEMSTRUCT dis) 
{
        HBITMAP bm = (HBITMAP)dis->itemData;

        if(bm != NULL)
           ::DeleteObject(bm);
        
        CComboBox::DeleteItem(dis);
}

void CMenuCheck::DrawItem(LPDRAWITEMSTRUCT dis) 
{
 CRect r = dis->rcItem;
 CDC * dc = CDC::FromHandle(dis->hDC);
 CBrush br;
 CBrush * oldbrush;
 COLORREF txcolor;
 COLORREF bkcolor;
 int op;

 int saved = dc->SaveDC();

 if(dis->itemState & ODS_SELECTED)
    { /* selected */
     bkcolor = GetSysColor(COLOR_HIGHLIGHT);
     txcolor = GetSysColor(COLOR_HIGHLIGHTTEXT);
     op = MERGEPAINT;
    } /* selected */
 else
    { /* unselected */
     if(dis->itemState & (ODS_DISABLED | ODS_GRAYED))
        txcolor = GetSysColor(COLOR_GRAYTEXT);
     else
        txcolor = GetSysColor(COLOR_WINDOWTEXT);

     bkcolor = GetSysColor(COLOR_WINDOW);
     op = SRCCOPY;
    } /* unselected */

 dc->SetBkColor(bkcolor);
 dc->SetTextColor(txcolor);

 br.CreateSolidBrush(bkcolor);
 oldbrush = dc->SelectObject(&br);

 dc->PatBlt(r.left, r.top, r.Width(), r.Height(), PATCOPY);

 dc->SelectObject(oldbrush);
 br.DeleteObject();

 if(dis->itemID != (UINT)-1 
    && (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) == 0)
    { /* has item */
     HBITMAP bm = (HBITMAP)(dis->itemData);

     
     // Since we don't know the actual text size, and TextOut will 
     // write anywhere it wants to (including over top of the dropdown
     // button, at least on NT3.51), we force a clipping region.

     int tsaved = dc->SaveDC();
     CRgn hrgn;

     hrgn.CreateRectRgnIndirect(&dis->rcItem);
     dc->SelectClipRgn(&hrgn);
     
     if(bm == NULL)
        { /* no bitmap */
         CString s;
         s.LoadString(IDS_NULL);

         dc->TextOut(r.left, r.top, s);
        } /* no bitmap */
     else
        { /* has bitmap */
         CDC memDC;
         CSize sz;
         if(Is3())
            { /* old API */
             DWORD size = GetMenuCheckMarkDimensions();
             sz.cx = LOWORD(size);
             sz.cy = HIWORD(size);
            } /* old API */
         else
            { /* new API */
             sz.cx = ::GetSystemMetrics(SM_CXMENUCHECK);
             sz.cy = ::GetSystemMetrics(SM_CYMENUCHECK);
            } /* new API */

         memDC.CreateCompatibleDC(dc);
         CBitmap * oldbm = memDC.SelectObject(CBitmap::FromHandle(bm));
         dc->BitBlt(dis->rcItem.left, dis->rcItem.top, sz.cx, sz.cy, &memDC,
                                        0, 0, op);
         memDC.SelectObject(oldbm);
        } /* has bitmap */

     dc->RestoreDC(tsaved);
    } /* has item */
 
 if(dis->itemState & ODS_FOCUS && dis->itemAction != ODA_SELECT)
    dc->DrawFocusRect(&r);

 dc->RestoreDC(saved);
        
        
}

void CMenuCheck::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
        // TODO: Add your code to determine the size of specified item
        
}

/****************************************************************************
*                            CMenuCheck::GetCurItem
* Result: HBITMAP
*       Handle to bitmap; could be NULL if no selection or selection has
*       NULL handle
****************************************************************************/

HBITMAP CMenuCheck::GetCurItem()
    {
     int i = GetCurSel();
     if(i == CB_ERR)
        return NULL;
     HBITMAP bm = (HBITMAP)GetItemData(i);
     return bm;
    }

void CMenuCheck::OnDropdown() 
{
 int n = GetCount();
 n = max(n, 2);

 int ht = GetItemHeight(0);
 CRect r;
 GetWindowRect(&r);

 if(maxlen > 0)
    n = max(maxlen, 2);

 CSize sz;
 sz.cx = r.Width();
 sz.cy = ht * (n + 2);

 if(maxlen < 0)
    { /* screen limit */
     if(r.top - sz.cy < 0 || r.bottom + sz.cy > ::GetSystemMetrics(SM_CYSCREEN))
        { /* invoke limit */
         // Compute the largest distance the dropdown can appear, 
         // relative to the screen (not the window!)

         int k = max( (r.top / ht), 
                      (::GetSystemMetrics(SM_CYSCREEN) - r.bottom) / ht);

         // compute new space. Note that we don't really fill the screen.
         sz.cy = ht * k;
        } /* invoke limit */
    } /* screen limit */

 SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOMOVE | SWP_NOZORDER);
        
}
