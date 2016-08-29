// CPlace.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharacterPlacement dialog

class CCharacterPlacement : public CDialog
{
// Construction
public:
        CCharacterPlacement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CCharacterPlacement)
        enum { IDD = IDD_PLACEMENT };
        CStatic c_Sample;
        CButton c_GCP_MaxExtent;
        CEdit   c_MaxExtent;
        CEdit   c_lpString;
        CStatic c_c_MaxExtent;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCharacterPlacement)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void enableExtent(BOOL mode);

        // Generated message map functions
        //{{AFX_MSG(CCharacterPlacement)
        afx_msg void OnFont();
        afx_msg void OnGcpMaxextent();
        virtual BOOL OnInitDialog();
        afx_msg void OnGetcharplacement();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
