// FontExplorer.h : main header file for the FONTEXPLORER application
//

#ifndef __AFXWIN_H__
        #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerApp:
// See FontExplorer.cpp for the implementation of this class
//

class CFontExplorerApp : public CWinApp
{
public:
        CFontExplorerApp();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFontExplorerApp)
        public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

// Implementation

        //{{AFX_MSG(CFontExplorerApp)
        afx_msg void OnAppAbout();
        afx_msg void OnFontexplorer();
        afx_msg void OnLogfont();
        afx_msg void OnFontsetup();
        afx_msg void OnKernexp();
        afx_msg void OnGetcharplacement();
        afx_msg void OnUpdateGetcharplacement(CCmdUI* pCmdUI);
        afx_msg void OnRaster();
        afx_msg void OnStockfont();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
