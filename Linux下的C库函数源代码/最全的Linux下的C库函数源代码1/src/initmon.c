/***
*initmon.c - contains __init_monetary
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the locale-category initialization function: __init_monetary().
*
*       Each initialization function sets up locale-specific information
*       for their category, for use by functions which are affected by
*       their locale category.
*
*       *** For internal use by setlocale() only ***
*
*******************************************************************************/

#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <setlocal.h>
#include <malloc.h>
#include <limits.h>
#include <dbgint.h>
#include <mtdll.h>

void __cdecl __free_lconv_mon(struct lconv *);
static void fix_grouping(char *);

/*
 *  Note that __lconv_c is used when the monetary category is in the C locale
 *  but the numeric category may not necessarily be in the C locale.
 */


/***
*int __init_monetary() - initialization for LC_MONETARY locale category.
*
*Purpose:
*       In non-C locales, read the localized monetary strings into
*       __lconv_intl, and also copy the numeric strings from __lconv into
*       __lconv_intl.  Set __lconv to point to __lconv_intl.  The old
*       __lconv_intl is not freed until the new one is fully established.
*
*       In the C locale, the monetary fields in lconv are filled with
*       contain C locale values.  Any allocated __lconv_intl fields are freed.
*
*       At startup, __lconv points to a static lconv structure containing
*       C locale strings.  This structure is never used again if
*       __init_monetary is called.
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

int __cdecl __init_monetary (
        pthreadlocinfo ploci
        )
{
    struct lconv *lc;
    int ret;
    LCID ctryid;
    int *lc_refcount;
    int *lconv_mon_refcount = NULL;
    _locale_tstruct locinfo;

    locinfo.locinfo = ploci;
    locinfo.mbcinfo = 0;

    if ( (ploci->lc_handle[LC_MONETARY] != _CLOCALEHANDLE) ||
         (ploci->lc_handle[LC_NUMERIC] != _CLOCALEHANDLE) )
    {
        /*
         * Allocate structure filled with NULL pointers
         */
        if ( (lc = (struct lconv *)
             _calloc_crt(1, sizeof(struct lconv))) == NULL )
            return 1;

        /*
         * Allocate a new reference counter for the lconv structure
         */
        if ( (lc_refcount = _malloc_crt(sizeof(int))) == NULL )
        {
            _free_crt(lc);
            return 1;
        }
        *lc_refcount = 0;

        if ( ploci->lc_handle[LC_MONETARY] != _CLOCALEHANDLE )
        {
            /*
             * Allocate a new reference counter for the numeric info
             */
            if ( (lconv_mon_refcount = _malloc_crt(sizeof(int))) == NULL )
            {
                _free_crt(lc);
                _free_crt(lc_refcount);
                return 1;
            }
            *lconv_mon_refcount = 0;
            /*
             * Currency is country--not language--dependent. NT
             * work-around.
             */
            ctryid = MAKELCID(ploci->lc_id[LC_MONETARY].wCountry, SORT_DEFAULT);

            ret = 0;

            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SINTLSYMBOL, (void *)&lc->int_curr_symbol );
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SCURRENCY, (void *)&lc->currency_symbol );
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SMONDECIMALSEP, (void *)&lc->mon_decimal_point );
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SMONTHOUSANDSEP, (void *)&lc->mon_thousands_sep );
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SMONGROUPING, (void *)&lc->mon_grouping );

            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SPOSITIVESIGN, (void *)&lc->positive_sign);
            ret |= __getlocaleinfo(&locinfo, LC_STR_TYPE, ctryid,
                    LOCALE_SNEGATIVESIGN, (void *)&lc->negative_sign);

            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_IINTLCURRDIGITS, (void *)&lc->int_frac_digits);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_ICURRDIGITS, (void *)&lc->frac_digits);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_IPOSSYMPRECEDES, (void *)&lc->p_cs_precedes);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_IPOSSEPBYSPACE, (void *)&lc->p_sep_by_space);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_INEGSYMPRECEDES, (void *)&lc->n_cs_precedes);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_INEGSEPBYSPACE, (void *)&lc->n_sep_by_space);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_IPOSSIGNPOSN, (void *)&lc->p_sign_posn);
            ret |= __getlocaleinfo(&locinfo, LC_INT_TYPE, ctryid,
                    LOCALE_INEGSIGNPOSN, (void *)&lc->n_sign_posn);

            if ( ret != 0 ) {
                __free_lconv_mon(lc);
                _free_crt(lc);
                _free_crt(lc_refcount);
                _free_crt(lconv_mon_refcount);
                return 1;
            }

            fix_grouping(lc->mon_grouping);
        }
        else {
            /*
             * C locale for monetary category (the numeric category fields,
             * which are NOT of the C locale, get fixed up below). Note
             * that __lconv_c is copied, rather than directly assigning
             * the fields of lc because of the uncertainty of the values of
             * the int_frac_digits,..., n_sign_posn fields (SCHAR_MAX or
             * UCHAR_MAX, depending on whether or a compliand was built
             * with -J.
             */
            *lc = __lconv_c;
        }

        /*
         * Copy the numeric locale fields from the old struct
         */
        lc->decimal_point = ploci->lconv->decimal_point;
        lc->thousands_sep = ploci->lconv->thousands_sep;
        lc->grouping = ploci->lconv->grouping;

        *lc_refcount = 1;
        if (lconv_mon_refcount)
            *lconv_mon_refcount = 1;
    }
    else {
        /*
         * C locale for BOTH monetary and numeric categories.
         */
        lconv_mon_refcount = NULL;
        lc_refcount = NULL;
        lc = &__lconv_c;           /* point to new one */

    }

    if ( (ploci->lconv_mon_refcount != NULL) &&
         (InterlockedDecrement(ploci->lconv_mon_refcount) == 0))
    {
        _ASSERTE(ploci->lconv_mon_refcount > 0);
    }
    if ( (ploci->lconv_intl_refcount != NULL) &&
         (InterlockedDecrement(ploci->lconv_intl_refcount) == 0))
    {
        _free_crt(ploci->lconv);
        _free_crt(ploci->lconv_intl_refcount);
    }
    ploci->lconv_mon_refcount = lconv_mon_refcount;
    ploci->lconv_intl_refcount = lc_refcount;
    ploci->lconv = lc;                       /* point to new one */

    return 0;
}

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


/*
 *  Free the lconv monetary strings.
 *  Numeric values do not need to be freed.
 */
void __cdecl __free_lconv_mon(
        struct lconv *l
        )
{
    if (l == NULL)
        return;

    if ( l->int_curr_symbol != __lconv_c.int_curr_symbol )
        _free_crt(l->int_curr_symbol);

    if ( l->currency_symbol != __lconv_c.currency_symbol )
        _free_crt(l->currency_symbol);

    if ( l->mon_decimal_point != __lconv_c.mon_decimal_point )
        _free_crt(l->mon_decimal_point);

    if ( l->mon_thousands_sep != __lconv_c.mon_thousands_sep )
        _free_crt(l->mon_thousands_sep);

    if ( l->mon_grouping != __lconv_c.mon_grouping )
        _free_crt(l->mon_grouping);

    if ( l->positive_sign != __lconv_c.positive_sign )
        _free_crt(l->positive_sign);

    if ( l->negative_sign != __lconv_c.negative_sign )
        _free_crt(l->negative_sign);
}
