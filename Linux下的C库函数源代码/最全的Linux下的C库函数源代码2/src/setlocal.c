/***
*setlocal.c - Contains the setlocale function
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Contains the setlocale() function.
*
*******************************************************************************/

#include <locale.h>
#include <internal.h>

#if !defined (_WIN32)

static char _clocalestr[] = "C";

#else  /* !defined (_WIN32) */

#include <cruntime.h>
#include <setlocal.h>
#include <mtdll.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h> /* for strtol */
#include <dbgint.h>
#include <ctype.h>
#include <awint.h>
#include <mbctype.h>
#include <rterr.h>

/* C locale */
extern char __clocalestr[];


#ifdef _DEBUG
// free as much of memory as possible in debug mode to minimise leaks.
#include <sect_attribs.h>
#include <internal.h>
void __cdecl _locterm(void);

_CRTALLOC(".CRT$XPX") static _PVFV pterm = _locterm;

#endif  /* _DEBUG */


__declspec(selectany) struct {
        const char * catname;
        char * locale;
        int (* init)(threadlocinfo *);
} const __lc_category[LC_MAX-LC_MIN+1] = {
        /* code assumes locale initialization is "__clocalestr" */
        { "LC_ALL",     NULL,           __init_dummy /* never called */ },
        { "LC_COLLATE", __clocalestr,    __init_collate  },
        { "LC_CTYPE",   __clocalestr,    __init_ctype    },
        { "LC_MONETARY",__clocalestr,    __init_monetary },
        { "LC_NUMERIC", __clocalestr,    __init_numeric  },
        { "LC_TIME",    __clocalestr,    __init_time }
};

static const char _first_127char[] = {
        1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
        69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
        86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100,101,102,
        103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
        120,121,122,123,124,125,126,127
};

extern struct __lc_time_data __lc_time_c;
extern threadlocinfo __initiallocinfo;
extern const unsigned short _wctype[];
static const short *_ctype_loc_style = _wctype+2;
int __locale_changed=0;



/*
 * Flag indicating whether or not setlocale() is active. Its value is the
 * number of setlocale() calls currently active.
 *
 * WARNING - This flag and query function are obsolete, and are here only to
 * preserve existing exports for binary compatibility.
 */
_CRTIMP int __setlc_active;
_CRTIMP int __cdecl ___setlc_active_func(void)
{
    return __setlc_active;
}

/*
 * Flag indicating whether or not a function which references the locale
 * without having locked it is active. Its value is the number of such
 * functions.
 *
 * WARNING - This flag and query function are obsolete, and are here only to
 * preserve existing exports for binary compatibility.
 */
_CRTIMP int __unguarded_readlc_active;
_CRTIMP int * __cdecl ___unguarded_readlc_active_add_func(void)
{
    return &__unguarded_readlc_active;
}

/* helper function prototypes */
char * _expandlocale(char *, char *, size_t, LC_ID *, UINT *, int);
void _strcats(char *, size_t, int, ...);
void __lc_lctostr(char *, size_t, const LC_STRINGS *);
int __lc_strtolc(LC_STRINGS *, const char *);
static char * __cdecl _setlocale_set_cat(pthreadlocinfo, int, const char *);
static char * __cdecl _setlocale_get_all(pthreadlocinfo);
static pthreadlocinfo __cdecl _updatetlocinfo_nolock(void);
static char * __cdecl _setlocale_nolock(pthreadlocinfo, int, const char *);
int __cdecl _setmbcp_nolock(int, pthreadmbcinfo);
void __cdecl __free_lconv_mon(struct lconv *);
void __cdecl __free_lconv_num(struct lconv *);
void __cdecl __free_lc_time(struct __lc_time_data *);
#endif  /* !defined (_WIN32) */



/***
*__freetlocinfo() - free threadlocinfo
*
*Purpose:
*       Free up the per-thread locale info structure specified by the passed
*       pointer.
*
*Entry:
*       pthreadlocinfo ptloci
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl __freetlocinfo (
        pthreadlocinfo ptloci
        )
{
    int category;
    /*
     * Free up lconv struct
     */
    if ( (ptloci->lconv != NULL) &&
         (ptloci->lconv != &__lconv_c) &&
         ((ptloci->lconv_intl_refcount != NULL) &&
         (*(ptloci->lconv_intl_refcount) == 0)))
    {
        if ( (ptloci->lconv_mon_refcount != NULL) &&
             (*(ptloci->lconv_mon_refcount) == 0))
        {
            _free_crt(ptloci->lconv_mon_refcount);
            __free_lconv_mon(ptloci->lconv);
        }

        if ( (ptloci->lconv_num_refcount != NULL) &&
             (*(ptloci->lconv_num_refcount) == 0))
        {
            _free_crt(ptloci->lconv_num_refcount);
            __free_lconv_num(ptloci->lconv);
        }

        _free_crt(ptloci->lconv_intl_refcount);
        _free_crt(ptloci->lconv);
    }

    /*
     * Free up ctype tables
     */
    if ( (ptloci->ctype1_refcount != NULL) &&
         (*(ptloci->ctype1_refcount) == 0) )
    {
        _free_crt(ptloci->ctype1-_COFFSET);
        _free_crt((char *)(ptloci->pclmap - _COFFSET - 1));
        _free_crt((char *)(ptloci->pcumap - _COFFSET - 1));
        _free_crt(ptloci->ctype1_refcount);
    }

    /*
     * Free up the __lc_time_data struct
     */
    if ( ptloci->lc_time_curr != &__lc_time_c &&
         ((ptloci->lc_time_curr->refcount) == 0) )
    {
        __free_lc_time(ptloci->lc_time_curr);
        _free_crt(ptloci->lc_time_curr);
    }

    for (category = LC_MIN; category <= LC_MAX; ++category) {
        if ((ptloci->lc_category[category].locale != __clocalestr) &&
              (ptloci->lc_category[category].refcount != NULL) &&
              (*ptloci->lc_category[category].refcount == 0) )
        {
            _free_crt(ptloci->lc_category[category].refcount);
        }
        _ASSERTE(((ptloci->lc_category[category].wlocale != NULL) && (ptloci->lc_category[category].wrefcount != NULL)) ||
                 ((ptloci->lc_category[category].wlocale == NULL) && (ptloci->lc_category[category].wrefcount == NULL)));
        if ((ptloci->lc_category[category].wlocale != NULL) &&
              (ptloci->lc_category[category].wrefcount != NULL) &&
              (*ptloci->lc_category[category].wrefcount == 0) )
        {
            _free_crt(ptloci->lc_category[category].wrefcount);
        }
    }

    /*
     * Free up the threadlocinfo struct
     */
    _free_crt(ptloci);
}

/***
* __addlocaleref(pthreadlocinfo ptloci)
*
* Purpose:
*       Increment the refrence count for each element in the localeinfo struct.
*
*******************************************************************************/
void __cdecl __addlocaleref( pthreadlocinfo ptloci)
{
    int category;
    InterlockedIncrement(&(ptloci->refcount));
    if ( ptloci->lconv_intl_refcount != NULL )
        InterlockedIncrement(ptloci->lconv_intl_refcount);

    if ( ptloci->lconv_mon_refcount != NULL )
        InterlockedIncrement(ptloci->lconv_mon_refcount);

    if ( ptloci->lconv_num_refcount != NULL )
        InterlockedIncrement(ptloci->lconv_num_refcount);

    if ( ptloci->ctype1_refcount != NULL )
        InterlockedIncrement(ptloci->ctype1_refcount);

    for (category = LC_MIN; category <= LC_MAX; ++category) {
        if (ptloci->lc_category[category].locale != __clocalestr &&
            ptloci->lc_category[category].refcount != NULL)
            InterlockedIncrement(ptloci->lc_category[category].refcount);
        if (ptloci->lc_category[category].wlocale != NULL &&
            ptloci->lc_category[category].wrefcount != NULL)
            InterlockedIncrement(ptloci->lc_category[category].wrefcount);
    }
    InterlockedIncrement(&(ptloci->lc_time_curr->refcount));
}

/***
* __removelocaleref(pthreadlocinfo ptloci)
*
* Purpose:
*       Decrement the refrence count for each elemnt in the localeinfo struct.
*
******************************************************************************/
void * __cdecl __removelocaleref( pthreadlocinfo ptloci)
{
    int category;
    if ( ptloci != NULL )
    {
        InterlockedDecrement(&(ptloci->refcount));

        if ( ptloci->lconv_intl_refcount != NULL )
            InterlockedDecrement(ptloci->lconv_intl_refcount);

        if ( ptloci->lconv_mon_refcount != NULL )
            InterlockedDecrement(ptloci->lconv_mon_refcount);

        if ( ptloci->lconv_num_refcount != NULL )
            InterlockedDecrement(ptloci->lconv_num_refcount);

        if ( ptloci->ctype1_refcount != NULL )
            InterlockedDecrement(ptloci->ctype1_refcount);

        for (category = LC_MIN; category <= LC_MAX; ++category) {
            if (ptloci->lc_category[category].locale != __clocalestr &&
                ptloci->lc_category[category].refcount != NULL)
                InterlockedDecrement(ptloci->lc_category[category].refcount);
            if (ptloci->lc_category[category].wlocale != NULL &&
                ptloci->lc_category[category].wrefcount != NULL)
                InterlockedDecrement(ptloci->lc_category[category].wrefcount);
        }
        InterlockedDecrement(&(ptloci->lc_time_curr->refcount));
    }
    return ptloci;
}

/***
*
* _copytlocinfo_nolock(pthreadlocinfo ptlocid, pthreadlocinfo ptlocis)
*
* Purpose:
*       Copy the contents of ptlocis to ptlocid and increase the refcount of all the
*       elements in ptlocid after copy.
*
******************************************************************************/
static void __cdecl _copytlocinfo_nolock(
    pthreadlocinfo ptlocid,
    pthreadlocinfo ptlocis)
{
    if (ptlocis != NULL && ptlocid != NULL && ptlocid != ptlocis) {
        *ptlocid = *ptlocis;
        ptlocid->refcount = 0;
        __addlocaleref(ptlocid);
    }
}

/***
*
* _updatelocinfoEx_nolock(pthreadlocinfo *pptlocid, pthreadlocinfo ptlocis)
*
* Purpose:
*       Update *pptlocid to ptlocis. This might include freeing contents of *pptlocid.
*
******************************************************************************/
static pthreadlocinfo __cdecl _updatetlocinfoEx_nolock(
    pthreadlocinfo *pptlocid,
    pthreadlocinfo ptlocis)
{
    pthreadlocinfo ptloci;

    if (ptlocis == NULL || pptlocid == NULL)
        return NULL;
    ptloci = *pptlocid;
    if ( ptloci != ptlocis)
    {
        /*
         * Update to the current locale info structure and increment the
         * reference counts.
         */
        *pptlocid = ptlocis;
        __addlocaleref(ptlocis);
        /*
         * Decrement the reference counts in the old locale info
         * structure.
         */
        if ( ptloci != NULL )
        {
            __removelocaleref(ptloci);
        }
        /*
         * Free the old locale info structure, if necessary.  Must be done
         * after incrementing reference counts in current locale in case
         * any refcounts are shared with the old locale.
         */
        if ( (ptloci != NULL) &&
             (ptloci->refcount == 0) &&
             (ptloci != &__initiallocinfo) )
            __freetlocinfo(ptloci);

    }

    return ptlocis;
}

/***
*__updatetlocinfo() - refresh the thread's locale info
*
*Purpose:
*       If this thread does not have it's ownlocale which means that either
*       ownlocale flag in ptd is not set or ptd->ptloci == NULL, then Update
*       the current thread's reference to the locale information to match the
*       current global locale info. Decrement the reference on the old locale
*       information struct and if this count is now zero (so that no threads
*       are using it), free it.
*
*Entry:
*
*Exit:
*
*       if (!_getptd()->ownlocale || _getptd()->ptlocinfo == NULL)
*           _getptd()->ptlocinfo == __ptlocinfo
*       else
*           _getptd()->ptlocinfo
*
*Exceptions:
*
*******************************************************************************/
pthreadlocinfo __cdecl __updatetlocinfo(void)
{
    pthreadlocinfo ptloci;
    _ptiddata ptd = _getptd();

    if (!(ptd->_ownlocale & __globallocalestatus)|| !ptd->ptlocinfo) {
        _mlock(_SETLOCALE_LOCK);
        __try
        {
            ptloci = _updatetlocinfoEx_nolock(&ptd->ptlocinfo, __ptlocinfo);
        }
        __finally
        {
            _munlock(_SETLOCALE_LOCK);
        }
    } else {
        ptloci = _getptd()->ptlocinfo;
    }
    if(!ptloci)
    {
        _amsg_exit(_RT_LOCALE);
    }

    return ptloci;
}

/***
* _configthreadlocale(int i)
*
* Purpose:
*       To set _ownlocale flag on threadinfo sturct. If this flag is set, this thread
*       is going own it's threadlocinfo struct. Setlocale call on other thread will have
*       no effect on this thread's locale. If 0 is passed then nothing is changed, but
*       current status is returned.
* Exit   :
*       Returns the current status - i.e. per thread locale is enabled or not.
*
*******************************************************************************/
int __cdecl _configthreadlocale(int i)
{
    /*
     * ownlocale flag struct:
     * bits: 000000000000000000000000000000P1
     * P is set when _ENABLE_PER_THREAD_LOCALE is called for this thread
     * or _ENABLE_PER_THREAD_LOCALE_NEW was set when this thread was created.
     *
     * __globallocalestatus structure:
     * bits: 11111111111111111111111111111N1G
     * G is set if _ENABLE_PER_THREAD_LOCALE_GLOBAL is set.
     * G is 0 if _ENABLE_PER_THREAD_LOCALE_GLOBAL is not set.
     * N is set if _ENABLE_PER_THREAD_LOCALE_NEW is set.
     * N is 0 if _ENABLE_PER_THREAD_LOCALE_NEW is not set.
     */
    _ptiddata ptd = _getptd();
    int retval = (ptd->_ownlocale & _PER_THREAD_LOCALE_BIT)==0 ? _DISABLE_PER_THREAD_LOCALE:_ENABLE_PER_THREAD_LOCALE;

    switch(i)
    {
        case _ENABLE_PER_THREAD_LOCALE :
            ptd->_ownlocale = ptd->_ownlocale | _PER_THREAD_LOCALE_BIT;
            break;

        case _DISABLE_PER_THREAD_LOCALE :
            ptd->_ownlocale = ptd->_ownlocale & ~_PER_THREAD_LOCALE_BIT;
            break;

        case 0 :
            break;

        /* used only during dll startup for linkopt */
        case -1 :
            __globallocalestatus=-1;
            break;

        default :
            _VALIDATE_RETURN(("Invalid parameter for _configthreadlocale",0),EINVAL,-1);
            break;
    }

    return retval;

}

#ifdef _DEBUG
void __cdecl _locterm(void)
{
    if (__ptlocinfo != &__initiallocinfo) {
        _mlock(_SETLOCALE_LOCK);
        __try
        {
            __ptlocinfo = _updatetlocinfoEx_nolock(&__ptlocinfo, &__initiallocinfo);
        }
        __finally
        {
            _munlock(_SETLOCALE_LOCK);
        }
    }
}
#endif  /* _DEBUG */

/***
* void sync_legacy_variables_lk()
*
* Purpose:
*   Syncs all the legacy locale specific variables to the global locale.
*
*******************************************************************************/
static __inline void sync_legacy_variables_lk()
{
    __lc_codepage = __ptlocinfo->lc_codepage;
    __lc_collate_cp = __ptlocinfo->lc_collate_cp;
    __lc_clike = __ptlocinfo->lc_clike;
    __lc_time_curr = __ptlocinfo->lc_time_curr;
    __lconv = __ptlocinfo->lconv;
    _pctype = __ptlocinfo->pctype;
    __mb_cur_max = __ptlocinfo->mb_cur_max;
}
/***
*_free_locale() - free threadlocinfo
*
*Purpose:
*       Free up the per-thread locale info structure specified by the passed
*       pointer.
*
*Entry:
*       pthreadlocinfo ptloci
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _free_locale(
        _locale_t plocinfo
        )
{
    if (plocinfo != NULL)
    {
        if (plocinfo->mbcinfo != NULL &&
                InterlockedDecrement(&(plocinfo->mbcinfo->refcount)) == 0 &&
                plocinfo->mbcinfo != &__initialmbcinfo )
        {
            _free_crt(plocinfo->mbcinfo);
        }
        if (plocinfo->locinfo != NULL)
        {
            /*
             * this portion has to be in locale lock as there may be case when
             * not this thread but some other thread is still holding to this
             * locale and is also trying to free this locale. In this case
             * we may end up leaking memory.
             */
            _mlock(_SETLOCALE_LOCK);
            __try
            {
                __removelocaleref(plocinfo->locinfo);
                if ( (plocinfo->locinfo != NULL) &&
                     (plocinfo->locinfo->refcount == 0) &&
                     (plocinfo->locinfo != &__initiallocinfo) )
                    __freetlocinfo(plocinfo->locinfo);
            }
            __finally
            {
                _munlock(_SETLOCALE_LOCK);
            }
        }
        /*
         * set plocinfo structure to zero. This will reduce the chance of
         * using plocinfo after it is being freed.
         */
        plocinfo->locinfo = (void *)(((char *)NULL) + 0xbaadf00d);
        plocinfo->mbcinfo = (void *)(((char *)NULL) + 0xbaadf00d);
        _free_crt(plocinfo);
    }
}

/* __free_locale will be removed in the next LKG */
void __cdecl __free_locale(
        _locale_t plocinfo
        )
{
    _free_locale(plocinfo);
}

/***
* _locale_t _create_locale(int category, char *locale) -
*    Set one or all locale categories of global locale structure
*
* Purpose:
*       The _create_locale() routine allows the user to create a _locale_t
*       object that can be used with other locale functions.
*
* Entry:
*       int category = One of the locale categories defined in locale.h
*       char *locale = String identifying a specific locale.
*
* Exit:
*       If supplied locale pointer != NULL:
*
*           If locale string is '\0', set locale to default.
*
*           If desired setting can be honored, return a pointer to the
*           locale string for the appropriate category.
*
*           If desired setting can NOT be honored, return NULL.
*
* Exceptions:
*       Compound locale strings of the form "LC_COLLATE=xxx;LC_CTYPE=xxx;..."
*       are allowed for the LC_ALL category.  This is to support the ability
*       to restore locales with the returned string, as specified by ANSI.
*       Setting the locale with a compound locale string will succeed unless
*       *all* categories failed.  The returned string will reflect the current
*       locale.  For example, if LC_CTYPE fails in the above string, setlocale
*       will return "LC_COLLATE=xxx;LC_CTYPE=yyy;..." where yyy is the
*       previous locale (or the C locale if restoring the previous locale
*       also failed).  Unrecognized LC_* categories are ignored.
*
*******************************************************************************/

_locale_t __cdecl _create_locale(
        int _category,
        const char *_locale
        )
{
    _locale_t retval = NULL;

    /* Validate input */
    if ( (_category < LC_MIN) || (_category > LC_MAX) || _locale == NULL)
        return NULL;

    if ((retval = _calloc_crt(sizeof(_locale_tstruct), 1)) == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    if ((retval->locinfo = _calloc_crt(sizeof(threadlocinfo), 1)) == NULL)
    {
        _free_crt(retval);
        errno = ENOMEM;
        return NULL;
    }
    if ((retval->mbcinfo = _calloc_crt(sizeof(threadmbcinfo), 1)) == NULL)
    {
        _free_crt(retval->locinfo);
        _free_crt(retval);
        errno = ENOMEM;
        return NULL;
    }
    _copytlocinfo_nolock(retval->locinfo, &__initiallocinfo);

    if (_setlocale_nolock(retval->locinfo, _category, _locale) == NULL)
    {
        __removelocaleref(retval->locinfo);
        __freetlocinfo(retval->locinfo);

        _free_crt(retval);
        retval = NULL;
    }
    else
    {
        if (_setmbcp_nolock(retval->locinfo->lc_codepage, retval->mbcinfo) != 0)
        {
            _free_crt(retval->mbcinfo);
            __removelocaleref(retval->locinfo);
            __freetlocinfo(retval->locinfo);
            _free_crt(retval);
            retval = NULL;
        }
        else
        {
            retval->mbcinfo->refcount = 1;
            retval->mbcinfo->refcount = 1;
        }
    }

    return retval;
}

/* __create_locale will be removed in the next LKG */
_locale_t __cdecl __create_locale(
        int _category,
        const char *_locale
        )
{
    return _create_locale(_category, _locale);
}

/***
* _locale_t _get_current_locale() -
*    Gets the current locale setting.
*
* Purpose:
*       Gets the current locale setting for this thread. Returns locale
*       in form of _locale_t, which then can be used with other locale
*       aware string funcitons.
*
* Entry:
*
* Exit:
*
* Exceptions:
*
*******************************************************************************/

_locale_t __cdecl _get_current_locale(void)
{
    _locale_t retval = NULL;
    _ptiddata ptd = _getptd();

    if ((retval = _calloc_crt(sizeof(_locale_tstruct), 1)) == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }

    __updatetlocinfo();
    __updatetmbcinfo();
    /*
     * No one can free the data pointed to by ptlocinfo while we're copying
     * it, since we're copying this thread's ptlocinfo, which won't be updated
     * during the copy.  So there are no worries about it being freed from
     * under us.  We still need a lock while adding a reference for the new
     * copy, though, because of the race condition found in _wsetlocale.
     */
    retval->locinfo = ptd->ptlocinfo;
    retval->mbcinfo = ptd->ptmbcinfo;
    _mlock(_SETLOCALE_LOCK);
    __try {
        __addlocaleref(retval->locinfo);
    }
    __finally {
        _munlock(_SETLOCALE_LOCK);
    }
    InterlockedIncrement(&(retval->mbcinfo->refcount));

    return retval;
}

/* __get_current_locale will be removed in the next LKG */
_locale_t __cdecl __get_current_locale(void)
{
    return _get_current_locale();
}

/***
*char * setlocale(int category, char *locale) - Set one or all locale categories
*
*Purpose:
*       The setlocale() routine allows the user to set one or more of
*       the locale categories to the specific locale selected by the
*       user.  [ANSI]
*
*       NOTE: Under !_INTL, the C libraries only support the "C" locale.
*       Attempts to change the locale will fail.
*
*Entry:
*       int category = One of the locale categories defined in locale.h
*       char *locale = String identifying a specific locale or NULL to
*                  query the current locale.
*
*Exit:
*       If supplied locale pointer == NULL:
*
*           Return pointer to current locale string and do NOT change
*           the current locale.
*
*       If supplied locale pointer != NULL:
*
*           If locale string is '\0', set locale to default.
*
*           If desired setting can be honored, return a pointer to the
*           locale string for the appropriate category.
*
*           If desired setting can NOT be honored, return NULL.
*
*Exceptions:
*       Compound locale strings of the form "LC_COLLATE=xxx;LC_CTYPE=xxx;..."
*       are allowed for the LC_ALL category.  This is to support the ability
*       to restore locales with the returned string, as specified by ANSI.
*       Setting the locale with a compound locale string will succeed unless
*       *all* categories failed.  The returned string will reflect the current
*       locale.  For example, if LC_CTYPE fails in the above string, setlocale
*       will return "LC_COLLATE=xxx;LC_CTYPE=yyy;..." where yyy is the
*       previous locale (or the C locale if restoring the previous locale
*       also failed).  Unrecognized LC_* categories are ignored.
*
*******************************************************************************/

#if !defined (_WIN32)

char * __cdecl setlocale (
        int _category,
        const char *_locale
        )
{
    if ( (_locale == NULL) ||
         (_locale[0] == '\0') ||
         ( (_locale[0]=='C') && (_locale[1]=='\0'))  )
        return(__clocalestr);
    else
        return(NULL);
}

#else  /* !defined (_WIN32) */

char * __cdecl setlocale (
        int _category,
        const char *_locale
        )
{
    char * retval=NULL;
    pthreadlocinfo ptloci = NULL;
    _ptiddata ptd;

    /* Validate category */
    _VALIDATE_RETURN(LC_MIN <= _category && _category <= LC_MAX, EINVAL, NULL);

    ptd = _getptd();

    __updatetlocinfo();
    // Note here that we increment the _ownlocale for this thread. We need this
    // to make sure that the locale is not updated to some other locale by call to
    // stricmp().
    // Don't set any flag that aligns with N, P or G
    ptd->_ownlocale |= 0x10;
    __try {
        if ((ptloci = _calloc_crt(sizeof(threadlocinfo), 1)) != NULL)
        {
            /*
             * No one can free the data pointed to by ptlocinfo while we're
             * copying it, since we're copying this thread's ptlocinfo, which
             * won't be updated during the copy.  So there are no worries
             * about it being freed from under us.  We still need a lock while
             * making the copy, though, because of the race condition found in
             * _wsetlocale.
             */
            _mlock(_SETLOCALE_LOCK);
            __try {
                _copytlocinfo_nolock(ptloci, ptd->ptlocinfo);
            }
            __finally {
                _munlock(_SETLOCALE_LOCK);
            }

            if (( ptloci != NULL) && (retval = _setlocale_nolock(ptloci, _category, _locale))) {
                /*
                * What we are trying here is that if no call has been made to
                * setlocale to change locale from "C" locale to some other locale
                * we keep __locale_changed = 0. Other funcitons depending on locale
                * use this variable to optimize performance for C locale which is
                * normally the case in 90% of the applications.
                */
                if (_locale != NULL && strcmp(_locale, __clocalestr))
                {
                    __locale_changed = 1;
                }

                _mlock(_SETLOCALE_LOCK);
                __try {
                    (void)_updatetlocinfoEx_nolock(&ptd->ptlocinfo, ptloci);
                    __removelocaleref(ptloci);
                    // Note that after incrementing _ownlocale, if this thread doesn't
                    // have it's own locale, _ownlocale variable should be 1.
                    if (!(ptd->_ownlocale & _PER_THREAD_LOCALE_BIT) &&
                        !(__globallocalestatus & _GLOBAL_LOCALE_BIT)) {
                        (void)_updatetlocinfoEx_nolock(&__ptlocinfo, ptd->ptlocinfo);
                        // All these variables are for backward compatiblity.
                        memcpy(__lc_handle, __ptlocinfo->lc_handle, sizeof(LCID)*(LC_MAX-LC_MIN+1));
                        sync_legacy_variables_lk();
                    }
                } __finally {
                    _munlock(_SETLOCALE_LOCK);
                }
            } else {
                __removelocaleref(ptloci);
                __freetlocinfo(ptloci);
            }
        }
    } __finally {
        // Undo the previous action.
        ptd->_ownlocale &= ~0x10;
    }

    return retval;
}

static char * __cdecl _setlocale_nolock(
        pthreadlocinfo ploci,
        int _category,
        const char *_locale
        )
{
    char * retval;
    /* Interpret locale */

    if (_category != LC_ALL)
    {
        retval = (_locale) ? _setlocale_set_cat(ploci, _category,_locale) :
            ploci->lc_category[_category].locale;

    } else { /* LC_ALL */
        char lctemp[MAX_LC_LEN];
        int i;
        int same = 1;
        int fLocaleSet = 0; /* flag to indicate if anything successfully set */

        if (_locale != NULL)
        {
            if ( (_locale[0]=='L') && (_locale[1]=='C') && (_locale[2]=='_') )
            {
                /* parse compound locale string */
                size_t len;
                const char * p = _locale;  /* start of string to parse */
                const char * s;

                do {
                    s = strpbrk(p,"=;");

                    if ((s==(char *)NULL) || (!(len=(size_t)(s-p))) || (*s==';'))
                        return NULL;  /* syntax error */

                    /* match with known LC_ strings, if possible, else ignore */
                    for (i=LC_ALL+1; i<=LC_MAX; i++)
                    {
                        if ((!strncmp(__lc_category[i].catname,p,len))
                            && (len==strlen(__lc_category[i].catname)))
                        {
                            break;  /* matched i */
                        }
                    } /* no match if (i>LC_MAX) -- just ignore */

                    if ((!(len = strcspn(++s,";"))) && (*s!=';'))
                        return NULL;  /* syntax error */

                    if (i<=LC_MAX)
                    {
                        _ERRCHECK(strncpy_s(lctemp, _countof(lctemp), s, len));
                        lctemp[len]='\0';   /* null terminate string */

                        /* don't fail unless all categories fail */
                        if (_setlocale_set_cat(ploci, i,lctemp))
                            fLocaleSet++;       /* record a success */
                    }
                    if (*(p = s+len)!='\0')
                        p++;  /* skip ';', if present */

                } while (*p);

                retval = (fLocaleSet) ? _setlocale_get_all(ploci) : NULL;

            } else { /* simple LC_ALL locale string */

                /* confirm locale is supported, get expanded locale */
                if (retval = _expandlocale((char *)_locale, lctemp, _countof(lctemp), NULL, NULL, _category))
                {
                    for (i=LC_MIN; i<=LC_MAX; i++)
                    {
                        if (i!=LC_ALL)
                        {
                            if (strcmp(lctemp, ploci->lc_category[i].locale))
                            {
                                if (_setlocale_set_cat(ploci, i, lctemp))
                                {
                                    fLocaleSet++;   /* record a success */
                                }
                                else
                                {
                                    same = 0;       /* record a failure */
                                }
                            }
                            else
                                fLocaleSet++;   /* trivial succcess */
                        }
                    }
                    if (same) /* needn't call setlocale_get_all() if all the same */
                    {
                        retval = _setlocale_get_all(ploci);
                        /* retval set above */
                    }
                    else
                        retval = (fLocaleSet) ? _setlocale_get_all(ploci) : NULL;
                }
            }
        } else { /* LC_ALL & NULL */
            retval = _setlocale_get_all (ploci);
        }
    }

    /* common exit point */
    return retval;
} /* setlocale */


static char * __cdecl _setlocale_set_cat (
        pthreadlocinfo ploci,
        int category,
        const char * locale
        )
{
    char * oldlocale;
    LCID oldhandle;
    UINT oldcodepage;
    LC_ID oldid;

    LC_ID idtemp;
    UINT cptemp;
    char lctemp[MAX_LC_LEN];
    char * pch = NULL;
    size_t cch = 0;
    short out[sizeof(_first_127char)];
    int i;
    _ptiddata _ptd = _getptd();
    struct _is_ctype_compatible *_Lcid_c = _ptd->_setloc_data._Lcid_c; // __setloc_data._Lcid_c is array
    int _LOC_CCACHE = sizeof(_ptd->_setloc_data._Lcid_c)/sizeof(struct _is_ctype_compatible);
    struct _is_ctype_compatible buf1, buf2;

    if (!_expandlocale((char *)locale, lctemp, _countof(lctemp), &idtemp, &cptemp, category))
    {
        return NULL;            /* unrecognized locale */
    }
    if (!strcmp(lctemp, ploci->lc_category[category].locale))
    {
        return ploci->lc_category[category].locale;
    }

    cch = sizeof(int)+strlen(lctemp)+1;
    if (!(pch = (char *)_malloc_crt(cch)))
    {
        return NULL;  /* error if malloc fails */
    }

    oldlocale = ploci->lc_category[category].locale; /* save for possible restore*/
    oldhandle = ploci->lc_handle[category];
    memcpy((void *)&oldid, (void *)&ploci->lc_id[category], sizeof(oldid));
    oldcodepage = ploci->lc_codepage;

    /* update locale string */
    _ERRCHECK(strcpy_s(pch + sizeof(int), cch - sizeof(int), lctemp));
    ploci->lc_category[category].locale = pch + sizeof(int);
    ploci->lc_handle[category] = MAKELCID(idtemp.wLanguage, SORT_DEFAULT);
    memcpy((void *)&ploci->lc_id[category], (void *)&idtemp, sizeof(idtemp));

    /* To speedup locale based comparisions, we identify if the current
     * local has first 127 character set same as CLOCALE. If yes then
     * ploci->lc_clike = TRUE.
     */

    if (category==LC_CTYPE)
    {
        ploci->lc_codepage = cptemp;
        buf1 = _Lcid_c[_LOC_CCACHE -1];
        /* brings the recently used codepage to the top. or else shifts
         * every thing down by one so that new _Lcid_c can be placed at
         * the top.
         */
        for ( i = 0; i < _LOC_CCACHE; i++)
        {
            if (ploci->lc_codepage == _Lcid_c[i].id)
            {
                /* We don't really want to swap cache around in case what we are looking
                 *  for is the first element of the cache
                 */
                if (i!=0)
                {
                    _Lcid_c[0] = _Lcid_c[i];
                    _Lcid_c[i] = buf1;
                }
                break;
            }
            else
            {
                buf2 = _Lcid_c[i];
                _Lcid_c[i] = buf1;
                buf1 = buf2;
            }
        }
        if ( i == _LOC_CCACHE)
        {
            if ( __crtGetStringTypeA(NULL, CT_CTYPE1,
                                      _first_127char,
                                      sizeof(_first_127char),
                                      out,
                                      ploci->lc_codepage,
                                      ploci->lc_handle[LC_CTYPE],
                                      TRUE ))
            {
                int j;
                for ( j = 0; j < sizeof(_first_127char); j++)
                    out[j] = out[j]&
                            (_UPPER|_LOWER|_DIGIT|_SPACE|_PUNCT|_CONTROL|_BLANK|_HEX|_ALPHA);
                if ( !memcmp(out, _ctype_loc_style, (sizeof(_first_127char)/sizeof(char))*sizeof(short)))
                {
                    _Lcid_c[0].is_clike = TRUE;
                }
                else
                {
                    _Lcid_c[0].is_clike = FALSE;
                }
            }
            else
                _Lcid_c[0].is_clike = FALSE;
            _Lcid_c[0].id = ploci->lc_codepage;
        }
        ploci->lc_clike = _Lcid_c[0].is_clike;
    }

    if ( category == LC_COLLATE )
        ploci->lc_collate_cp = cptemp;

    if (__lc_category[category].init(ploci))
    {
        /* restore previous state! */
        ploci->lc_category[category].locale = oldlocale;
        _free_crt(pch);
        ploci->lc_handle[category] = oldhandle;
        ploci->lc_codepage = oldcodepage;

        return NULL; /* error if non-zero return */
    }

    /* locale set up successfully */
    /* Cleanup */
    if ((oldlocale != __clocalestr) &&
        (InterlockedDecrement(ploci->lc_category[category].refcount) == 0)
        )
    {
        _ASSERT(0);
        _free_crt(ploci->lc_category[category].refcount);
        _free_crt(ploci->lc_category[category].wrefcount);
        ploci->lc_category[category].wlocale = NULL;
    }
    if (pch) {
        *(int *)pch  = 1;
    }
    ploci->lc_category[category].refcount = (int *)pch;

    return ploci->lc_category[category].locale;
} /* _setlocale_set_cat */



static char * __cdecl _setlocale_get_all ( pthreadlocinfo ploci)
{
    int i;
    int same = 1;
    char *pch = NULL;
    size_t cch = 0;
    int *refcount = NULL;
    size_t refcountSize = 0;
    /* allocate memory if necessary */
    refcountSize = sizeof(int)+(MAX_LC_LEN+1) * (LC_MAX-LC_MIN+1) + CATNAMES_LEN;
    if ( (refcount = _malloc_crt(refcountSize))
           == NULL)
        return NULL;

    pch = (char *)(refcount + 1);
    cch = refcountSize - sizeof(int);
    *pch = '\0';
    *refcount = 1;
    for (i=LC_MIN+1; ; i++)
    {
        _strcats(pch, cch, 3, __lc_category[i].catname,"=",ploci->lc_category[i].locale);
        if (i<LC_MAX)
        {
            _ERRCHECK(strcat_s(pch, cch, ";"));
            if (strcmp(ploci->lc_category[i].locale, ploci->lc_category[i+1].locale))
                same=0;
        }
        else
        {
            if (!same) {
                if (ploci->lc_category[LC_ALL].refcount != NULL &&
                    InterlockedDecrement(ploci->lc_category[LC_ALL].refcount) == 0) {
                    _ASSERT(0);
                    _free_crt(ploci->lc_category[LC_ALL].refcount);
                }
                if (ploci->lc_category[LC_ALL].wrefcount != NULL &&
                    InterlockedDecrement(ploci->lc_category[LC_ALL].wrefcount) == 0) {
                    _ASSERT(0);
                    _free_crt(ploci->lc_category[LC_ALL].wrefcount);
                }
                ploci->lc_category[LC_ALL].wrefcount = NULL;
                ploci->lc_category[LC_ALL].wlocale = NULL;
                ploci->lc_category[LC_ALL].refcount = refcount;
                return ploci->lc_category[LC_ALL].locale = pch;
            } else {
                _free_crt(refcount);
                if (ploci->lc_category[LC_ALL].refcount != NULL &&
                    InterlockedDecrement(ploci->lc_category[LC_ALL].refcount) == 0) {
                    _ASSERT(0);
                    _free_crt(ploci->lc_category[LC_ALL].refcount);
                }
                if (ploci->lc_category[LC_ALL].wrefcount != NULL &&
                    InterlockedDecrement(ploci->lc_category[LC_ALL].wrefcount) == 0) {
                    _ASSERT(0);
                    _free_crt(ploci->lc_category[LC_ALL].wrefcount);
                }
                ploci->lc_category[LC_ALL].wrefcount = NULL;
                ploci->lc_category[LC_ALL].wlocale = NULL;
                ploci->lc_category[LC_ALL].refcount = NULL;
                ploci->lc_category[LC_ALL].locale = NULL;
                return ploci->lc_category[LC_CTYPE].locale;
            }
        }
    }
} /* _setlocale_get_all */


char * _expandlocale (
        char *expr,
        char * output,
        size_t sizeInChars,
        LC_ID * id,
        UINT * cp,
        int category
        )
{
    _psetloc_struct _psetloc_data = &_getptd()->_setloc_data;
    UINT *pcachecp = &_psetloc_data->_cachecp;
    LC_ID *pcacheid = &_psetloc_data->_cacheid;
    char *cachein = _psetloc_data->_cachein;
    size_t cacheinSize = _countof(_psetloc_data->_cachein);
    char *cacheout = _psetloc_data->_cacheout;
    size_t cacheoutSize = _countof(_psetloc_data->_cacheout);
    size_t charactersInExpression = 0;

    if (!expr)
        return NULL; /* error if no input */

    if (output == NULL || sizeInChars == 0)
        return NULL; /* error if no output */


    if (((*expr=='C') && (!expr[1]))
        )  /* for "C" locale, just return */
    {

        _ERRCHECK(strcpy_s(output, sizeInChars, "C"));
        if (id)
        {
            id->wLanguage = 0;
            id->wCountry  = 0;
            id->wCodePage = 0;
        }
        if (cp)
        {
            *cp = CP_ACP; /* return to ANSI code page */
        }
        return output; /* "C" */
    }

    /* first, make sure we didn't just do this one */
    charactersInExpression = strlen(expr);
    if (charactersInExpression >= MAX_LC_LEN ||       /* we would never have cached this */
        (strcmp(cacheout,expr) && strcmp(cachein,expr)))
    {
        /* do some real work */
        LC_STRINGS names;
        const char *source=NULL;
        size_t charactersInSource = 0;

        if (__lc_strtolc(&names, expr))
            return NULL;  /* syntax error */

        if (!__get_qualified_locale(&names, pcacheid, &names))
            return NULL;    /* locale not recognized/supported */

        /* begin: cache atomic section */

        *pcachecp = pcacheid->wCodePage;

        __lc_lctostr(cacheout, cacheoutSize, &names);

        if (*expr && charactersInExpression < MAX_LC_LEN)
        {
            source = expr;
            charactersInSource = charactersInExpression;
        }
        else
        {
            /* Don't cache "" empty string or over-long string */
            source = "";
        }
        /* Ensure that whatever we are about to copy in will be null terminated */

        _ERRCHECK(strncpy_s(cachein, cacheinSize, source, charactersInSource + 1));

        /* end: cache atomic section */
    }
    if (id)
        memcpy((void *)id, (void *)pcacheid, sizeof(*pcacheid));   /* possibly return LC_ID */
    if (cp)
        memcpy((void *)cp, (void *)pcachecp, sizeof(*pcachecp));   /* possibly return cp */

    _ERRCHECK(strcpy_s(output, sizeInChars, cacheout));
    return cacheout; /* return fully expanded locale string */
}

/* helpers */

int __cdecl __init_dummy(pthreadlocinfo ploci)  /* default routine for locale initializer */
{
        return 0;
}

void _strcats ( char *outstr, size_t sizeInBytes, int n, ...)
{
    int i;
    va_list substr;

    va_start (substr, n);

    for (i =0; i<n; i++)
    {
        _ERRCHECK(strcat_s(outstr, sizeInBytes, va_arg(substr, char *)));
    }
    va_end(substr);
}

int __lc_strtolc ( LC_STRINGS *names, const char *locale)
{
    int i;
    size_t len;
    char ch;

    memset((void *)names, '\0', sizeof(LC_STRINGS));  /* clear out result */

    if (*locale=='\0')
        return 0; /* trivial case */

    /* only code page is given */
    if (locale[0] == '.' && locale[1] != '\0')
    {
        _ERRCHECK(strncpy_s(names->szCodePage, _countof(names->szCodePage), &locale[1], MAX_CP_LEN-1));
        /* Make sure to null terminate the string in case locale is > MAX_CP_LEN */
        names->szCodePage[ MAX_CP_LEN -1] = 0;
        return 0;
    }

    for (i=0; ; i++)
    {
        if (!(len=strcspn(locale,"_.,")))
            return -1;  /* syntax error */

        ch = locale[len];

        if ((i==0) && (len<MAX_LANG_LEN) && (ch!='.'))
            _ERRCHECK(strncpy_s(names->szLanguage, _countof(names->szLanguage), locale, len));

        else if ((i==1) && (len<MAX_CTRY_LEN) && (ch!='_'))
            _ERRCHECK(strncpy_s(names->szCountry, _countof(names->szCountry), locale, len));

        else if ((i==2) && (len<MAX_CP_LEN) && (ch=='\0' || ch==','))
            _ERRCHECK(strncpy_s(names->szCodePage, _countof(names->szCodePage), locale, len));

        else
            return -1;  /* error parsing locale string */

        if (ch==',')
        {
            /* modifier not used in current implementation, but it
               must be parsed to for POSIX/XOpen conformance */
        /*  strncpy(names->szModifier, locale, MAX_MODIFIER_LEN-1); */
            break;
        }

        if (!ch)
            break;
        locale+=(len+1);
    }
    return 0;
}

void __lc_lctostr ( char *locale, size_t sizeInBytes, const LC_STRINGS *names)
{
    _ERRCHECK(strcpy_s(locale, sizeInBytes, (char *)names->szLanguage));
    if (*(names->szCountry))
        _strcats(locale, sizeInBytes, 2, "_", names->szCountry);
    if (*(names->szCodePage))
        _strcats(locale, sizeInBytes, 2, ".", names->szCodePage);
}


#endif  /* !defined (_WIN32) */
