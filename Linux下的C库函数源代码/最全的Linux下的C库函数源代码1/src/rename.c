/***
*rename.c - rename file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines rename() - rename a file
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <io.h>
#include <tchar.h>

/***
*int rename(oldname, newname) - rename a file
*
*Purpose:
*       Renames a file to a new name -- no file with new name must
*       currently exist.
*
*Entry:
*       _TSCHAR *oldname -      name of file to rename
*       _TSCHAR *newname -      new name for file
*
*Exit:
*       returns 0 if successful
*       returns not 0 and sets errno if not successful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _trename (
        const _TSCHAR *oldname,
        const _TSCHAR *newname
        )
{
        ULONG dosretval;

        /* ask OS to move file */

        if (!MoveFile((LPTSTR)oldname, (LPTSTR)newname))
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
