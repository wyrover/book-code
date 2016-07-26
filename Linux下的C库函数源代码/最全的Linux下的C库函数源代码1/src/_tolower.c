/***
*_tolower.c - convert character to lower case
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines _Tolower().
*
*******************************************************************************/

#include <cruntime.h>
#include <ctype.h>
#include <stddef.h>
#include <xlocinfo.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>
#include <awint.h>
#include <stdlib.h>
#include <dbgint.h>
#include <yvals.h>

/* remove macro definitions of _tolower() and tolower()
 */
#undef  _tolower
#undef  tolower

/***
*int _Tolower(c) - convert character to lower case
*
*Purpose:
*       _Tolower() is a version of tolower with a locale argument.
*
*Entry:
*       c - int value of character to be converted
*       const _Ctypevec * = pointer to locale info
*
*Exit:
*       returns int value of lower case representation of c
*
*Exceptions:
*
*******************************************************************************/

_MRTIMP2_NCEEPURE int __CLRCALL_PURE_OR_CDECL _Tolower (
        int c,
        const _Ctypevec *ploc
        )
{
        int size;
        unsigned char inbuffer[3];
        unsigned char outbuffer[3];

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

        if (handle == _CLOCALEHANDLE)
        {
            if ( (c >= 'A') && (c <= 'Z') )
                c = c + ('a' - 'A');
            return c;
        }

        /* if checking case of c does not require API call, do it */
        if ((unsigned)c < 256)
        {
            if (ploc == 0)
            {
                if (!isupper(c))
                {
                    return c;
                }
            }
            else
            {
                if (!(ploc->_Table[c] & _UPPER))
                {
                    return c;
                }
            }
        }

        /* convert int c to multibyte string */
        if (ploc == 0 ? _cpp_isleadbyte((c >> 8) & 0xff)
                      : (ploc->_Table[(c >> 8) & 0xff] & _LEADBYTE) != 0)
        {
            inbuffer[0] = (c >> 8 & 0xff);
            inbuffer[1] = (unsigned char)c;
            inbuffer[2] = 0;
            size = 2;
        } else {
            inbuffer[0] = (unsigned char)c;
            inbuffer[1] = 0;
            size = 1;
        }

        /* convert wide char to lowercase */
        if (0 == (size = __crtLCMapStringA(NULL, handle, LCMAP_LOWERCASE,
            (const char *)inbuffer, size, (char *)outbuffer, 3, codepage, TRUE)))
        {
            return c;
        }

        /* construct integer return value */
        if (size == 1)
            return ((int)outbuffer[0]);
        else
            return ((int)outbuffer[1] | ((int)outbuffer[0] << 8));

}


/***
*_Ctypevec _Getctype() - get ctype info for current locale
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

_MRTIMP2_NCEEPURE _Ctypevec __CLRCALL_PURE_OR_CDECL _Getctype()
{
        /* get ctype info for current locale */
        _Ctypevec ctype;

        ctype._Hand = ___lc_handle_func()[LC_COLLATE];
        ctype._Page = ___lc_codepage_func();
        ctype._Table = (const short *)_calloc_crt(256, sizeof (*__pctype_func()));
        if (ctype._Table != 0)
        {
            memcpy((void *)ctype._Table, __pctype_func(), 256 * sizeof (*__pctype_func()));
            ctype._Delfl = 1;
        }
        else
        {
            ctype._Table = (const short *)__pctype_func();
            ctype._Delfl = 0;
        }

        return (ctype);
}
