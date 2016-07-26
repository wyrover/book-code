/***
*vfwprintf.c - fwprintf from variable arg list
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vfwprintf() - print formatted output, but take args from
*       a stdargs pointer.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <wchar.h>
#include <dbgint.h>
#include <stdarg.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>
#include <stddef.h>

/***
*int vfwprintf(stream, format, ap) - print to file from varargs
*
*Purpose:
*       Performs formatted output to a file.  The arg list is a variable
*       argument list pointer.
*
*Entry:
*       FILE *stream - stream to write data to
*       wchar_t *format - format string containing data format
*       va_list ap - variable arg list pointer
*
*Exit:
*       returns number of correctly output wide characters
*       returns negative number if error occurred
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vfwprintf_helper (
        WOUTPUTFN woutfn,
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
/*
 * 'V'ariable argument 'F'ile (stream) 'W'char_t 'PRINT', 'F'ormatted
 */
{
        REG1 FILE *stream;
        REG2 int buffing;
        REG3 int retval;

        _VALIDATE_RETURN( (str != NULL), EINVAL, -1);
        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        buffing = _stbuf(stream);
        retval = woutfn(stream,format,plocinfo,ap );
        _ftbuf(buffing, stream);

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

int __cdecl _vfwprintf_l (
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vfwprintf_helper(_woutput_l, str, format, plocinfo, ap);
}

int __cdecl _vfwprintf_s_l (
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vfwprintf_helper(_woutput_s_l, str, format, plocinfo, ap);
}

int __cdecl _vfwprintf_p_l (
        FILE *str,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vfwprintf_helper(_woutput_p_l, str, format, plocinfo, ap);
}

int __cdecl vfwprintf (
        FILE *str,
        const wchar_t *format,
        va_list ap
        )
{
    return vfwprintf_helper(_woutput_l, str, format, NULL, ap);
}

int __cdecl vfwprintf_s (
        FILE *str,
        const wchar_t *format,
        va_list ap
        )
{
    return vfwprintf_helper(_woutput_s_l, str, format, NULL, ap);
}

int __cdecl _vfwprintf_p (
        FILE *str,
        const wchar_t *format,
        va_list ap
        )
{
    return vfwprintf_helper(_woutput_p_l, str, format, NULL, ap);
}

