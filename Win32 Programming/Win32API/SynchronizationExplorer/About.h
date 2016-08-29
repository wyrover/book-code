// About.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAbout dialog

typedef enum { typeDefault        = 0,
               typeAdvancedServer = 1,
               typeWorkstation    = 2,
               typeServer         = 3 } NTTYPE ;

class CAbout : public CDialog
{
// Construction
public:
        CAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CAbout)
        enum { IDD = IDD_ABOUTBOX };
        CStatic c_LegalCopyright;
        CStatic c_FileDescription;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CAbout)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void DisplayOperatingSystemVersionInfo ();
        void DisplayExecutableVersionInfo () ;
        void DisplayProcessorVersionInfo ();
        NTTYPE GetNTVersion ();
        // Generated message map functions
        //{{AFX_MSG(CAbout)
        afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
        afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
