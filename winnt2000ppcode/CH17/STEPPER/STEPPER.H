/*----------------------------------------------------
        STEPPER.H
  ----------------------------------------------------*/
#include "rsrc.h"     /* resource IDs */
  
/*----------------------------------------------------
    TYPE DEFINITIONS
  ----------------------------------------------------*/
typedef struct bpnode{
  UINT uLine;       /* source line of bp */
  PBYTE pAddress;   /* code address of bp */
  BYTE Opcode;      /* displaced instruction */
  BOOL bOnceOnly;   /* TRUE for soft break */
  struct bpnode *pNext; /* next node in list */
} BPNODE;
typedef BPNODE *PBPNODE;

/* NOTE: EVENT_RUNNING must be the first enum value
   EVENT_KILL must be the last enum value
*/
enum EVENTS {
  EVENT_RUNNING, EVENT_RESUME, EVENT_KILL    
};
#define NUM_COMMAND_EVENTS  3

/* flags for SetLinePrefix() */
enum PREFIX_MARK {
  PREFIX_BREAK_LINE, PREFIX_BREAKPOINT, PREFIX_CURRENT_LINE
};

#define ERROR_MSG(h) ShowErrorMsg((h), __FILE__, __LINE__)

/*----------------------------------------------------
        FUNCTION PROTOTYPES
  ----------------------------------------------------*/
/* STEPPER.C */
void SetLinePrefix(int iLine, int iPrefix);
void StartDebugging(HWND hWnd);
void SetWindowTitle(void);
void ShowErrorMsg(HWND hwndParent, LPSTR szFile, int iLine);

/* DEBUG.C */
LRESULT DebuggingThread(LPTSTR lpszFileName);
BOOL ToggleBreakpoint(UINT uLine);
BOOL SetAllBreakpoints(void);
BOOL ClearAllBreakpoints(void);
BOOL RunToLine(UINT uLine);
void MarkAllBreakLines(void);

/* BPNODE.C */
PBPNODE NewNode(void);
BOOL DeleteNode(PBPNODE pNode);
PBPNODE FirstNode(void);
PBPNODE NextNode(PBPNODE pNode);
PBPNODE LastNode(void);
PBPNODE FindBPbyAddress(PBYTE pAddress);
PBPNODE FindNextBPbyLine(PBPNODE pStart, UINT uLine);
void DestroyList(void);


