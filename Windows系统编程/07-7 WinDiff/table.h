

/*
 * TABLE.H
 *
 * public interface definition for table window class.
 *
 * include after gutils.h and commdlg.h
 */

/* -------class and message names --------------------------------------*/

/* create a window of this class */
#define  TableClassName "GTableClass"


/* all messages to the owner window are sent with this message.
 * call RegisterWindowsMessage with this string for the message UINT.
 */
#define TableMessage  "GTableQuery"

/* -------- messages to and from table class  --------------------------*/

/* messages to owner window are:
 *      message:        TableMessage
 *      wParam:         command code (below)
 * lParam:              struct pointer according to code
 * below is list of wParam codes & associated lParam struct
 */
#define TQ_GETSIZE      1       /* lParam: lpTableHdr */
#define TQ_GETCOLPROPS  2       /* lParam: lpColPropList */
#define TQ_GETDATA      3       /* lParam: lpCellDataList */
#define TQ_PUTDATA      4       /* lParam: lpCellDataList */
#define TQ_SELECT       5       /* lParam: lpTableSelection */
#define TQ_ENTER        6       /* lParam: lpTableSelection */
#define TQ_CLOSE        7       /* lParam: the data id to be closed */

/* optional */
#define TQ_SCROLL       8       /* lParam: the new top row nr */


/* messages to Table class */

/* data, or nrows has changed  wParam/lParam null*/
#define TM_REFRESH      (WM_USER)

/* nr cols/props/layout has changed  - wparam/lparam null */
#define TM_NEWLAYOUT    (WM_USER+1)

/* Close old id, and display new - wParam null, lParam has new id */
#define TM_NEWID        (WM_USER+2)

/* Select and show this area - wParam null, lParam is lpTableSelection */
#define TM_SELECT       (WM_USER+3)

/* Print current table - wParam null, lParam either null
 * or lpPrintContext.
 */
#define TM_PRINT        (WM_USER+4)

/* Return the top row in the window. If wParam is TRUE, then set
 * lParam to be the new toprow. top row is the number of rows scrolled down
 * from the top. Thus the first visible non-fixed row is toprow+fixedrows
 */
#define TM_TOPROW       (WM_USER+5)


/* Return the end row visible. This is the 0-based rownr of the last
 * visible row in the window
 */
#define TM_ENDROW       (WM_USER+6)

/* New rows have been added to the end of the table, but no other
 * rows or cols or properties have been changed.
 * wParam contains the new total nr of rows. lParam contains the id
 * in case this has changed.
 */
#define TM_APPEND       (WM_USER+7)

/*-----display properties -------------------------------------------------*/

/*
 * Display properties struct. can be set for whole table, for
 * each column, or for each cell. When looking for
 * a property, we search cell->column->table
 */
typedef struct {
        UINT valid;             /* flags (below) for what props we set */

/* remaining fields only valid when corresponding flag set in valid */

        DWORD forecolour;       /* RGB colour value */
        DWORD backcolour;       /* ditto */
        /* font to use - also set through WM_SETFONT. owner application
         * is responsible for DeleteObject call when no longer used
         */
        HFONT hFont;            /* handle to font  - caller should delete*/
        UINT alignment;         /* flags below */
        UINT box;               /* whether cell boxed (see below) */

        /* width/height settings not valid at cell level - only table or col.*/
        int width;              /* pixel width of this cell/column */
        int height;             /* pixel cell height */
} Props, FAR * lpProps;

/* Valid flags for fields that are changed in this Props struct */
#define P_FCOLOUR       1
#define P_BCOLOUR       2
#define P_FONT          4
#define P_ALIGN         8
#define P_BOX           0x20
#define P_WIDTH         0x40
#define P_HEIGHT        0x80

/* Box settings  or-ed together */
#define P_BOXTOP        1
#define P_BOXBOTTOM     2
#define P_BOXLEFT       4
#define P_BOXRIGHT      8
#define P_BOXALL        0xF

/* Alignment settings (expand later to include various tab-align settings */
#define P_LEFT          0
#define P_RIGHT         1
#define P_CENTRE        2

/* This struct is the master information about a table. It is
 * passed to the owner window with the id field filled in; fill in
 * all remaining fields and return.
 */
typedef struct {
        DWORD id;               /* owner's data id */

        long nrows;             /* how many rows ? TM_REFRESH to change */
        int ncols;              /* how many columns ? TM_NEWLAYOUT to chg */

        int fixedrows;          /* for headers - usually 0 or 1 */
        int fixedcols;          /* for hdrs - 0 or 1 normally */
        BOOL fixedselectable;   /* is fixed area selectable ? */
        BOOL hseparator;        /* is there a horz. line after fixed rows */
        BOOL vseparator;        /* is there a vert. line after fixed rows */

        UINT selectmode;        /* multiple/single selection - flags below*/
        BOOL sendscroll;        /* TRUE if TQ_SCROLL to be sent on scrolling*/

        Props props;
} TableHdr, FAR * lpTableHdr;

/*
 * selection mode;
 *
 * choose TM_CELL or TM_ROW, and TM_SINGLE or TM_MANY, and
 * TM_SOLID or TM_FOCUS and or them together.
 *
 * current implementation does not support TM_MANY !!
 */
#define TM_ROW          1       /* selectable items are rows */
#define TM_CELL         0       /* selectable items are cells */

#define TM_MANY         2       /* multiple selects possible */
#define TM_SINGLE       0       /* single item selectable at once only */

#define TM_SOLID        0       /* (default) use a solid black for selection*/
#define TM_FOCUS        4       /* use a dotted focus rect for selection */


/* --------- column header structs --------------------------------------*/

/*
 * This struct is sent to request column width and properties -
 * owner window must fill nchars and props.valid, at minimum.
 */
typedef struct {
        int nchars;     /* expected text width in chars */
        Props props;
} ColProps, FAR * lpColProps;


/* This is a set of column requests - owner should fill each one*/
typedef struct {
        DWORD id;               /* caller's id for data */
        int startcol;           /* zero-based column nr of first request */
        int ncols;              /* nr of columns in this set */
        lpColProps plist;       /* ptr to _array_ of ColProps */
} ColPropsList, FAR * lpColPropsList;


/* --- cell data structs ---------------------------------------------*/

/* This is the per-cell data struct.
 * When providing data (responding to TQ_GETDATA), fill out ptext[] and
 * props as appropriate. ptext will be pre-allocated with nchars bytes of
 * space. This may be larger than ColProps->nchars if the user has
 * stretched this column's width on screen
 *
 * Don't re-alloc ptext, or change flags.
 */
typedef struct {
        int nchars;             /* space in buffer */
        LPSTR ptext;            /* ptr to nchars of text space */
        Props props;            /* per-cell props */
        DWORD flags;            /* private table class flags */
} CellData, FAR * lpCellData;

/* List of cell data structures - please fill out all of these*/
typedef struct {
        DWORD id;               /* caller's id for data */
        long row;               /* zero-based row nr to fetch */
        int startcell;          /* zero-based cell nr on this row */
        int ncells;             /* count of cells to fetch */
        lpCellData plist;       /* ptr to array CellData[ncells] */
} CellDataList, FAR * lpCellDataList;


/*----- current selection----------------------------------------------*/

/* Describes the current selection - a rectangular selection area */
typedef struct {
        DWORD id;               /* caller's id for data */
        long startrow;          /* zero-based row nr of start of sel. */
        long startcell;         /* zero-based col nr of  start of sel */
        long nrows;             /* vertical depth of selection */
        long ncells;            /* horz width of selection */
} TableSelection, FAR * lpTableSelection;



/*----- print context -----------------------------------------------*/

/* Describes the margin settings for the print job - these are in CMs*/
typedef struct {
        int left;               /* edge of paper to start of print area */
        int right;              /* edge of paper to start of print area */
        int top;                /* edge of paper to start of hdr */
        int bottom;             /* end of hdr to end of paper */
        int topinner;           /* start of hdr to start of data */
        int bottominner;        /* end of data to start of hdr */
} Margin, FAR * lpMargin;

/* Position and clipping info - only used by table class
 */
typedef struct {
        int start;              /* co-ord of cell start (left or top) */
        int clipstart;          /* start of clipping (vis area) */
        int clipend;            /* end of clipping (vis area) */
        int size;               /* pixel size of cell (width or height) */
} CellPos, FAR * lpCellPos;


/* One of these for each header lines (top and bottom) */
typedef struct {
        CellPos xpos, ypos;     /* private: for table-class use only */
        Props props;
        LPSTR ptext;
} Title, FAR * lpTitle;

/* Print context data structure - any or all 4 pointers may be null */
typedef struct {
        DWORD id;               /* id of table to print */
        lpTitle head;
        lpTitle foot;
        lpMargin margin;
        PRINTDLG FAR * pd;
} PrintContext, FAR * lpPrintContext;


