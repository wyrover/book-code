/***
*fprintf.c - print formatted data to stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fprintf() - print formatted data to stream
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
*int fprintf(stream, format, ...) - print formatted data
*
*Purpose:
*       Prints formatted data on the given using the format string to
*       format data and getting as many arguments as called for
*       _output does the real work here
*
*Entry:
*       FILE *stream - stream to print on
*       char *format - format string to control data format/number of arguments
*       followed by arguments to print, number and type controlled by
*       format string
*
*Exit:
*       returns number of characters printed
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fprintf (
        FILE *str,
        const char *format,
        ...
        )
/*
 * 'F'ile (stream) 'PRINT', 'F'ormatted
 */
{
        va_list(arglist);
        REG1 FILE *stream;
        REG2 int buffing;
        int retval=0;

        _VALIDATE_RETURN( (str != NULL), EINVAL, -1);
        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        va_start(arglist, format);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        _VALIDATE_STREAM_ANSI_SETRET(stream, EINVAL, retval, -1);
                if(retval==0)
                {
                        buffing = _stbuf(stream);
                        retval = _output_l(stream,format,NULL,arglist);
                        _ftbuf(buffing, stream);
                }

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

int __cdecl _fprintf_l (
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vfprintf_l(str, format, plocinfo, arglist);
}

int __cdecl _fprintf_s_l (
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vfprintf_s_l(str, format, plocinfo, arglist);
}

int __cdecl fprintf_s (
        FILE *str,
        const char *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vfprintf_s_l(str, format, NULL, arglist);
}

int __cdecl _fprintf_p_l (
        FILE *str,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vfprintf_p_l(str, format, plocinfo, arglist);
}

int __cdecl _fprintf_p (
        FILE *str,
        const char *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vfprintf_p_l(str, format, NULL, arglist);
}
