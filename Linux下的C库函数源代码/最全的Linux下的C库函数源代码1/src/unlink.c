/***
*unlink.c - unlink a file
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines unlink() - unlink a file
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <io.h>
#include <tchar.h>

/***
*int _unlink(path) - unlink(delete) the given file
*
*Purpose:
*       This version deletes the given file because there is no
*       distinction between a linked file and non-linked file.
*
*       NOTE: remove() is an alternative entry point to the _unlink()
*       routine* interface is identical.
*
*Entry:
*       _TSCHAR *path - file to unlink/delete
*
*Exit:
*       returns 0 if successful
*       returns -1 and sets errno if unsuccessful
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _tremove (
        const _TSCHAR *path
        )
{
        ULONG dosretval;

        if (!DeleteFile((LPTSTR)path))
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

int __cdecl _tunlink (
        const _TSCHAR *path
        )
{
        /* remove is synonym for unlink */
        return _tremove(path);
}
