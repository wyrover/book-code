
#include "pwalk.h"
#include <memory.h>


/* module globals */
LPVOID	  PMAAddress;
LPVOID	  PMABuffer;
DWORD	  PMASize;
extern	  char	  szCurPath[MAX_PATH];


/* local debug functions */
BOOL   WINAPI DebugEventThread (DBGPROCESS *);
BOOL   WINAPI CreateDebugEvents (LPHANDLE);
void   WINAPI AddThreadNode (DBGPROCESS *, DWORD, HANDLE, int, LPTHREAD_START_ROUTINE, HANDLE);
int    WINAPI HandleBreakPoint(DBGPROCESS *, DWORD);
void   WINAPI RemoveThreadNode (DBGPROCESS *, DWORD);
void   WINAPI AddDllNode (DBGPROCESS *, LOAD_DLL_DEBUG_INFO *);
void   WINAPI RemoveDllNode (DBGPROCESS *, LOAD_DLL_DEBUG_INFO *);
void   WINAPI SuspendDebuggeeProcess (DBGPROCESS *);
void   WINAPI ResumeDebuggeeProcess (DBGPROCESS *);
void   WINAPI NameObjects (HANDLE, LPVOID, LPVMOBJECT, int, char *, char *);
BOOL   WINAPI VMCompare (LPVMOBJECT, LPVMOBJECT);
BOOL   WINAPI InterruptThread_HookProcess (DBGPROCESS *, LPPROCESS_STATE);
void   WINAPI ResetInterruptedThread (DBGPROCESS *, LPPROCESS_STATE);
void   WINAPI RecordException (DBGPROCESS *, DEBUG_EVENT *);
HANDLE WINAPI FindThreadHandle (DBGPROCESS *, DWORD);



/* start debug thread, and return event active handle */
DBGPROCESS* WINAPI StartChildProcess (
    HWND	hWnd,
    char	*lpszModule,
    LPHANDLE	lpDbgEvents)
{
    DWORD	  TID;
    HANDLE	  hDebugHeap;
    DBGPROCESS	  *lpDbgProcess;
    int 	  i;

    /* create unique debug events using debuggee process ID */
    if (!CreateDebugEvents (lpDbgEvents))
	return NULL;

    /* create serialized heap of dynamic size */
    if (!(hDebugHeap = HeapCreate (0, sizeof (DBGPROCESS) + sizeof (DBGTHREAD), 0)))
	{
	/* close all event handles */
	for (i=0; i<nDEBUGEVENTS; i++)
	    CloseHandle (lpDbgEvents[i]);

	return NULL;
	}

    /* allocate and initialize debug heap structure */
    lpDbgProcess = (DBGPROCESS *)HeapAlloc (hDebugHeap, 0, sizeof (DBGPROCESS));
    lpDbgProcess->hDbgHeap = hDebugHeap;
    strcpy (lpDbgProcess->szModule, lpszModule);
    lpDbgProcess->hWnd = hWnd;
    lpDbgProcess->lpThreads = NULL;
    lpDbgProcess->lpSection = NULL;
    lpDbgProcess->lpERs = NULL;

    /* create debug thread */
    if (!(CreateThread ((LPSECURITY_ATTRIBUTES)NULL,
			4096,
			(LPTHREAD_START_ROUTINE)DebugEventThread,
			(LPVOID)lpDbgProcess,
			0,
			&TID)))
	return NULL;

    /* wait 15 seconds for debugger to complete initialization, else error */
    if (WAIT_TIMEOUT == WaitForSingleObject (lpDbgEvents[ACKNOWLEDGE], 15000))
	{
	HeapDestroy (lpDbgProcess->hDbgHeap);

	/* close all event handles */
	for (i=0; i<nDEBUGEVENTS; i++)
	    CloseHandle (lpDbgEvents[i]);

	return NULL;
	}

    /* reset acknowledge event */
    ResetEvent (lpDbgEvents[ACKNOWLEDGE]);

    /* successfull thread and event start */
    return lpDbgProcess;
}




/* function notifies debug thread to terminate, frees handles, and destroys heap */
void   WINAPI CloseChildProcess (
    DBGPROCESS *lpDbgProcess,
    LPHANDLE   lpDbgEvents)
{
    int 	 i;
    DBGTHREAD	 *pNode = lpDbgProcess->lpThreads;

    /* set close event for debug thread and wait for acknowledge */
    SetEvent (lpDbgEvents[CLOSEDEBUGGER]);
    WaitForSingleObject (lpDbgEvents[ACKNOWLEDGE], INFINITE);

    /* close all event handles */
    for (i=0; i<nDEBUGEVENTS; i++)
	CloseHandle (lpDbgEvents[i]);

    /* close all thread handles in the list */
    while (pNode != NULL)
	{
	RemoveThreadNode (lpDbgProcess, pNode->dwThreadID);
	pNode = lpDbgProcess->lpThreads;
	}

    /* destroy the debug heap */
    HeapDestroy (lpDbgProcess->hDbgHeap);
}




/* local function creates debug event objects for thread synchronization */
BOOL WINAPI CreateDebugEvents (
    LPHANDLE	lpDbgEvents)
{
    char    szEvent[MAX_PATH];


    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTACTIVE, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[DEBUGACTIVE] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						  TRUE,
						  TRUE,
						  szEvent)))
	return FALSE;

    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTCLOSE, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[CLOSEDEBUGGER] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						   TRUE,
						   FALSE,
						   szEvent)))
	{
	CloseHandle (lpDbgEvents[DEBUGACTIVE]);
	return FALSE;
	}

    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTSTOP, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[SUSPENDDEBUGGER] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						      TRUE,
						      FALSE,
						      szEvent)))
	{
	CloseHandle (lpDbgEvents[DEBUGACTIVE]);
	CloseHandle (lpDbgEvents[CLOSEDEBUGGER]);
	return FALSE;
	}

    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTSTART, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[RESUMEDEBUGGER] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						     TRUE,
						     FALSE,
						     szEvent)))
	{
	CloseHandle (lpDbgEvents[DEBUGACTIVE]);
	CloseHandle (lpDbgEvents[CLOSEDEBUGGER]);
	CloseHandle (lpDbgEvents[SUSPENDDEBUGGER]);
	return FALSE;
	}

    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTREAD, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[READMEMORY] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						 TRUE,
						 FALSE,
						 szEvent)))
	{
	CloseHandle (lpDbgEvents[DEBUGACTIVE]);
	CloseHandle (lpDbgEvents[CLOSEDEBUGGER]);
	CloseHandle (lpDbgEvents[SUSPENDDEBUGGER]);
	CloseHandle (lpDbgEvents[RESUMEDEBUGGER]);
	return FALSE;
	}

    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTWRITE, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[WRITEMEMORY] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						  TRUE,
						  FALSE,
						  szEvent)))
	{
	CloseHandle (lpDbgEvents[DEBUGACTIVE]);
	CloseHandle (lpDbgEvents[CLOSEDEBUGGER]);
	CloseHandle (lpDbgEvents[SUSPENDDEBUGGER]);
	CloseHandle (lpDbgEvents[RESUMEDEBUGGER]);
	CloseHandle (lpDbgEvents[READMEMORY]);
	return FALSE;
	}

    LoadString (GetModuleHandle (NULL), IDS_DBGEVNTACK, szEvent, sizeof (szEvent));
    if (!(lpDbgEvents[ACKNOWLEDGE] = CreateEvent ((LPSECURITY_ATTRIBUTES)NULL,
						  TRUE,
						  FALSE,
						  szEvent)))
	{
	CloseHandle (lpDbgEvents[DEBUGACTIVE]);
	CloseHandle (lpDbgEvents[CLOSEDEBUGGER]);
	CloseHandle (lpDbgEvents[SUSPENDDEBUGGER]);
	CloseHandle (lpDbgEvents[RESUMEDEBUGGER]);
	CloseHandle (lpDbgEvents[READMEMORY]);
	CloseHandle (lpDbgEvents[WRITEMEMORY]);
	return FALSE;
	}

    /* success */
    return TRUE;
}





/* main daddyo thread that is the debugger residing over a debuggee */
BOOL WINAPI DebugEventThread (
    DBGPROCESS	  *lpDbgProcess)
{
    DEBUG_EVENT 	   de;
    HANDLE		   hDbgEvent[nDEBUGEVENTS];
    STARTUPINFO 	   si;
    PROCESS_INFORMATION    pi;
    HANDLE		   hChildProcess;
    BOOL		   bHooked, bUnHooked;
//    PROCESS_STATE	   ProcessState;


    bHooked = FALSE;
    bUnHooked = FALSE;

    /* initialize process startup information */
    si.cb	       = sizeof (si);
    si.lpReserved      = NULL;
    si.lpDesktop       = NULL;
    si.lpTitle	       = NULL;
    si.dwX	       = 0;
    si.dwY	       = 0;
    si.dwXSize	       = 0;
    si.dwYSize	       = 0;
    si.dwXCountChars   = 0;
    si.dwYCountChars   = 0;
    si.dwFillAttribute = 0;
    si.dwFlags	       = STARTF_FORCEONFEEDBACK | STARTF_USESHOWWINDOW;
    si.wShowWindow     = SW_SHOWNORMAL;
    si.cbReserved2     = 0;
    si.lpReserved2     = NULL;

    /* create debug process on module name */
    if (!CreateProcess (lpDbgProcess->szModule,
			NULL,
			(LPSECURITY_ATTRIBUTES)NULL,
			(LPSECURITY_ATTRIBUTES)NULL,
			FALSE,
			DEBUG_PROCESS,
			NULL,
			NULL,
			(LPSTARTUPINFO)&si,
			(LPPROCESS_INFORMATION)&pi))
	{
	ReportError (IDS_ERRCREATEPROCESS);
	return FALSE;
	}
    /* open process for all access */
    if ((hChildProcess = OpenProcess (PROCESS_ALL_ACCESS,
				      FALSE,
				      pi.dwProcessId)) == NULL)
	{
	ReportError (IDS_ERROPENPROCESS);
	TerminateProcess (pi.hProcess, 0);
	return FALSE;
	}

    /* store process info */
    lpDbgProcess->hProcess = hChildProcess;
    lpDbgProcess->dwPriority = GetPriorityClass (pi.hProcess);
    lpDbgProcess->dwProcessID = pi.dwProcessId;
    lpDbgProcess->bActive = TRUE;

    /* close process and thread handles in pi structure */
    CloseHandle (pi.hThread);
    CloseHandle (pi.hProcess);

    /* open debug events */
    CreateDebugEvents (hDbgEvent);

    /* signale completion of task */
    SetEvent (hDbgEvent[ACKNOWLEDGE]);

    /* start debug event loop */
    while (TRUE)
	{
	int    nIndex;

	/* wait for debugger active */
	switch (nIndex = WaitForMultipleObjects (nDEBUGEVENTS, hDbgEvent, FALSE, INFINITE))
	    {
	    case CLOSEDEBUGGER:
		{
		int    i;

		/* terminate debuggee process */
		TerminateProcess (lpDbgProcess->hProcess, 0);
		CloseHandle (lpDbgProcess->hProcess);

		/* signal completion of task */
		SetEvent (hDbgEvent[ACKNOWLEDGE]);

		/* close all debug events */
		for (i=0; i<nDEBUGEVENTS; i++)
		    CloseHandle (hDbgEvent[i]);

		/* exit debugger now */
		return TRUE;
		}
		break;

	    case SUSPENDDEBUGGER:
		SuspendDebuggeeProcess (lpDbgProcess);
		ResetEvent (hDbgEvent[DEBUGACTIVE]);
		ResetEvent (hDbgEvent[SUSPENDDEBUGGER]);
		break;

	    case RESUMEDEBUGGER:
		ResumeDebuggeeProcess (lpDbgProcess);
		SetEvent (hDbgEvent[DEBUGACTIVE]);
		ResetEvent (hDbgEvent[RESUMEDEBUGGER]);
		break;

	    case READMEMORY:
		{
		MEMORY_BASIC_INFORMATION    mbi;
		DWORD			    Protect = 0;

		/* reset event so we don't do repeat */
		ResetEvent (hDbgEvent [READMEMORY]);

		/* if not committed memory abort */
		if (!VirtualQueryEx (lpDbgProcess->hProcess,
				     PMAAddress,
				     &mbi,
				     sizeof (MEMORY_BASIC_INFORMATION)) ||
		    mbi.State != MEM_COMMIT)
		    {
		    PMASize = 0;
		    SetEvent (hDbgEvent [ACKNOWLEDGE]);
		    break;
		    }

		/* if guarded memory, change protection temporarily */
		if (!(mbi.Protect & PAGE_READONLY) &&
		    !(mbi.Protect & PAGE_READWRITE))
		    VirtualProtectEx (lpDbgProcess->hProcess,
				      PMAAddress,
				      PMASize,
				      PAGE_READONLY,
				      &Protect);

		if (!ReadProcessMemory (lpDbgProcess->hProcess,
					PMAAddress,
					PMABuffer,
					PMASize,
					NULL))
		    {
		    if (mbi.AllocationProtect != PAGE_READONLY &&
			mbi.AllocationProtect != PAGE_READWRITE)
			NotifyUser (NULL, IDS_ERROR, 0, "BaseProtect NOACCESS", 0);
		    else
			{
			ReportError (IDS_ERRREADPROCESSMEMORY);
			PMASize = 0;
			}
		    }

		/* reset protection if changed */
		if (Protect)
		    VirtualProtectEx (lpDbgProcess->hProcess,
				      PMAAddress,
				      PMASize,
				      Protect,
				      &Protect);

		/* acknowledge success */
		SetEvent (hDbgEvent [ACKNOWLEDGE]);
		}
		break;

	    case WRITEMEMORY:
		if (!WriteProcessMemory (lpDbgProcess->hProcess,
					 PMAAddress,
					 PMABuffer,
					 PMASize,
					 NULL))
		    {
		    ReportError (IDS_ERRWRITEPROCESSMEMORY);
		    PMASize = 0;
		    }

		ResetEvent (hDbgEvent [WRITEMEMORY]);
		SetEvent (hDbgEvent [ACKNOWLEDGE]);
		break;

	    case DEBUGACTIVE:
		/* if debug active */
		if ((WaitForDebugEvent (&de, (DWORD)100)))
		    {
		    if (de.dwProcessId == lpDbgProcess->dwProcessID)
			{
			switch (de.dwDebugEventCode)
			    {
			    case EXIT_PROCESS_DEBUG_EVENT:
				SetStatusText (lpDbgProcess->hWnd, IDS_EXITPROCESS, 0);

				/* uninitialize probe dll */
				ResetProbe ();

				/* process is going away so notify main window */
				SendNotifyMessage (lpDbgProcess->hWnd,
						   WM_COMMAND,
						   IDM_PROCESSUNLOAD,
						   0);
				break;

			    case LOAD_DLL_DEBUG_EVENT:
				SetStatusText (lpDbgProcess->hWnd, IDS_LOADDLL, 0);
				AddDllNode (lpDbgProcess,
					    (LOAD_DLL_DEBUG_INFO *)&(de.u.LoadDll));
				break;

			    case UNLOAD_DLL_DEBUG_EVENT:
				SetStatusText (lpDbgProcess->hWnd, IDS_UNLOADDLL, 0);
				RemoveDllNode (lpDbgProcess,
					       (LOAD_DLL_DEBUG_INFO *)&(de.u.LoadDll));
				break;

			    case CREATE_PROCESS_DEBUG_EVENT:
				SetStatusText (lpDbgProcess->hWnd, IDS_CREATEPROCESS, 0);
				/* add first thread to linked list of dbg structures */
				AddThreadNode (lpDbgProcess,
						de.dwThreadId,
						de.u.CreateProcessInfo.hThread,
						GetThreadPriority (de.u.CreateProcessInfo.hThread),
						de.u.CreateProcessInfo.lpStartAddress,
						de.u.CreateProcessInfo.hFile);
				lpDbgProcess->hFile = de.u.CreateProcessInfo.hFile;
				lpDbgProcess->lpImage = de.u.CreateProcessInfo.lpBaseOfImage;
				lpDbgProcess->dwDbgInfoOffset = de.u.CreateProcessInfo.dwDebugInfoFileOffset;
				lpDbgProcess->nDbgInfoSize = de.u.CreateProcessInfo.nDebugInfoSize;
				break;

			    case CREATE_THREAD_DEBUG_EVENT:
				SetStatusText (lpDbgProcess->hWnd, IDS_CREATETHREAD, 0);
				/* add thread to linked list of dbg structures */
				AddThreadNode (lpDbgProcess,
					 de.dwThreadId,
					 de.u.CreateThread.hThread,
					 GetThreadPriority (de.u.CreateThread.hThread),
					 de.u.CreateThread.lpStartAddress,
					 NULL);
				break;

			    case EXIT_THREAD_DEBUG_EVENT:
				SetStatusText (lpDbgProcess->hWnd, IDS_EXITTHREAD, 0);
				/* remove thread record */
				RemoveThreadNode (lpDbgProcess,
					    de.dwThreadId);
				break;

			    case EXCEPTION_DEBUG_EVENT:
				switch (de.u.Exception.ExceptionRecord.ExceptionCode)
				    {
				    case EXCEPTION_BREAKPOINT:

					HandleBreakPoint(lpDbgProcess, de.dwThreadId);
					SetStatusText (lpDbgProcess->hWnd,
						       IDS_BREAKPOINTEXCEPTION,
						       RGB (0, 0xff, 0));

					/* post message to get ball rolling in main thread */
					PostMessage (lpDbgProcess->hWnd,
							 WM_COMMAND,
							 IDM_PROCESSREWALK,
							 0);
					break;

				    case EXCEPTION_ACCESS_VIOLATION:
					/* record exception information */
					SetStatusText (lpDbgProcess->hWnd,
						       IDS_ACCESSVIOLATIONEXCEPTION,
						       RGB (0xff, 0, 0));
					RecordException (lpDbgProcess, &de);
					break;

				    default:
					SetStatusText (lpDbgProcess->hWnd,
						       IDS_UNHANDLEDEXCEPTION,
						       RGB (0xff, 0, 0));
					RecordException (lpDbgProcess, &de);
					break;
				    }
				break;

			    case RIP_EVENT:
				SetStatusText (lpDbgProcess->hWnd,
					       IDS_RIPEVENT,
					       RGB (0, 0xff, 0));
				break;

			    case OUTPUT_DEBUG_STRING_EVENT:
				SetStatusText (lpDbgProcess->hWnd,
					       IDS_OUTPUTDEBUGSTRING,
					       RGB (0, 0xff, 0));
				break;

			    default:
				break;
			    }
			}

            if(de.u.Exception.ExceptionRecord.ExceptionCode != EXCEPTION_BREAKPOINT)
		        ContinueDebugEvent (de.dwProcessId, de.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
            else
		        ContinueDebugEvent (de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
		    }
		break;
	    }

	}

    return TRUE;
}




void WINAPI SuspendDebuggeeProcess (
    DBGPROCESS	  *lppr)
{
    DBGTHREAD	 *lpth = (DBGTHREAD *)lppr->lpThreads;

    while (lpth)
	{
	SuspendThread (lpth->hThread);
	lpth = (DBGTHREAD *)lpth->Next;
	}

    /* inform user via status bar */
    SetStatusText (lppr->hWnd, IDS_PROCESSSUSPENDED, RGB (0xff, 0, 0));
    lppr->bActive = FALSE;
}




void WINAPI ResumeDebuggeeProcess (
    DBGPROCESS	  *lppr)
{
    DBGTHREAD	 *lpth = (DBGTHREAD *)lppr->lpThreads;

    while (lpth)
	{
	ResumeThread (lpth->hThread);
	lpth = (DBGTHREAD *)lpth->Next;
	}

    /* inform user via status bar */
    SetStatusText (lppr->hWnd, IDS_PROCESSRESUMED, RGB (0, 0xff, 0));
    lppr->bActive = TRUE;
}




void WINAPI AddThreadNode (
    DBGPROCESS		      *lppr,
    DWORD		      dwThreadID,
    HANDLE		      hThread,
    int 		      nPriority,
    LPTHREAD_START_ROUTINE    lpStart,
    HANDLE		      hFile)
{
    DBGTHREAD	 *lpth;
    DBGTHREAD	 *pNode = lppr->lpThreads;

    /* allocate thread node off heap */
    lpth = (DBGTHREAD *)HeapAlloc (lppr->hDbgHeap, 0, sizeof (DBGTHREAD));

    /* initialize thread data */
    lpth->lpStartAddress = lpStart;
    lpth->nPriority = nPriority;
    lpth->dwThreadID = dwThreadID;
    lpth->hThread = hThread;
    lpth->Next = NULL;

    /* retieve section names for executable module if file handle passed */
    if (hFile)
	{
	RetrieveModuleName (lppr->szModule, hFile);
	lppr->lpSection = NULL;
	RetrieveSectionNames (lppr->hDbgHeap, hFile, &(lppr->lpSection));
	}

    /* set linked list pointers */
    while (pNode && pNode->Next)
	pNode = (DBGTHREAD *)pNode->Next;

    if (!pNode)
	lppr->lpThreads = lpth;
    else
	(DBGTHREAD *)pNode->Next = lpth;
}




void WINAPI RemoveThreadNode (
    DBGPROCESS	  *lppr,
    DWORD	  dwThreadID)
{
    DBGTHREAD	 *pNode = lppr->lpThreads;
    DBGTHREAD	 *lpth;

    while (pNode->Next &&
	   ((DBGTHREAD *)(pNode->Next))->dwThreadID != dwThreadID)
	pNode = (DBGTHREAD *)pNode->Next;

    if (pNode->Next)
	{
	lpth = (DBGTHREAD *)pNode->Next;
	pNode->Next = ((DBGTHREAD *)(pNode->Next))->Next;
	CloseHandle (lpth->hThread);
	HeapFree (lppr->hDbgHeap, 0, (LPSTR)lpth);
	}

    else if (pNode->dwThreadID == dwThreadID)
	{
	lpth = pNode;
	lppr->lpThreads = NULL;
	CloseHandle (lpth->hThread);
	HeapFree (lppr->hDbgHeap, 0, (LPSTR)lpth);
	}
}



/* find a thread's handle based on thread ID */
HANDLE	WINAPI FindThreadHandle (
    DBGPROCESS	  *lppr,
    DWORD	  dwThreadId)
{
    DBGTHREAD	 *pNode = lppr->lpThreads;

    while (pNode != NULL)
	if (pNode->dwThreadID == dwThreadId)
	    return (pNode->hThread);
    return NULL;
}




void WINAPI AddDllNode (
    DBGPROCESS		   *lppr,
    LOAD_DLL_DEBUG_INFO    *lpdbgDll)
{
    DBGDLL    *lpdll;
    DBGDLL    *pNode = lppr->lpDlls;

    /* allocate Dll node off heap */
    if ((lpdll = (DBGDLL *)HeapAlloc (lppr->hDbgHeap, 0, sizeof (DBGDLL))) == NULL)
	ReportError (IDS_ERRHEAPALLOC);

    /* initialize Dll structure */
    lpdll->hFile = lpdbgDll->hFile;
    lpdll->lpBaseOfDll = lpdbgDll->lpBaseOfDll;
    lpdll->dwDebugInfoFileOffset = lpdbgDll->dwDebugInfoFileOffset;
    lpdll->nDebugInfoSize = lpdbgDll->nDebugInfoSize;
    lpdll->fUnicode = lpdbgDll->fUnicode;
    lpdll->Next = NULL;
    lpdll->lpSection = NULL;

	RetrieveModuleName (lpdll->szImageName, lpdbgDll->hFile);

    /* get section names for DLL */
    RetrieveSectionNames (lppr->hDbgHeap, lpdbgDll->hFile, &(lpdll->lpSection));

    /* set linked list pointers */
    while (pNode && pNode->Next)
	pNode = (DBGDLL *)pNode->Next;

    if (!pNode)
	lppr->lpDlls = lpdll;
    else
	(DBGDLL *)pNode->Next = lpdll;
}



void WINAPI RemoveDllNode (
    DBGPROCESS		   *lppr,
    LOAD_DLL_DEBUG_INFO    *lpdbgDll)
{
    DBGDLL	  *pNode = lppr->lpDlls;
    DBGDLL	  *lpdll;
    SECTIONINFO    *pSection, *pNext;

    while (pNode->Next &&
	   ((DBGDLL *)(pNode->Next))->lpBaseOfDll != lpdbgDll->lpBaseOfDll)
	pNode = (DBGDLL *)pNode->Next;

    if (pNode->Next)
	{
	lpdll = (DBGDLL *)pNode->Next;
	pNode->Next = ((DBGDLL *)(pNode->Next))->Next;

	pSection = pNext = lpdll->lpSection;
	while (pNext)
	    {
	    pNext = (SECTIONINFO *)pSection->Next;
	    HeapFree (lppr->hDbgHeap, 0, (LPSTR)pSection);
	    pSection = pNext;
	    }

	HeapFree (lppr->hDbgHeap, 0, (LPSTR)lpdll);
	}

    else if (pNode->lpBaseOfDll == lpdbgDll->lpBaseOfDll)
	{
	lpdll = pNode;
	lppr->lpDlls = NULL;

	pSection = pNext = lpdll->lpSection;
	while (pNext)
	    {
	    pNext = (SECTIONINFO *)pSection->Next;
	    HeapFree (lppr->hDbgHeap, 0, (LPSTR)pSection);
	    pSection = pNext;
	    }

	HeapFree (lppr->hDbgHeap, 0, (LPSTR)lpdll);
	}
}



void WINAPI RecordException (
    DBGPROCESS	   *lppr,
    DEBUG_EVENT    *de)
{
    DBGEXCEPTREC    *lper;
    DBGEXCEPTREC    *per = lppr->lpERs;
    HANDLE	    hThread;
    int 	    i;

    /* allocate Dll node off heap */
    if ((lper = (DBGEXCEPTREC *)HeapAlloc (lppr->hDbgHeap, 0, sizeof (DBGEXCEPTREC))) == NULL)
	ReportError (IDS_ERRHEAPALLOC);

    /* initialize exception record structure */
    lper->dwThreadId = de->dwThreadId;
    lper->dwFirstChance = de->u.Exception.dwFirstChance;
    lper->ExceptRecord.ExceptionCode = de->u.Exception.ExceptionRecord.ExceptionCode;
    lper->ExceptRecord.ExceptionFlags = de->u.Exception.ExceptionRecord.ExceptionFlags;
    lper->ExceptRecord.ExceptionRecord = NULL;
    lper->ExceptRecord.ExceptionAddress = de->u.Exception.ExceptionRecord.ExceptionAddress;
    lper->ExceptRecord.NumberParameters = de->u.Exception.ExceptionRecord.NumberParameters;
    for (i=0; i<EXCEPTION_MAXIMUM_PARAMETERS; i++)
	lper->ExceptRecord.ExceptionInformation[i] =
	   de->u.Exception.ExceptionRecord.ExceptionInformation[i];

    /* get exception thread handle */
    hThread = FindThreadHandle (lppr, de->dwThreadId);
    lper->Context.ContextFlags = CONTEXT_CONTROL;
    GetThreadContext (hThread, &(lper->Context));

    /* set linked list pointers */
    while (per && per->Next)
	per = (DBGEXCEPTREC *)per->Next;
    if (!per)
	lppr->lpERs = lper;
    else
	(DBGEXCEPTREC *)per->Next = lper;
}



/*
 * MIPS/ALPHA/PPC must increment the FIR/Iar on a breakpoint
 * in order to fetch the next instruction
 */

int WINAPI HandleBreakPoint(
    DBGPROCESS		      *lppr,
    DWORD		      dwThreadID
    )
{
    CONTEXT   ThreadContext;
    DBGTHREAD *pNode = lppr->lpThreads;
    HANDLE    hThread;

    while (pNode && pNode->dwThreadID != dwThreadID)
	pNode = (DBGTHREAD *)pNode->Next;

    if(!pNode)
	return(0);
    else
	{
        hThread = (HANDLE) pNode->hThread;
	
        ThreadContext.ContextFlags = CONTEXT_CONTROL;
        if (!GetThreadContext (hThread, &ThreadContext))
	    {
	    ReportError (IDS_ERRGETTHREADCONTEXT);
            return(0);
	    }

#ifdef _PPC_
        ThreadContext.Iar += 4;
#elif !defined _X86_
        ThreadContext.Fir += 4;
#endif

        if (!SetThreadContext (hThread, &ThreadContext))
	    {
	    ReportError (IDS_ERRSETTHREADCONTEXT);
	    return (0);
	    }

	return (1);
	}
}



int   WINAPI WhereIsStack (
    HANDLE	  hThreadContext)
{
    CONTEXT			ThreadContext;

    ThreadContext.ContextFlags = CONTEXT_CONTROL;
    if (!GetThreadContext (hThreadContext, &ThreadContext))
	ReportError (IDS_ERRGETTHREADCONTEXT);

#ifdef _X86_
    return ThreadContext.Esp;
#elif defined (_PPC_)
    return (int) ThreadContext.Gpr1;
#else /* _MIPS_ */
    return (int) ThreadContext.IntSp;
#endif

}



/* function walks memory regions of process */
int    WINAPI WalkProcess (
    HANDLE    hChildProcess,
    LPVOID    *lpWalk,
    LPINT     *lpObjects)
{
    LPVMOBJECT	  lpList;
    LPVOID	  lpMem = 0;
    LPVOID	  lpStack = 0;
    int 	  nCnt, i;
    SYSTEM_INFO   si;

    /* if pointer exists, reset to no commit */
    if (*lpWalk)
	{
	if (!VirtualFree (*lpWalk, 0, MEM_DECOMMIT))
	    ReportError (IDS_ERRVIRTUALFREE);
	}

    /* else perform initial reserve */
    else
	if ((*lpWalk = VirtualAlloc (NULL,
				     TOTALVMRESERVE,
				     MEM_RESERVE,
				     PAGE_NOACCESS)) == NULL)
	    ReportError (IDS_ERRVIRTUALALLOC);

    /* initialize list pointer to beginning of walker list */
    lpList = (LPVMOBJECT)*lpWalk;

    /* Get maximum address range from system info */
    GetSystemInfo(&si);

    /* walk process addresses */
    while (lpMem < si.lpMaximumApplicationAddress)
	{
	try  /* virtual memory exception handler automatically commits mem */
	    {
	    /* touch memory in VMOBJECT structure that exists after mbi field to trigger
	       access violation for a new page of memory.  Do this here since VirtualQueryEx
	       does not seem to generate exceptions, rather it just fails calls */
	    *lpList->szObjType = 0;
	    *lpList->szModule = 0;
	    *lpList->szSection = 0;
	    lpList->bNew = 0;

	    VirtualQueryEx (hChildProcess,
			    lpMem,
			    &(lpList->mbi),
			    sizeof (MEMORY_BASIC_INFORMATION));

	    /* increment lpMem to next region of memory */
	    lpMem = (LPVOID)((DWORD)lpList->mbi.BaseAddress +
			     (DWORD)lpList->mbi.RegionSize);

	    lpList++;
	    }

	except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION)
	    {
	    /* commit next page of walker list memory */
	    if (((int)lpList + 4096) < (int)*lpWalk + TOTALVMRESERVE)
		VirtualAlloc ((LPVOID)((int)lpList + sizeof (VMOBJECT)),
			      4096,
			      MEM_COMMIT,
			      PAGE_READWRITE);
	    else
		{
		NotifyUser (NULL,
			    IDS_ERROR,
			    IDS_NOTENOUGHMEM,
			    " reserved for Objects",
			    MB_OK);
		return 0;
		}
	    }
	}

    /* allocate objects index array */
    if (*lpObjects)
	LocalFree (*lpObjects);
    nCnt = (((int)(LPVOID)lpList)-(int)*lpWalk)/sizeof (VMOBJECT);
    *lpObjects = LocalAlloc (LPTR, nCnt * sizeof (int));
    for (i=0; i<nCnt; i++)
	(*lpObjects)[i] = i;

    /* return number of item in list */
    return (nCnt);
}




void WINAPI AnalyzeProcess (
    DBGPROCESS	  *lpDbgProcess,
    LPVMOBJECT	  lpVMObject,
    int 	  nObjects)
{
    DBGTHREAD			*pTh = lpDbgProcess->lpThreads;
    DBGDLL			*pDll = lpDbgProcess->lpDlls;
    int 			nStack;
    int 			nThreadCnt = 0;
    char			szNum[10];
    char			szObjDesc[MAX_PATH];
    SECTIONINFO 		 *pSection;
    int 			i;
    MEMORY_BASIC_INFORMATION	mbi;
    LPPROBE			lpProbe;


    /* name default heap in process if available */
    if (lpProbe = RetrieveProbeData ())
	{
	VirtualQueryEx (lpDbgProcess->hProcess, (PVOID)lpProbe->hDefHeap, &mbi, sizeof (mbi));

	/* ignore invalid regions with a base region of 0 */
	if (mbi.AllocationBase)
	    {
	    /* find all objects with same base region */
	    for (i=0; i<nObjects; i++)
		{
		if (lpVMObject[i].mbi.AllocationBase == mbi.AllocationBase)
		    {
		    strcpy (lpVMObject[i].szObjType, "heap");
		    strcpy (lpVMObject[i].szModule, "process default");
		    }
		}
	    }
	}

    /* name stack object for each thread in process */
    while (pTh != NULL)
	{
	/* get stack location for thread */
	nStack = WhereIsStack (pTh->hThread);

	strcpy (szObjDesc, "Thread ");
	strcat (szObjDesc, itoa (nThreadCnt, szNum, 10));

	/* locate base region */
	VirtualQueryEx (lpDbgProcess->hProcess, (PVOID)nStack, &mbi, sizeof (mbi));

	/* ignore invalid regions with a base region of 0 */
	if (mbi.AllocationBase)
	    {
	    /* find all objects with same base region */
	    for (i=0; i<nObjects; i++)
		{
		if (lpVMObject[i].mbi.AllocationBase == mbi.AllocationBase)
		    {
		    strcpy (lpVMObject[i].szObjType, "stack");
		    strcpy (lpVMObject[i].szModule, szObjDesc);
		    }
		}
	    }

	/* locate and identify the guard page in each stack.
	   The guard page is a single page of committed memory at the lower end of
	   committed memory and immediately adjacent to the stack's reserved memory.
	   If there is no more reserved stack space left, the guard page will be the
	   last page of committed memory.  A stack cannot exist without a guard page.
	   There will be at most three regions of memory in the stack: the actual stack
	   space is committed, the guard page is committed and the remaining address
	   space is marked reserved. */
	/* locate lowest region of stack, since objects array is not sorted until after
	   this function, it is always in ascending address order :) */
	i=0;
	while (i<nObjects &&
	       (int)lpVMObject[i].mbi.BaseAddress < nStack &&
	       lpVMObject[i].mbi.AllocationBase != mbi.AllocationBase)
	    i++;

	if (i<nObjects &&
	    lpVMObject[i].mbi.AllocationBase == mbi.AllocationBase)
	    {
	    if (lpVMObject[i].mbi.State == MEM_RESERVE)
		i++;

	    /* identify guard page in section field */
	    strcpy (lpVMObject[i].szSection, "guard");
	    }

	/* increment thread count */
	nThreadCnt++;

	/* increment list pointer */
	pTh = (DBGTHREAD *)pTh->Next;
	}

    /* name DLL objects in process */
    while (pDll != NULL)
	{
	/* locate dll base region */
	VirtualQueryEx (lpDbgProcess->hProcess, pDll->lpBaseOfDll, &mbi, sizeof (mbi));

	/* ignore invalid regions with a base region of 0 */
	if (mbi.AllocationBase)
	    {
	    /* find all objects with same base region */
	    for (i=0; i<nObjects; i++)
		{
		if (lpVMObject[i].mbi.AllocationBase == mbi.AllocationBase)
		    {
		    strcpy (lpVMObject[i].szObjType, "dll");
		    strcpy (lpVMObject[i].szModule, pDll->szImageName);
		    }
		}
	    }

	/* name dll sections */
	pSection = (SECTIONINFO *)pDll->lpSection;
	while (pSection != NULL)
	    {
	    i = 0;
	    while (i++<nObjects)
		{
		if (lpVMObject[i].mbi.BaseAddress ==
			((char *)pDll->lpBaseOfDll + pSection->uVirtualAddress))
		    {
		    strcpy (lpVMObject[i].szSection, pSection->szSection);
		    break;
		    }
		}
	    pSection = (SECTIONINFO *)pSection->Next;
	    }

	/* increment list pointer */
	pDll = (DBGDLL *)pDll->Next;
	}

    /* locate exe base region */
    VirtualQueryEx (lpDbgProcess->hProcess, lpDbgProcess->lpImage, &mbi, sizeof (mbi));

    /* ignore invalid regions with a base region of 0 */
    if (mbi.AllocationBase)
	{
	/* find all objects with same base region */
	for (i=0; i<nObjects; i++)
	    {
	    if (lpVMObject[i].mbi.AllocationBase == mbi.AllocationBase)
		{
		strcpy (lpVMObject[i].szObjType, "exe");
		strcpy (lpVMObject[i].szModule, lpDbgProcess->szModule);
		}
	    }
	}

    /* name exe sections */
    pSection = lpDbgProcess->lpSection;
    while (pSection != NULL)
	{
	i = 0;
	while (i++<nObjects)
	    {
	    if (lpVMObject[i].mbi.BaseAddress ==
		    ((char *)lpDbgProcess->lpImage + pSection->uVirtualAddress))
		{
		strcpy (lpVMObject[i].szSection, pSection->szSection);
		break;
		}
	    }
	pSection = (SECTIONINFO *)pSection->Next;
	}
}




/* find all occurrances of objects having same base region */
void WINAPI NameObjects (
    HANDLE	  hProcess,
    LPVOID	  lpAddress,
    LPVMOBJECT	  lpVMObj,
    int 	  nObjects,
    char	  *lpszObjType,
    char	  *lpszModule)
{
    int 			i;
    MEMORY_BASIC_INFORMATION	mbi;

    /* locate base region */
    VirtualQueryEx (hProcess, lpAddress, &mbi, sizeof (mbi));

    /* ignore invalid regions with a base region of 0 */
    if (!mbi.AllocationBase)
	return;

    /* find all objects with same base region */
    for (i=0; i<nObjects; i++)
	{
	if (lpVMObj[i].mbi.AllocationBase == mbi.AllocationBase)
	    {
	    strcpy (lpVMObj[i].szObjType, lpszObjType);
	    strcpy (lpVMObj[i].szModule, lpszModule);
	    }
	}
}




void WINAPI IdentifyNewObjects (
    LPVMOBJECT	  lpVMObjectOld,
    int 	  nObjectsOld,
    LPVMOBJECT	  lpVMObject,
    int 	  nObjects)
{
    int    i, j;

    for (i=0; i<nObjects; i++)
	{
	for (j=0; j<nObjectsOld; j++)
	    {
	    if (VMCompare (lpVMObject+i, lpVMObjectOld+j))
		goto NEXT;
	    }

	/* if not found must be a new item */
	lpVMObject[i].bNew = TRUE;
NEXT:;
	}
}


BOOL WINAPI VMCompare (
    LPVMOBJECT	  lpVM1,
    LPVMOBJECT	  lpVM2)
{

    /* compare memory info */
    if (lpVM1->mbi.AllocationBase      != lpVM2->mbi.AllocationBase	  ||
	lpVM1->mbi.BaseAddress	       != lpVM2->mbi.BaseAddress	  ||
	lpVM1->mbi.RegionSize	       != lpVM2->mbi.RegionSize 	  ||
	lpVM1->mbi.Protect	       != lpVM2->mbi.Protect		  ||
	lpVM1->mbi.AllocationProtect   != lpVM2->mbi.AllocationProtect	  ||
	lpVM1->mbi.State	       != lpVM2->mbi.State		  ||
	lpVM1->mbi.Type 	       != lpVM2->mbi.Type		     )
	return FALSE;

    /* compare character information */
    if (memcmp ((LPVOID)lpVM1->szObjType, (LPVOID)lpVM2->szObjType, 12) ||
	memcmp ((LPVOID)lpVM1->szSection,
		(LPVOID)lpVM2->szSection,
		IMAGE_SIZEOF_SHORT_NAME)				||
	memcmp ((LPVOID)lpVM1->szModule, (LPVOID)lpVM2->szModule, MAX_PATH))
	return FALSE;

    /* if still here, must be a match */
    return TRUE;
}




/* test to see if all memory objects in range are committed */
BOOL   WINAPI CommittedMemoryRange (
    int 	  i,
    int 	  j,
    LPVMOBJECT	  lpvm,
    int 	  *Objects)
{
    int    k;

    /* test each memory object in range */
    for (k = min(i, j); k <= max (i, j); k++)
	/* report any non committed memory regions */
	if (lpvm[Objects[k]].mbi.State != MEM_COMMIT)
	    return FALSE;

    /* report all committed range */
    return TRUE;
}




/* signal debugger thread to access process memory */
BOOL WINAPI AccessProcessMemory (
    HANDLE    hMemoryEvent,
    HANDLE    hAckEvent,
    LPVOID    lpAddress,
    LPVOID    lpBuffer,
    DWORD     *dwSize)
{
    DWORD    dwResult;

    /* copy data to module globals */
    PMAAddress = lpAddress;
    PMABuffer = lpBuffer;
    PMASize = *dwSize;

    /* signal debugger thread to read memory from process */
    SetEvent (hMemoryEvent);

    /* wait on debugger thread to signal completion of memory task */
    dwResult = WaitForSingleObject (hAckEvent, 100000);
    ResetEvent (hAckEvent);

    /* reset size accessed to verify operation */
    *dwSize = PMASize;

    return (dwResult == WAIT_OBJECT_0 &&
	    PMASize != 0);
}




BOOL WINAPI InterruptThread_HookProcess (
    DBGPROCESS	       *lpDbgProcess,
    LPPROCESS_STATE    lpState)
{
    DBGDLL			*pDlls = lpDbgProcess->lpDlls;
    int 			nBytes, nLen, i;
    MEMORY_BASIC_INFORMATION	mbi;
    LPVOID			lpLoadLibrary = NULL;
    char			szKernel[] = "KERNEL32.DLL";
    char			szFunction[] = "LoadLibraryA";
    BYTE			pCode[PAGESIZE];
    BYTE			pStack[PAGESIZE];
    HANDLE			hDll;


    /* initialize stack and code pages */
    for (i=0; i<PAGESIZE; i++)
	{
	pStack[i] = 0;
	pCode[i] = 0;
	}

    /* find kernel32 Dll */
    while (pDlls != NULL)
	{
	if (!stricmp (pDlls->szImageName, szKernel))
	    break;
	pDlls = (DBGDLL *)pDlls->Next;
	}

    /* if DLL not loaded abort */
    if (pDlls == NULL)
	return FALSE;

    /* load the dll in this process, find the function offset in this
       process and normalize to the offset in the child process */
    hDll = LoadLibrary (szKernel);
    lpLoadLibrary = GetProcAddress (hDll, szFunction);
    VirtualQuery (lpLoadLibrary, &mbi, sizeof (mbi));
    (int)lpLoadLibrary += ((int)pDlls->lpBaseOfDll - (int)mbi.AllocationBase);
    FreeLibrary (hDll);

    /* get thread context information and save for replacement */
    lpState->Context.ContextFlags = CONTEXT_FULL;
    if (!GetThreadContext (lpDbgProcess->lpThreads->hThread, &(lpState->Context)))
	return FALSE;

#ifdef _X86_
    lpState->Eip = (LPVOID)lpState->Context.Eip;
    lpState->Esp = (LPVOID)lpState->Context.Esp;
#elif defined(_PPC_)
    lpState->Eip = (LPVOID)lpState->Context.Iar;
    lpState->Esp = (LPVOID)lpState->Context.Gpr1;
#else /* MIPS */
    lpState->Eip = (LPVOID)lpState->Context.Fir;
    lpState->Esp = (LPVOID)lpState->Context.IntSp;
#endif

    /* locate first writeable code page in exe module */
    lpState->pCodePage = lpDbgProcess->lpImage;
    VirtualQueryEx (lpDbgProcess->hProcess, lpState->pCodePage, &mbi, sizeof (mbi));
    while (!(mbi.Protect & PAGE_READWRITE) &&
	   mbi.AllocationBase == lpDbgProcess->lpImage)
	{
	(int)lpState->pCodePage = (int)mbi.BaseAddress + mbi.RegionSize;
	VirtualQueryEx (lpDbgProcess->hProcess, lpState->pCodePage, &mbi, sizeof (mbi));
	}

    if (mbi.AllocationBase != lpDbgProcess->lpImage)
	return FALSE;

    /* save code page for reset process */
    ReadProcessMemory (lpDbgProcess->hProcess, lpState->pCodePage, lpState->Code, PAGESIZE, &nBytes);

    /* write DLL path to code page */
    strcpy ((char *)pCode, szCurPath);
    strcat ((char *)pCode, "\\probe.dll");
    nLen = strlen ((char *)pCode) + 1;

    /* find current stack page and save  */

#ifdef _X86_
    lpState->pStackPage = (LPVOID)((((int)lpState->Context.Esp)/PAGESIZE) * PAGESIZE);
#elif defined(_PPC_)
    lpState->pStackPage = (LPVOID)((((int)lpState->Context.Gpr1)/PAGESIZE) * PAGESIZE);
#else /* MIPS */
    lpState->pStackPage = (LPVOID)((((int)lpState->Context.IntSp)/PAGESIZE) * PAGESIZE);
#endif

    ReadProcessMemory (lpDbgProcess->hProcess, lpState->pStackPage, lpState->Stack, PAGESIZE, &nBytes);

    /* push address of DLL string on stack */
    *((int *)(pStack+4092)) = (int)lpState->pCodePage;

    /* push return address on stack */
    *((int *)(pStack+4088)) = (int)lpState->pCodePage+nLen;

    /* return to Int 3 breakpoint instruction */
    pCode[nLen] = 0xCC;

    /* write new code and stack pages */
    WriteProcessMemory (lpDbgProcess->hProcess, lpState->pCodePage, pCode, PAGESIZE, &nBytes);
    WriteProcessMemory (lpDbgProcess->hProcess, lpState->pStackPage, pStack, PAGESIZE, &nBytes);

    /* update Eip to execute at actual LoadLibrary function */
    /* adjust stack pointer to point to return address */

#ifdef _X86_
    lpState->Context.Eip = (UINT)lpLoadLibrary;
    lpState->Context.Esp = (UINT)lpState->pStackPage+4088;
#elif defined(_PPC_)
    lpState->Context.Iar = (UINT)lpLoadLibrary;
    lpState->Context.Gpr1 = (UINT)lpState->pStackPage+4088;
#else /* _MIPS_ */
    lpState->Context.Fir = (UINT)lpLoadLibrary;
    lpState->Context.IntSp = (UINT)lpState->pStackPage+4088;
#endif

    if (!SetThreadContext (lpDbgProcess->lpThreads->hThread, &(lpState->Context)))
	{
	ReportError (IDS_ERRSETTHREADCONTEXT);
	/* replace code and stack pages */
	WriteProcessMemory (lpDbgProcess->hProcess, lpState->pCodePage, lpState->Code, PAGESIZE, &nBytes);
	WriteProcessMemory (lpDbgProcess->hProcess, lpState->pStackPage, lpState->Stack, PAGESIZE, &nBytes);
	return FALSE;
	}

    return TRUE;
}




void WINAPI ResetInterruptedThread (
    DBGPROCESS	       *lpDbgProcess,
    LPPROCESS_STATE    lpState)
{
    int    nBytes;

    /* return thread context information */

#ifdef _X86_
    lpState->Context.Eip = (UINT)lpState->Eip;
    lpState->Context.Esp = (UINT)lpState->Esp;
#elif defined(_PPC_)
    lpState->Context.Iar = (UINT)lpState->Eip;
    lpState->Context.Gpr1 = (UINT)lpState->Esp;
#else /* MIPS */
    lpState->Context.Fir = (UINT)lpState->Eip;
    lpState->Context.IntSp = (UINT)lpState->Esp;
#endif

    SetThreadContext (lpDbgProcess->lpThreads->hThread, &(lpState->Context));

    /* return code and stack information */
    WriteProcessMemory (lpDbgProcess->hProcess, lpState->pCodePage, lpState->Code, PAGESIZE, &nBytes);
    WriteProcessMemory (lpDbgProcess->hProcess, lpState->pStackPage, lpState->Stack, PAGESIZE, &nBytes);
}
