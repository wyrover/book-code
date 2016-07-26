/***
*swprintn.cpp - Standard version of swprintf
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       The swprintf() flavor takes a count argument that is
*       the max number of wide characters that should be written to
*       the user's buffer.
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
*int swprintf(string, cnt, format, ...) - print formatted data to string
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
*       The swprintf C++ flavor take a count argument that is the max
*       number of wide characters that should be written to the user's buffer.
*
*       Multi-thread: (1) Since there is no stream, this routine must
*       never try to get the stream lock (i.e., there is no stream
*       lock either). (2) Also, since there is only one statically
*       allocated 'fake' iob, we must lock/unlock to prevent collisions.
*
*Entry:
*       wchar_t *string - pointer to place to put output
*       size_t count - max number of wide characters to put in buffer
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

/* We don't pull in the inline version of _vswprintf_l in the headers
Hence we have to prototype _vswprintf_l here to pull in the one in the
library */
int __cdecl _vswprintf_l (wchar_t *, size_t ,const wchar_t *, _locale_t, va_list );

#if defined (_NATIVE_WCHAR_T_DEFINED)

int __cdecl swprintf (
        unsigned short *string,
        size_t count,
        const unsigned short *format,
        ...
        )
{
                va_list arglist;
                va_start(arglist, format);
                int ret = _vswprintf_l(reinterpret_cast<wchar_t *>(string), count, reinterpret_cast<const wchar_t *>(format), NULL, arglist);
                va_end(arglist);
                return ret;
}
#endif  /* defined (_NATIVE_WCHAR_T_DEFINED) */

int __cdecl swprintf (
        wchar_t *string,
        size_t count,
        const wchar_t *format,
        ...
        )

{
        FILE str;
        REG1 FILE *outfile = &str;
        va_list arglist;
        REG2 int retval;

        va_start(arglist, format);

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

        retval = _woutput_l(outfile,format,NULL,arglist);

        _putc_nolock('\0',outfile); /* no-lock version */
        _putc_nolock('\0',outfile); /* 2nd null byte for wchar_t version */

        return(retval);
}

