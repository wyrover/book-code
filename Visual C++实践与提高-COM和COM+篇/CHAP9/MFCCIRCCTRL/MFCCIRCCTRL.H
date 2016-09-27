#if !defined(AFX_MFCCIRCCTRL_H__4A8C9F3C_2A74_44E2_A9D1_376194109822__INCLUDED_)
#define AFX_MFCCIRCCTRL_H__4A8C9F3C_2A74_44E2_A9D1_376194109822__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

// MfcCircCtrl.h : main header file for MFCCIRCCTRL.DLL

#if !defined( __AFXCTL_H__ )
    #error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcCircCtrlApp : See MfcCircCtrl.cpp for implementation.

class CMfcCircCtrlApp : public COleControlModule
{
public:
    BOOL InitInstance();
    int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCCIRCCTRL_H__4A8C9F3C_2A74_44E2_A9D1_376194109822__INCLUDED)
