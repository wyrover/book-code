// IDCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageCombo window


class CImageCombo : public CComboBox
{
// Construction
public:
        CImageCombo();

// Attributes
public:

// Operations
public:
        int AddString(int data);
        BOOL AddStrings(LPINT data, int def = 0);
        int Select(DWORD itemval);
        DWORD GetCurItem();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CImageCombo)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CImageCombo();
        int maxlen;  // maximum number of items displayed
                     // 0 - no limit
                     // -1 - limit to screen height (NYI)

        // Generated message map functions
protected:
        //{{AFX_MSG(CImageCombo)
        afx_msg void OnDropdown();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
