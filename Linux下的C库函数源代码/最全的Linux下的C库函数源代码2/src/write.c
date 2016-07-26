/***
*write.c - write to a file handle
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _write() - write to a file handle
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <ctype.h>
#include <io.h>
#include <errno.h>
#include <msdos.h>
#include <mtdll.h>
#include <stdlib.h>
#include <string.h>
#include <internal.h>
#include <setlocal.h>
#include <locale.h>
#include <wchar.h>

#define BUF_SIZE    1025    /* size of LF translation buffer */

#define LF '\n'      /* line feed */
#define CR '\r'      /* carriage return */
#define CTRLZ 26     /* ctrl-z */

/***
*int _write(fh, buf, cnt) - write bytes to a file handle
*
*Purpose:
*       Writes count bytes from the buffer to the handle specified.
*       If the file was opened in text mode, each LF is translated to
*       CR-LF.  This does not affect the return value.  In text
*       mode ^Z indicates end of file.
*
*       Multi-thread notes:
*       (1) _write() - Locks/unlocks file handle
*           _write_nolock() - Does NOT lock/unlock file handle
*
*Entry:
*       int fh - file handle to write to
*       char *buf - buffer to write from
*       unsigned int cnt - number of bytes to write
*
*Exit:
*       returns number of bytes actually written.
*       This may be less than cnt, for example, if out of disk space.
*       returns -1 (and set errno) if fails.
*
*Exceptions:
*
*******************************************************************************/


/* define normal version that locks/unlocks, validates fh */
int __cdecl _write (
        int fh,
        const void *buf,
        unsigned cnt
        )
{
        int r;                          /* return value */


        /* validate handle */
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);

        _lock_fh(fh);                   /* lock file */

        __try {
                if ( _osfile(fh) & FOPEN )
                        r = _write_nolock(fh, buf, cnt);    /* write bytes */
                else {
                        errno = EBADF;
                        _doserrno = 0;  /* not o.s. error */
                        r = -1;
                        _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
                }
        }
        __finally {
                _unlock_fh(fh);         /* unlock file */
        }

        return r;
}

/* now define version that doesn't lock/unlock, validate fh */
int __cdecl _write_nolock (
        int fh,
        const void *buf,
        unsigned cnt
        )
{

        int lfcount;            /* count of line feeds */
        int charcount;          /* count of chars written so far */
        int written;            /* count of chars written on this write */
        ULONG dosretval;        /* o.s. return value */
        char tmode ;            /* textmode - ANSI or UTF-16 */
        BOOL toConsole = 0;     /* true when writing to console */
        BOOL isCLocale = 0;     /* true when locale handle is C locale */


        lfcount = charcount = 0;        /* nothing written yet */

        if (cnt == 0)
                return 0;               /* nothing to do */

        _VALIDATE_CLEAR_OSSERR_RETURN( (buf != NULL), EINVAL, -1 );

        tmode = _textmode(fh);

        if(tmode == __IOINFO_TM_UTF16LE ||
                tmode == __IOINFO_TM_UTF8)
        {
            /* For a UTF-16 file, the count must always be an even number */
            _VALIDATE_CLEAR_OSSERR_RETURN(((cnt & 1) == 0), EINVAL, -1);
        }

        if (_osfile(fh) & FAPPEND) {
                /* appending - seek to end of file; ignore error, because maybe
                   file doesn't allow seeking */
#if _INTEGRAL_MAX_BITS >= 64
                (void)_lseeki64_nolock(fh, 0, FILE_END);
#else  /* _INTEGRAL_MAX_BITS >= 64 */
                (void)_lseek_nolock(fh, 0, FILE_END);
#endif  /* _INTEGRAL_MAX_BITS >= 64 */
        }

        /* check for text mode with LF's in the buffer */

        /*
         * Note that in case the handle belongs to Console, write file will
         * generate garbage output. For user to print these characters
         * correctly, we will need to print ANSI.
         *
         * Also note that in case of printing to Console, we still have to
         * convert the characters to console codepage.
         */

        if (_isatty(fh) && (_osfile(fh) & FTEXT))
        {
            DWORD dwMode;
            _ptiddata ptd = _getptd();
            isCLocale = (ptd->ptlocinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE);
            toConsole = GetConsoleMode((HANDLE)_osfhnd(fh), &dwMode);
        }

        /* don't need double conversion if it's ANSI mode C locale */
        if (toConsole && !(isCLocale && (tmode == __IOINFO_TM_ANSI))) {
            UINT consoleCP = GetConsoleCP();
            char mboutbuf[MB_LEN_MAX];
            wchar_t tmpchar;
            int size = 0;
            int written = 0;
            char *pch;

            for (pch = (char *)buf; (unsigned)(pch - (char *)buf) < cnt; ) {
                BOOL bCR;

                if (tmode == __IOINFO_TM_ANSI) {
                    bCR = *pch == LF;
                    /*
                     * Here we need to do double convert. i.e. convert from
                     * multibyte to unicode and then from unicode to multibyte in
                     * Console codepage.
                     */
                    if (!isleadbyte(*pch)) {
                        if (mbtowc(&tmpchar, pch, 1) == -1) {
                            break;
                        }
                    } else if ((cnt - (pch - (char*)buf)) > 1) {
                        if (mbtowc(&tmpchar, pch, 2) == -1) {
                            break;
                        }
                        /*
                         * Increment pch to accomodate DBCS character.
                         */
                        ++pch;
                    } else {
                        break;
                    }
                    ++pch;
                } else if (tmode == __IOINFO_TM_UTF8 || tmode == __IOINFO_TM_UTF16LE) {
                    /*
                     * Note that bCR set above is not valid in case of UNICODE
                     * stream. We need to set it using unicode character.
                     */
                    tmpchar = *(wchar_t *)pch;
                    bCR = tmpchar == LF;
                    pch += 2;
                }

                if (tmode == __IOINFO_TM_ANSI)
                {
                    if( (size = WideCharToMultiByte(consoleCP,
                                                    0,
                                                    &tmpchar,
                                                    1,
                                                    mboutbuf,
                                                    sizeof(mboutbuf),
                                                    NULL,
                                                    NULL)) == 0) {
                        break;
                    } else {
                        if ( WriteFile( (HANDLE)_osfhnd(fh),
                                        mboutbuf,
                                        size,
                                        (LPDWORD)&written,
                                        NULL) ) {
                            charcount += written;
                            if (written < size)
                                break;
                        } else {
                            dosretval = GetLastError();
                            break;
                        }
                    }

                    if (bCR) {
                        size = 1;
                        mboutbuf[0] = CR;
                        if (WriteFile((HANDLE)_osfhnd(fh),
                                      mboutbuf,
                                      size,
                                      (LPDWORD)&written,
                                      NULL) ) {
                            if (written < size)
                                break;
                            lfcount ++;
                            charcount++;
                        } else {
                            dosretval = GetLastError();
                            break;
                        }
                    }
                }
                else if ( tmode == __IOINFO_TM_UTF8 || tmode == __IOINFO_TM_UTF16LE)
                {
                    if ( _putwch_nolock(tmpchar) == tmpchar )
                    {
                        charcount++;
                    }
                    else
                    {
                        dosretval = GetLastError();
                        break;
                    }
                    if (bCR) /* emit carriage return */
                    {
                        size = 1;
                        tmpchar = CR;
                        if ( _putwch_nolock(tmpchar) == tmpchar )
                        {
                            charcount++;
                            lfcount++;
                        }
                        else
                        {
                            dosretval = GetLastError();
                            break;
                        }
                    }
                }
            }
        } else if ( _osfile(fh) & FTEXT ) {
            /* text mode, translate LF's to CR/LF's on output */

            dosretval = 0;          /* no OS error yet */

            if(tmode == __IOINFO_TM_ANSI) {
                char ch;                    /* current character */
                char *p = NULL, *q = NULL;  /* pointers into buf and lfbuf resp. */
                char lfbuf[BUF_SIZE];
                p = (char *)buf;        /* start at beginning of buffer */
                while ( (unsigned)(p - (char *)buf) < cnt ) {
                    q = lfbuf;      /* start at beginning of lfbuf */

                    /* fill the lf buf, except maybe last char */
                    while ( q - lfbuf < sizeof(lfbuf) - 1 &&
                            (unsigned)(p - (char *)buf) < cnt ) {
                        ch = *p++;
                        if ( ch == LF ) {
                            ++lfcount;
                            *q++ = CR;
                        }
                        *q++ = ch;
                    }

                    /* write the lf buf and update total */
                    if ( WriteFile( (HANDLE)_osfhnd(fh),
                                lfbuf,
                                (int)(q - lfbuf),
                                (LPDWORD)&written,
                                NULL) )
                    {
                        charcount += written;
                        if (written < q - lfbuf)
                            break;
                    }
                    else {
                        dosretval = GetLastError();
                        break;
                    }
                }
            } else if ( tmode == __IOINFO_TM_UTF16LE ){
                char lfbuf[BUF_SIZE];
                wchar_t wch;            /* current wide char */
                wchar_t *pu = (wchar_t *)buf;
                wchar_t *qu = NULL;

                while ( (unsigned)((char *)pu - (char *)buf) < cnt ) {
                    qu = (wchar_t *)lfbuf; /* start at beginning of lfbuf */

                    /* fill the lf buf, except maybe last wchar_t */
                    while ( (((char *)qu - lfbuf) < (sizeof(lfbuf) - 2)) &&
                            ((unsigned)((char *)pu - (char *)buf) < cnt )) {
                        wch = *pu++;
                        if ( wch == LF ) {
                            lfcount+=2;
                            *qu++ = CR;
                        }
                        *qu++ = wch;
                    }

                    /* write the lf buf and update total */
                    if ( WriteFile( (HANDLE)_osfhnd(fh),
                                lfbuf,
                                (int)((char*)qu - lfbuf),
                                (LPDWORD)&written,
                                NULL) )
                    {
                        charcount += written;
                        if (written < ((char *)qu - lfbuf))
                            break;
                    }
                    else {
                        dosretval = GetLastError();
                        break;
                    }
                }
            } else {
                /*
                 * Let's divide the lfbuf in 1:2 wher 1 is for storing
                 * widecharacters and 2 if for converting it to UTF8.  This takes
                 * into account the worst case scenario where all the UTF8
                 * characters are 4 byte long.
                 */
                char utf8_buf[(BUF_SIZE*2)/3];
                wchar_t utf16_buf[BUF_SIZE/6];

                wchar_t wch;            /* current wide char */
                wchar_t *pu = (wchar_t *)buf;
                wchar_t *qu = NULL;

                pu = (wchar_t *)buf;
                while ((unsigned)((char *)pu - (char *)buf) < cnt) {
                    int bytes_converted = 0;
                    qu = utf16_buf; /* start at beginning of lfbuf */

                    while ( (((char *)qu - (char *)utf16_buf) <
                             (sizeof(utf16_buf) - 2)) &&
                            ((unsigned)((char *)pu - (char *)buf) < cnt )) {
                        wch = *pu++;
                        if ( wch == LF ) {
                            /* no need to count the linefeeds here: we calculate the written chars in another way */
                            *qu++ = CR;
                        }
                        *qu++ = wch;
                    }

                    bytes_converted = WideCharToMultiByte(
                            CP_UTF8,
                            0,
                            utf16_buf,
                            ((int)((char *)qu - (char *)utf16_buf))/2,
                            utf8_buf,
                            sizeof(utf8_buf),
                            NULL,
                            NULL);

                    if (bytes_converted == 0) {
                        dosretval = GetLastError();
                        break;
                    } else {
                        /*
                         * Here we need to make every attempt to write all the
                         * converted characters. The resaon behind this is,
                         * incase half the bytes of a UTF8 character is
                         * written, it may currupt whole of the stream or file.
                         *
                         * The loop below will make sure we exit only if all
                         * the bytes converted are written (which makes sure no
                         * partial MBCS is written) or there was some error in
                         * the stream.
                         */
                        int bytes_written = 0;
                        do {
                            if (WriteFile(
                                        (HANDLE)_osfhnd(fh),
                                        utf8_buf + bytes_written,
                                        bytes_converted - bytes_written,
                                        &written,
                                        NULL)) {
                                bytes_written += written;
                            } else {
                                dosretval = GetLastError();
                                break;
                            }
                        } while ( bytes_converted > bytes_written);

                        /*
                         * Only way the condition below could be true is if
                         * there was en error. In case of error we need to
                         * break this loop as well.
                         */
                        if (bytes_converted > bytes_written) {
                            break;
                        }
                        /* if this chunk has been committed successfully, update charcount */
                        charcount = (int)((char *)pu - (char *)buf);
                    }
                }
            }
        }
        else {
                /* binary mode, no translation */
                if ( WriteFile( (HANDLE)_osfhnd(fh),
                                (LPVOID)buf,
                                cnt,
                               (LPDWORD)&written,
                                NULL) )
                {
                        dosretval = 0;
                        charcount = written;
                }
                else
                        dosretval = GetLastError();
        }

        if (charcount == 0) {
                /* If nothing was written, first check if an o.s. error,
                   otherwise we return -1 and set errno to ENOSPC,
                   unless a device and first char was CTRL-Z */
                if (dosretval != 0) {
                        /* o.s. error happened, map error */
                        if (dosretval == ERROR_ACCESS_DENIED) {
                            /* wrong read/write mode should return EBADF, not
                               EACCES */
                                errno = EBADF;
                                _doserrno = dosretval;
                        }
                        else
                                _dosmaperr(dosretval);
                        return -1;
                }
                else if ((_osfile(fh) & FDEV) && *(char *)buf == CTRLZ)
                        return 0;
                else {
                        errno = ENOSPC;
                        _doserrno = 0;  /* no o.s. error */
                        return -1;
                }
        }
        else
                /* return adjusted bytes written */
                return charcount - lfcount;
}
