// Locales.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "WinNLS.h"
#include "Locales.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))


/////////////////////////////////////////////////////////////////////////////
// CLocales

CLocales::CLocales()
{
}

CLocales::~CLocales()
{
}


BEGIN_MESSAGE_MAP(CLocales, CComboBox)
        //{{AFX_MSG_MAP(CLocales)
                // NOTE - the ClassWizard will add and remove mapping macros here.
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// This rather ugly hack is required because the iterator function for
// locales does not take a 32-bit user-specified parameter.  Note that this
// is at least thread-safe, although it is not re-entrant (but it shouldn't
// need to be).

static _declspec(thread) CComboBox * locales;

static BOOL CALLBACK EnumLocalesProc(LPTSTR locale)
{
 // The locale string is an 8-digit hex number
 DWORD lcid = strtoul(locale, NULL, 16);
 TCHAR EngLanguage[256];
 TCHAR SLanguage[256];
 CString name;

 GetLocaleInfo(lcid, LOCALE_SENGLANGUAGE, EngLanguage, DIM(EngLanguage));
 GetLocaleInfo(lcid, LOCALE_SLANGUAGE, SLanguage, DIM(SLanguage));

 // EngLanguage contains a name like "Spanish"
 // SLanguage contains a name like "Brazilian Spanish"

 // Convert these to a printable string

 name = (LPCTSTR)EngLanguage;
 if(lstrcmp(EngLanguage, SLanguage) != 0)
    { /* has qualifier */
     LPTSTR pos = _tcsstr(SLanguage, EngLanguage);
     if(pos != NULL)
        { /* found it */
         *(--pos) = _T('\0');
         name += _T(" (");
         name += SLanguage;
         name += _T(")");
        } /* found it */
    } /* has qualifier */
 CString num;
 num.Format(_T(" - %04x"), lcid);
 name += num;
 int i = locales->AddString(name);
 locales->SetItemData(i, lcid);
 return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLocales message handlers

/****************************************************************************
*                               CLocales::load
* Result: void
*       
* Effect: 
*       Loads up the set of locales; sets the initial selection to the
*       current default
****************************************************************************/

void CLocales::load()
    {
     locales = this; // ugly hack due to deficiency in enumerator

     ::EnumSystemLocales(EnumLocalesProc, LCID_INSTALLED);

     // Now select the one which is our current default

     int select = 0;
     DWORD current_locale = GetUserDefaultLCID();
     for(int i = 0; i < GetCount(); i++)
        { /* scan each */
         DWORD data = GetItemData(i);
         if(data == current_locale)
            { /* found it */
             select = i;
             break;
            } /* found it */
        } /* scan each */
     SetCurSel(select);

    }
