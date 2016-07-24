

/****************************** Module Header *******************************
* Module Name: TSCROLL.C
*
* Scrolling and selection routines.
*
* Functions:
*
* gtab_msg_vscroll()
* gtab_msg_hscroll()
* gtab_dovscroll()
* gtab_dohscroll()
* gtab_linetorow()
* gtab_rowtoline()
* gtab_select()
* gtab_ytoline()
* gtab_xtocol()
* gtab_isborder()
* gtab_enter()
* gtab_trackcol()
* gtab_press()
* gtab_release()
* gtab_move()
* gtab_dblclick()
* gtab_showsel()
* gtab_showsel_middle()
* gtab_changesel()
* gtab_selhome()
* gtab_key()
*
* Comments:
*
* This implementation currently only supports TM_SINGLE, not TM_MANY
* modes of selection.
*
****************************************************************************/

#include <windows.h>
#include <commdlg.h>

#include "gutils.h"
#include "table.h"
#include "tpriv.h"

/***************************************************************************
 * Function: gtab_msg_vscroll
 *
 * Purpose:
 *
 * Handle a vscroll message 
 */
void
gtab_msg_vscroll(HWND hwnd, lpTable ptab, int opcode, int pos)
{
        long change;

        switch(opcode) {
        case SB_THUMBPOSITION:
                change = (pos * ptab->scrollscale) - ptab->toprow;
                break;

        case SB_LINEUP:
                change = -1;
                break;

        case SB_LINEDOWN:
                change = 1;
                break;

        case SB_PAGEUP:
                change = - (ptab->nlines - 3);
                break;

        case SB_PAGEDOWN:
                change = (ptab->nlines - 3);
                break;

        default:
                return;
        }
        gtab_dovscroll(hwnd, ptab, change);
}

/***************************************************************************
 * Function: gtab_msg_hscroll
 *
 * Purpose:
 *
 * Handle a hscroll message 
 */
void
gtab_msg_hscroll(HWND hwnd, lpTable ptab, int opcode, int pos)
{
        int change;

        switch(opcode) {
        case SB_THUMBPOSITION:
                change = pos - ptab->scroll_dx;
                break;

        case SB_LINEUP:
                change = -(ptab->avewidth);
                break;

        case SB_LINEDOWN:
                change = ptab->avewidth;
                break;

        case SB_PAGEUP:
                change = - (ptab->winwidth * 2 / 3);
                break;

        case SB_PAGEDOWN:
                change = (ptab->winwidth * 2 / 3);
                break;

        default:
                return;
        }
        gtab_dohscroll(hwnd, ptab, change);
}



/***************************************************************************
 * Function: gtab_dovscroll
 *
 * Purpose:
 *
 * Set new vertical scroll pos,
 * adjust linedata array
 * set line win-relative start posns & clip top/bottom posns
 * revise display.
 */
void
gtab_dovscroll(HWND hwnd, lpTable ptab, long change)
{
        int cury, i;
        long ncopy;
        lpCellPos cp;
        LineData ldtemp;
        RECT rc, rcpaint;
        long range;
        long newtop;
        int newpos;


        range = ptab->hdr.nrows - (ptab->nlines - 1);
        newtop = ptab->toprow + change;
        if (range < 0) {
                range = 0;
        }
        if (newtop > range) {
                change = range - ptab->toprow;
        } else if (newtop < 0) {
                change = -(ptab->toprow);
        }
        ptab->toprow += change;

        newpos = (int) (newtop / ptab->scrollscale);
        SetScrollPos(hwnd, SB_VERT, newpos, TRUE);

        if (ptab->hdr.sendscroll) {
                gtab_sendtq(hwnd, TQ_SCROLL, ptab->toprow);
        }

        /* adjust data ptrs rather than invalidate, to retain the
         * data we know is still valid
         */
        if (abs(change) >= ptab->nlines) {
                gtab_invallines(hwnd, ptab, ptab->hdr.fixedrows,
                        ptab->nlines - ptab->hdr.fixedrows);
                InvalidateRect(hwnd, NULL, TRUE);
                change = 0;
        } else if (change < 0) {
                /* copy data down */
                ncopy = (ptab->nlines - ptab->hdr.fixedrows) - abs(change);
                for (i =  ptab->nlines - 1;
                  i >= (ptab->hdr.fixedrows + abs(change)); i--) {
                        ldtemp = ptab->pdata[i - abs(change)];
                        ptab->pdata[i - abs(change)] = ptab->pdata[i];
                        ptab->pdata[i] = ldtemp;
                }
                gtab_invallines(hwnd, ptab,
                        ptab->hdr.fixedrows, (int) abs(change));
        } else if (change > 0) {
                ncopy = (ptab->nlines - ptab->hdr.fixedrows) - change;
                for (i = ptab->hdr.fixedrows;
                  i < (ncopy + ptab->hdr.fixedrows); i++) {
                        ldtemp = ptab->pdata[i + change];
                        ptab->pdata[i + change] = ptab->pdata[i];
                        ptab->pdata[i] = ldtemp;
                }
                gtab_invallines(hwnd, ptab,
                        (int) ncopy + ptab->hdr.fixedrows, (int) change);
        }

        /* scroll window */
        GetClientRect(hwnd, &rc);
        rcpaint = rc;
        if (change > 0) {
                rc.top += (int) (change + ptab->hdr.fixedrows) * ptab->rowheight;
                rcpaint.top = (ptab->hdr.fixedrows * ptab->rowheight);
                rcpaint.top += rc.bottom - rc.top;
        } else if (change < 0) {
                rc.top += (ptab->hdr.fixedrows * ptab->rowheight);
                rc.bottom -= (int) (change * ptab->rowheight);
                rcpaint.bottom -= rc.bottom - rc.top;
        }

        /* loop through each line setting relative posn and clipping */

        /* set up all rows  - the fixed/moveable difference for
         * rows is made at fetch-time during painting, when we remember
         * which absolute row nr to ask for, for a given screen line
         */
        cury = 0;
        for (i = 0; i < ptab->nlines; i++) {
                cp = &ptab->pdata[i].linepos;
                cp->start = cury;
                cp->clipstart = cury;
                cp->clipend = cury + cp->size;
                cury += cp->size;
        }

        /* now move and repaint the window */
        if (change != 0) {
                if (rc.top < rc.bottom) {
                        ScrollWindow(hwnd, 0, (int) -(change * ptab->rowheight),
                                &rc, NULL);

                }

                /* force repaint now, not just post message for later,
                 * since we want to repaint that line before the next
                 * scroll down occurs
                 */
                RedrawWindow(hwnd, &rcpaint, NULL,
                        RDW_ERASE | RDW_INVALIDATE|RDW_UPDATENOW);
        }
}

/***************************************************************************
 * Function: gtab_dohscroll
 *
 * Purpose:
 *
 * Set new horizontal scroll pos,
 * set col win-relative start posns & clip left/right posns
 * revise display.
 */
void
gtab_dohscroll(HWND hwnd, lpTable ptab, long change)
{
        int curx, i;
        int moveable;
        lpCellPos cp;
        int newdx, range;


        /* check that the new scroll pos is still within the valid range */
        range = ptab->rowwidth - ptab->winwidth;
        newdx = ptab->scroll_dx + (int) change;
        if (range < 0) {
                range = 0;
        }
        if (newdx > range) {
                change = range - ptab->scroll_dx;
        } else if (newdx < 0) {
                change = -(ptab->scroll_dx);
        }
        ptab->scroll_dx += (int) change;

        SetScrollPos(hwnd, SB_HORZ, ptab->scroll_dx, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
        
        /* loop through each col setting relative posn and clipping */
        /* clip off 1 pixel left and right (we added 2 on to size for this) */

        /* first set up fixed columns */
        curx = 0;
        for (i = 0; i < ptab->hdr.fixedcols; i++) {
                cp = &ptab->pcellpos[i];
                cp->start = curx + 1;
                cp->clipstart = cp->start;
                cp->clipend = cp->start + cp->size - 2;
                curx += cp->size;
        }

        /* now moveable columns. remember start of moveable cols */
        moveable = curx;
        curx = - ptab->scroll_dx;       /* rel. pos of col */
        for (i = ptab->hdr.fixedcols; i < ptab->hdr.ncols; i++) {
                cp = &ptab->pcellpos[i];
                cp->start = curx + moveable + 1;
                cp->clipstart = max(moveable+1, cp->start);
                cp->clipend = cp->start + cp->size - 2;
                curx += cp->size;
        }
}

/***************************************************************************
 * Function: gtab_linetorow
 *
 * Purpose:
 *
 * Convert screen line nr to table row nr
 */
long
gtab_linetorow(HWND hwnd, lpTable ptab, int line)
{
        if (line < ptab->hdr.fixedrows) {
                return(line);
        }

        return (line + ptab->toprow);
}

/***************************************************************************
 * Function: gtab_rowtoline
 *
 * Purpose:
 *
 * Convert table row nr to screen line nr or -1 if not on screen
 */
int
gtab_rowtoline(HWND hwnd, lpTable ptab, long row)
{
        if (row < ptab->hdr.fixedrows) {
                return( (int) row);
        }

        row -= ptab->toprow;
        if ((row >= ptab->hdr.fixedrows) && (row < ptab->nlines)) {
                return ( (int) row);
        }
        return(-1);
}

/***************************************************************************
 * Function: gtab_select
 *
 * Purpose:
 *
 * Replace old selection with new. Notify owner if bNotify. Change
 * display to reflect new display.
 */
void
gtab_select(
        HWND hwnd,
        lpTable ptab,
        long row,
        long col,
        long nrows,
        long ncells,
        BOOL bNotify)
{
        int line;

        /* if in ROW mode, force col and ncells to reflect the entire row. */
        if (ptab->hdr.selectmode & TM_ROW) {
                col = 0;
                ncells = ptab->hdr.ncols;
        }

        /* clear existing sel if valid and visible */
        if ((ptab->select.nrows > 0) && (ptab->selvisible == TRUE)) {

                /* only clear sel if it is different from the new one */
                if ((ptab->select.startrow != row) ||
                    (ptab->select.startcell != col) ||
                    (ptab->select.nrows != nrows) ||
                    (ptab->select.ncells != ncells)) {
                        line = gtab_rowtoline(hwnd, ptab,
                                ptab->select.startrow);
                        if (line >= 0) {
                                gtab_invertsel(hwnd, ptab, NULL);
                        }
                        ptab->selvisible = FALSE;
                }
        }

        /* set select fields and send TQ_SELECT */
        if (row < ptab->hdr.nrows) {
                ptab->select.startrow = row;
                ptab->select.startcell = col;
                ptab->select.nrows = nrows;
                ptab->select.ncells = ncells;
        } else {
                ptab->select.nrows = 0;
                ptab->select.startrow = 0;
                ptab->select.startcell = 0;
                ptab->select.ncells = 0;
        }

        if (bNotify) {
                gtab_sendtq(hwnd, TQ_SELECT, (long) (LPSTR) &ptab->select);
        }

        /* paint in selection */
        if (nrows > 0) {
                if (!ptab->selvisible) {
                        gtab_invertsel(hwnd, ptab, NULL);
                        ptab->selvisible = TRUE;
                }
        } else {
                if (ptab->selvisible) {
                        gtab_invertsel(hwnd, ptab, NULL);
                        ptab->selvisible = FALSE;
                }
                ptab->selvisible = FALSE;
        }
}

/***************************************************************************
 * Function: gtab_ytoline
 *
 * Purpose:
 *
 * Convert window y co-ord to a line nr
 */
int
gtab_ytoline(HWND hwnd, lpTable ptab, int y)
{
        return(y / ptab->rowheight);
}

/***************************************************************************
 * Function: gtab_xtocol
 *
 * Purpose:
 *
 * Convert window x co-ord to a cell nr
 */
int
gtab_xtocol(HWND hwnd, lpTable ptab, int x)
{
        int i;
        lpCellPos ppos;

        for (i = 0; i < ptab->hdr.ncols; i++) {
                ppos = &ptab->pcellpos[i];
                if (ppos->clipstart < ppos->clipend) {
                        if ( (x >= ppos->clipstart) && (x < ppos->clipend)) {
                                return(i);
                        }
                }
        }
        return(-1);
}


/***************************************************************************
 * Function: gtab_isborder
 *
 * Purpose:
 *
 * Check if x co-ord is 'near' (+- 2 pixels) the right border of given cell
 */
BOOL
gtab_isborder(HWND hwnd, lpTable ptab, int x, int col)
{
        
        if (abs(ptab->pcellpos[col].clipend - x) < 2) {
                return(TRUE);
        } else {
                return(FALSE);
        }
}


/***************************************************************************
 * Function: gtab_enter
 *
 * Purpose:
 *
 * Set selection and send 'TQ_ENTER' event to owner
 */
void
gtab_enter(HWND hwnd, lpTable ptab, long row, long col, long nrows,
        long ncells)
{
        int line;

        /* clear existing sel if valid and visible */
        if ((ptab->select.nrows > 0) && (ptab->selvisible == TRUE)) {

                /* only clear sel if it is different from the new one */
                if ((ptab->select.startrow != row) ||
                    (ptab->select.startcell != col) ||
                    (ptab->select.nrows != nrows) ||
                    (ptab->select.ncells != ncells)) {
                        line = gtab_rowtoline(hwnd, ptab,
                                ptab->select.startrow);
                        if (line >= 0) {
                                gtab_invertsel(hwnd, ptab, NULL);
                        }
                        ptab->selvisible = FALSE;
                }
        }

        /* set select fields and send TQ_SELECT */
        if (row < ptab->hdr.nrows) {
                ptab->select.startrow = row;
                ptab->select.startcell = col;
                ptab->select.nrows = nrows;
                ptab->select.ncells = ncells;
        } else {
                ptab->select.nrows = 0;
                ptab->select.startrow = 0;
                ptab->select.startcell = 0;
                ptab->select.ncells = 0;
        }

        /* paint in selection */
        if (nrows > 0) {
                if (!ptab->selvisible) {
                        gtab_invertsel(hwnd, ptab, NULL);
                        ptab->selvisible = TRUE;
                }
                /* do this at end because it could cause a layout-change */
                gtab_sendtq(hwnd, TQ_ENTER, (long) (LPSTR) &ptab->select);
        } else {
                if (ptab->selvisible) {
                        gtab_invertsel(hwnd, ptab, NULL);
                }
                ptab->selvisible = FALSE;
        }
}


/***************************************************************************
 * Function: gtab_trackcol
 *
 * Purpose:
 *
 * Start re-sizing a column
 */
void
gtab_trackcol(HWND hwnd, lpTable ptab, int col, int x)
{

        /* ensure we see the mouse-up */
        SetCapture(hwnd);
        ptab->trackmode = TRACK_COLUMN;
        ptab->tracknr = col;
        ptab->trackline1 = x;

        /* if line at other side of cell is visible, draw that too */
        if (ptab->pcellpos[col].start >= ptab->pcellpos[col].clipstart) {
                ptab->trackline2 = ptab->pcellpos[col].start;
        } else {
                ptab->trackline2 = -1;
        }
        gtab_drawvertline(hwnd, ptab);
}



/***************************************************************************
 * Function: gtab_press
 *
 * Purpose:
 *
 * Called on mouse-down events. decide what to start tracking.
 */
void
gtab_press(HWND hwnd, lpTable ptab, int x, int y)
{
        int cell;
        long row;

        if (ptab->trackmode != TRACK_NONE) {
                return;
        }

        /* has he grabbed a cell-edge to resize ? */
        cell = gtab_xtocol(hwnd, ptab, x);
        if (cell == -1) {
                return;
        }
        if (gtab_isborder(hwnd, ptab, x, cell)) {
                gtab_trackcol(hwnd, ptab, cell, x);
                return;
        }
        if ( (cell > 0) && gtab_isborder(hwnd, ptab, x, cell-1)) {
                gtab_trackcol(hwnd, ptab, cell, x);
                return;
        }

        /* find which line he selected */
        row = gtab_linetorow(hwnd, ptab, gtab_ytoline(hwnd, ptab, y));

        /* is he selecting a disabled fixed area ? */
        if ( (row < ptab->hdr.fixedrows) || (cell < ptab->hdr.fixedcols)) {
                if (ptab->hdr.fixedselectable == FALSE) {
                        return;
                }
        }

        /* ok, start cell selection */
        ptab->trackmode = TRACK_CELL;
        SetCapture(hwnd);

        /* record and paint new selection */
        if (ptab->hdr.selectmode & TM_ROW) {
                gtab_select(hwnd, ptab, row, 0, 1, ptab->hdr.ncols, FALSE);
        } else {
                gtab_select(hwnd, ptab, row, cell, 1, 1, FALSE);
        }
        return;
}

/***************************************************************************
 * Function: gtab_release
 *
 * Purpose:
 *
 * Called on mouse-up. complete any tracking that was happening
 */
void
gtab_release(HWND hwnd, lpTable ptab, int x, int y)
{
        lpCellPos ppos;
        lpProps pprop;
        long row;
        int cx;

        switch(ptab->trackmode) {

        case TRACK_NONE:
                return;
        
        case TRACK_COLUMN:
                /* erase marker lines */
                gtab_drawvertline(hwnd, ptab);
                ReleaseCapture();
                ptab->trackmode = TRACK_NONE;

                /* adjust cell width */
                ppos = &ptab->pcellpos[ptab->tracknr];
                cx = ptab->trackline1 - ppos->start;
                pprop = &ptab->pcolhdr[ptab->tracknr].props;
                pprop->valid |= P_WIDTH;
                pprop->width = cx;
                gtab_calcwidths(hwnd, ptab);
                gtab_setsize(hwnd, ptab);
                InvalidateRect(hwnd, NULL, TRUE);
                return;

        case TRACK_CELL:
                row = gtab_linetorow(hwnd, ptab, gtab_ytoline(hwnd, ptab, y));
                ReleaseCapture();
                ptab->trackmode = TRACK_NONE;

                /* keep the same selection. if the mouse is still
                 * in the box, select it, otherwise de-select it
                 */
                if ((row == ptab->select.startrow) &&
                  ( (ptab->hdr.selectmode & TM_ROW) ||
                    (ptab->select.startcell == gtab_xtocol(hwnd, ptab, x))) ) {

                        gtab_select(hwnd, ptab, ptab->select.startrow,
                                ptab->select.startcell,
                                ptab->select.nrows, ptab->select.ncells, TRUE);
                } else {
                        gtab_select(hwnd, ptab, 0, 0, 0, 0, TRUE);
                }
                return;
        }
}


/***************************************************************************
 * Function: gtab_move
 *
 * Purpose:
 *
 * Called on mouse-move. if tracking - adjust position, if not,
 * set correct cursor
 */
void
gtab_move(HWND hwnd, lpTable ptab, int x, int y)
{
        BOOL fOK;
        long row;
        int col;
        lpCellPos ppos;

        switch(ptab->trackmode) {

        case TRACK_NONE:
                col = gtab_xtocol(hwnd, ptab, x);
                if (col == -1) {
                        SetCursor(hNormCurs);
                        return;
                }
                if (gtab_isborder(hwnd, ptab, x, col)) {
                        SetCursor(hVertCurs);
                        return;
                }
                if ( (col > 0) && gtab_isborder(hwnd, ptab, x, col-1)) {
                        SetCursor(hVertCurs);
                        return;
                }
                SetCursor(hNormCurs);
                return;

        case TRACK_CELL:
                row = gtab_linetorow(hwnd, ptab, gtab_ytoline(hwnd, ptab, y));

                /* keep the same selection. if the mouse is still
                 * in the box, select it, otherwise de-select it
                 */
                if ((row == ptab->select.startrow) &&
                  ( (ptab->hdr.selectmode & TM_ROW) ||
                    (ptab->select.startcell == gtab_xtocol(hwnd, ptab, x))) ) {

                        if (!ptab->selvisible) {
                                gtab_invertsel(hwnd, ptab, NULL);
                                ptab->selvisible = TRUE;
                        }
                } else {
                        if (ptab->selvisible) {
                                gtab_invertsel(hwnd, ptab, NULL);
                                ptab->selvisible = FALSE;
                        }
                }
                return;
        
        case TRACK_COLUMN:
                /* check that new x is still visible/valid */
                ppos = &ptab->pcellpos[ptab->tracknr];
                fOK = FALSE;

                if (ptab->tracknr < ptab->hdr.fixedcols)  {
                        if ((x > ppos->start) && (x < ptab->winwidth)) {
                                fOK = TRUE;
                        }
                } else {
                        if ((x > ppos->clipstart) && (x < ptab->winwidth)) {
                                fOK = TRUE;
                        }
                }
                if (fOK == TRUE) {
                        gtab_drawvertline(hwnd, ptab);
                        ptab->trackline1 = x;
                        gtab_drawvertline(hwnd, ptab);
                }
                return;
        }
}

/***************************************************************************
 * Function: gtab_dblclick
 *
 * Purpose:
 *
 * dbl-click - send an TQ_ENTER event to the owner (if valid) 
 */
void
gtab_dblclick(HWND hwnd, lpTable ptab, int x, int y)
{
        int cell, line;
        long row;

        line = gtab_ytoline(hwnd, ptab, y);
        cell = gtab_xtocol(hwnd, ptab, x);
        if ( (line < ptab->hdr.fixedrows) || (cell < ptab->hdr.fixedcols) ) {
                if (!ptab->hdr.fixedselectable) {
                        return;
                }
        }
        row = gtab_linetorow(hwnd, ptab, line);

        if (ptab->hdr.selectmode & TM_ROW) {
                gtab_enter(hwnd, ptab, row, 0, 1, ptab->hdr.ncols);
        } else {
                gtab_enter(hwnd, ptab, row, cell, 1, 1);
        }
}

/***************************************************************************
 * Function: gtab_showsel
 *
 * Purpose:
 *
 * Move selection area to visible part of window. Argument bToBottom
 * indicates whether to move the line onto the bottom or the top of the
 * window if not visible - this affects the smoothness of scrolling
 * line-by-line.
 */
void
gtab_showsel(HWND hwnd, lpTable ptab, BOOL bToBottom)
{
        int line;
        long change;

        line = gtab_rowtoline(hwnd, ptab, ptab->select.startrow);

        /* move up if last line or not at all visible */
        if ( (line < 0) || line == (ptab->nlines - 1)) {
                change = ptab->select.startrow - ptab->toprow;
                if (bToBottom) {
                        /* change to bottom of window. subtract 2 not 1
                         * since nlines includes one line that is only
                         * partly visible
                         */
                        change -= (ptab->nlines - 2);
                }
                change -= ptab->hdr.fixedrows;
                gtab_dovscroll(hwnd, ptab, change);
        }
        /* add support for TM_CELL here! */
}

/***************************************************************************
 * Function: gtab_showsel_middle
 *
 * Purpose:
 *
 * Scroll the window so that if possible, the selected row is in the
 * middle 60% of the screen so that context around it is visible.
 */
void
gtab_showsel_middle(HWND hwnd, lpTable ptab)
{
        int line;
        long change;
        int mid_top, mid_end;

        line = gtab_rowtoline(hwnd, ptab, ptab->select.startrow);


        /* is this within the middle 60 % ?  */
        mid_top = ptab->nlines * 20 / 100;
        mid_end = ptab->nlines * 80 / 100;
        if ((line < mid_top) || (line > mid_end)) {

                /* no - scroll so that selected line is at
                 * the 20% mark
                 */
                change = (ptab->select.startrow - mid_top) - ptab->toprow;              
                change -= ptab->hdr.fixedrows;
                gtab_dovscroll(hwnd, ptab, change);
        }
        /* again - need code here for TM_CELL mode to ensure that
         * active cell is horizontally scrolled correctly
         */
}



/***************************************************************************
 * Function: gtab_changesel
 *
 * Purpose:
 *
 * Move the selection a specified nr of rows or cells
 * if no selection, select first visible unit
 */
VOID
gtab_changesel(HWND hwnd, lpTable ptab, long rowincr, int cellincr, BOOL bToBottom)
{
        long row, col, nrows, ncols;

        /* is there a selection ? */
        if (ptab->select.nrows < 1) {

                /* no selection - force a selection
                 * at the first visible unit
                 */
                if (ptab->hdr.fixedselectable) {
                        row = 0;
                        col = 0;
                } else {
                        row = gtab_linetorow(hwnd, ptab, ptab->hdr.fixedrows);
                        /* should really check for first visible cell */
                        col = ptab->hdr.fixedcols;
                }
                nrows = 1;
                ncols = 1;
                if (ptab->hdr.selectmode & TM_ROW) {
                        col = 0;
                        ncols = ptab->hdr.ncols;
                }
        } else {
                row = ptab->select.startrow + rowincr;
                col = ptab->select.startcell + cellincr;
                while (col >= ptab->hdr.ncols) {
                        col -= ptab->hdr.ncols;
                        row++;
                }
                while (col < 0) {
                        col += ptab->hdr.ncols;
                        row--;
                }
                if (row < 0) {
                        row = 0;
                }
                if (row >= ptab->hdr.nrows) {
                        row = ptab->hdr.nrows-1;
                }
                /* check we haven't moved into non-selectable region */
                if ((row < ptab->hdr.fixedrows) &&
                        (!ptab->hdr.fixedselectable)) {
                                row = ptab->hdr.fixedrows;
                }
                nrows = ptab->select.nrows;
                ncols = ptab->select.ncells;
        }
        gtab_select(hwnd, ptab, row, col, nrows, ncols, TRUE);
        /* ensure selection visible */
        gtab_showsel(hwnd, ptab, bToBottom);
}

/***************************************************************************
 * Function: gtab_selhome
 *
 * Purpose:
 *
 * Set the topmost selectable unit in window as the selection 
 */
void
gtab_selhome(HWND hwnd, lpTable ptab)
{
        long row;

        if (ptab->hdr.fixedselectable) {
                row = gtab_linetorow(hwnd, ptab, 0);
                if (ptab->hdr.selectmode & TM_ROW) {
                        gtab_select(hwnd, ptab, row, 0, 1,
                                ptab->hdr.ncols, TRUE);
                } else {
                        gtab_select(hwnd, ptab, row, 0, 1, 1, TRUE);
                }
        } else {
                row = gtab_linetorow(hwnd, ptab, ptab->hdr.fixedrows);
                if (ptab->hdr.selectmode & TM_ROW) {
                        gtab_select(hwnd, ptab, row, 0, 1,
                                ptab->hdr.ncols, TRUE);
                } else {
                        gtab_select(hwnd, ptab, row,
                                ptab->hdr.fixedcols, 1, 1, TRUE);
                }
        }
}


/***************************************************************************
 * Function: gtab_key
 *
 * Purpose:
 *
 * Handle key-down events - scroll windows and/or move selection 
 */
int
gtab_key(HWND hwnd, lpTable ptab, int vkey)
{
        long row;
        BOOL bControl = FALSE;

        if (GetKeyState(VK_CONTROL) & 0x8000) {
                bControl = TRUE;
        }

        switch(vkey) {

        case VK_UP:
                if (bControl) {
                        /* control-uparrow scrolls window without selection.
                         * the selection is de-selected (to avoid surprises
                         * moving back to it).
                         */
                        gtab_select(hwnd, ptab, 0, 0, 0, 0, TRUE);
                        gtab_dovscroll(hwnd, ptab, -1);
                } else {
                        /* uparrow moves selection up one line */
                        gtab_changesel(hwnd, ptab, -1, 0, FALSE);
                }
                return(0);

        case VK_DOWN:
                if (bControl) {
                        /* control downarrow scrolls window without
                         * a selection.
                         */
                        gtab_select(hwnd, ptab, 0, 0, 0, 0, TRUE);
                        gtab_dovscroll(hwnd, ptab, 1);
                } else {
                        /* the normal gtab_changesel behaviour is
                         * that if the selected line is not visible now,
                         * we scroll it to the top of the window. This is fine
                         * in most cases but causes unacceptable jumps when
                         * repeatedly scrolling down with the down key.
                         *
                         * Thus we now have an argument to changesel to say
                         * that in this case, if you need to move the line onto
                         * the window, move it to the bottom and not the top
                         */
                        gtab_changesel(hwnd, ptab, 1, 0, TRUE);
                }
                return(0);

        case VK_LEFT:
                /* if cell-selection mode, move left one cell.
                 * otherwise the whole row is selected - scroll
                 * the line left a little
                 */

                if (ptab->hdr.selectmode & TM_ROW) {
                        if (bControl) {
                                /* ctrl-left moves to start of line */
                                gtab_dohscroll(hwnd, ptab, -(ptab->scroll_dx));
                        } else {
                                gtab_dohscroll(hwnd, ptab, -(ptab->avewidth));
                        }
                } else {
                        gtab_changesel(hwnd, ptab, 0, -1, FALSE);
                }
                return(0);

        case VK_RIGHT:
                /* if cell-selection mode, move right one cell.
                 * otherwise the whole row is selected - scroll
                 * the line right a little
                 */
                if (ptab->hdr.selectmode & TM_ROW) {
                        if (bControl) {
                                /* control-right moves to right end of line */
                                gtab_dohscroll(hwnd, ptab, ptab->rowwidth -
                                                ptab->winwidth);
                        } else {
                                gtab_dohscroll(hwnd, ptab, ptab->avewidth);
                        }
                } else {
                        gtab_changesel(hwnd, ptab, 0, 1, TRUE);
                }
                return(0);

        case VK_HOME:
                if (bControl) {
                        /* control-home == top of file */
                        gtab_dovscroll(hwnd, ptab, -(ptab->toprow));
                }
                /* top of window */
                gtab_selhome(hwnd, ptab);
                gtab_showsel(hwnd, ptab, FALSE);
                
                return(0);

        case VK_END:
                if (bControl) {
                        /* control-end -> end of file */
                        row = ptab->hdr.nrows-1;
                } else {
                        row = gtab_linetorow(hwnd, ptab, ptab->nlines - 1);
                        if (row >= ptab->hdr.nrows) {
                                row = ptab->hdr.nrows-1;
                        }
                }
                if (ptab->hdr.selectmode & TM_ROW) {
                        gtab_select(hwnd, ptab, row, 0, 1,
                                ptab->hdr.ncols, TRUE);
                } else {
                        gtab_select(hwnd, ptab, row,
                                ptab->hdr.ncols-1, 1, 1, TRUE);
                }
                /* we have selected the bottom line. We don't want to
                 * move it up into the window, since the intended
                 * effect is to select the lowest line. This doesn't
                 * apply to the ctrl-end behaviour (move to bottom of
                 * buffer.
                 */
                if (bControl) {
                        /* move the selection to make it visible - but move it
                         * to the bottom and not to the top of the window
                         */
                        gtab_showsel(hwnd, ptab, TRUE);
                }
                return(0);

        case VK_RETURN:
                if (ptab->select.nrows > 0) {
                        gtab_showsel(hwnd, ptab, FALSE);
                        gtab_enter(hwnd, ptab, ptab->select.startrow,
                                ptab->select.startcell,
                                ptab->select.nrows, ptab->select.ncells);
                }
                return(0);

        case VK_SPACE:
                /* toggle the selection */
                if (ptab->select.nrows < 1) {
                        /* no selection - make one */
                        gtab_changesel(hwnd, ptab, 0, 0, TRUE);
                } else {
                        /* there is a selection - deselect it */
                        gtab_select(hwnd, ptab, 0, 0, 0, 0, TRUE);
                }
                return(0);

        case VK_PRIOR:          /* page up */

                gtab_dovscroll(hwnd, ptab, -(ptab->nlines - 3));
                gtab_selhome(hwnd, ptab);
                return(0);

        case VK_NEXT:           /* page down */

                /* scroll down one page */
                gtab_dovscroll(hwnd, ptab, (ptab->nlines - 3));

                /* select new bottom line */
                row = gtab_linetorow(hwnd, ptab, ptab->nlines - 1);
                if (row >= ptab->hdr.nrows) {
                        row = ptab->hdr.nrows-1;
                }
                /* select bottom line, but don't call showsel
                 * since we don't want to adjust it's position - we
                 * want it to remain at the bottom of the window
                 */
                if (ptab->hdr.selectmode & TM_ROW) {
                        gtab_select(hwnd, ptab, row, 0, 1,
                                ptab->hdr.ncols, TRUE);
                } else {
                        gtab_select(hwnd, ptab, row,
                                ptab->hdr.ncols-1, 1, 1, TRUE);
                }

                return(0);

        default:
                return(1);
        }
}

