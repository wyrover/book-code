// StrDisp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStringDisplay window

class CStringDisplay : public CComboBox
{
// Construction
public:
        CStringDisplay();

// Attributes
public:

// Operations
public:
        int AddString(CMyData * p);
        int FindString(int start, LPCTSTR str);
        int FindStringExact(int start, LPCTSTR str);
        CMyData * GetItemData(int index);
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CStringDisplay)
        public:
        virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
        virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CStringDisplay();

        // Generated message map functions
protected:
        //{{AFX_MSG(CStringDisplay)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
