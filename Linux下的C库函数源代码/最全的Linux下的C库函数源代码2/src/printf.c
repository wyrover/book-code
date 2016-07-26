/***
*printf.c - print formatted
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines printf() - print formatted data
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
#include <process.h>

/***
*int printf(format, ...) - print formatted data
*
*Purpose:
*       Prints formatted data on stdout using the format string to
*       format data and getting as many arguments as called for
*       Uses temporary buffering to improve efficiency.
*       _output does the real work here
*
*Entry:
*       char *format - format string to control data format/number of arguments
*       followed by list of arguments, number and type controlled by
*       format string
*
*Exit:
*       returns number of characters printed
*
*Exceptions:
*
*******************************************************************************/

int __cdecl printf (
        const char *format,
        ...
        )
/*
 * stdout 'PRINT', 'F'ormatted
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

        retval = _output_l(stdout,format,NULL,arglist);

        _ftbuf(buffing, stdout);

        }
        __finally {
            _unlock_str2(1, stdout);
        }

        return(retval);
}

int __cdecl _printf_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vprintf_l(format, plocinfo, arglist);
}


int __cdecl _printf_s_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vprintf_s_l(format, plocinfo, arglist);
}

int __cdecl printf_s (
        const char *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vprintf_s_l(format, NULL, arglist);
}

int __cdecl _printf_p_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vprintf_p_l(format, plocinfo, arglist);
}

int __cdecl _printf_p (
        const char *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vprintf_p_l(format, NULL, arglist);
}

static UINT_PTR __enable_percent_n = 0;

/***
 *int _set_printf_count_output(int)
 *
 *Purpose:
 *   Enables or disables %n format specifier for printf family functions
 *
 *Internals:
 *   __enable_percent_n is set to (__security_cookie|1) for security reasons;
 *   if set to a static value, an attacker could first modify __enable_percent_n
 *   and then provide a malicious %n specifier.  The cookie is ORed with 1
 *   because a zero cookie is a possibility.
 ******************************************************************************/
int __cdecl _set_printf_count_output(int value)
{
    int old = (__enable_percent_n == (__security_cookie | 1));
    __enable_percent_n = (value ? (__security_cookie | 1) : 0);
    return old;
}

/***
 *int _get_printf_count_output()
 *
 *Purpose:
 *   Checks whether %n format specifier for printf family functions is enabled
 ******************************************************************************/
int __cdecl _get_printf_count_output()
{
    return ( __enable_percent_n == (__security_cookie | 1));
}
