// MenuList.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "menuitem.h"
#include "xform.h"
#include "MenuList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuList

CMenuList::CMenuList()
{
 autoresize = FALSE;
 newsize.cx = 0;
 newsize.cy = 0;
 oldsize.cx = 0;
 oldsize.cy = 0;
 initialized = FALSE;
 zoomed = FALSE;
}

CMenuList::~CMenuList()
{
}


BEGIN_MESSAGE_MAP(CMenuList, CTreeCtrl)
	//{{AFX_MSG_MAP(CMenuList)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuList message handlers

/****************************************************************************
*                           CMenuList::SetItemData
* Inputs:
*	HTREEITEM ti: Item whose itemdata is to be set
*       menuitem * mi: item data to establish
* Result: BOOL
*       TRUE if successful, FALSE if error
* Effect: 
*       Sets the menuitem information for the selected item
****************************************************************************/

BOOL CMenuList::SetItemData(HTREEITEM ti, menuitem * mi)
    {
     return CTreeCtrl::SetItemData(ti, (LPARAM)mi);
    }

/****************************************************************************
*                           CMenuList::GetItemData
* Inputs:
*       HTREEITEM ti: Tree item whose menu data is to be returned
* Result: menuitem *
*       The menu item, or NULL if not set
****************************************************************************/

menuitem * CMenuList::GetItemData(HTREEITEM ti)
    {
     if(ti == NULL)
	return NULL;
     return (menuitem *)CTreeCtrl::GetItemData(ti);
    }

/****************************************************************************
*                           CMenuList::findItem
* Inputs:
*       HMENU menu: Menu item to find
*	HTREEITEM parent: Tree item to start search at; NULL => root
* Result: HTREEITEM
*       The HTREEITEM whose menu matches the specified menu
****************************************************************************/

HTREEITEM CMenuList::findItem(HMENU menu, HTREEITEM parent)
    {
     if(parent == NULL)
	 return findItem(menu, GetRootItem());

     menuitem * mi = GetItemData(parent);
     if(mi != NULL && mi->hmenu == menu)
	 return parent;

     if(ItemHasChildren(parent))
        { /* process child windows */
	 HTREEITEM ti = findItem(menu, GetChildItem(parent));
	 if(ti != NULL)
	    return ti;  // found it in child list
	} /* process child windows */

     // The menu is not the parent, and was not found in the child list,
     // walk thru sibling list

     for(HTREEITEM ci = GetNextSiblingItem(parent); ci != NULL; ci = GetNextSiblingItem(ci))
        { /* try each */
	 menuitem * mi = GetItemData(ci);
	 if(mi != NULL && mi->hmenu == menu)
	    return ci;

	 HTREEITEM ti = NULL;
	 if(ItemHasChildren(ci))
	    ti = findItem(menu, ci);
	 if(ti != NULL)
	    return ti;
	} /* try each */

     // doesn't exist
     return NULL;
    }

/****************************************************************************
*                           CMenuList::findMenuItem
* Inputs:
*       HMENU menu: Menu item to search for
*	HTREEITEM Parent: Parent, NULL to start at root
* Result: menuitem *
*       The associated menuitem value, or NULL if error or not found
****************************************************************************/

menuitem * CMenuList::findMenuItem(HMENU menu, HTREEITEM parent)
    {
     HTREEITEM ti = findItem(menu, parent);
     if(ti == NULL)
	return NULL;
     return GetItemData(ti);
    }

/****************************************************************************
*                            CMenuList::InsertItem
* Inputs:
*	menuitem * mi: Menu item to be inserted
*       HTREEITEM parent: Tree item below which this is inserted
* Result: HTREEITEM
*       New tree item
* Effect: 
*       Inserts the item using the mi->name field as the text, attaches
*	the menuitem as the itemdata
****************************************************************************/

HTREEITEM CMenuList::InsertItem(menuitem * mi, HTREEITEM parent)
    {
     HTREEITEM t = CTreeCtrl::InsertItem(mi->name, parent);
     if(t == NULL)
	return NULL;
     SetItemData(t, mi);
     return t;
    }

/****************************************************************************
*                          CMenuList::deleteChildren
* Inputs:
*       HTREEITEM ti: Tree item of interest
* Result: void
*       
* Effect: 
*       Deletes all child nodes of the tree node specified
* Notes:
*	Note that the next-child node is carefully computed *before* the
*	child node is deleted!
****************************************************************************/

void CMenuList::deleteChildren(HTREEITEM ti)
    {
     if(!ItemHasChildren(ti))
	return;
  
     HTREEITEM nextchild;
	 
     for(HTREEITEM child = GetChildItem(ti); child != NULL; 
				          child = nextchild)
        { /* delete each child */
	 nextchild = GetNextSiblingItem(child);
	 DeleteItem(child);
	} /* delete each child */
    }

/****************************************************************************
*                       CMenuList::insertMenuItems
* Inputs:
*       CMenu * menu: Top level menu 
*	HTREEITEM root: Root object into which to insert components
* Result: void
*       
* Effect: 
*       Constructs the menu item list
****************************************************************************/

void CMenuList::insertMenuItems(CMenu * menu, HTREEITEM parent)
    {
     for(UINT i = 0; i < menu->GetMenuItemCount(); i++)
        { /* add menu items */
	 CString s;
	 int state = menu->GetMenuState(i, MF_BYPOSITION);
	 int id = menu->GetMenuItemID(i);
	 
	 if(state & MF_BITMAP)
	    { /* bitmap */
	     s.LoadString(IDS_BITMAP_MENU);
	    } /* bitmap */
	 else
	 if((state & MF_POPUP) == 0)
	    { /* non-popup */
	     if(state & MF_SEPARATOR)
		{ /* separator */
		 s.LoadString(IDS_SEPARATOR);
		} /* separator */
	     else
	     if(state & MF_OWNERDRAW)
	        { /* owner-draw */
		 s.LoadString(IDS_OWNERDRAW_MENU);
		} /* owner-draw */
	     else
	        { /* string */
		 menu->GetMenuString(i, s, MF_BYPOSITION);
		 s = strToEsc(s); // expand \t to "\t", for example
		} /* string */
	    } /* non-popup */
	 else
	    { /* popup */
	     menu->GetMenuString(i, s, MF_BYPOSITION);
	     s = strToEsc(s);
	    } /* popup */


	 menuitem * item = new menuitem;
	 CMenu * sub = menu->GetSubMenu(i);

	 if(sub != NULL)
	    { /* recurse */
	     item->flags = M_POPUP;
	     item->hmenu = sub->m_hMenu;
	     item->name = s; 
	     item->id = id;
	     HTREEITEM ti = InsertItem(item, parent);
	     insertMenuItems(sub, ti);
	    } /* recurse */
	 else
	    { /* normal */
	     item->flags = M_COMMAND;
	     item->hmenu = NULL;
	     item->name = s;
	     item->id = id;
	     InsertItem(item, parent);
	    } /* normal */
	} /* add menu items */
    }

/****************************************************************************
*                            CMenuList::getItemPos
* Inputs:
*       HTREEITEM ti: Tree item
* Result: int
*       Position of item in subtree (and by inference, its position
*	in the menu list)
****************************************************************************/

int CMenuList::getItemPos(HTREEITEM ti)
    {
     int i = 0;
     for(HTREEITEM pi = GetPrevSiblingItem(ti); pi != NULL; 
				pi = GetPrevSiblingItem(pi), i++) 
	{
	 // do nothing
	}
     return i;	    
    }


/****************************************************************************
*                            CMenuList::OnSetFocus
* Inputs:
*       CWnd * pOldWnd: Window losing focus
* Result: void
*       
* Effect: 
*       If the control is about to gain focus, expand it so that it zooms
*	out and shows more useful info.  We can set the new size via the
*	setZoomSize method, or if it is not set, we default to something
*	that seems reasonable at the time.
* Notes:
*	For this to work properly, the control must have a higher tab order
*	than any control which it would cover.  
*
*	Nothing is simple.  We also have to force HWMD_TOPMOST on it to 
*	make sure that it reacts properly.  We also have to react to
*	WM_ACTIVATEAPP to remove the HWND_TOPMOST flag, but WM_ACTIVATEAPP
*	is only sent to the topmost frame, so the frame has to know to 
*	forward it to us!
*
*	To do this properly, we should ideally deactivate any controls it
*	covers, and re-enable them when it unzooms.  The algorithm for doing
*	this is as follows:
*		* Compute the rectangle of the zoomed control
*		* For each control in the dialog, if it intersects the zoomed
*		  control, 
*			* get its current enabled state
*			* hide it
*			* Save the previous enabled state of each window
*			  using SetProp
*	When the window is unzoomed
*		* For each control in the dialog, if it has a saved-enable
*		  property:
*			* set its enabled state to the state saved
*			* remove the property
*	Note this has the implication that the TAB key might logically want
*	to move to one of the disabled controls, which means we have to 
*	additionally intercept the tab/shift-tab key and immediately unzoom.
*	This is a lot of work.  
****************************************************************************/

void CMenuList::OnSetFocus(CWnd* pOldWnd) 
{
	CTreeCtrl::OnSetFocus(pOldWnd);

 	if(!autoresize)
	   return;
	
	CRect r;
	CRect parent;
	GetWindowRect(&r);
	GetParent()->GetClientRect(&parent);
        if(!initialized)
	   { /* initialize */
	    if(newsize.cx == 0)
	       { /* default width */
	        newsize.cx = 3 * r.Width() / 2;  // arbitrary choice 1.5 wider
	       } /* default width */

	    if(newsize.cy == 0)
	       { /* default height */
		newsize.cy = parent.Height();
	       } /* default height */
	    oldsize.cx = r.Width();
	    oldsize.cy = r.Height();
	    zoomed = FALSE;
	    initialized = TRUE;
	   } /* initialize */
	
	if(!zoomed)
	   { /* zoom it */
	    CWnd * pred = GetWindow(GW_HWNDPREV);
	    if(pred != NULL)
	       predecessor = pred->m_hWnd;
            else
	       pred = NULL;

	    SetWindowPos(&wndTopMost, 0, 0, newsize.cx, newsize.cy, 
	    					SWP_NOMOVE);
	    zoomed = TRUE;
	   } /* zoom it */

}

void CMenuList::OnKillFocus(CWnd* pNewWnd) 
{
	CTreeCtrl::OnKillFocus(pNewWnd);
	
 	if(zoomed)
	   { /* restore */
	    SetWindowPos(CWnd::FromHandle(predecessor), 
	    				0, 0, oldsize.cx, oldsize.cy, 
	    					SWP_NOMOVE);
	    zoomed = FALSE;
	    HTREEITEM ti = GetSelectedItem();
	    EnsureVisible(ti);
	   } /* restore */
	
}
