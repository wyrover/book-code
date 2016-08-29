#include "stdafx.h"
#include "resource.h"
#include "odmenu.h"
#include "odinfo.h"

/****************************************************************************
*                       COwnerDrawMenu::COwnerDrawMenu
* Effect: 
*       Constructor
****************************************************************************/

COwnerDrawMenu::COwnerDrawMenu()
    {
     
    }

/****************************************************************************
*                       COwnerDrawMenu::~COwnerDrawMenu
* Effect: 
*       Detaches the menu from the class.  This is necessary so the menu
*	is not accidently destroyed before its time, since it is normally
*	part of another menu which may have a different extent.
* Notes:
*	See the article "MFC Self-Drawing Menus" by Dale Robertson, on the
*	MSDN CD-ROM, for much more detail about how this entire package
*	works.
****************************************************************************/

COwnerDrawMenu::~COwnerDrawMenu()
    {
     Detach();
    }

/****************************************************************************
*                         COwnerDrawMenu::MeasureItem
* Inputs:
*       LPMEASUREITEMSTRUCT mis: Measure structure
* Result: void
*       
* Effect: 
*       Sets the width and height fields of the measure item
****************************************************************************/

void COwnerDrawMenu::MeasureItem(LPMEASUREITEMSTRUCT mis)
    {
     int checkwidth = LOWORD(GetMenuCheckMarkDimensions());
     odinfo * mi = (odinfo *)mis->itemData;
     CSize size;

     if(mi->height == 0 || mi->width == 0)
        { /* get text height */
	 CClientDC dc(AfxGetMainWnd());
	 size = dc.GetTextExtent(mi->s);
	} /* get text height */

     if(mi->height > 0)
	mis->itemHeight = mi->height;
     else
	mis->itemHeight = size.cy;

     if(mi->width > 0)
	mis->itemWidth  = mi->width;
     else
	mis->itemWidth  = size.cx;

     if(mi->subtractCheckWidth)
	mis->itemWidth -= checkwidth;
    }

/****************************************************************************
*                          COwnerDrawMenu::DrawItem
* Inputs:
*       LPDRAWITEMSTRUCT dis: DrawItem struct
* Result: void
*       
* Effect: 
*       Draws the menu item according to the selected style
****************************************************************************/

void COwnerDrawMenu::DrawItem(LPDRAWITEMSTRUCT dis)
    {
     odinfo * mi = (odinfo *)dis->itemData;
     
     switch(mi->style)
        { /* decode */
	 case 0:
		 // illegal case, return
		 return;
	 case 1:
		 // 
		 DrawItem1(dis);
		 return;
	 case 2:
		 DrawItem2(dis);
		 return;
	} /* decode */
    }

/****************************************************************************
*                          COwnerDrawMenu::DrawItem1
* Inputs:
*       LPDRAWITEMSTRUCT dis: 
* Result: void
*       
* Effect: 
*       Draws a style-1 menu.  This menu is very simple: it looks exactly
*	like a regular menu except that it uses custom colors for the 
*	following:
*		menu text
*		menu background
*		highlighted text
*		highlighted background
*	it uses a custom bitmap for a checkmark.
****************************************************************************/

void COwnerDrawMenu::DrawItem1(LPDRAWITEMSTRUCT dis)
    {
     odinfo * mi = (odinfo *)dis->itemData;
     COLORREF text;  // text color
     COLORREF bkg;  // background color
     CDC * dc = CDC::FromHandle(dis->hDC);
     int rop;

     int saved = dc->SaveDC();

     if(dis->itemState & ODS_SELECTED)
        { /* item is selected */
	 text = mi->hilitetext;
	 bkg = mi->hilitebackground;
	 rop = MERGEPAINT;
	} /* item is selected */
     else
        { /* unselected */
	 text = mi->text;
	 bkg = mi->background;
	 rop = SRCAND;
	} /* unselected */

     // In either case, if the item is grayed, show it in the gray color
     if(dis->itemState & ODS_GRAYED);
	text = mi->gray;

     RECT r = dis->rcItem;
     CSize checksize;
     checksize.cx = LOWORD(GetMenuCheckMarkDimensions());
     checksize.cy = HIWORD(GetMenuCheckMarkDimensions());

     CBrush br(bkg);
     dc->FillRect(&dis->rcItem, &br);

     // Compute a new position allowing space for the checkmark
     if(!mi->subtractCheckWidth)
	r.left += checksize.cx;

     dc->SetBkMode(TRANSPARENT);
     dc->SetTextColor(text);

     dc->TextOut(r.left, r.top, mi->s, lstrlen(mi->s));

     // Now draw the checkmark if the item is checked
     if(dis->itemState & ODS_CHECKED)
        { /* draw checkmark */
	 CDC memDC;
	 memDC.CreateCompatibleDC(dc);
	 CBitmap * oldbm = memDC.SelectObject(CBitmap::FromHandle(mi->bmcheck));
	 dc->BitBlt(dis->rcItem.left, dis->rcItem.top, checksize.cx, 
	 				checksize.cy, &memDC, 0, 0, rop);
	} /* draw checkmark */
     dc->RestoreDC(saved);
    }

/****************************************************************************
*                          COwnerDrawMenu::DrawItem2
* Inputs:
*       LPDRAWITEMSTRUCT dis: 
* Result: void
*       
* Effect: 
*       Draws a style-2 menu. 
****************************************************************************/

void COwnerDrawMenu::DrawItem2(LPDRAWITEMSTRUCT dis)
    {
    }
