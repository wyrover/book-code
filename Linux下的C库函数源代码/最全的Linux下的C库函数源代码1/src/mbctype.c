/***
*mbctype.c - MBCS table used by the functions that test for types of char
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       table used to determine the type of char
*
*******************************************************************************/

#ifdef _MBCS

#include <windows.h>
#include <sect_attribs.h>
#include <cruntime.h>
#include <dbgint.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mtdll.h>
#include <stdlib.h>
#include <stdio.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>
#include <awint.h>
#include <rterr.h>

#ifndef CRTDLL

extern "C" int __cdecl __initmbctable(void);
/*
 * Flag to ensure multibyte ctype table is only initialized once
 */
extern "C" int __mbctype_initialized;

_CRTALLOC(".CRT$XIC") static _PIFV pinit = __initmbctable;

#endif  /* CRTDLL */

#define _CHINESE_SIMP_CP    936
#define _KOREAN_WANGSUNG_CP 949
#define _CHINESE_TRAD_CP    950
#define _KOREAN_JOHAB_CP    1361

#define NUM_CHARS 257 /* -1 through 255 */

#define NUM_CTYPES 4 /* table contains 4 types of info */
#define MAX_RANGES 8 /* max number of ranges needed given languages so far */

/* character type info in ranges (pair of low/high), zeros indicate end */
typedef struct
{
    int             code_page;
    unsigned short  mbulinfo[NUM_ULINFO];
    unsigned char   rgrange[NUM_CTYPES][MAX_RANGES];
} code_page_info;

extern "C"
{
threadmbcinfo __initialmbcinfo =
{
    0,
    _CLOCALECP, /* _MB_CP_ANSI */
    0,
    0,
    { 0, 0, 0, 0, 0, 0 },
    {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 /* rest is zero */
    },
    {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
    0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
    0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00 /* rest is zero */
    }
};

/* MBCS ctype array */
unsigned char _mbctype[NUM_CHARS] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00 /* rest is zero */
};
unsigned char _mbcasemap[256] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
    0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73,
    0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b,
    0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x00, 0x00, 0x00, 0x00, 0x00 /* rest is zero */
};

/* global variable to indicate current code page */
int __mbcodepage = _CLOCALECP; /* __initialmbcinfo.mbcodepage; */

/* global flag indicating if current code page is a multibyte code page */
int __ismbcodepage = 0; /*__initialmbcinfo.ismbcodepage; */

/* global variable to indicate current LCID */
int __mblcid = 0; /* __initialmbcinfo.mblcid; */

/* global variable to indicate current full-width-latin upper/lower info */
unsigned short __mbulinfo[NUM_ULINFO]; /* __initialmbcinfo.mbulinfo */

/* global pointer to the current per-thread mbc information structure. */
pthreadmbcinfo __ptmbcinfo = &__initialmbcinfo;
}

static int fSystemSet;

static char __rgctypeflag[NUM_CTYPES] = { _MS, _MP, _M1, _M2 };

static code_page_info __rgcode_page_info[] =
{
    {
      _KANJI_CP, /* Kanji (Japanese) Code Page */
      { 0x8260, 0x8279,   /* Full-Width Latin Upper Range 1 */
        0x8281 - 0x8260,  /* Full-Width Latin Case Difference 1 */

        0x0000, 0x0000,   /* Full-Width Latin Upper Range 2 */
        0x0000            /* Full-Width Latin Case Difference 2 */
#ifndef _WIN32
        ,
        0x8281, 0x829A,   /* Full-Width Latin Lower Range 1 */

        0x0000, 0x0000,   /* Full-Width Latin Lower Range 2 */

        0x824F, 0x8258    /* Full-Width Latin Digit Range */
#endif  /* _WIN32 */
      },
      {
        { 0xA6, 0xDF, 0,    0,    0,    0,    0, 0, }, /* Single Byte Ranges */
        { 0xA1, 0xA5, 0,    0,    0,    0,    0, 0, }, /* Punctuation Ranges */
        { 0x81, 0x9F, 0xE0, 0xFC, 0,    0,    0, 0, }, /* Lead Byte Ranges */
        { 0x40, 0x7E, 0x80, 0xFC, 0,    0,    0, 0, }, /* Trail Byte Ranges */
      }
    },
    {
      _CHINESE_SIMP_CP, /* Chinese Simplified (PRC) Code Page */
      { 0xA3C1, 0xA3DA,   /* Full-Width Latin Upper Range 1 */
        0xA3E1 - 0xA3C1,  /* Full-Width Latin Case Difference 1 */

        0x0000, 0x0000,   /* Full-Width Latin Upper Range 2 */
        0x0000            /* Full-Width Latin Case Difference 2 */
#ifndef _WIN32
        ,
        0xA3E1, 0xA3FA,   /* Full-Width Latin Lower Range 1 */

        0x0000, 0x0000,   /* Full-Width Latin Lower Range 2 */

        0xA3B0, 0xA3B9    /* Full-Width Latin Digit Range */
#endif  /* _WIN32 */
      },
      {
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Single Byte Ranges */
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Punctuation Ranges */
        { 0x81, 0xFE, 0,    0,    0,    0,    0, 0, }, /* Lead Byte Ranges */
        { 0x40, 0xFE, 0,    0,    0,    0,    0, 0, }, /* Trail Byte Ranges */
      }
    },
    {
      _KOREAN_WANGSUNG_CP, /* Wangsung (Korean) Code Page */
      { 0xA3C1, 0xA3DA,   /* Full-Width Latin Upper Range 1 */
        0xA3E1 - 0xA3C1,  /* Full-Width Latin Case Difference 1 */

        0x0000, 0x0000,   /* Full-Width Latin Upper Range 2 */
        0x0000            /* Full-Width Latin Case Difference 2 */
#ifndef _WIN32
        ,
        0xA3E1, 0xA3FA,   /* Full-Width Latin Lower Range 1 */

        0x0000, 0x0000,   /* Full-Width Latin Lower Range 2 */

        0xA3B0, 0xA3B9    /* Full-Width Latin Digit Range */
#endif  /* _WIN32 */
      },
      {
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Single Byte Ranges */
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Punctuation Ranges */
        { 0x81, 0xFE, 0,    0,    0,    0,    0, 0, }, /* Lead Byte Ranges */
        { 0x41, 0xFE, 0,    0,    0,    0,    0, 0, }, /* Trail Byte Ranges */
      }
    },
    {
      _CHINESE_TRAD_CP, /* Chinese Traditional (Taiwan) Code Page */
      { 0xA2CF, 0xA2E4,   /* Full-Width Latin Upper Range 1 */
        0xA2E9 - 0xA2CF,  /* Full-Width Latin Case Difference 1 */

        0xA2E5, 0xA2E8,   /* Full-Width Latin Upper Range 2 */
        0xA340 - 0XA2E5   /* Full-Width Latin Case Difference 2 */
#ifndef _WIN32
        ,
        0xA2E9, 0xA2FE,   /* Full-Width Latin Lower Range 1 */

        0xA340, 0xA343,   /* Full-Width Latin Lower Range 2 */

        0xA2AF, 0xA2B8    /* Full-Width Latin Digit Range */
#endif  /* _WIN32 */
      },
      {
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Single Byte Ranges */
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Punctuation Ranges */
        { 0x81, 0xFE, 0,    0,    0,    0,    0, 0, }, /* Lead Byte Ranges */
        { 0x40, 0x7E, 0xA1, 0xFE, 0,    0,    0, 0, }, /* Trail Byte Ranges */
      }
    },
    {
      _KOREAN_JOHAB_CP, /* Johab (Korean) Code Page */
      { 0xDA51, 0xDA5E,   /* Full-Width Latin Upper Range 1 */
        0xDA71 - 0xDA51,  /* Full-Width Latin Case Difference 1 */

        0xDA5F, 0xDA6A,   /* Full-Width Latin Upper Range 2 */
        0xDA91 - 0xDA5F   /* Full-Width Latin Case Difference 2 */
#ifndef _WIN32
        ,
        0xDA71, 0xDA7E,   /* Full-Width Latin Lower Range 1 */

        0xDA91, 0xDA9C,   /* Full-Width Latin Lower Range 2 */

        0xDA40, 0xDA49    /* Full-Width Latin Digit Range */
#endif  /* _WIN32 */
      },
      {
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Single Byte Ranges */
        { 0,    0,    0,    0,    0,    0,    0, 0, }, /* Punctuation Ranges */
        { 0x81, 0xD3, 0xD8, 0xDE, 0xE0, 0xF9, 0, 0, }, /* Lead Byte Ranges */
        { 0x31, 0x7E, 0x81, 0xFE, 0,    0,    0, 0, }, /* Trail Byte Ranges */
      }
    }
};

extern "C" int __cdecl _setmbcp_nolock(int, pthreadmbcinfo);

/***
*getSystemCP - Get system default CP if requested.
*
*Purpose:
*       Get system default CP if requested.
*
*Entry:
*       codepage - user requested code page/world script
*Exit:
*       requested code page
*
*Exceptions:
*
*******************************************************************************/

static int getSystemCP (int codepage)
{
    _locale_t plocinfo = NULL;
    _LocaleUpdate _loc_update(plocinfo);
    fSystemSet = 0;

    /* get system code page values if requested */

    if (codepage == _MB_CP_OEM)
    {
        fSystemSet = 1;
        return GetOEMCP();
    }

    else if (codepage == _MB_CP_ANSI)
    {
        fSystemSet = 1;
        return GetACP();
    }

    else
    if (codepage == _MB_CP_LOCALE)
    {
        fSystemSet = 1;
        return __LC_CODEPAGE(_loc_update.GetLocaleT()->locinfo);
    }

    return codepage;
}


/***
*CPtoLCID() - Code page to LCID.
*
*Purpose:
*       Some API calls want an LCID, so convert MB CP to appropriate LCID,
*       and then API converts back to ANSI CP for that LCID.
*
*Entry:
*   codepage - code page to convert
*Exit:
*       returns appropriate LCID
*
*Exceptions:
*
*******************************************************************************/

static int CPtoLCID (int codepage)
{
    switch (codepage) {
    case 932:
        return MAKELCID(MAKELANGID(LANG_JAPANESE,SUBLANG_DEFAULT),
                        SORT_DEFAULT);
    case 936:
        return MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),
                        SORT_DEFAULT);
    case 949:
        return MAKELCID(MAKELANGID(LANG_KOREAN,SUBLANG_DEFAULT),
                        SORT_DEFAULT);
    case 950:
        return MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL),
                        SORT_DEFAULT);
    }

    return 0;
}


/***
*setSBCS() - Set MB code page to SBCS.
*
*Purpose:
*           Set MB code page to SBCS.
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

static void setSBCS (pthreadmbcinfo ptmbci)
{
    int i;

    /* set for single-byte code page */
    for (i = 0; i < NUM_CHARS; i++)
        ptmbci->mbctype[i] = 0;

    /* code page has changed, set global flag */
    ptmbci->mbcodepage = 0;

    /* clear flag to indicate single-byte code */
    ptmbci->ismbcodepage = 0;

    ptmbci->mblcid = 0;

    for (i = 0; i < NUM_ULINFO; i++)
        ptmbci->mbulinfo[i] = 0;

    for ( i = 0 ; i < 257 ; i++ )
        ptmbci->mbctype[i] = __initialmbcinfo.mbctype[i];

    for ( i = 0 ; i < 256 ; i++ )
        ptmbci->mbcasemap[i] = __initialmbcinfo.mbcasemap[i];
}

/***
*setSBUpLow() - Set single byte upper/lower mappings
*
*Purpose:
*           Set single byte mapping for tolower/toupper.
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

static void setSBUpLow (pthreadmbcinfo ptmbci)
{
    BYTE *  pbPair;
    UINT    ich;
    CPINFO  cpinfo;
    UCHAR   sbVector[256];
    UCHAR   upVector[256];
    UCHAR   lowVector[256];
    USHORT  wVector[256];

    //    test if codepage exists
    if (GetCPInfo(ptmbci->mbcodepage, &cpinfo) != 0)
    {
        //  if so, create vector 0-255
        for (ich = 0; ich < 256; ich++)
            sbVector[ich] = (UCHAR) ich;

        //  set byte 0 and any leading byte value to non-alpha char ' '
        sbVector[0] = (UCHAR)' ';
        for (pbPair = &cpinfo.LeadByte[0]; *pbPair; pbPair += 2)
            for (ich = *pbPair; ich <= *(pbPair + 1); ich++)
                sbVector[ich] = (UCHAR)' ';

        //  get char type for character vector

        __crtGetStringTypeA(NULL, CT_CTYPE1, (LPCSTR)sbVector, 256, wVector,
                            ptmbci->mbcodepage, ptmbci->mblcid, FALSE);

        //  get lower case mappings for character vector

        __crtLCMapStringA(NULL, ptmbci->mblcid, LCMAP_LOWERCASE, (LPCSTR)sbVector, 256,
                                    (LPSTR)lowVector, 256, ptmbci->mbcodepage, FALSE);

        //  get upper case mappings for character vector

        __crtLCMapStringA(NULL, ptmbci->mblcid, LCMAP_UPPERCASE, (LPCSTR)sbVector, 256,
                                    (LPSTR)upVector, 256, ptmbci->mbcodepage, FALSE);

        //  set _SBUP, _SBLOW in ptmbci->mbctype if type is upper. lower
        //  set mapping array with lower or upper mapping value

        for (ich = 0; ich < 256; ich++)
            if (wVector[ich] & _UPPER)
            {
                ptmbci->mbctype[ich + 1] |= _SBUP;
                ptmbci->mbcasemap[ich] = lowVector[ich];
            }
            else if (wVector[ich] & _LOWER)
            {
                ptmbci->mbctype[ich + 1] |= _SBLOW;
                ptmbci->mbcasemap[ich] = upVector[ich];
            }
            else
                ptmbci->mbcasemap[ich] = 0;
    }
    else
    {
        //  if no codepage, set 'A'-'Z' as upper, 'a'-'z' as lower

        for (ich = 0; ich < 256; ich++)
            if (ich >= (UINT)'A' && ich <= (UINT)'Z')
            {
                ptmbci->mbctype[ich + 1] |= _SBUP;
                ptmbci->mbcasemap[ich] = ich + ('a' - 'A');
            }
            else if (ich >= (UINT)'a' && ich <= (UINT)'z')
            {
                ptmbci->mbctype[ich + 1] |= _SBLOW;
                ptmbci->mbcasemap[ich] = ich - ('a' - 'A');
            }
            else
                ptmbci->mbcasemap[ich] = 0;
    }
}


/***
*__updatetmbcinfo() - refresh the thread's mbc info
*
*Purpose:
*       Update the current thread's reference to the multibyte character
*       information to match the current global mbc info. Decrement the
*       reference on the old mbc information struct and if this count is now
*       zero (so that no threads are using it), free it.
*
*Entry:
*
*Exit:
*       _getptd()->ptmbcinfo == __ptmbcinfo
*
*Exceptions:
*
*******************************************************************************/

extern "C" pthreadmbcinfo __cdecl __updatetmbcinfo(void)
{
        pthreadmbcinfo ptmbci;

        _ptiddata ptd = _getptd();
        if (!(ptd->_ownlocale & __globallocalestatus)|| !ptd->ptlocinfo) {
        _mlock(_MB_CP_LOCK);

        __try
        {

            if ( (ptmbci = ptd->ptmbcinfo) != __ptmbcinfo )
            {
                /*
                 * Decrement the reference count in the old mbc info structure
                 * and free it, if necessary
                 */
                    if ( (ptmbci != NULL) && (InterlockedDecrement((volatile LONG *)&(ptmbci->refcount)) == 0) && ptmbci != &__initialmbcinfo )
                {
                    /*
                     * Free it
                     */
                    _free_crt(ptmbci);
                }

                /*
                 * Point to the current mbc info structure and increment its
                 * reference count.
                 */
                ptmbci = ptd->ptmbcinfo = __ptmbcinfo;
                    InterlockedIncrement((volatile LONG *)&(ptmbci->refcount));
            }
        }
        __finally
        {
            _munlock(_MB_CP_LOCK);
        }
        } else {
            ptmbci = ptd->ptmbcinfo;
        }

        if(!ptmbci)
        {
            _amsg_exit(_RT_LOCALE);
        }

        return ptmbci;
}


/***
*_setmbcp() - Set MBC data based on code page
*
*Purpose:
*       Init MBC character type tables based on code page number. If
*       given code page is supported, load that code page info into
*       mbctype table. If not, query OS to find the information,
*       otherwise set up table with single byte info.
*
*       Multithread Notes: First, allocate an mbc information struct. Set the
*       mbc info in the static vars and arrays as does the single-thread
*       version. Then, copy this info into the new allocated struct and set
*       the current mbc info pointer (__ptmbcinfo) to point to it.
*
*Entry:
*       codepage - code page to initialize MBC table
*           _MB_CP_OEM = use system OEM code page
*           _MB_CP_ANSI = use system ANSI code page
*           _MB_CP_SBCS = set to single byte 'code page'
*
*Exit:
*        0 = Success
*       -1 = Error, code page not changed.
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _setmbcp (int codepage)
{
    int retcode = -1;           /* initialize to failure */
    pthreadmbcinfo ptmbci;
    int i;

    _ptiddata ptd = _getptd();

    __updatetmbcinfo();
    ptmbci = ptd->ptmbcinfo;


    codepage = getSystemCP(codepage);

    if ( codepage != ptmbci->mbcodepage )
    {
        /*
         * Always allocate space so that we don't have to take lock
         * for any update.
         */
        ptmbci = (pthreadmbcinfo)_malloc_crt( sizeof(threadmbcinfo) );

        if (ptmbci != NULL)
        {
            *ptmbci = *(ptd->ptmbcinfo);
            /*
             * Note that refcount for this structure is just one so
             * update the refcount.
             */
            /*
             * Note that the refcount is set to zero because we don't count this
             * refcount as we won't be calling InterlockedDecrement for this when
             * the variable goes out of scope.
             */
            ptmbci->refcount = 0;

            /*
             * Install the codepage and copy the info into the struct
             */
            if ( (retcode = _setmbcp_nolock(codepage, ptmbci)) == 0)
            {
                if (InterlockedDecrement((volatile LONG *)&ptd->ptmbcinfo->refcount) == 0 && ptd->ptmbcinfo != &__initialmbcinfo)
                    _free_crt(ptd->ptmbcinfo);
                /*
                 * Assign and increment the refcount of this structure.
                 */
                ptd->ptmbcinfo = ptmbci;
                InterlockedIncrement((volatile LONG *)&(ptd->ptmbcinfo->refcount));
                if (!(ptd->_ownlocale & _PER_THREAD_LOCALE_BIT) &&
                        !(__globallocalestatus & _GLOBAL_LOCALE_BIT)) {

                    _mlock(_MB_CP_LOCK);
                    __try
                    {
                        /*
                         * Fill in the mbc info struct
                         */
                        __mbcodepage = ptmbci->mbcodepage;
                        __ismbcodepage = ptmbci->ismbcodepage;
                        __mblcid = ptmbci->mblcid;
                        for ( i = 0 ; i < 5 ; i++ )
                            __mbulinfo[i] = ptmbci->mbulinfo[i];
                        for ( i = 0 ; i < 257 ; i++ )
                            _mbctype[i] = ptmbci->mbctype[i];
                        for ( i = 0 ; i < 256 ; i++ )
                            _mbcasemap[i] = ptmbci->mbcasemap[i];

                        if (InterlockedDecrement((volatile LONG *)&__ptmbcinfo->refcount) == 0 && __ptmbcinfo != &__initialmbcinfo)
                            _free_crt(__ptmbcinfo);
                        /*
                         * Update __ptmbcinfo
                         */
                        __ptmbcinfo = ptmbci;
                        InterlockedIncrement((volatile LONG *)&ptmbci->refcount);
                    }
                    __finally
                    {
                        _munlock(_MB_CP_LOCK);
                    }
                }
            }
            else if (retcode == -1)
            {
                /*
                 * Free up the newly malloc-ed struct (note: a free of
                 * NULL is legal)
                 */
                if (ptmbci != &__initialmbcinfo)
                    _free_crt(ptmbci);
                errno = EINVAL;
                /* note that if malloc fails, it sets errno to ENOMEM already */
            }
        }

    }
    else
        /*
         * Not a new codepage after all. Nothing to do but return
         * success.
         */
        retcode = 0;

#pragma warning(disable:4390)
    if ( (retcode == -1) && (__ptmbcinfo == NULL) )
        /*
         * Fatal error!
         */
        ;

    return retcode;
}

extern "C" int __cdecl _setmbcp_nolock(int codepage, pthreadmbcinfo ptmbci)
{
        unsigned int icp;
        unsigned int irg;
        unsigned int ich;
        unsigned char *rgptr;
        CPINFO cpinfo;

        codepage = getSystemCP(codepage);

        /* user wants 'single-byte' MB code page */
        if (codepage == _MB_CP_SBCS)
        {
            setSBCS(ptmbci);
            return 0;
        }

        /* check for CRT code page info */
        for (icp = 0;
            icp < (sizeof(__rgcode_page_info) / sizeof(code_page_info));
            icp++)
        {
            /* see if we have info for this code page */
            if (__rgcode_page_info[icp].code_page == codepage)
            {
                /* clear the table */
                for (ich = 0; ich < NUM_CHARS; ich++)
                    ptmbci->mbctype[ich] = 0;

                /* for each type of info, load table */
                for (irg = 0; irg < NUM_CTYPES; irg++)
                {
                    /* go through all the ranges for each type of info */
                    for (rgptr = (unsigned char *)__rgcode_page_info[icp].rgrange[irg];
                        rgptr[0] && rgptr[1];
                        rgptr += 2)
                    {
                        /* set the type for every character in range */
                        for (ich = rgptr[0]; ich <= rgptr[1]; ich++)
                            ptmbci->mbctype[ich + 1] |= __rgctypeflag[irg];
                    }
                }
                /* code page has changed */
                ptmbci->mbcodepage = codepage;
                /* all the code pages we keep info for are truly multibyte */
                ptmbci->ismbcodepage = 1;
                ptmbci->mblcid = CPtoLCID(ptmbci->mbcodepage);
                for (irg = 0; irg < NUM_ULINFO; irg++)
                    ptmbci->mbulinfo[irg] = __rgcode_page_info[icp].mbulinfo[irg];

                /* return success */
                setSBUpLow(ptmbci);
                return 0;
            }
        }


        /* code page not supported by CRT, try the OS */
        if (GetCPInfo(codepage, &cpinfo) != 0) {
            BYTE *lbptr;

            /* clear the table */
            for (ich = 0; ich < NUM_CHARS; ich++)
                ptmbci->mbctype[ich] = 0;

            ptmbci->mbcodepage = codepage;
            ptmbci->mblcid = 0;

            if (cpinfo.MaxCharSize > 1)
            {
                /* LeadByte range always terminated by two 0's */
                for (lbptr = cpinfo.LeadByte; *lbptr && *(lbptr + 1); lbptr += 2)
                {
                    for (ich = *lbptr; ich <= *(lbptr + 1); ich++)
                        ptmbci->mbctype[ich + 1] |= _M1;
                }

                /* All chars > 1 must be considered valid trail bytes */
                for (ich = 0x01; ich < 0xFF; ich++)
                    ptmbci->mbctype[ich + 1] |= _M2;

                /* code page has changed */
                ptmbci->mblcid = CPtoLCID(ptmbci->mbcodepage);

                /* really a multibyte code page */
                ptmbci->ismbcodepage = 1;
            }
            else
                /* single-byte code page */
                ptmbci->ismbcodepage = 0;

            for (irg = 0; irg < NUM_ULINFO; irg++)
                ptmbci->mbulinfo[irg] = 0;

            setSBUpLow(ptmbci);
            /* return success */
            return 0;
        }


        /* If system default call, don't fail - set to SBCS */
        if (fSystemSet)
        {
            setSBCS(ptmbci);
            return 0;
        }

        /* return failure, code page not changed */
        return -1;
}

/***
*_getmbcp() - Get the current MBC code page
*
*Purpose:
*           Get code page value.
*Entry:
*       none.
*Exit:
*           return current MB codepage value.
*
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl _getmbcp (void)
{
    _locale_t plocinfo = NULL;
    _LocaleUpdate _loc_update(plocinfo);
    if ( _loc_update.GetLocaleT()->mbcinfo->ismbcodepage )
        return _loc_update.GetLocaleT()->mbcinfo->mbcodepage;
    else
        return 0;
}


/***
*_initmbctable() - Set MB ctype table to initial default value.
*
*Purpose:
*       Initialization.
*Entry:
*       none.
*Exit:
*       Returns 0 to indicate no error.
*Exceptions:
*
*******************************************************************************/

extern "C" int __cdecl __initmbctable (void)
{
#ifdef CRTDLL

        _setmbcp(_MB_CP_ANSI);

#else  /* CRTDLL */

        /*
         * Ensure we only initialize _mbctype[] once
         */
        if ( __mbctype_initialized == 0 ) {
            _setmbcp(_MB_CP_ANSI);
            __mbctype_initialized = 1;
        }

#endif  /* CRTDLL */

        return 0;
}

#endif  /* _MBCS */


/************************ Code Page info from NT/Win95 ********************


*** Code Page 932 ***

0x824f  ;Fullwidth Digit Zero
0x8250  ;Fullwidth Digit One
0x8251  ;Fullwidth Digit Two
0x8252  ;Fullwidth Digit Three
0x8253  ;Fullwidth Digit Four
0x8254  ;Fullwidth Digit Five
0x8255  ;Fullwidth Digit Six
0x8256  ;Fullwidth Digit Seven
0x8257  ;Fullwidth Digit Eight
0x8258  ;Fullwidth Digit Nine

0x8281  0x8260  ;Fullwidth Small A -> Fullwidth Capital A
0x8282  0x8261  ;Fullwidth Small B -> Fullwidth Capital B
0x8283  0x8262  ;Fullwidth Small C -> Fullwidth Capital C
0x8284  0x8263  ;Fullwidth Small D -> Fullwidth Capital D
0x8285  0x8264  ;Fullwidth Small E -> Fullwidth Capital E
0x8286  0x8265  ;Fullwidth Small F -> Fullwidth Capital F
0x8287  0x8266  ;Fullwidth Small G -> Fullwidth Capital G
0x8288  0x8267  ;Fullwidth Small H -> Fullwidth Capital H
0x8289  0x8268  ;Fullwidth Small I -> Fullwidth Capital I
0x828a  0x8269  ;Fullwidth Small J -> Fullwidth Capital J
0x828b  0x826a  ;Fullwidth Small K -> Fullwidth Capital K
0x828c  0x826b  ;Fullwidth Small L -> Fullwidth Capital L
0x828d  0x826c  ;Fullwidth Small M -> Fullwidth Capital M
0x828e  0x826d  ;Fullwidth Small N -> Fullwidth Capital N
0x828f  0x826e  ;Fullwidth Small O -> Fullwidth Capital O
0x8290  0x826f  ;Fullwidth Small P -> Fullwidth Capital P
0x8291  0x8270  ;Fullwidth Small Q -> Fullwidth Capital Q
0x8292  0x8271  ;Fullwidth Small R -> Fullwidth Capital R
0x8293  0x8272  ;Fullwidth Small S -> Fullwidth Capital S
0x8294  0x8273  ;Fullwidth Small T -> Fullwidth Capital T
0x8295  0x8274  ;Fullwidth Small U -> Fullwidth Capital U
0x8296  0x8275  ;Fullwidth Small V -> Fullwidth Capital V
0x8297  0x8276  ;Fullwidth Small W -> Fullwidth Capital W
0x8298  0x8277  ;Fullwidth Small X -> Fullwidth Capital X
0x8299  0x8278  ;Fullwidth Small Y -> Fullwidth Capital Y
0x829a  0x8279  ;Fullwidth Small Z -> Fullwidth Capital Z


*** Code Page 936 ***

0xa3b0  ;Fullwidth Digit Zero
0xa3b1  ;Fullwidth Digit One
0xa3b2  ;Fullwidth Digit Two
0xa3b3  ;Fullwidth Digit Three
0xa3b4  ;Fullwidth Digit Four
0xa3b5  ;Fullwidth Digit Five
0xa3b6  ;Fullwidth Digit Six
0xa3b7  ;Fullwidth Digit Seven
0xa3b8  ;Fullwidth Digit Eight
0xa3b9  ;Fullwidth Digit Nine

0xa3e1  0xa3c1  ;Fullwidth Small A -> Fullwidth Capital A
0xa3e2  0xa3c2  ;Fullwidth Small B -> Fullwidth Capital B
0xa3e3  0xa3c3  ;Fullwidth Small C -> Fullwidth Capital C
0xa3e4  0xa3c4  ;Fullwidth Small D -> Fullwidth Capital D
0xa3e5  0xa3c5  ;Fullwidth Small E -> Fullwidth Capital E
0xa3e6  0xa3c6  ;Fullwidth Small F -> Fullwidth Capital F
0xa3e7  0xa3c7  ;Fullwidth Small G -> Fullwidth Capital G
0xa3e8  0xa3c8  ;Fullwidth Small H -> Fullwidth Capital H
0xa3e9  0xa3c9  ;Fullwidth Small I -> Fullwidth Capital I
0xa3ea  0xa3ca  ;Fullwidth Small J -> Fullwidth Capital J
0xa3eb  0xa3cb  ;Fullwidth Small K -> Fullwidth Capital K
0xa3ec  0xa3cc  ;Fullwidth Small L -> Fullwidth Capital L
0xa3ed  0xa3cd  ;Fullwidth Small M -> Fullwidth Capital M
0xa3ee  0xa3ce  ;Fullwidth Small N -> Fullwidth Capital N
0xa3ef  0xa3cf  ;Fullwidth Small O -> Fullwidth Capital O
0xa3f0  0xa3d0  ;Fullwidth Small P -> Fullwidth Capital P
0xa3f1  0xa3d1  ;Fullwidth Small Q -> Fullwidth Capital Q
0xa3f2  0xa3d2  ;Fullwidth Small R -> Fullwidth Capital R
0xa3f3  0xa3d3  ;Fullwidth Small S -> Fullwidth Capital S
0xa3f4  0xa3d4  ;Fullwidth Small T -> Fullwidth Capital T
0xa3f5  0xa3d5  ;Fullwidth Small U -> Fullwidth Capital U
0xa3f6  0xa3d6  ;Fullwidth Small V -> Fullwidth Capital V
0xa3f7  0xa3d7  ;Fullwidth Small W -> Fullwidth Capital W
0xa3f8  0xa3d8  ;Fullwidth Small X -> Fullwidth Capital X
0xa3f9  0xa3d9  ;Fullwidth Small Y -> Fullwidth Capital Y
0xa3fa  0xa3da  ;Fullwidth Small Z -> Fullwidth Capital Z


*** Code Page 949 ***

0xa3b0  ;Fullwidth Digit Zero
0xa3b1  ;Fullwidth Digit One
0xa3b2  ;Fullwidth Digit Two
0xa3b3  ;Fullwidth Digit Three
0xa3b4  ;Fullwidth Digit Four
0xa3b5  ;Fullwidth Digit Five
0xa3b6  ;Fullwidth Digit Six
0xa3b7  ;Fullwidth Digit Seven
0xa3b8  ;Fullwidth Digit Eight
0xa3b9  ;Fullwidth Digit Nine

0xa3e1  0xa3c1  ;Fullwidth Small A -> Fullwidth Capital A
0xa3e2  0xa3c2  ;Fullwidth Small B -> Fullwidth Capital B
0xa3e3  0xa3c3  ;Fullwidth Small C -> Fullwidth Capital C
0xa3e4  0xa3c4  ;Fullwidth Small D -> Fullwidth Capital D
0xa3e5  0xa3c5  ;Fullwidth Small E -> Fullwidth Capital E
0xa3e6  0xa3c6  ;Fullwidth Small F -> Fullwidth Capital F
0xa3e7  0xa3c7  ;Fullwidth Small G -> Fullwidth Capital G
0xa3e8  0xa3c8  ;Fullwidth Small H -> Fullwidth Capital H
0xa3e9  0xa3c9  ;Fullwidth Small I -> Fullwidth Capital I
0xa3ea  0xa3ca  ;Fullwidth Small J -> Fullwidth Capital J
0xa3eb  0xa3cb  ;Fullwidth Small K -> Fullwidth Capital K
0xa3ec  0xa3cc  ;Fullwidth Small L -> Fullwidth Capital L
0xa3ed  0xa3cd  ;Fullwidth Small M -> Fullwidth Capital M
0xa3ee  0xa3ce  ;Fullwidth Small N -> Fullwidth Capital N
0xa3ef  0xa3cf  ;Fullwidth Small O -> Fullwidth Capital O
0xa3f0  0xa3d0  ;Fullwidth Small P -> Fullwidth Capital P
0xa3f1  0xa3d1  ;Fullwidth Small Q -> Fullwidth Capital Q
0xa3f2  0xa3d2  ;Fullwidth Small R -> Fullwidth Capital R
0xa3f3  0xa3d3  ;Fullwidth Small S -> Fullwidth Capital S
0xa3f4  0xa3d4  ;Fullwidth Small T -> Fullwidth Capital T
0xa3f5  0xa3d5  ;Fullwidth Small U -> Fullwidth Capital U
0xa3f6  0xa3d6  ;Fullwidth Small V -> Fullwidth Capital V
0xa3f7  0xa3d7  ;Fullwidth Small W -> Fullwidth Capital W
0xa3f8  0xa3d8  ;Fullwidth Small X -> Fullwidth Capital X
0xa3f9  0xa3d9  ;Fullwidth Small Y -> Fullwidth Capital Y
0xa3fa  0xa3da  ;Fullwidth Small Z -> Fullwidth Capital Z


*** Code Page 950 ***

0xa2af  ;Fullwidth Digit Zero
0xa2b0  ;Fullwidth Digit One
0xa2b1  ;Fullwidth Digit Two
0xa2b2  ;Fullwidth Digit Three
0xa2b3  ;Fullwidth Digit Four
0xa2b4  ;Fullwidth Digit Five
0xa2b5  ;Fullwidth Digit Six
0xa2b6  ;Fullwidth Digit Seven
0xa2b7  ;Fullwidth Digit Eight
0xa2b8  ;Fullwidth Digit Nine

0xa2e9  0xa2cf  ;Fullwidth Small A -> Fullwidth Capital A
0xa2ea  0xa2d0  ;Fullwidth Small B -> Fullwidth Capital B
0xa2eb  0xa2d1  ;Fullwidth Small C -> Fullwidth Capital C
0xa2ec  0xa2d2  ;Fullwidth Small D -> Fullwidth Capital D
0xa2ed  0xa2d3  ;Fullwidth Small E -> Fullwidth Capital E
0xa2ee  0xa2d4  ;Fullwidth Small F -> Fullwidth Capital F
0xa2ef  0xa2d5  ;Fullwidth Small G -> Fullwidth Capital G
0xa2f0  0xa2d6  ;Fullwidth Small H -> Fullwidth Capital H
0xa2f1  0xa2d7  ;Fullwidth Small I -> Fullwidth Capital I
0xa2f2  0xa2d8  ;Fullwidth Small J -> Fullwidth Capital J
0xa2f3  0xa2d9  ;Fullwidth Small K -> Fullwidth Capital K
0xa2f4  0xa2da  ;Fullwidth Small L -> Fullwidth Capital L
0xa2f5  0xa2db  ;Fullwidth Small M -> Fullwidth Capital M
0xa2f6  0xa2dc  ;Fullwidth Small N -> Fullwidth Capital N
0xa2f7  0xa2dd  ;Fullwidth Small O -> Fullwidth Capital O
0xa2f8  0xa2de  ;Fullwidth Small P -> Fullwidth Capital P
0xa2f9  0xa2df  ;Fullwidth Small Q -> Fullwidth Capital Q
0xa2fa  0xa2e0  ;Fullwidth Small R -> Fullwidth Capital R
0xa2fb  0xa2e1  ;Fullwidth Small S -> Fullwidth Capital S
0xa2fc  0xa2e2  ;Fullwidth Small T -> Fullwidth Capital T
0xa2fd  0xa2e3  ;Fullwidth Small U -> Fullwidth Capital U
0xa2fe  0xa2e4  ;Fullwidth Small V -> Fullwidth Capital V

...Note break in sequence...

0xa340  0xa2e5  ;Fullwidth Small W -> Fullwidth Capital W
0xa341  0xa2e6  ;Fullwidth Small X -> Fullwidth Capital X
0xa342  0xa2e7  ;Fullwidth Small Y -> Fullwidth Capital Y
0xa343  0xa2e8  ;Fullwidth Small Z -> Fullwidth Capital Z


*** Code Page 1361 ***

Not yet available (05/17/94)



****************************************************************************/
