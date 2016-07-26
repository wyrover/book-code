/***
*ioinit.c - Initialization for lowio functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains initialization and termination routines for lowio.
*       Currently, this includes:
*           1. Initial allocation of array(s) of ioinfo structs.
*           2. Processing of inherited file info from parent process.
*           3. Special case initialization of the first three ioinfo structs,
*              the ones that correspond to handles 0, 1 and 2.
*
*******************************************************************************/

#include <cruntime.h>
#include <windows.h>
#include <internal.h>
#include <malloc.h>
#include <msdos.h>
#include <rterr.h>
#include <stddef.h>
#include <stdlib.h>
#include <dbgint.h>

/*
 * Special static ioinfo structure. This is referred to only by the
 * _pioinfo_safe() macro, and its derivatives, in internal.h. These, in turn
 * are used in certain stdio-level functions to more gracefully handle a FILE
 * with -1 in the _file field.
 */
_CRTIMP ioinfo __badioinfo = {
        (intptr_t)(-1), /* osfhnd */
        (char)FTEXT,          /* osfile */
        10,             /* pipech */
        0               /* lockinitflag */
        };

/*
 * Number of ioinfo structs allocated at any given time. This number ranges
 * from a minimum of IOINFO_ARRAY_ELTS to a maximum of _NHANDLE_ (==
 * IOINFO_ARRAY_ELTS * IOINFO_ARRAYS) in steps of IOINFO_ARRAY_ELTS.
 */
int _nhandle;

/*
 * Array of pointers to arrays of ioinfo structs.
 */
_CRTIMP ioinfo * __pioinfo[IOINFO_ARRAYS];

/*
 * macro used to map 0, 1 and 2 to right value for call to GetStdHandle
 */
#define stdhndl(fh)  ( (fh == 0) ? STD_INPUT_HANDLE : ((fh == 1) ? \
                       STD_OUTPUT_HANDLE : STD_ERROR_HANDLE) )

/***
*_ioinit() -
*
*Purpose:
*       Allocates and initializes initial array(s) of ioinfo structs. Then,
*       obtains and processes information on inherited file handles from the
*       parent process (e.g., cmd.exe).
*
*       Obtains the StartupInfo structure from the OS. The inherited file
*       handle information is pointed to by the lpReserved2 field. The format
*       of the information is as follows:
*
*           bytes 0 thru 3          - integer value, say N, which is the
*                                     number of handles information is passed
*                                     about
*
*           bytes 4 thru N+3        - the N values for osfile
*
*           bytes N+4 thru 5*N+3    - N double-words, the N OS HANDLE values
*                                     being passed
*
*       Next, osfhnd and osfile for the first three ioinfo structs,
*       corrsponding to handles 0, 1 and 2, are initialized as follows:
*
*           If the value in osfhnd is INVALID_HANDLE_VALUE, then try to
*           obtain a HANDLE by calling GetStdHandle, and call GetFileType to
*           help set osfile. Otherwise, assume _osfhndl and _osfile are
*           valid, but force it to text mode (standard input/output/error
*           are to always start out in text mode).
*
*       Notes:
*           1. In general, not all of the passed info from the parent process
*              will describe open handles! If, for example, only C handle 1
*              (STDOUT) and C handle 6 are open in the parent, info for C
*              handles 0 thru 6 is passed to the the child.
*
*           2. Care is taken not to 'overflow' the arrays of ioinfo structs.
*
*           3. See exec\dospawn.c for the encoding of the file handle info
*              to be passed to a child process.
*
*Entry:
*       No parameters: reads the STARTUPINFO structure.
*
*Exit:
*       0 on success, -1 if error encountered
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _ioinit (
        void
        )
{
        STARTUPINFO StartupInfo;
        int cfi_len;
        int fh;
        int i;
        ioinfo *pio;
        char *posfile;
        UNALIGNED intptr_t *posfhnd;
        intptr_t stdfh;
        DWORD htype;

        __try {
                        /*
                        Note: MSDN specifically notes that GetStartupInfo returns no error, and throws unspecified SEH if it fails, so
                        the very general exception handler below is appropriate
                        */
            GetStartupInfo( &StartupInfo );
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return -1;
        }

        /*
         * Allocate and initialize the first array of ioinfo structs. This
         * array is pointed to by __pioinfo[0]
         */
        if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))
             == NULL )
        {
            return -1;
        }

        __pioinfo[0] = pio;
        _nhandle = IOINFO_ARRAY_ELTS;

        for ( ; pio < __pioinfo[0] + IOINFO_ARRAY_ELTS ; pio++ ) {
            pio->osfile = 0;
            pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
            pio->pipech = 10;                   /* linefeed/newline char */
            pio->lockinitflag = 0;              /* uninitialized lock */
            pio->textmode = 0;
            pio->unicode = 0;
            pio->pipech2[0] = 10;
            pio->pipech2[1] = 10;
        }

        /*
         * Process inherited file handle information, if any
         */

        if ( (StartupInfo.cbReserved2 != 0) &&
             (StartupInfo.lpReserved2 != NULL) )
        {
            /*
             * Get the number of handles inherited.
             */
            cfi_len = *(UNALIGNED int *)(StartupInfo.lpReserved2);

            /*
             * Set pointers to the start of the passed file info and OS
             * HANDLE values.
             */
            posfile = (char *)(StartupInfo.lpReserved2) + sizeof( int );
            posfhnd = (UNALIGNED intptr_t *)(posfile + cfi_len);

            /*
             * Ensure cfi_len does not exceed the number of supported
             * handles!
             */
            cfi_len = __min( cfi_len, _NHANDLE_ );

            /*
             * Allocate sufficient arrays of ioinfo structs to hold inherited
             * file information.
             */
            for ( i = 1 ; _nhandle < cfi_len ; i++ ) {

                /*
                 * Allocate another array of ioinfo structs
                 */
                if ( (pio = _calloc_crt( IOINFO_ARRAY_ELTS, sizeof(ioinfo) ))
                    == NULL )
                {
                    /*
                     * No room for another array of ioinfo structs, reduce
                     * the number of inherited handles we process.
                     */
                    cfi_len = _nhandle;
                    break;
                }

                /*
                 * Update __pioinfo[] and _nhandle
                 */
                __pioinfo[i] = pio;
                _nhandle += IOINFO_ARRAY_ELTS;

                for ( ; pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ; pio++ ) {
                    pio->osfile = 0;
                    pio->osfhnd = (intptr_t)INVALID_HANDLE_VALUE;
                    pio->pipech = 10;
                    pio->lockinitflag = 0;
                    pio->textmode = 0;
                    pio->pipech2[0] = 10;
                    pio->pipech2[1] = 10;
                }
            }

            /*
             * Validate and copy the passed file information
             */
            for ( fh = 0 ; fh < cfi_len ; fh++, posfile++, posfhnd++ ) {
                /*
                 * Copy the passed file info iff it appears to describe
                 * an open, valid file or device.
                 *
                 * Note that GetFileType cannot be called for pipe handles
                 * since it may 'hang' if there is blocked read pending on
                 * the pipe in the parent.
                 */
                if ( (*posfhnd != (intptr_t)INVALID_HANDLE_VALUE) &&
                     (*posfhnd != _NO_CONSOLE_FILENO) &&
                     (*posfile & FOPEN) &&
                     ((*posfile & FPIPE) ||
                      (GetFileType( (HANDLE)*posfhnd ) != FILE_TYPE_UNKNOWN)) )
                {
                    pio = _pioinfo( fh );
                    pio->osfhnd = *posfhnd;
                    pio->osfile = *posfile;
                    /* Allocate the lock for this handle. */
                    if ( !__crtInitCritSecAndSpinCount( &pio->lock,
                                                        _CRT_SPINCOUNT ))
                        return -1;
                    pio->lockinitflag++;
                }
            }
        }

        /*
         * If valid HANDLE-s for standard input, output and error were not
         * inherited, try to obtain them directly from the OS. Also, set the
         * appropriate bits in the osfile fields.
         */
        for ( fh = 0 ; fh < 3 ; fh++ ) {

            pio = __pioinfo[0] + fh;

            if ( (pio->osfhnd == (intptr_t)INVALID_HANDLE_VALUE) ||
                 (pio->osfhnd == _NO_CONSOLE_FILENO)) {
                /*
                 * mark the handle as open in text mode.
                 */
                pio->osfile = (char)(FOPEN | FTEXT);

                if ( ((stdfh = (intptr_t)GetStdHandle( stdhndl(fh) )) != (intptr_t)INVALID_HANDLE_VALUE) &&
                     (stdfh!=((intptr_t)NULL)) &&
                     ((htype = GetFileType( (HANDLE)stdfh )) != FILE_TYPE_UNKNOWN) )
                {
                    /*
                     * obtained a valid HANDLE from GetStdHandle
                     */
                    pio->osfhnd = stdfh;

                    /*
                     * finish setting osfile: determine if it is a character
                     * device or pipe.
                     */
                    if ( (htype & 0xFF) == FILE_TYPE_CHAR )
                        pio->osfile |= FDEV;
                    else if ( (htype & 0xFF) == FILE_TYPE_PIPE )
                        pio->osfile |= FPIPE;

                    /* Allocate the lock for this handle. */
                    if ( !__crtInitCritSecAndSpinCount( &pio->lock,
                                                        _CRT_SPINCOUNT ))
                        return -1;
                    pio->lockinitflag++;
                }
                else {
                    /*
                     * For stdin, stdout & stderr, if there is no valid HANDLE,
                     * treat the CRT handle as being open in text mode on a
                     * device with _NO_CONSOLE_FILENO underlying it. We use this
                     * value different from _INVALID_HANDLE_VALUE to distinguish
                     * between a failure in opening a file & a program run
                     * without a console.
                     */
                    pio->osfile |= FDEV;
                    pio->osfhnd = _NO_CONSOLE_FILENO;
                }
            }
            else  {
                /*
                 * handle was passed to us by parent process. make
                 * sure it is text mode.
                 */
                pio->osfile |= FTEXT;
            }
        }

        /*
         * Set the number of supported HANDLE-s to _nhandle
         */
        (void)SetHandleCount( (unsigned)_nhandle );

        return 0;
}


/***
*_ioterm() -
*
*Purpose:
*       Free the memory holding the ioinfo arrays.
*
*       In the multi-thread case, first walk each array of ioinfo structs and
*       delete any all initialized critical sections (locks).
*
*Entry:
*       No parameters.
*
*Exit:
*       No return value.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _ioterm (
        void
        )
{
        int i;
        ioinfo *pio;

        for ( i = 0 ; i < IOINFO_ARRAYS ; i++ ) {

            if ( __pioinfo[i] != NULL ) {
                /*
                 * Delete any initialized critical sections.
                 */
                for ( pio = __pioinfo[i] ;
                      pio < __pioinfo[i] + IOINFO_ARRAY_ELTS ;
                      pio++ )
                {
                    if ( pio->lockinitflag )
                        DeleteCriticalSection( &(pio->lock) );
                }
                /*
                 * Free the memory which held the ioinfo array.
                 */
                _free_crt( __pioinfo[i] );
                __pioinfo[i] = NULL;
            }
        }
}
