/****************************** Module Header *******************************
* Module Name: VIEW.C
*
* Maps rows in window to items in COMPLIST
*
* Functions:
*
* view_new()
* view_setcomplist()
* view_getcomplist()
* view_close()
* view_delete()
* view_outline()
* view_expand()
* view_gettext()
* view_getlinenr_left()
* view_getlinenr_right()
* view_getwidth()
* view_getrowcount()
* view_getstate()
* view_getitem()
* view_isexpanded()
* view_getcurrenttag()
* view_newitem()
* view_changeviewoptions()
* view_changediffoptions()
* view_findchange()
* view_outline_opt()
* view_freemappings()
* view_findrow()
* view_expand_item()
*
* Comments:
*
* A view owns a COMPLIST, and talks to a table window. The table window
* shows 3 columns: line nr, tag and text. We also need to supply a state
* for each row (used to select colour scheme).
*
* The COMPLIST can give us a list of its COMPITEMs. Each of these can give
* us a tag (eg the filenames compared) and the text (usually the compare
* result), and the state. We make the line number from the
* COMPITEM's place in the list.
*
* If we are asked to switch to 'expand' mode, we ask the selected COMPITEM
* for its composite section list. We can then get the state (and thus
* the tag) from each SECTION, and the line nr and text from the LINEs within
* each section.
*
* When moving between expand and outline, and when refreshing the view
* for some option change, we have to be careful to keep the current row
* and the selected row in the table what the user would expect.
*
* Functions in this module can be called from the UI thread (to refresh
* the display) and simultaneously from a worker thread to update the
* view mapping (view_setcomplist, view_newitem). We use a critical section
* to manage the synchronisation. We need to protect all access/modification
* to the view structure elements (particularly bExpand, rows, pLines and
* pItems), BUT we must not hold the critical section over any calls
* to SendMessage.
*
* We use the global options in windiff.h, and we allocate memory from the
* heap hHeap which has been initialised elsewhere. Points in time-intensive
* loops call Poll() defined elsewhere.
*
****************************************************************************/

#include <windows.h>
#include <stdlib.h>
#include <commdlg.h>

#include "gutils.h"
#include "table.h"
#include "state.h"
#include "windiff.h"
#include "wdiffrc.h"
#include "list.h"
#include "line.h"
#include "scandir.h"
#include "file.h"
#include "section.h"
#include "compitem.h"
#include "complist.h"
#include "view.h"

/*
 * data structures
 */

/* in expand mode, we keep an array of one of these per screen line. */
typedef struct viewline {
        LINE line;              /* handle to LINE for this row */
        SECTION section;        /* handle to section containing this line */
        int nr_left;            /* line nr in left file */
        int nr_right;           /* line nr in right file */
} VIEWLINE, FAR * PVIEWLINE;


/*
 * The users VIEW handle is in fact a pointer to this structure
 */
struct view {

        HWND     hwnd;          /* the table window to send notifies to */

        COMPLIST cl;            /* the complist that we own */

   BOOL          bExpand;       /* true if we are in expand mode */

        COMPITEM ciSelect;      /* selected compitem (in expand mode) */

        int      rows;          /* number of rows in this view */

        char     nrtext[12];    /* we use this in view_gettext for the line
                                 * number column. overwritten on each call
                                 */
        int      maxtag, maxrest;/* column widths in characters for cols 1, 2 */

        /* if we are in outline mode, we map the row number to one entry
         * in this array of COMPITEM handles. this pointer will
         * be NULL in expand mode
         */
        COMPITEM FAR * pItems;

        /* in expand mode we use this array of line and section handles */
        PVIEWLINE pLines;
};


CRITICAL_SECTION CSView;
static BOOL bDoneInit = FALSE;

#define ViewEnter()     EnterCriticalSection(&CSView);
#define ViewLeave()     LeaveCriticalSection(&CSView);

void view_outline_opt(VIEW view, BOOL bRedraw);
void view_freemappings(VIEW view);
int view_findrow(VIEW view, int number, BOOL bRight);
BOOL view_expand_item(VIEW view, COMPITEM ci);


/***************************************************************************
 * Function: view_new
 *
 * Purpose:
 *
 * Create a new view. At this point, we are told the table window handle,
 * and nothing else.
 *
 */
VIEW
view_new(HWND hwndTable)
{
        VIEW view;

        if (!bDoneInit) {
                InitializeCriticalSection(&CSView);
                bDoneInit = TRUE;
        }

        /* alloc the view from the heap */
        view = (VIEW) gmem_get(hHeap, sizeof(struct view));

        /* set the default fields */
        view->hwnd = hwndTable;
        view->cl = NULL;
        view->bExpand = FALSE;
        view->ciSelect = NULL;
        view->rows = 0;
        view->pItems = NULL;
        view->pLines = NULL;

        return(view);
}


/***************************************************************************
 * Function: view_setcomplist
 *
 * Purpose:
 *
 * We have to separate view_new and view_setcomplist because we need
 * to give the view handle to the complist and the complist handle to the
 * view. So do a view_new to create a null view; then complist_new() to
 * which you pass a view handle. The complist will then register itself
 * with the view by calling this function. During the build of the complist,
 * it will also update us by calling view_additem, so that we can refresh
 * the display.
 *
 * Here we should initialise an outline view of the complist.
 *
 * We also talk to the status bar using SetNames to set the names of
 * the two items.
 */
BOOL
view_setcomplist(VIEW view, COMPLIST cl)
{
        LPSTR left, right, both;

        if (view == NULL) {
                return(FALSE);
        }

        /* there can be only one call to this per VIEW */
        if (view->cl != NULL) {
                return (FALSE);
        }

        ViewEnter();

        view->cl = cl;

        /* set names on status bar to root names of left and right trees */
        left = complist_getroot_left(cl);
        right = complist_getroot_right(cl);
        both = gmem_get(hHeap, lstrlen(left) + lstrlen(right) +4);
        wsprintf((LPTSTR)both, "%s : %s", left, right);
        ViewLeave();   
        SetNames(both);
        ViewEnter();   
        gmem_free(hHeap, both, lstrlen(both)+1);
        complist_freeroot_left(cl, left);
        complist_freeroot_right(cl, right);

        ViewLeave();

        view_outline(view);
}


/***************************************************************************
 * Function: view_getcomplist
 *
 * Purpose:
 *
 * Return a handle to the complist owned by this view
 */
COMPLIST
view_getcomplist(VIEW view)
{
        if (view == NULL) {
                return(NULL);
        }

        return(view->cl);
}


/***************************************************************************
 * Function: view_close
 *
 * Purpose:
 *
 * Close a view. Notify the table window that this view should be
 * closed. When the table window has finished with it, it will send
 * a TQ_CLOSE notify that should result in view_delete being called
 * and the memory being freed.
 */
void
view_close(VIEW view)
{
        if (view == NULL) {
                return;
        }

        SendMessage(view->hwnd, TM_NEWID, 0, 0);
}


/***************************************************************************
 * Function: view_delete
 *
 * Purpose:
 *
 * Delete a view and all associated data.
 *
 * This function should only be called in response to the table window
 * sending a TQ_CLOSE message. To close the view, call view_close and
 * wait for the TQ_CLOSE before calling this.
 *
 * We delete the associated COMPLIST and all its associated structures.
 */
void
view_delete(VIEW view)
{
        if (view == NULL) {
                return;
        }

        /* we have two arrays that are used for the mapping - an array
         * of compitem handles in outline mode, and an array of
         * VIEWLINE structures in expand mode
         */

        view_freemappings(view);

        complist_delete(view->cl);

        gmem_free(hHeap, (LPSTR) view, sizeof(struct view));
}


/***************************************************************************
 * Function: view_outline
 *
 * Purpose:
 *
 * Build an outline mode mapping where one row represents one COMPITEM in
 * the list. Check the global option flag outline_include to see which items
 * we should include.
 *
 * If we were in expand mode, then set as the selection the row in outline mode
 * that we were expanding. Also remember to free up the expand mode mapping
 * array
 *
 * Once we have built the new mapping, notify the table window to
 * redraw itself.
 */
void
view_outline(VIEW view)
{
        if (view == NULL) {
                return;
        }

        /* all work done by view_outline_opt - this function
         * gives us the option of not updating the display
         */
        view_outline_opt(view, TRUE);
}



/***************************************************************************
 * Function: view_expand
 *
 * Purpose:
 *
 * Switch to expand mode, expanding the given row into a view
 * of the differences in that file.
 *
 * Map the given row nr into a compitem handle, and then
 * call the internal function with that.
 */
BOOL    
view_expand(VIEW view, long row)
{
        COMPITEM ci;
        BOOL bRet;

        ViewEnter();

        if ((view == NULL) || (view->bExpand)) {
                /* no view, or already expanded */
                ViewLeave();
                return(FALSE);
        }

        if (row >= view->rows) {
                /* no such row */
                ViewLeave();
                return FALSE;
        }

        /* remember the compitem we are expanding */
        ci = view->pItems[row];

        bRet = view_expand_item(view, ci);
        // view_expand_item does the...
        // ViewLeave();
        return(bRet);
}


/***************************************************************************
 * Function: view_gettext
 *
 * Purpose:
 *
 * Return the text associated with a given column of a given row.
 * Return a pointer that does not need to be freed after use - ie
 * a pointer into our data somewhere, not a copy
 */
LPSTR
view_gettext(VIEW view, long row, int col)
{
        int line;
        int state;
        LPSTR pstr;


        if (view == NULL) {
                return (NULL);
        }

        ViewEnter();

        if (row >= view->rows) {
                ViewLeave();
                return(NULL);
        }

        if (view->bExpand) {
                /* we are in expand mode */
                
                state = section_getstate(view->pLines[row].section);

                switch(col) {
                case 0:
                        /* row nr */
                                                
                        /* line numbers can be from either original file
                         * this is a menu-selectable option
                         */
                        switch(line_numbers) {
                        case IDM_NONRS:
                                pstr = NULL;
                                break;

                        case IDM_LNRS:
                                line = view->pLines[row].nr_left;
                                if (state == STATE_MOVEDRIGHT) {
                                        line = -line;
                                }
                                break;

                        case IDM_RNRS:
                                line = view->pLines[row].nr_right;
                                if (state == STATE_MOVEDLEFT) {
                                        line = -line;
                                }
                                break;
                        }
                        if (line == 0) {
                                ViewLeave();
                                return(NULL);
                        }

                        if (line < 0) {
                                /* lines that are moved appear twice.
                                 * show the correct-sequence line nr
                                 * for the out-of-seq. copy in brackets.
                                 */
                                wsprintf((LPTSTR)view->nrtext, "(%d)", abs(line));
                        } else  {
                                wsprintf((LPTSTR)view->nrtext, "%d", line);
                        }
                        pstr = view->nrtext;
                        break;

                case 1:
                        /* tag text - represents the state of the line */


                        switch(state) {
                        case STATE_SAME:
                                pstr = "    ";
                                break;

                        case STATE_LEFTONLY:
                                pstr = " <! ";
                                break;

                        case STATE_RIGHTONLY:
                                pstr = " !> ";
                                break;

                        case STATE_MOVEDLEFT:
                                pstr = " <- ";
                                break;

                        case STATE_MOVEDRIGHT:
                                pstr = " -> ";
                                break;
                        }
                        break;

                case 2:
                        /* main text - line */
                        pstr = line_gettext(view->pLines[row].line);
                        break;
                }
        } else {
                /* outline mode */
                switch(col) {
                case 0:
                        /* row number - just the line number */
                        wsprintf((LPTSTR)view->nrtext, "%d", row+1);
                        pstr = view->nrtext;
                        break;

                case 1:
                        /* tag */
                        pstr = compitem_gettext_tag(view->pItems[row]);
                        break;

                case 2:
                        /* result text */
                        pstr = compitem_gettext_result(view->pItems[row]);
                        break;
                }
        }
        ViewLeave();
        return(pstr);
}

/***************************************************************************
 * Function: view_getlinenr_left
 *
 * Purpose:
 *
 * Return the line number that this row had in the original left
 * file. 0 if not in expand mode. 0 if this row was not in the left file.
 * -(linenr) if this row is a MOVED line, and this is the right file
 * copy
 */
int
view_getlinenr_left(VIEW view, long row)
{
        int state, line;

        if ((view == NULL) || (row >= view->rows) || !view->bExpand) {
                return 0;
        }

        ViewEnter();
        state = section_getstate(view->pLines[row].section);
        line = view->pLines[row].nr_left;
        if (state == STATE_MOVEDRIGHT) {
                line = -line;
        }
        ViewLeave();

        return(line);
}

/***************************************************************************
 * Function: view_getlinenr_right
 *
 * Purpose:
 *
 * Return the line number that this row had in the original right
 * file. 0 if not in expand mode. 0 if this row was not in the right file.
 * -(linenr) if this row is a MOVED line, and this is the left file
 * copy
 */
int
view_getlinenr_right(VIEW view, long row)
{
        int state, line;

        if ((view == NULL) || (row > view->rows) || !view->bExpand) {
                return 0;
        }

        ViewEnter();

        state = section_getstate(view->pLines[row].section);
        line = view->pLines[row].nr_right;
        if (state == STATE_MOVEDLEFT) {
                line = -line;
        }
        ViewLeave();

        return(line);
}


/***************************************************************************
 * Function: view_getwidth
 *
 * Purpose:
 *
 * Find the maximum width in characters for the given column 
 */
int
view_getwidth(VIEW view, int col)
{
        if (view == NULL) {
                return(0);
        }

        switch(col) {
        case 0:
                /* line nr column - always 5 characters wide */
                return(5);

        case 1:
                /* this is a proportional font field, so add on a margin
                 * for error
                 */
                return(view->maxtag + (view->maxtag / 20));
        case 2:
                /* this now includes the tab expansion allowance */
                return(view->maxrest);
        default:
                return(0);
        }
}

/***************************************************************************
 * Function: view_getrowcount
 *
 * Purpose:
 *
 * How many rows are there in this view ? 
 */
long
view_getrowcount(VIEW view)
{
        if (view == NULL) {
                return(0);
        }

        return(view->rows);
}

/***************************************************************************
 * Function: view_getstate
 *
 * Purpose:
 *
 * Return the state for the current row. This is used
 * to select the text colour for the row
 *
 * States for sections are obtained from section_getstate (and apply, and
 * to all lines in that section. States for compitems are obtained
 * from compitem_getstate.
 */
int
view_getstate(VIEW view, long row)
{
        int state;

        if (view == NULL) {
                return(0);
        }

        ViewEnter();
        if (row >= view->rows) {
                state = 0;
        } else if (view->bExpand) {
                /* its a line state that's needed */
                state = section_getstate(view->pLines[row].section);
        } else {

                /* its a compitem state */
                state = compitem_getstate(view->pItems[row]);
        }
        ViewLeave();
        return(state);
}

/***************************************************************************
 * Function: view_gethandle
 *
 * Purpose:
 *
 * Return a handle to the current compitem. In expand mode,
 * returns the handle to the compitem we are expanding. In outline
 * mode, returns the handle to the compitem for the given row, if valid,
 * or NULL otherwise. row is only used if not in expand mode.
 */
COMPITEM
view_getitem(VIEW view, long row)
{
        COMPITEM ci;

        if (view == NULL) {
                return(NULL);
        }

        ViewEnter();

        if (!view->bExpand) {
                if ((row >= 0) && (row < view->rows)) {
                        ci = view->pItems[row];
                } else {
                        ci = NULL;
                }
        } else {
                ci = view->ciSelect;
        }

        ViewLeave();
        return(ci);
}

/***************************************************************************
 * Function: view_isexpanded
 *
 * Purpose:
 *
 * Return TRUE if the current mapping is expanded mode
 */
BOOL
view_isexpanded(VIEW view)      
{
        if (view == NULL) {
                return(FALSE);
        }
        return(view->bExpand);
}


/***************************************************************************
 * Function: view_getcurrenttag
 *
 * Purpose:
 *
 * Return a text string describing the view. This is NULL in outline mode,
 * or the tag text for the current compitem in expanded mode
 */
LPSTR
view_getcurrenttag(VIEW view)
{
        LPSTR str;

        if ((view == NULL) || (!view->bExpand)) {
                return(NULL);
        } else {
                ViewEnter();

                str = compitem_gettext_tag(view->ciSelect);

                ViewLeave();
                return(str);

        }
}


/***************************************************************************
 * Function: view_newitem
 *
 * Purpose:
 *
 * Notify that CompItems have been added to the complist.
 *
 * Rebuild the view (if in outline mode), and refresh the table. Use
 * the table message TM_APPEND if possible (if column widths have not
 * change). If we have to do TM_NEWLAYOUT, then ensure we scroll
 * back to the right row afterwards.
 *
 * This causes a Poll() to take place. We return TRUE if an abort is
 * pending - in this case, the caller should abandon the scan loop.
 *
 * Enter the critical section for this function since this can be
 * called from the worker thread while the UI thread is using the
 * view that we are about to change.
 *
 * EXCEPT THAT WE DON'T DARE.  We cannot ever call SendMessage from the
 * worker thread within CSView.  If there is conflict, it will hang.
 */
BOOL
view_newitem(VIEW view)
{
        int maxtag, maxrest;
        long rownr;

        if ((view == NULL) || (view->bExpand)) {
                /* not in outline mode - nothing to do */
                return(Poll());
        }

        /* save some state about the present mapping */
        maxtag = view->maxtag;
        maxrest = view->maxrest;

        /* re-do the outline mapping, but don't tell the table
         * class.
         */
        view_outline_opt(view, FALSE);

        /* have the column widths changed ? */
        if ((maxtag < view->maxtag) || (maxrest < view->maxrest)) {
                /* yes - need complete redraw */

                /* find the row at the top of the window */
                rownr = SendMessage(view->hwnd, TM_TOPROW, FALSE, 0);

                /* switch to new mapping */
                SendMessage(view->hwnd, TM_NEWLAYOUT, 0, (DWORD) view);

                /* return to old row if possible - we know
                 * that row is still there since we have only added
                 * rows, and not changed any of the existing mapping
                 *
                 * Alas this is no longer true.  However the table class
                 * will defend itself against calls for a bogus top row.
                 */
                if (rownr >= 0) {
                        SendMessage(view->hwnd, TM_TOPROW, TRUE, rownr);
                }
        } else {
                /* no - we can just append */

                /*
                 * The mapping may have
                 * changed since we released the critsec. however we are still
                 * safe. The table will not allow us to reduce the number of
                 * rows, so the worst that can happen is that the table will
                 * think there are too many rows, and the table message handler
                 * will handle this correctly (return null for the text).
                 * The only visible effect is therefore that the scrollbar
                 * position is wrong.
                 */

                SendMessage(view->hwnd, TM_APPEND, view->rows, (DWORD) view);
        }


        /* Poll to keep the UI updated on NT. Returns true if abort pending.
         */
        return(Poll());
}

/***************************************************************************
 * Function: view_changeviewoptions
 *
 * Purpose:
 *
 * The view mapping options (eg outline_include, expand_mode) have changed -
 * re-do the mapping and then scroll back to the same position in the window
 * if possible.
 */
void
view_changeviewoptions(VIEW view)
{
        long row;
        int state, number;
        BOOL bRight;

        if (view == NULL) {
                return;
        }

        /* find what row we are currently on. Do this BEFORE we enter CSView */
        row = SendMessage(view->hwnd, TM_TOPROW, FALSE, 0);

        ViewEnter();

        if (!view->bExpand) {

                /* outline mode. maintaining current position is
                 * unimportant
                 */
                view_outline(view);
                ViewLeave();
                return;
        }

        /* expanded mode */
        

        /* save the line number on one side (and remember which side) */
        if (row >= view->rows) {
                number = -1;
        } else {
                state = section_getstate(view->pLines[row].section);
                if ((state == STATE_MOVEDRIGHT) ||
                    (state == STATE_RIGHTONLY)) {
                            bRight = TRUE;
                            number = view->pLines[row].nr_right;
                } else {
                        bRight = FALSE;
                        number = view->pLines[row].nr_left;
                }
        }

        /* make the new mapping */
        view_expand_item(view, view->ciSelect);

        /* find the nearest row in the new view */
        if (number >= 0) {

                ViewEnter();
                row = view_findrow(view, number, bRight);
                ViewLeave();
        
                /* scroll this row to top of window */
                if (row >= 0) {

                        SendMessage(view->hwnd, TM_TOPROW, TRUE, row);
                        return;
                }
        }
}

/***************************************************************************
 * Function: view_changediffoptions
 *
 * Purpose:
 *
 * The compare options have changed - re-do the compare completely
 * and make the new mapping. Retain current position in the file.
 */
void
view_changediffoptions(VIEW view)
{
        int state, number;
        long row;
        BOOL bRight;
        LIST li;
        COMPITEM ci;

        if (view == NULL) {
                return;
        }

        /*
         * get current row before entering critsec.
         */
        row = SendMessage(view->hwnd, TM_TOPROW, FALSE, 0);

        ViewEnter();

        /* find the current line number so we can go back to it
         * (only if we are in expanded mode
         */
        if (view->bExpand) {

                state = section_getstate(view->pLines[row].section);
                if ((state == STATE_MOVEDRIGHT) ||
                    (state == STATE_RIGHTONLY)) {
                            bRight = TRUE;
                            number = view->pLines[row].nr_right;
                } else {
                        bRight = FALSE;
                        number = view->pLines[row].nr_left;
                }
        }

        /* To force a recompare using the new options, we must
         * tell each compitem to discard its current compare result.
         * We need to traverse the list of compitems calling this
         * for each compare.
         */
        li = complist_getitems(view->cl);

        for (ci = (COMPITEM) List_First(li); ci != NULL; ci = (COMPITEM) List_Next(ci)) {
                compitem_discardsections(ci);
        }

        /* if we are in outline mode, we have nothing more to do */
        if (!view->bExpand) {
                ViewLeave();
                return;
        }

        view_expand_item(view, view->ciSelect);

        /* find the nearest row in the new view */
        ViewEnter();
        row = view_findrow(view, number, bRight);
        ViewLeave();

        /* scroll this row to top of window */
        if (row >= 0) {
                SendMessage(view->hwnd, TM_TOPROW, TRUE, row);
        }
}


/***************************************************************************
 * Function: view_findchange
 *
 * Purpose:
 *
 * Find the next changed - ie non-same - row in a given direction.
 * For outline mode we find the next STATE_DIFFER. For expand mode, we
 * find the next section
 */
long
view_findchange(VIEW view, long startrow, BOOL bForward)
{
        long i;

        if (view == NULL) {
                return(0);
        }

        ViewEnter();

        if (bForward) {

                if (startrow >= view->rows) {
                        ViewLeave();
                        return(-1);
                }

                if (!view->bExpand) {

                        /* look for next compitem with an expandable state*/
                        for (i = startrow; i < view->rows; i++) {
                                if (compitem_getstate(view->pItems[i]) == STATE_DIFFER) {
                                        ViewLeave();
                                        return(i);
                                }
                        }
                        /* none found */
                        ViewLeave();
                        return(-1);
                } else {
                        /*
                         * find the next line that matches, then go on to the
                         * next line that does not match
                         *
                         */
                        for (i= startrow; i < view->rows; i++) {
                                if (section_getstate(view->pLines[i].section)
                                        == STATE_SAME) {
                                                break;
                                }
                        }
                        for ( ; i < view->rows; i++) {
                                if (section_getstate(view->pLines[i].section)
                                        != STATE_SAME) {
                                                ViewLeave();
                                                return(i);
                                }
                        }

                        ViewLeave();

                        return(-1);
                }
        } else {
                /* same search backwards */
                if (startrow <= 0) {
                        ViewLeave();
                        return(-1);
                }
                if (view->bExpand) {
                        /* search backwards for first row that is not
                         * changed (has state SAME). then carry on for
                         * the next changed row.
                         */
                        for (i = startrow; i >= 0; i--) {
                                if (section_getstate(view->pLines[i].section)
                                        == STATE_SAME) {
                                                break;
                                }
                        }
                        for ( ; i >= 0; i--) {
                                if (section_getstate(view->pLines[i].section)
                                        != STATE_SAME) {
                                                ViewLeave();
                                                return(i);
                                }
                        }
                        ViewLeave();
                        return(-1);
                } else {
                        for (i = startrow; i >= 0; i--) {
                                if(compitem_getstate(view->pItems[i]) == STATE_DIFFER) {
                                        ViewLeave();
                                        return(i);
                                }
                        }
                        ViewLeave();
                        return(-1);
                }
        }
}



/***************************************************************************
 * Function: view_findrow
 *
 * Purpose:
 *
 * Find the new row number for the line numbered 'number'
 * or the nearest line if possible. If bRight is true, number is
 * a right file number; otherwise it is a left file number.
 *
 * We must be in expand mode
 */
int     
view_findrow(VIEW view, int number, BOOL bRight)
{
        int i;

        if (!view->bExpand) {   
                return(0);
        }

        for (i = 0; i < view->rows; i++) {

                if (bRight) {
                        if (view->pLines[i].nr_right == number) {

                                /* found the exact number */
                                return(i);

                        } else if (view->pLines[i].nr_right > number) {

                                /* passed our line -stop here */
                                return(i);
                        }
                } else {
                        if (view->pLines[i].nr_left == number) {

                                /* found the exact number */
                                return(i);

                        } else if (view->pLines[i].nr_left > number) {

                                /* passed our line -stop here */
                                return(i);
                        }
                }
        }
        return(-1);
}

/***************************************************************************
 * Function: view_freemappings
 *
 * Purpose:
 *
 * Free memory associated with the expand mode or outline mode mappings
 * called whenever we rebuild the mapping, and on deletion
 */
void
view_freemappings(VIEW view)
{

        if (view->pLines) {
                gmem_free(hHeap, (LPSTR) view->pLines,
                        view->rows * sizeof(VIEWLINE));
                view->pLines = NULL;
        } else if (view->pItems) {

                /* previous outline mapping array is still there - free it
                 * before we build a new one
                 */

                gmem_free(hHeap, (LPSTR) view->pItems,
                        view->rows * sizeof(COMPLIST));
                view->pItems = NULL;
        }
}

/***************************************************************************
 * Function: view_outline_opt
 *
 * Purpose:
 *
 * Build a view outline to map one row to a COMPITEM handle by traversing
 * the list of COMPITEMs obtained from our complist.
 * Optionally tell the table class to redraw (if bRedraw), and if so,
 * scroll the new table to select the row that represents the
 * file we were expanding, if possible
 */
void
view_outline_opt(VIEW view, BOOL bRedraw)
{
        int prev_row = -1;      /* the row nr of the previously-expanded row*/
        int i;                  /* nr of includable items */
        LIST li;
        COMPITEM ci;
        int state;
        TableSelection select;

        /*
         * check that view_setcomplist has already been called. if not,
         * nothing to do
         */
        if (view->cl == NULL) {
                return;
        }

        ViewEnter();

        /* clear the mode flag and free up memory associated with expand mode */
        view->bExpand = FALSE;
        view_freemappings(view);

        /* traverse the list of compitems counting up the number of
         * includable items
         */
        li = complist_getitems(view->cl);

        ci = (COMPITEM) List_First(li);
        for (i = 0; ci != NULL; ci = (COMPITEM) List_Next(ci)) {

                state = compitem_getstate(ci);

                if (((outline_include & INCLUDE_SAME) && (state == STATE_SAME)) ||
                    ((outline_include & INCLUDE_DIFFER) && (state == STATE_DIFFER)) ||
                    ((outline_include & INCLUDE_LEFTONLY) && (state == STATE_FILELEFTONLY)) ||
                    ((outline_include & INCLUDE_RIGHTONLY) && (state == STATE_FILERIGHTONLY))) {
                        i++;
                }
        }


        /* allocate an array big enough for all of these */
        view->pItems = (COMPITEM FAR *) gmem_get(hHeap, i * sizeof(COMPITEM));
        view->rows = i;

        /* keep track of the column widths */
        view->maxtag = 0;
        view->maxrest = 0;

        /* loop through again filling the array, and at the same time looking
         * out for the handle of the previously expanded item
         */
        ci = (COMPITEM) List_First(li);
        for (i = 0; ci != NULL; ci = (COMPITEM) List_Next(ci)) {

                state = compitem_getstate(ci);

                if (((outline_include & INCLUDE_SAME) && (state == STATE_SAME)) ||
                    ((outline_include & INCLUDE_DIFFER) && (state == STATE_DIFFER)) ||
                    ((outline_include & INCLUDE_LEFTONLY) && (state == STATE_FILELEFTONLY)) ||
                    ((outline_include & INCLUDE_RIGHTONLY) && (state == STATE_FILERIGHTONLY))) {

                        view->pItems[i] = ci;

                        if (ci == view->ciSelect) {
                                prev_row = i;
                        }

                        /* check the column widths in characters */
                        view->maxtag = max(view->maxtag,
                                           lstrlen(compitem_gettext_tag(ci)));
                        view->maxrest = max(view->maxrest,
                                            lstrlen(compitem_gettext_result(ci)));


                        i++;

                }
        }
        ViewLeave();

        /* inform table of new layout of table - force refresh */       
        if (bRedraw) {
                SendMessage(view->hwnd, TM_NEWLAYOUT, 0, (DWORD) view);
        
                /* scroll to and highlight the row that represents the file
                 * we were previously expanding
                 */
                if (prev_row != -1) {
                        select.startrow = prev_row;
                        select.startcell = 0;
                        select.nrows = 1;
                        select.ncells = 1;
                        SendMessage(view->hwnd, TM_SELECT, 0,
                                (DWORD) (LPSTR) &select);
                }
        }
}


/***************************************************************************
 * Function: view_expand_item
 *
 * Purpose:
 *
 * Expand a view - given the handle to the compitem to expand.
 *
 * Called from view_expand, and also to re-do an expanded view
 * after options change in view_changediffoptions and _changeviewoptions
 *
 * We get the composite section list from the compitem,
 * and pick out all the sections that are includable (according
 * to the global option expand_mode: we include all sections, or
 * just those in one side left or right). Once we know the count of rows,
 * allocate the mapping array: in each element of the array we keep
 * a handle to the section for that row (to get the state and hence the
 * tag text), and a handle to the line within that section (for the line text).
 *
 * We no longer insist on only expanding text files that differ - if the
 * compitem can give us a composite section list, we will map it.
 *
 * We need to be able to give a line number for a line, in either of
 * the original files according to which option is in force. Each section
 * can give us its base line number (number of first line in section) in
 * each of the two files or 0 if not present, and we track these here.
 *
 * MUST BE INSIDE CSView BEFORE CALLING HERE.
 */
BOOL
view_expand_item(VIEW view, COMPITEM ci)
{
        LIST li;
        SECTION sh;
        LINE line1, line2;
        int i, base_left, base_right, state;

        /* remember the compitem we are expanding */
        view->ciSelect = ci;

        /* get the composite section list */
        li = compitem_getcomposite(view->ciSelect);
        if (li == NULL) {
                ViewLeave();
                return FALSE;
        }

        /* switch modes and free the current mapping
         *
         * NOTE: must do this AFTER the compitem_getcomposite,
         * since that can fail: if it fails it could put up a
         * message box, and that could cause a queued paint message
         * to be processed, which would cause us to use these mappings
         * and gpfault if they had been cleared first.
         */
        view->bExpand = TRUE;
        view_freemappings(view);


        /* loop through totalling the lines in sections
         * that we should include
         */
        view->rows = 0;
        for (sh = (SECTION) List_First(li); sh != NULL;
            sh = (SECTION) List_Next(sh)) {
                
                state = section_getstate(sh);
                
                if (expand_mode == IDM_RONLY) {
                        if ((state == STATE_LEFTONLY) ||
                            (state == STATE_MOVEDLEFT)) {
                                    continue;
                        }
                } else if (expand_mode == IDM_LONLY) {
                        if ((state == STATE_RIGHTONLY) ||
                            (state == STATE_MOVEDRIGHT)) {
                                    continue;
                        }
                }

                /* include all lines in this section */
                view->rows += section_getlinecount(sh);
        }
        
        /* allocate the memory for the mapping array */
        view->pLines = (PVIEWLINE) gmem_get(hHeap, view->rows * sizeof(VIEWLINE));
        
        /* loop through the sections again filling in the mapping array */
        i = 0;
        view->maxtag = 5;
        view->maxrest = 0;
        for (sh = (SECTION) List_First(li); sh != NULL;
            sh = (SECTION) List_Next(sh)) {
                
                state = section_getstate(sh);
                
                if (expand_mode == IDM_RONLY) {
                        if ((state == STATE_LEFTONLY) ||
                            (state == STATE_MOVEDLEFT)) {
                                    continue;
                        }
                } else if (expand_mode == IDM_LONLY) {
                        if ((state == STATE_RIGHTONLY) ||
                            (state == STATE_MOVEDRIGHT)) {
                                    continue;
                        }
                }

                /* find the base line number in each file */
                base_left = section_getleftbasenr(sh);
                base_right = section_getrightbasenr(sh);

                /* add each line in section to the view. section_getfirst()
                 * returns us to a handle that is in a list. We can
                 * call List_Next and will eventually get to the
                 * line returned by section_getlast(). Sections always have
                 * at least one line
                 */
                line1 = section_getfirstline(sh);
                line2 = section_getlastline(sh);

                for (; line1 != NULL; line1 = (LINE) List_Next(line1)) {

                        view->pLines[i].line = line1;
                        view->pLines[i].section = sh;

                        /* calculate the line number for this line by
                         * incrementing the base nr for this section
                         */
                
                        view->pLines[i].nr_left = base_left;
                        if (base_left != 0) {
                                base_left++;
                        }

                        view->pLines[i].nr_right = base_right;
                        if (base_right != 0) {
                                base_right++;
                        }

                        /* increment index into view */
                        i++;

                        /* check the column widths */
                        view->maxrest = max(view->maxrest,
                                            (line_gettabbedlength(line1, 8)));

                        /* end of section ? */
                        if (line1 == line2) {
                                break;
                        }
                }
        }

        /* We must NOT hold a critical section here as SendMessage may hang */
        ViewLeave();

        /*inform table window of revised mapping */
        SendMessage(view->hwnd, TM_NEWLAYOUT, 0, (DWORD) view);

        return(TRUE);
}

