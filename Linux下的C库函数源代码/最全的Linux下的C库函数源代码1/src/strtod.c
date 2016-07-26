/***
*strtod.c - convert string to floating point number
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert character string to floating point number
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <fltintrn.h>
#include <string.h>
#include <ctype.h>
#include <mbctype.h>
#include <errno.h>
#include <math.h>
#include <internal.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*double strtod(nptr, endptr) - convert string to double
*
*Purpose:
*       strtod recognizes an optional string of tabs and spaces,
*       then an optional sign, then a string of digits optionally
*       containing a decimal point, then an optional e or E followed
*       by an optionally signed integer, and converts all this to
*       to a floating point number.  The first unrecognized
*       character ends the string, and is pointed to by endptr.
*
*Entry:
*       nptr - pointer to string to convert
*
*Exit:
*       returns value of character string
*       char **endptr - if not NULL, points to character which stopped
*                       the scan
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" double __cdecl _strtod_l (
        const char *nptr,
        REG2 char **endptr,
        _locale_t plocinfo
        )
{

        struct _flt answerstruct;

        FLT      answer;
        double       tmp;
        unsigned int flags;
        REG1 char *ptr = (char *) nptr;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        if (endptr != NULL)
        {
            /* store beginning of string in endptr */
            *endptr = (char *)nptr;
        }
        _VALIDATE_RETURN(nptr != NULL, EINVAL, 0.0);

        /* scan past leading space/tab characters */

        while ( _isspace_l((int)(unsigned char)*ptr, _loc_update.GetLocaleT()) )
                ptr++;

        /* let _fltin routine do the rest of the work */

        /* ok to take address of stack variable here; fltin2 knows to use ss */
        answer = _fltin2( &answerstruct, ptr, (int)strlen(ptr), 0, 0, _loc_update.GetLocaleT());

        if ( endptr != NULL )
                *endptr = (char *) ptr + answer->nbytes;

        flags = answer->flags;
        if ( flags & (512 | 64)) {
                /* no digits found or invalid format:
                   ANSI says return 0.0, and *endptr = nptr */
                tmp = 0.0;
                if ( endptr != NULL )
                        *endptr = (char *) nptr;
        }
        else if ( flags & (128 | 1) ) {
                if ( *ptr == '-' )
                        tmp = -HUGE_VAL;        /* negative overflow */
                else
                        tmp = HUGE_VAL;         /* positive overflow */
                errno = ERANGE;
        }
        else if ( (flags & 256) && answer->dval == 0.0 ) {
                tmp = 0.0;                      /* underflow (denormals OK) */
                errno = ERANGE;
        }
        else
                tmp = answer->dval;

        return(tmp);
}

extern "C" double __cdecl strtod (
        const char *nptr,
        REG2 char **endptr
        )
{
    return _strtod_l(nptr, endptr, NULL);
}
