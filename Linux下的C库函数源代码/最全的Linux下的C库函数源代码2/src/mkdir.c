/***
*mkdir.c - make directory
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines function _mkdir() - make a directory
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <direct.h>
#include <tchar.h>

/***
*int _mkdir(path) - make a directory
*
*Purpose:
*       creates a new directory with the specified name
*
*Entry:
*       _TSCHAR *path - name of new directory
*
*Exit:
*       returns 0 if successful
*       returns -1 and sets errno if unsuccessful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tmkdir (
        const _TSCHAR *path
        )
{
        ULONG dosretval;

        /* ask OS to create directory */

        if (!CreateDirectory((LPTSTR)path, (LPSECURITY_ATTRIBUTES)NULL))
            dosretval = GetLastError();
        else
            dosretval = 0;

        if (dosretval) {
            /* error occured -- map error code and return */
            _dosmaperr(dosretval);
            return -1;
        }

        return 0;
}
