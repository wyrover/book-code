/***
*perror.c - print system error message
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines perror() - print system error message
*       System error message are indexed by errno; conforms to XENIX
*       standard, with much compatability with 1983 uniforum draft standard.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syserr.h>
#include <mtdll.h>
#include <io.h>
#include <internal.h>

/***
*void perror(message) - print system error message
*
*Purpose:
*       prints user's error message, then follows it with ": ", then the system
*       error message, then a newline.  All output goes to stderr.  If user's
*       message is NULL or a null string, only the system error message is
*       printer.  If errno is weird, prints "Unknown error".
*
*Entry:
*       const char *message - users message to prefix system error message
*
*Exit:
*       Prints message; no return value.
*
*Exceptions:
*
*******************************************************************************/

void __cdecl perror (
        REG1 const char *message
        )
{

        REG2 int fh = 2;

        _lock_fh( fh );         /* acquire file handle lock */
        __try {


        if (message && *message)
        {

            _write_nolock(fh,(char *)message,(unsigned int)strlen(message));
            _write_nolock(fh,": ",2);
        }

        message = _get_sys_err_msg( errno );

        _write_nolock(fh,(char *)message,(unsigned int)strlen(message));
        _write_nolock(fh,"\n",1);

        }
        __finally {
            _unlock_fh( fh );   /* release file handle lock */
        }

}
