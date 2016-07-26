/***
*atof.c - convert char string to floating point number
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Converts a character string into a floating point number.
*
*******************************************************************************/

#include <stdlib.h>
#include <math.h>
#include <cruntime.h>
#include <fltintrn.h>
#include <string.h>
#include <ctype.h>
#include <mbctype.h>
#include <locale.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*double atof(nptr) - convert string to floating point number
*
*Purpose:
*       atof recognizes an optional string of whitespace, then
*       an optional sign, then a string of digits optionally
*       containing a decimal point, then an optional e or E followed
*       by an optionally signed integer, and converts all this to
*       to a floating point number.  The first unrecognized
*       character ends the string.
*
*Entry:
*       nptr - pointer to string to convert
*
*Exit:
*       returns floating point value of character representation
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

double __cdecl _atof_l(
        REG1 const char *nptr,
        _locale_t plocinfo
        )
{

        struct _flt fltstruct;      /* temporary structure */
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(nptr != NULL, EINVAL, 0.0);

        /* scan past leading space/tab characters */

        while ( _isspace_l((int)(unsigned char)*nptr, _loc_update.GetLocaleT()) )
                nptr++;

        /* let _fltin routine do the rest of the work */

        return( *(double *)&(_fltin2( &fltstruct, nptr, (int)strlen(nptr), 0, 0, _loc_update.GetLocaleT())->dval) );
}

double __cdecl atof(
        REG1 const char *nptr
        )
{
    return _atof_l(nptr, NULL);
}

unsigned int __strgtold12
(
    _LDBL12 *pld12,
    const char * *p_end_ptr,
    const char * str,
    int mult12,
    int scale,
    int decpt,
    int implicit_E
)
{
        _LocaleUpdate _loc_update(NULL);

        return __strgtold12_l(pld12, p_end_ptr, str, mult12, scale, decpt, implicit_E, _loc_update.GetLocaleT());
}

unsigned __STRINGTOLD
(
        _LDOUBLE *pld,
        const char  * *p_end_ptr,
        const char  *str,
        int mult12
)
{
        _LocaleUpdate _loc_update(NULL);

        return __STRINGTOLD_L(pld, p_end_ptr, str, mult12, _loc_update.GetLocaleT());
}
