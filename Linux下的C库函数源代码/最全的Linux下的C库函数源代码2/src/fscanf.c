/***
*fscanf.c - read formatted data from stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fscanf() - reads formatted data from stream
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>

/***
*int vfscanf(stream, format, ...) - read formatted data from stream
*
*Purpose:
*       This is a helper function to be called from fscanf & fscanf_s
*
*Entry:
*       INPUTFN inputfn - fscanf & fscanf_s pass either _input or _input_s
*                   which is then used to do the real work.
*       FILE *stream    - stream to read data from
*       char *format    - format string
*       va_list arglist - arglist of output pointers
*
*Exit:
*       returns number of fields read and assigned
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vfscanf (
        INPUTFN inputfn,
        FILE *stream,
        const char *format,
        _locale_t plocinfo,
        va_list arglist
        )
/*
 * 'F'ile (stream) 'SCAN', 'F'ormatted
 */
{
        int retval=0;

        _VALIDATE_RETURN((stream != NULL), EINVAL, EOF);
        _VALIDATE_RETURN((format != NULL), EINVAL, EOF);

        _lock_str(stream);
        __try {
        _VALIDATE_STREAM_ANSI_SETRET(stream, EINVAL, retval, EOF);

                if(retval==0)
                {
                        retval = (inputfn(stream, format, plocinfo, arglist));
                }

        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}

/***
*int fscanf(stream, format, ...) - read formatted data from stream
*
*Purpose:
*       Reads formatted data from stream into arguments.  _input does the real
*       work here.
*
*Entry:
*       FILE *stream - stream to read data from
*       char *format - format string
*       followed by list of pointers to storage for the data read.  The number
*       and type are controlled by the format string.
*
*Exit:
*       returns number of fields read and assigned
*
*Exceptions:
*
*******************************************************************************/

int __cdecl fscanf (
        FILE *stream,
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vfscanf(_input_l, stream, format, NULL, arglist);
}

int __cdecl _fscanf_l (
        FILE *stream,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return vfscanf(_input_l, stream, format, plocinfo, arglist);
}

/***
*int fscanf_s(stream, format, ...)
*
*   Same as fscanf above except that it calls _input_s to do the real work.
*   _input_s has a size check for array parameters.
*
*******************************************************************************/

int __cdecl fscanf_s (
        FILE *stream,
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vfscanf(_input_s_l, stream, format, NULL, arglist);
}

int __cdecl _fscanf_s_l (
        FILE *stream,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return vfscanf(_input_s_l, stream, format, plocinfo, arglist);
}
