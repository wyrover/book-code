
/****************************** Module Header *******************************
* Module Name: TABLE.C
*
* Standard table class and main interface functions.
*
* Functions:
*
* gtab_init()
* gtab_deltools()
* gtab_sendtq()
* gtab_freelinedata()
* gtab_wndproc()
* gtab_createtools()
* gtab_deltable()
* gtab_buildtable()
* gtab_setsize()
* gtab_newsize()
* gtab_calcwidths()
* gtab_alloclinedata()
* gtab_invallines()
* gtab_append()
*
* Comments:
*
* The table class communicates with its 'owner' window to
* get the layout info and the data to display. The owner window handle
* can be sent as the lParam in CreateWindow - if not, the parent window will
* be used.
*
* After creating the window, send it a TM_NEWID message, with a 'data id'
* as the lParam. This is any non-zero 32-bit value. The table will then call
* back to its owner window to find out how many rows/columns, then to fetch
* the name/properties of each column, and finally to get the data to display.
*
* Send TM_NEWID of 0 to close (or destroy the window) - wait for TQ_CLOSE
* (in either case) before discarding data. Send
* TM_REFRESH if data or row-count changes; send TM_NEWLAYOUT if column
* properties or nr cols change etc - this is the same as sending TM_NEWID
* except that no TQ_CLOSE happens on TM_NEWLAYOUT.
*
* TQ_SELECT is sent whenever the current selection changes. TQ_ENTER is sent
* when enter or double-click occurs.
*
****************************************************************************/

#include <windows.h>
#include <commdlg.h>

#include "gutils.h"
#include "table.h"
#include "tpriv.h"

/* global tools etc */
extern HANDLE hLibInst;
HANDLE hVertCurs;
HANDLE hNormCurs;
HPEN hpenDotted;
UINT gtab_msgcode;

/* function prototypes */
long FAR PASCAL gtab_wndproc(HWND, UINT, UINT, long);
void gtab_createtools(void);
void gtab_deltable(HWND hwnd, lpTable ptab);
lpTable gtab_buildtable(HWND hwnd, DWORD id);
void gtab_setsize(HWND hwnd, lpTable ptab);
void gtab_newsize(HWND hwnd, lpTable ptab);
void gtab_calcwidths(HWND hwnd, lpTable ptab);
BOOL gtab_alloclinedata(HWND hwnd, HANDLE heap, lpTable ptab);
void gtab_invallines(HWND hwnd, lpTable ptab, int start, int count);
void gtab_append(HWND hwnd, lpTable ptab, int rows, DWORD id);

/***************************************************************************
 * Function: gtab_init
 *
 * Purpose:
 *
 * Initialise window class - called from DLL main init
 */
void
gtab_init(void)
{
        WNDCLASS wc;

        gtab_createtools();
        gtab_msgcode = RegisterWindowMessage(TableMessage);

        wc.style = CS_GLOBALCLASS | CS_DBLCLKS;
        wc.lpfnWndProc = gtab_wndproc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = WLTOTAL;
        wc.hInstance = hLibInst;
        wc.hIcon = NULL;
        wc.hCursor = NULL;
        wc.hbrBackground = GetStockObject(WHITE_BRUSH);
        wc.lpszClassName = TableClassName;
        wc.lpszMenuName = NULL;

        RegisterClass(&wc);
}

/***************************************************************************
 * Function: gtab_createtools
 *
 * Purpose:
 *
 * Load cursors and pens.
 */
 void
gtab_createtools(void)
{
        hVertCurs = LoadCursor(hLibInst, "VertLine");
        hNormCurs = LoadCursor(NULL, IDC_ARROW);

        hpenDotted = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
}

/***************************************************************************
 * Function: gtab_deltools
 *
 * Purpose:
 *
 * Delete pen
 */
 void
gtab_deltools(void)
{
        DeleteObject(hpenDotted);
}


/***************************************************************************
 * Function: gtab_wndproc
 *
 * Purpose:
 *
 * Window procedure for table
 */
 long FAR PASCAL
gtab_wndproc(HWND hwnd, UINT msg, UINT wParam, long lParam)
{
        CREATESTRUCT FAR * csp;
        HWND hOwner;
        lpTable ptab;
        HANDLE hHeap;
        PAINTSTRUCT ps;
        int y, y2, i;
        HDC hDC;
        lpTableSelection pselect;
        long oldtop;
        long change;

        switch(msg) {

        case WM_CREATE:
                /* create window. set the wnd extra bytes to
                 * contain the owner window, a heap and a null table.
                 * Owner window is either in lParam or the parent.
                 * Then wait for TM_NEWID.
                 */
                csp = (CREATESTRUCT FAR *) lParam;
                if (csp->lpCreateParams == NULL) {
                        hOwner = GetParent(hwnd);
                } else {
                        hOwner = (HWND) (long) csp->lpCreateParams;
                }
                ptab = NULL;
                hHeap = gmem_init();
                SetWindowLong(hwnd, WL_TABLE, (LONG) ptab);
                SetWindowLong(hwnd, WW_OWNER, (LONG) hOwner);
                SetWindowLong(hwnd, WW_HEAP, (LONG) hHeap);

                SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
                SetScrollRange(hwnd, SB_HORZ, 0, 0, TRUE);
                break;

        case TM_NEWID:
                /* complete change of table.
                 * close old table, discard memory and
                 * build new table
                 */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_sendtq(hwnd, TQ_CLOSE, ptab->hdr.id);
                        gtab_deltable(hwnd, ptab);
                        SetCursor(hNormCurs);
                        SetWindowLong(hwnd, WL_TABLE, 0);
                }
                if ( (ptab = gtab_buildtable(hwnd, lParam)) != NULL) {
                        SetWindowLong(hwnd, WL_TABLE, (long) (LPSTR) ptab);
                        gtab_setsize(hwnd, ptab);
                } else {
                        SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
                        SetScrollRange(hwnd, SB_HORZ, 0, 0, TRUE);
                }
                InvalidateRect(hwnd, NULL, TRUE);
                break;

        case TM_NEWLAYOUT:
                /* change of layout but for same id. no TQ_CLOSE,
                 * but otherwise same as TM_NEWID
                 */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_deltable(hwnd, ptab);
                        SetCursor(hNormCurs);
                        SetWindowLong(hwnd, WL_TABLE, 0);
                }
                if ( (ptab = gtab_buildtable(hwnd, lParam)) != NULL) {
                        SetWindowLong(hwnd, WL_TABLE, (long) (LPSTR) ptab);
                        gtab_setsize(hwnd, ptab);
                } else {
                        SetScrollRange(hwnd, SB_VERT, 0, 0, TRUE);
                        SetScrollRange(hwnd, SB_HORZ, 0, 0, TRUE);
                }
                InvalidateRect(hwnd, NULL, TRUE);
                break;

        case TM_REFRESH:
                /* data in table has changed. nrows may have
                 * changed. ncols and col types have not changed
                 */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_newsize(hwnd, ptab);
                }
                InvalidateRect(hwnd, NULL, TRUE);
                break;

        case TM_SELECT:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        pselect = (lpTableSelection) lParam;

                        /*
                         * we only support TM_SINGLE - so force the
                         * selection to a single row or cell.
                         */
                        gtab_select(hwnd, ptab, pselect->startrow,
                                pselect->startcell,
                                1,
                                (ptab->hdr.selectmode & TM_ROW) ?
                                        ptab->hdr.ncols : 1,
                                TRUE);
                        gtab_showsel_middle(hwnd, ptab);
                }
                break;

        case TM_PRINT:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                hHeap = (HANDLE) GetWindowLong(hwnd, WW_HEAP);
                if (ptab != NULL) {
                        gtab_print(hwnd, ptab, hHeap, (lpPrintContext) lParam);
                        return(TRUE);
                }

        case TM_TOPROW:

                /* return top row. if wParam is TRUE, set lParam
                 * as the new toprow
                 */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab == NULL) {
                        return(0);
                }
                oldtop = ptab->toprow;
                if ((wParam) && (lParam < ptab->hdr.nrows)) {
                        change = lParam - ptab->toprow;
                        change -= ptab->hdr.fixedrows;
                        gtab_dovscroll(hwnd, ptab, change);
                }
                return(oldtop);

        case TM_ENDROW:
                /* return the last visible row in the window */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab == NULL) {
                        return(0);
                }
                return(ptab->nlines + ptab->toprow - 1);


        case TM_APPEND:
                /* new rows have been added to the end of the
                 * table, but the rest of the table has no
                 * been change. Update without forcing redraw of
                 * everything.
                 * lParam contains the new total nr of rows
                 */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_append(hwnd, ptab, wParam, lParam);
                        return(TRUE);
                }
                break;

        case WM_SIZE:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_setsize(hwnd, ptab);
                }
                break;

        case WM_DESTROY:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_sendtq(hwnd, TQ_CLOSE, ptab->hdr.id);
                        gtab_deltable(hwnd, ptab);
                }
                hHeap = (HANDLE) GetWindowLong(hwnd, WW_HEAP);
                gmem_freeall(hHeap);
                break;

        case WM_PAINT:
                hDC = BeginPaint(hwnd, &ps);
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        /* separator lines between fixed rows/columns
                         * (ie headers) and the rest - if enabled
                         */
                        /* paint here first for good impression,
                         * and again after to clean up!!
                         */
                        if (ptab->hdr.vseparator) {
                                gtab_vsep(hwnd, ptab, hDC);
                        }
                        if (ptab->hdr.hseparator) {
                                gtab_hsep(hwnd, ptab, hDC);
                        }

                        /* paint only the rows that need painting */
                        for (i = 0; i < ptab->nlines; i++) {
                                y = ptab->pdata[i].linepos.start;
                                y2 = y + ptab->pdata[i].linepos.size;
                                if ( (y <= ps.rcPaint.bottom) &&
                                     (y2 >= ps.rcPaint.top)) {
                                        gtab_paint(hwnd, hDC, ptab, i);
                                }
                        }
                        if (ptab->hdr.vseparator) {
                                gtab_vsep(hwnd, ptab, hDC);
                        }
                        if (ptab->hdr.hseparator) {
                                gtab_hsep(hwnd, ptab, hDC);
                        }
                        if (ptab->selvisible) {
                                gtab_invertsel(hwnd, ptab, hDC);
                        }
                }

                EndPaint(hwnd, &ps);
                break;

        case WM_HSCROLL:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_msg_hscroll(hwnd, ptab,
                          GET_SCROLL_OPCODE(wParam, lParam),
                          GET_SCROLL_POS(wParam, lParam));
                }
                break;

        case WM_VSCROLL:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_msg_vscroll(hwnd, ptab,
                          GET_SCROLL_OPCODE(wParam, lParam),
                          GET_SCROLL_POS(wParam, lParam));
                }
                break;

        case WM_MOUSEMOVE:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_move(hwnd, ptab, LOWORD(lParam), HIWORD(lParam));
                } else {
                        SetCursor(hNormCurs);
                }
                break;

        case WM_LBUTTONDOWN:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_press(hwnd, ptab, LOWORD(lParam), HIWORD(lParam));
                }
                break;

        case WM_LBUTTONUP:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_release(hwnd, ptab,
                                LOWORD(lParam), HIWORD(lParam));
                }
                break;

        case WM_LBUTTONDBLCLK:
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        gtab_dblclick(hwnd, ptab,
                                LOWORD(lParam), HIWORD(lParam));
                }
                break;

        case WM_KEYDOWN:
                /* handle key presses for cursor movement about
                 * the table, and return/space for selection.
                 * Any key we don't handle is passed to the owner window
                 * for him to handle.
                 * The table window should have the focus
                 */
                ptab = (lpTable) GetWindowLong(hwnd, WL_TABLE);
                if (ptab != NULL) {
                        if (gtab_key(hwnd, ptab, wParam) != 0) {
                                /* pass key to owner since
                                 * we don't know what to do with it
                                 */
                                hOwner = (HANDLE) GetWindowLong(hwnd, WW_OWNER);
                                return(SendMessage(hOwner, WM_KEYDOWN,
                                        wParam, lParam));
                        } else {
                                return(0);      
                        }
                }
                break;

        default:
                return(DefWindowProc(hwnd, msg, wParam, lParam));
        }
        return(TRUE);
}

/***************************************************************************
 * Function: gtab_sendtq
 *
 * Purpose:
 *
 * Send a table-query message to the owner window. Returns message
 * value.
 */
long
gtab_sendtq(HWND hwnd, UINT cmd, long lParam)
{
        HWND hOwner;

        hOwner = (HANDLE) GetWindowLong(hwnd, WW_OWNER);
        return (SendMessage(hOwner, gtab_msgcode, cmd, lParam));
}

/***************************************************************************
 * Function: gtab_freelinedata
 *
 * Purpose:
 *
 * Free the memory allocated for the array of lines (each containing
 * an array of Cells, each containing an array of chars for the actual
 * data). Called on any occasion that would change the number of visible lines
 */
void
gtab_freelinedata(HANDLE hHeap, lpTable ptab)
{
        int i, j, ncols;
        lpCellData cd;


        ncols = ptab->hdr.ncols;

        /* for each line */
        for(i = 0; i < ptab->nlines; i++) {
                /* for each cell */
                for (j = 0; j < ncols; j++) {
                        /* free up the actual text space */
                        cd = &ptab->pdata[i].pdata[j];
                        gmem_free(hHeap, (LPSTR) cd->ptext, cd->nchars);
                }
                /* dealloc array of CellData */
                gmem_free(hHeap, (LPSTR) ptab->pdata[i].pdata,
                        sizeof(CellData) * ncols);
        }
        /* de-alloc array of linedatas */
        gmem_free(hHeap, (LPSTR) ptab->pdata,
                sizeof(LineData) * ptab->nlines);
        ptab->pdata = NULL;
}

/***************************************************************************
 * Function: gtab_alloclinedata
 *
 * Purpose:
 *
 * Allocate and init array of linedatas (include cell array
 * and text for each cell)
 */
BOOL
gtab_alloclinedata(HWND hwnd, HANDLE heap, lpTable ptab)
{
        lpLineData pline;
        lpCellData cd;
        int i, j;

        ptab->pdata = (lpLineData) gmem_get(heap,
                sizeof(LineData) * ptab->nlines);
        if (ptab->pdata == NULL) {
                return(FALSE);
        }
        for (i = 0; i < ptab->nlines; i++) {
                pline = &ptab->pdata[i];
                pline->linepos.size = ptab->rowheight;
                pline->pdata = (lpCellData) gmem_get(heap,
                        sizeof(CellData) * ptab->hdr.ncols);
                if (pline->pdata == NULL) {
                        return(FALSE);
                }
                for (j = 0; j < ptab->hdr.ncols; j++) {
                        cd = &pline->pdata[j];
                        cd->props.valid = 0;
                        cd->flags = 0;
                        cd->nchars = ptab->pcolhdr[j].nchars;
                        if (cd->nchars > 0) {
                                cd->ptext = gmem_get(heap, cd->nchars);
                                if (cd->ptext == NULL) {
                                        return(FALSE);
                                }
                        }
                }
        }
}

/***************************************************************************
 * Function: gtab_deltable
 *
 * Purpose:
 *
 * Free up all table data structures. Called for new layout or new data.
 */
void
gtab_deltable(HWND hwnd, lpTable ptab)
{
        HANDLE hHeap;
        int ncols;

        if (ptab == NULL) {
                return;
        }
        hHeap = (HANDLE) GetWindowLong(hwnd, WW_HEAP);
        ncols = ptab->hdr.ncols;

        if (ptab->pcolhdr != NULL) {
                gmem_free(hHeap, (LPSTR) ptab->pcolhdr,
                        sizeof(ColProps) * ncols);
        }
        if (ptab->pcellpos != NULL) {
                gmem_free(hHeap, (LPSTR) ptab->pcellpos,
                        sizeof(CellPos) * ncols);
        }
        if (ptab->pdata != NULL) {
                gtab_freelinedata(hHeap, ptab);
        }
        gmem_free(hHeap, (LPSTR) ptab, sizeof(Table));
}


/***************************************************************************
 * Function: gtab_buildtable
 *
 * Purpose:
 *
 * Build up a Table struct (excluding data allocation and
 * anything to do with font or window size).
 * Return ptr to this or NULL if error
 */
lpTable
gtab_buildtable(HWND hwnd, DWORD id)
{
        lpTable ptab;
        HANDLE hHeap;
        int ncols, i;
        ColPropsList cplist;

        hHeap = (HANDLE) GetWindowLong(hwnd, WW_HEAP);
        ptab = (lpTable) gmem_get(hHeap, sizeof(Table));
        if (ptab == NULL) {
                return(NULL);
        }

        /* get the row/column count from owner window */
        ptab->hdr.id = id;
        ptab->hdr.props.valid = 0;
        ptab->hdr.sendscroll = FALSE;
        if (gtab_sendtq(hwnd, TQ_GETSIZE, (long) (LPSTR)&ptab->hdr) == FALSE) {
                return(NULL);
        }

        ncols = ptab->hdr.ncols;
        ptab->pcolhdr = (lpColProps) gmem_get(hHeap, sizeof(ColProps) * ncols);
        if (ptab->pcolhdr == NULL) {
                /* should prob send TQ_CLOSE at this point */
                return(NULL);
        }

        /* init col properties to default */
        for (i=0; i < ncols; i++) {
                ptab->pcolhdr[i].props.valid = 0;
                ptab->pcolhdr[i].nchars = 0;
        }
        /* get the column props from owner */
        cplist.plist = ptab->pcolhdr;
        cplist.id = id;
        cplist.startcol = 0;
        cplist.ncols = ncols;
        gtab_sendtq(hwnd, TQ_GETCOLPROPS, (long) (LPSTR)&cplist);

        /* init remaining fields */
        ptab->pcellpos = (lpCellPos) gmem_get(hHeap, sizeof(CellPos) * ncols);
        if (ptab->pcellpos == NULL) {
                return(NULL);
        }

        ptab->scrollscale = 1;
        ptab->scroll_dx = 0;
        ptab->toprow = 0;
        ptab->pdata = NULL;
        ptab->nlines = 0;
        ptab->trackmode = TRACK_NONE;

        /* we have to notify owner of the current selection
         * whenever it is changed
         */
        ptab->select.id = id;
        gtab_select(hwnd, ptab, 0, 0, 0, 0, TRUE);

        /* calc ave height/width, cell widths and min height.
         * these change only when cell properties / col count changes -
         * ie only on rebuild-header events
         */
        gtab_calcwidths(hwnd, ptab);
        return(ptab);
}

/***************************************************************************
 * Function: gtab_setsize
 *
 * Purpose:
 *
 * Set sizes that are based on window size and scroll pos
 * set:
 *      winwidth
 *      nlines
 *      cellpos start, clip start/end
 * Alloc linedata and init
 */
void
gtab_setsize(HWND hwnd, lpTable ptab)
{
        RECT rc;
        int nlines;
        HANDLE heap;
        long range, change;

        GetClientRect(hwnd, &rc);
        ptab->winwidth = rc.right - rc.left;
        nlines = (rc.bottom - rc.top) / ptab->rowheight;
        /* nlines is the number of whole lines - add one extra
         * for the partial line at the bottom
         */
        nlines += 1;

        /* alloc space for nlines of data - if nlines has changed */
        if (nlines != ptab->nlines) {
                heap = (HANDLE) GetWindowLong(hwnd, WW_HEAP);
                gtab_freelinedata(heap, ptab);
                ptab->nlines = nlines;
                if (!gtab_alloclinedata(hwnd, heap, ptab)) {
                        ptab->nlines = 0;
                        return;
                }
        }

        /* set scroll vertical range */
        range = ptab->hdr.nrows - (ptab->nlines - 1);
        if (range < 0) {
                range = 0;
                change =  -(ptab->toprow);
        } else if (ptab->toprow > range) {
                change = range - ptab->toprow;
        } else {
                change = 0;
        }
        /* the scroll range must be 16-bits for Win3
         * scale until this is true
         */
        ptab->scrollscale = 1;
        while (range > 32766) {
                ptab->scrollscale *= 16;
                range /= 16;
        }

        SetScrollRange(hwnd, SB_VERT, 0, (int) range, TRUE);
        gtab_dovscroll(hwnd, ptab, change);

        /* set horz scroll range */
        range = ptab->rowwidth - ptab->winwidth;
        if (range < 0) {
                range = 0;
                change = -(ptab->scroll_dx);
        } else if (ptab->scroll_dx > range) {
                change = range - ptab->scroll_dx;
        } else {
                change = 0;
        }
        /* horz scroll range will always be < 16 bits */
        SetScrollRange(hwnd, SB_HORZ, 0, (int) range, TRUE);
        gtab_dohscroll(hwnd, ptab, change);
}

/***************************************************************************
 * Function: gtab_calcwidths
 *
 * Purpose:
 *
 * Set column widths/height and totals (based on column props)
 * - no assumption of window size (see gtab_setsize)
 * sets avewidth,rowheight,cellpos.size,rowwidth (total of cellpos.size)
 */
void
gtab_calcwidths(HWND hwnd, lpTable ptab)
{
        int i, cxtotal, cx, ave;
        TEXTMETRIC tm, tmcol;
        HDC hdc;
        lpProps hdrprops, cellprops;
        HFONT hfont;

        hdrprops = &ptab->hdr.props;
        hdc = GetDC(hwnd);
        if (hdrprops->valid & P_FONT) {
                hfont = SelectObject(hdc, hdrprops->hFont);
        }
        GetTextMetrics(hdc, &tm);
        if (hdrprops->valid & P_FONT) {
                SelectObject(hdc, hfont);
        }
        ReleaseDC(hwnd, hdc);

        /* get width and height of average character */
        ptab->avewidth = tm.tmAveCharWidth;
        ptab->rowheight = tm.tmHeight + tm.tmExternalLeading;
        if (hdrprops->valid & P_HEIGHT) {
                ptab->rowheight = hdrprops->height;
        }

        /* set pixel width of each cell (and add up for row total)
         * based on ave width * nr chars, unless P_WIDTH set
         */
        cxtotal = 0;
        for (i = 0; i < ptab->hdr.ncols; i++) {
                cellprops = &ptab->pcolhdr[i].props;

                if (cellprops->valid & P_WIDTH) {
                        cx = cellprops->width;
                } else if (hdrprops->valid & P_WIDTH) {
                        cx = hdrprops->width;
                } else {

                        if (cellprops->valid & P_FONT) {
                                hdc = GetDC(hwnd);
                                hfont = SelectObject(hdc, cellprops->hFont);
                                GetTextMetrics(hdc, &tmcol);
                                SelectObject(hdc, hfont);
                                ReleaseDC(hwnd, hdc);
                                ave = tmcol.tmAveCharWidth;
                        } else {
                                ave = ptab->avewidth;
                        }
                        /* ave width * nchars */
                        cx =  ptab->pcolhdr[i].nchars + 1;
                        cx *= ave;
                }
                /* add 2 pixels for box lines */
                cx += 2;
                ptab->pcellpos[i].size = cx;
                cxtotal += cx;
        }
        ptab->rowwidth = cxtotal;
}

/***************************************************************************
 * Function: gtab_newsize
 *
 * Purpose:
 *
 * Called when row data + possible nrows changes.
 * other changes are ignored
 */
void
gtab_newsize(HWND hwnd, lpTable ptab)
{
        TableHdr hdr;

        /* get new row count */
        hdr = ptab->hdr;
        gtab_sendtq(hwnd, TQ_GETSIZE, (long) (LPSTR) &hdr);
        if (hdr.nrows != ptab->hdr.nrows) {
                ptab->hdr.nrows = hdr.nrows;
                gtab_setsize(hwnd, ptab);
        }

        gtab_invallines(hwnd, ptab, 0, ptab->nlines);

        InvalidateRect(hwnd, NULL, TRUE);
}

void
gtab_invallines(HWND hwnd, lpTable ptab, int start, int count)
{
        int i, j;

        for (i = start; i < start + count; i++) {
                for (j = 0; j < ptab->hdr.ncols; j++) {
                        ptab->pdata[i].pdata[j].flags = 0;
                }
        }
}

/***************************************************************************
 * Function: gtab_append
 *
 * Purpose:
 *
 * New rows have been added to the table. Adjust the scroll range and
 * position, and redraw the rows if the end of the table is currently
 * visible.
 * rows = the new total row count.
 */
void
gtab_append(HWND hwnd, lpTable ptab, int rows, DWORD id)
{
        long range;
        long oldrows;
        int line, nupdates;
        RECT rc;


        /* change to the new id */
        ptab->hdr.id = id;
        ptab->select.id = id;

        /* update the header, but remember the old nr of rows
         * so we know where to start updating
         */
        oldrows = ptab->hdr.nrows;

        /* check that the new nr of rows is not smaller. this is
         * illegal at this point and should be ignored
         */
        if (oldrows >= rows) {
                return; 
        }

        ptab->hdr.nrows = rows;

        /* set the vertical scroll range */
        range = rows - (ptab->nlines - 1);

        if (range < 0) {
                range = 0;      
        }

        /* force the scroll range into 16-bits for win 3.1 */
        ptab->scrollscale = 1;
        while (range > 32766) {
                ptab->scrollscale *= 16;
                range /= 16;
        }

        /* now set the scroll bar range and position */
        SetScrollRange(hwnd, SB_VERT, 0, (int) range, TRUE);
        if (range > 0) {
                SetScrollPos(hwnd, SB_VERT,
                        (int) (ptab->toprow / ptab->scrollscale), TRUE);
        }

        /* calculate which screen lines need to be updated - find what
         * screen line the start of the new section is at
         */
        line = gtab_rowtoline(hwnd, ptab, oldrows);
        if (line == -1) {
                /* not visible -> no more to do */
                return;
        }

        /* how many lines to update - rest of screen or nr of
         * new lines if less than rest of screen
         */
        nupdates = min((ptab->nlines - line), (int)(rows - oldrows));

        /* invalidate the screen line buffers to indicate data
         * needs to be refetch from parent window
         */
        gtab_invallines(hwnd, ptab, line, nupdates);

        /* calculate the region of the screen to be repainted -
         * left and right are same as window. top and bottom
         * need to be calculated from screen line height
         */
        
        GetClientRect(hwnd, &rc);
        rc.top += line * ptab->rowheight;
        rc.bottom = rc.top + (nupdates * ptab->rowheight);

        /* force a repaint of the updated region */
        InvalidateRect(hwnd, &rc, TRUE);
}
        


        
