// InfoBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoBox window

class CInfoBox : public CComboBox
{
// Construction
public:
        CInfoBox();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CInfoBox)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CInfoBox();
        int AddString(LPCSTR s, int i);
        int FindItem(int id);

        // Generated message map functions
protected:
        //{{AFX_MSG(CInfoBox)
        afx_msg void OnDropdownInfo();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
