// Kerning.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKerning dialog

class CKerning : public CDialog
{
// Construction
public:
        CKerning(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CKerning)
        enum { IDD = IDD_KERNING };
        CEdit   c_TrackVal;
        CStatic c_c_Track;
        CSpinButtonCtrl c_Track;
        CEdit   c_Sample;
        CFontDisplay    c_Kerned;
        CFontDisplay    c_Unkerned;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CKerning)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        BOOL kernedonly;

        void Resize();
        void enableControls();
        void enableTracking(BOOL mode);

        // Generated message map functions
        //{{AFX_MSG(CKerning)
        afx_msg void OnChangeSample();
        afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
        afx_msg void OnFont();
        virtual BOOL OnInitDialog();
        afx_msg void OnOptions();
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnCopy();
        //}}AFX_MSG
        afx_msg LRESULT OnGetLogFont(WPARAM, LPARAM);
        afx_msg LRESULT OnGetColor(WPARAM, LPARAM);
        DECLARE_MESSAGE_MAP()
};
