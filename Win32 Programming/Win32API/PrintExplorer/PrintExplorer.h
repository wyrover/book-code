// PrintExplorer.h : main header file for the PRINTEXPLORER application
//

#ifndef __AFXWIN_H__
        #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerApp:
// See PrintExplorer.cpp for the implementation of this class
//

class CPrintExplorerApp : public CWinApp
{
public:
        CPrintExplorerApp();
        void doPrintDlg(CView * view);
        PRINTDLG pd;

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPrintExplorerApp)
        public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

// Implementation

        //{{AFX_MSG(CPrintExplorerApp)
        afx_msg void OnAppAbout();
        afx_msg void OnPrintdlg();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
