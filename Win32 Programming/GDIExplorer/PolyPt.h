// PolyPt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPolyPoints dialog

class CPolyPoints : public CDialog
{
// Construction
public:
        CPolyPoints(CWnd* pParent = NULL);   // standard constructor
        PolyPolyData pd;

// Dialog Data
        //{{AFX_DATA(CPolyPoints)
        enum { IDD = IDD_POLYPOLYSET };
        CButton c_OK;
        CListBox        c_Points;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPolyPoints)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        // Generated message map functions
        //{{AFX_MSG(CPolyPoints)
        virtual void OnOK();
        virtual BOOL OnInitDialog();
        afx_msg void OnDestroy();
        afx_msg void OnSelchangePoints();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};
