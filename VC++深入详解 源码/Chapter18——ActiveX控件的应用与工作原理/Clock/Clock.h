#if !defined(AFX_CLOCK_H__B718F913_4FC8_41D2_A826_E3A277A30A4E__INCLUDED_)
#define AFX_CLOCK_H__B718F913_4FC8_41D2_A826_E3A277A30A4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Clock.h : main header file for CLOCK.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CClockApp : See Clock.cpp for implementation.

class CClockApp : public COleControlModule
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

#endif // !defined(AFX_CLOCK_H__B718F913_4FC8_41D2_A826_E3A277A30A4E__INCLUDED)
