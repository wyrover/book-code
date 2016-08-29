// StrBlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaskBlt dialog

typedef struct {
        CPoint src;
        orgext dst;
        CPoint mask;
               } mskparms;


class CMaskBlt : public CGenBlt
{
        DECLARE_DYNCREATE(CMaskBlt)

// Construction
public:
        CMaskBlt();
        ~CMaskBlt();

// Dialog Data
        enum { IDD = CGenBlt::IDD };
        //{{AFX_DATA(CMaskBlt)
        CBitRop c_Rop;
        CBitRop c_Rop2;
        CStatic c_c_Rop2;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CMaskBlt)
        public:
        virtual BOOL OnSetActive();
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        virtual void makeOp();
        virtual void updateControls();
        virtual BOOL hasSource();
        virtual BOOL hasPat();
        virtual void doBLT();
        virtual void loadRop(BOOL ext);
        virtual int getStrMode();
        virtual CString getStrModeStr();
        virtual void enableOp();
        void getParms(mskparms * p);
        BOOL isMonochrome();

        CBrush brush;
        CBitmap mask;

        // Generated message map functions
        //{{AFX_MSG(CMaskBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokRop();
        afx_msg void OnSelendokRop2();
        afx_msg void OnSetmask();
        afx_msg void OnClearmask();
        afx_msg void OnChangeXmask();
        afx_msg void OnChangeYmask();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
