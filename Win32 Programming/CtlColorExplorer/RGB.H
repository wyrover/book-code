// RGB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRGB window

class CRGB : public CStatic
{
// Construction
public:
        CRGB();

// Attributes
public:

// Operations
public:
        void SetWindowText(COLORREF rgb);

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRGB)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CRGB();

        // Generated message map functions
protected:
        //{{AFX_MSG(CRGB)
                // NOTE - the ClassWizard will add and remove member functions here.
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
