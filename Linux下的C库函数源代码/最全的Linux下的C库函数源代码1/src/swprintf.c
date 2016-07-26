/***
*swprintf.c - print formatted to string
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _swprintf(), _swprintf_c and _snwprintf() - print formatted data
*       to string
*
*******************************************************************************/


#include <cruntime.h>

/* This is prevent pulling in the inline
versions of (v)swprintf */
#define _INC_SWPRINTF_INL_
#include <stdio.h>
#include <wchar.h>
#include <dbgint.h>
#include <stdarg.h>
#include <internal.h>
#include <limits.h>
#include <mtdll.h>
#include <stddef.h>

#define MAXSTR INT_MAX


/***
*ifndef _COUNT_
*int _swprintf(string, format, ...) - print formatted data to string
*else
*ifndef _SWPRINTFS_ERROR_RETURN_FIX
*int _snwprintf(string, cnt, format, ...) - print formatted data to string
*else
*int _swprintf_c(string, cnt, format, ...) - print formatted data to string
*endif
*endif
*
*Purpose:
*       Prints formatted data to the using the format string to
*       format data and getting as many arguments as called for
*       Sets up a FILE so file i/o operations can be used, make
*       string look like a huge buffer to it, but _flsbuf will
*       refuse to flush it if it fills up.  Appends '\0' to make
*       it a true string. _output does the real work here
*
*       Allocate the 'fake' _iob[] entry statically instead of on
*       the stack so that other routines can assume that _iob[]
*       entries are in are in DGROUP and, thus, are near.
*
*       We alias swprintf to _swprintf
*
*ifdef _COUNT_
*ifndef _SWPRINTFS_ERROR_RETURN_FIX
*       The _snwprintf() flavor takes a count argument that is
*       the max number of wide characters that should be written to the
*       user's buffer.
*       We don't expose this function directly in the headers.
*else
*       The _swprintf_c() flavor does the same thing as the _snwprintf
*       above, but, it also fixes a bug in the return value in the case
*       when there isn't enough space to write the null terminator
*       We don't fix this bug in _snwprintf because of backward
*       compatibility. In new code, however, _snwprintf is #defined to
*       _swprintf_c so users get the bugfix.
*
*endif
*
*       Multi-thread: (1) Since there is no stream, this routine must
*       never try to get the stream lock (i.e., there is no stream
*       lock either). (2) Also, since there is only one statically
*       allocated 'fake' iob, we must lock/unlock to prevent collisions.
*
*Entry:
*       wchar_t *string - pointer to place to put output
*ifdef _COUNT_
*       size_t count - max number of wide characters to put in buffer
*endif
*       wchar_t *format - format string to control data format/number
*       of arguments followed by list of arguments, number and type
*       controlled by format string
*
*Exit:
*       returns number of wide characters printed
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_

int __cdecl _swprintf (
        wchar_t *string,
        const wchar_t *format,
        ...
        )
#else  /* _COUNT_ */

#ifndef _SWPRINTFS_ERROR_RETURN_FIX
/* Here we implement _snwprintf without the
return value bugfix */

int __cdecl _snwprintf (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        ...
        )
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
int __cdecl _swprintf_c (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        ...
        )
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */

#endif  /* _COUNT_ */

{
        FILE str;
        REG1 FILE *outfile = &str;
        va_list arglist;
        REG2 int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

#ifdef _COUNT_
        _VALIDATE_RETURN( (count == 0) || (string != NULL), EINVAL, -1 );
#else  /* _COUNT_ */
        _VALIDATE_RETURN( (string != NULL), EINVAL, -1 );
#endif  /* _COUNT_ */
        va_start(arglist, format);

        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = (char *) string;
#ifndef _COUNT_
        outfile->_cnt = MAXSTR;
#else  /* _COUNT_ */
        if(count>(INT_MAX/sizeof(wchar_t)))
        {
            /* old-style functions allow any large value to mean unbounded */
            outfile->_cnt = INT_MAX;
        }
        else
        {
            outfile->_cnt = (int)(count*sizeof(wchar_t));
        }
#endif  /* _COUNT_ */

        retval = _woutput_l(outfile,format,NULL,arglist);

#ifndef _SWPRINTFS_ERROR_RETURN_FIX

        _putc_nolock('\0',outfile); /* no-lock version */
        _putc_nolock('\0',outfile); /* 2nd null byte for wchar_t version */

        return(retval);
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
        if((retval >= 0) && (_putc_nolock('\0',outfile) != EOF) && (_putc_nolock('\0',outfile) != EOF))
            return(retval);

        string[0] = 0;
        return -1;
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */
}

#ifndef _COUNT_

int __cdecl __swprintf_l (
        wchar_t *string,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);
#pragma warning(push)
#pragma warning(disable:4996) // Disable deprecation warning since calling function is also deprecated
    return __vswprintf_l(string, format, plocinfo, arglist);
#pragma warning(pop)
}

#else  /* _COUNT_ */

#ifndef _SWPRINTFS_ERROR_RETURN_FIX
/* Here we implement _snwprintf without the
return value bugfix */

int __cdecl _snwprintf_l (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);
#pragma warning(push)
#pragma warning(disable:4996) // Disable deprecation warning since calling function is also deprecated
    return _vsnwprintf_l(string, count, format, plocinfo, arglist);
#pragma warning(pop)
}

#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
int __cdecl _swprintf_c_l (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);

    return _vswprintf_c_l(string, count, format, plocinfo, arglist);
}

#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */

#endif  /* _COUNT_ */

#ifndef _COUNT_
int __cdecl swprintf_s (
        wchar_t *string,
        size_t sizeInWords,
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vswprintf_s_l(string, sizeInWords, format, NULL, arglist);
}

int __cdecl _snwprintf_s (
        wchar_t *string,
        size_t sizeInWords,
        size_t count,
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vsnwprintf_s_l(string, sizeInWords, count, format, NULL, arglist);
}

int __cdecl _swprintf_p (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        ...
        )
{
    va_list arglist;

    va_start(arglist, format);

    return _vswprintf_p_l(string, count, format, NULL, arglist);
}

int __cdecl _swprintf_s_l (
        wchar_t *string,
        size_t sizeInWords,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vswprintf_s_l(string, sizeInWords, format, plocinfo, arglist);
}

int __cdecl _snwprintf_s_l (
        wchar_t *string,
        size_t sizeInWords,
        size_t count,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vsnwprintf_s_l(string, sizeInWords, count, format, plocinfo, arglist);
}

int __cdecl _swprintf_p_l (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;

    va_start(arglist, plocinfo);

    return _vswprintf_p_l(string, count, format, plocinfo, arglist);
}

#endif  /* _COUNT_ */

/***
* _scwprintf() - counts the number of character needed to print the formatted
* data
*
*Purpose:
*       Counts the number of characters in the fotmatted data.
*
*Entry:
*       wchar_t *format - format string to control data format/number
*       of arguments followed by list of arguments, number and type
*       controlled by format string
*
*Exit:
*       returns number of characters needed to print formatted data.
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_
int __cdecl _scwprintf (
        const wchar_t *format,
        ...
        )
{
        va_list arglist;

        va_start(arglist, format);

        return _vscwprintf(format, arglist);
}

int __cdecl _scwprintf_p (
        const wchar_t *format,
        ...
        )
{
        va_list arglist;

        va_start(arglist, format);

        return _vscwprintf_p(format, arglist);
}

int __cdecl _scwprintf_l (
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);

    return _vscwprintf_l(format, plocinfo,arglist);
}

int __cdecl _scwprintf_p_l (
        const wchar_t *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;

        va_start(arglist, plocinfo);

        return _vscwprintf_p_l(format, plocinfo, arglist);
}

#endif  /* _COUNT_ */
