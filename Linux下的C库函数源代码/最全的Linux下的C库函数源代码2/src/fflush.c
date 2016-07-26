/***
*fflush.c - flush a stream buffer
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fflush() - flush the buffer on a stream
*               _flushall() - flush all stream buffers
*
*******************************************************************************/

#include <sect_attribs.h>
#include <cruntime.h>
#include <stdio.h>
#include <file2.h>
#include <io.h>
#include <mtdll.h>
#include <internal.h>


/* Values passed to flsall() to distinguish between _flushall() and
 * fflush(NULL) behavior
 */
#define FLUSHALL        1
#define FFLUSHNULL      0

/* Core routine for fflush(NULL) and flushall()
 */
static int __cdecl flsall(int);


/***
*int fflush(stream) - flush the buffer on a stream
*
*Purpose:
*       if file open for writing and buffered, flush the buffer. if problems
*       flushing the buffer, set the stream flag to error
*       Always flushes the stdio stream and forces a commit to disk if file
*       was opened in commit mode.
*
*Entry:
*       FILE *stream - stream to flush
*
*Exit:
*       returns 0 if flushed successfully, or no buffer to flush
*       returns EOF and sets file error flag if fails.
*       FILE struct entries affected: _ptr, _cnt, _flag.
*
*Exceptions:
*
*******************************************************************************/


int __cdecl fflush (
        REG1 FILE *stream
        )
{
        int rc;

        /* if stream is NULL, flush all streams
         */
        if ( stream == NULL )
                return(flsall(FFLUSHNULL));

        _lock_str(stream);

        __try {
                rc = _fflush_nolock(stream);
        }
        __finally {
                _unlock_str(stream);
        }

        return(rc);
}


/***
*_fflush_nolock() - Flush the buffer on a stream (stream is already locked)
*
*Purpose:
*       Core flush routine; assumes stream lock is held by caller.
*
*       [See fflush() above for more information.]
*
*Entry:
*       [See fflush()]
*Exit:
*       [See fflush()]
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _fflush_nolock (
        REG1 FILE *str
        )
{

        /* if stream is NULL, flush all streams
         */
        if ( str == NULL )
                return(flsall(FFLUSHNULL));


        if (_flush(str) != 0) {
                /* _flush failed, don't attempt to commit */
                return(EOF);
        }

        /* lowio commit to ensure data is written to disk */
        if (str->_flag & _IOCOMMIT) {
                return (_commit(_fileno(str)) ? EOF : 0);
        }
        return 0;
}


/***
*int _flush(stream) - flush the buffer on a single stream
*
*Purpose:
*       If file open for writing and buffered, flush the buffer.  If
*       problems flushing the buffer, set the stream flag to error.
*       Multi-thread version assumes stream lock is held by caller.
*
*Entry:
*       FILE* stream - stream to flush
*
*Exit:
*       Returns 0 if flushed successfully, or if no buffer to flush.,
*       Returns EOF and sets file error flag if fails.
*       File struct entries affected: _ptr, _cnt, _flag.
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _flush (
        FILE *str
        )
{
        REG1 FILE *stream;
        REG2 int rc = 0; /* assume good return */
        REG3 int nchar;

        /* Init pointer to stream */
        stream = str;


        if ((stream->_flag & (_IOREAD | _IOWRT)) == _IOWRT && bigbuf(stream)
                && (nchar = (int)(stream->_ptr - stream->_base)) > 0)
        {
                if ( _write(_fileno(stream), stream->_base, nchar) == nchar ) {
                        /* if this is a read/write file, clear _IOWRT so that
                         * next operation can be a read
                         */
                        if ( _IORW & stream->_flag )
                                stream->_flag &= ~_IOWRT;
                }
                else {
                        stream->_flag |= _IOERR;
                        rc = EOF;
                }
        }

        stream->_ptr = stream->_base;
        stream->_cnt = 0;

        return(rc);
}


/***
*int _flushall() - flush all output buffers
*
*Purpose:
*       flushes all the output buffers to the file, clears all input buffers.
*
*Entry:
*       None.
*
*Exit:
*       returns number of open streams
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _flushall (
        void
        )
{
        return(flsall(FLUSHALL));
}


/***
*static int flsall(flushflag) - flush all output buffers
*
*Purpose:
*       Flushes all the output buffers to the file and, if FLUSHALL is passed,
*       clears all input buffers. Core routine for both fflush(NULL) and
*       flushall().
*
*       MTHREAD Note: All the locking/unlocking required for both fflush(NULL)
*       and flushall() is performed in this routine.
*
*Entry:
*       int flushflag - flag indicating the exact semantics, there are two
*                       legal values: FLUSHALL and FFLUSHNULL
*
*Exit:
*       if flushflag == FFLUSHNULL then flsbuf returns:
                0, if successful
*               EOF, if an error occurs while flushing one of the streams
*
*       if flushflag == FLUSHALL then flsbuf returns the number of streams
*       successfully flushed
*
*Exceptions:
*
*******************************************************************************/

static int __cdecl flsall (
        int flushflag
        )
{
        REG1 int i;
        int count = 0;
        int err = 0;

        _mlock(_IOB_SCAN_LOCK);
        __try {

        for ( i = 0 ; i < _nstream ; i++ ) {

                if ( (__piob[i] != NULL) && (inuse((FILE *)__piob[i])) ) {

                        /*
                         * lock the stream. this is not done until testing
                         * the stream is in use to avoid unnecessarily creating
                         * a lock for every stream. the price is having to
                         * retest the stream after the lock has been asserted.
                         */
                        _lock_str2(i, __piob[i]);

                        __try {
                                /*
                                 * if the stream is STILL in use (it may have been
                                 * closed before the lock was asserted), see about
                                 * flushing it.
                                 */
                                if ( inuse((FILE *)__piob[i]) ) {

                        if ( flushflag == FLUSHALL ) {
                                /*
                                 * FLUSHALL functionality: fflush the read or
                                 * write stream and, if successful, update the
                                 * count of flushed streams
                                 */
                                if ( _fflush_nolock(__piob[i]) != EOF )
                                        /* update count of successfully flushed
                                         * streams
                                         */
                                        count++;
                        }
                        else if ( (flushflag == FFLUSHNULL) &&
                                  (((FILE *)__piob[i])->_flag & _IOWRT) ) {
                                /*
                                 * FFLUSHNULL functionality: fflush the write
                                 * stream and kept track of the error, if one
                                 * occurs
                                 */
                                if ( _fflush_nolock(__piob[i]) == EOF )
                                        err = EOF;
                        }

                                }
                        }
                        __finally {
                                _unlock_str2(i, __piob[i]);
                        }
                }
        }

        }
        __finally {
                _munlock(_IOB_SCAN_LOCK);
        }

        if ( flushflag == FLUSHALL )
                return(count);
        else
                return(err);
}
