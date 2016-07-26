/***
*wprintf.c - print formatted
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wprintf() - print formatted data
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>
#include <stddef.h>

/***
*int wprintf(format, ...) - print formatted data
*
*Purpose:
*       Prints formatted data on stdout using the format string to
*       format data and getting as many arguments as called for
*       Uses temporary buffering to improve efficiency.
*       _output does the real work here
*
*Entry:
*       wchar_t *format - format string to control data format/number of arguments
*       followed by list of arguments, number and type controlled by
*       format string
*
*Exit:
*       returns number of wide characters printed
*
*Exceptions:
*
*******************************************************************************/

int __cdecl wprintf (
        const wchar_t *format,
        ...
        )
/*
 * stdout 'W'char_t 'PRINT', 'F'ormatted
 */
{
        va_list arglist;
        int buffing;
        int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        va_start(arglist, format);

        _lock_str2(1, stdout);
        __try {

        buffing = _stbuf(stdout);

        retval = _woutput_l(stdout,format,NULL,arglist);

        _ftbuf(buffing, stdout);

        }
        __finally {
            _unlock_str2(1, stdout);
        }

        return(retval);
}

errno_t __cdecl _wprintf_l (
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vwprintf_l(format, plocinfo, arglist);
}

errno_t __cdecl _wprintf_s_l (
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vwprintf_s_l(format, plocinfo, arglist);
}

errno_t __cdecl wprintf_s (
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vwprintf_s_l(format, NULL, arglist);
}

errno_t __cdecl _wprintf_p_l (
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vwprintf_p_l(format, plocinfo, arglist);
}

errno_t __cdecl _wprintf_p (
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vwprintf_p_l(format, NULL, arglist);
}

