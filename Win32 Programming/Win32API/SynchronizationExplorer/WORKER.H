// Worker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorker form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWorker : public CFormView
{
protected:
        CWorker();           // protected constructor used by dynamic creation
        DECLARE_DYNCREATE(CWorker)

// Form Data
public:
        // There is one "global" event handle to synchronize the creation of
        // threads.  That's because this handle is "owned" by the GUI (main)
        // thread, and it will create only one task at a time.  Therefore,
        // only one event is required to synchronize the two.

        static CEvent event;  
        static CSingleLock synchronizer;
        //{{AFX_DATA(CWorker)
        enum { IDD = IDD_TASK };
        CIntEdit        c_Priority;
        CSpinButtonCtrl c_SpinPriority;
        CButton c_RunStop;
        CButton c_Step;
        CButton c_Unlock;
        CButton c_Store;
        CButton c_Lock;
        CButton c_Increment;
        CButton c_Fetch;
        CStatic c_N;
        CStatic c_Iterations;
        //}}AFX_DATA

// Attributes
public:
        CThread threadstuff;  // our private thread object for this simulated thread
// Operations
public:

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CWorker)
	public:
        virtual void OnInitialUpdate();
	protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
        ///////////////////////////////////////////////////////////////////
        //                        GUI THREAD CONTROL VALUES
        int loops;

        CFont nfont;

        virtual ~CWorker();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif
        // We have both a thread handle and a thread ID:
        BOOL initialized;
        CWinThread * thread;
        int mode;
#define MODE_WAITING 0
#define MODE_RUNNING 1

        //////////////////////////////////////////////////////////////////////

        void endThread();
        // Generated message map functions
        afx_msg LRESULT OnState(WPARAM, LPARAM);
        afx_msg LRESULT OnWaiting(WPARAM, LPARAM);
        afx_msg LRESULT OnRunning(WPARAM, LPARAM);
        //{{AFX_MSG(CWorker)
        afx_msg void OnClose();
        afx_msg void OnStep();
        afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
        afx_msg void OnDestroy();
        afx_msg void OnRunstop();
        afx_msg void OnChangePriority();
        afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
