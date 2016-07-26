/***
*scanf.c - read formatted data from stdin
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines scanf() - reads formatted data from stdin
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
*int vscanf(format, ...) - read formatted data from stdin
*
*Purpose:
*       This is a helper function to be called from fscanf & fscanf_s
*
*Entry:
*       INPUTFN inputfn - scanf & scanf_s pass either _input_l or _input_s_l
*                   which is then used to do the real work.
*       char *format - format string
*       va_list arglist - arglist of output pointers
*
*Exit:
*       returns number of fields read and assigned
*
*Exceptions:
*
*******************************************************************************/

int __cdecl vscanf (
        INPUTFN inputfn,
        const char *format,
        _locale_t plocinfo,
        va_list arglist
        )
/*
 * stdin 'SCAN', 'F'ormatted
 */
{
        int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, EOF);

        _lock_str2(0, stdin);
        __try {

        retval = (inputfn(stdin, format, plocinfo, arglist));

        }
        __finally {
            _unlock_str2(0, stdin);
        }

        return(retval);
}

/***
*int scanf(format, ...) - read formatted data from stdin
*
*Purpose:
*       Reads formatted data from stdin into arguments.  _input_l does the real
*       work here.
*
*Entry:
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
int __cdecl scanf (
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vscanf(_input_l, format, NULL, arglist);
}

int __cdecl _scanf_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return vscanf(_input_l, format, plocinfo, arglist);
}

/***
*int scanf_s(format, ...) - read formatted data from stdin
*
*   Same as scanf above except that it calls _input_s_l to do the real work.
*   _input_s_l has a size check for array parameters.
*
*******************************************************************************/
int __cdecl scanf_s (
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return vscanf(_input_s_l, format, NULL, arglist);
}

int __cdecl _scanf_s_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return vscanf(_input_s_l, format, plocinfo, arglist);
}
