// GenBlt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlgBlt dialog

class CGenBlt : public CBlt
{
        DECLARE_DYNCREATE(CGenBlt)

// Construction
public:
        CGenBlt();
        CGenBlt(int id);
        ~CGenBlt();

// Dialog Data
        enum { IDD = CBlt::IDD };
        //{{AFX_DATA(CGenBlt)
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CGenBlt)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        virtual void updateControls();
        virtual BOOL hasSource();
        virtual int getStrMode();
        virtual CString getStrModeStr();
        virtual void setLastError();

        // Generated message map functions
        //{{AFX_MSG(CGenBlt)
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokBitmap();
        afx_msg void OnChangePlgx();
        afx_msg void OnChangePlgy();
        afx_msg void OnChangeXmask();
        afx_msg void OnChangeYmask();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};
