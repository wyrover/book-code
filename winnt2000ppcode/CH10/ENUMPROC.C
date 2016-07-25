/*-------------------------------------------------
  ENUM PROC LIBRARY
  Library clients call EnumProcesses to list
  all the processes currently running.

  Written by Brian G. Myers
  -------------------------------------------------*/
  
/* library must match caller's UNICODE setting */
#define UNICODE			/* for windows headers */
#define STRICT

#include <windows.h>
#include <winperf.h>		/* performance structures */
#include <limits.h>		/* UINT_MAX */
#include "enumproc.h"

/*-------------------------------------------------
  MANIFEST CONSTANTS
  -------------------------------------------------*/

#ifdef UNICODE
#define NULL_CHAR UNICODE_NULL
#else
#define NULL_CHAR '\0'
#endif

#define GET_INDEX_FAILURE	UINT_MAX
#define RESERVED 			0	
#define BUFFER_INCREMENT	1024
#define NAME_BLOCK_SUBKEY \
	TEXT("SOFTWARE\\Microsoft\\Windows NT\
\\CurrentVersion\\Perflib\\009")

/*-------------------------------------------------
  PROTOTYPES
  -------------------------------------------------*/

PWSTR GetInstanceName(
        PPERF_INSTANCE_DEFINITION pInstance );
BOOL CollectCounterValues(
	PPERF_OBJECT_TYPE pProcessObjectType,	 
	PPERF_INSTANCE_DEFINITION pProcess,
	PPROCESSENUMDATA pProcessEnumData );
BOOL StoreProcessCounterValue(
	PPERF_COUNTER_DEFINITION pPerfCounterDef,
	PBYTE pCounter, PPROCESSENUMDATA pProcessEnumData );
PPERF_INSTANCE_DEFINITION NextInstance(
	PPERF_INSTANCE_DEFINITION pInstanceDef );
PPERF_DATA_BLOCK LoadCounterData( PTSTR szComputerName,
	PTSTR pObjectIndexString );
BOOL GetTitleIndices( PTSTR szComputerName );
PTSTR LoadNameStrings( PTSTR szComputerName );
UINT GetTitleIndexValue( PTSTR pNameBlock,
	PTSTR pTitle );
PTSTR GetTitleIndexString( PTSTR pNameBlock,
	PTSTR pTitle );
PTSTR NextString( PTSTR pstr );
UINT tstr_to_uint( PTSTR pNumString );
    
/*-------------------------------------------------
  GLOBAL VARIABLE
  The Index structure holds title string index    
  values for all the counters the library passes  
  to its caller. It must be thread-local in case  
  concurrent threads retrieve performance data    
  from different machines. 
  -------------------------------------------------*/
typedef struct {
  UINT IDProcess;
  UINT ThreadCount;
  UINT PriorityBase;
  UINT ProcessIndexValue;
  PTSTR ProcessIndexString;
} TITLE_INDEX_STRUCT;

/* Older Borland compilers used __thread instead of     */
/* Microsoft's declspec.  More recent Borland compilers */
/* support both.  The THREADLOCAL macro provides a      */
/* portable syntax.                                     */

#ifdef __BORLANDC__			   
#define THREADLOCAL( type, name ) type __thread name
#else  // using Microsoft tools
#define THREADLOCAL( type, name ) \
 __declspec( thread ) type name
#endif

THREADLOCAL( TITLE_INDEX_STRUCT, Index );

/*-------------------------------------------------
  ENUMERATE PROCESSES (exported function)
  Call up the performance monitor data for
  process objects and pass some information
  about each existing process to the caller.
  Return TRUE if enumeration succeeds and 
  FALSE if an error interrupts enumeration.
  -------------------------------------------------*/
BOOL WINAPI EnumProcesses ( PTSTR szComputerName,
                            UINT uProcessID, 
                            PROCESSENUMPROC ProcessEnumProc,
                            LPARAM lParam ) 
{
  PPERF_DATA_BLOCK pDataBlock;
  PPERF_OBJECT_TYPE pObjectType;
  unsigned long lNumProcesses;
  PPERF_INSTANCE_DEFINITION pThisProcess;
  unsigned i;
  PROCESSENUMDATA ProcessEnumData;
  BOOL bFound = FALSE;
  BOOL bSuccess = TRUE;
  
  /* get name string index values for process counters */
  if (!GetTitleIndices( szComputerName ))
    return( FALSE );
  
  /* query performance monitor for process data */
  ZeroMemory( &ProcessEnumData,
              sizeof(ProcessEnumData) );
  pDataBlock = LoadCounterData( szComputerName,
                                Index.ProcessIndexString );
  if (!pDataBlock)	
    return( FALSE );	/* unable to load data */
  
  /* set a pointer to start of data for first object */
  pObjectType = (PPERF_OBJECT_TYPE)
    ((PBYTE)pDataBlock + pDataBlock->HeaderLength);
  
  /* loop through objects to find Process object */
  for (i=0; i<pDataBlock->NumObjectTypes; i++)
  {
    if (pObjectType->ObjectNameTitleIndex
        == Index.ProcessIndexValue) {
      bFound = TRUE;
      break;
    } else {				/* next object */
      pObjectType = (PPERF_OBJECT_TYPE)
        ((PBYTE)pObjectType
         + pObjectType->HeaderLength);
    }
    
  }
  if (!bFound)			/* no processes found! */
    return( FALSE );	/* (this shouldn't happen) */
  
  /* how many processes were found? */
  lNumProcesses = pObjectType->NumInstances;
  
  /* point to first process instance definition */
  pThisProcess = (PPERF_INSTANCE_DEFINITION)
    ((PBYTE)pObjectType
     + pObjectType->DefinitionLength);
  
  /* Loop through all instances.  Send info about */
  /* each process object back to the client program */
  /* through its callback function, ProcessEnumProc. */
  
  for (i = 0; i < lNumProcesses; i++)	{
    
    /* Copy instance name to ProcessEnumData. */
    /* If UNICODE is not defined, convert to */
    /* a narrow-character string. */
    
#ifdef UNICODE
    lstrcpy( ProcessEnumData.InstanceName,
             GetInstanceName(pThisProcess) );
#else
    WideCharToMultiByte( CP_ACP, 0,
                         GetInstanceName(pThisProcess), -1,
                         ProcessEnumData.InstanceName,
                         sizeof(ProcessEnumData.InstanceName),
                         NULL, NULL );
#endif	
    
    /* copy counter values to ProcessEnumData */
    if (!CollectCounterValues( pObjectType,
                               pThisProcess, &ProcessEnumData )) {
      bSuccess = FALSE;	/* no values found (this */
      break;				/* (shouldn't happen) */
    }
    
    /* if caller wants info for this process, send it */
    if ((uProcessID == ENUM_PROCESSES_ALL)
        || (uProcessID == ProcessEnumData.IDProcess)) {
      
      if (!ProcessEnumProc( &ProcessEnumData, lParam ))
        break;	/* caller wants no more data */
    }
    
    /* point to the subsequent process */
    pThisProcess = NextInstance( pThisProcess );
  }
  
  LocalFree( Index.ProcessIndexString );
  LocalFree( pDataBlock );
  return( bSuccess );
}

/*-------------------------------------------------
  GET INSTANCE NAME
  Given a pointer to an instance definition
  structure, return a pointer to the name of that
  instance.  Return a pointer to Unicode name
  string, if successful, or UNICODE_NULL if for 
  any reason the name cannot be found.
  -------------------------------------------------*/
PWSTR GetInstanceName ( PPERF_INSTANCE_DEFINITION pInstance )
{
  PWSTR pNameString;
  
  if (!pInstance)
    pNameString = UNICODE_NULL;
  else if ((pInstance->NameLength != 0) && (pInstance->NameOffset != 0))
    pNameString = (PWSTR)((PBYTE)pInstance + pInstance->NameOffset);	
  
  return( pNameString );
}

/*-------------------------------------------------
  COLLECT COUNTER VALUES
  Run through all the counters associated with
  one process. Interpret counter values and copy
  them into fields of a PROCESSENUMDATA structure.
  Return TRUE if any values are copied into
  pEnumData.
  -------------------------------------------------*/
BOOL CollectCounterValues ( PPERF_OBJECT_TYPE pObjectType,
                            PPERF_INSTANCE_DEFINITION pProcess,
                            PPROCESSENUMDATA pEnumData ) 
{
  UINT i;
  UINT uNumCounters;
  PPERF_COUNTER_BLOCK pCounterBlock;
  PPERF_COUNTER_DEFINITION pThisCounterDef;
  PBYTE pThisCounter;
  BOOL bSuccess = FALSE;
  
  uNumCounters = (UINT)pObjectType->NumCounters;
  
  /* point to the first counter definition */
  pThisCounterDef = (PPERF_COUNTER_DEFINITION)
    ((PBYTE)pObjectType + pObjectType->HeaderLength);
  
  /* point to the first counter block */
  pCounterBlock = (PPERF_COUNTER_BLOCK)
    ((PBYTE)pProcess + pProcess->ByteLength);
  
  for (i = 0; i < uNumCounters; i++) {
    
    /* point to the counter data that */
    /* corresponds to the counter definition */
    pThisCounter = (PBYTE)pCounterBlock
      + pThisCounterDef->CounterOffset;
    
    /* copy a counter value into pEnumData */
    if (StoreProcessCounterValue( pThisCounterDef,
                                  pThisCounter, pEnumData )) 
      bSuccess = TRUE;	
    
    /* point to the next counter definition */
    pThisCounterDef = (PPERF_COUNTER_DEFINITION)
      ((PBYTE)pThisCounterDef +
       pThisCounterDef->ByteLength);
  }
  
  return( bSuccess );		
}

/*-------------------------------------------------
  STORE PROCESS COUNTER VALUE
  Interpret the value in pCounter and store it
  in the appropriate field of pEnumData.  To
  interpret the value, the procedure needs to
  see the related counter	definition structure.
  Returns TRUE if pCounter is identified and
  stored.
  -------------------------------------------------*/
BOOL StoreProcessCounterValue ( PPERF_COUNTER_DEFINITION pPerfCounterDef,
                                PBYTE pCounter, 
                                PPROCESSENUMDATA pEnumData ) 
{
  DWORD dwCounterIndex =
    pPerfCounterDef->CounterNameTitleIndex;
  
  if (dwCounterIndex == Index.ThreadCount) {
    pEnumData->ThreadCount = *(PDWORD)pCounter;
  } else if (dwCounterIndex == Index.PriorityBase) {
    pEnumData->PriorityBase = *(PDWORD)pCounter;
  } else if (dwCounterIndex == Index.IDProcess) {
    pEnumData->IDProcess = *(PDWORD)pCounter;
  } else 
    return (FALSE);		/* no value was stored */
  
  return( TRUE );		/* a value was stored */
}

/*-------------------------------------------------
  NEXT INSTANCE
  Given a pointer to an instance definition
  structure, return a pointer to the next
  instance definition structure that follows.
  -------------------------------------------------*/
PPERF_INSTANCE_DEFINITION NextInstance (
     PPERF_INSTANCE_DEFINITION pInstanceDef ) 
{
  PPERF_COUNTER_BLOCK pCounterBlock;
  
  pCounterBlock = (PPERF_COUNTER_BLOCK)
    ((PBYTE)pInstanceDef + pInstanceDef->ByteLength);
  
  return( (PPERF_INSTANCE_DEFINITION)
          ((PBYTE)pCounterBlock + pCounterBlock->ByteLength) );
}

/*-------------------------------------------------
  LOAD COUNTER DATA
  Read from the performance monitor a block of
  data holding counter values for the given
  object.  Return a pointer to the data block,
  which the caller must free, or NULL on errors.
  -------------------------------------------------*/
PPERF_DATA_BLOCK LoadCounterData ( PTSTR szComputerName,
                                   PTSTR pObjectTitleIndex ) 
{
  PPERF_DATA_BLOCK pCounters;
  DWORD dwDataSize = 30*BUFFER_INCREMENT;		/* 30k */
  LONG lResult;                 
  DWORD Type;
  HKEY hPerfKey = HKEY_PERFORMANCE_DATA;
  
  /* attempt a remote connection if caller requests it */
  if (szComputerName)
  {
    lResult = RegConnectRegistry( szComputerName,
                                  HKEY_PERFORMANCE_DATA, &hPerfKey );
    if (lResult != ERROR_SUCCESS)
      return( NULL );
  }
  
  /* allocate buffer for data (guessing its size) */
  pCounters = (PPERF_DATA_BLOCK)
    LocalAlloc(	LMEM_FIXED, (UINT)dwDataSize );
  
  do {
    /* try to load the data */
    lResult = RegQueryValueEx (hPerfKey, 
                               pObjectTitleIndex, 
                               NULL, 
                               &Type,
                               (PBYTE)pCounters, 
                               &dwDataSize );
    
    /* if there wasn't room, expand the buffer */
    if (lResult == ERROR_MORE_DATA) {
      LocalFree( pCounters );
      dwDataSize += BUFFER_INCREMENT;
      pCounters = (PPERF_DATA_BLOCK)
        LocalAlloc( LMEM_FIXED, (UINT)dwDataSize );
    }
    /* if some other error occurred, quit */
    else if (!pCounters)	
      return( NULL );
    
  } while (lResult == ERROR_MORE_DATA);
  
  /* close key so system won't lock monitored objects */
  RegCloseKey( hPerfKey );
  return( pCounters );
}

/*-------------------------------------------------
  GET PROCESS COUNTER TITLE INDICES
  Loads block of strings from performance
  monitor and searches for string index
  values for process-related counters.
  Stores the values in the global Index
  structure.  Returns FALSE for errors.
  -------------------------------------------------*/
BOOL GetTitleIndices ( PTSTR szComputerName ) 
{
  PTSTR pstrNameBlock;
  
  pstrNameBlock = LoadNameStrings( szComputerName );
  if (!pstrNameBlock)
    return( FALSE );
  
  /* search data block for index numbers to  */
  /* process counters  */
  Index.ThreadCount  = (UINT)GetTitleIndexValue( pstrNameBlock, 
                                             TEXT("Thread Count") );
  Index.PriorityBase = (UINT)GetTitleIndexValue( pstrNameBlock, 
                                             TEXT("Priority Base") );
  Index.IDProcess    = (UINT)GetTitleIndexValue( pstrNameBlock, 
                                             TEXT("ID Process") );

  /* get string and value of index to object name  */
  Index.ProcessIndexString = GetTitleIndexString( pstrNameBlock, 
                                             TEXT("Process") );
  Index.ProcessIndexValue = tstr_to_uint( Index.ProcessIndexString );

  GlobalFree( pstrNameBlock );

  /* return TRUE if all indices were found */
  return( (Index.ThreadCount != GET_INDEX_FAILURE)
          && (Index.PriorityBase != GET_INDEX_FAILURE)
          && (Index.IDProcess != GET_INDEX_FAILURE)
          && (Index.ProcessIndexString != NULL) );
}

/*-------------------------------------------------
  LOAD NAME STRINGS
  Read in the block of object name strings.
  Return a valid pointer to the block of
  MULTI_SZ strings or NULL if an error occurs.
  -------------------------------------------------*/
PTSTR LoadNameStrings ( PTSTR szComputerName ) 
{
  HKEY hNamesKey;
  LONG lResult;
  DWORD dwDataType;
  DWORD dwDataSize;
  PTSTR pstrNameBlock;	/* points to block of name strings */
  HKEY hMachineKey = HKEY_LOCAL_MACHINE;

  /* attempt a remote connection if caller requests it */
  if (szComputerName)
  {
    lResult = RegConnectRegistry( szComputerName,
                                  HKEY_LOCAL_MACHINE, &hMachineKey );
    if (lResult != ERROR_SUCCESS)
      return( NULL );
  }

  /* open the key where the block of names is kept */
  lResult = RegOpenKeyEx( hMachineKey,
                          NAME_BLOCK_SUBKEY, RESERVED, KEY_QUERY_VALUE,
                          &hNamesKey );
  if (lResult != ERROR_SUCCESS)
    return( NULL );
  
  /* find out how big the data block is */
  lResult = RegQueryValueEx( hNamesKey,
                             TEXT("Counters"), RESERVED,
                             &dwDataType, NULL, &dwDataSize );
  if (lResult != ERROR_SUCCESS)
    return( NULL );
  
  /* allocate room for all the names */
  pstrNameBlock = (PTSTR)
    GlobalAlloc( GMEM_ZEROINIT, dwDataSize );
  if (!pstrNameBlock)	
    return( NULL );
  
  /* load all the names into the buffer */
  lResult = RegQueryValueEx( hNamesKey,
                             TEXT("Counters"), RESERVED, &dwDataType,
                             (PBYTE)pstrNameBlock, &dwDataSize );
  if (lResult != ERROR_SUCCESS)	
    return( NULL );
  
  /* close the key that holds the names */
  RegCloseKey( hNamesKey );
  RegCloseKey( hMachineKey );
  return( pstrNameBlock );
}

/*-------------------------------------------------
  GET TITLE INDEX VALUE
  Search through the name block for the given
  string pTitle.  If convert the corresponding
  index string (e.g. "230") to an unsigned
  integer.  Return the integer, or if not found,
  return GET_INDEX_FAILURE.
  -------------------------------------------------*/

UINT GetTitleIndexValue ( PTSTR pstrNameBlock, PTSTR pTitle ) 
{
  PTSTR pIndexString;
  UINT uValue = GET_INDEX_FAILURE;
  
  if ((!pstrNameBlock) || (!pTitle)) 
    return( GET_INDEX_FAILURE );
  
  pIndexString = GetTitleIndexString( pstrNameBlock, pTitle );
  
  if (pIndexString) {
    uValue = tstr_to_uint( pIndexString );
    LocalFree( pIndexString );
  }
	
  return( uValue );
}

/*-------------------------------------------------
  GET TITLE INDEX STRING
  Search through the name block for the given
  string pTitle.  If found, copy and return
  the corresponding index string (e.g. "230").
  Caller must free the string.  NULL return
  for errors.
  -------------------------------------------------*/
PTSTR GetTitleIndexString ( PTSTR pstrNameBlock, PTSTR pTitle )
{
  BOOL bEndOfData = FALSE;
  PTSTR pThisIndex, pThisTitle;
  PTSTR pReturn = NULL;

  if ((!pstrNameBlock) || (!pTitle)) 
    return( NULL );

  /* point to first two strings in the name block */
  pThisIndex = pstrNameBlock;
  pThisTitle = NextString( pstrNameBlock );
  
  /* search through the name block for a match */
  while (!bEndOfData &&
         (lstrcmp(pTitle, pThisTitle) != 0)) {
    
    pThisIndex = NextString( pThisTitle );
    if (*pThisIndex != NULL_CHAR) 
      pThisTitle = NextString( pThisIndex );
    if (*pThisTitle == NULL_CHAR) 
      bEndOfData = TRUE;
  }
  
  /* if a match was found, copy and return the string *//* */
  if (!bEndOfData) {
    pReturn = LocalAlloc( LPTR, lstrlen(pThisIndex)+1 );
    if (pReturn) 
      lstrcpy( pReturn, pThisIndex );
  }
  return( pReturn );
}

/*-------------------------------------------------
  NEXT STRING
  Return a pointer to the subsequent string in
  a data block of MULTI_SZ strings.
  -------------------------------------------------*/
PTSTR NextString ( PTSTR pstr ) 
{
  if (pstr == NULL)	
    return( NULL );
  
  while (*pstr != NULL_CHAR)	
    pstr++;
  
  return( ++pstr );
}

/*-------------------------------------------------
  TSTR TO UINT
  Convert the given string (e.g. "230") from
  a TCHAR string to an unsigned integer.
  -------------------------------------------------*/
UINT tstr_to_uint ( PTSTR pNumString ) 
{
  UINT uValue = 0;	

  while (isdigit( *pNumString )) {
    uValue *= 10;
    uValue += (BYTE)*pNumString - TEXT('0');
    pNumString++;
  }

  return( uValue );
}
