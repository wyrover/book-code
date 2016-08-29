// MenuChk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuCheck window

class CMenuCheck : public CComboBox
{
// Construction
public:
        CMenuCheck();
        int maxlen;
        void load();
        HBITMAP GetCurItem();
// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMenuCheck)
        public:
        virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CMenuCheck();

        // Generated message map functions
protected:
        //{{AFX_MSG(CMenuCheck)
        afx_msg void OnDropdown();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
