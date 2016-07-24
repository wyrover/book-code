
#include "pwalk.h"


/* process  globals */
HANDLE		    hMMFile = 0;
LPPROBE 	    lpProbe = NULL;
CONTEXT 	    gContext;
DWORD		    gReturn;
LPVOID		    gLocation;
CRITICAL_SECTION    gCS;


BOOL   WINAPI ProbeProcess (HANDLE);
void   WINAPI FreeProbe ();
void   WINAPI GetFuncAddrInChildProc (char *, char *, DBGDLL *, LPVOID *);


/* entry point for DLL loading and unloading */
BOOL WINAPI DllMain (
    HANDLE    hModule,
    DWORD     dwFunction,
    LPVOID    lpNot)
{
    switch (dwFunction)
	{
	case DLL_PROCESS_ATTACH:
	    ProbeProcess (hModule);
	    break;

	case DLL_PROCESS_DETACH:
	    FreeProbe ();
	    break;

	default:
	    break;
	}

    return TRUE;
}



/* function initializes port structures */
BOOL WINAPI ProbeProcess (
    HANDLE    hDLL)
{
    char	szMapFileName[MAX_PATH];

    /* load name for global file mapping */
    LoadString (hDLL, IDS_MAPFILENAME, szMapFileName, MAX_PATH);

    /* ProcessWalker creates file mapping then child process attaches */
    if ((hMMFile = OpenFileMapping (FILE_MAP_WRITE, FALSE, szMapFileName)))
	{
	/* map global view of file for hook function access */
	lpProbe = (LPPROBE)MapViewOfFile (hMMFile, FILE_MAP_WRITE, 0, 0, 0);

	/* initialize known process specific information in probe */
	lpProbe->hProcess = GetCurrentProcess ();
	lpProbe->hDefHeap = GetProcessHeap ();
	}

    else
	if (!(hMMFile = CreateFileMapping ((HANDLE)0xffffffff,
					   NULL,
					   PAGE_READWRITE,
					   0,
					   0x100000,
					   szMapFileName)))
	    return FALSE;
	else
	    {
	    lpProbe = (LPPROBE)MapViewOfFile (hMMFile, FILE_MAP_WRITE, 0, 0, 0);
	    ResetProbe ();
	    }

    /* success */
    return TRUE;
}




/* release process objects */
void WINAPI FreeProbe ()
{
    if (lpProbe != NULL)
	/* remove child process objects */
	UnmapViewOfFile ((char *)lpProbe);

    CloseHandle (hMMFile);
}




/* function passes pointer to probe info back to ProcessWalker */
LPPROBE  WINAPI RetrieveProbeData ()
{
    return lpProbe;
}




/* reset probe data to null */
void  WINAPI ResetProbe ()
{
    lpProbe->hProcess = NULL;
    lpProbe->hDefHeap = NULL;
}
