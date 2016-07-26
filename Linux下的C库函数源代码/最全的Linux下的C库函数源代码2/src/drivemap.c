/***
*drivemap.c - _getdrives
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _getdrives()
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <direct.h>

#if !defined (_WIN32)
#error ERROR - ONLY WIN32 TARGET SUPPORTED!
#endif  /* !defined (_WIN32) */

/***
*void _getdrivemap(void) - Get bit map of all available drives
*
*Purpose:
*
*Entry:
*
*Exit:
*       drive map with drive A in bit 0, B in 1, etc.
*
*Exceptions:
*
*******************************************************************************/

#if defined (CRTDLL) && !defined (_SYSCRT)
/*
 * Note that we are renaming _getdrives to __getdrives so that we can take dependency on msvcrt.dll. We will still export _getdrive using = trick in msvcrt.def file.
 */
unsigned long __cdecl __getdrives()
#else  /* defined (CRTDLL) && !defined (_SYSCRT) */
unsigned long __cdecl _getdrives()
#endif  /* defined (CRTDLL) && !defined (_SYSCRT) */
{
    return (GetLogicalDrives());
}
