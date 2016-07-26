/***
*read.c - read from a file handle
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _read() - read from a file handle
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <mtdll.h>
#include <io.h>
#include <internal.h>
#include <stdlib.h>
#include <errno.h>
#include <msdos.h>
#include <dbgint.h>

#define LF 10           /* line feed */
#define CR 13           /* carriage return */
#define CTRLZ 26        /* ctrl-z means eof for text */


/*
    This lookup table returns
    1 for pattern 110xxxxx - 1 trailbyte
    2 for pattern 1110xxxx - 2 trailbytes
    3 for pattern 11110xxx - 3 trailbytes
    0 for everything else (including invalid patterns)
    We return 0 for invalid patterns also because we
    rely on MultiByteToWideChar to do the validations
*/

char _lookuptrailbytes[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Most significant Bit */
#define _msbit(c) ((c) & 0x80)

/* Independent byte has most significant bit set to 0 */
#define  _utf8_is_independent(c)    (_msbit(c) == 0)

/* Any leadbyte will have the patterns 11000xxx 11100xxx or 11110xxx */
#define  _utf8_is_leadbyte(c)       (_lookuptrailbytes[(unsigned char)c] != 0)

/* Get no of trailing bytes from the lookup table */
#define  _utf8_no_of_trailbytes(c)  _lookuptrailbytes[(unsigned char)c]

/***
*int _read(fh, buf, cnt) - read bytes from a file handle
*
*Purpose:
*       Attempts to read cnt bytes from fh into a buffer.
*       If the file is in text mode, CR-LF's are mapped to LF's, thus
*       affecting the number of characters read.  This does not
*       affect the file pointer.
*
*       NOTE:  The stdio _IOCTRLZ flag is tied to the use of FEOFLAG.
*       Cross-reference the two symbols before changing FEOFLAG's use.
*
*Entry:
*       int fh - file handle to read from
*       char *buf - buffer to read into
*       int cnt - number of bytes to read
*
*Exit:
*       Returns number of bytes read (may be less than the number requested
*       if the EOF was reached or the file is in text mode).
*       returns -1 (and sets errno) if fails.
*
*Exceptions:
*
*******************************************************************************/


/* define normal version that locks/unlocks, validates fh */
int __cdecl _read (
        int fh,
        void *buf,
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
                r = _read_nolock(fh, buf, cnt); /* read bytes */
            else {
                errno = EBADF;
                _doserrno = 0;
                r = -1;
                _ASSERTE(("Invalid file descriptor. File possibly closed by a different thread",0));
            }
        }
        __finally {
            _unlock_fh(fh);             /* unlock file */
        }

        return r;
}

/* now define version that doesn't lock/unlock, validate fh */
int __cdecl _read_nolock (
        int fh,
        void *inputbuf,
        unsigned cnt
        )
{

        int bytes_read;                 /* number of bytes read */
        char *buffer;                   /* buffer to read to */
        int os_read;                    /* bytes read on OS call */
        char *p, *q;                    /* pointers into buffer */
        wchar_t *pu, *qu;               /* wchar_t pointers into buffer for UTF16 */
        char peekchr;                   /* peek-ahead character */
        wchar_t wpeekchr;               /* peek-ahead wchar_t */
        __int64 filepos;                /* file position after seek */
        ULONG dosretval;                /* o.s. return value */
        char tmode;                         /* textmode - ANSI/UTF-8/UTF-16 */
        void *buf;                          /* buffer to read to */
        int retval = -2;                    /* return value */
        unsigned inputsize = cnt;

        /* validate fh */
        _CHECK_FH_CLEAR_OSSERR_RETURN( fh, EBADF, -1 );
        _VALIDATE_CLEAR_OSSERR_RETURN((fh >= 0 && (unsigned)fh < (unsigned)_nhandle), EBADF, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);


        bytes_read = 0;                 /* nothing read yet */

        if (cnt == 0 || (_osfile(fh) & FEOFLAG)) {
            /* nothing to read or at EOF, so return 0 read */
            return 0;
        }

        _VALIDATE_CLEAR_OSSERR_RETURN( (inputbuf != NULL), EINVAL, -1 );

        tmode = _textmode(fh);

        switch(tmode) {
            case __IOINFO_TM_UTF8 :
                /* For a UTF-8 file, we need 2 buffers, because after reading we
                   need to convert it into UNICODE - MultiByteToWideChar doesn't do
                   in-place conversions. */

                /* MultiByte To WideChar conversion may double the size of the
                   buffer required & hence we divide cnt by 2 */

                /*
                 * Since we are reading UTF8 stream, cnt bytes read may vary
                 * from cnt wchar_t characters to cnt/4 wchar_t characters. For
                 * this reason if we need to read cnt characters, we will
                 * allocate MBCS buffer of cnt. In case cnt is 0, we will
                 * have 4 as minimum value. This will make sure we don't
                 * overflow for reading from pipe case.
                 *
                 *
                 * In this case the numbers of wchar_t characters that we can
                 * read is cnt/2. This means that the buffer size that we will
                 * require is cnt/2.
                 */

                /* For UTF8 we want the count to be an even number */
                _VALIDATE_CLEAR_OSSERR_RETURN(((cnt & 1) == 0), EINVAL, -1);

                cnt = (cnt/2) < 4 ? 4 : (cnt/2);

                buf = _malloc_crt(cnt);

                if(!buf) {
                    errno = ENOMEM;
                    _doserrno = E_nomem;
                    return -1;
                }
                break;

            case __IOINFO_TM_UTF16LE :
                /* For UTF16 the count always needs to be an even number */
                _VALIDATE_CLEAR_OSSERR_RETURN(((cnt & 1) == 0), EINVAL, -1);

                cnt &= (~1);

                /* Fall Through to default */

            default :
                /* For non-UTF8 files, we need only 1 buffer - make buf point to
                   the users input buffer */
                buf = inputbuf;
        }


        buffer = buf;

        if ((_osfile(fh) & (FPIPE|FDEV)) && _pipech(fh) != LF && cnt != 0) {
            /* a pipe/device and pipe lookahead non-empty: read the lookahead
             * char */
            *buffer++ = _pipech(fh);
            ++bytes_read;
            --cnt;
            _pipech(fh) = LF;           /* mark as empty */

            /* For UTF16, there maybe one more look ahead char. For UTF8,
               there maybe 2 more look ahead chars */
            if((tmode != __IOINFO_TM_ANSI) && (_pipech2(fh)[0] != LF) && cnt != 0) {
                *buffer++ = _pipech2(fh)[0];
                ++bytes_read;
                --cnt;
                _pipech2(fh)[0] = LF;   /* mark as empty */

                if((tmode == __IOINFO_TM_UTF8) && (_pipech2(fh)[1] != LF) && cnt != 0) {
                    *buffer++ = _pipech2(fh)[1];
                    ++bytes_read;
                    --cnt;
                    _pipech2(fh)[1] = LF;   /* mark as empty */
                }

            }

        }

        /* read the data */

        if ( !ReadFile( (HANDLE)_osfhnd(fh), buffer, cnt, (LPDWORD)&os_read,
                    NULL ) || os_read < 0 || (size_t)os_read > cnt)
        {
            /* ReadFile has reported an error. recognize two special cases.
             *
             *      1. map ERROR_ACCESS_DENIED to EBADF
             *
             *      2. just return 0 if ERROR_BROKEN_PIPE has occurred. it
             *         means the handle is a read-handle on a pipe for which
             *         all write-handles have been closed and all data has been
             *         read. */

            if ( (dosretval = GetLastError()) == ERROR_ACCESS_DENIED ) {
                /* wrong read/write mode should return EBADF, not EACCES */
                errno = EBADF;
                _doserrno = dosretval;
                retval = -1;
                goto error_return;

            }
            else if ( dosretval == ERROR_BROKEN_PIPE ) {
                retval = 0;
                goto error_return;
            }
            else {
                _dosmaperr(dosretval);
                retval = -1;
                goto error_return;
            }
        }

        bytes_read += os_read;          /* update bytes read */

        if (_osfile(fh) & FTEXT) {
            /* now must translate CR-LFs to LFs in the buffer */

            /* For ANSI & UTF8, we read byte by byte.
               For UTF16, we need to read 2 bytes (wchar_t's) at a time */
            if(tmode != __IOINFO_TM_UTF16LE) {
                /* set CRLF flag to indicate LF at beginning of buffer */
                if ( (os_read != 0) && (*(char *)buf == LF) )
                    _osfile(fh) |= FCRLF;
                else
                    _osfile(fh) &= ~FCRLF;

                /* convert chars in the buffer: p is src, q is dest */
                p = q = buf;
                while (p < (char *)buf + bytes_read) {
                    if (*p == CTRLZ) {
                        /* if fh is not a device, set ctrl-z flag */
                        if ( !(_osfile(fh) & FDEV) )
                            _osfile(fh) |= FEOFLAG;
                        else
                            *q++ = *p++;
                        break;              /* stop translating */
                    }
                    else if (*p != CR)
                        *q++ = *p++;
                    else {
                        /* *p is CR, so must check next char for LF */
                        if (p < (char *)buf + bytes_read - 1) {
                            if (*(p+1) == LF) {
                                p += 2;
                                *q++ = LF;  /* convert CR-LF to LF */
                            }
                            else
                                *q++ = *p++;    /* store char normally */
                        }
                        else {
                            /* This is the hard part.  We found a CR at end of
                               buffer.  We must peek ahead to see if next char
                               is an LF. */
                            ++p;

                            dosretval = 0;
                            if ( !ReadFile( (HANDLE)_osfhnd(fh), &peekchr, 1,
                                        (LPDWORD)&os_read, NULL ) )
                                dosretval = GetLastError();

                            if (dosretval != 0 || os_read == 0) {
                                /* couldn't read ahead, store CR */
                                *q++ = CR;
                            }
                            else {
                                /*
                                 * peekchr now has the extra character -- we now
                                 * have several possibilities:
                                 *
                                 * 1. disk file and char is not LF; just seek
                                 *    back and copy CR
                                 * 2. disk file and char is LF; seek back and
                                 *    discard CR
                                 * 3. disk file, char is LF but this is a
                                 *    one-byte read: store LF, don't seek back
                                 * 4. pipe/device and char is LF; store LF.
                                 * 5. pipe/device and char isn't LF, store CR
                                 *    and put char in pipe lookahead buffer.
                                 */
                                if (_osfile(fh) & (FDEV|FPIPE)) {
                                    /* non-seekable device */
                                    if (peekchr == LF)
                                        *q++ = LF;
                                    else {
                                        *q++ = CR;
                                        _pipech(fh) = peekchr;
                                    }
                                }
                                else {
                                    /* disk file */
                                    if (q == buf && peekchr == LF) {
                                        /* nothing read yet; must make some
                                           progress */
                                        *q++ = LF;
                                    }
                                    else {
                                        /* seek back */
                                        filepos = _lseeki64_nolock(fh, -1i64, FILE_CURRENT);
                                        if (peekchr != LF)
                                            *q++ = CR;
                                    }
                                }
                            }
                        }
                    }
                }


                /* we now change bytes_read to reflect the true number of chars
                   in the buffer */
                bytes_read = (int)(q - (char *)buf);

                if((tmode == __IOINFO_TM_UTF8) && (bytes_read != 0)) {
                    /* UTF8 reads need to be converted into UTF16 */

                    --q; /* q has gone beyond the last char */

                    /*
                     * If the last byte is a standalone UTF-8 char. We then
                     * take the whole buffer. Otherwise we skip back till we
                     * come to a lead byte. If the leadbyte forms a complete
                     * UTF-8 character will the remaining part of the buffer,
                     * then again we take the whole buffer. If not, we skip to
                     * one byte which should be the final trail byte of the
                     * previous UTF-8 char or a standalone UTF-8 character
                     */

                    if(_utf8_is_independent(*q)) {
                        ++q;
                        /*
                         * Final byte is standalone, we reset q, because we
                         * will now consider the full buffer which we have read
                         */
                    }
                    else {
                        int ctr = 1;
                        int cnt_trailbytes;

                        while(!_utf8_is_leadbyte(*q) && ctr <= 4 && q >= (char *)buf) {
                            --q;
                            ++ctr;
                        }

                        cnt_trailbytes = _utf8_no_of_trailbytes(*q);

                        if(cnt_trailbytes == 0) {
                            /*
                             * Should have exited the while by finding a lead
                             * byte else, the file has incorrect UTF-8 chars
                             */
                                errno = EILSEQ;
                                retval = -1;
                                goto error_return;
                            }

                        if(cnt_trailbytes + 1 == ctr) {
                            /*
                             * The leadbyte + the remaining bytes form a full
                             * set
                             */
                            q += ctr;
                        }
                        else {
                            /* Seek back */

                            if (_osfile(fh) & (FDEV|FPIPE)) {
                                /*
                                 * non-seekable device. Put the extra chars in
                                 * _pipech & _pipech2. We would have a maximum
                                 * of 3 extra chars
                                 */
                                _pipech(fh) = *q;
                                ++q;

                                if(ctr >= 2) {
                                    _pipech2(fh)[0] = *q;
                                    ++q;
                                }
                                if(ctr == 3) {
                                    _pipech2(fh)[1] = *q;
                                    ++q;
                                }

                                /*
                                 * We need to point q back to beyond whatever
                                 * we actually took in.
                                 */
                                q -= ctr;

                            }
                            else {
                                /* We have read extra chars, so we seek back */
                                filepos = _lseeki64_nolock(fh, -ctr, FILE_CURRENT);

                        }

                        }
                    }

                    bytes_read = (int)(q - (char *)buf);
                    bytes_read = MultiByteToWideChar(
                            CP_UTF8,
                            0,
                            buf,
                            bytes_read,
                            inputbuf,
                            inputsize/2);

                    if(!bytes_read) {
                        _dosmaperr(GetLastError());
                        retval = -1;
                        goto error_return;
                    }

                    /* MultiByteToWideChar returns no of wchar_t's. Double it */
                    bytes_read = bytes_read*2;
                }
            }
            else {
                /* set CRLF flag to indicate LF at beginning of buffer */
                if ( (os_read != 0) && (*(wchar_t *)buf == LF) )
                    _osfile(fh) |= FCRLF;
                else
                    _osfile(fh) &= ~FCRLF;

                /* convert chars in the buffer: pu is src, qu is dest */
                pu = qu = (wchar_t *)buf;
                while ((char *)pu < (char *)buf + bytes_read) {
                    if (*pu == CTRLZ) {
                        /* if fh is not a device, set ctrl-z flag */
                        if ( !(_osfile(fh) & FDEV) )
                            _osfile(fh) |= FEOFLAG;
                        else
                            *qu++ = *pu++;
                        break;              /* stop translating */
                    }
                    else if (*pu != CR)
                        *qu++ = *pu++;
                    else {
                        /* *pu is CR, so must check next wchar_t for LF */
                        if ((char *)pu < (char *)buf + bytes_read - 2) {
                            if (*(pu+1) == LF) {
                                pu += 2;
                                *qu++ = LF;  /* convert CR-LF to LF */
                            }
                            else
                                *qu++ = *pu++;    /* store char normally */
                        }
                        else {
                            /* This is the hard part.  We found a CR at end of
                               buffer.  We must peek ahead to see if next
                               wchar_t is an LF. */
                            ++pu;

                            dosretval = 0;
                            if ( !ReadFile( (HANDLE)_osfhnd(fh), &wpeekchr, 2,
                                        (LPDWORD)&os_read, NULL ) )
                                dosretval = GetLastError();

                            if (dosretval != 0 || os_read == 0) {
                                /* couldn't read ahead, store CR */
                                *qu++ = CR;
                            }
                            else {
                                /*
                                 * peekchr now has the extra character -- we
                                 * now have several possibilities:
                                 * 1. wchar_t is not LF; just seek back and
                                 * copy CR
                                 * 2. wchar_t is LF; seek back and discard CR
                                 * 3. disk file, wchar_t is LF but this is a
                                 * one-byte read: store LF, don't seek back.
                                 */

                                if (_osfile(fh) & (FDEV|FPIPE)) {
                                    /* non-seekable device */
                                    if (wpeekchr == LF)
                                        *qu++ = LF;
                                    else {
                                        char * pwpeekchr = (char *)&wpeekchr;
                                        *qu++ = CR;
                                        _pipech(fh) = *pwpeekchr;
                                        ++pwpeekchr;
                                        _pipech2(fh)[0] = *pwpeekchr;
                                        _pipech2(fh)[1] = LF; /* Mark as empty */
                                    }
                                }
                                else {
                                    if ((char *)qu == buf && wpeekchr == LF) {
                                        /* nothing read yet; must make some
                                           progress */
                                        *qu++ = LF;
                                    }
                                    else {
                                        /* seek back */
                                        filepos = _lseeki64_nolock(fh, -2, FILE_CURRENT);
                                        if (wpeekchr != LF)
                                            *qu++ = CR;
                                    }
                                }
                            }
                        }
                    }
                }

                /* we now change bytes_read to reflect the true number of chars
                   in the buffer */
                bytes_read = (int)((char *)qu - (char *)buf);

            }

        }

error_return:
        if(buf != inputbuf) {
            free(buf);
        }

        return (retval == -2) ? bytes_read : retval ;

}
