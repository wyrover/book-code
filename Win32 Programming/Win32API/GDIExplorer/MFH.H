// MFH.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMFH dialog

class CMFH : public CDialog
{
// Construction
public:
        CMFH(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CMFH)
        enum { IDD = IDD_MFH };
        int             m_inch;
        int             m_bottom;
        int             m_left;
        int             m_right;
        int             m_top;
        int     m_lastbottom;
        int     m_lastinch;
        int     m_lastleft;
        int     m_lastright;
        int     m_lasttop;
        int     m_OldPelX;
        int     m_OldPelY;
        int             m_PelX;
        int             m_PelY;
        BOOL    m_SetOrigin;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMFH)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(CMFH)
        afx_msg void OnUseold();
        afx_msg void OnUseoldpelxy();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
