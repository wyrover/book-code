/*-------------------------------------------------
  SHOW PROCEDURES
  Prints a list of all the procedures running in
  the system. Generates the list by calling
  EnumProcesses from the EnumProc DLL.

  Written by Brian G. Myers
  -------------------------------------------------*/

/* must match library's UNICODE setting */
#define UNICODE		
#define STRICT
  
#include <windows.h>
#include <stdio.h>	 /* printf */
#include "enumproc.h"

/*-------------------------------------------------
  PROCESS ENUMERATION CALLBACK PROCEDURE
  -------------------------------------------------*/

BOOL CALLBACK ProcessEnumProc (	PPROCESSENUMDATA pProcessData,
                                LPARAM lParam )
{
  TCHAR szOutputLine[FILENAME_MAX * 2];
  HANDLE hStdOut;
  
  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (!hStdOut)
    return( FALSE );
  
  wsprintf( szOutputLine, TEXT("%5lu%9lu%8lu %s\n"),
            pProcessData->IDProcess,
            pProcessData->ThreadCount,
            pProcessData->PriorityBase,    	
            pProcessData->InstanceName );

  WriteConsole( hStdOut, szOutputLine,
		lstrlen(szOutputLine), NULL, NULL );
  return( TRUE );
}

/*-------------------------------------------------
  MAIN
  -------------------------------------------------*/

void main ( void )
{
  printf( "%5s%9s%8s %s\n", "PID", "Threads",
          "PriBase", "Name" );
  
  if (!EnumProcesses( NULL, ENUM_PROCESSES_ALL,
                      ProcessEnumProc, 0L ))
    printf( "Enumeration failed.\n" );
  
  return;
}


