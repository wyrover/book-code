/***
*nlsdata2.c - globals for international library - locale handles and code page
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module defines the locale handles and code page.  The handles are
*       required by almost all locale dependent functions.  This module is
*       separated from nlsdatax.c for granularity.
*
*******************************************************************************/

#include <locale.h>
#include <setlocal.h>
#include <mbctype.h>

char __clocalestr[] = "C";
extern struct __lc_time_data __lc_time_c;       /* defined in strftime.c */

/*
 * initial locale information struct, set to the C locale. Used only until the
 * first call to setlocale()
 */
threadlocinfo __initiallocinfo = {
    1,                                        /* refcount                 */
    _CLOCALECP,                               /* lc_codepage              */
    _CLOCALECP,                               /* lc_collate_cp            */
    {   _CLOCALEHANDLE,                       /* lc_handle[_ALL]          */
        _CLOCALEHANDLE,                       /* lc_handle[_COLLATE]      */
        _CLOCALEHANDLE,                       /* lc_handle[_CTYPE]        */
        _CLOCALEHANDLE,                       /* lc_handle[_MONETARY]     */
        _CLOCALEHANDLE,                       /* lc_handle[_NUMERIC]      */
        _CLOCALEHANDLE                        /* lc_handle[_TIME]         */
    },
    {   {0, 0, 0},                            /* lc_id[LC_ALL]            */
        {0, 0, 0},                            /* lc_id[LC_COLLATE]        */
        {0, 0, 0},                            /* lc_id[LC_CTYPE]          */
        {0, 0, 0},                            /* lc_id[LC_MONETARY]       */
        {0, 0, 0},                            /* lc_id[LC_NUMERIC]        */
        {0, 0, 0}                             /* lc_id[LC_TIME]           */
    },
    {   {NULL, NULL, NULL, NULL},             /* lc_category[LC_ALL]      */
        {__clocalestr, NULL, NULL, NULL},     /* lc_category[LC_COLLATE]  */
        {__clocalestr, NULL, NULL, NULL},     /* lc_category[LC_CTYPE]    */
        {__clocalestr, NULL, NULL, NULL},     /* lc_category[LC_MONETARY] */
        {__clocalestr, NULL, NULL, NULL},     /* lc_category[LC_NUMERIC]  */
        {__clocalestr, NULL, NULL, NULL}      /* lc_category[LC_TIME]     */
    },
    1,                                        /* lc_clike                 */
    1,                                        /* mb_cur_max               */
    NULL,                                     /* lconv_intl_refcount      */
    NULL,                                     /* lconv_num_refcount       */
    NULL,                                     /* lconv_mon_refcount       */
    &__lconv_c,                               /* lconv                    */
    NULL,                                     /* ctype1_refcount          */
    NULL,                                     /* ctype1                   */
    __newctype + 128,                         /* pctype                   */
    __newclmap + 128,                         /* pclmap                   */
    __newcumap + 128,                         /* pcumap                   */
    &__lc_time_c,                             /* lc_time_curr             */
};

/*
 * global pointer to the current per-thread locale information structure.
 */
pthreadlocinfo __ptlocinfo = &__initiallocinfo;

/*
 *  Locale handles.
 */
LCID __lc_handle[] =
    {   _CLOCALEHANDLE,                       /* lc_handle[_ALL]          */
        _CLOCALEHANDLE,                       /* lc_handle[_COLLATE]      */
        _CLOCALEHANDLE,                       /* lc_handle[_CTYPE]        */
        _CLOCALEHANDLE,                       /* lc_handle[_MONETARY]     */
        _CLOCALEHANDLE,                       /* lc_handle[_NUMERIC]      */
        _CLOCALEHANDLE                        /* lc_handle[_TIME]         */
    };

/*
 *  Code page.
 */
UINT __lc_codepage = _CLOCALECP;                /* CP_ACP */

/*
 * Code page for LC_COLLATE
 */
UINT __lc_collate_cp = _CLOCALECP;

/* if this locale has first 127 character set same as CLOCALE.
 */
int __lc_clike = 1;

_locale_tstruct __initiallocalestructinfo =
{
    &__initiallocinfo,
    &__initialmbcinfo
};
