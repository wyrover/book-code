/***
*ctime64.c - convert time argument to a string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _ctime64() - convert time value to string
*
*******************************************************************************/

#include <cruntime.h>
#include <time.h>
#include <internal.h>
#include <internal_securecrt.h>
#include <stddef.h>
#include <tchar.h>

/***
*errno_t _tctime64_s(buffer, sizeInChars, time) - converts a time stored as a __time64_t
*to a string
*
*Purpose:
*       Converts a time stored as a __time64_t to a string of the form:
*              Tue May 01 14:25:03 1984
*
*Entry:
*       __time64_t *time - time value in XENIX format
*
*Exit:
*       errno_t = 0 success
*                 buffer contains time converted to a string
*       errno_t = correct error code
*                 buffer null terminated if it is at least 1 character in size
*
*Exceptions:
*
*******************************************************************************/

errno_t __cdecl _tctime64_s (
        _TSCHAR * buffer,
        size_t sizeInChars,
        const __time64_t *timp
        )
{
        struct tm tmtemp;
        errno_t e;

        _VALIDATE_RETURN_ERRCODE(
            ( ( buffer != NULL ) && ( sizeInChars > 0 ) ),
            EINVAL
        )

        _RESET_STRING(buffer, sizeInChars);

        _VALIDATE_RETURN_ERRCODE( ( timp != NULL ), EINVAL )
        _VALIDATE_RETURN_ERRCODE_NOEXC( ( *timp >= 0 ), EINVAL )

        e = _localtime64_s(&tmtemp, timp);
        if ( e == 0 )
        {
            e = _tasctime_s(buffer, sizeInChars, &tmtemp);
        }
        return e;
}

/***
*_TSCHAR *_tctime64(time) - converts a time stored as a __time64_t to a string
*
*Purpose:
*       Converts a time stored as a __time64_t to a string of the form:
*              Tue May 01 14:25:03 1984
*
*Entry:
*       __time64_t *time - time value in internal, 64-bit format
*
*Exit:
*       returns pointer to static string or NULL if an error occurs
*
*Exceptions:
*
*******************************************************************************/

_TSCHAR * __cdecl _tctime64 (
        const __time64_t *timp
        )
{
        struct tm tmtemp;
        errno_t e;

        _VALIDATE_RETURN( ( timp != NULL ), EINVAL, NULL )
        _VALIDATE_RETURN_NOEXC( ( *timp >= 0 ), EINVAL, NULL )

        e = _localtime64_s(&tmtemp, timp);
        if ( e == 0 )
        {
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
            return _tasctime(&tmtemp);
_END_SECURE_CRT_DEPRECATION_DISABLE
        }
        else
        {
            return(NULL);
        }
}
