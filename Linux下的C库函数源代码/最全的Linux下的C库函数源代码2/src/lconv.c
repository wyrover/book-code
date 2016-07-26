/***
*lconv.c - Contains the localeconv function
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Contains the localeconv() function.
*
*******************************************************************************/

#include <cruntime.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>

/* pointer to original static to avoid freeing */
char __lconv_static_decimal[] = ".";
char __lconv_static_null[] = "";

/* lconv settings for "C" locale */
struct lconv __lconv_c = {
                __lconv_static_decimal, /* decimal_point */
                __lconv_static_null,       /* thousands_sep */
                __lconv_static_null,       /* grouping */
                __lconv_static_null,       /* int_curr_symbol */
                __lconv_static_null,       /* currency_symbol */
                __lconv_static_null,       /* mon_decimal_point */
                __lconv_static_null,       /* mon_thousands_sep */
                __lconv_static_null,       /* mon_grouping */
                __lconv_static_null,       /* positive_sign */
                __lconv_static_null,       /* negative_sign */
                CHAR_MAX,                           /* int_frac_digits */
                CHAR_MAX,                           /* frac_digits */
                CHAR_MAX,                           /* p_cs_precedes */
                CHAR_MAX,                           /* p_sep_by_space */
                CHAR_MAX,                           /* n_cs_precedes */
                CHAR_MAX,                           /* n_sep_by_space */
                CHAR_MAX,                           /* p_sign_posn */
                CHAR_MAX                               /* n_sign_posn */
                };


/* pointer to current lconv structure */

struct lconv *__lconv = &__lconv_c;

/***
*struct lconv *localeconv(void) - Return the numeric formatting convention
*
*Purpose:
*       The localeconv() routine returns the numeric formatting conventions
*       for the current locale setting.  [ANSI]
*
*Entry:
*       void
*
*Exit:
*       struct lconv * = pointer to struct indicating current numeric
*                        formatting conventions.
*
*Exceptions:
*
*******************************************************************************/

struct lconv * __cdecl localeconv (
        void
        )
{
    /*
     * Note that we don't need _LocaleUpdate in this function.
     * The main reason being, that this is a leaf function in
     * locale usage terms.
     */
    _ptiddata ptd = _getptd();
    pthreadlocinfo ptloci = ptd->ptlocinfo;

    __UPDATE_LOCALE(ptd, ptloci);
    return(__lconv);
}
