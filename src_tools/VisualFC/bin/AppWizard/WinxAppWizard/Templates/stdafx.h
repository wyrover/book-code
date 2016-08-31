// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)
#define [!output FILE_NAME_SYMBOL]_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Change these values to use different versions
[!if WINX_USE_DEFSDK]
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

#define WINX_USE_DEFSDK
[!else]
//#define WINX_USE_DEFSDK
[!endif]

#define _ATL_APARTMENT_THREADED

#include <winx/Config.h>
#include <winx.h>
#include <winx/CommonDialogs.h>
[!if WINX_VIEWTYPE_SCROLL]
#include <winx/ScrollWindow.h>
[!endif]
[!if WINX_USE_GDIPLUS]
#include <winx/Gdiplus.h>
[!endif]
[!if WINX_VIEWTYPE_HTML]
#include <winx/WebBrowser.h>
[!endif]
[!if WINX_USE_LOOKNFEEL]
#include <winx/LookNFeel.h>
[!endif]

[!if WINX_APPTYPE_DLG_FRAME]
[!if WINX_USE_ADDVFCFILES]
#include "framedlg.h"
[!else]
#include <framedlg.h>
[!endif]
[!endif]

[!if WINX_USE_EMBEDDED_MANIFEST]

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
[!endif]

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// [!output INSERT_LOCATION_COMMENT]

#endif // !defined([!output FILE_NAME_SYMBOL]_INCLUDED_)
