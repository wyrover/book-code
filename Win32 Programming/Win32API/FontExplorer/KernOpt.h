// KernOpt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKernOptions dialog

class CKernOptions : public CDialog
{
// Construction
public:
        CKernOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CKernOptions)
        enum { IDD = IDD_KERNOPTIONS };
        CButton c_Basal;
        CButton c_Pair;
        BOOL    m_KernedOnly;
        //}}AFX_DATA
        display_options opts;


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CKernOptions)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void enableControls();
        // Generated message map functions
        //{{AFX_MSG(CKernOptions)
        afx_msg void OnBasal();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
