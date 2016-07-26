/***
*vfprintf.c - fprintf from variable arg list
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vfprintf() - print formatted output, but take args from
*       a stdargs pointer.
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
*int vfprintf(stream, format, ap) - print to file from varargs
*
*Purpose:
*       Performs formatted output to a file.  The arg list is a variable
*       argument list pointer.
*
*Entry:
*       FILE *stream - stream to write data to
*       char *format - format string containing data format
*       va_list ap - variable arg list pointer
*
*Exit:
*       returns number of correctly output characters
*       returns negative number if error occurred
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vfprintf_helper (
        OUTPUTFN outfn,
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
/*
 * 'V'ariable argument 'F'ile (stream) 'PRINT', 'F'ormatted
 */
{
        REG1 FILE *stream;
        REG2 int buffing;
        REG3 int retval=0;

        _VALIDATE_RETURN( (str != NULL), EINVAL, -1);
        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        _VALIDATE_STREAM_ANSI_SETRET(stream, EINVAL, retval, -1);
                if(retval==0)
                {
                        buffing = _stbuf(stream);
                        retval = outfn(stream,format,plocinfo, ap );
                        _ftbuf(buffing, stream);
                }

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

int __cdecl _vfprintf_l (
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vfprintf_helper(_output_l, str, format, plocinfo, ap);
}

int __cdecl _vfprintf_s_l (
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vfprintf_helper(_output_s_l, str, format, plocinfo, ap);
}

int __cdecl _vfprintf_p_l (
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return vfprintf_helper(_output_p_l, str, format, plocinfo, ap);
}

int __cdecl vfprintf (
        FILE *str,
        const char *format,
        va_list ap
        )
{
    return vfprintf_helper(_output_l, str, format, NULL, ap);
}

int __cdecl vfprintf_s (
        FILE *str,
        const char *format,
        va_list ap
        )
{
    return vfprintf_helper(_output_s_l, str, format, NULL, ap);
}

int __cdecl _vfprintf_p (
        FILE *str,
        const char *format,
        va_list ap
        )
{
    return vfprintf_helper(_output_p_l, str, format, NULL, ap);
}
