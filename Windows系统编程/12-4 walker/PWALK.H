#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "pefile.h"


#define IDR_MAINICON		     20
#define IDR_SYSSTATICON 	     21
#define IDR_PROSTATICON 	     22
#define IDR_WALKERMENU		     30

#define IDS_WALKERCLASS 	     11000
#define IDS_SYSSTATCLASS	     11001
#define IDS_PROSTATCLASS	     11002
#define IDS_STATUSCLASS 	     11004
#define IDS_MEMVIEWCLASS	     11005
#define IDS_SYSSTATTITLE	     11010
#define IDS_PROSTATTITLE	     11011
#define IDS_MEMVIEWTITLE	     11013
#define IDS_CAPTION		     11020
#define IDS_UNAVAILABLE 	     11021
#define IDS_LISTBOX		     11022
#define IDS_EXEFILEEXT		     11023
#define IDS_FILEOPENTITLE	     11024
#define IDS_SELF		     11025
#define IDS_ERROR		     11030
#define IDS_NOTENOUGHMEM	     11031
#define IDS_HEAPCREATEFAILED	     11032
#define IDS_DBGEVNTACTIVE	     11042
#define IDS_DBGEVNTCLOSE	     11043
#define IDS_DBGEVNTSTOP 	     11044
#define IDS_DBGEVNTSTART	     11045
#define IDS_DBGEVNTREAD 	     11046
#define IDS_DBGEVNTWRITE	     11047
#define IDS_DBGEVNTACK		     11048
#define IDS_EXCEPTION		     11050
#define IDS_APPEXCEPTION	     11051
#define IDS_CHILDAPPEXCEPTION	     11052
#define IDS_ERRDUPLICATEHANDLE	     11060
#define IDS_ERRGETTHREADCONTEXT      11061
#define IDS_ERRCREATEPROCESS	     11062
#define IDS_ERROPENPROCESS	     11063
#define IDS_ERRCREATEFILEMAPPING     11064
#define IDS_ERRHEAPALLOC	     11065
#define IDS_ERRREADPROCESSMEMORY     11066
#define IDS_ERRCREATEWINDOW	     11067
#define IDS_ERRVIRTUALFREE	     11068
#define IDS_ERRVIRTUALALLOC	     11069
#define IDS_STATUSREADY 	     11070
#define IDS_NOTCOMMITTEDMEMORY	     11071
#define IDS_ERRWRITEPROCESSMEMORY    11072
#define IDS_COULDNOTREADPROCESS      11073
#define IDS_ERRSETTHREADCONTEXT      11074
#define IDS_EXITPROCESS 	     12000
#define IDS_EXITTHREAD		     12001
#define IDS_CREATEPROCESS	     12002
#define IDS_CREATETHREAD	     12003
#define IDS_LOADDLL		     12004
#define IDS_UNLOADDLL		     12005
#define IDS_OUTPUTDEBUGSTRING	     12006
#define IDS_RIPEVENT		     12007
#define IDS_BREAKPOINTEXCEPTION      12008
#define IDS_ACCESSVIOLATIONEXCEPTION 12009
#define IDS_UNHANDLEDEXCEPTION	     12010
#define IDS_PROCESSSUSPENDED	     12011
#define IDS_PROCESSRESUMED	     12012
#define IDS_PROCESSINIT 	     12013
#define IDS_MAPFILENAME 	     12014
#define IDS_INITPROCESSFAILURE	     12015
#define IDS_INITPROBE		     12016
#define IDS_HOOKPROCESS 	     12017

#define IDC_LISTBOX		     100
#define IDC_STATUSWND		     101
#define IDC_REFRESH		     200
#define IDC_CANCEL		     201
#define IDC_REWALK		     202

#define IDM_POPUPMENUS		     1000
#define IDM_POPUPPROCESS	     1000
#define IDM_POPUPSORT		     1001
#define IDM_POPUPVIEW		     1002
#define IDM_POPUPOPTIONS	     1003
#define MENUPOPUPS		     4

#define IDM_PROCESSREWALK	     2000
#define IDM_PROCESSLOAD 	     2001
#define IDM_PROCESSUNLOAD	     2002
#define IDM_PROCESSSUSPEND	     2003
#define IDM_PROCESSRESUME	     2004
#define IDM_EXIT		     2010
#define IDM_SORTADDRESS 	     2020
#define IDM_SORTSTATE		     2021
#define IDM_SORTPROTECTION	     2022
#define IDM_SORTSIZE		     2023
#define IDM_SORTBASEADDRESS	     2024
#define IDM_VIEWSYSSTAT 	     2030
#define IDM_VIEWPROSTAT 	     2031
#define IDM_VIEWMEMORY		     2033
#define IDM_VIEWADDRESS 	     2034
#define IDM_OPTBYTES                 2040
#define IDM_OPTPAGES                 2041

#define IDD_ADDR		     3000
#define IDC_ADDR		     3001
#define IDC_HEX 		     3002

#define IDD_INITIALIZING	     4000
#define UM_ENDDIALOG		     WM_USER+2

#define TOTALVMRESERVE		     0x00100000
#define PAGESIZE		     0x1000
#define IDM_REMOVEVIEWWND	     WM_USER+0

#define IDT_STATUS		     3000
#define UM_UPDATE		     WM_USER+1
#define UM_STARTINITDIALOG	     WM_USER+3

#define STATUSWXB		     12
#define WXB_HPENHILITE		     0
#define WXB_HPENSHADOW		     4
#define WXB_LPWINDOWTEXT	     8

#define VIEWWXB 		     8
#define WXB_LPMEMVIEW		     0
#define WXB_LPOLDMEMVIEW	     4

/* define event handle array indeces */
#define CLOSEDEBUGGER		     0
#define SUSPENDDEBUGGER 	     1
#define RESUMEDEBUGGER		     2
#define READMEMORY		     3
#define WRITEMEMORY		     4
#define DEBUGACTIVE		     5
#define ACKNOWLEDGE		     6
#define nDEBUGEVENTS		     7


typedef struct DBGTHREAD    *LPDBGTHREAD;
typedef struct tagDbgThread
    {
    HANDLE		      hThread;
    DWORD		      dwThreadID;
    LPTHREAD_START_ROUTINE    lpStartAddress;
    int 		      nPriority;
    LPDBGTHREAD 	      Next;
    }DBGTHREAD;

typedef struct DBGDLL	 *LPDBGDLL;
typedef struct tagDbgDll
    {
    HANDLE	  hFile;
    LPVOID	  lpBaseOfDll;
    DWORD	  dwDebugInfoFileOffset;
    DWORD	  nDebugInfoSize;
    char	  szImageName[MAX_PATH];
    WORD	  fUnicode;
    LPDBGDLL	  Next;
    SECTIONINFO   *lpSection;  /* from PEFILE.H */
    }DBGDLL;

typedef struct DBGEXCEPTREC    *LPDBGEXCEPTREC;
typedef struct tagDbgExcept
    {
    EXCEPTION_RECORD	ExceptRecord;
    CONTEXT		Context;
    DWORD		dwThreadId;
    DWORD		dwFirstChance;
    LPDBGEXCEPTREC	Next;
    }DBGEXCEPTREC;

typedef struct tagDbgProcess
    {
    HANDLE	 hDbgHeap;
    DWORD	 dwProcessID;
    HANDLE	 hProcess;
    HANDLE	 hFile;
    LPVOID	 lpImage;
    DWORD	 dwDbgInfoOffset;
    DWORD	 nDbgInfoSize;
    DBGTHREAD	 *lpThreads;
    DBGDLL	 *lpDlls;
    SECTIONINFO  *lpSection;
    HWND	 hWnd;
    DWORD	 dwPriority;
    BOOL	 bActive;
    char	 szModule[MAX_PATH];
    DBGEXCEPTREC *lpERs;
    }DBGPROCESS;

typedef struct tagVMObject
    {
    MEMORY_BASIC_INFORMATION	mbi;
    char			szObjType[12];
    char			szModule[MAX_PATH];
    char			szSection[IMAGE_SIZEOF_SHORT_NAME];
    BOOL			bNew;
    }VMOBJECT, * LPVMOBJECT;


/* struct for memory view window */
typedef struct tagMemView
    {
    LPVOID    lpMem;
    int       nBase;
    int       nSize;
    int       xWin, yWin;
    int       PosV, RangeV;
    long      nLines;
    int       nExtraBytes;
} MEMVIEW, * LPMEMVIEW;


/* struct for process probe dll */
typedef struct tagProbe
    {
    HANDLE	 hProcess;
    HANDLE	 hDefHeap;
    }PROBE, * LPPROBE;


/* struct representing process state */
typedef struct tagProcessState
    {
    CONTEXT    Context;
    LPVOID     Esp;
    LPVOID     pStackPage;
    BYTE       Stack[PAGESIZE];
    LPVOID     Eip;
    LPVOID     pCodePage;
    BYTE       Code[PAGESIZE];
    }PROCESS_STATE, * LPPROCESS_STATE;


/* prototypes for functions in pwalk.c */
LONG   WINAPI WalkerWndProc (HWND, UINT, WPARAM, LPARAM);
int    WINAPI NotifyUser (HWND, int, int, char *, UINT);
void   WINAPI ReportError (int);
BOOL   WINAPI GetFreeDiskSpace (LPDWORD, LPDWORD);
BOOL   WINAPI InitDlgProc (HWND, UINT, WPARAM, LPARAM);

/* prototypes for functions in pwalkio.c */
BOOL   WINAPI GetFileName (HWND, char *, char *);
void   WINAPI GetFileFromPath (char *, char *);
BOOL   WINAPI GetCmdLine (char *, char *, BOOL *);
BOOL   WINAPI IsValidFile (char *);

/* prototypes for functions in pstat.c */
LONG   WINAPI SysStatWndProc (HWND, UINT, WPARAM, LPARAM);
LONG   WINAPI ProStatWndProc (HWND, UINT, WPARAM, LPARAM);
LONG   WINAPI StatusWndProc (HWND, UINT, WPARAM, LPARAM);
void   WINAPI SetStatusText (HWND, int, COLORREF);

/* prototypes for functions in pview.c */
HWND   WINAPI ViewMemory (HWND, char *, LPVOID, int, int);
LONG   WINAPI MemWndProc (HWND, UINT, WPARAM, LPARAM);
void   WINAPI ActivateViewWindow (ATOM);
HWND   WINAPI EnumViewWindows (HWND, HWND);
BOOL   WINAPI AddrDlgProc (HWND, UINT, WPARAM, LPARAM);

/* prototypes for functions in pdebug.c */
DBGPROCESS* WINAPI StartChildProcess (HWND, char *, LPHANDLE);
void   WINAPI CloseChildProcess (DBGPROCESS *, LPHANDLE);
int    WINAPI WhereIsStack (HANDLE);
int    WINAPI WalkProcess (HANDLE, LPVOID *, LPINT *);
void   WINAPI AnalyzeProcess (DBGPROCESS *, LPVMOBJECT, int);
void   WINAPI IdentifyNewObjects (LPVMOBJECT, int, LPVMOBJECT, int);
BOOL   WINAPI CommittedMemoryRange (int, int, LPVMOBJECT, int *);
BOOL   WINAPI AccessProcessMemory (HANDLE, HANDLE, LPVOID, LPVOID, DWORD *);

/* prototypes for functions in probe.c, a DLL */
void	WINAPI ResetProbe ();
LPPROBE WINAPI RetrieveProbeData ();
