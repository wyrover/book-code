// StockFnt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStockFont dialog

class CStockFont : public CDialog
{
// Construction
public:
        CStockFont(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CStockFont)
        enum { IDD = IDD_STOCKFONTS };
        CButton c_SYSTEM_FONT;
        CButton c_SYSTEM_FIXED_FONT;
        CButton c_OEM_FIXED_FONT;
        CButton c_DEVICE_DEFAULT_FONT;
        CButton c_ANSI_VAR_FONT;
        CButton c_ANSI_FIXED_FONT;
        CStatic c_Sample;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CStockFont)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void setSample(CButton & caption, int fontid);

        // Generated message map functions
        //{{AFX_MSG(CStockFont)
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        virtual BOOL OnInitDialog();
        afx_msg void OnAnsiFixedFont();
        afx_msg void OnAnsiVarFont();
        afx_msg void OnCopy();
        afx_msg void OnDeviceDefaultFont();
        afx_msg void OnOemFixedFont();
        afx_msg void OnSystemFixedFont();
        afx_msg void OnSystemFont();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
