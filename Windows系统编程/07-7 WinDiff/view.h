/*
 * VIEW.H
 */

/* view.h includes the term COMPLIST: complist.h uses the term VIEW.
 */
#ifndef INC_VIEW_COMPLIST
#define INC_VIEW_COMPLIST
typedef struct view FAR * VIEW;                 /* handle to a VIEW     */
typedef struct complist FAR * COMPLIST;         /* handle to a complist */
#endif // INC_VIEW_COMPLIST

VIEW view_new(HWND hwndTable);
BOOL view_setcomplist(VIEW view, COMPLIST cl);
COMPLIST view_getcomplist(VIEW view);
void view_close(VIEW view);
void view_delete(VIEW view);
LPSTR view_gettext(VIEW view, long row, int col);
int view_getlinenr_left(VIEW view, long row);
int view_getlinenr_right(VIEW view, long row);
int view_getwidth(VIEW view, int col);
long view_getrowcount(VIEW view);
int view_getstate(VIEW view, long row);
BOOL view_expand(VIEW view, long row);
void view_outline(VIEW);
COMPITEM view_getitem(VIEW view, long row);
BOOL view_isexpanded(VIEW view);        
LPSTR view_getcurrenttag(VIEW view);
BOOL view_newitem(VIEW view);
void view_changeviewoptions(VIEW view);
void view_changediffoptions(VIEW view);
long view_findchange(VIEW view, long startrow, BOOL bForward);

