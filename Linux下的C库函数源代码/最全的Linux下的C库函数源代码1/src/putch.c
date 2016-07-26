/***
*putch.c - contains the _putch() routine
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       The routine "_putch()" writes a single character to the console.
*
*       NOTE: In real-mode MS-DOS the character is actually written to standard
*       output, and is therefore redirected when standard output is redirected.
*       However, under Win32 console mode, the character is ALWAYS written
*       to the console, even when standard output has been redirected.
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <conio.h>
#include <internal.h>
#include <mtdll.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/*
 * declaration for console handle
 */
extern intptr_t _confh;

/***
*int _putch(c) - write a character to the console
*
*Purpose:
*       Calls WriteConsole to output the character
*       Note: in Win32 console mode always writes to console even
*       when stdout redirected
*
*Entry:
*       c - Character to be output
*
*Exit:
*       If an error is returned from WriteConsole
*           Then returns EOF
*       Otherwise
*           returns character that was output
*
*Exceptions:
*
*******************************************************************************/

/* normal version lock and unlock the console, and then call the _lk version
   which directly accesses the console without locking. */

int __cdecl _putch (
        int c
        )
{
        int ch;

        _mlock(_CONIO_LOCK);            /* secure the console lock */
        __TRY
            ch = _putch_nolock(c);              /* output the character */
        __FINALLY
            _munlock(_CONIO_LOCK);          /* release the console lock */
        __END_TRY_FINALLY

        return ch;
}

/* define version which accesses the console directly - normal version in
   non-_MT situations, special _lk version in _MT */

int __cdecl _putch_nolock (
        int c
        )
{
        _ptiddata ptd = _getptd();
        unsigned char *ch_buf = ptd->_con_ch_buf;
        unsigned short *pch_buf_used = &(ptd->_ch_buf_used);

        /* can't use ch directly unless sure we have a big-endian machine */
        unsigned char ch = (unsigned char)c;
        wchar_t wchar;

        /*
         * Why are we using putwch to write to Console when we could have
         * written straight away to Console? The problem we have in writing to
         * Console is that CRT codepage is different from Console codepage and
         * thus to write to console, we will need to convert the codepage. Here
         * we can use unicode version of these routines and this way we will
         * only have to do one conversion and rest will be handled by putwch.
         */

        /*
         * The usual way people call putch is character by character. Also
         * there is noway we can convert partial MBCS to unicode character. To
         * address this issue, we buffer all the lead bytes and combine them
         * with trail bytes and then do the conversion.
         */
        if (*pch_buf_used == 1)
        {
            _ASSERTE(isleadbyte(ch_buf[0]) != 0);

            ch_buf[1] = ch;
        }
        else
        {
            ch_buf[0] = ch;
        }

        if (*pch_buf_used == 0 && isleadbyte(ch_buf[0]))
        {
            /*
             * We still need trail byte, wait for it.
             */
            *pch_buf_used = 1;
        }
        else
        {
            if (mbtowc(&wchar, ch_buf, (*pch_buf_used)+1) == -1 ||
                    _putwch_nolock(wchar) == WEOF)
            {
                ch = EOF;
            }
            /*
             * Since we have processed full MBCS character, we should reset ch_buf_used.
             */
            (*pch_buf_used) = 0;
        }

        return ch;
}
