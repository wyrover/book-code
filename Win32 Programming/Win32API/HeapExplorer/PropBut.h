// PropBut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropButton window

class CPropButton : public CButton
{
// Construction
public:
        CPropButton();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPropButton)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CPropButton();

        // Generated message map functions
protected:
        //{{AFX_MSG(CPropButton)
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
