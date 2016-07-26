/***
*sprintf.c - print formatted to string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines sprintf() and _snprintf() - print formatted data to string
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <internal.h>
#include <limits.h>
#include <mtdll.h>
#include <stddef.h>

#define MAXSTR INT_MAX


/***
*ifndef _COUNT_
*int sprintf(string, format, ...) - print formatted data to string
*else
*int _snprintf(string, cnt, format, ...) - print formatted data to string
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
*ifdef _COUNT_
*       The _snprintf() flavor takes a count argument that is
*       the max number of bytes that should be written to the
*       user's buffer.
*endif
*
*       Multi-thread: (1) Since there is no stream, this routine must
*       never try to get the stream lock (i.e., there is no stream
*       lock either). (2) Also, since there is only one statically
*       allocated 'fake' iob, we must lock/unlock to prevent collisions.
*
*Entry:
*       char *string - pointer to place to put output
*ifdef _COUNT_
*       size_t count - max number of bytes to put in buffer
*endif
*       char *format - format string to control data format/number
*       of arguments followed by list of arguments, number and type
*       controlled by format string
*
*Exit:
*       returns number of characters printed
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_

int __cdecl sprintf (
        char *string,
        const char *format,
        ...
        )
#else  /* _COUNT_ */

#ifndef _SWPRINTFS_ERROR_RETURN_FIX

int __cdecl _snprintf (
        char *string,
        size_t count,
        const char *format,
        ...
        )
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */

int __cdecl _snprintf_c (
        char *string,
        size_t count,
        const char *format,
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

#ifndef _COUNT_
        outfile->_cnt = MAXSTR;
#else  /* _COUNT_ */
        if(count>INT_MAX)
        {
            /* old-style functions allow any large value to mean unbounded */
            outfile->_cnt = INT_MAX;
        }
        else
        {
            outfile->_cnt = (int)(count);
        }
#endif  /* _COUNT_ */
        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = string;

        retval = _output_l(outfile,format,NULL,arglist);

        if (string == NULL)
            return(retval);

#ifndef _SWPRINTFS_ERROR_RETURN_FIX
        _putc_nolock('\0',outfile); /* no-lock version */

        return(retval);
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
        if((retval >= 0) && (_putc_nolock('\0',outfile) != EOF))
            return(retval);

        string[0] = 0;
        return -1;
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */
}

#ifndef _COUNT_

int __cdecl _sprintf_l (
        char *string,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);

#pragma warning(push)
#pragma warning(disable:4996) // Disable deprecation warning since calling function is also deprecated
    return _vsprintf_l(string, format, plocinfo, arglist);
#pragma warning(pop)
}

#else  /* _COUNT_ */
#ifndef _SWPRINTFS_ERROR_RETURN_FIX

int __cdecl _snprintf_l (
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);

#pragma warning(push)
#pragma warning(disable:4996) // Disable deprecation warning since calling function is also deprecated
    return _vsnprintf_l(string, count, format, plocinfo, arglist);
#pragma warning(pop)
}
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */

int __cdecl _snprintf_c_l (
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
    va_list arglist;
    va_start(arglist, plocinfo);

    return _vsnprintf_c_l(string, count, format, plocinfo, arglist);

}

#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */
#endif  /* _COUNT_ */

#ifndef _COUNT_
int __cdecl sprintf_s (
        char *string,
        size_t sizeInBytes,
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return _vsprintf_s_l(string, sizeInBytes, format, NULL, arglist);
}

int __cdecl _sprintf_s_l (
        char *string,
        size_t sizeInBytes,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return _vsprintf_s_l(string, sizeInBytes, format, plocinfo, arglist);
}

int __cdecl _snprintf_s (
        char *string,
        size_t sizeInBytes,
        size_t count,
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return _vsnprintf_s_l(string, sizeInBytes, count, format, NULL, arglist);
}

int __cdecl _snprintf_s_l (
        char *string,
        size_t sizeInBytes,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return _vsnprintf_s_l(string, sizeInBytes, count, format, plocinfo, arglist);
}

int __cdecl _sprintf_p (
        char *string,
        size_t count,
        const char *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
        return _vsprintf_p_l(string, count, format, NULL, arglist);
}

int __cdecl _sprintf_p_l (
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
        return _vsprintf_p_l(string, count, format, plocinfo, arglist);
}

#endif  /* _COUNT_ */

/***
* _scprintf() - counts the number of character needed to print the formatted
* data
*
*Purpose:
*       Counts the number of characters in the fotmatted data.
*
*Entry:
*       char *format - format string to control data format/number
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
int __cdecl _scprintf (
        const char *format,
        ...
        )
{
        va_list arglist;

        va_start(arglist, format);

        return _vscprintf(format, arglist);
}

int __cdecl _scprintf_p (
        const char *format,
        ...
        )
{
        va_list arglist;

        va_start(arglist, format);

        return _vscprintf_p(format, arglist);

}

int __cdecl _scprintf_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
       va_list arglist;
       va_start(arglist, plocinfo);

       return _vscprintf_l(format, plocinfo, arglist);
}

int __cdecl _scprintf_p_l (
        const char *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;

        va_start(arglist, plocinfo);

        return _vscprintf_p_l(format, plocinfo, arglist);

}
#endif  /* _COUNT_ */
