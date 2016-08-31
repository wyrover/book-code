// FontSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontSetup dialog

class CFontSetup : public CDialog
{
// Construction
public:
        CFontSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CFontSetup)
        enum { IDD = IDD_FONT_SETUP };
        CEdit   c_FontInfoName;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFontSetup)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(CFontSetup)
        afx_msg void OnChange();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
