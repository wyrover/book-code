/***
*sscanf.c - read formatted data from string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines scanf() - reads formatted data from string
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <dbgint.h>
#include <stdarg.h>
#include <string.h>
#include <internal.h>
#include <mtdll.h>
#include <tchar.h>

/***
*static int vscan_fn([w]inputfn, string, [count], format, ...)
*
*Purpose:
*       this is a helper function which is called by the other functions
*       in this file - sscanf/swscanf/snscanf etc. It calls either _(w)input or
*       _(w)input_s depending on the first parameter.
*
*******************************************************************************/
static int __cdecl vscan_fn (
        TINPUTFN inputfn,
        REG2 const _TCHAR *string,
#ifdef _SNSCANF
        size_t count,
#endif  /* _SNSCANF */
        const _TCHAR *format,
        _locale_t plocinfo,
        va_list arglist
        )
/*
 * 'S'tring 'SCAN', 'F'ormatted
 */
{
        FILE str;
        REG1 FILE *infile = &str;
        REG2 int retval;
#ifndef _SNSCANF
        size_t count=_tcslen(string);
#endif  /* _SNSCANF */

        _VALIDATE_RETURN( (string != NULL), EINVAL, EOF);
        _VALIDATE_RETURN( (format != NULL), EINVAL, EOF);

        infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
        infile->_ptr = infile->_base = (char *) string;

        if(count>(INT_MAX/sizeof(_TCHAR)))
        {
            /* old-style functions allow any large value to mean unbounded */
            infile->_cnt = INT_MAX;
        }
        else
        {
            infile->_cnt = (int)count*sizeof(_TCHAR);
        }

        retval = (inputfn(infile, format, plocinfo, arglist));

        return(retval);
}

/***
*int sscanf(string, format, ...) - read formatted data from string
*
*Purpose:
*       Reads formatted data from string into arguments.  _input does the real
*       work here.  Sets up a FILE so file i/o operations can be used, makes
*       string look like a huge buffer to it, but _filbuf will refuse to refill
*       it if it is exhausted.
*
*       Allocate the 'fake' _iob[] entryit statically instead of on
*       the stack so that other routines can assume that _iob[] entries are in
*       are in DGROUP and, thus, are near.
*
*       Multi-thread: (1) Since there is no stream, this routine must never try
*       to get the stream lock (i.e., there is no stream lock either).  (2)
*       Also, since there is only one staticly allocated 'fake' iob, we must
*       lock/unlock to prevent collisions.
*
*Entry:
*       char *string - string to read data from
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
/***
*int snscanf(string, size, format, ...) - read formatted data from string of
*    given length
*
*Purpose:
*       Reads formatted data from string into arguments.  _input does the real
*       work here.  Sets up a FILE so file i/o operations can be used, makes
*       string look like a huge buffer to it, but _filbuf will refuse to refill
*       it if it is exhausted.
*
*       Allocate the 'fake' _iob[] entryit statically instead of on
*       the stack so that other routines can assume that _iob[] entries are in
*       are in DGROUP and, thus, are near.
*
*       Multi-thread: (1) Since there is no stream, this routine must never try
*       to get the stream lock (i.e., there is no stream lock either).  (2)
*       Also, since there is only one staticly allocated 'fake' iob, we must
*       lock/unlock to prevent collisions.
*
*Entry:
*       char *string - string to read data from
*       size_t count - length of string
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
#ifdef _UNICODE
#ifdef _SNSCANF
int __cdecl _snwscanf (
#else  /* _SNSCANF */
int __cdecl swscanf (
#endif  /* _SNSCANF */
#else  /* _UNICODE */
#ifdef _SNSCANF
int __cdecl _snscanf (
#else  /* _SNSCANF */
int __cdecl sscanf (
#endif  /* _SNSCANF */
#endif  /* _UNICODE */
        REG2 const _TCHAR *string,
#ifdef _SNSCANF
        size_t count,
#endif  /* _SNSCANF */
        const _TCHAR *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
#ifdef _SNSCANF
        return vscan_fn(_tinput_l, string, count, format, NULL, arglist);
#else  /* _SNSCANF */
        return vscan_fn(_tinput_l, string, format, NULL, arglist);
#endif  /* _SNSCANF */

}

#ifdef _UNICODE
#ifdef _SNSCANF
int __cdecl _snwscanf_l (
#else  /* _SNSCANF */
int __cdecl _swscanf_l (
#endif  /* _SNSCANF */
#else  /* _UNICODE */
#ifdef _SNSCANF
int __cdecl _snscanf_l (
#else  /* _SNSCANF */
int __cdecl _sscanf_l (
#endif  /* _SNSCANF */
#endif  /* _UNICODE */
        REG2 const _TCHAR *string,
#ifdef _SNSCANF
        size_t count,
#endif  /* _SNSCANF */
        const _TCHAR *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
#ifdef _SNSCANF
        return vscan_fn(_tinput_l, string, count, format, plocinfo, arglist);
#else  /* _SNSCANF */
        return vscan_fn(_tinput_l, string, format, plocinfo, arglist);
#endif  /* _SNSCANF */

}

/***
*int sscanf_s(string, format, ...)
*   Same as sscanf above except that it calls _input_s to do the real work.
*
*int snscanf_s(string, size, format, ...)
*   Same as snscanf above except that it calls _input_s to do the real work.
*
*   _input_s has a size check for array parameters.
*
*******************************************************************************/
#ifdef _UNICODE
#ifdef _SNSCANF
int __cdecl _snwscanf_s (
#else  /* _SNSCANF */
int __cdecl swscanf_s (
#endif  /* _SNSCANF */
#else  /* _UNICODE */
#ifdef _SNSCANF
int __cdecl _snscanf_s (
#else  /* _SNSCANF */
int __cdecl sscanf_s (
#endif  /* _SNSCANF */
#endif  /* _UNICODE */
        REG2 const _TCHAR *string,
#ifdef _SNSCANF
        size_t count,
#endif  /* _SNSCANF */
        const _TCHAR *format,
        ...
        )
{
        va_list arglist;
        va_start(arglist, format);
#ifdef _SNSCANF
        return vscan_fn(_tinput_s_l, string, count, format, NULL, arglist);
#else  /* _SNSCANF */
        return vscan_fn(_tinput_s_l, string, format, NULL, arglist);
#endif  /* _SNSCANF */

}

#ifdef _UNICODE
#ifdef _SNSCANF
int __cdecl _snwscanf_s_l (
#else  /* _SNSCANF */
int __cdecl _swscanf_s_l (
#endif  /* _SNSCANF */
#else  /* _UNICODE */
#ifdef _SNSCANF
int __cdecl _snscanf_s_l (
#else  /* _SNSCANF */
int __cdecl _sscanf_s_l (
#endif  /* _SNSCANF */
#endif  /* _UNICODE */
        REG2 const _TCHAR *string,
#ifdef _SNSCANF
        size_t count,
#endif  /* _SNSCANF */
        const _TCHAR *format,
        _locale_t plocinfo,
        ...
        )
{
        va_list arglist;
        va_start(arglist, plocinfo);
#ifdef _SNSCANF
        return vscan_fn(_tinput_s_l, string, count, format, plocinfo, arglist);
#else  /* _SNSCANF */
        return vscan_fn(_tinput_s_l, string, format, plocinfo, arglist);
#endif  /* _SNSCANF */

}

