

/****************************** Module Header *******************************
* Module Name: WINDIFF.C
*
* File and directory comparisions.
*
* Functions:
*
* windiff_UI()
* WinMain()
* windiff_usage()
* Poll()
* DoResize()
* AboutBox()
* DoPrint()
* FindNextChange()
* FindPrevChange()
* WriteProfileInt()
* ToOutline()
* ToMoved()
* do_editfile()
* do_editthread()
* SetStatus()
* SetNames()
* IsBusy()
* BusyError()
* StateToColour()
* SetSelection()
* do_gethdr()
* do_getprops()
* do_getdata()
* SvrClose()
* TableServer()
* wd_dirdialog()
* wd_copy()
* InitApplication()
* InitInstance()
* CreateTools()
* DeleteTools()
* MainWndProc()
* SetBusy()
* SetNotBusy()
* SetSelection()
* SetButtonText()
* ToExpand()
* ParseArgs()
* wd_initial()
*
* Comments:
*
* Compare two directories (including all files and subdirs). Look for names
* that are present in both (report all that are not). For files that
* are present in both, produce a line-by-line comparison of the differences
* between the two files (if any).
*
* Overview of Windiff internals - the whole program.
*
* Windiff is built from several modules (a "module" has a .h file
* which describes its interface and a .c file which implements it).
* Apart from THIS comment which tries to give an overview of the whole
* scheme of things, each module is as self-contained as possible.
* This is enforced by the use of opaque data types.  Modules cannot
* see each others' internal data structures.  Modules are abstract
* data types.  The term "Module" (from Modula2) and "Class" (from C++)
* are used synonymously.
*
*    Windiff  - main program - parse arguments, put up main window,
*               handle input, calling other modules as needed
*               invoke table class to create the main display and
*               service callbacks from the table class.
*               Contains global flags for options (e.g. ignore_blanks)
*    list     - (in gutils) a generalised LIST of anything data type
*               has full set of operations for insert, delete, join etc.
*    line     - a LINE is a numbered line of text.  Information is kept to
*               allow fast comparisons of LINEs.  A LINE can hold a
*               link to another LINE.  The links are used to connect
*               lines in one file to matching lines in the other file.
*    file     - a FILEDATA represents a file as a file name in the form
*               of a DIRITEM and a LIST of LINEs
*    scandir  - a DIRITEM represents information about a file.  (for
*               instance its name, whether it has a local copy).
*    compitem - a COMPITEM is a pair of files together with information
*               on how they compare in the form of a breakdown of the
*               files into a LIST of matching or non-matching sections.
*               Either file can be absent.  This module contains the
*               file "contrast" algorithm used for the actual comparison
*    tree       (in gutils) A binary tree.  Important because it is what
*               gives the file comparison its speed as it makes it
*               an "N log N" algorithm rather than "N squared"
*    complist - a COMPLIST is the master data structure.  It has a DIRLIST
*               of the left hand files, a DIRLIST of the right hand files
*               and a LIST of COMPITEMs. The left and right hand DIRLISTs
*               are working data used to produce the COMPLIST.  The LIST
*               is displayed as the outline table.  Any given COMPITEM can
*               be displayed as an expanded item.
*    section  - a SECTION is a section of a file (first line, last line)
*               and information as to what it matches in the other file.
*    bar.c    - the picture down the left of the screen
*               has a WNDPROC.  
*    view     - Although the COMPLIST is the master state, it doesn't do
*               all the work itself.  The data is actually displayed by
*               the table class which is highly generalised.  View
*               owns a COMPLIST (and therefore calls upon the functions
*               in complist to fill it and interrogate it) and calls
*               upon (and is called back by) the functions in table to
*               actually display it.  Read about table in gutils.h
*    table.c    (in gutils) a highly generalised system for displaying
*               data in rows and columns.  The interface is in gutils.h.
*    status.c   (in gutils) the status line at the top. See gutils.h
*************************************************************************
*
* Overview of this file:
*
*   We create a table window (gutils.dll) to show the files and the
*   results of their comparisons. We create a COMPLIST object representing
*   a list of files and their differences, and a VIEW object to map between
*   the rows of the table window and the COMPLIST.
*
*   This module is responsible for creating and managing the main window,
*   placing the child windows (table, status window etc) within it, and
*   handling all menu items. We maintain global option flags set by
*   menu commands.
*
*   Creating a COMPLIST creates a list of unmatched files, and of matching
*   files that are compared with each other (these are COMPITEMS).
*   The VIEW provides a mapping between rows on the screen, and items in
*   the COMPLIST.
*
*   This version tries to maintain a responsive user interface by
*   creating worker threads to do long jobs.  This potentially creates
*   conflicts between the threads as they will both want to update common
*   variables (for instance the UI thread may be changing the options to
*   exclude identical files while the worker thread is adding in the
*   results of new comparisons).  Critical sections are used to manage
*   the conflicts.
*
*   The Edit options invoke an editor on a separate thread.  This allows
*   us to repaint our window and thereby allow the user to refer back to
*   what he saw before invoking the editor.  When he's finished editing,
*   we would of course like to refresh things and if this is still on the
*   separate thread it might clash. We avoid this clash by POSTing ourselves
*   a (WM_COMMAND, IDM_UPDATE) message.
*
****************************************************************************/

#include <windows.h>
#include <shellapi.h>
#include <stdlib.h>
#include <commdlg.h>
#include <string.h>

#include "gutils.h"
#include "table.h"
#include "list.h"
#include "scandir.h"            /* needed for file.h     */
#include "file.h"               /* needed for compitem.h */
#include "compitem.h"           /* needed for view.h     */
#include "complist.h"
#include "view.h"
#include "state.h"
#include "windiff.h"
#include "wdiffrc.h"


/*--constants and data types--------------------------------------------*/

int Version = 2;
int SubVersion = 01;

/* When we print the current table, we pass this id as the table id
 * When we are queried for the properties of this table, we know they
 * want the printing properties for the current view. We use this to
 * select different fonts and colours for the printer.
 */
#define TABID_PRINTER   1



/*
 * structure containing args passed to worker thread in initial
 * case (executing command line instructions). 
 */
typedef struct {

        LPSTR first;
        LPSTR second;
        LPSTR savelist;
        UINT saveopts;
        VIEW view;
        BOOL fDeep;
} THREADARGS, FAR * PTHREADARGS;


/* Structure containing all the arguments we'd like to give to do_editfile
   Need a structure because CreateThread only allows for one argument.
*/
typedef struct {
        VIEW view;
        int option;
        int selection;
} EDITARGS, FAR * PEDITARGS;

/*---- colour scheme------------------------------- */

/* outline */
DWORD rgb_outlinehi = RGB(255, 0, 0);   /* hilighted files in outline mode  */

/* expand view */
DWORD rgb_leftfore =   RGB(  0,   0,   0);         /* foregrnd for left lines */
DWORD rgb_leftback  =  RGB(255,   0,   0);         /* backgrnd for left lines */
DWORD rgb_rightfore =  RGB(  0,   0,   0);         /* foregrnd for right lines*/
DWORD rgb_rightback =  RGB(255, 255,   0);         /* backgrnd for right lines*/

/* moved lines */
DWORD rgb_mleftfore =  RGB(  0,   0, 128);         /* foregrnd for moved-left */
DWORD rgb_mleftback =  RGB(255,   0,   0);         /* backgrnd for moved-left */
DWORD rgb_mrightfore = RGB(  0,   0, 255);         /* foregrnd for moved-right*/
DWORD rgb_mrightback = RGB(255, 255,   0);         /* backgrnd for moved-right*/

/* bar window */
DWORD rgb_barleft =    RGB(255,   0,   0);         /* bar sections in left only  */
DWORD rgb_barright =   RGB(255, 255,   0);         /* bar sections in right only */
DWORD rgb_barcurrent = RGB(  0,   0, 255);         /* current pos markers in bar */


/* module static data -------------------------------------------------*/


/* current value of window title */
char AppTitle[256];


HWND hwndClient;        /* main window */
HWND hwndRCD;           /* table window */
HWND hwndStatus;        /* status bar across top */
HWND hwndBar;           /* graphic of sections as vertical bars */

HACCEL haccel;

/* The status bar told us it should be this high. Rest of client area
 * goes to the hwndBar and hwndRCD.
 */
int status_height;

HINSTANCE hInst;   /* handle to current app instance */
HMENU hMenu;    /* handle to menu for hwndClient */

int nMinMax = SW_SHOWNORMAL;         /* default state of window normal */

/* The message sent to us as a callback by the table window needs to be
 * registered - table_msgcode is the result of the RegisterMessage call
 */
UINT table_msgcode;

/* True if we are currently doing some scan or comparison.
 * Must get critical section before checking/changing this (call
 * SetBusy.
 */
BOOL fBusy = FALSE;

int     selection       =       -1;     /* selected row in table*/

/* Options for DisplayMode field indicating what is currently shown.
 * We use this to know whether or not to show the graphic bar window.
 */
#define MODE_NULL       0       /* nothing displayed */
#define MODE_OUTLINE    1       /* a list of files displayed */
#define MODE_EXPAND     2       /* view is expanded view of one file */

int DisplayMode = MODE_NULL;    /* indicates whether we are in expand mode */

VIEW current_view = NULL;

/* command line parameters */
extern int __argc;
extern char ** __argv;

BOOL bAbort = FALSE;    /* set to request abort of current operation */

char editor_cmdline[256] = "notepad %p";  /* editor cmdline */
                          /* slick version is "s %p -#%l" */

/* app-wide global data --------------------------------------------- */

/* Handle returned from gmem_init - we use this for all memory allocations */
HANDLE hHeap;

/* Current state of menu options */
int line_numbers = IDM_LNRS;
int expand_mode = IDM_BOTHFILES;
int outline_include = INCLUDE_LEFTONLY|INCLUDE_RIGHTONLY|INCLUDE_SAME|INCLUDE_DIFFER;
BOOL ignore_blanks = TRUE;
BOOL picture_mode = TRUE;

/* function prototypes ---------------------------------------------*/

BOOL InitApplication(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
void CreateTools(void);
void DeleteTools(void);
long APIENTRY MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);
BOOL SetBusy(void);
void SetNotBusy(void);
void SetSelection(long rownr);
void SetButtonText(LPSTR cmd);
BOOL ToExpand(HWND hwnd);
void ParseArgs(int argc, char ** argv);


DWORD wd_initial(LPVOID arg);

static HANDLE ghThread = NULL;

static DWORD gdwMainThreadId;     /* threadid of main (user interface) thread
                                     initialised in winmain(), thereafter constant.
                                     See windiff_UI()
                                  */

/***************************************************************************
 * Function: windiff_UI
 *
 * Purpose:
 *
 * If you are about to put up a dialog box or in fact process input in any way
 * on any thread other than the main thread - or if you MIGHT be on a thread other
 * than the main thread, then you must call this function with TRUE before doing
 * it and with FALSE immediately afterwards.  Otherwise you will get one of a
 * number of flavours of not-very-responsiveness
 */
void windiff_UI(BOOL bAttach)
{
        DWORD dwThreadId = GetCurrentThreadId();
        if (dwThreadId==gdwMainThreadId) return;

        if (bAttach) GetDesktopWindow();
        AttachThreadInput(dwThreadId, gdwMainThreadId, bAttach);
} /* windiff_UI */

/***************************************************************************
 * Function: WinMain
 *
 * Purpose:
 *
 * Main entry point. Register window classes, create windows,
 * parse command line arguments and then perform a message loop
 */
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

        MSG msg;

        gdwMainThreadId = GetCurrentThreadId();

        /* create any pens/brushes etc and read in profile defaults */
        CreateTools();

        /* init window class unless other instances running */
        if (!hPrevInstance)
            if (!InitApplication(hInstance))
                return(FALSE);


        /* init this instance - create all the windows */
        if (!InitInstance(hInstance, nCmdShow))
            return(FALSE);

        ParseArgs(__argc, __argv);


        /* message loop */
        while(GetMessage(&msg, NULL, 0, 0)) {
                if (!TranslateAccelerator(hwndClient, haccel, &msg)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
        }

        return (msg.wParam);

}

/***************************************************************************
 * Function: InitApplication
 *
 * Purpose:
 *
 * Register window class for the main window and the bar window.
 */
BOOL
InitApplication(HINSTANCE hInstance)
{
        WNDCLASS    wc;
        BOOL resp;


        /* register the bar window class */
        InitBarClass(hInstance);

        wc.style = 0;
        wc.lpfnWndProc = MainWndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(hInstance, "WinDiff");
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszClassName =  "WinDiffViewerClass";
        wc.lpszMenuName = NULL;

        resp = RegisterClass(&wc);

        return(resp);
}

/***************************************************************************
 * Function: InitInstance
 *
 * Purpose:
 *
 * Create and show the windows
 */
BOOL
InitInstance(HINSTANCE hInstance, int nCmdShow)
{
        RECT rect;
        HANDLE hstatus;
        int bar_width;
        RECT childrc;

        hInst = hInstance;

        /* initialise a heap. we use this one heap throughout
         * the app. for all memory requirements
         */
        hHeap = gmem_init();
        /* initialise the list package */
        List_Init();


        hMenu = LoadMenu(hInstance, "WinDiffMenu");
        haccel = LoadAccelerators(hInstance, "WinDiffAccel");

        /* create the main window */
        hwndClient = CreateWindow("WinDiffViewerClass",
                            "WinDiff",
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            NULL,
                            hMenu,
                            hInstance,
                            NULL
                );



        if (!hwndClient) {
            return(FALSE);
        }

        /* create 3 child windows, one status, one table and one bar
         * Initially, the bar window is hidden and covered by the table.
         */

        /* create a status bar window as
         * a child of the main window.
         */

        /* build a status struct for two labels and an abort button */
        hstatus = StatusAlloc(3);
        StatusAddItem(hstatus, 0, SF_STATIC, SF_LEFT|SF_VAR|SF_SZMIN, IDL_STATLAB, 14, NULL);
        StatusAddItem(hstatus, 1, SF_BUTTON, SF_RIGHT|SF_RAISE, IDM_ABORT, 8,
                LoadRcString(IDS_EXIT));
        StatusAddItem(hstatus, 2, SF_STATIC, SF_LOWER|SF_LEFT|SF_VAR,
                        IDL_NAMES, 60, NULL);

        /* ask the status bar how high it should be for the controls
         * we have chosen, and save this value for re-sizing.
         */
        status_height = StatusHeight(hstatus);

        /* create a window of this height */
        GetClientRect(hwndClient, &rect);
        childrc = rect;
        childrc.bottom = status_height;
        hwndStatus = StatusCreate(hInst, hwndClient, IDC_STATUS, &childrc,
                        hstatus);

        /* layout constants are stated as percentages of the window width */
        bar_width = (rect.right - rect.left) * BAR_WIN_WIDTH / 100;

        /* create the table class covering all the remaining part of
         * the main window
         */
        hwndRCD = CreateWindow(TableClassName,
                        NULL,
                        WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
                        0,
                        status_height,
                        (int)(rect.right - rect.left),
                        (int)(rect.bottom - status_height),
                        hwndClient,
                        (HANDLE) IDC_RCDISP1,
                        hInst,
                        NULL);

        /* create a bar window as a child of the main window.
         * this window remains hidden until we switch into MODE_EXPAND
         */
        hwndBar = CreateWindow("BarClass",
                        NULL,
                        WS_CHILD | WS_VISIBLE,
                        0,
                        status_height,
                        bar_width,
                        (int)(rect.bottom - status_height),
                        hwndClient,
                        (HANDLE) IDC_BAR,
                        hInst,
                        NULL);

        /* nMinMax indicates whether we are to be minimised on startup,
         * on command line parameters
         */
        ShowWindow(hwndBar, SW_HIDE);

        if (GetProfileInt(APPNAME, "OutlineSaved", 0))
        {
                WINDOWPLACEMENT wp;

                /* restore the previous expanded size and position */
                wp.length                  = sizeof( WINDOWPLACEMENT );
                wp.flags                   = 0;
                wp.showCmd                 = GetProfileInt( APPNAME, "OutlineShowCmd",
                                                            SW_SHOWNORMAL);
                wp.ptMaxPosition.x         = GetProfileInt( APPNAME, "OutlineMaxX",       0);
                wp.ptMaxPosition.y         = GetProfileInt( APPNAME, "OutlineMaxY",       0);
                wp.rcNormalPosition.left   = (int)GetProfileInt( APPNAME, "OutlineNormLeft",  (UINT)(-1));
                wp.rcNormalPosition.top    = (int)GetProfileInt( APPNAME, "OutlineNormTop",   (UINT)(-1));
                wp.rcNormalPosition.right  = (int)GetProfileInt( APPNAME, "OutlineNormRight", (UINT)(-1));
                wp.rcNormalPosition.bottom = (int)GetProfileInt( APPNAME, "OutlineNormBottom",(UINT)(-1));
                SetWindowPlacement(hwndClient,&wp);
        }
        else ShowWindow(hwndClient, nMinMax);

        UpdateWindow(hwndClient);


        /* initialise busy flag and status line to show we are idle
         * (ie not comparing or scanning)
         */
        SetNotBusy();

        return(TRUE);

} /* InitInstance */

/***************************************************************************
 * Function: windiff_usage
 *
 * Purpose:
 *
 * Complain to command line users about poor syntax,
 * will be replaced by proper help file.
 */
void
windiff_usage(LPSTR msg)
{
        int retval;
        TCHAR szBuf[MAX_PATH];

        if (msg==NULL)
                msg = LoadRcString(IDS_USAGE_STR);

        LoadString(hInst, IDS_WINDIFF_USAGE, szBuf, sizeof(szBuf));
        retval = MessageBox(hwndClient,
                msg,
                szBuf, MB_ICONINFORMATION|MB_OKCANCEL);
        if (retval == IDCANCEL) {
                exit(1);
        }
}

/***************************************************************************
 * Function: ParseArgs
 *
 * Purpose:
 *
 * Parse command line arguments
 *
 * The user can give one or two paths. If only one, we assume the second
 * is '.' for the current directory. If one of the two paths is a directory
 * and the other a file, we compare a file of the same name in the two dirs.
 *
 * The command -s filename causes the outline list to be written to a file
 * and then the program exits. -s{slrd} filename allows selection of which
 * files are written out; by default, we assume -sld for files left and different.
 *
 * -T means tree.  Go deep.
 *
 * The default is Deep, -L overrides and implies shallow, -T overrides -L
 */
void
ParseArgs(int argc, char ** argv)
{
        int i;
        LPSTR chp;
        PTHREADARGS ta;
        DWORD threadid;

        /* thread args can't be on the stack since the stack will change
         * before the thread completes execution
         */
        ta = (PTHREADARGS) gmem_get(hHeap, sizeof(THREADARGS));
        ta->first = NULL;
        ta->second = NULL;
        ta->savelist = NULL;
        ta->saveopts = 0;
        ta->fDeep = FALSE;  /* No -T option seen yet */

        for (i = 1; i < argc; i++) {

                /* is this an option ? */
                if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
                        switch(argv[i][1]) {

                        case 's':
                        case 'S':
                                /* read letters for the save option: s,l,r,d */
                                for(chp = &argv[i][2]; *chp != '\0'; chp++) {
                                        switch(*chp) {
                                        case 's':
                                        case 'S':
                                                ta->saveopts |= INCLUDE_SAME;
                                                break;
                                        case 'l':
                                        case 'L':
                                                ta->saveopts |= INCLUDE_LEFTONLY;
                                                break;
                                        case 'r':
                                        case 'R':
                                                ta->saveopts |= INCLUDE_RIGHTONLY;
                                                break;
                                        case 'd':
                                        case 'D':
                                                ta->saveopts |= INCLUDE_DIFFER;
                                                break;
                                        default:
                                                windiff_usage(NULL);
                                                return;
                                        }
                                }

                                if (ta->saveopts == 0) {
                                        /* default to left and differ */
                                        ta->saveopts = (INCLUDE_LEFTONLY) | (INCLUDE_DIFFER);
                                }
                                ta->savelist = argv[++i];
                                break;
                        case 't':
                        case 'T':
                                ta->fDeep = TRUE;
                                break;
                        default:
                                windiff_usage(NULL);
                                return;
                        }
                } else {
                        if (ta->first == NULL) {
                                ta->first = argv[i];
                        } else {
                                ta->second = argv[i];
                        }
                }
        }

        /* set the correct depth */
        if (ta->fDeep)
                ;                       /* explicitly set -- leave it alone */
        else ta->fDeep = TRUE;          /* global default */

        /* any paths to scan ? */
        if (ta->first == NULL) {
                return;
        }

        if (ta->second == NULL) {
                ta->second = ".";
        }

        SetBusy();

        /* minimise the window if -s flag given */
        if (ta->savelist != NULL) {
                ShowWindow(hwndClient, SW_MINIMIZE);
        }

        /* make an empty view */
        current_view = view_new(hwndRCD);
        DisplayMode = MODE_OUTLINE;

        ta->view = current_view;

        /* attempt to create a worker thread */

        ghThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wd_initial, (LPVOID) ta,
                        0, &threadid);
        if (ghThread == NULL)
        {
                wd_initial( (LPVOID) ta);
        }
} /* ParseArgs */


/***************************************************************************
 * Function: CreateTools
 *
 * Purpose:
 *
 * Create any pens/brushes, and read defaults
 * from the profile file for menu settings etc.
 */
void
CreateTools(void)
{

        /* standard message that table class sends us for
         * notifications and queries.
         */
        table_msgcode = RegisterWindowMessage(TableMessage);

        line_numbers = GetProfileInt(APPNAME, "LineNumbers", line_numbers);
        outline_include = GetProfileInt(APPNAME, "FileInclude", outline_include);
        ignore_blanks = GetProfileInt(APPNAME, "Blanks", ignore_blanks);
        picture_mode = GetProfileInt(APPNAME, "Picture", picture_mode);

        GetProfileString(APPNAME, "Editor", editor_cmdline, (LPTSTR)editor_cmdline,
                        sizeof(editor_cmdline));
        InitializeCriticalSection(&CSWindiff);

}

/***************************************************************************
 * Function: DeleteTools
 *
 * Purpose:
 *
 * Delete any pens or brushes that were created in CreateTools 
 */
void
DeleteTools(void)
{

        DeleteCriticalSection(&CSWindiff);

}


/***************************************************************************
 * Function:
 *
 * Purpose:
 *
 * Check whether we have had an abort request (IDM_ABORT), and
 * return TRUE if abort requested, otherwise FALSE
 */
BOOL
Poll(void)
{
    return(bAbort);
}


/***************************************************************************
 * Function: DoResize
 *
 * Purpose:
 *
 * Position child windows on a resize of the main window
 */
void
DoResize(HWND hWnd)
{
        RECT rc;
        int bar_width;

        GetClientRect(hWnd, &rc);
        MoveWindow(hwndStatus, 0, 0, rc.right - rc.left, status_height, TRUE);

        bar_width = (rc.right - rc.left) * BAR_WIN_WIDTH / 100;

        /* bar window is hidden unless in expand mode */
        if ((DisplayMode == MODE_EXPAND) && (picture_mode)) {
                ShowWindow(hwndBar, SW_SHOW);
                MoveWindow(hwndBar, 0, status_height,
                        bar_width, rc.bottom - status_height, TRUE);
                MoveWindow(hwndRCD, bar_width, status_height,
                        (rc.right - rc.left) - bar_width,
                        rc.bottom - status_height, TRUE);
        } else {
                MoveWindow(hwndRCD, 0, status_height, (rc.right - rc.left),
                        rc.bottom - status_height, TRUE);
                ShowWindow(hwndBar, SW_HIDE);
        }

}
/***************************************************************************
 * Function: AboutBox
 *
 * Purpose:
 *
 * Standard processing for About box.
 */
int APIENTRY
AboutBox(HWND hDlg, unsigned message, UINT wParam, LONG lParam)
{
        char ch[256];

        switch (message) {

        case WM_INITDIALOG:
                wsprintf((LPTSTR)ch, "%d.%02d", Version, SubVersion);
                SetDlgItemText(hDlg, IDD_VERSION, ch);
                return(TRUE);

        case WM_COMMAND:
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDOK:
                        EndDialog(hDlg, 0);
                        return(TRUE);
                }
                break;
        }
        return(FALSE);
}


/* -- menu commands ---------------------------------------------------*/

/***************************************************************************
 * Function: DoPrint
 *
 * Purpose:
 *
 * Print the current view 
 */
void
DoPrint(void)
{
        Title head, foot;
        PrintContext context;
        TCHAR szPage[50];

        /* print context contains the header and footer. Use the
         * default margins and printer selection
         */

        /* we set the table id to be TABID_PRINTER. When the table calls
         * back to get text and properties, we use this to indicate
         * that the table refered to is the 'current_view', but in print
         * mode, and thus we will use different colours/fonts.
         */
        context.head = &head;
        context.foot = &foot;
        context.margin = NULL;
        context.pd = NULL;
        context.id = TABID_PRINTER;

        /* header is filenames or just WinDiff if no names known*/
        if (strlen(AppTitle) > 0) {
                head.ptext = AppTitle;
        } else {
                head.ptext = "WinDiff";
        }

        /* header is centred, footer is right-aligned and
         * consists of the page number
         */
        head.props.valid = P_ALIGN;
        head.props.alignment = P_CENTRE;
        lstrcpy(szPage,LoadRcString(IDS_PAGE));
        foot.ptext = (LPSTR)szPage;
        foot.props.valid = P_ALIGN;
        foot.props.alignment = P_RIGHT;

        SendMessage(hwndRCD, TM_PRINT, 0, (DWORD) (LPSTR) &context);
}

/***************************************************************************
 * Function: FindNextChange
 *
 * Purpose:
 *
 * Find the next line in the current view that is
 * not STATE_SAME. Start from the current selection, if valid, or
 * from the top of the window if no selection.
 *
 */
BOOL
FindNextChange(void)
{
        long row;

        /* start from the selection or top of the window if no selection */
        if (selection >= 0) {
                row = selection;
        } else {
                row = (int) SendMessage(hwndRCD, TM_TOPROW, FALSE, 0);
        }


        /* find the next 'interesting' line */
        row = view_findchange(current_view, row, TRUE);
        if (row >= 0) {
                SetSelection(row);
                return(TRUE);
        } else {
                windiff_UI(TRUE);
                MessageBox(hwndClient, LoadRcString(IDS_NO_MORE_CHANGES), "Windiff",
                        MB_ICONINFORMATION|MB_OK);
                windiff_UI(FALSE);

                return(FALSE);
        }
}

/***************************************************************************
 * Function: FindPrevChange
 *
 * Purpose:
 *
 * Find the previous line in the current view that is not STATE_SAME
 */
BOOL
FindPrevChange(void)
{
        long row;

        /* start from the selection or top of window if no selection */
        if (selection >= 0) {
                row = selection;
        } else {
                row = (int) SendMessage(hwndRCD, TM_TOPROW, FALSE, 0);
        }

        /* find the previous 'interesting' line */
        row = view_findchange(current_view, row, FALSE);
        if (row >= 0) {
                SetSelection(row);
                return(TRUE);
        } else {
                windiff_UI(TRUE);
                MessageBox(hwndClient, LoadRcString(IDS_NO_PREV_CHANGES), "Windiff",
                        MB_ICONINFORMATION|MB_OK);
                windiff_UI(FALSE);

                return(FALSE);
        }

}
/***************************************************************************
 * Function: WriteProfileInt
 *
 */
 
BOOL WriteProfileInt(LPSTR AppName, LPSTR Key, int Int)
{       char Str[40];

        wsprintf((LPTSTR)Str, "%d", Int);
        return WriteProfileString(AppName, Key, Str);

} /* WriteProfileInt */


/***************************************************************************
 * Function: ToExpand
 *
 * Purpose:
 *
 * Switch to expand view of the selected line 
 */
BOOL
ToExpand(HWND hwnd)
{
        if (selection < 0) {
                return(FALSE);
        }

        if (!view_isexpanded(current_view)) {
                /* save the current outline size and position */
                WINDOWPLACEMENT wp;
                if (GetWindowPlacement(hwndClient,&wp)) {
                        WriteProfileInt(APPNAME, "OutlineShowCmd", wp.showCmd);
                        WriteProfileInt(APPNAME, "OutlineMaxX", wp.ptMaxPosition.x);
                        WriteProfileInt(APPNAME, "OutlineMaxY", wp.ptMaxPosition.y);
                        WriteProfileInt(APPNAME, "OutlineNormLeft", wp.rcNormalPosition.left);
                        WriteProfileInt(APPNAME, "OutlineNormTop", wp.rcNormalPosition.top);
                        WriteProfileInt(APPNAME, "OutlineNormRight", wp.rcNormalPosition.right);
                        WriteProfileInt(APPNAME, "OutlineNormBottom", wp.rcNormalPosition.bottom);
                        WriteProfileInt(APPNAME, "OutlineSaved", 1);
                }

                /* restore the previous expanded size and position, if any */
                if (GetProfileInt(APPNAME, "ExpandedSaved", 0)) {
                        wp.flags                   = 0;
                        wp.showCmd
                                = GetProfileInt( APPNAME, "ExpandShowCmd"
                                               , SW_SHOWMAXIMIZED);
                        wp.ptMaxPosition.x
                                = GetProfileInt( APPNAME, "ExpandMaxX", 0);
                        wp.ptMaxPosition.y
                                = GetProfileInt( APPNAME, "ExpandMaxY", 0);
                        wp.rcNormalPosition.left
                                = GetProfileInt( APPNAME, "ExpandNormLeft"
                                               , wp.rcNormalPosition.left);
                        wp.rcNormalPosition.top
                                = GetProfileInt( APPNAME, "ExpandNormTop"
                                               , wp.rcNormalPosition.top);
                        wp.rcNormalPosition.right
                                = GetProfileInt( APPNAME, "ExpandNormRight"
                                               , wp.rcNormalPosition.right);
                        wp.rcNormalPosition.bottom
                                = GetProfileInt( APPNAME, "ExpandNormBottom"
                                               , wp.rcNormalPosition.bottom);
                        SetWindowPlacement(hwndClient,&wp);
                }
                else ShowWindow(hwndClient, SW_SHOWMAXIMIZED);
        }

        /*change the view mapping to expand mode */
        if (view_expand(current_view, selection)) {

                /* ok - we now have an expanded view - change status
                 * to show this
                 */

                DisplayMode = MODE_EXPAND;

                /* resize to show the graphic bar picture */
                DoResize(hwndClient);


                /* change button,status text-if we are not still busy*/
                if (!fBusy) {
                        TCHAR szBuf[10];
                        lstrcpy(szBuf,LoadRcString(IDS_OUTLINE));
                        /* the status field when we are expanded shows the
                         * tag field (normally the file name) for the
                         * item we are expanding
                         */
                        SetStatus(view_getcurrenttag(current_view) );
                        SetButtonText(szBuf);
                }

                return(TRUE);
        }
        return(FALSE);
} /* ToExpand */

/***************************************************************************
 * Function: ToOutline
 *
 * Purpose:
 *
 * Switch back to outline view - showing just the list of file names.
 */
void
ToOutline(HWND hwnd)
{
        if (view_isexpanded(current_view)) {
                /* save the current expanded size and position */
                WINDOWPLACEMENT wp;
                if (GetWindowPlacement(hwndClient,&wp)) {
                        WriteProfileInt(APPNAME, "ExpandShowCmd", wp.showCmd);
                        WriteProfileInt(APPNAME, "ExpandMaxX", wp.ptMaxPosition.x);
                        WriteProfileInt(APPNAME, "ExpandMaxY", wp.ptMaxPosition.y);
                        WriteProfileInt(APPNAME, "ExpandNormLeft", wp.rcNormalPosition.left);
                        WriteProfileInt(APPNAME, "ExpandNormTop", wp.rcNormalPosition.top);
                        WriteProfileInt(APPNAME, "ExpandNormRight", wp.rcNormalPosition.right);
                        WriteProfileInt(APPNAME, "ExpandNormBottom", wp.rcNormalPosition.bottom);
                        WriteProfileInt(APPNAME, "ExpandedSaved", 1);
                }

                /* restore the previous expanded size and position, if any */
                if (GetProfileInt(APPNAME, "OutlineSaved", 0))  {
                        wp.flags = 0;
                        wp.showCmd
                                = GetProfileInt( APPNAME, "OutlineShowCmd"
                                               , SW_SHOWNORMAL);
                        wp.ptMaxPosition.x
                                = GetProfileInt( APPNAME, "OutlineMaxX", 0);
                        wp.ptMaxPosition.y
                                = GetProfileInt( APPNAME, "OutlineMaxY", 0);
                        wp.rcNormalPosition.left
                                = GetProfileInt( APPNAME, "OutlineNormLeft"
                                               , wp.rcNormalPosition.left);
                        wp.rcNormalPosition.top
                                = GetProfileInt( APPNAME, "OutlineNormTop"
                                               , wp.rcNormalPosition.top);
                        wp.rcNormalPosition.right
                                = GetProfileInt( APPNAME, "OutlineNormRight"
                                               , wp.rcNormalPosition.right);
                        wp.rcNormalPosition.bottom
                                = GetProfileInt( APPNAME, "OutlineNormBottom"
                                               , wp.rcNormalPosition.bottom);
                        SetWindowPlacement(hwndClient,&wp);
                }
                ShowWindow(hwndClient, SW_SHOWNORMAL);
        }

        DisplayMode = MODE_OUTLINE;

        /* switch mapping back to outline view */
        view_outline(current_view);

        /* hide bar window and resize to cover */
        DoResize(hwndClient);


        /* change label on button */
        if (!fBusy) {
                TCHAR szBuf[8];
                lstrcpy(szBuf,LoadRcString(IDS_EXPAND));
                SetButtonText(szBuf);
                SetStatus(NULL);
        }
} /* ToOutline */

/***************************************************************************
 * Function: ToMoved
 *
 * Purpose:
 *
 * If the user clicks on a MOVED line in expand mode, we jump to the
 * other line. We return TRUE if this was possible,  or FALSE otherwise.
 */
BOOL
ToMoved(HWND hwnd)
{
        BOOL bIsLeft;
        int linenr, state;
        long i, total;

        if (DisplayMode != MODE_EXPAND) {
                return(FALSE);
        }
        if (selection < 0) {
                return(FALSE);
        }

        state = view_getstate(current_view, selection);
        if (state == STATE_MOVEDLEFT) {
                bIsLeft = TRUE;
                /* get the linenr of the other copy */
                linenr = abs(view_getlinenr_right(current_view, selection));
        } else if (state == STATE_MOVEDRIGHT) {
                bIsLeft = FALSE;
                /* get the linenr of the other copy */
                linenr = abs(view_getlinenr_left(current_view, selection));
        } else {
                /* not a moved line - so we can't find another copy */
                return(FALSE);
        }

        /* search the view for this line nr */
        total = view_getrowcount(current_view);
        for (i = 0; i < total; i++) {
                if (bIsLeft) {
                        if (linenr == view_getlinenr_right(current_view, i)) {
                                /* found it */
                                SetSelection(i);
                                return(TRUE);
                        }
                } else {
                        if (linenr == view_getlinenr_left(current_view, i)) {
                                SetSelection(i);
                                return(TRUE);
                        }
                }
        }
        return(FALSE);
}

/***************************************************************************
 * Function: do_editfile
 *
 * Purpose:
 *
 * Launch an editor on the current file (the file we are expanding, or
 * in outline mode the selected row. Option allows selection of the
 * left file, the right file or the composite view of this item.
 * pe points to a packet of parameters that must be freed before returning.
 * The return value is meaningless (just to conform to CreateThread).
 */
LONG
do_editfile(PEDITARGS pe)
{
        VIEW view = pe->view;
        int option = pe->option;
        int selection = pe->selection;

        COMPITEM item;
        LPSTR fname;
        char cmdline[256];
        int currentline;
        char * pOut = cmdline;
        char * pIn = editor_cmdline;

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        item = view_getitem(view, selection);
        if (item == NULL) {
                return -1;
        }

        fname = compitem_getfilename(item, option);

        if ( 0 == fname )
        {
            windiff_UI(TRUE);
            MessageBox(hwndClient, LoadRcString(IDS_FILE_DOESNT_EXIST),
                       "Windiff", MB_ICONSTOP|MB_OK);
            windiff_UI(FALSE);
            goto error;
        }

       switch ( option )
        {
        case CI_LEFT:
            currentline = view_getlinenr_left( view,
                                               selection > 0 ? selection : 1);
            break;

        case CI_RIGHT:
            currentline = view_getlinenr_right( view,
                                                selection > 0 ? selection : 1);
            break;

        default:
            currentline = 1;
            break;
        }

        while( *pIn )
        {
            switch( *pIn )
            {
            case '%':
                pIn++;
                switch ( *pIn )
                {
                case 'p':
                    lstrcpy( (LPTSTR)pOut, fname );
                    while ( *pOut )
                        pOut++;
                    break;

                case 'l':
                    _ltoa( currentline, pOut, 10 );
                    while ( *pOut )
                        pOut++;
                    break;

                default:
                    if (IsDBCSLeadByte(*pIn) && *(pIn+1)) {
                        *pOut++ = *pIn++;
                    }
                    *pOut++ = *pIn;
                    break;
                }
                pIn++;
                break;

            default:
                if (IsDBCSLeadByte(*pIn) && *(pIn+1)) {
                    *pOut++ = *pIn++;
                }
                *pOut++ = *pIn++;
                break;
            }
        }


        /* Launch the process and waits for it to complete */

        si.cb = sizeof(STARTUPINFO);
        si.lpReserved = NULL;
        si.lpReserved2 = NULL;
        si.cbReserved2 = 0;
        si.lpTitle = (LPSTR)cmdline; 
        si.lpDesktop = (LPTSTR)NULL;
        si.dwFlags = STARTF_FORCEONFEEDBACK;


        if (!CreateProcess(NULL,
                        cmdline,
                        NULL,
                        NULL,
                        FALSE,
                        NORMAL_PRIORITY_CLASS,
                        NULL,
                        (LPTSTR)NULL,
                        &si,
                        &pi)) {
                windiff_UI(TRUE);
                MessageBox(hwndClient, LoadRcString(IDS_FAILED_TO_LAUNCH_EDT),
                        "Windiff", MB_ICONSTOP|MB_OK);
                windiff_UI(FALSE);
                goto error;
        }

        /* wait for completion. */
        WaitForSingleObject(pi.hProcess, INFINITE);

        /* close process and thread handles */
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        /* finished with the filename. deletes it if it was a temp. */
        compitem_freefilename(item, option, fname);

        /*
         * refresh cached view always .  A common trick is to edit the
         * composite file and then save it as a new left or right file.
         * Equally the user can edit the left and save as a new right.
         */

        /* We want to force both files to be re-read, but it's not a terribly
         * good idea to throw the lines away on this thread.  Someone might
         * be reading them on another thread!
         */
        /* file_discardlines(compitem_getleftfile(item)) */
        /* file_discardlines(compitem_getrightfile(item)) */

        /* force the compare to be re-done */
        PostMessage(hwndClient, WM_COMMAND, IDM_UPDATE, (LONG)item);
error:
        gmem_free(hHeap, (LPSTR) pe, sizeof(EDITARGS));

        return 0;

} /* do_editfile */


/***************************************************************************
 * Function: do_editthread
 *
 * Purpose:
 *
 * Launch an editor on a separate thread.  It will actually get a separate
 * process, but we want our own thread in this process.  This thread will
 * wait until it's finished and then order up a refresh of the UI.
 * Need to give it its parameters as a gmem allocated packet because
 * it IS on a separate thread.
 */
void do_editthread(VIEW view, int option)
{
        PEDITARGS pe;
        HANDLE thread;
        DWORD threadid;

        pe = (PEDITARGS) gmem_get(hHeap, sizeof(EDITARGS));
        pe->view = view;
        pe->option = option;
        pe->selection = selection;

        thread = CreateThread( NULL
                             , 0
                             , (LPTHREAD_START_ROUTINE)do_editfile
                             , (LPVOID) pe
                             , 0
                             , &threadid
                             );
        if (thread == NULL)
        {
                /* The createthread failed, do without the extra thread - just
                 * call the function synchronously
                 */
                 do_editfile(pe);
        }
        else CloseHandle(thread);
} /* do_editthread */


/* status bar and busy flags --------------------------------------------*/


/***************************************************************************
 * Function: SetButtonText
 *
 * Purpose:
 *
 * Set the Text on the statusbar button to reflect the current state 
 */
void
SetButtonText(LPSTR cmd)
{
        SendMessage(hwndStatus, SM_SETTEXT, IDM_ABORT, (DWORD) cmd);
}

/***************************************************************************
 * Function: SetStatus
 *
 * Purpose:
 *
 * Set the status field (left-hand part) of the status bar. 
 */
void
SetStatus(LPSTR cmd)
{
        SendMessage(hwndStatus, SM_SETTEXT, IDL_STATLAB, (DWORD) cmd);
}


/***************************************************************************
 * Function: SetNames
 *
 * Purpose:
 *
 * Set the names field - the central box in the status bar 
 */
void
SetNames(LPSTR names)
{
        SendMessage(hwndStatus, SM_SETTEXT, IDL_NAMES, (DWORD) names);
        if (names == NULL) {
                AppTitle[0] = '\0';
        } else {
                strncpy(AppTitle, names, sizeof(AppTitle));
        }
}

/***************************************************************************
 * Function: SetBusy
 *
 * Purpose:
 *
 * If we are not already busy, set the busy flag.
 *
 * Enter critical section first.
 */
BOOL
SetBusy(void)
{
        HMENU hmenu;


        WDEnter();

        if (fBusy) {
                WDLeave();
                return(FALSE);
        }


        fBusy = TRUE;

        SetStatus(LoadRcString(IDS_COMPARING));
        /* status also on window text, so that you can see even from
         * the icon when the scan has finished
         */
        SetWindowText(hwndClient, LoadRcString(IDS_SCANNING));

        /* disable appropriate parts of menu */
        hmenu = GetMenu(hwndClient);
        EnableMenuItem(hmenu, IDM_FILE,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
        EnableMenuItem(hmenu, IDM_DIR,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);
        EnableMenuItem(hmenu, IDM_PRINT,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);

        /* enable abort only when busy */
        EnableMenuItem(hmenu, IDM_ABORT,MF_ENABLED|MF_BYCOMMAND);
        SetButtonText(LoadRcString(IDS_ABORT));  /* leave DisplayMode unchanged */

        WDLeave();
        return(TRUE);
} /* SetBusy */
/***************************************************************************
 * Function: SetNotBusy
 *
 * Purpose:
 *
 * This function can be called from the worker thread.
 * Thus we must not cause any SendMessage calls to windows
 * owned by the main thread while holding the CritSec or we
 * could cause deadlock.
 *
 * The critsec is only needed to protect the fBusy flag - so
 * clear the busy flag last, and only get the crit sec as needed.
 */
void
SetNotBusy(void)
{
        HMENU hmenu;

        /* reset button and status bar (clearing out busy flags) */
        if (current_view == NULL) {
                SetButtonText(LoadRcString(IDS_EXIT));
                SetStatus(NULL);
                DisplayMode = MODE_NULL;
        } else if (view_isexpanded(current_view)) {
                TCHAR szBuf[10];
                lstrcpy(szBuf,LoadRcString(IDS_OUTLINE));
                SetButtonText(szBuf);
                SetStatus(view_getcurrenttag(current_view) );
                DisplayMode = MODE_EXPAND;
        } else {
                TCHAR szBuf[8];
                lstrcpy(szBuf,LoadRcString(IDS_EXPAND));
                SetButtonText(szBuf);
                SetStatus(NULL);
                DisplayMode = MODE_OUTLINE;
        }

        SetWindowText(hwndClient, "WinDiff");

        /* re-enable appropriate parts of menu */
        hmenu = GetMenu(hwndClient);
        EnableMenuItem(hmenu, IDM_FILE,MF_ENABLED|MF_BYCOMMAND);
        EnableMenuItem(hmenu, IDM_DIR,MF_ENABLED|MF_BYCOMMAND);
        EnableMenuItem(hmenu, IDM_PRINT,MF_ENABLED|MF_BYCOMMAND);

        /* disable abort now no longer busy */
        EnableMenuItem(hmenu, IDM_ABORT,MF_DISABLED|MF_GRAYED|MF_BYCOMMAND);


        /* clear the busy flag, protected by critical section */
        WDEnter();

        fBusy = FALSE;
        bAbort = FALSE;

        if (ghThread!=NULL){
            CloseHandle(ghThread);
            ghThread = NULL;
        }
        WDLeave();
} /* SetNotBusy */

/***************************************************************************
 * Function: IsBusy
 *
 * Purpose:
 *
 * Checks whether or not crit sec is open
 */
BOOL
IsBusy()
{
        BOOL bOK;

        WDEnter();
        bOK = fBusy;
        WDLeave();
        return(bOK);
} /* IsBusy */

/***************************************************************************
 * Function: BusyError
 *
 * Purpose:
 *
 * Puts up message box that system is busy.
 */
void
BusyError(void)
{
        windiff_UI(TRUE);
        MessageBox(hwndClient,
                LoadRcString(IDS_PLEASE_WAIT),
                "WinDiff", MB_OK|MB_ICONSTOP);
        windiff_UI(FALSE);
} /* BusyError */

/* --- colour scheme --------------------------------------------------- */

/***************************************************************************
 * Function: StateToColour
 *
 * Purpose:
 *
 * Map the state given into a foreground and a background colour
 * for states that are highlighted. Return P_FCOLOUR if the foreground
 * colour (put in *foreground) is to be used, return P_FCOLOUR|P_BCOLOUR if
 * both *foreground and *background are to be used, or 0 if the default
 * colours are to be used.
 */
UINT
StateToColour(int state, int col, DWORD FAR * foreground, DWORD FAR * background)
{


        switch (state) {

        case STATE_DIFFER:
                /* files that differ are picked out in a foreground highlight,
                 * with the default background
                 */
                *foreground = rgb_outlinehi;
                return(P_FCOLOUR);

        case STATE_LEFTONLY:
                /* lines only in the left file */
                *foreground = rgb_leftfore;
                *background = rgb_leftback;
                return(P_FCOLOUR|P_BCOLOUR);

        case STATE_RIGHTONLY:
                /* lines only in the right file */
                *foreground = rgb_rightfore;
                *background = rgb_rightback;
                return(P_FCOLOUR|P_BCOLOUR);

        case STATE_MOVEDLEFT:
                /* displaced lines in both files - left file version */
                *foreground = rgb_mleftfore;
                *background = rgb_mleftback;
                return(P_FCOLOUR|P_BCOLOUR);

        case STATE_MOVEDRIGHT:
                /* displaced lines in both files - right file version */
                *foreground = rgb_mrightfore;
                *background = rgb_mrightback;
                return(P_FCOLOUR|P_BCOLOUR);

        default:

                /* no highlighting - default colours */
                return(0);
        }

}

/* table window communication routines ---------------------------------*/

/***************************************************************************
 * Function: SetSelection
 *
 * Purpose:
 *
 * Set a given row as the selected row in the table window 
 */
void
SetSelection(long rownr)
{
        TableSelection select;

        select.startrow = rownr;
        select.startcell = 0;
        select.nrows = 1;
        select.ncells = 1;
        SendMessage(hwndRCD, TM_SELECT, 0, (long) (LPSTR)&select);
}


/***************************************************************************
 * Function: do_gethdr
 *
 * Purpose:
 *
 * Handle table class call back to get nr of rows and columns,
 * and properties for the whole table.
 * The 'table id' is either TABID_PRINTER - meaning we are
 * printing the current_view, or it is the view to
 * use for row/column nr information
 */
long
do_gethdr(HWND hwnd, lpTableHdr phdr)
{
        VIEW view;
        BOOL bIsPrinter = FALSE;

        if (phdr->id == TABID_PRINTER) {
                view = current_view;
                bIsPrinter = TRUE;
        } else {
                view = (VIEW) phdr->id;
        }
        if (view == NULL) {
                return(FALSE);
        }

        phdr->nrows = view_getrowcount(view);

        /*  three columns: line nr, tag and rest of line */

        /*
         * if IDM_NONRS (no line numbers) is selected, suppress the
         * line-nr column entirely to save screen space
         */
        if (line_numbers == IDM_NONRS) {
                phdr->ncols = 2;
                phdr->fixedcols = 0;
        } else {
                phdr->ncols = 3;
                phdr->fixedcols = 1;
        }

        phdr->fixedrows = 0;
        phdr->fixedselectable = FALSE;
        phdr->hseparator = TRUE;
        phdr->vseparator = TRUE;

        phdr->selectmode = TM_ROW | TM_SINGLE;
        /*
         * find if we are in expand mode - ask for the item we are expanding.
         */
        if (view_isexpanded(view) == TRUE) {

                /* use focus rect as selection mode in expand mode
                 * so as not to interfere with background colours.
                 */
                phdr->selectmode |= TM_FOCUS;
        } else {
                /* use default solid inversion when possible as it is clearer.*/
                phdr->selectmode |= TM_SOLID;
        }

        /* please send TQ_SCROLL notifications when the table is scrolled */
        phdr->sendscroll = TRUE;
        phdr->props.valid = 0;

        return TRUE;
}

/***************************************************************************
 * Function: do_getprops
 *
 * Purpose:
 *
 * Respond to table callback asking for the size and properties
 * of each column. Table id is either TABID_PRINTER (meaning the
 * current_view, for printing) or it is the view to be used.
 */
long
do_getprops(HWND hwnd, lpColPropsList propslist)
{
        int i, cell;
        BOOL bIsPrinter = FALSE;
        VIEW view;

        if (propslist->id == TABID_PRINTER) {
                view = current_view;
                bIsPrinter = TRUE;
        } else {
                view = (VIEW) propslist->id;
        }
        if (view == NULL) {
                return(FALSE);
        }

        /* The table inteface is slightly confused here. we are not
         * guaranteed which columns we are being asked about, so instead
         * of just setting each column cols[0], cols[1] etc, we need
         * to loop through, looking at each column in the table and
         * seeing which it is.
         */
        for (i = 0; i < propslist->ncols; i++) {
                cell = i + propslist->startcol;
                propslist->plist[i].props.valid = 0;

                /* for all column widths, add on 1 for the NULL char. */

                /*
                 * skip the line nr column if IDM_NONRS
                 */
                if (line_numbers == IDM_NONRS) {
                        cell++;
                }

                if (cell == 0) {
                        /* properties for line nr column */

                        propslist->plist[i].nchars = view_getwidth(view, 0)+1;
                        propslist->plist[i].props.valid |= P_ALIGN;
                        propslist->plist[i].props.alignment = P_CENTRE;
                } else if (cell == 1) {

                        /* properties for tag field */
                        propslist->plist[i].nchars = view_getwidth(view, 1)+1;
                        propslist->plist[i].props.valid |= P_ALIGN;
                        propslist->plist[i].props.alignment = P_LEFT;
                } else {
                        /* properties for main text column -
                         * use a fixed font unless printing (if
                         * printing, best to use the default font, because
                         * of resolution differences.
                         * add on 8 chars to the width to ensure that
                         * the width of lines beginning with tabs
                         * works out ok
                         */
                        propslist->plist[i].nchars = view_getwidth(view, 2)+1;
                        propslist->plist[i].props.valid |= P_ALIGN;
                        propslist->plist[i].props.alignment = P_LEFT;
                        if (!bIsPrinter) {
                                propslist->plist[i].props.valid |= P_FONT;
                                propslist->plist[i].props.hFont =
                                        GetStockObject(SYSTEM_FIXED_FONT);
                        }
                }
        }
        return (TRUE);
}

/***************************************************************************
 * Function: do_getdata
 *
 * Purpose:
 *
 * Respond to a table callback asking for the contents of individual cells.
 * table id is either TABID_PRINTER, or it is a pointer to the view
 * to use for data. If going to the printer, don't set the
 * colours (stick to black and white).
 */
long
do_getdata(HWND hwnd, lpCellDataList cdlist)
{
        int start, endcell, col, i;
        lpCellData cd;
        VIEW view;
        LPSTR textp;
        BOOL bIsPrinter = FALSE;

        if (cdlist->id == TABID_PRINTER) {
                view = current_view;
                bIsPrinter = TRUE;
        } else {
                view = (VIEW) cdlist->id;
        }

        start = cdlist->startcell;
        endcell = cdlist->ncells + start;
        if (cdlist->row >= view_getrowcount(view)) {
                return(FALSE);
        }
        for (i = start; i < endcell; i++) {
                cd = &cdlist->plist[i - start];


                /* skip the line number column if IDM_NONRS */
                if (line_numbers == IDM_NONRS) {
                        col = i+1;
                } else {
                        col = i;
                }

                /* set colour of text to mark out
                 * lines that are changed, if not printer - for the
                 * printer everything should stay in the default colours
                 */

                if (!bIsPrinter) {

                        /* convert the state of the requested row into a
                         * colour scheme. returns P_FCOLOUR and/or
                         * P_BCOLOUR if it sets either of the colours
                         */
                        cd->props.valid |=
                            StateToColour(view_getstate(view, cdlist->row), col,
                                        &cd->props.forecolour,
                                        &cd->props.backcolour);
                }

                textp = view_gettext(view, cdlist->row, col);
                if (cd->nchars != 0) {
                        if (textp == NULL) {
                                cd->ptext[0] = '\0';
                        } else {
                                strncpy(cd->ptext, textp, cd->nchars -1);
                                cd->ptext[cd->nchars - 1] = '\0';
                        }
                }

        }
        return(TRUE);
}

/***************************************************************************
 * Function: SvrClose
 *
 * Purpose:
 *
 * Table window has finished with this view. It can be deleted.
 */
void
SvrClose(void)
{
        view_delete(current_view);
        current_view = NULL;

        /* hide picture - only visible when we are in MODE_EXPAND */
        DisplayMode = MODE_NULL;
        DoResize(hwndClient);

        /* if we already busy when closing this view (ie
         * we are in the process of starting a new scan,
         * then leave the status bar alone, otherwise
         * we should clean up the state of the status bar
         */
        if (!fBusy) {
                SetButtonText(LoadRcString(IDS_EXIT));
                SetNames(NULL);
                SetStatus(NULL);

        }

} /* SvrClose */


/***************************************************************************
 * Function: TableServer
 *
 * Purpose:
 *
 * Handle callbacks and notifications from the table class 
 */
long
TableServer(HWND hwnd, UINT cmd, long lParam)
{
        lpTableHdr phdr;
        lpColPropsList proplist;
        lpCellDataList cdlist;
        lpTableSelection pselect;

        switch(cmd) {
        case TQ_GETSIZE:
                /* get the nr of rows and cols in this table */
                phdr = (lpTableHdr) lParam;
                return(do_gethdr(hwnd, phdr));

        case TQ_GETCOLPROPS:
                /* get the size and properties of each column */
                proplist = (lpColPropsList) lParam;
                return (do_getprops(hwnd, proplist));

        case TQ_GETDATA:
                /* get the contents of individual cells */
                cdlist = (lpCellDataList) lParam;
                return (do_getdata(hwnd, cdlist));


        case TQ_SELECT:
                /* selection has changed */
        case TQ_ENTER:
                /* user has double-clicked or pressed enter */

                pselect = (lpTableSelection) lParam;

                /* store location for use in later search (IDM_FCHANGE) */
                if (pselect->nrows < 1) {
                        selection = -1;
                } else {
                        selection = (int) pselect->startrow;
                        if (cmd == TQ_ENTER) {
                                /* try to expand this row */
                                if (!ToExpand(hwnd)) {
                                        /* expand failed - maybe this
                                         * is a moved line- show the other
                                         * copy
                                         */
                                        ToMoved(hwnd);
                                }

                        }
                }
                break;

        case TQ_CLOSE:
                /* close this table - table class no longer needs data*/
                SvrClose();
                break;

        case TQ_SCROLL:
                /* notification that the rows visible in the window
                 * have changed -change the current position lines in
                 * the graphic bar view (the sections picture)
                 */
                if (picture_mode) {
                        BarDrawPosition(hwndBar, NULL, TRUE);
                }
                break;

        default:
                return(FALSE);
        }
        return(TRUE);
}


/***************************************************************************
 * Function: wd_initial
 *
 * Purpose:
 *
 * Called on worker thread (not UI thread) to handle the work
 * requested on the command line. 
 * arg is a pointer to a THREADARGS block allocated from gmem_get(hHeap). This
 * needs to be freed before exiting.
 */
DWORD
wd_initial(LPVOID arg)
{
        PTHREADARGS pta = (PTHREADARGS) arg;
        COMPLIST cl;


        /* build a complist from these args,
         * and register with the view we have made
         */
        cl = complist_args(pta->first, pta->second, pta->view, pta->fDeep);

        if (cl == NULL) {
                view_close(pta->view);
                gmem_free(hHeap, (LPSTR) pta, sizeof(THREADARGS));
                SetNotBusy();
                return 0;
        }


        /* if savelist was selected, write out the list and exit */
        if(pta->savelist != NULL) {
                complist_savelist(cl, pta->savelist, pta->saveopts);
                gmem_free(hHeap, (LPSTR) pta, sizeof(THREADARGS));
                SetNotBusy();
                exit(0);
        }

        /* if there was only one file, expand it */
        if (view_getrowcount(pta->view) == 1) {
                SetSelection(0);
                ToExpand(hwndClient);
        }


        gmem_free(hHeap, (LPSTR) pta, sizeof(THREADARGS));
        SetNotBusy();
        return(0);
} /* wd_initial */


/***************************************************************************
 * Function: wd_dirdialog
 *
 * Purpose:
 *
 * Called on worker thread (not UI thread) to handle a Dir request
 */
DWORD
wd_dirdialog(LPVOID arg)
{

        VIEW view = (VIEW) arg;

        /* make a COMPLIST using the directory dialog,
         * and notify the view
         */
        if (complist_dirdialog(view) == NULL) {
                view_close(view);
        }

        /* all done! */
        SetNotBusy();
        return(0);
}


/***************************************************************************
 * Function: wd_copy
 *
 * Purpose:
 *
 * Called on worker thread to do a copy-files operation
 */
DWORD
wd_copy(LPVOID arg)
{

        VIEW view = (VIEW) arg;

        complist_copyfiles(view_getcomplist(view), NULL, 0);

        SetNotBusy();

        return(0);
}


/***************************************************************************
 * Function: MainWndProc
 *
 * Purpose:
 *
 * Window processing for main window
 */
long APIENTRY
MainWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
        char str[32];
        long ret;
        DWORD threadid;

        switch(message) {


        case WM_CREATE:

                /* initialise menu options to default/saved
                 * option settings
                 */

                CheckMenuItem(hMenu, IDM_INCSAME,
                      (outline_include & INCLUDE_SAME) ?
                                MF_CHECKED:MF_UNCHECKED);

                CheckMenuItem(hMenu, IDM_INCLEFT,
                      (outline_include & INCLUDE_LEFTONLY) ?
                                MF_CHECKED:MF_UNCHECKED);

                CheckMenuItem(hMenu, IDM_INCRIGHT,
                      (outline_include & INCLUDE_RIGHTONLY) ?
                                MF_CHECKED:MF_UNCHECKED);
                CheckMenuItem(hMenu, IDM_INCDIFFER,
                      (outline_include & INCLUDE_DIFFER) ?
                                MF_CHECKED:MF_UNCHECKED);

                CheckMenuItem(hMenu, line_numbers, MF_CHECKED);
                CheckMenuItem(hMenu, expand_mode, MF_CHECKED);

                CheckMenuItem(hMenu, IDM_IGNBLANKS,
                        ignore_blanks ? MF_CHECKED : MF_UNCHECKED);
                CheckMenuItem(hMenu, IDM_PICTURE,
                        picture_mode ? MF_CHECKED : MF_UNCHECKED);

                /* nothing currently displayed */
                DisplayMode = MODE_NULL;

                break;


        case WM_COMMAND:
                switch (GET_WM_COMMAND_ID(wParam, lParam)) {
                case IDM_EXIT:
                        if (ghThread!=NULL) {
                                extern CRITICAL_SECTION CSView;
                                /* Stop any other thread from allocating things that we
                                   want to free!  See the threads DOGMA at the top
                                   of this file.
                                */

                                /* Because the thread that we are about to kill might be in
                                   a critical section, we first grab them both.  It is
                                   essential that anyone else who ever does this, does
                                   so in the same order!
                                */
                                WDEnter();
                                EnterCriticalSection(&CSView);
                                TerminateThread(ghThread, 31);
                                CloseHandle(ghThread);
                                ghThread = NULL;
                                LeaveCriticalSection(&CSView);
                                WDLeave();
                        }
                        if (!view_isexpanded(current_view)) {
                                /* save the current outline size and position */
                                WINDOWPLACEMENT wp;
                                if (GetWindowPlacement(hwndClient,&wp)) {
                                        WriteProfileInt(APPNAME, "OutlineShowCmd", wp.showCmd);
                                        WriteProfileInt(APPNAME, "OutlineMaxX", wp.ptMaxPosition.x);
                                        WriteProfileInt(APPNAME, "OutlineMaxY", wp.ptMaxPosition.y);
                                        WriteProfileInt(APPNAME, "OutlineNormLeft", wp.rcNormalPosition.left);
                                        WriteProfileInt(APPNAME, "OutlineNormTop", wp.rcNormalPosition.top);
                                        WriteProfileInt(APPNAME, "OutlineNormRight", wp.rcNormalPosition.right);
                                        WriteProfileInt(APPNAME, "OutlineNormBottom", wp.rcNormalPosition.bottom);
                                        WriteProfileInt(APPNAME, "OutlineSaved", 1);
                                }
                        } else {
                                /* save the current expanded size and position */
                                WINDOWPLACEMENT wp;
                                if (GetWindowPlacement(hwndClient,&wp)) {
                                        WriteProfileInt(APPNAME, "ExpandShowCmd", wp.showCmd);
                                        WriteProfileInt(APPNAME, "ExpandMaxX", wp.ptMaxPosition.x);
                                        WriteProfileInt(APPNAME, "ExpandMaxY", wp.ptMaxPosition.y);
                                        WriteProfileInt(APPNAME, "ExpandNormLeft", wp.rcNormalPosition.left);
                                        WriteProfileInt(APPNAME, "ExpandNormTop", wp.rcNormalPosition.top);
                                        WriteProfileInt(APPNAME, "ExpandNormRight", wp.rcNormalPosition.right);
                                        WriteProfileInt(APPNAME, "ExpandNormBottom", wp.rcNormalPosition.bottom);
                                        WriteProfileInt(APPNAME, "ExpandedSaved", 1);
                                }
                        }
                        DestroyWindow(hWnd);
                        break;

                case IDM_ABORT:
                        /* abort menu item, or status bar button.
                         * the status bar button text gives the appropriate
                         * action depending on our state - abort, outline
                         * or expand. But the command sent is always
                         * IDM_ABORT. Thus we need to check the state
                         * to see what to do. If we are busy, set the abort
                         * flag. If there is nothing to view,
                         * exit, otherwise switch outline<->expand
                         */
                        if (IsBusy()) {
                                bAbort = TRUE;
                                SetStatus(LoadRcString(IDS_ABORT_PENDING));

                        } else if (DisplayMode == MODE_NULL) {
                                DestroyWindow(hWnd);
                        } else if (DisplayMode == MODE_EXPAND) {
                                ToOutline(hWnd);
                        } else {
                                ToExpand(hWnd);
                        }
                        break;

                case IDM_FILE:
                        /* select two files and compare them */
                        if (SetBusy()) {

                               /* close the current view */
                                view_close(current_view);

                                /* make a new empty view */
                                current_view = view_new(hwndRCD);

                                /* make a COMPLIST using the files dialog,
                                 * and notify the view
                                 */
                                if (complist_filedialog(current_view) == NULL) {
                                        view_close(current_view);
                                }

                                /* all done! */
                                SetNotBusy();
                        } else {
                                BusyError();
                        }
                        break;

                case IDM_DIR:

                        /* read two directory names, scan them and
                         * compare all the files and subdirs.
                         */
                        if (SetBusy()) {

                                /* close the current view */
                                view_close(current_view);

                                /* make a new empty view */
                                current_view = view_new(hwndRCD);

                                ghThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wd_dirdialog,
                                        (LPVOID) current_view, 0, &threadid);

                                if (ghThread == NULL)
                                {
                                        wd_dirdialog( (LPVOID) current_view);
                                }

                        } else {
                                BusyError();
                        }
                        break;

                case IDM_CLOSE:
                        /* close the output list -
                         * discard all results so far
                         */
                        if (!IsBusy()) {
                                view_close(current_view);
                        }
                        break;

                case IDM_PRINT:
                        /* print the current view -
                         * either the outline list of filenames,
                         * or the currently expanded file.
                         */
                        if (!IsBusy()) {
                                DoPrint();
                        } else {
                                BusyError();
                        }
                        break;

                case IDM_TIME:
                        /* show time it took */
                        {       char msg[50];
                                DWORD tim;
                                if (IsBusy()) {
                                         BusyError();
                                }
                                else{
                                        tim = complist_querytime();
                                        wsprintf((LPTSTR)msg, LoadRcString(IDS_SECONDS), tim/1000, tim%1000);
                                }
                        }
                        break;

                case IDM_SAVELIST:
                        /* allow user to save list of same/different files
                         * to a text file. dialog box to give filename
                         * and select which types of file to include
                         */
                        complist_savelist(view_getcomplist(current_view), NULL, 0);
                        break;

                case IDM_COPYFILES:
                        /*
                         * copy files that are same/different to a new
                         * root directory. dialog box allows user
                         * to select new root and inclusion options
                         */
                        if (current_view == NULL) {
                                MessageBox(hWnd,
                                    LoadRcString(IDS_CREATE_DIFF_LIST),
                                    "WinDiff", MB_OK|MB_ICONSTOP);
                                break;
                        }

                        if (SetBusy()) {
                                ghThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)wd_copy,
                                        (LPVOID) current_view, 0, &threadid);
                                if (ghThread == NULL)
                                {
                                        wd_copy( (LPVOID) current_view);
                                }

                        } else {
                                BusyError();
                        }

                        break;

                case IDM_ABOUT:

                        DialogBox( hInst, "About", hWnd, (DLGPROC)AboutBox);
                        break;

                /* launch an editor on the current item - left, right or
                 * composite view
                 */
                case IDM_EDITLEFT:
                        do_editthread(current_view, CI_LEFT);
                        break;

                case IDM_EDITRIGHT:
                        do_editthread(current_view, CI_RIGHT);
                        break;

                case IDM_EDITCOMP:
                        do_editthread(current_view, CI_COMP);
                        break;

                /* allow customisation of the editor command line */
                case IDM_SETEDIT:
                        if (StringInput(editor_cmdline, sizeof(editor_cmdline),
                                        LoadRcString(IDS_ENTER_EDT_CMD_LINE),
                                        "Windiff", editor_cmdline))  {
                                WriteProfileString(APPNAME, "Editor",
                                        (LPCSTR)editor_cmdline);
                        }
                        break;


                case IDM_LNRS:
                case IDM_RNRS:
                case IDM_NONRS:

                        /* option selects whether the line nrs displayed
                         * in expand mode are the line nrs in the left
                         * file, the right file or none
                         */

                        CheckMenuItem(GetMenu(hWnd),
                                line_numbers, MF_UNCHECKED);
                        line_numbers = GET_WM_COMMAND_ID(wParam, lParam);
                        CheckMenuItem(GetMenu(hWnd), line_numbers, MF_CHECKED);
                        wsprintf((LPTSTR)str, "%d", line_numbers);
                        WriteProfileString(APPNAME, "LineNumbers", str);

                        /* change the display to show the line nr style
                         * chosen
                         */

                        view_changeviewoptions(current_view);


                        break;

                /*
                 * options selecting which files to include in the
                 * outline listing, based on their state
                 */
                case IDM_INCLEFT:


                        /* toggle flag in outline_include options */
                        outline_include ^= INCLUDE_LEFTONLY;

                        /* check/uncheck as necessary */
                        CheckMenuItem(hMenu, IDM_INCLEFT,
                              (outline_include & INCLUDE_LEFTONLY) ?
                                        MF_CHECKED:MF_UNCHECKED);

                        wsprintf((LPTSTR)str, "%d", outline_include);
                        WriteProfileString(APPNAME, "FileInclude", str);
                        view_changeviewoptions(current_view);


                        break;

                case IDM_INCRIGHT:


                        outline_include ^= INCLUDE_RIGHTONLY;

                        CheckMenuItem(hMenu, IDM_INCRIGHT,
                              (outline_include & INCLUDE_RIGHTONLY) ?
                                        MF_CHECKED:MF_UNCHECKED);
                        wsprintf((LPTSTR)str, "%d", outline_include);
                        WriteProfileString(APPNAME, "FileInclude", str);
                        view_changeviewoptions(current_view);

                        break;

                case IDM_INCSAME:


                        outline_include ^= INCLUDE_SAME;

                        CheckMenuItem(hMenu, IDM_INCSAME,
                              (outline_include & INCLUDE_SAME) ?
                                        MF_CHECKED:MF_UNCHECKED);
                        wsprintf((LPTSTR)str, "%d", outline_include);
                        WriteProfileString(APPNAME, "FileInclude", str);
                        view_changeviewoptions(current_view);


                        break;


                case IDM_INCDIFFER:



                        outline_include ^= INCLUDE_DIFFER;

                        CheckMenuItem(hMenu, IDM_INCDIFFER,
                              (outline_include & INCLUDE_DIFFER) ?
                                        MF_CHECKED:MF_UNCHECKED);

                        wsprintf((LPTSTR)str, "%d", outline_include);
                        WriteProfileString(APPNAME, "FileInclude", str);
                        view_changeviewoptions(current_view);


                        break;

                case IDM_UPDATE:
                        /* update the display.  Options or files may have changed */
                        /* discard lines  (thereby forcing re-read).
                         */
                        file_discardlines(compitem_getleftfile( (COMPITEM)lParam) );
                        file_discardlines(compitem_getrightfile( (COMPITEM)lParam) );

                        view_changediffoptions(current_view);

                        /* force repaint of bar window */
                        InvalidateRect(hwndBar, NULL, TRUE);
                        break;



                case IDM_LONLY:
                case IDM_RONLY:
                case IDM_BOTHFILES:
                        /* option selects whether the expanded file
                         * show is the combined file, or just one
                         * or other of the input files.
                         *
                         * if we are not in expand mode, this also
                         * causes us to expand the selection
                         */


                        CheckMenuItem(GetMenu(hWnd), expand_mode, MF_UNCHECKED);
                        expand_mode = GET_WM_COMMAND_ID(wParam, lParam);
                        CheckMenuItem(GetMenu(hWnd), expand_mode, MF_CHECKED);

                        /* change the current view to show only the lines
                         * of the selected type.
                         */
                        if (DisplayMode == MODE_OUTLINE) {
                                ToExpand(hWnd);
                        } else {
                                view_changeviewoptions(current_view);
                        }


                        break;


                case IDM_IGNBLANKS:

                        /* if selected, ignore all spaces and tabs on
                         * comparison - expand view only: outline view
                         * will still show that 'text files differ'
                         */

                        ignore_blanks = !ignore_blanks;
                        CheckMenuItem(hMenu, IDM_IGNBLANKS,
                                ignore_blanks? MF_CHECKED:MF_UNCHECKED);
                        wsprintf((LPTSTR)str, "%d", ignore_blanks);
                        WriteProfileString(APPNAME, "Blanks", str);

                        /* invalidate all diffs since we have
                         * changed diff options, and re-do and display the
                         * current diff if we are in expand mode.
                         */
                        view_changediffoptions(current_view);

                        /* force repaint of bar window */
                        InvalidateRect(hwndBar, NULL, TRUE);

                        break;

                case IDM_PICTURE:
                        /* do we show the bar picture in expand mode ? */
                        picture_mode = !picture_mode;
                        CheckMenuItem(hMenu, IDM_PICTURE,
                                picture_mode? MF_CHECKED:MF_UNCHECKED);
                        wsprintf((LPTSTR)str, "%d", picture_mode);
                        WriteProfileString(APPNAME, "Picture", str);
                        DoResize(hWnd);
                        break;


                case IDM_EXPAND:

                        /* show the expanded view of the
                         * selected file
                         */
                        if (current_view != NULL) {
                                ToExpand(hWnd);
                        }

                        break;

                case IDM_OUTLINE:
                        /* return to the outline view (list of filenames) */
                        ToOutline(hWnd);

                        break;

                case IDM_FCHANGE:
                        /* find the next line in the current view
                         * that is not the same in both files -
                         * in outline view, finds the next filename that
                         * is not identical
                         */
                        FindNextChange();

                        break;

                case IDM_FPCHANGE:
                        /* same as IDM_FCHANGE, but going backwards from
                         * current position
                         */
                        FindPrevChange();

                        break;
                }
                break;

        case WM_SIZE:
                DoResize(hWnd);
                break;

        case WM_SETFOCUS:
                /* set the focus on the table class so it can process
                 * page-up /pagedown keys etc.
                 */
                SetFocus(hwndRCD);
                break;

        case WM_KEYDOWN:
                /* although the table window has the focus, he passes
                 * back to us any keys he doesn't understand
                 * We handle escape here to mean 'return to outline view'
                 */
                if (wParam == VK_ESCAPE) {
                        ToOutline(hWnd);
                }
                break;

        case WM_CLOSE:
                /* don't allow close when busy - process this message in
                 * order to ensure this
                 */
                if (IsBusy()) {
                        return(TRUE);
                } else {
                        return(DefWindowProc(hWnd, message, wParam, lParam));
                }
                break;

        case WM_DESTROY:

                DeleteTools();
                PostQuitMessage(0);
                break;

        case TM_CURRENTVIEW:
                /* allow other people such as the bar window to query the
                 * current view
                 */
                return((DWORD) current_view);

        default:
                /* handle registered table messages */
                if (message == table_msgcode) {
                        ret = TableServer(hWnd, wParam, lParam);
                        return(ret);
                }
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        return(0);
}

/***************************************************************************
 * Function: My_mbschr
 *
 * Purpose:
 *
 * DBCS version of strchr
 *
 */
unsigned char * _CRTAPI1 My_mbschr(
    unsigned char *psz, unsigned short uiSep)
{
    while (*psz != '\0' && *psz != uiSep) {
        psz = CharNext(psz);
    }
    return *psz == uiSep ? psz : NULL;
}
/***************************************************************************
 * Function: My_mbsncpy
 *
 * Purpose:
 *
 * DBCS version of strncpy
 *
 */
unsigned char * _CRTAPI1 My_mbsncpy(
	unsigned char *psz1, const unsigned char *psz2, size_t Length)
{
        int nLen = (int)Length;
	unsigned char *pszSv = psz1;

	while (0 < nLen) {
		if (*psz2 == '\0') {
			*psz1++ = '\0';
			nLen--;
		} else if (IsDBCSLeadByte(*psz2)) {
			if (nLen == 1) {
				*psz1 = '\0';
			} else {
				*psz1++ = *psz2++;
				*psz1++ = *psz2++;
			}
			nLen -= 2;
		} else {
			*psz1++ = *psz2++;
			nLen--;
		}
	}
	return pszSv;
}
/***************************************************************************
 * Function: My_mbsrchr
 *
 * Purpose:
 *
 * DBCS version of strrchr
 *
 */
unsigned char * _CRTAPI1 My_mbsrchr(
    unsigned char *psz, unsigned short uiSep)
{
    unsigned char *pszHead;

    pszHead = psz;

    while (*psz != '\0') {
        psz++;
    }
    if (uiSep == '\0') {
        return psz;
    }

    while (psz > pszHead) {
        psz = CharPrev(pszHead, psz);
        if (*psz == uiSep) {
            break;
        }
    }
    return *psz == uiSep ? psz : NULL;
}
/***************************************************************************
 * Function: My_mbsncmp
 *
 * Purpose:
 *
 * DBCS version of strncmp
 * If 'nLen' splits a DBC, this function compares the DBC's 2nd byte also.
 *
 */
int _CRTAPI1 My_mbsncmp(
    const unsigned char *psz1, const unsigned char *psz2, size_t nLen)
{
    int Length = (int)nLen;

    while (0 < Length) {
        if ('\0' == *psz1 || '\0' == *psz2) {
            return *psz1 - *psz2;
        }
        if (IsDBCSLeadByte(*psz1) || IsDBCSLeadByte(*psz2)) {
            if (*psz1 != *psz2 || *(psz1+1) != *(psz2+1)) {
                return *psz1 - *psz2;
            }
            psz1 += 2;
            psz2 += 2;
            Length -= 2;
        } else {
            if (*psz1 != *psz2) {
                return *psz1 - *psz2;
            }
            psz1++;
            psz2++;
            Length--;
        }
    }
    return 0;
}

