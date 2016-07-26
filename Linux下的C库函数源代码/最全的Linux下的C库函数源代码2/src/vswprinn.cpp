/***
*vswprint.c - print formatted data into a string from var arg list
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       defines vswprintf() and _vsnwprintf() - print formatted output to
*       a string, get the data from an argument ptr instead of explicit
*       arguments.
*
*******************************************************************************/


#ifdef CRTDLL
/*
 * Suppress the inline definitions of iswalpha et al.  Necessary to avoid
 * a conflict with the dllexport versions from _wctype.c in the DLL build.
 */
#define _WCTYPE_INLINE_DEFINED
#endif  /* CRTDLL */

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

#define MAXSTR INT_MAX

/***
*int vswprintf(string, cnt, format, ap) - print formatted data to string from arg ptr
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
*       The vswprintf() flavor takes a count argument that is
*       the max number of bytes that should be written to the
*       user's buffer.
*
*       Multi-thread: (1) Since there is no stream, this routine must never try
*       to get the stream lock (i.e., there is no stream lock either).  (2)
*       Also, since there is only one staticly allocated 'fake' iob, we must
*       lock/unlock to prevent collisions.
*
*Entry:
*       wchar_t *string - place to put destination string
*       size_t count - max number of bytes to put in buffer
*       wchar_t *format - format string, describes format of data
*       va_list ap - varargs argument pointer
*
*Exit:
*       returns number of wide characters in string
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _vswprintf_l (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        _locale_t plocinfo,
        va_list ap
        )
{
        FILE str;
        REG1 FILE *outfile = &str;
        REG2 int retval;

        _ASSERTE(string != NULL);
        _ASSERTE(format != NULL);

        outfile->_flag = _IOWRT|_IOSTRG;
        outfile->_ptr = outfile->_base = (char *) string;
        if(count>(INT_MAX/sizeof(wchar_t)))
        {
            /* old-style functions allow any large value to mean unbounded */
            outfile->_cnt = INT_MAX;
        }
        else
        {
            outfile->_cnt = (int)(count*sizeof(wchar_t));
        }

        retval = _woutput_l(outfile,format,plocinfo,ap );
        _putc_nolock('\0',outfile);     /* no-lock version */
        _putc_nolock('\0',outfile);     /* 2nd byte for wide char version */

        return(retval);
}

int __cdecl vswprintf (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        va_list ap
        )
{
    return _vswprintf_l(string, count, format, NULL, ap);
}

#if defined (_NATIVE_WCHAR_T_DEFINED)
int __cdecl _vswprintf_l (
        unsigned short *string,
        size_t count,
        const unsigned short *format,
        _locale_t plocinfo,
        va_list ap
        )
{
    return _vswprintf_l(reinterpret_cast<wchar_t *>(string), count, reinterpret_cast<const wchar_t *>(format), plocinfo,ap);
}

int __cdecl vswprintf (
        unsigned short *string,
        size_t count,
        const unsigned short *format,
        va_list ap
        )
{
    return _vswprintf_l(reinterpret_cast<wchar_t *>(string), count, reinterpret_cast<const wchar_t *>(format), NULL, ap);
}
#endif  /* defined (_NATIVE_WCHAR_T_DEFINED) */

