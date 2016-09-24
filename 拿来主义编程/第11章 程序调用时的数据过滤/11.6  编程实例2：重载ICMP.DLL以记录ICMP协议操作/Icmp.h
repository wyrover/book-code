// Icmp.h : main header file for the ICMP DLL
//

#if !defined(AFX_ICMP_H__5005E903_411C_41EC_AF86_12441FA23DFE__INCLUDED_)
#define AFX_ICMP_H__5005E903_411C_41EC_AF86_12441FA23DFE__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp
// See Icmp.cpp for the implementation of this class
//

#include "IPHlpApi.h"

class CIcmpApp : public CWinApp
{
public:
    HMODULE hInst;
    HANDLE IcmpCreateFile();
    DWORD IcmpSendEcho(HANDLE IcmpHandle,
                       IPAddr DestinationAddress,
                       LPVOID RequestData,
                       WORD RequestSize,
                       PIP_OPTION_INFORMATION RequestOptions,
                       LPVOID ReplyBuff,
                       DWORD ReplySize,
                       DWORD Timeout);
    BOOL IcmpCloseHandle(HANDLE IcmpHandle);

    CIcmpApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIcmpApp)
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CIcmpApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICMP_H__5005E903_411C_41EC_AF86_12441FA23DFE__INCLUDED_)
