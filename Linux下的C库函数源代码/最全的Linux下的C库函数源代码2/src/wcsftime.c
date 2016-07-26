/***
*wcsftime.c - String Format Time
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
*******************************************************************************/


#include <cruntime.h>
#include <internal.h>
#include <mtdll.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <time.h>
#include <dbgint.h>
#include <malloc.h>
#include <excpt.h>
#include <errno.h>
#include <locale.h>
#include <setlocal.h>

extern "C" size_t __cdecl _Strftime_l (char *string, size_t maxsize, const char *format,
        const struct tm *timeptr, void *lc_time_arg, _locale_t plocinfo);


/***
*size_t wcsftime(wstring, maxsize, format, timeptr) - Format a time string
*
*Purpose:
*       The wcsftime functions is equivalent to to the strftime function, except
*       that the argument 'wstring' specifies an array of a wide string into
*       which the generated output is to be placed. The wcsftime acts as if
*       strftime were called and the result string converted by mbstowcs().
*       [ISO]
*
*Entry:
*       wchar_t *wstring = pointer to output string
*       size_t maxsize = max length of string
*       const wchar_t *format = format control string
*       const struct tm *timeptr = pointer to tb data structure
*
*Exit:
*       !0 = If the total number of resulting characters including the
*       terminating null is not more than 'maxsize', then return the
*       number of wide chars placed in the 'wstring' array (not including the
*       null terminator).
*
*       0 = Otherwise, return 0 and the contents of the string are
*       indeterminate.
*
*Exceptions:
*
*******************************************************************************/

static size_t __cdecl _wcsftime_l_stat (
        wchar_t *wstring,
        size_t maxsize,
        const wchar_t *wformat,
        const struct tm *timeptr,
        _locale_t plocinfo
        )
{
    size_t retval = 0;
    char *format = NULL;
    char *string = NULL;
    size_t flen = 0;

    _VALIDATE_RETURN( ( wstring != NULL ), EINVAL, 0)
    _VALIDATE_RETURN( ( maxsize != 0 ), EINVAL, 0)
    if ( maxsize > 0 )
    {
        *wstring = '\0';
    }

    _VALIDATE_RETURN( ( wformat != NULL ), EINVAL, 0)

    flen = wcslen(wformat) + 1;

    string = (char *)_calloca(sizeof(char)*2, maxsize);
    if ( string == NULL )
    {
        return 0;
    }

    format = (char *)_calloca(sizeof(char)*2, flen);
    if ( format == NULL )
    {
        goto done;
    }

    if (_ERRCHECK_EINVAL_ERANGE(_wcstombs_s_l(NULL, format, flen * 2, wformat, flen * 2 - 1, plocinfo)) != 0)
    {
        goto done;
    }

    if (_Strftime_l(string, maxsize * 2, format, timeptr, 0, plocinfo))
    {
        if (_ERRCHECK_EINVAL_ERANGE(_mbstowcs_s_l(&retval, wstring, maxsize, string, _TRUNCATE, plocinfo)) != 0)
        {
            // VSW 435529: Set errno here if we overflowed the buffer.
            if (retval >= maxsize)
            {
                errno = ERANGE;
            }
            retval = 0;
        }
        /* mbstowcs_s  returns the number of characters including the null-terminator;
         * _wcsftime just returns the number of characters.
         */
        if (retval > 0)
        {
            --retval;
        }
    }
    else
    {
        retval = 0;
    }

done:
    if ( format != NULL )
        _freea(format);

    _freea(string);

    if (retval == 0)
    {
        *wstring = '\0';
    }

    return retval;
}

extern "C" size_t __cdecl _wcsftime_l (
        wchar_t *wstring,
        size_t maxsize,
        const wchar_t *wformat,
        const struct tm *timeptr,
        _locale_t plocinfo
        )
{
    _LocaleUpdate _loc_update(plocinfo);

    return _wcsftime_l_stat(
            wstring,
            maxsize,
            wformat,
            timeptr,
            _loc_update.GetLocaleT());
}

extern "C" size_t __cdecl wcsftime (
        wchar_t *wstring,
        size_t maxsize,
        const wchar_t *wformat,
        const struct tm *timeptr
        )
{
    return _wcsftime_l(wstring, maxsize, wformat, timeptr, NULL);
}

