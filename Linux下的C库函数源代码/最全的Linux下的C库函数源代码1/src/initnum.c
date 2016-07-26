/***
*initnum.c - contains __init_numeric
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the locale-category initialization function: __init_numeric().
*
*       Each initialization function sets up locale-specific information
*       for their category, for use by functions which are affected by
*       their locale category.
*
*       *** For internal use by setlocale() only ***
*
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>
#include <setlocal.h>
#include <malloc.h>
#include <nlsint.h>
#include <dbgint.h>
#include <mtdll.h>

void __cdecl __free_lconv_num(struct lconv *);

static void fix_grouping(
        char *grouping
        )
{
    /*
     * ANSI specifies that the fields should contain "\3" [\3\0] to indicate
     * thousands groupings (100,000,000.00 for example).
     * NT uses "3;0"; ASCII 3 instead of value 3 and the ';' is extra.
     * So here we convert the NT version to the ANSI version.
     */

    while (*grouping)
    {
        /* convert '3' to '\3' */
        if (*grouping >= '0' && *grouping <= '9')
        {
            *grouping = *grouping - '0';
            grouping++;
        }

        /* remove ';' */
        else if (*grouping == ';')
        {
            char *tmp = grouping;

            do
                *tmp = *(tmp+1);
            while (*++tmp);
        }

        /* unknown (illegal) character, ignore */
        else
            grouping++;
    }
}

/***
*int __init_numeric() - initialization for LC_NUMERIC locale category.
*
*Purpose:
*
*Entry:
*       None.
*
*Exit:
*       0 success
*       1 fail
*
*Exceptions:
*
*******************************************************************************/

int __cdecl __init_numeric (
        pthreadlocinfo ploci
        )
{
    struct lconv *lc;
    int ret = 0;
    LCID ctryid;
    int *lc_refcount;
    int *lconv_num_refcount = NULL;
    _locale_tstruct locinfo;

    locinfo.locinfo = ploci;
    locinfo.mbcinfo = 0;

    if ( (ploci->lc_handle[LC_NUMERIC] != _CLOCALEHANDLE) ||
         (ploci->lc_handle[LC_MONETARY] != _CLOCALEHANDLE) )
    {
        /*
         * Allocate structure filled with NULL pointers
         */
        if ( (lc = (struct lconv *)_calloc_crt(1, sizeof(struct lconv)))
             == NULL )
            return 1;

        /*
         * Copy over all fields (esp., the monetary category)
         */
        *lc = *ploci->lconv;

        /*
         * Allocate a new reference counter for the lconv structure
         */
        if ( (lc_refcount = _malloc_crt(sizeof(int))) == NULL )
        {
            _free_crt(lc);
            return 1;
        }
        *lc_refcount = 0;

        if ( ploci->lc_handle[LC_NUMERIC] != _CLOCALEHANDLE )
        {
            /*
             * Allocate a new reference counter for the numeric info
             */
            if ( (lconv_num_refcount = _malloc_crt(sizeof(int))) == NULL )
            {
                _free_crt(lc);
                _free_crt(lc_refcount);
                return 1;
            }
            *lconv_num_refcount = 0;

            /*
             * Numeric data is country--not language--dependent. NT
             * work-around.
             */
            ctryid = MAKELCID(ploci->lc_id[LC_NUMERIC].wCountry, SORT_DEFAULT);

            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid, LOCALE_SDECIMAL,
                    (void *)&lc->decimal_point);
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid, LOCALE_STHOUSAND,
                    (void *)&lc->thousands_sep);
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid, LOCALE_SGROUPING,
                    (void *)&lc->grouping);

            if (ret) {
                    /* Clean up before returning failure */
                    __free_lconv_num(lc);
                    _free_crt(lc);
                    _free_crt(lc_refcount);
                    return -1;
            }

            fix_grouping(lc->grouping);
        }
        else {
            /*
             * C locale for just the numeric category.
             */
            /*
             * NULL out the reference count pointer
             */
            lconv_num_refcount = NULL;
            lc->decimal_point = __lconv_c.decimal_point;
            lc->thousands_sep = __lconv_c.thousands_sep;
            lc->grouping = __lconv_c.grouping;
        }
        (*lc_refcount) = 1;
        if (lconv_num_refcount)
            (*lconv_num_refcount) = 1;
    }

    else {
        /*
         * C locale for BOTH numeric and monetary categories.
         */
        lconv_num_refcount = NULL;
        lc_refcount = NULL;
        lc = &__lconv_c;           /* point to new one */
    }
    /*
     * If this is part of LC_ALL, then we need to free the old ploci->lconv
     * set up in init_monetary() before this.
     */
    if ( (ploci->lconv_num_refcount != NULL) &&
         (InterlockedDecrement(ploci->lconv_num_refcount) == 0))
    {
        _ASSERTE(ploci->lconv_num_refcount > 0);
    }
    if ( (ploci->lconv_intl_refcount != NULL) &&
         (InterlockedDecrement(ploci->lconv_intl_refcount) == 0))
    {
        _free_crt(ploci->lconv_intl_refcount);
        _free_crt(ploci->lconv);
    }

    ploci->lconv_num_refcount = lconv_num_refcount;
    ploci->lconv_intl_refcount = lc_refcount;

    ploci->lconv = lc;
    return 0;
}

/*
 *  Free the lconv numeric strings.
 *  Numeric values do not need to be freed.
 */
void __cdecl __free_lconv_num(
        struct lconv *l
        )
{
    if (l == NULL)
        return;

    if ( l->decimal_point != __lconv_c.decimal_point )
        _free_crt(l->decimal_point);

    if ( l->thousands_sep != __lconv_c.thousands_sep )
        _free_crt(l->thousands_sep);

    if ( l->grouping != __lconv_c.grouping )
        _free_crt(l->grouping);
}
