/***
*vprintf.c - printf from a var args pointer
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vprintf() - print formatted data from an argument list pointer
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <internal.h>
#include <file2.h>
#include <mtdll.h>
#include <stddef.h>

/***
*int vprintf(format, ap) - print formatted data from an argument list pointer
*
*Purpose:
*       Prints formatted data items to stdout.  Uses a pointer to a
*       variable length list of arguments instead of an argument list.
*
*Entry:
*       char *format - format string, describes data format to write
*       va_list ap - pointer to variable length arg list
*
*Exit:
*       returns number of characters written
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vprintf_helper (
        OUTPUTFN outfn,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
/*
 * stdout 'V'ariable, 'PRINT', 'F'ormatted
 */
{
        REG1 FILE *stream = stdout;
        REG2 int buffing;
        REG3 int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);


        _lock_str(stream);
        __try {

        buffing = _stbuf(stream);
        retval = outfn(stream, format, plocinfo, ap );
        _ftbuf(buffing, stream);

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

int __cdecl _vprintf_l (
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vprintf_helper(_output_l,format, plocinfo, ap);
}

int __cdecl _vprintf_s_l (
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vprintf_helper(_output_s_l,format, plocinfo, ap);
}

int __cdecl _vprintf_p_l (
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vprintf_helper(_output_p_l,format, plocinfo, ap);
}

int __cdecl vprintf (
        const char *format,
        va_list ap
        )
{
    return vprintf_helper(_output_l,format, NULL, ap);
}

int __cdecl vprintf_s (
        const char *format,
        va_list ap
        )
{
    return vprintf_helper(_output_s_l,format, NULL, ap);
}

int __cdecl _vprintf_p (
        const char *format,
        va_list ap
        )
{
    return vprintf_helper(_output_p_l,format, NULL, ap);
}
