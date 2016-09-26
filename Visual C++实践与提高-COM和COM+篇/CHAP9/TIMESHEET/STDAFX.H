#if !defined(AFX_STDAFX_H__AFDC158B_F993_11D1_96ED_F05F03C10000__INCLUDED_)
#define AFX_STDAFX_H__AFDC158B_F993_11D1_96ED_F05F03C10000__INCLUDED_

#if _MSC_VER >= 1000
    #pragma once
#endif // _MSC_VER >= 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>          // MFC database classes
#include <afxdao.h>         // MFC DAO database classes

// For Form ActiveX control
#include "FormCtrl.h"

#define DELETE_EXCEPTION(e) do (e->Delete();} while (0)
#define WM_ACTIVATE_CONTROL (WM_USER+10)

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AFDC158B_F993_11D1_96ED_F05F03C10000__INCLUDED_)
