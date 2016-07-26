/***
*strnicmp.c - compare n chars of strings, ignoring case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _strnicmp() - Compares at most n characters of two strings,
*       without regard to case.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*int _strnicmp(first, last, count) - compares count char of strings, ignore case
*
*Purpose:
*       Compare the two strings for lexical order.  Stops the comparison
*       when the following occurs: (1) strings differ, (2) the end of the
*       strings is reached, or (3) count characters have been compared.
*       For the purposes of the comparison, upper case characters are
*       converted to lower case.
*
*Entry:
*       char *first, *last - strings to compare
*       size_t count - maximum number of characters to compare
*
*Exit:
*       returns <0 if first < last
*       returns 0 if first == last
*       returns >0 if first > last
*       Returns _NLSCMPERROR is something went wrong
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _strnicmp_l (
        const char * dst,
        const char * src,
        size_t count,
        _locale_t plocinfo
        )
{
    int f,l;

    if ( count )
    {
        _LocaleUpdate _loc_update(plocinfo);

        /* validation section */
        _VALIDATE_RETURN(dst != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(src != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(count <= INT_MAX, EINVAL, _NLSCMPERROR);

        if ( __LC_HANDLE(_loc_update.GetLocaleT()->locinfo)[LC_CTYPE] == _CLOCALEHANDLE )
        {
            return __ascii_strnicmp(dst, src, count);
        }
        else
        {
            do
            {
                f = _tolower_l( (unsigned char)(*(dst++)), _loc_update.GetLocaleT() );
                l = _tolower_l( (unsigned char)(*(src++)), _loc_update.GetLocaleT() );
            }
            while (--count && f && (f == l) );
        }
        return( f - l );
    }

    return( 0 );
}


#ifndef _M_IX86

extern "C" int __cdecl __ascii_strnicmp (
        const char * first,
        const char * last,
        size_t count
        )
{
    if(count)
    {
        int f=0;
        int l=0;

        do
        {

            if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
                    (f <= 'Z') )
                f -= 'A' - 'a';

            if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
                    (l <= 'Z') )
                l -= 'A' - 'a';

        }
        while ( --count && f && (f == l) );

        return ( f - l );
    }
    else
    {
        return 0;
    }
}

#endif  /* _M_IX86 */

extern "C" int __cdecl _strnicmp (
        const char * dst,
        const char * src,
        size_t count
        )
{

    if (__locale_changed == 0)
    {
        /* validation section */
        _VALIDATE_RETURN(dst != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(src != NULL, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(count <= INT_MAX, EINVAL, _NLSCMPERROR);

        return __ascii_strnicmp(dst, src, count);
    }
    else
    {
        return _strnicmp_l(dst, src, count, NULL);
    }

}
