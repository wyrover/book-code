/***
*vwprintf.c - wprintf from a var args pointer
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vwprintf() - print formatted data from an argument list pointer
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <wchar.h>
#include <dbgint.h>
#include <stdarg.h>
#include <internal.h>
#include <file2.h>
#include <mtdll.h>
#include <stddef.h>

/***
*int vwprintf(format, ap) - print formatted data from an argument list pointer
*
*Purpose:
*       Prints formatted data items to stdout.  Uses a pointer to a
*       variable length list of arguments instead of an argument list.
*
*Entry:
*       wchar_t *format - format string, describes data format to write
*       va_list ap - pointer to variable length arg list
*
*Exit:
*       returns number of wide characters written
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vwprintf_helper (
        WOUTPUTFN woutfn,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
/*
 * stdout 'V'ariable, 'W'char_t 'PRINT', 'F'ormatted
 */
{
        REG1 FILE *stream = stdout;
        REG2 int buffing;
        REG3 int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        _lock_str(stream);
        __try {

        buffing = _stbuf(stream);
        retval = woutfn(stream, format, plocinfo, ap );
        _ftbuf(buffing, stream);

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

int __cdecl _vwprintf_l (
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vwprintf_helper(_woutput_l, format, plocinfo, ap);
}

int __cdecl _vwprintf_s_l (
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vwprintf_helper(_woutput_s_l, format, plocinfo, ap);
}

int __cdecl _vwprintf_p_l (
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vwprintf_helper(_woutput_p_l, format, plocinfo, ap);
}

int __cdecl vwprintf (
        const wchar_t *format,
        va_list ap
        )
{
    return vwprintf_helper(_woutput_l, format, NULL, ap);
}

int __cdecl vwprintf_s (
        const wchar_t *format,
        va_list ap
        )
{
    return vwprintf_helper(_woutput_s_l, format, NULL, ap);
}

int __cdecl _vwprintf_p (
        const wchar_t *format,
        va_list ap
        )
{
    return vwprintf_helper(_woutput_p_l, format, NULL, ap);
}

