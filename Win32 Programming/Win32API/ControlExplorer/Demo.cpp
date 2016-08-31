#include "stdafx.h"
#include "resource.h"
#include <commctrl.h>
#include <afxtempl.h>

#include "msglog.h"
#include "TinyButt.h" 
#include "demo.h"
#include "resize.h"
#include "Tree.h"


/****************************************************************************
*                                 CTree::insertDemo
* Inputs:
*       treetable * tt: TreeTable entry
*	HTREEITEM parent: Parent for insertion
* Result: void
*       
* Effect: 
*       Inserts, under the current tree, an entry from the table
****************************************************************************/

treedata * CTree::insertDemo(treedata * data, HTREEITEM parent, int depth)
    {
     while(TRUE)
        { /* insert this */
	 TV_INSERTSTRUCT tvi;
	 tvi.hParent = parent;
	 tvi.hInsertAfter = TVI_LAST;

	 tvi.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;

	 CString s;
	 s.LoadString(data->str);

	 tvi.item.stateMask = TVIS_STATEIMAGEMASK;
         tvi.item.state = INDEXTOSTATEIMAGEMASK(data->state);

	 tvi.item.pszText = (LPTSTR)(LPCTSTR)s;
	 tvi.item.lParam = data->lParam;

	 HTREEITEM current = (HTREEITEM)c_Tree.SendMessage(TVM_INSERTITEM, 0, (LPARAM)&tvi);
     
	 data++;
	 while(data->depth > depth)
	    { /* deeper */
	     data = insertDemo(data, current, depth + 1);
	     if(data == NULL)
		return NULL;
	    } /* deeper */
         if(data->depth < depth)
	    { /* done */
	     return data;
	    } /* done */
	} /* insert this */
    }
