// Degrees.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDegrees window

class CDegrees : public CEdit
{
// Construction
public:
        CDegrees();

// Attributes
public:

// Operations
public:
        void SetWindowInt(int degrees);
        int GetWindowInt();
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDegrees)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CDegrees();

        // Generated message map functions
protected:
        //{{AFX_MSG(CDegrees)
        afx_msg void OnChange();
        afx_msg void OnKillFocus(CWnd* pNewWnd);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
