// ShowRgn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowRegion dialog

class CShowRegion : public CDialog
{
// Construction
public:
        CShowRegion(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CShowRegion)
        enum { IDD = IDD_RGN };
        CButton c_Monochrome;
        CRegionDisplayIntersect c_Intersected;
        CRegionDisplayContained c_Contained;
        CButton c_FrameRgn;
        CButton c_FillRgn;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CShowRegion)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        int op;
        int which;
        int how;
        void update();

        void doCopy(CWnd * wnd);

        // Generated message map functions
        //{{AFX_MSG(CShowRegion)
        afx_msg void OnRgn1();
        afx_msg void OnRgn2();
        afx_msg void OnRgnAnd();
        afx_msg void OnRgnDiff();
        afx_msg void OnRgnOr();
        afx_msg void OnRgnResult();
        afx_msg void OnRgnXor();
        virtual BOOL OnInitDialog();
        afx_msg void OnFillrgn();
        afx_msg void OnFramergn();
        afx_msg void OnCopy1();
        afx_msg void OnCopy2();
        afx_msg void OnMono();
        //}}AFX_MSG
        afx_msg LRESULT OnRgnWhat(WPARAM, LPARAM);
        afx_msg LRESULT OnRgnHow(WPARAM, LPARAM);
        afx_msg LRESULT OnRgnOp(WPARAM, LPARAM);
        afx_msg LRESULT OnRgnMono(WPARAM, LPARAM);

        DECLARE_MESSAGE_MAP()
};
