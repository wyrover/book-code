/***
*cputs.c - direct console output
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _cputs() - write string directly to console
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <internal.h>
#include <mtdll.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * declaration for console handle
 */
extern intptr_t _confh;

/***
*int _cputs(string) - put a string to the console
*
*Purpose:
*       Writes the string directly to the console.  No newline
*       is appended.
*
*Entry:
*       char *string - string to write
*
*Exit:
*       Good return = 0
*       Error return = !0
*
*Exceptions:
*
*******************************************************************************/

int __cdecl _cputs (
        const char *string
        )
{
        int error = 0;                   /* error occurred? */

        _VALIDATE_CLEAR_OSSERR_RETURN((string != NULL), EINVAL, -1);

        _mlock(_CONIO_LOCK);             /* acquire console lock */
        __try {

            /*
             * _confh, the handle to the console output, is created the
             * first time that either _putch() or _cputs() is called.
             */

            if (_confh == -2)
                __initconout();

            /* write string to console file handle */

            /*
             * What is more important when writing to console. I don't think if
             * speed should matter too much. This justification is used for writing
             * the string to Console. Here we are converting each and every
             * character to wide character and then writing it to console.
             */
            while(*string)
            {
                if (_putch_nolock(*string++) == EOF) {
                    error = -1;
                    break;
                }
            }

        }
        __finally {
            _munlock(_CONIO_LOCK);          /* release console lock */
        }

        return error;
}
