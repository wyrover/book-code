
/****************************** Module Header *******************************
* Module Name: TPAINT.C
*
* Paint functions
*
* Functions:
*
* GetTextExtent()
* gtab_updatecontig()
* gtab_delcr()
* gtab_updateline()
* gtab_updatecontig()
* gtab_boxcell()
* gtab_paintcell()
* gtab_paint()
* gtab_vsep()
* gtab_hsep()
* gtab_drawvertline()
* gtab_invertsel()
* 
* Comments:
*
* See table.h for interface design.
*
****************************************************************************/
#include <string.h>
#include <windows.h>
#include <commdlg.h>

#include "gutils.h"
#include "table.h"
#include "tpriv.h"


/***************************************************************************
 * Function: GetTextExtent
 *
 * Purpose:
 * 
 * Calls GetTextExtentPoint - for ease of porting.
 */
 int
GetTextExtent(HDC hdc, LPSTR text, int len)
{
    SIZE sz;

    GetTextExtentPoint(hdc, text, len, &sz);
    return(sz.cx);
}

void gtab_updatecontig(HWND hwnd, lpTable ptab, int line, int cell1, int count);

/***************************************************************************
 * Function: gtab_delcr
 *
 * Purpose:
 *
 * change all cr/lf chars in input text to spaces 
 */
void gtab_delcr(LPSTR ptext)
{
        LPSTR chp;

        if (ptext == NULL) {
                return;
        }
        for(chp = ptext; (chp = strchr(chp, '\r')) != NULL; ) {
                *chp = ' ';
        }
        for(chp = ptext; (chp = strchr(chp, '\n')) != NULL; ) {
                *chp = ' ';
        }
}

/***************************************************************************
 * Function: gtab_updateline
 *
 * Purpose:
 *
 * Ensures that all visible cells in the given line have valid
 * text and property contents. loop through the cells, picking out
 * contiguous blocks of visible, invalid cells and call
 * gtab_updatecontig to update these from the owner window.
 */
void
gtab_updateline(HWND hwnd, lpTable ptab, int line)
{
        lpCellPos ppos;
        int cell1, cellcount;
        lpLineData pline;
        lpCellData cd;
        int i;

        pline = &ptab->pdata[line];
        cell1 = 0;
        cellcount = 0;
        for (i = 0; i < ptab->hdr.ncols; i++) {
                ppos = &ptab->pcellpos[i];
                cd = &pline->pdata[i];
                if (ppos->clipstart < ppos->clipend) {
                        if ((cd->flags & CELL_VALID) == 0) {
                                /* add a cell to the list to be updated*/
                                if (cellcount++ == 0) {
                                        cell1 = i;
                                }
                        } else {
                                /* this cell already valid - so end of
                                 * a contig block. if the contig
                                 * block just ended contained cells to update,
                                 * do it now
                                 */
                                if (cellcount > 0) {
                                        gtab_updatecontig(hwnd, ptab,
                                          line, cell1, cellcount);
                                }
                                cellcount = 0;
                        }
                }
                /* cell not visible - end of a contig block. If it was a
                 * non-empty contig block, then update it now.
                 */
                if (cellcount > 0)  {
                        gtab_updatecontig(hwnd, ptab, line, cell1, cellcount);
                        cellcount = 0;  
                }
        }
        if (cellcount > 0) {
                gtab_updatecontig(hwnd, ptab, line, cell1, cellcount);
                cellcount = 0;
        }
}

/***************************************************************************
 * Function: gtab_updatecontig
 *
 * Purpose:
 *
 * Updates a contiguous block of invalid cells by calling the owner window
 */
void
gtab_updatecontig(HWND hwnd, lpTable ptab, int line, int cell1, int count)
{
        lpLineData pline;
        lpCellData cd;
        CellDataList list;
        lpProps colprops;
        int i;

        pline = &ptab->pdata[line];
        cd = &pline->pdata[cell1];

        list.id = ptab->hdr.id;
        list.row = gtab_linetorow(hwnd, ptab, line);
        list.startcell = cell1;
        list.ncells = count;
        list.plist = cd;

        /* clear out prop flags */
        for (i = 0; i < count; i++) {
                cd[i].props.valid = 0;
                if (cd[i].nchars > 0) {
                        cd[i].ptext[0] = '\0';
                }
        }

        if (list.row < ptab->hdr.nrows) {
                gtab_sendtq(hwnd, TQ_GETDATA, (long) (LPSTR) &list);
        }

        /* for each cell, mark valid and set properties */
        for (i = 0; i < count; i++) {
                cd[i].flags |= CELL_VALID;
                gtab_delcr(cd[i].ptext);
                /* fetch properties from hdr and colhdr */
                colprops = &ptab->pcolhdr[i + cell1].props;
                if (!(cd[i].props.valid & P_FCOLOUR)) {
                        if (colprops->valid & P_FCOLOUR) {
                                cd[i].props.valid |= P_FCOLOUR;
                                cd[i].props.forecolour = colprops->forecolour;
                        } else if (ptab->hdr.props.valid & P_FCOLOUR) {
                                cd[i].props.valid |= P_FCOLOUR;
                                cd[i].props.forecolour =
                                        ptab->hdr.props.forecolour;
                        }
                }

                if (!(cd[i].props.valid & P_BCOLOUR)) {
                        if (colprops->valid & P_BCOLOUR) {
                                cd[i].props.valid |= P_BCOLOUR;
                                cd[i].props.backcolour = colprops->backcolour;
                        } else if (ptab->hdr.props.valid & P_BCOLOUR) {
                                cd[i].props.valid |= P_BCOLOUR;
                                cd[i].props.backcolour =
                                        ptab->hdr.props.backcolour;
                        }
                }

                if (!(cd[i].props.valid & P_FONT)) {
                        if (colprops->valid & P_FONT) {
                                cd[i].props.valid |= P_FONT;
                                cd[i].props.hFont = colprops->hFont;
                        } else if (ptab->hdr.props.valid & P_FONT) {
                                cd[i].props.valid |= P_FONT;
                                cd[i].props.hFont = ptab->hdr.props.hFont;
                        }
                }

                if (!(cd[i].props.valid & P_ALIGN)) {
                        if (colprops->valid & P_ALIGN) {
                                cd[i].props.valid |= P_ALIGN;
                                cd[i].props.alignment = colprops->alignment;
                        } else if (ptab->hdr.props.valid & P_ALIGN) {
                                cd[i].props.valid |= P_ALIGN;
                                cd[i].props.alignment =
                                        ptab->hdr.props.alignment;
                        }
                }

                if (!(cd[i].props.valid & P_BOX)) {
                        if (colprops->valid & P_BOX) {
                                cd[i].props.valid |= P_BOX;
                                cd[i].props.box = colprops->box;
                        } else if (ptab->hdr.props.valid & P_BOX) {
                                cd[i].props.valid |= P_BOX;
                                cd[i].props.box = ptab->hdr.props.box;
                        }
                }
                /* you can't set width/height per cell - this
                 * is ignored at cell level.
                 */
        }

}

/***************************************************************************
 * Function: gtab_boxcell
 *
 * Purpose:
 *
 * Draws box around a cell in a table.
 */ 
void
gtab_boxcell(HWND hwnd, HDC hdc, LPRECT rcp, LPRECT pclip, UINT boxmode)
{
        if (boxmode & P_BOXTOP) {
                MoveToEx(hdc, max(rcp->left, pclip->left),
                        max(rcp->top, pclip->top), NULL);
                LineTo(hdc, min(rcp->right, pclip->right),
                        max(rcp->top, pclip->top));
        }
        if (boxmode & P_BOXBOTTOM) {
                MoveToEx(hdc, max(rcp->left, pclip->left),
                        min(rcp->bottom, pclip->bottom), NULL);
                LineTo(hdc, min(rcp->right, pclip->right),
                        min(rcp->bottom, pclip->bottom));
        }
        if (boxmode & P_BOXLEFT) {
                MoveToEx(hdc, max(rcp->left, pclip->left),
                        max(rcp->top, pclip->top), NULL);
                MoveToEx(hdc, max(rcp->left, pclip->left),
                        min(rcp->bottom, pclip->bottom), NULL);
        }
        if (boxmode & P_BOXRIGHT) {
                MoveToEx(hdc, min(rcp->right, pclip->right),
                        max(rcp->top, pclip->top), NULL);
                LineTo(hdc, min(rcp->right, pclip->right),
                        min(rcp->bottom, pclip->bottom));
        }
}

/***************************************************************************
 * Function: gtab_paintcell
 *
 * Purpose:
 *
 * Paints a cell.
 */
void
gtab_paintcell(HWND hwnd, HDC hdc, lpTable ptab, int line, int cell)
{
        lpLineData pline;
        lpCellData cd;
        lpCellPos ppos;
        RECT rc, rcbox;
        int cx, x, y, tabwidth;
        UINT align;
        LPSTR chp, tabp;
        DWORD fcol, bkcol;
        HFONT hfont;
        TEXTMETRIC tm;
        HBRUSH hbr;

        /* init pointers to cell text and properties */
        pline = &ptab->pdata[line];
        cd = &pline->pdata[cell];
        ppos = &ptab->pcellpos[cell];

        /* clip all output to this rectangle */
        rc.top = pline->linepos.clipstart;
        rc.bottom = pline->linepos.clipend;
        rc.left = ppos->clipstart;
        rc.right = ppos->clipend;


        /* check cell properties and colours */
        if (cd->props.valid & P_ALIGN) {
                align = cd->props.alignment;
        } else {
                align = P_LEFT;
        }
        if (cd->props.valid & P_FONT) {
                hfont = SelectObject(hdc, cd->props.hFont);
                GetTextMetrics(hdc, &tm);
                tabwidth = tm.tmAveCharWidth * 8;
        } else {
                tabwidth = ptab->avewidth * 8;
        }

        /* set colours if not default */
        if (cd->props.valid & P_FCOLOUR) {
                fcol = SetTextColor(hdc, cd->props.forecolour);
        }
        if (cd->props.valid & P_BCOLOUR) {
                /* there is a non-default background colour.
                 * create a brush and fill the entire cell with it
                 */
                hbr = CreateSolidBrush(cd->props.backcolour);
                FillRect(hdc, &rc, hbr);
                DeleteObject(hbr);

                /* also set colour as background colour for the text */
                bkcol = SetBkColor(hdc, cd->props.backcolour);
        }

        /* calc offset of text within cell for right-align or centering */
        if (align == P_LEFT) {
                cx = ptab->avewidth/2;
        } else {
                if (cd->ptext == NULL) {
                        cx = 0;
                } else {
                        cx = LOWORD(GetTextExtent(hdc, cd->ptext,
                                        lstrlen(cd->ptext)));
                }
                if (align == P_CENTRE) {
                        cx = (ppos->size - cx) / 2;
                } else {
                        cx = ppos->size - cx - (ptab->avewidth/2);
                }
        }
        cx += ppos->start;

        /* expand tabs on output */
        x = 0;
        y = pline->linepos.start;

        for (chp = cd->ptext;
            ((chp != NULL) && ((tabp = strchr(chp, '\t')) != NULL)); ) {
                /* perform output upto tab char */
                ExtTextOut(hdc, x+cx, y, ETO_CLIPPED, &rc, chp, tabp-chp, NULL);
                
                /* advance past the tab */
                x += LOWORD(GetTextExtent(hdc, chp, tabp - chp));
                x = ( (x + tabwidth) / tabwidth) * tabwidth;
                chp = ++tabp;
        }

        /*no more tabs - output rest of string */
        if (chp != NULL) {
                ExtTextOut(hdc, x+cx, y, ETO_CLIPPED, &rc,
                                chp, lstrlen(chp), NULL);
        }

        /* reset colours to original if not default */
        if (cd->props.valid & P_FCOLOUR) {
                SetTextColor(hdc, fcol);
        }
        if (cd->props.valid & P_BCOLOUR) {
                SetBkColor(hdc, bkcol);
        }
        if (cd->props.valid & P_FONT) {
                SelectObject(hdc, hfont);
        }

        /* now box cell if marked */
        if (cd->props.valid & P_BOX) {
                if (cd->props.box != 0) {
                        rcbox.top = pline->linepos.start;
                        rcbox.bottom = rcbox.top + pline->linepos.size;
                        rcbox.left = ppos->start;
                        rcbox.right = ppos->start + ppos->size;
                        gtab_boxcell(hwnd, hdc, &rcbox, &rc, cd->props.box);
                }
        }
}

/***************************************************************************
 * Function: gtab_paint
 *
 * Purpose:
 *
 * Fetch and paint the specified line 
 */
void
gtab_paint(HWND hwnd, HDC hdc, lpTable ptab, int line)
{
        lpCellPos ppos;
        int i;

        gtab_updateline(hwnd, ptab, line);

        for (i = 0; i < ptab->hdr.ncols; i++) {
                ppos = &ptab->pcellpos[i];
                if (ppos->clipstart < ppos->clipend) {
                        gtab_paintcell(hwnd, hdc, ptab, line, i);
                }
        }
}


/***************************************************************************
 * Function: gtab_vsep
 *
 * Purpose:
 *
 */
void
gtab_vsep(HWND hwnd, lpTable ptab, HDC hdc)
{
        int x;
        RECT rc;

        if (ptab->hdr.fixedcols < 1) {
                return;
        }
        x = ptab->pcellpos[ptab->hdr.fixedcols - 1].clipend+1;
        GetClientRect(hwnd, &rc);
        MoveToEx(hdc, x, rc.top, NULL);
        LineTo(hdc, x, rc.bottom);
}

/***************************************************************************
 * Function: gtab_hsep
 *
 * Purpose:
 */
void
gtab_hsep(HWND hwnd, lpTable ptab, HDC hdc)
{
        int y;
        RECT rc;

        if (ptab->hdr.fixedrows < 1) {
                return;
        }
        y = ptab->rowheight * ptab->hdr.fixedrows;
        GetClientRect(hwnd, &rc);
        MoveToEx(hdc, rc.left, y-1, NULL);
        LineTo(hdc, rc.right, y-1);
}

/***************************************************************************
 * Function: gtab_drawverline
 *
 * Purpose:
 *
 * Draw in (inverting) the dotted selection lines for tracking a col width
 */
void
gtab_drawvertline(HWND hwnd, lpTable ptab)
{
        RECT rc;
        HDC hdc;
        HPEN hpen;

        hdc = GetDC(hwnd);
        SetROP2(hdc, R2_XORPEN);
        hpen = SelectObject(hdc, hpenDotted);
        GetClientRect(hwnd, &rc);

        MoveToEx(hdc, ptab->trackline1, rc.top, NULL);
        LineTo(hdc, ptab->trackline1, rc.bottom);
        if (ptab->trackline2 != -1) {
                MoveToEx(hdc, ptab->trackline2, rc.top, NULL);
                LineTo(hdc, ptab->trackline2, rc.bottom);
        }

        SelectObject(hdc, hpen);
        ReleaseDC(hwnd, hdc);
}
        

/***************************************************************************
 * Function: gtab_invertsel
 *
 * Purpose:
 *
 * Mark the selected line, if visible, in the style chosen by the
 * client app. This can be TM_SOLID, meaning an inversion of
 * the whole selected area or TM_FOCUS, meaning, inversion of the first
 * cell, and then a dotted focus rectangle for the rest.
 *
 * This function inverts either style, and so will turn the selection
 * both on and off.
 */
void
gtab_invertsel(HWND hwnd, lpTable ptab, HDC hdc_in)
{
        HDC hdc;
        int line;
        RECT rc;
        int lastcell;


        /* is row visible on screen ?  */
        line = gtab_rowtoline(hwnd, ptab, ptab->select.startrow);
        if (line < 0) {
                return;
        }

        /* selection mode includes a flag TM_FOCUS indicating we should
         * use a focus rect instead of the traditional inversion for
         * selections in this table. This interferes with multiple backgrnd
         * colours less.  However we still do inversion for fixedcols.
         */

        lastcell = (int)(ptab->select.startcell + ptab->select.ncells - 1);

        rc.top = ptab->pdata[line].linepos.clipstart;
        rc.bottom = ptab->pdata[line].linepos.clipend;

        /*
         * invert the whole area for TM_SOLID or just the first
         * cell for TM_FOCUS
         */
        rc.left = ptab->pcellpos[ptab->select.startcell].clipstart;
        if (ptab->hdr.selectmode & TM_FOCUS) {
                rc.right = ptab->pcellpos[ptab->select.startcell].clipend;
        }else {
                rc.right = ptab->pcellpos[lastcell].clipend;
        }

        if (hdc_in == NULL) {
                hdc = GetDC(hwnd);
        } else {
                hdc = hdc_in;
        }

        InvertRect(hdc, &rc);

        /*
         * draw focus rectangle around remaining cells on this line, if there
         * are any
         */
        if (ptab->hdr.selectmode & TM_FOCUS) {
                if (ptab->select.ncells > 1) {
                        rc.left = ptab->pcellpos[ptab->select.startcell+1].clipstart;
                        rc.right = ptab->pcellpos[lastcell].clipend;
                        DrawFocusRect(hdc, &rc);
                }
        }

        if (hdc_in == NULL) {
                ReleaseDC(hwnd, hdc);
        }
}
