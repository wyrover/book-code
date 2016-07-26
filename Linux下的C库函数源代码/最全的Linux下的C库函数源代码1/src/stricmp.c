/***
*stricmp.c - contains case-insensitive string comp routine _stricmp/_strcmpi
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains _stricmp(), also known as _strcmpi()
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>


/***
*int _strcmpi(dst, src), _strcmpi(dst, src) - compare strings, ignore case
*
*Purpose:
*       _stricmp/_strcmpi perform a case-insensitive string comparision.
*       For differences, upper case letters are mapped to lower case.
*       Thus, "abc_" < "ABCD" since "_" < "d".
*
*Entry:
*       char *dst, *src - strings to compare
*
*Return:
*       Returns <0 if dst < src
*       Returns 0 if dst = src
*       Returns >0 if dst > src
*       Returns _NLSCMPERROR is something went wrong
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _stricmp_l (
        const char * dst,
        const char * src,
        _locale_t plocinfo
        )
{
    int f,l;
    _LocaleUpdate _loc_update(plocinfo);

    /* validation section */
    _VALIDATE_RETURN(dst != NULL, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(src != NULL, EINVAL, _NLSCMPERROR);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
    {
        return __ascii_stricmp(dst, src);
    }
    else
    {
        do
        {
            f = _tolower_l( (unsigned char)(*(dst++)), _loc_update.GetLocaleT() );
            l = _tolower_l( (unsigned char)(*(src++)), _loc_update.GetLocaleT() );
        } while ( f && (f == l) );
    }

    return(f - l);
}

extern "C" int __cdecl __ascii_stricmp (
        const char * dst,
        const char * src
        )
{
    int f, l;

    do
    {
        if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
            l -= 'A' - 'a';
    }
    while ( f && (f == l) );

    return(f - l);
}

extern "C" int __cdecl _stricmp (
        const char * dst,
        const char * src
        )
{
    if (__locale_changed == 0)
    {
        /* validation section */
        _VALIDATE_RETURN(dst != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(src != NULL, EINVAL, _NLSCMPERROR);

        return __ascii_stricmp(dst, src);
    }
    else
    {
        return _stricmp_l(dst, src, NULL);
    }
}
