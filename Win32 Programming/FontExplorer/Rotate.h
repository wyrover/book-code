// Rotate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotation window

class CRotation : public CSpinButtonCtrl
{
// Construction
public:
        CRotation();

// Attributes
public:

// Operations
public:
        CWnd * SetBuddy(CDegrees * newbuddy);
        void SetRange(int lower, int upper);
        int SetPos(int pos, BOOL updatebuddy = TRUE);
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CRotation)
        public:
        virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CRotation();

        // Generated message map functions
protected:
        CDegrees * buddy;
        void pos_to_buddy();
public:
        //{{AFX_MSG(CRotation)
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
