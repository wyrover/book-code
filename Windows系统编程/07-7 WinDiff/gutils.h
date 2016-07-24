

/*
 * GUTILS.H
 */

/* win32 msg crackers */
#define GET_WM_COMMAND_ID(w, l) (LOWORD(w))
#define GET_WM_COMMAND_CMD(w, l) (HIWORD(w))
#define GET_WM_COMMAND_HWND(w, l) (l)
#define GET_SCROLL_OPCODE(w, l)     (LOWORD(w))
#define GET_SCROLL_POS(w, l)        (HIWORD(w))

/* ------- memory allocator ------------------------------------------*/
HANDLE APIENTRY gmem_init(void);
LPSTR APIENTRY gmem_get(HANDLE hHeap, int len);
void APIENTRY gmem_free(HANDLE hHeap, LPSTR ptr, int len);
void APIENTRY gmem_freeall(HANDLE hHeap);


/* --------- date conversion functions    -----------------------*/
void APIENTRY gdate_daytodmy(LONG days,
        int FAR* yrp, int FAR* monthp, int FAR* dayp);
LONG APIENTRY gdate_dmytoday(int yr, int month, int day);
int APIENTRY gdate_monthdays(int month, int year);
int APIENTRY gdate_weekday(long daynr);


/* --- status line window class ---------------------------------- */
/* The status line is a bar across the top or bottom of the window.
 * It can hold a number of fields which can be either static text
 * or buttons.  The so called "static" text can be changed at any time.
 * The fields can be left or right aligned (default is RIGHT).
 * If the text is marked as VAR then the screen real estate allocated
 * for it will be adjusted whenever the text changes.  VAR fields
 * can be given minimum or maximum sizes (but not both).
 *
 * STATIC text fields can be drawn as raised or lowered rectangles (using
 * shades of grey), or (default) without a border. BUTTON fields will
 * always be drawn as raised rectangles, and will lower when pressed.
 *
 * Button fields will send WM_COMMAND messages when clicked including the
 * field id and the WM_LBUTTONUP notification code. Note that that this
 * is not a full implementation of the button class, and no other messages
 * will be sent. In general, none of the fields of a status bar are
 * implemented as separate windows, so GetDlgItem() and similar calls will not
 * work. Buttons only respond to mouse down events, and there is no handling
 * of the focus or of keyboard events.
 *
 * To use:
 *    call StatusAlloc giving the number of items you are going to add to the
 *    status bar. This returns a handle to use in subsequent calls.
 *
 *    Then call StatusAddItem to define each item in turn.
 *    Buttons are placed in order of definition along the bar starting from
 *    the left (SF_LEFT) and from the right (SF_RIGHT) until the two
 *    sides meet.
 *
 *    Call StatusHeight to find the expected height of this status bar, and
 *    set its position within the parent window, then call StatusCreate to
 *    create the window.
 *
 * Having created the window, send SM_SETTEXT messages to set the new
 * text of a field (static or button), or SM_NEW with a handle (obtained from
 * StatusAlloc) to change the contents of the status line.
 */

/* values for type argument to StatusAddItem */
#define SF_BUTTON       1
#define SF_STATIC       2

/* bits in flags argument to StatusAddItem */
#define SF_RAISE        1       /* paint static as raised 3D rectangle */
#define SF_LOWER        2       /* paint static as lowered 3D rectangle */
#define SF_LEFT         4       /* align field on left of status bar */
#define SF_RIGHT        8       /* align field on right (DEFAULT) */
#define SF_VAR          0x10    /* size of field depends on actual text extent*/
#define SF_SZMAX        0x20    /* (with SF_VAR): width argument is maximum */
#define SF_SZMIN        0x40    /* (with SF_VAR) width arg is minimum size */

HWND APIENTRY StatusCreate(HANDLE hInst, HWND hParent, int id,
                LPRECT rcp, HANDLE hmem);
int APIENTRY StatusHeight(HANDLE hmem);
HANDLE APIENTRY StatusAlloc(int nitems);
BOOL APIENTRY StatusAddItem(HANDLE hmem, int itemnr, int type, int flags,
        int id, int width, LPSTR text);

/* send these window messages to the class */

#define SM_NEW          (WM_USER+1)     /* wParam handle for new status line */
#define SM_SETTEXT      (WM_USER+2)     /* wparam: item id, lparam new label*/

void APIENTRY gbit_init(DWORD FAR * map, long nblks);
BOOL APIENTRY gbit_alloc(DWORD FAR * map, long blknr, long nblks);
BOOL APIENTRY gbit_free(DWORD FAR * map, long blknr, long nblks);
long APIENTRY gbit_findfree(DWORD FAR* map, long nblks,
                long mapsize, long FAR * blknr);


/* ----- buffered line input ----------------------------------*/
 /* handle to a file buffer */
typedef struct filebuffer * FILEBUFFER;

FILEBUFFER APIENTRY readfile_new(int fh);
LPSTR APIENTRY readfile_next(FILEBUFFER fb, int FAR * plen);
void APIENTRY readfile_delete(FILEBUFFER fb);

LPTSTR APIENTRY LoadRcString(UINT);
LPTSTR APIENTRY LoadRcString2(UINT);

/* ------ hashing  ------------------------------------------- */
DWORD APIENTRY hash_string(LPSTR string, BOOL bIgnoreBlanks);
BOOL APIENTRY utils_isblank(LPSTR string);
int APIENTRY utils_CompPath(LPSTR left, LPSTR right);

/* --- simple input ------------------------------------------------------*/
int APIENTRY StringInput(LPSTR result, int resultsize, LPSTR prompt,
                         LPSTR caption, LPSTR def_input);

// DBCS friendly versions of string library functions
// These are for both WINDIFF.EXE and GUTILS.DLL.
#define strchr          My_mbschr
#define Old_strncpy	strncpy
#define strncpy         My_mbsncpy
unsigned char * _CRTAPI1 My_mbschr(unsigned char *, unsigned short);
unsigned char * _CRTAPI1 My_mbsncpy(
                unsigned char *, const unsigned char *, size_t);

// These are for WINDIFF.EXE.
#define strrchr         My_mbsrchr
#define strncmp         My_mbsncmp
unsigned char * _CRTAPI1 My_mbsrchr(unsigned char *, unsigned short);
int _CRTAPI1 My_mbsncmp(const unsigned char *, const unsigned char *, size_t);
