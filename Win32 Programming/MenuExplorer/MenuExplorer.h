// MenuExplorer.h : main header file for the MENUEXPLORER application
//

#ifndef __AFXWIN_H__
        #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMenuExplorerApp:
// See MenuExplorer.cpp for the implementation of this class
//

class CMenuExplorerApp : public CWinApp
{
public:
        CMenuExplorerApp();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMenuExplorerApp)
        public:
        virtual BOOL InitInstance();
        //}}AFX_VIRTUAL

// Implementation

        //{{AFX_MSG(CMenuExplorerApp)
        afx_msg void OnAppAbout();
        afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
        afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
        afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
        afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
