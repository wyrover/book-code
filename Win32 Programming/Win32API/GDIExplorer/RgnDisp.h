// RgnDisp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplay window

class CRegionDisplay : public CStatic
{
// Construction
public:
        CRegionDisplay();
        virtual void initialize();

// Attributes
public:

// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRegionDisplay)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CRegionDisplay();

        // Generated message map functions
protected:
        HRGN Rgn1;
        HRGN Rgn2;
        HRGN Result;

#define RGN_1      0 // responses to UWM_QUERY_RGNWHAT
#define RGN_2      1
#define RGN_RESULT 2

#define RGN_FILL   1 // responses to UWM_QUERY_RGNHOW: one or both of these
#define RGN_FRAME  2
        //{{AFX_MSG(CRegionDisplay)
        afx_msg void OnPaint();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};


#define UWM_QUERY_RGNWHAT (WM_USER+100)
#define UWM_QUERY_RGNOP   (WM_USER+101)
#define UWM_QUERY_RGNHOW  (WM_USER+102)
#define UWM_QUERY_MONO    (WM_USER+103)

/////////////////////////////////////////////////////////////////////////////
