/***
*getqloc.c - get qualified locale
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines __get_qualified_locale - get complete locale information
*
*******************************************************************************/

#include <cruntime.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <setlocal.h>
#include <awint.h>
#include <mtdll.h>
#include <internal.h>
#include <winnls.h>


//  local defines
#define __LCID_DEFAULT  0x1     //  default language locale for country
#define __LCID_PRIMARY  0x2     //  primary language locale for country
#define __LCID_FULL     0x4     //  fully matched language locale for country
#define __LCID_LANGUAGE 0x100   //  language default seen
#define __LCID_EXISTS   0x200   //  language is installed

//  local structure definitions
typedef struct tagLOCALETAB
{
    CHAR *  szName;
    CHAR    chAbbrev[4];
} LOCALETAB;

typedef struct tagRGLOCINFO
{
    LCID        lcid;
    char        chILanguage[8];
    char *      pchSEngLanguage;
    char        chSAbbrevLangName[4];
    char *      pchSEngCountry;
    char        chSAbbrevCtryName[4];
    char        chIDefaultCodepage[8];
    char        chIDefaultAnsiCodepage[8];
} RGLOCINFO;

//  function prototypes
BOOL __cdecl __get_qualified_locale(const LPLC_STRINGS, LPLC_ID, LPLC_STRINGS);
static BOOL TranslateName(const LOCALETAB *, int, const char **);

static void GetLcidFromLangCountry (_psetloc_struct _psetloc_data);
static BOOL CALLBACK LangCountryEnumProc(LPSTR);

static void GetLcidFromLanguage (_psetloc_struct _psetloc_data);
static BOOL CALLBACK LanguageEnumProc(LPSTR);

static void GetLcidFromCountry (_psetloc_struct _psetloc_data);
static BOOL CALLBACK CountryEnumProc(LPSTR);

static void GetLcidFromDefault (_psetloc_struct _psetloc_data);

static int ProcessCodePage (LPSTR lpCodePageStr, _psetloc_struct _psetloc_data);
static BOOL TestDefaultCountry(LCID);
static BOOL TestDefaultLanguage (LCID lcid, BOOL bTestPrimary, _psetloc_struct _psetloc_data);

static int __stdcall crtGetLocaleInfoA(LCID, LCTYPE, LPSTR, int);

static LCID LcidFromHexString(LPSTR);
static int GetPrimaryLen(LPSTR);

//  non-NLS language string table
__declspec(selectany) const LOCALETAB __rg_language[] =
{
    {"american",                    "ENU"},
    {"american english",            "ENU"},
    {"american-english",            "ENU"},
    {"australian",                  "ENA"},
    {"belgian",                     "NLB"},
    {"canadian",                    "ENC"},
    {"chh",                         "ZHH"},
    {"chi",                         "ZHI"},
    {"chinese",                     "CHS"},
    {"chinese-hongkong",            "ZHH"},
    {"chinese-simplified",          "CHS"},
    {"chinese-singapore",           "ZHI"},
    {"chinese-traditional",         "CHT"},
    {"dutch-belgian",               "NLB"},
    {"english-american",            "ENU"},
    {"english-aus",                 "ENA"},
    {"english-belize",              "ENL"},
    {"english-can",                 "ENC"},
    {"english-caribbean",           "ENB"},
    {"english-ire",                 "ENI"},
    {"english-jamaica",             "ENJ"},
    {"english-nz",                  "ENZ"},
    {"english-south africa",        "ENS"},
    {"english-trinidad y tobago",   "ENT"},
    {"english-uk",                  "ENG"},
    {"english-us",                  "ENU"},
    {"english-usa",                 "ENU"},
    {"french-belgian",              "FRB"},
    {"french-canadian",             "FRC"},
    {"french-luxembourg",           "FRL"},
    {"french-swiss",                "FRS"},
    {"german-austrian",             "DEA"},
    {"german-lichtenstein",         "DEC"},
    {"german-luxembourg",           "DEL"},
    {"german-swiss",                "DES"},
    {"irish-english",               "ENI"},
    {"italian-swiss",               "ITS"},
    {"norwegian",                   "NOR"},
    {"norwegian-bokmal",            "NOR"},
    {"norwegian-nynorsk",           "NON"},
    {"portuguese-brazilian",        "PTB"},
    {"spanish-argentina",           "ESS"},
    {"spanish-bolivia",             "ESB"},
    {"spanish-chile",               "ESL"},
    {"spanish-colombia",            "ESO"},
    {"spanish-costa rica",          "ESC"},
    {"spanish-dominican republic",  "ESD"},
    {"spanish-ecuador",             "ESF"},
    {"spanish-el salvador",         "ESE"},
    {"spanish-guatemala",           "ESG"},
    {"spanish-honduras",            "ESH"},
    {"spanish-mexican",             "ESM"},
    {"spanish-modern",              "ESN"},
    {"spanish-nicaragua",           "ESI"},
    {"spanish-panama",              "ESA"},
    {"spanish-paraguay",            "ESZ"},
    {"spanish-peru",                "ESR"},
    {"spanish-puerto rico",         "ESU"},
    {"spanish-uruguay",             "ESY"},
    {"spanish-venezuela",           "ESV"},
    {"swedish-finland",             "SVF"},
    {"swiss",                       "DES"},
    {"uk",                          "ENG"},
    {"us",                          "ENU"},
    {"usa",                         "ENU"}
};

//  non-NLS country/region string table
__declspec( selectany ) const LOCALETAB __rg_country[] =
{
    {"america",                     "USA"},
    {"britain",                     "GBR"},
    {"china",                       "CHN"},
    {"czech",                       "CZE"},
    {"england",                     "GBR"},
    {"great britain",               "GBR"},
    {"holland",                     "NLD"},
    {"hong-kong",                   "HKG"},
    {"new-zealand",                 "NZL"},
    {"nz",                          "NZL"},
    {"pr china",                    "CHN"},
    {"pr-china",                    "CHN"},
    {"puerto-rico",                 "PRI"},
    {"slovak",                      "SVK"},
    {"south africa",                "ZAF"},
    {"south korea",                 "KOR"},
    {"south-africa",                "ZAF"},
    {"south-korea",                 "KOR"},
    {"trinidad & tobago",           "TTO"},
    {"uk",                          "GBR"},
    {"united-kingdom",              "GBR"},
    {"united-states",               "USA"},
    {"us",                          "USA"},
};

//  LANGID's of locales of nondefault languages
__declspec( selectany ) const LANGID __rglangidNotDefault[] =
{
    MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_CANADIAN),
    MAKELANGID(LANG_SERBIAN, SUBLANG_SERBIAN_CYRILLIC),
    MAKELANGID(LANG_GERMAN, SUBLANG_GERMAN_LUXEMBOURG),
    MAKELANGID(LANG_AFRIKAANS, SUBLANG_DEFAULT),
    MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_BELGIAN),
    MAKELANGID(LANG_BASQUE, SUBLANG_DEFAULT),
    MAKELANGID(LANG_CATALAN, SUBLANG_DEFAULT),
    MAKELANGID(LANG_FRENCH, SUBLANG_FRENCH_SWISS),
    MAKELANGID(LANG_ITALIAN, SUBLANG_ITALIAN_SWISS),
    MAKELANGID(LANG_SWEDISH, SUBLANG_SWEDISH_FINLAND)
};

/***
*BOOL __get_qualified_locale - return fully qualified locale
*
*Purpose:
*       get default locale, qualify partially complete locales
*
*Entry:
*       lpInStr - input strings to be qualified
*       lpOutId - pointer to numeric LCIDs and codepage output
*       lpOutStr - pointer to string LCIDs and codepage output
*
*Exit:
*       TRUE if success, qualified locale is valid
*       FALSE if failure
*
*Exceptions:
*
*******************************************************************************/
BOOL __cdecl __get_qualified_locale(const LPLC_STRINGS lpInStr, LPLC_ID lpOutId,
                                    LPLC_STRINGS lpOutStr)
{
    int     iCodePage;
    unsigned int osplatform = 0;
    _psetloc_struct    _psetloc_data = &_getptd()->_setloc_data;

    //  initialize pointer to call locale info routine based on operating system

    if (!lpInStr)
    {
        //  if no input defined, just use default LCID
        GetLcidFromDefault(_psetloc_data);
    }
    else
    {
        _psetloc_data->pchLanguage = lpInStr->szLanguage;

        //  convert non-NLS country strings to three-letter abbreviations
        _psetloc_data->pchCountry = lpInStr->szCountry;
        if (_psetloc_data->pchCountry && *_psetloc_data->pchCountry)
            TranslateName(__rg_country,
                          sizeof(__rg_country) / sizeof(LOCALETAB) - 1,
                          &_psetloc_data->pchCountry);

        _psetloc_data->iLcidState = 0;

        if (_psetloc_data->pchLanguage && *_psetloc_data->pchLanguage)
        {
            if (_psetloc_data->pchCountry && *_psetloc_data->pchCountry)
            {
                //  both language and country strings defined
                GetLcidFromLangCountry(_psetloc_data);
            }
            else
            {
                //  language string defined, but country string undefined
                GetLcidFromLanguage(_psetloc_data);
            }

            if (!_psetloc_data->iLcidState) {
                //  first attempt failed, try substituting the language name
                //  convert non-NLS language strings to three-letter abbrevs
                if (TranslateName(__rg_language,
                                  sizeof(__rg_language) / sizeof(LOCALETAB) - 1,
                                  &_psetloc_data->pchLanguage))
                {
                    if (_psetloc_data->pchCountry && *_psetloc_data->pchCountry)
                    {
                        GetLcidFromLangCountry(_psetloc_data);
                    }
                    else
                    {
                        GetLcidFromLanguage(_psetloc_data);
                    }
                }
            }
        }
        else
        {
            if (_psetloc_data->pchCountry && *_psetloc_data->pchCountry)
            {
                //  country string defined, but language string undefined
                GetLcidFromCountry(_psetloc_data);
            }
            else
            {
                //  both language and country strings undefined
                GetLcidFromDefault(_psetloc_data);
            }
        }
    }

    //  test for error in LCID processing
    if (!_psetloc_data->iLcidState)
        return FALSE;

    //  process codepage value
    iCodePage = ProcessCodePage(lpInStr ? lpInStr->szCodePage: NULL, _psetloc_data);

    //  verify codepage validity
    if (!iCodePage || iCodePage == CP_UTF7 || iCodePage == CP_UTF8 ||
        !IsValidCodePage((WORD)iCodePage))
        return FALSE;

    //  verify locale is installed
    if (!IsValidLocale(_psetloc_data->lcidLanguage, LCID_INSTALLED))
        return FALSE;

    //  set numeric LCID and codepage results
    if (lpOutId)
    {
        lpOutId->wLanguage = LANGIDFROMLCID(_psetloc_data->lcidLanguage);
        lpOutId->wCountry = LANGIDFROMLCID(_psetloc_data->lcidCountry);
        lpOutId->wCodePage = (WORD)iCodePage;
    }

    //  set string language, country, and codepage results
    if (lpOutStr)
    {
        // Norwegian-Nynorsk is special case because Langauge and country pair
        // for Norwegian-Nynorsk and Norwegian is same ie. Norwegian_Norway
        if ( lpOutId->wLanguage ==  0x0814)
            _ERRCHECK(strcpy_s(lpOutStr->szLanguage, _countof(lpOutStr->szLanguage), "Norwegian-Nynorsk"));
        else if (GetLocaleInfoA(_psetloc_data->lcidLanguage, LOCALE_SENGLANGUAGE,
                                 lpOutStr->szLanguage, MAX_LANG_LEN) == 0)
            return FALSE;
        if (GetLocaleInfoA(_psetloc_data->lcidCountry, LOCALE_SENGCOUNTRY,
                                 lpOutStr->szCountry, MAX_CTRY_LEN) == 0)
            return FALSE;
        _itoa_s((int)iCodePage, (char *)lpOutStr->szCodePage, MAX_CP_LEN, 10);
    }
    return TRUE;
}

/***
*BOOL TranslateName - convert known non-NLS string to NLS equivalent
*
*Purpose:
*   Provide compatibility with existing code for non-NLS strings
*
*Entry:
*   lpTable  - pointer to LOCALETAB used for translation
*   high     - maximum index of table (size - 1)
*   ppchName - pointer to pointer of string to translate
*
*Exit:
*   ppchName - pointer to pointer of string possibly translated
*   TRUE if string translated, FALSE if unchanged
*
*Exceptions:
*
*******************************************************************************/
static BOOL TranslateName (
    const LOCALETAB * lpTable,
    int               high,
    const char **     ppchName)
{
    int     i;
    int     cmp = 1;
    int     low = 0;

    //  typical binary search - do until no more to search or match
    while (low <= high && cmp != 0)
    {
        i = (low + high) / 2;
        cmp = _stricmp(*ppchName, (const char *)(*(lpTable + i)).szName);

        if (cmp == 0)
            *ppchName = (*(lpTable + i)).chAbbrev;
        else if (cmp < 0)
            high = i - 1;
        else
            low = i + 1;
    }

    return !cmp;
}

/***
*void GetLcidFromLangCountry - get LCIDs from language and country strings
*
*Purpose:
*   Match the best LCIDs to the language and country string given.
*   After global variables are initialized, the LangCountryEnumProc
*   routine is registered as an EnumSystemLocalesA callback to actually
*   perform the matching as the LCIDs are enumerated.
*
*Entry:
*   pchLanguage     - language string
*   bAbbrevLanguage - language string is a three-letter abbreviation
*   pchCountry      - country string
*   bAbbrevCountry  - country string ia a three-letter abbreviation
*   iPrimaryLen     - length of language string with primary name
*
*Exit:
*   lcidLanguage - LCID of language string
*   lcidCountry  - LCID of country string
*
*Exceptions:
*
*******************************************************************************/
static void GetLcidFromLangCountry (_psetloc_struct _psetloc_data)
{
    //  initialize static variables for callback use
    _psetloc_data->bAbbrevLanguage = strlen(_psetloc_data->pchLanguage) == 3;
    _psetloc_data->bAbbrevCountry = strlen(_psetloc_data->pchCountry) == 3;
    _psetloc_data->lcidLanguage = 0;
    _psetloc_data->iPrimaryLen = _psetloc_data->bAbbrevLanguage ?
                             2 : GetPrimaryLen(_psetloc_data->pchLanguage);

    EnumSystemLocalesA(LangCountryEnumProc, LCID_INSTALLED);

    //  locale value is invalid if the language was not installed or the language
    //  was not available for the country specified
    if (!(_psetloc_data->iLcidState & __LCID_LANGUAGE) ||
        !(_psetloc_data->iLcidState & __LCID_EXISTS) ||
        !(_psetloc_data->iLcidState & (__LCID_FULL |
                                    __LCID_PRIMARY |
                                    __LCID_DEFAULT)))
        _psetloc_data->iLcidState = 0;
}

/***
*BOOL CALLBACK LangCountryEnumProc - callback routine for GetLcidFromLangCountry
*
*Purpose:
*   Determine if LCID given matches the language in pchLanguage
*   and country in pchCountry.
*
*Entry:
*   lpLcidString   - pointer to string with decimal LCID
*   pchCountry     - pointer to country name
*   bAbbrevCountry - set if country is three-letter abbreviation
*
*Exit:
*   iLcidState   - status of match
*       __LCID_FULL - both language and country match (best match)
*       __LCID_PRIMARY - primary language and country match (better)
*       __LCID_DEFAULT - default language and country match (good)
*       __LCID_LANGUAGE - default primary language exists
*       __LCID_EXISTS - full match of language string exists
*       (Overall match occurs for the best of FULL/PRIMARY/DEFAULT
*        and LANGUAGE/EXISTS both set.)
*   lcidLanguage - LCID matched
*   lcidCountry  - LCID matched
*   FALSE if match occurred to terminate enumeration, else TRUE.
*
*Exceptions:
*
*******************************************************************************/
static BOOL CALLBACK LangCountryEnumProc (LPSTR lpLcidString)
{
    _psetloc_struct    _psetloc_data = &_getptd()->_setloc_data;
    LCID    lcid = LcidFromHexString(lpLcidString);
    char    rgcInfo[120];

    //  test locale country against input value
    if (GetLocaleInfoA(lcid,
                             _psetloc_data->bAbbrevCountry ?
                             LOCALE_SABBREVCTRYNAME : LOCALE_SENGCOUNTRY,
                             rgcInfo, sizeof(rgcInfo)) == 0)
    {
        //  set error condition and exit
        _psetloc_data->iLcidState = 0;
        return TRUE;
    }
    if (!_stricmp(_psetloc_data->pchCountry, rgcInfo))
    {
        //  country matched - test for language match
        if (GetLocaleInfoA(lcid,
                                 _psetloc_data->bAbbrevLanguage ?
                                 LOCALE_SABBREVLANGNAME : LOCALE_SENGLANGUAGE,
                                 rgcInfo, sizeof(rgcInfo)) == 0)
        {
            //  set error condition and exit
            _psetloc_data->iLcidState = 0;
            return TRUE;
        }
        if (!_stricmp(_psetloc_data->pchLanguage, rgcInfo))
        {
            //  language matched also - set state and value
            _psetloc_data->iLcidState |= (__LCID_FULL |
                                       __LCID_LANGUAGE |
                                       __LCID_EXISTS);
            _psetloc_data->lcidLanguage = _psetloc_data->lcidCountry = lcid;
        }

        //  test if match already for primary langauage
        else if (!(_psetloc_data->iLcidState & __LCID_PRIMARY))
        {
            //  if not, use _psetloc_data->iPrimaryLen to partial match language string
            if (_psetloc_data->iPrimaryLen && !_strnicmp(_psetloc_data->pchLanguage, rgcInfo, _psetloc_data->iPrimaryLen))
            {
                //  primary language matched - set state and country LCID
                _psetloc_data->iLcidState |= __LCID_PRIMARY;
                _psetloc_data->lcidCountry = lcid;

                //  if language is primary only (no subtype), set language LCID
                if ((int)strlen(_psetloc_data->pchLanguage) == _psetloc_data->iPrimaryLen)
                    _psetloc_data->lcidLanguage = lcid;
            }

            //  test if default language already defined
            else if (!(_psetloc_data->iLcidState & __LCID_DEFAULT))
            {
                //  if not, test if locale language is default for country
                if (TestDefaultCountry(lcid))
                {
                    //  default language for country - set state, value
                    _psetloc_data->iLcidState |= __LCID_DEFAULT;
                    _psetloc_data->lcidCountry = lcid;
                }
            }
        }
    }
    //  test if input language both exists and default primary language defined
    if ((_psetloc_data->iLcidState & (__LCID_LANGUAGE | __LCID_EXISTS)) !=
                      (__LCID_LANGUAGE | __LCID_EXISTS))
    {
        //  test language match to determine whether it is installed
        if (GetLocaleInfoA(lcid, _psetloc_data->bAbbrevLanguage ? LOCALE_SABBREVLANGNAME
                                                       : LOCALE_SENGLANGUAGE,
                           rgcInfo, sizeof(rgcInfo)) == 0)
        {
            //  set error condition and exit
            _psetloc_data->iLcidState = 0;
            return TRUE;
        }

        if (!_stricmp(_psetloc_data->pchLanguage, rgcInfo))
        {
            //  language matched - set bit for existance
            _psetloc_data->iLcidState |= __LCID_EXISTS;

            if (_psetloc_data->bAbbrevLanguage)
            {
                //  abbreviation - set state
                //  also set language LCID if not set already
                _psetloc_data->iLcidState |= __LCID_LANGUAGE;
                if (!_psetloc_data->lcidLanguage)
                    _psetloc_data->lcidLanguage = lcid;
            }

            //  test if language is primary only (no sublanguage)
            else if (_psetloc_data->iPrimaryLen && ((int)strlen(_psetloc_data->pchLanguage) == _psetloc_data->iPrimaryLen))
            {
                //  primary language only - test if default LCID
                if (TestDefaultLanguage(lcid, TRUE, _psetloc_data))
                {
                    //  default primary language - set state
                    //  also set LCID if not set already
                    _psetloc_data->iLcidState |= __LCID_LANGUAGE;
                    if (!_psetloc_data->lcidLanguage)
                        _psetloc_data->lcidLanguage = lcid;
                }
            }
            else
            {
                //  language with sublanguage - set state
                //  also set LCID if not set already
                _psetloc_data->iLcidState |= __LCID_LANGUAGE;
                if (!_psetloc_data->lcidLanguage)
                    _psetloc_data->lcidLanguage = lcid;
            }
        }
        else if (!_psetloc_data->bAbbrevLanguage && _psetloc_data->iPrimaryLen
                               && !_stricmp(_psetloc_data->pchLanguage, rgcInfo))
        {
            //  primary language match - test for default language only
            if (TestDefaultLanguage(lcid, FALSE, _psetloc_data))
            {
                //  default primary language - set state
                //  also set LCID if not set already
                _psetloc_data->iLcidState |= __LCID_LANGUAGE;
                if (!_psetloc_data->lcidLanguage)
                    _psetloc_data->lcidLanguage = lcid;
            }
        }
    }

    //  if LOCALE_FULL set, return FALSE to stop enumeration,
    //  else return TRUE to continue
    return (_psetloc_data->iLcidState & __LCID_FULL) == 0;
}

/***
*void GetLcidFromLanguage - get LCIDs from language string
*
*Purpose:
*   Match the best LCIDs to the language string given.  After global
*   variables are initialized, the LanguageEnumProc routine is
*   registered as an EnumSystemLocalesA callback to actually perform
*   the matching as the LCIDs are enumerated.
*
*Entry:
*   pchLanguage     - language string
*   bAbbrevLanguage - language string is a three-letter abbreviation
*   iPrimaryLen     - length of language string with primary name
*
*Exit:
*   lcidLanguage - lcidCountry  - LCID of language with default
*                                 country
*
*Exceptions:
*
*******************************************************************************/
static void GetLcidFromLanguage (_psetloc_struct _psetloc_data)
{
    //  initialize static variables for callback use
    _psetloc_data->bAbbrevLanguage = strlen(_psetloc_data->pchLanguage) == 3;
    _psetloc_data->iPrimaryLen = _psetloc_data->bAbbrevLanguage ? 2 : GetPrimaryLen(_psetloc_data->pchLanguage);

    EnumSystemLocalesA(LanguageEnumProc, LCID_INSTALLED);

    //  locale value is invalid if the language was not installed
    //  or the language was not available for the country specified
    if (!(_psetloc_data->iLcidState & __LCID_FULL))
        _psetloc_data->iLcidState = 0;
}

/***
*BOOL CALLBACK LanguageEnumProc - callback routine for GetLcidFromLanguage
*
*Purpose:
*   Determine if LCID given matches the default country for the
*   language in pchLanguage.
*
*Entry:
*   lpLcidString    - pointer to string with decimal LCID
*   pchLanguage     - pointer to language name
*   bAbbrevLanguage - set if language is three-letter abbreviation
*
*Exit:
*   lcidLanguage - lcidCountry - LCID matched
*   FALSE if match occurred to terminate enumeration, else TRUE.
*
*Exceptions:
*
*******************************************************************************/
static BOOL CALLBACK LanguageEnumProc (LPSTR lpLcidString)
{
    _psetloc_struct    _psetloc_data = &_getptd()->_setloc_data;
    LCID    lcid = LcidFromHexString(lpLcidString);
    char    rgcInfo[120];

    //  test locale for language specified
    if (GetLocaleInfoA(lcid, _psetloc_data->bAbbrevLanguage ? LOCALE_SABBREVLANGNAME
                                                   : LOCALE_SENGLANGUAGE,
                       rgcInfo, sizeof(rgcInfo)) == 0)
    {
        //  set error condition and exit
        _psetloc_data->iLcidState = 0;
        return TRUE;
    }

    if (!_stricmp(_psetloc_data->pchLanguage, rgcInfo))
    {
        //  language matched - test if locale country is default
        //  or if locale is implied in the language string
        if (_psetloc_data->bAbbrevLanguage || TestDefaultLanguage(lcid, TRUE, _psetloc_data))
        {
            //  this locale has the default country
            _psetloc_data->lcidLanguage = _psetloc_data->lcidCountry = lcid;
            _psetloc_data->iLcidState |= __LCID_FULL;
        }
    }
    else if (!_psetloc_data->bAbbrevLanguage && _psetloc_data->iPrimaryLen
                              && !_stricmp(_psetloc_data->pchLanguage, rgcInfo))
    {
        //  primary language matched - test if locale country is default
        if (TestDefaultLanguage(lcid, FALSE, _psetloc_data))
        {
            //  this is the default country
            _psetloc_data->lcidLanguage = _psetloc_data->lcidCountry = lcid;
            _psetloc_data->iLcidState |= __LCID_FULL;
        }
    }

    return (_psetloc_data->iLcidState & __LCID_FULL) == 0;
}

/***
*void GetLcidFromCountry - get LCIDs from country string
*
*Purpose:
*   Match the best LCIDs to the country string given.  After global
*   variables are initialized, the CountryEnumProc routine is
*   registered as an EnumSystemLocalesA callback to actually perform
*   the matching as the LCIDs are enumerated.
*
*Entry:
*   pchCountry     - country string
*   bAbbrevCountry - country string is a three-letter abbreviation
*
*Exit:
*   lcidLanguage - lcidCountry  - LCID of country with default
*                                 language
*
*Exceptions:
*
*******************************************************************************/
static void GetLcidFromCountry (_psetloc_struct _psetloc_data)
{
    _psetloc_data->bAbbrevCountry = strlen(_psetloc_data->pchCountry) == 3;

    EnumSystemLocalesA(CountryEnumProc, LCID_INSTALLED);

    //  locale value is invalid if the country was not defined or
    //  no default language was found
    if (!(_psetloc_data->iLcidState & __LCID_FULL))
        _psetloc_data->iLcidState = 0;
}

/***
*BOOL CALLBACK CountryEnumProc - callback routine for GetLcidFromCountry
*
*Purpose:
*   Determine if LCID given matches the default language for the
*   country in pchCountry.
*
*Entry:
*   lpLcidString   - pointer to string with decimal LCID
*   pchCountry     - pointer to country name
*   bAbbrevCountry - set if country is three-letter abbreviation
*
*Exit:
*   lcidLanguage - lcidCountry - LCID matched
*   FALSE if match occurred to terminate enumeration, else TRUE.
*
*Exceptions:
*
*******************************************************************************/
static BOOL CALLBACK CountryEnumProc (LPSTR lpLcidString)
{
    _psetloc_struct    _psetloc_data = &_getptd()->_setloc_data;
    LCID    lcid = LcidFromHexString(lpLcidString);
    char    rgcInfo[120];

    //  test locale for country specified
    if (GetLocaleInfoA(lcid, _psetloc_data->bAbbrevCountry ? LOCALE_SABBREVCTRYNAME
                                                  : LOCALE_SENGCOUNTRY,
                       rgcInfo, sizeof(rgcInfo)) == 0)
    {
        //  set error condition and exit
        _psetloc_data->iLcidState = 0;
        return TRUE;
    }
    if (!_stricmp(_psetloc_data->pchCountry, rgcInfo))
    {
        //  language matched - test if locale country is default
        if (TestDefaultCountry(lcid))
        {
            //  this locale has the default language
            _psetloc_data->lcidLanguage = _psetloc_data->lcidCountry = lcid;
            _psetloc_data->iLcidState |= __LCID_FULL;
        }
    }
    return (_psetloc_data->iLcidState & __LCID_FULL) == 0;
}

/***
*void GetLcidFromDefault - get default LCIDs
*
*Purpose:
*   Set both language and country LCIDs to the system default.
*
*Entry:
*   None.
*
*Exit:
*   lcidLanguage - set to system LCID
*   lcidCountry  - set to system LCID
*
*Exceptions:
*
*******************************************************************************/
static void GetLcidFromDefault (_psetloc_struct _psetloc_data)
{
    _psetloc_data->iLcidState |= (__LCID_FULL | __LCID_LANGUAGE);
    _psetloc_data->lcidLanguage = _psetloc_data->lcidCountry = GetUserDefaultLCID();
}

/***
*int ProcessCodePage - convert codepage string to numeric value
*
*Purpose:
*   Process codepage string consisting of a decimal string, or the
*   special case strings "ACP" and "OCP", for ANSI and OEM codepages,
*   respectively.  Null pointer or string returns the ANSI codepage.
*
*Entry:
*   lpCodePageStr - pointer to codepage string
*
*Exit:
*   Returns numeric value of codepage.
*
*Exceptions:
*
*******************************************************************************/
static int ProcessCodePage (LPSTR lpCodePageStr, _psetloc_struct _psetloc_data)
{
    char    chCodePage[8];

    if (!lpCodePageStr || !*lpCodePageStr || !strcmp(lpCodePageStr, "ACP"))
    {
        //  get ANSI codepage for the country LCID
        if (GetLocaleInfoA(_psetloc_data->lcidCountry, LOCALE_IDEFAULTANSICODEPAGE,
                                 chCodePage, sizeof(chCodePage)) == 0)
            return 0;
        lpCodePageStr = chCodePage;
    }
    else if (!strcmp(lpCodePageStr, "OCP"))
    {
        //  get OEM codepage for the country LCID
        if (GetLocaleInfoA(_psetloc_data->lcidCountry, LOCALE_IDEFAULTCODEPAGE,
                                 chCodePage, sizeof(chCodePage)) == 0)
            return 0;
        lpCodePageStr = chCodePage;
    }

    //  convert decimal string to numeric value
    return (int)atol(lpCodePageStr);
}

/***
*BOOL TestDefaultCountry - determine if default locale for country
*
*Purpose:
*   Using a hardcoded list, determine if the locale of the given LCID
*   has the default sublanguage for the locale primary language.  The
*   list contains the locales NOT having the default sublanguage.
*
*Entry:
*   lcid - LCID of locale to test
*
*Exit:
*   Returns TRUE if default sublanguage, else FALSE.
*
*Exceptions:
*
*******************************************************************************/
static BOOL TestDefaultCountry (LCID lcid)
{
    LANGID  langid = LANGIDFROMLCID(lcid);
    int     i;

    for (i = 0; i < sizeof(__rglangidNotDefault) / sizeof(LANGID); i++)
    {
        if (langid == __rglangidNotDefault[i])
            return FALSE;
    }
    return TRUE;
}

/***
*BOOL TestDefaultLanguage - determine if default locale for language
*
*Purpose:
*   Determines if the given LCID has the default sublanguage.
*   If bTestPrimary is set, also allow TRUE when string contains an
*   implicit sublanguage.
*
*Entry:
*   LCID         - lcid of locale to test
*   bTestPrimary - set if testing if language is primary
*
*Exit:
*   Returns TRUE if sublanguage is default for locale tested.
*   If bTestPrimary set, TRUE is language has implied sublanguge.
*
*Exceptions:
*
*******************************************************************************/
static BOOL TestDefaultLanguage (LCID lcid, BOOL bTestPrimary, _psetloc_struct _psetloc_data)
{
    char    rgcInfo[120];
    LCID    lcidDefault = MAKELCID(MAKELANGID(PRIMARYLANGID(LANGIDFROMLCID(lcid)),
                                                  SUBLANG_DEFAULT), SORT_DEFAULT);

    if (GetLocaleInfoA(lcidDefault, LOCALE_ILANGUAGE, rgcInfo,
                                          sizeof(rgcInfo)) == 0)
        return FALSE;

    if (lcid != LcidFromHexString(rgcInfo))
    {
        //  test if string contains an implicit sublanguage by
        //  having a character other than upper/lowercase letters.
        if (bTestPrimary && GetPrimaryLen(_psetloc_data->pchLanguage) == (int)strlen(_psetloc_data->pchLanguage))
            return FALSE;
    }
    return TRUE;
}

/***
*LCID LcidFromHexString - convert hex string to value for LCID
*
*Purpose:
*   LCID values returned in hex ANSI strings - straight conversion
*
*Entry:
*   lpHexString - pointer to hex string to convert
*
*Exit:
*   Returns LCID computed.
*
*Exceptions:
*
*******************************************************************************/
static LCID LcidFromHexString (LPSTR lpHexString)
{
    char    ch;
    DWORD   lcid = 0;

    while (ch = *lpHexString++)
    {
        if (ch >= 'a' && ch <= 'f')
            ch += '9' + 1 - 'a';
        else if (ch >= 'A' && ch <= 'F')
            ch += '9' + 1 - 'A';
        lcid = lcid * 0x10 + ch - '0';
    }

    return (LCID)lcid;
}

/***
*int GetPrimaryLen - get length of primary language name
*
*Purpose:
*   Determine primary language string length by scanning until
*   first non-alphabetic character.
*
*Entry:
*   pchLanguage - string to scan
*
*Exit:
*   Returns length of primary language string.
*
*Exceptions:
*
*******************************************************************************/
static int GetPrimaryLen (LPSTR pchLanguage)
{
    int     len = 0;
    char    ch;

    ch = *pchLanguage++;
    while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    {
        len++;
        ch = *pchLanguage++;
    }

    return len;
}

