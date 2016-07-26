/***
*wsetlocal.c - Contains the setlocale function (wchar_t version)
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the _wsetlocale() function.
*
*******************************************************************************/


#include <wchar.h>
#include <stdlib.h>
#include <setlocal.h>
#include <locale.h>
#include <dbgint.h>
#include <mtdll.h>
#include <internal.h>
#include <malloc.h>

#define MAXSIZE ((MAX_LC_LEN+1) * (LC_MAX-LC_MIN+1) + CATNAMES_LEN)

wchar_t * __cdecl _wsetlocale (
        int _category,
        const wchar_t *_wlocale
        )
{
        size_t size = 0;
        char *inlocale = NULL;
        char *outlocale;
        pthreadlocinfo ptloci;
        int *refcount = NULL;
        wchar_t *outwlocale = NULL;
        _locale_tstruct locale;
        _ptiddata ptd;

        /* convert WCS string into ASCII string */

        if (_wlocale)
        {
            _ERRCHECK_EINVAL_ERANGE(wcstombs_s(&size, NULL, 0, _wlocale, INT_MAX));
            if (size==0 || (inlocale = (char *)_calloc_crt(size, sizeof(char))) == NULL)
                return NULL;
            if (_ERRCHECK_EINVAL_ERANGE(wcstombs_s(NULL, inlocale, size, _wlocale, _TRUNCATE)) != 0)
            {
                _free_crt (inlocale);
                return NULL;
            }
        }

        /* set the locale and get ASCII return string */

        outlocale = setlocale(_category, inlocale);
        _free_crt (inlocale);
        if (NULL == outlocale)
            return NULL;

        // We now have a locale string, but the global locale can be changed by
        // another thread. If we allow this thread's locale to be updated before we're done
        // with this string, it might be freed from under us.
        // Call versions of the MB-to-wide-char conversions that do not update the current thread's
        // locale.

        ptd = _getptd();
        locale.locinfo = ptd->ptlocinfo;
        locale.mbcinfo = ptd->ptmbcinfo;

        /* get space for WCS return value, first call only */

        size = 0;
        if (_ERRCHECK_EINVAL_ERANGE(_mbstowcs_s_l(&size, NULL, 0, outlocale, 0, &locale)) != 0)
            return NULL;

        /* ensure that (size * num) does not overflow */
        if(((_HEAP_MAXREQ-sizeof(int)) / sizeof(wchar_t)) < size)
        {
            return NULL;
        }
        refcount = (int *)_malloc_crt(size * sizeof(wchar_t) + sizeof(int));
        if (!refcount)
            return NULL;
        outwlocale = (wchar_t *)&refcount[1];

        /* convert return value to WCS */

        if ( _ERRCHECK_EINVAL_ERANGE(_mbstowcs_s_l(NULL, outwlocale, size, outlocale, _TRUNCATE, &locale)) != 0)
        {
            _free_crt(refcount);
            return NULL;
        }

        ptloci = locale.locinfo;
        _mlock(_SETLOCALE_LOCK);
        __try {
            _ASSERTE(((ptloci->lc_category[_category].wlocale != NULL) && (ptloci->lc_category[_category].wrefcount != NULL)) ||
                     ((ptloci->lc_category[_category].wlocale == NULL) && (ptloci->lc_category[_category].wrefcount == NULL)));
            if (ptloci->lc_category[_category].wrefcount != NULL &&
                InterlockedDecrement(ptloci->lc_category[_category].wrefcount) == 0) {
                _free_crt(ptloci->lc_category[_category].wrefcount);
            }
            /*
             * Note that we are using a risky trick here.  We are adding this
             * wlocale to an existing threadlocinfo struct, and thus starting
             * the wlocale's wrefcount with the same value as the whole struct.
             * That means all code which modifies both threadlocinfo::refcount
             * and threadlocinfo::lc_category[]::wrefcount in structs that are
             * potentially shared across threads must make those modifications
             * under _SETLOCALE_LOCK.  Otherwise, there's a race condition
             * for some other thread modifying threadlocinfo::refcount after
             * we load it but before we store it to wrefcount.
             */
            *refcount = ptloci->refcount;
            ptloci->lc_category[_category].wrefcount = refcount;
            ptloci->lc_category[_category].wlocale = outwlocale;
        }
        __finally {
            _munlock(_SETLOCALE_LOCK);
        }

        return outwlocale;
}

