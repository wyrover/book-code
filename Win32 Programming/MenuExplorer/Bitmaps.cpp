// Bitmaps.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Bitmaps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmaps

CBitmaps::CBitmaps()
{
}

CBitmaps::~CBitmaps()
{
}


BEGIN_MESSAGE_MAP(CBitmaps, CComboBox)
        //{{AFX_MSG_MAP(CBitmaps)
        ON_WM_DESTROY()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmaps message handlers

/****************************************************************************
*                               CBitmaps::load
* Result: void
*       
* Effect: 
*       Loads the bitmap array
****************************************************************************/

struct bitmapdata {int strid; int bmid; } bitmaps[] = {
        {IDS_FLOUNDER, IDB_FLOUNDER},
        {IDS_GRAYCAT,  IDB_GRAYCAT},
        {IDS_COMPUTER, IDB_COMPUTER},
        {IDS_PLAID,    IDB_PLAID},
        { 0, 0} // EOT
                                                    };

void CBitmaps::load()
    {
     for(int i = 0; bitmaps[i].strid != 0; i++)
        { /* load each */
         CBitmap bm;
         bm.LoadBitmap(bitmaps[i].bmid);
         CString s;
         s.LoadString(bitmaps[i].strid);
         int index = CComboBox::AddString(s);
         if(index != CB_ERR)
            { /* add bm */
             SetItemData(index, (DWORD)(HBITMAP)bm);
            } /* add bm */
         bm.Detach(); // avoid deletion by destructor
        } /* load each */
     SetCurSel(0);
     
    }

void CBitmaps::OnDestroy() 
{
        CComboBox::OnDestroy();
        
     for(int i = 0; i < GetCount(); i++)
        { /* kill each */
         HBITMAP hbm = (HBITMAP)GetItemData(i);
         CBitmap bm;
         bm.Attach(hbm);
         bm.DeleteObject();
        } /* kill each */
        
}

/****************************************************************************
*                              CBitmaps::Select
* Inputs:
*       HMENU menu: menu to select
* Result: int
*       Index of selected item, or CB_ERR if error
* Effect: 
*       Selects the item whose itemdata matches the hmenu
****************************************************************************/

int CBitmaps::Select(HMENU menu)
    {
     for(int i = 0; i < GetCount(); i++)
        { /* check it out */
         if((HMENU)GetItemData(i) == menu)
            { /* found it */
             SetCurSel(i);
             return i;
            } /* found it */
        } /* check it out */
     SetCurSel(CB_ERR);  // remove any existing selection
     return CB_ERR;
    }

/****************************************************************************
*                            CBitmaps::GetCurItem
* Result: HBITMAP
*       Bitmap handler or NULL if error
****************************************************************************/

HBITMAP CBitmaps::GetCurItem()
    {
     int sel = GetCurSel();
     if(sel == CB_ERR)
        return NULL;
     return (HBITMAP)GetItemData(sel);
    }
