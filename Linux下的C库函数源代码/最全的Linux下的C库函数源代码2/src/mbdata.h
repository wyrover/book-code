/***
*mbdata.h - MBCS lib data
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines data for use when building MBCS libs and routines
*
*       [Internal].
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifndef _INC_MBDATA
#define _INC_MBDATA

#include <crtdefs.h>

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if defined (_WIN32)

#define NUM_ULINFO 6 /* multibyte full-width-latin upper/lower info */

#else  /* defined (_WIN32) */

#define NUM_ULINFO 12 /* multibyte full-width-latin upper/lower info */

#endif  /* defined (_WIN32) */

#ifndef _THREADMBCINFO
typedef struct threadmbcinfostruct {
        int refcount;
        int mbcodepage;
        int ismbcodepage;
        int mblcid;
        unsigned short mbulinfo[6];
        unsigned char mbctype[257];
        unsigned char mbcasemap[256];
} threadmbcinfo;
#define _THREADMBCINFO
#endif  /* _THREADMBCINFO */

#ifndef _TAGLC_ID_DEFINED
typedef struct tagLC_ID {
        unsigned short wLanguage;
        unsigned short wCountry;
        unsigned short wCodePage;
} LC_ID, *LPLC_ID;
#define _TAGLC_ID_DEFINED
#endif  /* _TAGLC_ID_DEFINED */

#ifndef _THREADLOCALEINFO
typedef struct threadlocaleinfostruct {
        int refcount;
        unsigned int lc_codepage;
        unsigned int lc_collate_cp;
        unsigned long lc_handle[6]; /* LCID */
        LC_ID lc_id[6];
        struct {
            char *locale;
            wchar_t *wlocale;
            int *refcount;
            int *wrefcount;
        } lc_category[6];
        int lc_clike;
        int mb_cur_max;
        int * lconv_intl_refcount;
        int * lconv_num_refcount;
        int * lconv_mon_refcount;
        struct lconv * lconv;
        int * ctype1_refcount;
        unsigned short * ctype1;
        const unsigned short * pctype;
        const unsigned char * pclmap;
        const unsigned char * pcumap;
        struct __lc_time_data * lc_time_curr;
} threadlocinfo;
#define _THREADLOCALEINFO
#endif  /* _THREADLOCALEINFO */

/* global variable to indicate current code page */
extern int __mbcodepage;

/* global flag indicating if the current code page is a multibyte code page */
extern int __ismbcodepage;

#if defined (_WIN32)
/* global variable to indicate current LCID */
extern int __mblcid;
#endif  /* defined (_WIN32) */

/* global variable to indicate current full-width-latin upper/lower info */
extern unsigned short __mbulinfo[NUM_ULINFO];

/* global variable pointing to the current mbc information structure */
extern pthreadmbcinfo __ptmbcinfo;
/* function to update mbc info used by the current thread */
pthreadmbcinfo __cdecl __updatetmbcinfo(void);

/*
 * MBCS - Multi-Byte Character Set
 */

/*
 * general use macros for model dependent/independent versions.
 */

#define _ISMBCP     (__ismbcodepage != 0)
#define _ISNOTMBCP  (__ismbcodepage == 0)

#define _ISMBCP_MT(p)       (p->ismbcodepage != 0)
#define _ISNOTMBCP_MT(p)    (p->ismbcodepage == 0)

#define _ismbbtruelead(_lb,_ch) (!(_lb) && _ismbblead((_ch)))

/* internal use macros since tolower/toupper are locale-dependent */
#define _mbbisupper(_c) ((_mbctype[(_c) + 1] & _SBUP) == _SBUP)
#define _mbbislower(_c) ((_mbctype[(_c) + 1] & _SBLOW) == _SBLOW)

#define _mbbtolower(_c) (_mbbisupper(_c) ? _mbcasemap[_c] : _c)
#define _mbbtoupper(_c) (_mbbislower(_c) ? _mbcasemap[_c] : _c)

#define _ismbbtruelead_l(_lb,_ch,p)   (!(_lb) && _ismbblead_l((_ch), p))
#define _mbbisupper_l(_c, p)      ((p->mbcinfo->mbctype[(_c) + 1] & _SBUP) == _SBUP)
#define _mbbislower_l(_c, p)      ((p->mbcinfo->mbctype[(_c) + 1] & _SBLOW) == _SBLOW)
#define _mbbtolower_l(_c, p)      (_mbbisupper_l(_c, p) ? p->mbcinfo->mbcasemap[_c] : _c)
#define _mbbtoupper_l(_c, p)      (_mbbislower_l(_c, p) ? p->mbcinfo->mbcasemap[_c] : _c)

/* define full-width-latin upper/lower ranges */

#define _MBUPPERLOW1    __mbulinfo[0]
#define _MBUPPERHIGH1   __mbulinfo[1]
#define _MBCASEDIFF1    __mbulinfo[2]

#define _MBUPPERLOW2    __mbulinfo[3]
#define _MBUPPERHIGH2   __mbulinfo[4]
#define _MBCASEDIFF2    __mbulinfo[5]

#define _MBUPPERLOW1_MT(p)  p->mbcinfo->mbulinfo[0]
#define _MBUPPERHIGH1_MT(p) p->mbcinfo->mbulinfo[1]
#define _MBCASEDIFF1_MT(p)  p->mbcinfo->mbulinfo[2]

#define _MBUPPERLOW2_MT(p)  p->mbcinfo->mbulinfo[3]
#define _MBUPPERHIGH2_MT(p) p->mbcinfo->mbulinfo[4]
#define _MBCASEDIFF2_MT(p)  p->mbcinfo->mbulinfo[5]

#if !defined (_WIN32)

#define _MBLOWERLOW1    __mbulinfo[6]
#define _MBLOWERHIGH1   __mbulinfo[7]

#define _MBLOWERLOW2    __mbulinfo[8]
#define _MBLOWERHIGH2   __mbulinfo[9]

#define _MBDIGITLOW     __mbulinfo[10]
#define _MBDIGITHIGH    __mbulinfo[11]

#endif  /* !defined (_WIN32) */

/* Kanji-specific ranges */
#define _MBHIRALOW      0x829f  /* hiragana */
#define _MBHIRAHIGH     0x82f1

#define _MBKATALOW      0x8340  /* katakana */
#define _MBKATAHIGH     0x8396
#define _MBKATAEXCEPT   0x837f  /* exception */

#define _MBKIGOULOW     0x8141  /* kanji punctuation */
#define _MBKIGOUHIGH    0x81ac
#define _MBKIGOUEXCEPT  0x817f  /* exception */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_MBDATA */
