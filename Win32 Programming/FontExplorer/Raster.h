// Raster.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRaster dialog

class CRaster : public CDialog
{
// Construction
public:
        CRaster(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CRaster)
        enum { IDD = IDD_RASTER };
        CStatic c_c_Multiplier;
        CStatic c_c_Height;
        CButton c_Vector;
        CButton c_TrueType;
        CButton c_Raster;
        CIntEdit        c_Multiplier;
        CComboBox       c_FontSize;
        CComboBox       c_Fonts;
        CEdit   c_Text;
        CSpinButtonCtrl c_SpinSize;
        CStatic c_Sample;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRaster)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

public:
        DWORD fontmask;  // mask for font types
        BOOL scalable;

// Implementation
protected:
        void loadFontSet();
        void loadFontSizes();
        void setScalable(BOOL scale);
        BOOL initialized;
        void setNewFont();
        // Generated message map functions
        //{{AFX_MSG(CRaster)
        afx_msg void OnChangeText();
        virtual BOOL OnInitDialog();
        afx_msg void OnSelendokFonts();
        afx_msg void OnChangeMultiplier();
        afx_msg void OnSelendokFontSize();
        afx_msg void OnCopy();
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg void OnRaster();
        afx_msg void OnTruetype();
        afx_msg void OnVector();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
