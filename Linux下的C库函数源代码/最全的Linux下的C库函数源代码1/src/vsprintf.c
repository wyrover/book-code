/***
*vsprintf.c - print formatted data into a string from var arg list
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines vsprintf(), _vsnprintf() and _vsnprintf_s() - print formatted output to
*       a string, get the data from an argument ptr instead of explicit
*       arguments.
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
*int vsprintf(string, format, ap) - print formatted data to string from arg ptr
*else
*int _vsnprintf(string, cnt, format, ap) - print formatted data to string from arg ptr
*endif
*
*Purpose:
*       Prints formatted data, but to a string and gets data from an argument
*       pointer.
*       Sets up a FILE so file i/o operations can be used, make string look
*       like a huge buffer to it, but _flsbuf will refuse to flush it if it
*       fills up. Appends '\0' to make it a true string.
*
*       Allocate the 'fake' _iob[] entryit statically instead of on
*       the stack so that other routines can assume that _iob[] entries are in
*       are in DGROUP and, thus, are near.
*
*ifdef _COUNT_
*       The _vsnprintf() flavor takes a count argument that is
*       the max number of bytes that should be written to the
*       user's buffer.
*endif
*
*       Multi-thread: (1) Since there is no stream, this routine must never try
*       to get the stream lock (i.e., there is no stream lock either).  (2)
*       Also, since there is only one staticly allocated 'fake' iob, we must
*       lock/unlock to prevent collisions.
*
*Entry:
*       char *string - place to put destination string
*ifdef _COUNT_
*       size_t count - max number of bytes to put in buffer
*endif
*       char *format - format string, describes format of data
*       va_list ap - varargs argument pointer
*
*Exit:
*       returns number of characters in string
*       returns -2 if the string has been truncated (only in _vsnprintf_helper)
*       returns -1 in other error cases
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_

#ifdef _SWPRINTFS_ERROR_RETURN_FIX
#error "_COUNT_ must be defined if _SWPRINTFS_ERROR_RETURN_FIX is defined"
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */

int __cdecl _vsprintf_l (
        char *string,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )

#else  /* _COUNT_ */

#ifndef _SWPRINTFS_ERROR_RETURN_FIX

int __cdecl _vsnprintf_l (
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
int __cdecl _vsnprintf_helper (
        OUTPUTFN outfn,
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */
#endif  /* _COUNT_ */

{
        FILE str;
        REG1 FILE *outfile = &str;
        REG2 int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

#ifdef _COUNT_
        _VALIDATE_RETURN( (count == 0) || (string != NULL), EINVAL, -1 );
#else  /* _COUNT_ */
        _VALIDATE_RETURN( (string != NULL), EINVAL, -1 );
#endif  /* _COUNT_ */

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
            outfile->_cnt = (int)count;
        }
#endif  /* _COUNT_ */

        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = string;

#ifndef _SWPRINTFS_ERROR_RETURN_FIX
        retval = _output_l(outfile, format, plocinfo, ap );
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
        retval = outfn(outfile, format, plocinfo, ap );
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */

        if ( string==NULL)
            return(retval);

#ifndef _SWPRINTFS_ERROR_RETURN_FIX

        _putc_nolock('\0',outfile);

        return(retval);
#else  /* _SWPRINTFS_ERROR_RETURN_FIX */
        if((retval >= 0) && (_putc_nolock('\0',outfile) != EOF))
            return(retval);

        string[count - 1] = 0;

        if (outfile->_cnt < 0)
        {
            /* the buffer was too small; we return -2 to indicate truncation */
            return -2;
        }
        return -1;
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */
}

#ifndef _COUNT_
int _cdecl vsprintf(
        char *string,
        const char *format,
        va_list ap
        )
{
#pragma warning(push)
#pragma warning(disable:4996) // Disable deprecation warning since calling function is also deprecated
    return _vsprintf_l(string, format, NULL, ap);
#pragma warning(pop)
}

#else  /* _COUNT_ */
#ifndef _SWPRINTFS_ERROR_RETURN_FIX
int __cdecl _vsnprintf (
        char *string,
        size_t count,
        const char *format,
        va_list ap
        )
{
#pragma warning(push)
#pragma warning(disable:4996) // Disable deprecation warning since calling function is also deprecated
    return _vsnprintf_l(string, count, format, NULL, ap);
#pragma warning(pop)
}
#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */
#endif  /* _COUNT_ */

/* _SWPRINTFS_ERROR_RETURN_FIX implies _COUNT_ */
#ifdef _SWPRINTFS_ERROR_RETURN_FIX

int __cdecl _vsnprintf_c (
        char *string,
        size_t count,
        const char *format,
        va_list ap
        )
{
    int retval = _vsnprintf_helper(_output_l, string, count, format, NULL, ap);
    return (retval < 0 ? -1 : retval);
}

int __cdecl _vsnprintf_c_l (
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    int retval = _vsnprintf_helper(_output_l, string, count, format, plocinfo, ap);
    return (retval < 0 ? -1 : retval);
}

int __cdecl _vsprintf_s_l (
        char *string,
        size_t sizeInBytes,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    int retvalue = -1;

    /* validation section */
    _VALIDATE_RETURN(format != NULL, EINVAL, -1);
    _VALIDATE_RETURN(string != NULL && sizeInBytes > 0, EINVAL, -1);

    retvalue = _vsnprintf_helper(_output_s_l, string, sizeInBytes, format, plocinfo, ap);
    if (retvalue < 0)
    {
        string[0] = 0;
        _SECURECRT__FILL_STRING(string, sizeInBytes, 1);
    }
    if (retvalue == -2)
    {
        _VALIDATE_RETURN(("Buffer too small", 0), ERANGE, -1);
    }
    if (retvalue >= 0)
    {
        _SECURECRT__FILL_STRING(string, sizeInBytes, retvalue + 1);
    }

    return retvalue;
}

int __cdecl vsprintf_s (
        char *string,
        size_t sizeInBytes,
        const char *format,
        va_list ap
        )
{
    return _vsprintf_s_l(string, sizeInBytes, format, NULL, ap);
}

int __cdecl _vsnprintf_s_l (
        char *string,
        size_t sizeInBytes,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    int retvalue = -1;
    errno_t save_errno = 0;

    /* validation section */
    _VALIDATE_RETURN(format != NULL, EINVAL, -1);
    if (count == 0 && string == NULL && sizeInBytes == 0)
    {
        /* this case is allowed; nothing to do */
        return 0;
    }
    _VALIDATE_RETURN(string != NULL && sizeInBytes > 0, EINVAL, -1);

    if (sizeInBytes > count)
    {
        save_errno = errno;
        retvalue = _vsnprintf_helper(_output_s_l, string, count + 1, format, plocinfo, ap);
        if (retvalue == -2)
        {
            /* the string has been truncated, return -1 */
            _SECURECRT__FILL_STRING(string, sizeInBytes, count + 1);
            if (errno == ERANGE)
            {
                errno = save_errno;
            }
            return -1;
        }
    }
    else /* sizeInBytes <= count */
    {
        save_errno = errno;
        retvalue = _vsnprintf_helper(_output_s_l, string, sizeInBytes, format, plocinfo, ap);
        string[sizeInBytes - 1] = 0;
        /* we allow truncation if count == _TRUNCATE */
        if (retvalue == -2 && count == _TRUNCATE)
        {
            if (errno == ERANGE)
            {
                errno = save_errno;
            }
            return -1;
        }
    }

    if (retvalue < 0)
    {
        string[0] = 0;
        _SECURECRT__FILL_STRING(string, sizeInBytes, 1);
        if (retvalue == -2)
        {
            _VALIDATE_RETURN(("Buffer too small", 0), ERANGE, -1);
        }
        return -1;
    }

    _SECURECRT__FILL_STRING(string, sizeInBytes, retvalue + 1);

    return (retvalue < 0 ? -1 : retvalue);
}

int __cdecl _vsnprintf_s (
        char *string,
        size_t sizeInBytes,
        size_t count,
        const char *format,
        va_list ap
        )
{
    return _vsnprintf_s_l(string, sizeInBytes, count, format, NULL, ap);
}

int __cdecl _vsprintf_p (
        char *string,
        size_t count,
        const char *format,
        va_list ap
        )
{
    int retval = _vsnprintf_helper(_output_p_l, string, count, format, NULL, ap);
    return (retval < 0 ? -1 : retval);
}

int __cdecl _vsprintf_p_l (
        char *string,
        size_t count,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    int retval = _vsnprintf_helper(_output_p_l, string, count, format, plocinfo, ap);
    return (retval < 0 ? -1 : retval);
}

#endif  /* _SWPRINTFS_ERROR_RETURN_FIX */

/***
* _vscprintf() - counts the number of character needed to print the formatted
* data
*
*Purpose:
*       Counts the number of characters in the fotmatted data.
*
*Entry:
*       char *format - format string, describes format of data
*       va_list ap - varargs argument pointer
*
*Exit:
*       returns number of characters needed to print formatted data.
*
*Exceptions:
*
*******************************************************************************/

#ifndef _COUNT_

int __cdecl _vscprintf_helper (
        OUTPUTFN outfn,
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
        FILE str;
        REG1 FILE *outfile = &str;
        REG2 int retval;

        _VALIDATE_RETURN( (format != NULL), EINVAL, -1);

        outfile->_cnt = MAXSTR;
        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = NULL;

        retval = outfn(outfile, format, plocinfo, ap);
        return(retval);
}

int __cdecl _vscprintf (
        const char *format,
        va_list ap
        )
{
        return _vscprintf_helper(_output_l, format, NULL, ap);
}

int __cdecl _vscprintf_l (
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
        return _vscprintf_helper(_output_l, format, plocinfo, ap);
}

int __cdecl _vscprintf_p (
        const char *format,
        va_list ap
        )
{
        return _vscprintf_helper(_output_p_l, format, NULL, ap);
}

int __cdecl _vscprintf_p_l (
        const char *format,
        _locale_t plocinfo,
        va_list ap
        )
{
        return _vscprintf_helper(_output_p_l, format, plocinfo, ap);
}
#endif  /* _COUNT_ */
