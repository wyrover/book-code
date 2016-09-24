// CWebDeviceMonitor.h : main header file for the CWEBDEVICEMONITOR application
//

#if !defined(AFX_CWEBDEVICEMONITOR_H__04A96000_513B_4264_835D_8050EBA3E469__INCLUDED_)
#define AFX_CWEBDEVICEMONITOR_H__04A96000_513B_4264_835D_8050EBA3E469__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCWebDeviceMonitorApp:
// See CWebDeviceMonitor.cpp for the implementation of this class
//

class CCWebDeviceMonitorApp : public CWinApp
{
public:
    CCWebDeviceMonitorApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCWebDeviceMonitorApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CCWebDeviceMonitorApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWEBDEVICEMONITOR_H__04A96000_513B_4264_835D_8050EBA3E469__INCLUDED_)
