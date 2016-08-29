// BrDemo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrushDemo window

class CBrushDemo : public CStatic
{
// Construction
public:
        CBrushDemo();
        CBrush * brush;
// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CBrushDemo)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CBrushDemo();

        // Generated message map functions
protected:
        //{{AFX_MSG(CBrushDemo)
        afx_msg void OnPaint();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
