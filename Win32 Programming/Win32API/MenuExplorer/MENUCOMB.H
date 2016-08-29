// MenuComb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuCombo window

class CMenuCombo : public CComboBox
{
// Construction
public:
        CMenuCombo();

// Attributes
public:

// Operations
public:
        int AddString(HMENU menu, DWORD flags, CString * name);
        int Select(HMENU menu);
        menuitem * GetCurItem();
        void DeleteItem(HMENU menu);
        int FindItem(HMENU menu);

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMenuCombo)
        virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CMenuCombo();
        int maxlen;  // maximum number of items displayed
                     // 0 - no limit
                     // -1 - limit to screen height (NYI)

        // Generated message map functions
protected:
        //{{AFX_MSG(CMenuCombo)
        afx_msg void OnDropdown();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
