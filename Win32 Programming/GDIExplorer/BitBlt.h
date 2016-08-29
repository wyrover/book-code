// BitBlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitBlt dialog

class CBitBlt : public CBlt
{
        DECLARE_DYNCREATE(CBitBlt)

// Construction
public:
        CBitBlt();
        ~CBitBlt();

// Dialog Data
        CBitRop c_Rop;
        enum { IDD = CBlt::IDD };
        //{{AFX_DATA(CBitBlt)
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CBitBlt)
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
        BOOL hasPat();

        // Generated message map functions
        //{{AFX_MSG(CBitBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokRop();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
