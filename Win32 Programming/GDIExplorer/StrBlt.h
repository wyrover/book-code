// StrBlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStretchBlt dialog


typedef struct {
        orgext src;
        orgext dst;
               } strparms;


class CStretchBlt : public CGenBlt
{
        DECLARE_DYNCREATE(CStretchBlt)

// Construction
public:
        CStretchBlt();
        ~CStretchBlt();

// Dialog Data
        enum { IDD = CGenBlt::IDD };
        //{{AFX_DATA(CStretchBlt)
        CBitRop c_Rop;
        CStatic c_c_StretchBltMode;
        CStretchBltOp   c_StretchBlt;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CStretchBlt)
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
        virtual int getStrMode();
        virtual CString getStrModeStr();
        virtual void loadRop(BOOL ext);
        virtual void enableOp();
        void getParms(strparms * p);

        CBrush brush;

        // Generated message map functions
        //{{AFX_MSG(CStretchBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokRop();
        afx_msg void OnSelendokStretchblt();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
