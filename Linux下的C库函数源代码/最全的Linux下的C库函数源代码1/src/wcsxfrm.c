/***
*wcsxfrm.c - Transform a wide-character string using locale information
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Transform a wide-character string using the locale information as set by
*       LC_COLLATE.
*
*******************************************************************************/


#include <cruntime.h>
#include <windows.h>
#include <string.h>
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <awint.h>
#include <dbgint.h>
#include <malloc.h>
#include <internal.h>
#include <mtdll.h>
#include <setlocal.h>

/***
*size_t wcsxfrm() - Transform a string using locale information
*
*Purpose:
*       Transform the wide string pointed to by _string2 and place the
*       resulting wide string into the array pointed to by _string1.
*       No more than _count wide characters are placed into the
*       resulting string (including the null).
*
*       The transformation is such that if wcscmp() is applied to
*       the two transformed strings, the return value is equal to
*       the result of wcscoll() applied to the two original strings.
*       Thus, the conversion must take the locale LC_COLLATE info
*       into account.
*
*       In the C locale, wcsxfrm() simply resolves to wcsncpy()/wcslen().
*
*Entry:
*       wchar_t *_string1       = result string
*       const wchar_t *_string2 = source string
*       size_t _count           = max wide chars to move
*
*       [If _count is 0, _string1 is permitted to be NULL.]
*
*Exit:
*       Length of the transformed string (not including the terminating
*       null).  If the value returned is >= _count, the contents of the
*       _string1 array are indeterminate.
*
*Exceptions:
*       Non-standard: if OM/API error, return INT_MAX.
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/

extern "C" size_t __cdecl _wcsxfrm_l (
        wchar_t *_string1,
        const wchar_t *_string2,
        size_t _count,
        _locale_t plocinfo
        )
{
    int size = INT_MAX;

    /* validation section */
    _VALIDATE_RETURN(_count <= INT_MAX, EINVAL, INT_MAX);
    _VALIDATE_RETURN(_string1 != NULL || _count == 0, EINVAL, INT_MAX);
    _VALIDATE_RETURN(_string2 != NULL, EINVAL, INT_MAX);

    _LocaleUpdate _loc_update(plocinfo);

    if ( _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
    {
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
        wcsncpy(_string1, _string2, _count);
_END_SECURE_CRT_DEPRECATION_DISABLE
        return wcslen(_string2);
    }

    if ( 0 == (size = __crtLCMapStringW(_loc_update.GetLocaleT(),
                    _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE],
                    LCMAP_SORTKEY,
                    _string2,
                    -1,
                    NULL,
                    0,
                    _loc_update.GetLocaleT()->locinfo->lc_collate_cp )) )
    {
        errno = EILSEQ;
        size = INT_MAX;
    } else
    {
        if ( size <= (int)_count)
        {
            if ( 0 == (size = __crtLCMapStringW(
                            _loc_update.GetLocaleT(),
                            _loc_update.GetLocaleT()->locinfo->lc_handle[LC_COLLATE],
                            LCMAP_SORTKEY,
                            _string2,
                            -1,
                            (wchar_t *)_string1,
                            (int)_count,
                            _loc_update.GetLocaleT()->locinfo->lc_collate_cp )) )
            {
                errno = EILSEQ;
                size = INT_MAX; /* default error */
            } else
            {
                // Note that the size that LCMapStringW returns for
                // LCMAP_SORTKEY is number of bytes needed. That's why it
                // is safe to convert the buffer to wide char from end.
                _count = size--;
                for (;_count-- > 0;)
                {
                    _string1[_count] = (wchar_t)((unsigned char *)_string1)[_count];
                }
            }
        } else
        {
            *_string1 = '\0';
            errno = ERANGE;
            size--;
        }
    }

    return (size_t)size;
}

extern "C" size_t __cdecl wcsxfrm (
        wchar_t *_string1,
        const wchar_t *_string2,
        size_t _count
        )
{

    return _wcsxfrm_l(_string1, _string2, _count, NULL);
}

