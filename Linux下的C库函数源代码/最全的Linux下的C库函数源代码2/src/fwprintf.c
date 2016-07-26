/***
*fwprintf.c - print formatted data to stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fwprintf() - print formatted data to stream
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
*int fwprintf(stream, format, ...) - print formatted data
*
*Purpose:
*       Prints formatted data on the given using the format string to
*       format data and getting as many arguments as called for
*       _output does the real work here
*
*Entry:
*       FILE *stream - stream to print on
*       wchar_t *format - format string to control data format/number of arguments
*       followed by arguments to print, number and type controlled by
*       format string
*
*Exit:
*       returns number of wide characters printed
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fwprintf (
        FILE *str,
        const wchar_t *format,
        ...
        )
/*
 * 'F'ile (stream) 'W'char_t 'PRINT', 'F'ormatted
 */
{
        va_list(arglist);
        REG1 FILE *stream;
        REG2 int buffing;
        int retval;

        _VALIDATE_RETURN( (str != NULL), EINVAL, -1);
        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        va_start(arglist, format);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        buffing = _stbuf(stream);
        retval = _woutput_l(stream,format,NULL,arglist);
        _ftbuf(buffing, stream);

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

int __cdecl _fwprintf_l (
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vfwprintf_l(str, format, plocinfo, arglist);
}

int __cdecl _fwprintf_s_l (
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vfwprintf_s_l(str, format, plocinfo, arglist);
}

int __cdecl fwprintf_s (
        FILE *str,
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vfwprintf_s_l(str, format, NULL, arglist);
}

int __cdecl _fwprintf_p_l (
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vfwprintf_p_l(str, format, plocinfo, arglist);
}

int __cdecl _fwprintf_p (
        FILE *str,
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vfwprintf_p_l(str, format, NULL, arglist);
}

