/***
*getproc.c - Get the address of a procedure in a DLL.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _getdllprocadd() - gets a procedure address by name or
*       ordinal
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>

#define _CRT_ENABLE_OBSOLETE_LOADLIBRARY_FUNCTIONS

#include <process.h>

/***
*int (*)() _getdllprocaddr(handle, name, ordinal) - Get the address of a
*       DLL procedure specified by name or ordinal
*
*Purpose:
*
*Entry:
*       int handle - a DLL handle from _loaddll
*       char * name - Name of the procedure, or NULL to get by ordinal
*       int ordinal - Ordinal of the procedure, or -1 to get by name
*
*
*Exit:
*       returns a pointer to the procedure if found
*       returns NULL if not found
*
*Exceptions:
*
*******************************************************************************/

int (__cdecl * __cdecl _getdllprocaddr(
        intptr_t hMod,
        char * szProcName,
        intptr_t iOrdinal))(void)
{
        typedef int (__cdecl * PFN)(void);

        if (szProcName == NULL) {
            if (iOrdinal <= 65535) {
                return ((PFN)GetProcAddress((HANDLE)hMod, (LPSTR)iOrdinal));
            }
        }
        else {
            if (iOrdinal == (intptr_t)(-1)) {
                return ((PFN)GetProcAddress((HANDLE)hMod, szProcName));
            }
        }

        return (NULL);

}
