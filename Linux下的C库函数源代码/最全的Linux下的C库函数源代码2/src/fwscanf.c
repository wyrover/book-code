/***
*fwscanf.c - read formatted data from stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fwscanf() - reads formatted data from stream
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

/***
*int vfwscanf(stream, format, ...) - read formatted data from stream
*
*Purpose:
*       This is a helper function to be called from fwscanf & fwscanf_s
*
*Entry:
*       WINPUTFN winputfn - fwscanf & fwscanf_s pass either _winput or
*       _winput_s which is then used to do the real work.
*       FILE *stream - stream to read data from
*       wchar_t *format - format string
*       va_list arglist - arglist of output pointers
*
*Exit:
*       returns number of fields read and assigned
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vfwscanf (
        WINPUTFN winputfn,
        FILE *stream,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list arglist
        )
/*
 * 'F'ile (stream) 'W'char_t 'SCAN', 'F'ormatted
 */
{
        int retval;

        _VALIDATE_RETURN((stream != NULL), EINVAL, EOF);
        _VALIDATE_RETURN((format != NULL), EINVAL, EOF);

        _lock_str(stream);
        __try {

        retval = (winputfn(stream, format, plocinfo, arglist));

        }
        __finally {
            _unlock_str(stream);
        }

        return(retval);
}

/***
*int fwscanf(stream, format, ...) - read formatted data from stream
*
*Purpose:
*       Reads formatted data from stream into arguments.  _input does the real
*       work here.
*
*Entry:
*       FILE *stream - stream to read data from
*       wchar_t *format - format string
*       followed by list of pointers to storage for the data read.  The number
*       and type are controlled by the format string.
*
*Exit:
*       returns number of fields read and assigned
*
*Exceptions:
*
*******************************************************************************/
int __cdecl fwscanf (
        FILE *stream,
        const wchar_t *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vfwscanf(_winput_l, stream, format, NULL, arglist);
}

int __cdecl _fwscanf_l (
        FILE *stream,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return vfwscanf(_winput_l, stream, format, plocinfo, arglist);
}

/***
*int fwscanf_s(stream, format, ...)
*
*   Same as fwscanf above except that it calls _winput_s to do the real work.
*   _winput_s has a size check for array parameters.
*
*Exceptions:
*
*******************************************************************************/
int __cdecl fwscanf_s (
        FILE *stream,
        const wchar_t *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vfwscanf(_winput_s_l, stream, format, NULL, arglist);
}

int __cdecl _fwscanf_s_l (
        FILE *stream,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return vfwscanf(_winput_s_l, stream, format, plocinfo, arglist);
}

