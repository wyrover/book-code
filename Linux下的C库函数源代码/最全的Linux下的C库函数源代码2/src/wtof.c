/***
*wtof.c - convert wchar_t string to floating point number
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts a wide character string into a floating point number.
*
*******************************************************************************/

#ifndef _UNICODE
#define _UNICODE
#endif  /* _UNICODE */

#include <stdlib.h>
#include <math.h>
#include <cruntime.h>
#include <fltintrn.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*double _wtof(ptr) - convert wide char string to floating point number
*
*Purpose:
*       _wtof recognizes an optional string of whitespace, then
*       an optional sign, then a string of digits optionally
*       containing a decimal point, then an optional e or E followed
*       by an optionally signed integer, and converts all this to
*       to a floating point number.  The first unrecognized
*       character ends the string.
*
*Entry:
*       ptr - pointer to wide char string to convert
*
*Exit:
*       returns floating point value of wide character representation
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
double __cdecl _wtof_l(
        REG1 const wchar_t *nptr,
        _locale_t plocinfo
        )
{
        struct _flt fltstruct;      /* temporary structure */
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(nptr != NULL, EINVAL, 0.0);

        /* scan past leading space/tab characters */
        while ( _iswspace_l(*nptr, _loc_update.GetLocaleT()) )
                nptr++;

        /* let _fltin routine do the rest of the work */
        return( *(double *)&(_wfltin2( &fltstruct, nptr, (int)wcslen(nptr), 0, 0, _loc_update.GetLocaleT() )->dval) );
}

double __cdecl _wtof
(
    REG1 const wchar_t *nptr
)
{
    return _wtof_l(nptr, NULL);
}

unsigned int __wstrgtold12
(
    _LDBL12 *pld12,
    const wchar_t * *p_end_ptr,
    const wchar_t * str,
    int mult12,
    int scale,
    int decpt,
    int implicit_E
)
{
        _LocaleUpdate _loc_update(NULL);

        return __wstrgtold12_l(pld12, p_end_ptr, str, mult12, scale, decpt, implicit_E, _loc_update.GetLocaleT());
}

unsigned __WSTRINGTOLD
(
        _LDOUBLE *pld,
        const wchar_t  * *p_end_ptr,
        const wchar_t  *str,
        int mult12
)
{
        _LocaleUpdate _loc_update(NULL);

        return __WSTRINGTOLD_L(pld, p_end_ptr, str, mult12, _loc_update.GetLocaleT());
}

