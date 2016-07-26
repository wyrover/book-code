/***
*wrt2err.c - write an LSTRING to stderr (Win32 version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module contains a routine __wrt2err that writes an LSTRING
*       (one byte length followed by the several bytes of the string)
*       to the standard error handle (2).  This is a helper routine used
*       for MATH error messages (and also FORTRAN error messages).
*
*******************************************************************************/


#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>

/***
*__wrt2err(msg) - write an LSTRING to stderr
*
*Purpose:
*       Takes a pointer to an LSTRING which is to be written to standard error.
*       An LSTRING is a one-byte length followed by that many bytes for the
*       character string (as opposed to a null-terminated string).
*
*Entry:
*       char *msg = pointer to LSTRING to write to standard error.
*
*Exit:
*       Nothing returned.
*
*Exceptions:
*       None handled.
*
*******************************************************************************/

void __cdecl __wrt2err (
        char *msg
        )
{
        unsigned long length;           /* length of string to write */
        unsigned long numwritten;       /* number of bytes written */

        length = *msg++;                /* 1st byte is length */

        /* write the message to stderr */

        WriteFile((HANDLE)_osfhnd(2), msg, length, &numwritten, NULL);
}

