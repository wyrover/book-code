/*-------------------------------------------------
  ENUMPROC.H
  Interface header for the EnumProc DLL.  
  Contains definitions needed to call the 
  exported EnumProcesses procedure.

  Written by Brian G. Myers
  -------------------------------------------------*/

#ifdef __DLL__
# define DLLFUNC __declspec(dllexport)
#else
# define DLLFUNC __declspec(dllimport)
#endif

#include <stdio.h> 	 /* FILENAME_MAX */

/* structure for passing process info back to caller */
typedef struct {
  DWORD ThreadCount;		      /* number of threads */
  DWORD PriorityBase;  	              /* base running priority */
  DWORD IDProcess;		      /* process ID number */
  TCHAR InstanceName[FILENAME_MAX];   /* module name */
} PROCESSENUMDATA, *PPROCESSENUMDATA;

/* prototype for the enumeration callback procedure*/
#ifdef STRICT
typedef BOOL (CALLBACK* PROCESSENUMPROC)( PPROCESSENUMDATA, LPARAM );
#else
typedef FARPROC PROCESSENUMPROC
#endif

/* prototype for the library's main procedure */
DLLFUNC BOOL WINAPI EnumProcesses( PTSTR szComputerName,
                                   UINT uProcessID, 
                                   PROCESSENUMPROC ProcessEnumProc,
                                   LPARAM lParam );

/* A value for the uPID parameter of EnumProcesses */
#define ENUM_PROCESSES_ALL		0


