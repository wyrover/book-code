// DrwButt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawEdgeButton window

class CDrawEdgeButton : public CButton
{
// Construction
public:
        CDrawEdgeButton();
        void setDownStyle(DWORD style) { downstyle = style; }
        void setUpStyle(DWORD style) { upstyle = style; }
        DWORD getDownStyle() { return downstyle; }
        DWORD getUpStyle() { return upstyle; }
// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDrawEdgeButton)
        public:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CDrawEdgeButton();

        // Generated message map functions
protected:
        DWORD downstyle;
        DWORD upstyle;
        //{{AFX_MSG(CDrawEdgeButton)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
