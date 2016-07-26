/***
*openo.cpp - C++ Version of open which takes a default pmode parameter
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*
*******************************************************************************/


#include <io.h>
#include <share.h>
#include <internal.h>

_CRTIMP int __cdecl _open(const char * path, int oflag, int pmode /* = 0 */)
{
    int fh;
    /* Last parameter passed as 0 because we don't want to validate pmode from _open */
    errno_t e = _sopen_helper(path, oflag, _SH_DENYNO, pmode, &fh, 0);
    return e ? -1 : fh;
}

_CRTIMP int __cdecl _sopen(const char * path, int oflag, int shflag, int pmode /* = 0 */)
{
    int fh;
    /* Last parameter passed as 0 because we don't want to validate pmode from _sopen */
    errno_t e = _sopen_helper(path, oflag, shflag, pmode, &fh, 0);
    return e ? -1 : fh;
}

_CRTIMP int __cdecl _wopen(const wchar_t * path, int oflag, int pmode /* = 0 */)
{
    int fh;
    /* Last parameter passed as 0 because we don't want to validate pmode from _wopen */
    errno_t e = _wsopen_helper(path, oflag, _SH_DENYNO, pmode, &fh, 0);
    return e ? -1 : fh;
}

_CRTIMP int __cdecl _wsopen(const wchar_t * path, int oflag, int shflag, int pmode /* = 0 */)
{
    int fh;
    /* Last parameter passed as 0 because we don't want to validate pmode from _wsopen */
    errno_t e = _wsopen_helper(path, oflag, shflag, pmode, &fh, 0);
    return e ? -1 : fh;
}

