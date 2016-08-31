// PatBlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatBlt dialog

class CPatBlt : public CBlt
{
        DECLARE_DYNCREATE(CPatBlt)

// Construction
public:
        CPatBlt();
        ~CPatBlt();

// Dialog Data
        CPatRop c_Rop;

        enum { IDD = CBlt::IDD };
        //{{AFX_DATA(CPatBlt)
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CPatBlt)
        public:
        virtual BOOL OnSetActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        CBrush brush;
        virtual void makeOp();
        virtual void updateControls();
        virtual BOOL hasSource();
        virtual void doBLT();
        virtual void loadRop(BOOL ext);
        virtual void enableOp();

        // Generated message map functions
        //{{AFX_MSG(CPatBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokRop();
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
