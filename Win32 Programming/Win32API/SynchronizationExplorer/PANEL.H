// Panel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanel form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CPanel : public CFormView
{
protected:
        CPanel();           // protected constructor used by dynamic creation
        DECLARE_DYNCREATE(CPanel)

// Form Data
public:
        //{{AFX_DATA(CPanel)
        enum { IDD = IDD_CONTROL };
        CButton c_Synchronous;
        CStatic c_SpeedDisplay;
        CSliderCtrl     c_Speed;
        CButton c_Random;
        CEdit   c_N;
        CButton c_Locking;
        CDelta  c_ErrorBar;
        CEdit   c_Actual;
        //}}AFX_DATA

// Attributes
public:
        static HWND panel;  // for posting progress messages
// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CPanel)
	public:
        virtual void OnInitialUpdate();
	protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
        int loops;

        virtual ~CPanel();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif
        void showSpeed(int pos);


        afx_msg LRESULT OnN(WPARAM wParam, LPARAM);
        afx_msg LRESULT OnLoop(WPARAM, LPARAM);
        // Generated message map functions
        //{{AFX_MSG(CPanel)
        afx_msg void OnLocking();
        afx_msg void OnRandom();
        afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
        afx_msg void OnSynchronous();
	//}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
