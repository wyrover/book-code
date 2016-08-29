#include "stdafx.h"
#include "ControlExplorer.h"
#include "msglog.h"
#include "ixable.h"
#include "loggable.h"
#include "treeOps.h"

IMPLEMENT_DYNCREATE(CTreeOps, CLoggingPage)

#define DIM(x) (sizeof(x) / sizeof((x)[0]))

CTreeOps::CTreeOps() : CLoggingPage((int)0) // should never call this
{
 ASSERT(FALSE); // Must never call in context where default constructor used
}

CTreeOps::CTreeOps(int idd) : CLoggingPage(idd)
{
}

CTreeOps::~CTreeOps()
{
}

/****************************************************************************
*                             CTreeOps::walkTree
* Inputs:
*       CTreeCtrl * tree: Tree to walk, or NULL for c_Tree
*       HTREEITEM item: Item to start at, or TVI_ROOT
*       BOOL (*callback)(CTreeCtrl * tree, HTREEITEM item, LPARAM data, int depth): 
*                                               Callback function
*       LPARAM data: Data item passed to callback
*       int depth: indentation depth; should start at 0 (the default)
* Result: BOOL
*       When called from top level, TRUE indicates the entire tree has been
*       walked successfully; FALSE indicates a premature termination has
*       been requested.
*
*       When called recursively, the value is passed back to the caller.
* Effect: 
*       Iterates down the tree.  The callback should return FALSE to
*       terminate iteration.
****************************************************************************/

BOOL CTreeOps::walkTree(CTreeCtrl * tree, HTREEITEM item, BOOL (*callback)(CTreeCtrl * tree, HTREEITEM item, LPARAM data, int depth), LPARAM data, int depth)
    {
     if(item == NULL)
        item = tree->GetRootItem();  // can only pass in NULL at topmost level
     if(tree == NULL)
        tree = c_Tree;

     while(item != NULL)
        { /* all siblings */
         if(!callback(tree, item, data, depth))  // call for this item
            return FALSE;

         HTREEITEM child = tree->GetChildItem(item);
         if(child != NULL)
            { /* recurse */
             if(!walkTree(tree, child, callback, data, depth + 1))
                return FALSE;
            } /* recurse */
         item = tree->GetNextSiblingItem(item);
        } /* all siblings */
     return TRUE; // success
    }

/****************************************************************************
*                                 addToCombo
* Inputs:
*       CTreeCtrl * tree: Tree being addressed
*       HTREEITEM item: Item currently being considered
*       LPARAM data: Arbitrary data item (in this case, CComboBox *)
*       int depth: Depth of call
* Result: BOOL
*       TRUE, in principle always
* Effect: 
*       Adds the element to the list box
****************************************************************************/

BOOL CTreeOps::addToCombo(CTreeCtrl * tree, HTREEITEM item, LPARAM data, int depth)
    {
     TV_ITEM tvitem;
     TCHAR text[256];
     tvitem.mask = TVIF_TEXT;
     tvitem.pszText = text;
     tvitem.cchTextMax = DIM(text);
     tvitem.hItem = item;
     tree->GetItem(&tvitem);
     
     CString s; 
     s.Format(_T("%s"), text);
     for(int i = 0; i < depth; i++)
        s = _T(" ") + s;  // fake treeliness

     CComboBox * combo = (CComboBox *)data;
     int index = combo->AddString(s);
     combo->SetItemData(index, (LPARAM)item);
     return index != CB_ERR;
    }

/****************************************************************************
*                         CTreeOps::loadComboFromTree
* Inputs:
*       CComboBox * combo: Combo box to load
*       BOOL include_TVI_ROOT: TRUE to include TVI_ROOT, FALSE to exclude it
* Result: void
*       
* Effect: 
*       Loads up the combo box from c_Tree
****************************************************************************/

void CTreeOps::loadComboFromTree(CComboBox * combo, BOOL include_TVI_ROOT)
    {
     // Remember the selection so we can re-select it if it is still present
     int selection = combo->GetCurSel();
     HTREEITEM selection_item;

     if(selection != CB_ERR)
        selection_item = (HTREEITEM)combo->GetItemData(selection);
     else
        selection_item = TVI_ROOT;

     combo->ResetContent();

     if(include_TVI_ROOT)
        { /* root */
         CString s;
         s.LoadString(IDS_TVI_ROOT);

         int index = combo->AddString(s);  // first entry is always TVI_ROOT
                                           // combo box is *not* sorted!
         combo->SetItemData(index, (DWORD)TVI_ROOT);
        } /* root */

     // Now call the walker

     walkTree(c_Tree, c_Tree->GetRootItem(), 

                                        addToCombo, (LPARAM)combo);

     BOOL found = FALSE;

     for(int i = 0; i < combo->GetCount(); i++)
        { /* scan list */
         HTREEITEM t = (HTREEITEM)combo->GetItemData(i);
         if(t == selection_item)
            { /* found it */
             combo->SetCurSel(i);
             found = TRUE;
             break;
            } /* found it */
        } /* scan list */

     // If the entry wasn't found, it was because the last operation was a delete

     if(!found)
        { /* not found */
         combo->SetCurSel(CB_ERR); // no selection
        } /* not found */
     
    }


/****************************************************************************
*                          CTreeOps::addMessage
* Inputs:
*       CString s: String to add
* Result: void
*       
* Effect: 
*       Logs the formatted string
****************************************************************************/

void CTreeOps::addMessage(CString s)
    {
     CString ctlname;
     ctlname.LoadString(IDS_TREE);
     c_Messages->addMessage(c_Tree->m_hWnd, ctlname, s);
    }

/****************************************************************************
*                            CTreeOps::itemString
* Inputs:
*       HTREEITEM item: Item to be formatted
* Result: CString
*       The display version of the item
* Effect: 
*       Formats the item for message logging
****************************************************************************/

CString CTreeOps::itemString(HTREEITEM item)
    {
     CString s;

     if(item != NULL)
        { /* non-NULL */
         CString is;
         is = c_Tree->GetItemText(item);
         s.Format(_T("%08x: \"%s\""), item, is);
        } /* non-NULL */
     else
        { /* NULL */
         s.LoadString(IDS_NULL);
        } /* NULL */

     return s;
    }
