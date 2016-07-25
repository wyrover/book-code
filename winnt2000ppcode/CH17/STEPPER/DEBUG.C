/*----------------------------------------------------
    DEBUG.C
    Routines for the debugging thread of the 
    STEPPER program.

    (c) 1994 by Brian G. Myers
  ----------------------------------------------------*/
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>     /* _MAX_PATH */
#pragma hdrstop
#include "stepper.h"

/*----------------------------------------------------
    PROTOTYPES
  ----------------------------------------------------*/
BOOL LaunchDebuggee(LPTSTR pszFilePath,
  PPROCESS_INFORMATION ppi);
void CheckForDebugEvent(HANDLE hDebuggee);
BOOL DoExceptionEvent(LPDEBUG_EVENT pde, HANDLE hDebuggee);
PBPNODE OnBreakpointException(PBYTE pXAddress);
BOOL SetBreakpoint(HANDLE hProcess, PBPNODE pBP);
BOOL RemoveBreakpoint(HANDLE hProcess, PBPNODE pBP);
BOOL WriteOpcode(HANDLE hProcess, PBYTE pAddress,
  PBYTE pOpcode);
BOOL ReadOpcode(HANDLE hProcess, PBYTE pAddress,
  PBYTE pOpCode);
BOOL CanBreakOnLine(UINT uLine);
PBYTE GetNextAddressForLine(PINT pIndex, UINT uLine);
BOOL SetStepFlag(HANDLE hThread, BOOL bOn);
PBYTE GetIPAddress(HANDLE hThread);
BOOL DecrementIP(HANDLE hThread);

/*----------------------------------------------------
    TYPES AND DEFINITIONS
  ----------------------------------------------------*/
#define BP_OPCODE 0xCC  /* INT 3 instruction */
#define TF_BIT 0x100    /* single-step register bit */

typedef struct tagDEBUGGEE {
  PBYTE pStartAddress;  /* address of main() */
  HANDLE hProcess;      /* process under scrutiny */
  HANDLE hThread;       /* primary thread */
  DWORD dwProcessId;
  DWORD dwThreadId;
  BOOL bBreakSeen;  /* FALSE until first bp occurs */
} DEBUGGEE;
typedef DEBUGGEE *PDEBUGGEE;

/*----------------------------------------------------
    GLOBAL VARIABLES (all modules)
  ----------------------------------------------------*/
extern HWND ghMainWnd;
extern TCHAR gszAppTitle[];
extern BOOL gbAttached;
extern BOOL gbPaused;
extern HANDLE ghCommandEvent[NUM_COMMAND_EVENTS];

/*----------------------------------------------------
    STATIC VARIABLES (this module only)
  ----------------------------------------------------*/
DEBUGGEE Debuggee;
PBPNODE pbpPending = NULL;

/* FOO.EXE line information                      */
/* (assumes Foo was compiled with Borland tools) */
int LineAddress[] = {0x7C, 0x7D, 0x7F, 0x93, 0x98, 0x9A};
UINT BreakLines[] = {4, 7, 8, 9, 10, 0};
int iBreakLineCount = 6;

/*----------------------------------------------------
    DEBUGGING THREAD
  ----------------------------------------------------*/
LRESULT DebuggingThread( LPTSTR pszFilePath )
{
  PROCESS_INFORMATION pi; 
  TCHAR szMsg[_MAX_PATH + 25];
  int iCmdEvent;

  /* create the debuggee process */
  if (!LaunchDebuggee(pszFilePath, &pi)) {
    wsprintf( szMsg, TEXT("Cannot launch %s"),
              (PSTR)pszFilePath);
    MessageBox( ghMainWnd, szMsg, gszAppTitle,
                MB_OK | MB_ICONEXCLAMATION );
    gbAttached = FALSE;
  } else {
    gbAttached = TRUE;   /* creation succeeded */
    SetEvent(ghCommandEvent[EVENT_RUNNING]);
  }
  SetWindowTitle(); 

  __try {
    __try {
      while (gbAttached) {
        /* proceed only when a command event permits it */
        iCmdEvent = WaitForMultipleObjects(
          NUM_COMMAND_EVENTS,  (PHANDLE)&ghCommandEvent,
          FALSE, INFINITE);

        switch (iCmdEvent) {
   
          case EVENT_RUNNING:
            CheckForDebugEvent(pi.hProcess);
            break;
         
          case EVENT_RESUME:
            SetEvent(ghCommandEvent[EVENT_RUNNING]);  
            gbPaused = FALSE;
            SetWindowTitle();
            ContinueDebugEvent(Debuggee.dwProcessId,
              Debuggee.dwThreadId, DBG_CONTINUE);
            break;

          case EVENT_KILL:
            /* the termination handler cleans up */
            TerminateProcess(pi.hProcess, 0);
            gbAttached = FALSE;
            break;
            
        } /* end switch (iCmdEvent) */

      } /* end while (bAttached) */

    } __finally {
      /* clean up */
      ResetEvent(ghCommandEvent[EVENT_RUNNING]);            
      gbAttached = FALSE;
      SetWindowTitle();
      DestroyList();
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }

  }__except (EXCEPTION_EXECUTE_HANDLER) {
    MessageBox(ghMainWnd, "An unexpected error occurred.",
               gszAppTitle, MB_OK | MB_ICONSTOP);     
  }        
  return(0L);
}

/*----------------------------------------------------
    LAUNCH DEBUGGEE
  ----------------------------------------------------*/
BOOL LaunchDebuggee(
  LPTSTR pszFilePath, PPROCESS_INFORMATION ppi )
{
  STARTUPINFO sui;    
  BOOL bSuccess;
   
  /* fill in the process's startup information */
  ZeroMemory(&sui, sizeof(STARTUPINFO));
  sui.cb = sizeof(STARTUPINFO);
  sui.wShowWindow = SW_SHOWDEFAULT;
  sui.dwFlags = STARTF_USESHOWWINDOW;
  ppi->hProcess = NULL;

  /* create the debuggee process */
  bSuccess = CreateProcess(NULL, pszFilePath, 
    NULL, NULL, FALSE, DEBUG_PROCESS |
    DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &sui, ppi);
  if (!bSuccess) ERROR_MSG(ghMainWnd);
      
  return(bSuccess);
}

/*----------------------------------------------------
    CHECK FOR DEBUG EVENT
  ----------------------------------------------------*/
void CheckForDebugEvent(HANDLE hDebuggee)
{
  DEBUG_EVENT DebugEvent;
  BOOL bContinue = TRUE;
   
  /* wait up to 100 ms for a debug event to occur */
  if (WaitForDebugEvent(&DebugEvent, 100)) {
      
    /* determine what event occurred */
    switch (DebugEvent.dwDebugEventCode) {
         
      case EXCEPTION_DEBUG_EVENT:
        bContinue = DoExceptionEvent(&DebugEvent, hDebuggee);
        break;

      case CREATE_PROCESS_DEBUG_EVENT:
		 gbAttached = TRUE;
        Debuggee.pStartAddress =
          (PBYTE)DebugEvent.u.CreateProcessInfo.lpStartAddress;
        Debuggee.hProcess = hDebuggee;
        Debuggee.dwProcessId = DebugEvent.dwProcessId;
        Debuggee.hThread = DebugEvent.u.CreateProcessInfo.hThread;
        Debuggee.dwThreadId = DebugEvent.dwThreadId;
        Debuggee.bBreakSeen = FALSE;
        break;
           
      case EXIT_PROCESS_DEBUG_EVENT:
        gbAttached = FALSE;
        SetWindowTitle();
        
        /* The SetWindowPos commands work around a glitch */
        /* I've observed where the EXIT_PROCESS event     */
        /* somehow makes the STEPPER window go to the     */
        /* back of the Z-order and disappear behind other */
        /* windows.  It needs to come forward instead.    */
        
        SetWindowPos(ghMainWnd, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE);
        MessageBox(ghMainWnd, "The program ended.",
                   gszAppTitle, MB_OK | MB_ICONINFORMATION);    
        SetWindowPos(ghMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE);
        break;
               
    } /* end switch (EventCode) */

    /* Unless the debuggee is paused at a */
    /* breakpoint, resume execution of debuggee */
    if (bContinue) 
      ContinueDebugEvent(DebugEvent.dwProcessId,
        DebugEvent.dwThreadId, DBG_CONTINUE);
    else {
      gbPaused = TRUE;
      SetWindowTitle();
      ResetEvent(ghCommandEvent[EVENT_RUNNING]);
    }
  }     
}
 
/*----------------------------------------------------
    DO EXCEPTION EVENT
    Respond to EXCEPTION_DEBUG_EVENTS, particularly
    breakpoints.  Return TRUE if the debug loop
    may continue immediately to the next event, FALSE
    if it should wait for the user to resume.
  ----------------------------------------------------*/
BOOL DoExceptionEvent(
  LPDEBUG_EVENT pde, HANDLE hDebuggee )
{
  BOOL bContinue;   
  UINT uXCode =
    pde->u.Exception.ExceptionRecord.ExceptionCode;
  PBYTE pXAddress =
    pde->u.Exception.ExceptionRecord.ExceptionAddress;

  switch (uXCode) {
   
    case EXCEPTION_BREAKPOINT:
      pbpPending = OnBreakpointException(pXAddress);
      bContinue = FALSE;
      break;
         
    case EXCEPTION_SINGLE_STEP:
      /* end single-step mode */
      SetStepFlag(Debuggee.hThread, FALSE);
        
      /* restore the breakpoint we just stepped over */
      if (pbpPending)
        SetBreakpoint(hDebuggee, pbpPending);
      pbpPending = NULL;
      bContinue = TRUE;
      break;
  }
  return(bContinue);
}

/*----------------------------------------------------
    ON BREAKPOINT EXCEPTION
  ----------------------------------------------------*/
PBPNODE OnBreakpointException( PBYTE pXAddress )
{
  PBPNODE pBP;
  PBPNODE pbpPassed = NULL;
   
  /* The first breakpoint is supplied by */
  /* NT when the program loads */
  if (!Debuggee.bBreakSeen) {
    Debuggee.bBreakSeen = TRUE;
    return(NULL);
  }
   
  /* is this a known breakpoint? */
  pBP = FindBPbyAddress(pXAddress);            

  /* has the debuggee stopped on a known breakpoint? */
  if (pBP) {
    SetLinePrefix(pBP->uLine, PREFIX_CURRENT_LINE);
            
    /* get the INT3 opcode out of there */
    RemoveBreakpoint(Debuggee.hProcess, pBP);

    if (!pBP->bOnceOnly) {
      /* For a hard break, turn on single-stepping */
      /* to restore the INT 3 opcode later */
      SetStepFlag(Debuggee.hThread, TRUE);
      pbpPassed = pBP;
    } else 
      /* for a soft break, remove all traces */
      DeleteNode(pBP);
      
      /* Reset the IP to execute the instruction */
      /* displaced by the INT 3 opcode */
      DecrementIP(Debuggee.hThread);

  } else { /* unknown breakpoint */
    SetLinePrefix(0, PREFIX_CURRENT_LINE);
  }
  
  /* Return pointer to the breakpoint if it must */
  /* be restored after the next single-step exception */
  return(pbpPassed);
}

/*----------------------------------------------------
    TOGGLE BREAKPOINT
  ----------------------------------------------------*/
BOOL ToggleBreakpoint( UINT uLine )
{
  PBPNODE pBP;
  BOOL bFound = FALSE;

  if (!CanBreakOnLine(uLine))
    return(FALSE);
      
  /* search to end of list for */
  /* hard breaks on uLine */
   
  pBP = FindNextBPbyLine(NULL, uLine);
  while (pBP != NULL) {
    if (!pBP->bOnceOnly) {
      bFound = TRUE;
      RemoveBreakpoint(Debuggee.hProcess, pBP);
      DeleteNode(pBP);
      if (pBP == pbpPending)
        pbpPending = NULL;
      SetLinePrefix(pBP->uLine, PREFIX_BREAK_LINE);
    }
    /* any more breakpoints on this line? */
    pBP = FindNextBPbyLine(NULL, uLine);
  }

  /* if no breakpoints were found, create one */
  if (!bFound) {
    int iIndex = 0;
    PBYTE pAddress;
      
    pAddress = GetNextAddressForLine(&iIndex, uLine);
    while (pAddress) {
      pBP = NewNode();
      if (!pBP) return(FALSE);
      pBP->uLine = uLine;
      pBP->pAddress = pAddress;
      pBP->bOnceOnly = FALSE;
      if (!SetBreakpoint(Debuggee.hProcess, pBP)) {
        DeleteNode(pBP);  
        return(FALSE);
      }
      pAddress = GetNextAddressForLine(&iIndex, uLine);
    }
    SetLinePrefix(pBP->uLine, PREFIX_BREAKPOINT);
  }  
  return(TRUE);
}

/*----------------------------------------------------
    RUN TO LINE
  ----------------------------------------------------*/
BOOL RunToLine( UINT uLine )
{
  int iIndex = 0;
  PBYTE pAddress;
  PBPNODE pBP;

  if (!CanBreakOnLine(uLine))
    return(FALSE);

  pAddress = GetNextAddressForLine(&iIndex, uLine);
  if (pAddress == GetIPAddress(Debuggee.hThread)) 
    return(TRUE);
      
  pBP = NewNode();
  if (!pBP) return(FALSE);
  pBP->uLine = uLine;
  pBP->pAddress = pAddress;
  pBP->bOnceOnly = TRUE;
  if (!SetBreakpoint(Debuggee.hProcess, pBP)) {
    DeleteNode(pBP);  
    return(FALSE);
  }
  /* do not show breakpoint glyph on screen */
   
  /* initiate the Run command */
  FORWARD_WM_COMMAND(ghMainWnd, CMD_RUN, NULL,
    0, SendMessage);
  return(TRUE);
}
  
/*----------------------------------------------------
    SET ALL BREAKPOINTS
  ----------------------------------------------------*/
BOOL SetAllBreakpoints( void )
{  
  PBPNODE pBP;
  int i;

  for (i=0; i<iBreakLineCount; i++) {
    pBP = NewNode();
    if (!pBP) return(FALSE);
    pBP->uLine = BreakLines[i];
    pBP->pAddress = Debuggee.pStartAddress + LineAddress[i];
    pBP->bOnceOnly = FALSE;
    if (!SetBreakpoint(Debuggee.hProcess, pBP)) {
      DeleteNode(pBP);
      return(FALSE);
    }
    SetLinePrefix(pBP->uLine, PREFIX_BREAKPOINT);
  }
  return(TRUE);
}

/*----------------------------------------------------
    CLEAR ALL BREAKPOINTS
  ----------------------------------------------------*/
BOOL ClearAllBreakpoints( void )
{
  PBPNODE pTemp, pBP;
  BOOL bSuccess = TRUE;
   
  pBP = FirstNode();
  while (pBP && bSuccess) {
    bSuccess = RemoveBreakpoint(Debuggee.hProcess, pBP);
    if (bSuccess) {
      /* remove the breakpoint glyph */
      if (pBP->uLine != 0)
        SetLinePrefix(pBP->uLine, PREFIX_BREAK_LINE);

      /* remember not to restore bp after */
      /* next single-step */
      if (pBP == pbpPending)
        pbpPending = NULL;
            
      pTemp = NextNode(pBP);
      DeleteNode(pBP);
      pBP = pTemp;
    }
  }
  return(bSuccess);
}

/*----------------------------------------------------
    SET BREAKPOINT
    Write a breakpoint instruction into the debuggee's
    code.  Save the original instruction first.
  ----------------------------------------------------*/
BOOL SetBreakpoint( HANDLE hProcess, PBPNODE pBP )
{
  BOOL bSuccess;
  BYTE byOpcode = BP_OPCODE;

  __try {
    bSuccess = ReadOpcode(hProcess, pBP->pAddress,
      &pBP->Opcode);
    if (bSuccess)
      bSuccess = WriteOpcode(hProcess, pBP->pAddress,
        &byOpcode);
         
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    bSuccess = FALSE;
  }
   
  return(bSuccess);
}

/*----------------------------------------------------
    REMOVE BREAKPOINT
    Remove a breakpoint instruction from the
    debuggee code.  (Does not remove the BPNODE
    structure from the list of breakpoints.)
  ----------------------------------------------------*/
BOOL RemoveBreakpoint( HANDLE hProcess, PBPNODE pBP )
{
  BOOL bSuccess;
  __try {
   bSuccess = WriteOpcode(hProcess, pBP->pAddress,
        &pBP->Opcode);
            
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    bSuccess = FALSE;
  }
  return(bSuccess);
}

/*----------------------------------------------------
    WRITE OPCODE
	 Write a byte into the address space of the given
	 process.  (Call this to insert breakpoints.)
  ----------------------------------------------------*/
BOOL WriteOpcode(
  HANDLE hProcess, PBYTE pAddress, PBYTE pOpcode )
{
  BOOL bSuccess;
  DWORD dwBytes;
  DWORD dwNewFlags, dwOldFlags;

  /* change mem protection in debuggee for writing */
  bSuccess = VirtualProtectEx(hProcess, pAddress,
    1L, PAGE_READWRITE, &dwOldFlags);
  if (!bSuccess) {
    ERROR_MSG(ghMainWnd);
    return(FALSE);
  }

  __try {
    __try {
      /* write new byte to memory */
      bSuccess = WriteProcessMemory(hProcess,
        pAddress, pOpcode, 1L, &dwBytes);
         
    } __except (EXCEPTION_EXECUTE_HANDLER) {
      bSuccess = FALSE;
    }
      
  } __finally {
    /* restore original protection */
    dwNewFlags = dwOldFlags;
    VirtualProtectEx(hProcess, pAddress, 1L,
      dwNewFlags, &dwOldFlags);
  }
      
  if (!bSuccess || (dwBytes != 1)) 
    ERROR_MSG(ghMainWnd);                     
  return(bSuccess);          
}

/*----------------------------------------------------
    READ OPCODE
    Read one byte from the address space of the given
    process.  (Call this to save an instruction 
    before over-writing it.)
  ----------------------------------------------------*/
BOOL ReadOpcode(
  HANDLE hProcess, PBYTE pAddress, PBYTE pOpCode )
{
  BOOL bSuccess;
  DWORD dwBytes;

  __try {
    bSuccess = ReadProcessMemory(hProcess,
      pAddress, pOpCode, 1L, &dwBytes);
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    bSuccess = FALSE;
  }
   
  if (!bSuccess || (dwBytes != 1))
    ERROR_MSG(ghMainWnd);      
  return(bSuccess);          
}

/*----------------------------------------------------
    CAN BREAK ON LINE
    Return TRUE if it is possible to set a breakpoint
    on the given line of source code.
  ----------------------------------------------------*/
BOOL CanBreakOnLine( UINT uLine )
{
  int i = 0;
  BOOL bFound = FALSE;
   
  while ((i<iBreakLineCount) && !bFound)
    bFound = (uLine == BreakLines[i++]);
      
  return(bFound);
}

/*----------------------------------------------------
    GET NEXT ADDRESS FOR LINE
    Starting from the given array index, search for
    the next machine code address associated with
    a given line number.  Use this function to find
    all the stoppable instructions that fall on a
    particular source code line.
  ----------------------------------------------------*/
PBYTE GetNextAddressForLine( PINT pIndex, UINT uLine )
{
  PBYTE pAddress = NULL;
  BOOL bFound = FALSE;
  int iIndex = *pIndex;
   
  iIndex = max(iIndex, 0);      /* assert *pIndex >= 0 */
   
  while (!bFound && (iIndex < iBreakLineCount)) 
    bFound = (BreakLines[iIndex++] == uLine);

  if (bFound) {
    *pIndex = iIndex--;
    pAddress = LineAddress[iIndex] + Debuggee.pStartAddress;
  }
  return(pAddress);
}

/*----------------------------------------------------
    MARK ALL BREAK LINES
    Place a PREFIX_BREAK_LINE character in front of
    each source line where a breakpoint may be set.
  ----------------------------------------------------*/
void MarkAllBreakLines( void )
{
  int i;
  for (i=0; i<iBreakLineCount; i++)
    SetLinePrefix(BreakLines[i], PREFIX_BREAK_LINE);
}


/*****************************************************
      THE FOLLOWING ROUTINES ASSUME AN INTEL CPU
******************************************************/
   

/*----------------------------------------------------
    SET STEP FLAG
    Turn the TF (trap flag) off or on to enable or
    disable single-stepping mode.
  ----------------------------------------------------*/
BOOL SetStepFlag( HANDLE hThread, BOOL bOn )
{
  CONTEXT Context;
  BOOL bSuccess;

  Context.ContextFlags = CONTEXT_CONTROL;

  __try {
    bSuccess = GetThreadContext(hThread, &Context);
    if (bSuccess) {
      if (bOn)
        Context.EFlags |= TF_BIT;
      else
        Context.EFlags &= ~TF_BIT;    
        bSuccess = SetThreadContext(hThread, &Context);
    }
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    bSuccess = FALSE;
  }
  return(bSuccess);
}

/*----------------------------------------------------
    GET IP ADDRESS
    Return the current value of the instruction
    pointer from the context of a given thread.
  ----------------------------------------------------*/
PBYTE GetIPAddress( HANDLE hThread )
{
  CONTEXT Context;
   
  __try {  
    Context.ContextFlags = CONTEXT_CONTROL;
    GetThreadContext(hThread, &Context);
      
  } __except (EXCEPTION_EXECUTE_HANDLER) {
    Context.Eip = 0L;
  }
  return((PBYTE)Context.Eip);   
}

/*----------------------------------------------------
      DECREMENT IP
      Set the instruction pointer back one byte.
  ----------------------------------------------------*/
BOOL DecrementIP( HANDLE hThread )
{
  CONTEXT Context;
  BOOL bSuccess;

  Context.ContextFlags = CONTEXT_CONTROL;
  bSuccess = GetThreadContext(hThread, &Context);
  if (bSuccess) {
    Context.Eip--;
    bSuccess = SetThreadContext(hThread, &Context);
  }
  return(bSuccess);
}
