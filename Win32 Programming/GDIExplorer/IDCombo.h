// IDCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIDCombo window

typedef struct {
        int id;  // IDS_ for string, or 0 for end of table for AddStrings
        int val; // value for itemdata
               } IDData;


class CIDCombo : public CComboBox
{
// Construction
public:
        CIDCombo();

// Attributes
public:

// Operations
public:
        int AddString(IDData * data);
        int AddString(CString &s) { return CComboBox::AddString(s); }
        int AddStrings(IDData * data, int def = 0);
        int Select(DWORD itemval);
        DWORD GetCurItem();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CIDCombo)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CIDCombo();
        int maxlen;  // maximum number of items displayed
                     // 0 - no limit
                     // -1 - limit to screen height (NYI)

        // Generated message map functions
protected:
        //{{AFX_MSG(CIDCombo)
        afx_msg void OnDropdown();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
