// Options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptions dialog

class COptions : public CDialog
{
// Construction
public:
        COptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(COptions)
        enum { IDD = IDD_OPTIONS };
        CSpinButtonCtrl c_SpinKern;
        CButton c_SizeFixed;
        CButton c_SizeFit;
        CEdit   c_Sample;
        CString m_Sample;
        CString m_SmallSample;
        int             m_kern;
        //}}AFX_DATA
        display_options opts;


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(COptions)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(COptions)
        afx_msg void OnChangeSample();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
