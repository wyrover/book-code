
/*
 * WINDIFF.H
 */

/* application-wide variables -------------------------------------*/

/* This is the section name in the win.ini file to which we
 * write profile info
 */
#define APPNAME "WinDiff"

/* A gmem_init() heap shared by the app. call gmem_get to alloc. */
extern HANDLE hHeap;

/* The instance handle for this app. Needed by anyone who uses resources
 * such as dialogs
 */
extern HINSTANCE hInst;

extern HWND hwndClient;
extern HWND hwndRCD;

/* global option flags-------------------------------------------  */

/* Which files do we show in outline mode ? all, changed... */
extern int outline_include;

/* Outline_include is an OR of the following */
#define INCLUDE_SAME            1
#define INCLUDE_DIFFER          2
#define INCLUDE_LEFTONLY        4
#define INCLUDE_RIGHTONLY       8


/* Do we ignore blanks during the line-by-line diff ? */
extern BOOL ignore_blanks;

/* Which line numbers do we show - left original, right original or none ?*/
extern int line_numbers;

/* What lines do we show in expand mode - all, left only, right only ? */
extern int expand_mode;

/*--- colour scheme ----------------------------------------------  */

/* Outline */
extern DWORD rgb_outlinehi;

/* Expand view */
extern DWORD rgb_leftfore;
extern DWORD rgb_leftback;
extern DWORD rgb_rightfore;
extern DWORD rgb_rightback;
extern DWORD rgb_mleftfore;
extern DWORD rgb_mleftback;
extern DWORD rgb_mrightfore;
extern DWORD rgb_mrightback;

/* Bar window */
extern DWORD rgb_barleft;
extern DWORD rgb_barright;
extern DWORD rgb_barcurrent;



/* -- display layout constants---------------------------------------*/

/* Percentage of width of window taken by bar display (when visible) */
#define BAR_WIN_WIDTH   10

/* Following are horizontal positions within the bar window, expressed
 * in percent of the width of the bar window
 */
#define L_POS_START     10      /* start of left position marker */
#define L_POS_WIDTH     5       /* width of left position marker */
#define R_POS_START     80      /* start of right position marker */
#define R_POS_WIDTH     5       /* width of right position marker */

#define L_UNMATCH_START 30      /* start of left bar for unmatched section */
#define L_UNMATCH_WIDTH 10      /* width of above */
#define R_UNMATCH_START 60      /* start of right bar for unmatch section */
#define R_UNMATCH_WIDTH 10      /* width of right unmatched section marker */
#define L_MATCH_START   30      /* start of left bar for matched section */
#define L_MATCH_WIDTH   10      /* width of left bar for matched section */
#define R_MATCH_START   60      /* start of right bar for matched section */
#define R_MATCH_WIDTH   10      /* width of right bar for matched section */




/* windiff.c functions */
void windiff_UI(BOOL bAttach);
BOOL Poll(void);                /* true if abort pending */
void SetNames(LPSTR names);
void SetStatus(LPSTR state);

/* in bar.c */
BOOL InitBarClass(HINSTANCE hInstance);
void BarDrawPosition(HWND hwndBar, HDC hdcIn, BOOL bErase);

/*-- private messages -- */
/* Send this to the main window. Return value is the VIEW handle */
#define TM_CURRENTVIEW  WM_USER


/* --- synchronisation ----------------------------------------- */
/*
 * In WIN32 we spawn worker threads to do time-consuming actions.
 * This causes a possible conflict with the UI thread when accessing the
 * BUSY flag.
 *
 * To protect against this we have a critical section. The UI thread
 * will get this before checking/changing the Busy flag,
 * The worker thread will get this before Busy flag* changes.
 *
 */

CRITICAL_SECTION CSWindiff;

#define WDEnter()       EnterCriticalSection(&CSWindiff);
#define WDLeave()       LeaveCriticalSection(&CSWindiff);

