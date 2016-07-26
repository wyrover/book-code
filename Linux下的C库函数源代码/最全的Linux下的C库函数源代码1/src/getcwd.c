/***
*getcwd.c - get current working directory
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*       contains functions _getcwd, _getdcwd and _getcdrv for getting the
*       current working directory.  getcwd gets the c.w.d. for the default disk
*       drive, whereas _getdcwd allows one to get the c.w.d. for whatever disk
*       drive is specified. _getcdrv gets the current drive.
*
*******************************************************************************/

#include <cruntime.h>
#include <mtdll.h>
#include <msdos.h>
#include <errno.h>
#include <malloc.h>
#include <oscalls.h>
#include <stdlib.h>
#include <internal.h>
#include <direct.h>
#include <tchar.h>

#ifdef WPRFLAG
#define _tgetdcwd_nolock _wgetdcwd_nolock
#define _tgetdcwd_lk_dbg _wgetdcwd_lk_dbg
#else  /* WPRFLAG */
#define _tgetdcwd_nolock _getdcwd_nolock
#define _tgetdcwd_lk_dbg _getdcwd_lk_dbg
#endif  /* WPRFLAG */

/***
*_TSCHAR *_getcwd(pnbuf, maxlen) - get current working directory of default drive
*
*Purpose:
*       _getcwd gets the current working directory for the user,
*       placing it in the buffer pointed to by pnbuf.  It returns
*       the length of the string put in the buffer.  If the length
*       of the string exceeds the length of the buffer, maxlen,
*       then NULL is returned.  If pnbuf = NULL, maxlen is ignored.
*       An entry point "_getdcwd()" is defined with takes the above
*       parameters, plus a drive number.  "_getcwd()" is implemented
*       as a call to "_getcwd()" with the default drive (0).
*
*       If pnbuf = NULL, maxlen is ignored, and a buffer is automatically
*       allocated using malloc() -- a pointer to which is returned by
*       _getcwd().
*
*       side effects: no global data is used or affected
*
*Entry:
*       _TSCHAR *pnbuf = pointer to a buffer maintained by the user;
*       int maxlen = length of the buffer pointed to by pnbuf;
*
*Exit:
*       Returns pointer to the buffer containing the c.w.d. name
*       (same as pnbuf if non-NULL; otherwise, malloc is
*       used to allocate a buffer)
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tgetcwd (
        _TSCHAR *pnbuf,
        int maxlen
        )
{
    _TSCHAR *retval;

    _mlock( _ENV_LOCK );
    __try {

        retval = _tgetdcwd_nolock(0, pnbuf, maxlen);

    }
    _finally {
        _munlock( _ENV_LOCK );
    }

    return retval;
}

#ifdef _DEBUG

_TSCHAR * __cdecl _tgetcwd_dbg (
        _TSCHAR *pnbuf,
        int maxlen,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
    _TSCHAR *retval;

    _mlock( _ENV_LOCK );
    __try {

    retval = _tgetdcwd_lk_dbg(0, pnbuf, maxlen, nBlockUse, szFileName, nLine);

    }
    _finally {
        _munlock( _ENV_LOCK );
    }

    return retval;
}

#endif  /* _DEBUG */


/***
*_TSCHAR *_getdcwd(drive, pnbuf, maxlen) - get c.w.d. for given drive
*
*Purpose:
*       _getdcwd gets the current working directory for the user,
*       placing it in the buffer pointed to by pnbuf.  It returns
*       the length of the string put in the buffer.  If the length
*       of the string exceeds the length of the buffer, maxlen,
*       then NULL is returned.  If pnbuf = NULL, maxlen is ignored,
*       and a buffer is automatically allocated using malloc() --
*       a pointer to which is returned by _getdcwd().
*
*       side effects: no global data is used or affected
*
*Entry:
*       int drive   - number of the drive being inquired about
*                     0 = default, 1 = 'a:', 2 = 'b:', etc.
*       _TSCHAR *pnbuf - pointer to a buffer maintained by the user;
*       int maxlen  - length of the buffer pointed to by pnbuf;
*
*Exit:
*       Returns pointer to the buffer containing the c.w.d. name
*       (same as pnbuf if non-NULL; otherwise, malloc is
*       used to allocate a buffer)
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tgetdcwd (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen
        )
{
    _TSCHAR *retval;

    _mlock( _ENV_LOCK );
    __try {

        retval = _tgetdcwd_nolock(drive, pnbuf, maxlen);

    }
    _finally {
        _munlock( _ENV_LOCK );
    }

    return retval;
}

#ifdef _DEBUG

_TSCHAR * __cdecl _tgetdcwd_dbg (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
    _TSCHAR *retval;

    _mlock( _ENV_LOCK );
    __try {

        retval = _tgetdcwd_lk_dbg(drive, pnbuf, maxlen, nBlockUse, szFileName, nLine);

    }
    _finally {
        _munlock( _ENV_LOCK );
    }

    return retval;
}

#endif  /* _DEBUG */

#ifdef _DEBUG

_TSCHAR * __cdecl _tgetdcwd_nolock (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen
        )
{
    return _tgetdcwd_lk_dbg(drive, pnbuf, maxlen, _NORMAL_BLOCK, NULL, 0);
}

_TSCHAR * __cdecl _tgetdcwd_lk_dbg (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )

#else  /* _DEBUG */

_TSCHAR * __cdecl _tgetdcwd_nolock (
        int drive,
        _TSCHAR *pnbuf,
        int maxlen
        )

#endif  /* _DEBUG */
{
    _TSCHAR *p;
    _TSCHAR drvstr[4];
    int ret, count;
    _TSCHAR *pname; /* only used as argument to GetFullPathName */

    if ( drive != 0 ) {
        /*
         * Not the default drive - make sure it's valid.
         */
        if ( !_validdrive(drive) ) {
            _doserrno = ERROR_INVALID_DRIVE;
            _VALIDATE_RETURN(("Invalid Drive",0), EACCES, NULL);
        }
    } else {
        /* Get the drive index of the default drive */
        drive = _getdrive();

    }

    /* If pnbuf is NULL, we pass 0 in both the 2nd & 3rd parameter
    to find required length to allocate */

    if(pnbuf)
    {
        _VALIDATE_RETURN( (maxlen > 0), EINVAL, NULL);
        count = maxlen;
        pnbuf[0] = 0;
    }
    else
        count = 0;

    /*
     * Get the current directory string on that drive and its length
     */
    if(drive!=0)
    {
        drvstr[0] = _T('A') - 1 + drive;
        drvstr[1] = _T(':');
        drvstr[2] = _T('.');
        drvstr[3] = _T('\0');
    }
    else
    {
        drvstr[0] = _T('.');
        drvstr[1] = _T('\0');
    }

    ret = GetFullPathName( drvstr, count, pnbuf, &pname );

    if(ret == 0) {
        _dosmaperr( GetLastError() );
        return NULL;
    }

    if( pnbuf != NULL ) {
        if(ret < count) {
            /* GetFullPathName always returns a value less than that
            passed in if successful i.e. we got the required dir */
            return pnbuf;
        }

        errno = ERANGE;
        *pnbuf = 0;
        return NULL;
    }

    /* The code comes here only if pnbuf was NULL
    Allocate the required memory & call GetFullPathName again*/

    maxlen = __max(ret, maxlen) ;
#ifdef _DEBUG
    if( (p = (_TSCHAR *)_calloc_dbg( maxlen, sizeof(_TSCHAR), nBlockUse, szFileName, nLine)) == NULL) {
#else  /* _DEBUG */
    if( (p = (_TSCHAR *)calloc( maxlen, sizeof(_TSCHAR))) == NULL) {
#endif  /* _DEBUG */
        errno = ENOMEM;
        _doserrno = E_nomem;
        return NULL;
    }


    ret = GetFullPathName( drvstr, maxlen, p, &pname );

    if(ret == 0 || ret >= maxlen) {
        _dosmaperr( GetLastError() );
        return NULL;
    }

    return p;

}

#ifndef WPRFLAG

/***
*int _validdrive( unsigned drive ) -
*
*Purpose: returns non zero if drive is a valid drive number.
*
*Entry: drive = 0 => default drive, 1 => a:, 2 => b: ...
*
*Exit:  0 => drive does not exist.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _validdrive (
    unsigned drive
    )
{
        unsigned retcode;
        char drvstr[4];

        if ( drive == 0 )
            return 1;

        drvstr[0] = 'A' + drive - 1;
        drvstr[1] = ':';
        drvstr[2] = '\\';
        drvstr[3] = '\0';

        if ( ((retcode = GetDriveType( drvstr )) == DRIVE_UNKNOWN) ||
             (retcode == DRIVE_NO_ROOT_DIR) )
            return 0;

        return 1;
}

#endif  /* WPRFLAG */
