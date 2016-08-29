// GDI Explorer.h : main header file for the GDI EXPLORER application
//

#ifndef __AFXWIN_H__
        #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerApp:
// See GDI Explorer.cpp for the implementation of this class
//

class CGDIExplorerApp : public CWinApp
{
public:
        CGDIExplorerApp();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CGDIExplorerApp)
        public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

// Implementation

        //{{AFX_MSG(CGDIExplorerApp)
        afx_msg void OnAppAbout();
        afx_msg void OnXform();
        afx_msg void OnBltexp();
        afx_msg void OnRgnExp();
	afx_msg void OnUpdateMatexp(CCmdUI* pCmdUI);
	//}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
