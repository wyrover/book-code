// Box.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBox window

class CBox : public CButton
{
// Construction
public:
        CBox();
        void SetCheck(BOOL val);
        BOOL GetCheck();
        void toggle();
// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CBox)
        public:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CBox();

        // Generated message map functions
protected:
        //{{AFX_MSG(CBox)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
