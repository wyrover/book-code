// FontName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontName window

class CFontName : public CComboBox
{
// Construction
public:
        CFontName();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CFontName)
        public:
        virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CFontName();

        // Generated message map functions
protected:
        //{{AFX_MSG(CFontName)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
