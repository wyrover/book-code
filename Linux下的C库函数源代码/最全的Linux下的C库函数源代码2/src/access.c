/***
*access.c - access function
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file has the _access() function which checks on file accessability.
*
*******************************************************************************/

#include <cruntime.h>
#include <io.h>
#include <oscalls.h>
#include <stdlib.h>
#include <errno.h>
#include <msdos.h>
#include <internal.h>
#include <tchar.h>

/***
*int _access(path, amode) - check whether file can be accessed under mode
*
*Purpose:
*       Checks to see if the specified file exists and can be accessed
*       in the given mode.
*
*Entry:
*       _TSCHAR *path - pathname
*       int amode -     access mode
*                       (0 = exist only, 2 = write, 4 = read, 6 = read/write)
*
*Exit:
*       returns 0 if file has given mode
*       returns -1 and sets errno if file does not have given mode or
*       does not exist
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _taccess (
        const _TSCHAR *path,
        int amode
        )
{
    errno_t e;
    e = _taccess_s(path,amode);

    return e ? -1 : 0 ;
}

/***
*errno_t _access_s(path, amode) - check whether file can be accessed under mode
*
*Purpose:
*       Checks to see if the specified file exists and can be accessed
*       in the given mode.
*
*Entry:
*       _TSCHAR *path - pathname
*       int amode -     access mode
*                       (0 = exist only, 2 = write, 4 = read, 6 = read/write)
*
*Exit:
*       returns 0 if file has given mode
*       returns errno_t for any other errors
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _taccess_s (
        const _TSCHAR *path,
        int amode
        )
{

        DWORD attr;

        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE( (path != NULL), EINVAL);
        _VALIDATE_CLEAR_OSSERR_RETURN_ERRCODE( ((amode & (~6)) == 0), EINVAL);

        attr = GetFileAttributes((LPTSTR)path);
        if (attr  == 0xffffffff) {
                /* error occured -- map error code and return */
                _dosmaperr(GetLastError());
                return errno;
        }

        if(attr & FILE_ATTRIBUTE_DIRECTORY)
        {
            /* All directories have read & write access */
            return 0;
        }

        /* no error; see if returned premission settings OK */
        if ( (attr & FILE_ATTRIBUTE_READONLY) && (amode & 2) ) {
                /* no write permission on file, return error */
                _doserrno = E_access;
                errno = EACCES;
                return errno;
        }
        else
                /* file exists and has requested permission setting */
                return 0;

}
