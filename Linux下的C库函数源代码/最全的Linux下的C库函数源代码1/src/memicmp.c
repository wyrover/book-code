/***
*memicmp.c - compare memory, ignore case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _memicmp() - compare two blocks of memory for lexical
*       order.  Case is ignored in the comparison.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>
#include <mtdll.h>
#include <ctype.h>
#include <locale.h>
#include <internal.h>
#include <setlocal.h>

/***
*int _memicmp(first, last, count) - compare two blocks of memory, ignore case
*
*Purpose:
*       Compares count bytes of the two blocks of memory stored at first
*       and last.  The characters are converted to lowercase before
*       comparing (not permanently), so case is ignored in the search.
*
*Entry:
*       char *first, *last - memory buffers to compare
*       size_t count - maximum length to compare
*
*Exit:
*       Returns < 0 if first < last
*       Returns 0 if first == last
*       Returns > 0 if first > last
*       Returns _NLSCMPERROR is something went wrong
*
*Uses:
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" int __cdecl _memicmp_l (
        const void * first,
        const void * last,
        size_t count,
        _locale_t plocinfo
        )
{
    int f = 0, l = 0;
    const char *dst = (const char *)first, *src = (const char *)last;
    _LocaleUpdate _loc_update(plocinfo);

    /* validation section */
    _VALIDATE_RETURN(first != NULL || count == 0, EINVAL, _NLSCMPERROR);
    _VALIDATE_RETURN(last != NULL || count == 0, EINVAL, _NLSCMPERROR);

    if ( __LC_HANDLE(_loc_update.GetLocaleT()->locinfo)[LC_CTYPE] == _CLOCALEHANDLE )
    {
        return __ascii_memicmp(first, last, count);
    }
    else
    {
        while (count-- && f==l)
        {
            f = _tolower_l( (unsigned char)(*(dst++)), _loc_update.GetLocaleT() );
            l = _tolower_l( (unsigned char)(*(src++)), _loc_update.GetLocaleT() );
        }
    }
    return ( f - l );
}


#ifndef _M_IX86

extern "C" int __cdecl __ascii_memicmp (
        const void * first,
        const void * last,
        size_t count
        )
{
    int f = 0;
    int l = 0;

    while ( count-- )
    {
        if ( (*(unsigned char *)first == *(unsigned char *)last) ||
             ((f = __ascii_tolower( *(unsigned char *)first )) ==
              (l = __ascii_tolower( *(unsigned char *)last ))) )
        {
                first = (char *)first + 1;
                last = (char *)last + 1;
        }
        else
            break;
    }
    return ( f - l );
}

#endif  /* _M_IX86 */

extern "C" int __cdecl _memicmp (
        const void * first,
        const void * last,
        size_t count
        )
{
    if (__locale_changed == 0)
    {
        /* validation section */
        _VALIDATE_RETURN(first != NULL || count == 0, EINVAL, _NLSCMPERROR);
        _VALIDATE_RETURN(last != NULL || count == 0, EINVAL, _NLSCMPERROR);

        return __ascii_memicmp(first, last, count);
    }
    else
    {
        return _memicmp_l(first, last, count, NULL);
    }
}
