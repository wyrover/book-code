// PlgBlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlgBlt dialog

typedef struct {
        CPoint plg[3];
        CPoint src;
        CSize  size;
        CPoint mask;
               } plgparms;


class CPlgBlt : public CGenBlt
{
        DECLARE_DYNCREATE(CPlgBlt)

// Construction
public:
        CPlgBlt();
        ~CPlgBlt();

// Dialog Data
        enum { IDD = CGenBlt::IDD };
        //{{AFX_DATA(CPlgBlt)
        CStretchBltOp   c_Rop;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CPlgBlt)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        CBitmap mask;
        virtual void makeOp();
        virtual void updateControls();
        virtual BOOL hasSource();
        virtual void doBLT();
        virtual int getStrMode();
        virtual CString getStrModeStr();
        virtual void enableOp();
        virtual CPoint getCopyOrg();
        virtual CSize getCopyExt();

        void getParms(plgparms * p);
        BOOL isMonochrome();

        // Generated message map functions
        //{{AFX_MSG(CPlgBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokRop();
        afx_msg void OnSetmask();
        afx_msg void OnClearmask();
        afx_msg void OnChangeXmask();
        afx_msg void OnChangeYmask();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
