/***
*setmode.c - set file translation mode
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defined _setmode() - set file translation mode of a file
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <msdos.h>
#include <mtdll.h>
#include <stddef.h>
#include <internal.h>
#include <stdlib.h>

/***
*int _setmode(fh, mode) - set file translation mode
*
*Purpose:
*       changes file mode to text/binary, depending on mode arg. this affects
*       whether read's and write's on the file translate between CRLF and LF
*       or is untranslated
*
*Entry:
*       int fh - file handle to change mode on
*       int mode - file translation mode (one of O_TEXT and O_BINARY)
*
*Exit:
*       returns old file translation mode
*       returns -1 and sets errno if fails
*
*Exceptions:
*
*******************************************************************************/


int __cdecl _setmode (
        int fh,
        int mode
        )
{
        int retval;

        _VALIDATE_RETURN(((mode == _O_TEXT) ||
                          (mode == _O_BINARY) ||
                          (mode == _O_WTEXT) ||
                          (mode == _O_U8TEXT) ||
                          (mode == _O_U16TEXT)),
                         EINVAL, -1);
        _CHECK_FH_RETURN( fh, EBADF, -1 );
        _VALIDATE_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_RETURN((_osfile(fh) & FOPEN), EBADF, -1);

        /* lock the file */
        _lock_fh(fh);

        __try {
                if ( _osfile(fh) & FOPEN )
                        /* set the text/binary mode */
                        retval = _setmode_nolock(fh, mode);
                else {
                        errno = EBADF;
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                        retval = -1;
                }
        }
        __finally {
                /* unlock the file */
                _unlock_fh(fh);
        }

        /* Return to user (_setmode_nolock sets errno, if needed) */
        return(retval);
}

/***
*_setmode_nolock() - Perform core setmode operation
*
*Purpose:
*       Core setmode code.  Assumes:
*       (1) Caller has validated fh to make sure it's in range.
*       (2) Caller has locked the file handle.
*
*       [See _setmode() description above.]
*
*Entry: [Same as _setmode()]
*
*Exit:  [Same as _setmode()]
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _setmode_nolock (
        REG1 int fh,
        int mode
        )
{
        int oldmode;
        int oldtextmode;


        oldmode = _osfile(fh) & FTEXT;
        oldtextmode = _textmode(fh);

        switch(mode) {
            case _O_BINARY :
                _osfile(fh) &= ~FTEXT;
                break;

            case _O_TEXT :
                _osfile(fh) |= FTEXT;
                _textmode(fh) = __IOINFO_TM_ANSI;
                break;

            case _O_U8TEXT :
                _osfile(fh) |= FTEXT;
                _textmode(fh) = __IOINFO_TM_UTF8;
                break;

            case _O_U16TEXT:
            case _O_WTEXT :
                _osfile(fh) |= FTEXT;
                _textmode(fh) = __IOINFO_TM_UTF16LE;
                break;
        }

        if(oldmode == 0) {
            return _O_BINARY;
        }

        if(oldtextmode == __IOINFO_TM_ANSI) {
            return _O_TEXT;
        }
        else {
            return _O_WTEXT;
        }
}

errno_t __cdecl _set_fmode(int mode)
{
    _VALIDATE_RETURN_ERRCODE(((mode == _O_TEXT) || (mode == _O_BINARY) || (mode == _O_WTEXT)), EINVAL);

    _BEGIN_SECURE_CRT_DEPRECATION_DISABLE
    InterlockedExchange(&_fmode, mode);
    _END_SECURE_CRT_DEPRECATION_DISABLE

    return 0;

}

errno_t __cdecl _get_fmode(int * pMode)
{
    _VALIDATE_RETURN_ERRCODE((pMode != NULL), EINVAL);

    _BEGIN_SECURE_CRT_DEPRECATION_DISABLE
    *pMode = _fmode;
    _END_SECURE_CRT_DEPRECATION_DISABLE

    return 0;
}
