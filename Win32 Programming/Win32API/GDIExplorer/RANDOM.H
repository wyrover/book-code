// random.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRandom dialog

class CRandom : public CDialog
{
// Construction
public:
        CRandom(CWnd* pParent = NULL);   // standard constructor
        RandomData rd;

// Dialog Data
        //{{AFX_DATA(CRandom)
        enum { IDD = IDD_RANDOM };
        CEdit   c_Sides;
        CButton c_RandomSides;
        CSpinButtonCtrl c_SpinSides;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRandom)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        // Generated message map functions
        //{{AFX_MSG(CRandom)
        virtual BOOL OnInitDialog();
        virtual void OnOK();
        afx_msg void OnRandomSides();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
