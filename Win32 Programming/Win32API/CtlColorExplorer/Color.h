// Color.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColor window


class CColor : public CComboBox
{
// Construction
public:
        CColor();
        void LoadColors();
        int SetColor(COLORREF color);
        COLORREF GetColor();
// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CColor)
        public:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CColor();

        // Generated message map functions
protected:
        //{{AFX_MSG(CColor)
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
