/***
*rmdir.c - remove directory
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _rmdir() - remove a directory
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <direct.h>
#include <tchar.h>

/***
*int _rmdir(path) - remove a directory
*
*Purpose:
*       deletes the directory speicifed by path.  The directory must
*       be empty, and it must not be the current working directory or
*       the root directory.
*
*Entry:
*       _TSCHAR *path - directory to remove
*
*Exit:
*       returns 0 if successful
*       returns -1 and sets errno if unsuccessful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _trmdir (
        const _TSCHAR *path
        )
{
        ULONG dosretval;

        /* ask OS to remove directory */

        if (!RemoveDirectory((LPTSTR)path))
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
