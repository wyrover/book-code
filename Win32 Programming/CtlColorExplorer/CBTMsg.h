// CBTMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCBTMsgBox window

class CCBTMsgBox : public CWnd
{
// Construction
public:
        CCBTMsgBox();

// Attributes
public:
        // Hook function parameters
        BOOL     subclass;   // true if it should be subclassed
        HWND     hwnd;       // handle to message box window
        WNDPROC  MsgBoxProc; // Internal message box procedure
        HHOOK    hhook;      // hook for CBT function

        // Window parameters
        HBRUSH   hbrush;     // background brush
        COLORREF bkcolor;    // text background color
        COLORREF textcolor;  // text color
        int      bkmode;     // background mode
        HFONT    hfont;      // font handle
// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCBTMsgBox)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CCBTMsgBox();

        // Generated message map functions
protected:
        //{{AFX_MSG(CCBTMsgBox)
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
