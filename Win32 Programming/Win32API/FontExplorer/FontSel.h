// fontsel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontSelect dialog

class CFontSelect : public CDialog
{
// Construction
public:
        CFontSelect(CWnd* pParent = NULL);      // standard constructor

// Dialog Data
        //{{AFX_DATA(CFontSelect)
        enum { IDD = IDD_FONTS };
        CStatic c_FontSample;
        CStatic c_FontName;
        CComboBox       c_FontList;
        //}}AFX_DATA

// Implementation
protected:
        virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
        void DeleteNewFonts();
        void EstablishNewFonts();
        CFontObj * GetSelectedFontDescriptor();

        // Generated message map functions
        //{{AFX_MSG(CFontSelect)
        afx_msg void OnChangefont();
        afx_msg void OnDblclkFontlist();
        afx_msg void OnSelchangeFontlist();
        virtual void OnCancel();
        virtual void OnOK();
        virtual BOOL OnInitDialog();
        afx_msg void OnDestroy();
        //}}AFX_MSG
        void LoadFonts();

        BOOL AnyFontChanged;

        DECLARE_MESSAGE_MAP()
};
