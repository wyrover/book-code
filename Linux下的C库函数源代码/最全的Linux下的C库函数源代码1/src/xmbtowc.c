/***
*xmbtowc.c - Convert multibyte char to wide char.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Convert a multibyte character into the equivalent wide character.
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <mtdll.h>
#include <errno.h>
#include <dbgint.h>
#include <ctype.h>
#include <limits.h>              /* for INT_MAX */
#include <stdio.h>               /* for EOF */
#include <xlocinfo.h>            /* for _Cvtvec, _Mbrtowc */
#include <internal.h>
#include <locale.h>
#include <setlocal.h>
#include <mbctype.h>             /* for _ismbblead_l */
#include <sect_attribs.h>
#include <rterr.h>

/***
*int _Mbrtowc() - Convert multibyte char to wide character.
*
*Purpose:
*       Convert a multi-byte character into the equivalent wide character,
*       according to the specified LC_CTYPE category, or the current locale.
*       [ANSI].
*
*       NOTE:  Currently, the C libraries support the "C" locale only.
*              Non-C locale support now available under _INTL switch.
*Entry:
*       wchar_t  *pwc = pointer to destination wide character
*       const char *s = pointer to multibyte character
*       size_t      n = maximum length of multibyte character to consider
*       mbstate_t *pst      = pointer to state
*       const _Cvtvec *     = pointer to locale info
*
*Exit:
*       If s = NULL, returns 0, indicating we only use state-independent
*       character encodings.
*       If s != NULL, returns:  0 (if *s = null char)
*                               -1 (if the next n or fewer bytes not valid mbc)
*                               number of bytes comprising converted mbc
*
*Exceptions:
*
*******************************************************************************/

int _MRTIMP2 __cdecl _Mbrtowc(
        wchar_t  *pwc,
        const char *s,
        size_t n,
        mbstate_t *pst,
        const _Cvtvec *ploc
        )
{
        if ( !s || n == 0 )
            /* indicate do not have state-dependent encodings,
               handle zero length string */
            return 0;

        if ( !*s )
        {
            /* handle NULL char */
            if (pwc)
                *pwc = 0;
            return 0;
        }

        {   /* perform locale-dependent parse */
            LCID handle;
            UINT codepage;
            _locale_t locale;
            BOOL islead;

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
                if (pwc)
                    *pwc = (wchar_t)(unsigned char)*s;
                return sizeof(char);
            }

            locale = _GetLocaleForCP(codepage);

            _ASSERTE (___mb_cur_max_l_func(locale) == 1 ||
                      ___mb_cur_max_l_func(locale) == 2);

            if (*pst != 0)
            {   /* complete two-byte multibyte character */
                ((char *)pst)[1] = *s;
                if (___mb_cur_max_l_func(locale) <= 1 ||
                    (MultiByteToWideChar(codepage,
                                         MB_PRECOMPOSED|MB_ERR_INVALID_CHARS,
                                         (char *)pst,
                                         2,
                                         pwc,
                                         (pwc) ? 1 : 0) == 0))
                {   /* translation failed */
                    *pst = 0;
                    errno = EILSEQ;
                    return -1;
                }
                *pst = 0;
                return ___mb_cur_max_l_func(locale);
            }

            islead = (locale == NULL) ? _cpp_isleadbyte((unsigned char)*s)
                                      : _ismbblead_l((unsigned char)*s, locale);
            if ( islead )
            {
                /* multi-byte char */
                if (n < (size_t)___mb_cur_max_l_func(locale))
                {   /* save partial multibyte character */
                    ((char *)pst)[0] = *s;
                    return (-2);
                }
                else if ( ___mb_cur_max_l_func(locale) <= 1 ||
                          (MultiByteToWideChar( codepage,
                                                MB_PRECOMPOSED |
                                                    MB_ERR_INVALID_CHARS,
                                                s,
                                                ___mb_cur_max_l_func(locale),
                                                pwc,
                                                (pwc) ? 1 : 0) == 0) )
                {
                    /* validate high byte of mbcs char */
                    if (!*(s+1))
                    {
                        *pst = 0;
                        errno = EILSEQ;
                        return -1;
                    }
/*                  else translation failed with no complaint? [pjp] */
                }
                return ___mb_cur_max_l_func(locale);
            }
            else {
                /* single byte char */

                if ( MultiByteToWideChar( codepage,
                                          MB_PRECOMPOSED|MB_ERR_INVALID_CHARS,
                                          s,
                                          1,
                                          pwc,
                                          (pwc) ? 1 : 0) == 0 )
                {
                    errno = EILSEQ;
                    return -1;
                }

                return sizeof(char);
            }
        }
}

#ifdef MRTDLL
int _MRTIMP2 __cdecl _Mbrtowc(
        unsigned short* pwc,
        const char *s,
        size_t n,
        mbstate_t *pst,
        const _Cvtvec *ploc
        )
    {
    return _Mbrtowc((wchar_t *)pwc, s, n, pst, ploc);
    }
#endif  /* MRTDLL */


#pragma managed(push, off)

/*
 * Define a codepage-to-locale mapping hashtable to be used by _Mbrtowc et al.
 * The hash length is chosen to distribute the ~90 valid codepages so that a
 * minimal number of buckets (~3) have more than 1 codepage less than 10,000.
 */

typedef struct _CPLocEntry {
    struct _CPLocEntry *next;
    UINT                codepage;
    _locale_t           locale;
} _CPLocEntry;

#define CPHASHLEN   62

static _CPLocEntry *volatile _CPLocHash[CPHASHLEN];

static int __cdecl _InitCPLocHash(void);

_CRTALLOC(".CRT$XID") static _PIFV pinit = _InitCPLocHash;

/***
 * _locale_t _CreateLocForCP() - Return the locale ".<codepage>"
 *
 * Purpose:
 *      Call _create_locale for the given codepage.  This is split out of
 *      _GetLocaleForCP for performance reasons, since it almost never gets
 *      run, but it triggers buffer overrun checking.
 * Entry:
 *      UINT codepage = codepage to convert to a _locale_t
 * Exit:
 *      Returns the corresponding _locale_t, or NULL if an error occurs.
 ******************************************************************************/

static __declspec(noinline)
_locale_t __cdecl _CreateLocForCP(
    UINT codepage
    )
{
    char locale_name[32];

    locale_name[0] = '.';
    _ui64toa_s(codepage, locale_name + 1, sizeof(locale_name) - 1, 10);
    return _create_locale(LC_ALL, locale_name);
}

/***
 * _locale_t _GetLocaleForCP() - retrieve the _locale_t for a codepage.
 *
 * Purpose:
 *      Return the _locale_t for a given codepage.  Use a hashtable of all
 *      previously-used codepages to avoid the expensive locale recreation.
 * Entry:
 *      UINT codepage = codepage to retrieve
 * Exit:
 *      Returns the _locale_t corresponding to the locale ".<codepage>", or
 *      NULL if the codepage isn't valid, or we hit any other error.
 ******************************************************************************/

_locale_t __cdecl _GetLocaleForCP(
    UINT codepage
    )
{
    _CPLocEntry *volatile *pBucket;
    _CPLocEntry *pHead;
    _CPLocEntry *pLink;
    _CPLocEntry *pNewEntry;

    /*
     * Get hash bucket with linked list to search for existing codepage entry.
     */
    pBucket = _CPLocHash + (codepage % CPHASHLEN);

    /*
     * Loop until we find the codepage, successfully add a new entry for the
     * codepage, or hit an error.  If a search for the codepage fails, we
     * allocate a new entry and try to push it on the head of the list.  But
     * if the list has been modified in the meantime, then we need to restart
     * the search.
     */
    pNewEntry = NULL;
    for (;;)
    {
        /*
         * Look for the codepage in the linked list, remembering the original
         * list head.
         */
        pHead = *pBucket;
        for (pLink = pHead; pLink != NULL; pLink = pLink->next)
        {
            if (pLink->codepage == codepage)
            {
                /*
                 * The search was successful.  If this was a rescan of the
                 * list, then free the redundant entry we failed to push onto
                 * the list on the previous loop.
                 */
                if (pNewEntry != NULL)
                {
                    _free_locale(pNewEntry->locale);
                    _free_crt(pNewEntry);
                }
                return pLink->locale;
            }
        }

        /*
         * The codepage wasn't found.  If this is the first time through the
         * loop, create the corresponding _locale_t and allocate the new list
         * entry.  If that fails, return NULL as the mapping has failed.
         */
        if (pNewEntry == NULL)
        {
            pNewEntry = (_CPLocEntry *)_malloc_crt(sizeof(_CPLocEntry));
            if (pNewEntry == NULL)
            {
                return NULL;
            }

                        pNewEntry->locale = _CreateLocForCP(codepage);
            if (pNewEntry->locale == NULL)
            {
                                _free_crt(pNewEntry);
                return NULL;
            }

            pNewEntry->codepage = codepage;
        }

        /*
         * Try to push the new entry on the head of the list.  If successful,
         * return the newly-created locale.  Otherwise, the list head has
         * changed since we started this loop, and we'll have to retry the
         * search.
         */
        pNewEntry->next = pHead;
        if (InterlockedCompareExchangePointer(
                (volatile PVOID *)pBucket,
                pNewEntry,
                pHead) == pHead)
        {
            return pNewEntry->locale;
        }
    }
}

/***
 * void _ReleaseCPLocHash() - free the codepage-to-locale hashtable
 *
 * Purpose:
 *      Release all resources held by the codepage-to-locale hashtable.
 *      Called on CRT shutdown.
 ******************************************************************************/

/*
 * Problems in the PSDK header definitions of InterlockedExchangePointer
 * force us to disable some warnings.
 */
#pragma warning(push)
#pragma warning(disable: 4311 4312)

static void __cdecl _ReleaseCPLocHash(
    void
    )
{
    int i;
    _CPLocEntry *pLink;
    _CPLocEntry *pNext;

    for (i = 0; i < CPHASHLEN; ++i)
    {
        /*
         * No other threads should be active.  But just in case, use a locked
         * exchange to clear the hash bucket and retrieve the old value at the
         * same time, preventing other threads from seeing the linked list
         * being freed.  This doesn't help if other threads are already
         * traversing that list.
         */
        pLink = (_CPLocEntry *)InterlockedExchangePointer(
                                    (volatile PVOID *)&_CPLocHash[i],
                                    NULL);
        while (pLink != NULL)
        {
            pNext = pLink->next;
            _free_locale(pLink->locale);
            _free_crt(pLink);
            pLink = pNext;
        }
    }
}

#pragma warning(pop)

/***
 * void _InitCPLocHash() - Register cleanup of codepage-to-locale hashtable
 *
 * Purpose:
 *      Set up to release the codepage-to-locale hashtable on CRT shutdown.
 ******************************************************************************/

static int __cdecl _InitCPLocHash(
    void
    )
{
    int ret = atexit(_ReleaseCPLocHash);
    return ret == 0 ? 0 : _RT_ONEXIT;
}

#pragma managed(pop)
