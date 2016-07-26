/***
*wcstod.c - convert wide char string to floating point number
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert character string to floating point number
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <mbctype.h>
#include <errno.h>
#include <math.h>
#include <dbgint.h>
#include <stdlib.h>
#include <malloc.h>
#include <fltintrn.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*double wcstod(nptr, endptr) - convert wide string to double
*
*Purpose:
*       wcstod recognizes an optional string of tabs and spaces,
*       then an optional sign, then a string of digits optionally
*       containing a decimal point, then an optional e or E followed
*       by an optionally signed integer, and converts all this to
*       to a floating point number.  The first unrecognized
*       character ends the string, and is pointed to by endptr.
*
*Entry:
*       nptr - pointer to wide string to convert
*
*Exit:
*       returns value of wide character string
*       wchar_t **endptr - if not NULL, points to character which stopped
*               the scan
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" double __cdecl _wcstod_l (
        const wchar_t *nptr,
        REG2 wchar_t **endptr,
        _locale_t plocinfo
        )
{

        struct _flt answerstruct;

        FLT      answer;
        double       tmp;
        unsigned int flags;
        REG1 wchar_t *ptr = (wchar_t *) nptr;
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        if (endptr != NULL)
        {
            /* store beginning of string in endptr */
            *endptr = (wchar_t *)nptr;
        }
        _VALIDATE_RETURN(nptr != NULL, EINVAL, 0.0);

        /* scan past leading space/tab characters */

        while ( _iswspace_l(*ptr, _loc_update.GetLocaleT()) )
                ptr++;

        /* let _fltin routine do the rest of the work */

        /* ok to take address of stack variable here; fltin2 knows to use ss */
        answer = _wfltin2( &answerstruct, ptr, (int)wcslen(ptr), 0, 0, _loc_update.GetLocaleT());

        if ( endptr != NULL )
                *endptr = (wchar_t *) ptr + answer->nbytes;

        flags = answer->flags;
        if ( flags & (512 | 64)) {
                /* no digits found or invalid format:
                   ANSI says return 0.0, and *endptr = nptr */
                tmp = 0.0;
                if ( endptr != NULL )
                        *endptr = (wchar_t *) nptr;
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

extern "C" double __cdecl wcstod (
        const wchar_t *nptr,
        REG2 wchar_t **endptr
        )
{
    return _wcstod_l(nptr, endptr, NULL);
}
