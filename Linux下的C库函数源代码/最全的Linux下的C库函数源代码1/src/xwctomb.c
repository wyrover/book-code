/***
*xwctomb.c - Convert wide character to multibyte character, with locale.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a wide character into the equivalent multibyte character.
*
*******************************************************************************/


#include <cruntime.h>
#include <stdlib.h>
#include <mtdll.h>
#include <errno.h>
#include <limits.h>             /* for MB_LEN_MAX */
#include <string.h>             /* for memcpy */
#include <stdio.h>              /* for EOF */
#include <xlocinfo.h>           /* for _Cvtvec, _Wcrtomb */
#include <locale.h>
#include <setlocal.h>
#include <internal.h>

/***
*int _Wcrtomb() - Convert wide character to multibyte character.
*
*Purpose:
*       Convert a wide character into the equivalent multi-byte character,
*       according to the specified LC_CTYPE category, or the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       char *s             = pointer to multibyte character
*       wchar_t wchar       = source wide character
*       mbstate_t *pst      = pointer to state (not used)
*       const _Cvtvec *ploc = pointer to locale info
*
*Exit:
*       Returns:
*      -1 (if error) or number of bytes comprising converted mbc
*
*Exceptions:
*
*******************************************************************************/

/* Retained for backward compatibility of DLL exports only */
_MRTIMP2 int __cdecl __Wcrtomb_lk
        (
        char *s,
        wchar_t wchar,
        mbstate_t *pst,
        const _Cvtvec *ploc
        )
{
        return _Wcrtomb(s, wchar, pst, ploc);
}

_MRTIMP2 int __cdecl _Wcrtomb
        (
        char *s,
        wchar_t wchar,
        mbstate_t *pst,
        const _Cvtvec *ploc
        )
{
        LCID handle;
        UINT codepage;

        if (ploc == 0)
        {
            handle = ___lc_handle_func()[LC_CTYPE];
            codepage = ___lc_codepage_func();
        }
        else
        {
            handle = ploc->_Hand;
            codepage = ploc->_Page;
        }

        if ( handle == _CLOCALEHANDLE )
        {
            if ( wchar > 255 )  /* validate high byte */
            {
                errno = EILSEQ;
                return -1;
            }

            *s = (char) wchar;
            return sizeof(char);
        } else {
            int size;
            BOOL defused = 0;
            _locale_t locale = _GetLocaleForCP(codepage);

            if ( ((size = WideCharToMultiByte(codepage,
                                              0,
                                              &wchar,
                                              1,
                                              s,
                                              ___mb_cur_max_l_func(locale),
                                              NULL,
                                              &defused)) == 0) ||
                 (defused) )
            {
                errno = EILSEQ;
                return -1;
            }

            return size;
        }
}

#ifdef MRTDLL
_MRTIMP2 int __cdecl _Wcrtomb
        (
        char *s,
        unsigned short wchar,
        mbstate_t *pst,
        const _Cvtvec *ploc
        )
    {
    return _Wcrtomb(s,(wchar_t) wchar, pst, ploc);
    }
#endif  /* MRTDLL */

/***
*_Cvtvec _Getcvt() - get conversion info for current locale
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

_MRTIMP2 _Cvtvec __cdecl _Getcvt()
{
        _Cvtvec cvt;

        cvt._Hand = ___lc_handle_func()[LC_CTYPE];
        cvt._Page = ___lc_codepage_func();

        return (cvt);
}
