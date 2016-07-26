/***
*pipe.c - create a pipe
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _pipe() - creates a pipe (i.e., an I/O channel for interprocess
*                         communication)
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
#include <fcntl.h>

/***
*int _pipe(phandles, psize, textmode) - open a pipe
*
*Purpose:
*       Checks if the given handle is associated with a character device
*       (terminal, console, printer, serial port)
*
*       Multi-thread notes: No locking is performed or deemed necessary. The
*       handles returned by DOSCREATEPIPE are newly opened and, therefore,
*       should not be referenced by any thread until after the _pipe call is
*       complete. The function is not protected from some thread of the caller
*       doing, say, output to a previously invalid handle that becomes one of
*       the pipe handles. However, any such program is doomed anyway and
*       protecting the _pipe function such a case would be of little value.
*
*Entry:
*       int phandle[2] - array to hold returned read (phandle[0]) and write
*                        (phandle[1]) handles
*
*       unsigned psize - amount of memory, in bytes, to ask o.s. to reserve
*                        for the pipe
*
*       int textmode   - _O_TEXT, _O_BINARY, _O_NOINHERIT, or 0 (use default)
*
*Exit:
*       returns 0 if successful
*       returns -1 if an error occurs in which case, errno is set to:
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _pipe (
        int phandles[2],
        unsigned psize,
        int textmode
        )
{
        ULONG dosretval;                    /* o.s. return value */
        int handle0, handle1;
        int toomanyfiles = 0;

        HANDLE ReadHandle, WriteHandle;
        SECURITY_ATTRIBUTES SecurityAttributes;

        _VALIDATE_CLEAR_OSSERR_RETURN( (phandles != NULL), EINVAL, -1 );
        phandles[0] = phandles[1] = -1;

        _VALIDATE_CLEAR_OSSERR_RETURN(((textmode & ~(_O_NOINHERIT | _O_BINARY | _O_TEXT)) == 0), EINVAL, -1);
        _VALIDATE_CLEAR_OSSERR_RETURN((textmode & (_O_BINARY | _O_TEXT)) != (_O_BINARY | _O_TEXT), EINVAL, -1);

        SecurityAttributes.nLength = sizeof(SecurityAttributes);
        SecurityAttributes.lpSecurityDescriptor = NULL;

        if (textmode & _O_NOINHERIT) {
            SecurityAttributes.bInheritHandle = FALSE;
        }
        else {
            SecurityAttributes.bInheritHandle = TRUE;
        }

        if (!CreatePipe(&ReadHandle, &WriteHandle, &SecurityAttributes, psize)) {
            /* o.s. error */
            dosretval = GetLastError();
            _dosmaperr(dosretval);
            return -1;
        }

        /* now we must allocate C Runtime handles for Read and Write handles */

        if ((handle0 = _alloc_osfhnd()) != -1) {

            __try {

            _osfile(handle0) = (char)(FOPEN | FPIPE | FTEXT);
            _textmode(handle0) = 0;
            _tm_unicode(handle0) = 0;

            }
            __finally {
                _unlock_fh( handle0 );
            }

            if ((handle1 = _alloc_osfhnd()) != -1) {
                int fmode = 0;
                __try {

                _osfile(handle1) = (char)(FOPEN | FPIPE | FTEXT);
                _textmode(handle1) = 0;
                _tm_unicode(handle1) = 0;

                }
                __finally {
                    if ( handle1 != -1 )
                        _unlock_fh( handle1 );
                }

                _ERRCHECK(_get_fmode(&fmode));
                if ( (textmode & _O_BINARY) ||
                     (((textmode & _O_TEXT) == 0) &&
                      (fmode == _O_BINARY)) ) {
                    /* binary mode */
                    _osfile(handle0) &= ~FTEXT;
                    _osfile(handle1) &= ~FTEXT;
                }

                if ( textmode & _O_NOINHERIT ) {
                    _osfile(handle0) |= FNOINHERIT;
                    _osfile(handle1) |= FNOINHERIT;
                }

                _set_osfhnd(handle0, (intptr_t)ReadHandle);
                _set_osfhnd(handle1, (intptr_t)WriteHandle);
            }
            else {
                _osfile(handle0) = 0;
                toomanyfiles = 1;
            }
        }
        else {
            toomanyfiles = 1;
        }

        /* If error occurred, close Win32 handles and return -1 */
        if (toomanyfiles) {
            errno = EMFILE;
            CloseHandle(ReadHandle);
            CloseHandle(WriteHandle);
            _doserrno = 0;      /* not an o.s. error */
            return -1;
        }

        phandles[0] = handle0;
        phandles[1] = handle1;

        return 0;
}
