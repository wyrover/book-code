/***
*getpid.c - get current process id
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _getpid() - get current process id
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>

/***
*int _getpid() - get current process id
*
*Purpose:
*       Returns the current process id for the calling process.
*
*Entry:
*       None.
*
*Exit:
*       Returns the current process id.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _getpid (
        void
        )
{
        return GetCurrentProcessId();
}
