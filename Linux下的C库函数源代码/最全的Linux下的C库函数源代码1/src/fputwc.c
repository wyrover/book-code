/***
*fputwc.c - write a wide character to an output stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fputwc() - writes a wide character to a stream
*
*******************************************************************************/


#include <cruntime.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbgint.h>
#include <file2.h>
#include <internal.h>
#include <mtdll.h>
#include <msdos.h>
#include <errno.h>
#include <wchar.h>
#include <tchar.h>
#include <setlocal.h>


/***
*wint_t fputwc(ch, stream) - write a wide character to a stream
*
*Purpose:
*       Writes a wide character to a stream.  Function version of putwc().
*
*Entry:
*       wchar_t ch - wide character to write
*       FILE *stream - stream to write to
*
*Exit:
*       returns the wide character if successful
*       returns WEOF if fails
*
*Exceptions:
*
*******************************************************************************/

wint_t __cdecl fputwc (
        wchar_t ch,
        FILE *str
        )
{
        REG1 FILE *stream;
        REG2 wint_t retval;

        _VALIDATE_RETURN((str != NULL), EINVAL, WEOF);

        /* Init stream pointer */
        stream = str;

        _lock_str(stream);
        __try {

        retval = _fputwc_nolock(ch,stream);

        }
        __finally {
                _unlock_str(stream);
        }

        return(retval);
}

/***
*_fputwc_nolock() -  putwc() core routine (locked version)
*
*Purpose:
*       Core putwc() routine; assumes stream is already locked.
*
*       [See putwc() above for more info.]
*
*Entry: [See putwc()]
*
*Exit:  [See putwc()]
*
*Exceptions:
*
*******************************************************************************/

wint_t __cdecl _fputwc_nolock (
        wchar_t ch,
        FILE *str
        )
{


        if (!(str->_flag & _IOSTRG))
        {
            if (_textmode_safe(_fileno(str)) == __IOINFO_TM_UTF16LE)
            {
                /* binary (Unicode) mode */
                if ( (str->_cnt -= sizeof(wchar_t)) >= 0 ) {
                    return (wint_t) (0xffff & (*((wchar_t *)(str->_ptr))++ = (wchar_t)ch));
                } else {
                    return (wint_t) _flswbuf(ch, str);
                }
            }
            else if (_textmode_safe(_fileno(str)) == __IOINFO_TM_UTF8)
            {
                /*
                 * This is for files open for unicode writes. We need 2 chars
                 * instead of 1. Note that even if we are writing UTF8, we don't
                 * really need to worry about it here. _write will take care of
                 * proper conversion.
                 */

                char * p = (char *)&ch;

                if(_putc_nolock(*p, str) == EOF)
                    return WEOF;

                ++p;

                if(_putc_nolock(*p, str) == EOF)
                    return WEOF;


                return (wint_t)(0xffff & ch);

            }
            else if ((_osfile_safe(_fileno(str)) & FTEXT))
            {
                int size, i;
                char mbc[MB_LEN_MAX];

                /* text (multi-byte) mode */
                if (wctomb_s(&size, mbc, MB_LEN_MAX, ch) != 0)
                {
                        /*
                         * Conversion failed; errno is set by wctomb_s;
                         * we return WEOF to indicate failure.
                         */
                        return WEOF;
                }
                for ( i = 0; i < size; i++)
                {
                        if (_putc_nolock(mbc[i], str) == EOF)
                                return WEOF;
                }
                return (wint_t)(0xffff & ch);
            }
        }
        /* binary (Unicode) mode */
        if ( (str->_cnt -= sizeof(wchar_t)) >= 0 )
                return (wint_t) (0xffff & (*((wchar_t *)(str->_ptr))++ = (wchar_t)ch));
        else
                return (wint_t) _flswbuf(ch, str);
}

#undef putwc

wint_t __cdecl putwc (
        wchar_t ch,
        FILE *str
        )
{
        return fputwc(ch, str);
}

