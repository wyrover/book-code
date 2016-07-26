/***
*fgetwc.c - get a wide character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fgetwc() - read a wide character from a stream
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>
#include <msdos.h>
#include <errno.h>
#include <wchar.h>
#include <tchar.h>
#include <setlocal.h>


/***
*wint_t fgetwc(stream) - read a wide character from a stream
*
*Purpose:
*       reads a wide character from the given stream
*
*Entry:
*       FILE *stream - stream to read wide character from
*
*Exit:
*       returns the wide character read
*       returns WEOF if at end of file or error occurred
*
*Exceptions:
*
*******************************************************************************/

wint_t __cdecl fgetwc (
        REG1 FILE *stream
        )
{
        wint_t retval;

        _VALIDATE_RETURN( (stream != NULL), EINVAL, WEOF);

        _lock_str(stream);
        __try {

        retval = _getwc_nolock(stream);

        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}


#include <fgetwc_nolock.inl>

#undef getwc

wint_t __cdecl getwc (
        FILE *stream
        )
{
        return fgetwc(stream);
}

