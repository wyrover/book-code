// VCActiveXDlg.h : header file
//
//{{AFX_INCLUDES()
#include "calendar.h"
//}}AFX_INCLUDES

#if !defined(AFX_VCACTIVEXDLG_H__C09BCDC7_7E49_11D4_8BFA_0008DB000000__INCLUDED_)
#define AFX_VCACTIVEXDLG_H__C09BCDC7_7E49_11D4_8BFA_0008DB000000__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVCActiveXDlg dialog

class CVCActiveXDlg : public CDialog
{
// Construction
public:
    CVCActiveXDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CVCActiveXDlg)
    enum { IDD = IDD_VCACTIVEX_DIALOG };
    CCalendar   m_calendar;
    short   m_day;
    short   m_month;
    short   m_year;
    //}}AFX_DATA
    COleVariant m_variant;
    unsigned long m_BackColor;
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVCActiveXDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CVCActiveXDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSelectdate();
    afx_msg void OnNextweek();
    afx_msg void OnAfterUpdateCalendarctrl();
    DECLARE_EVENTSINK_MAP()
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCACTIVEXDLG_H__C09BCDC7_7E49_11D4_8BFA_0008DB000000__INCLUDED_)
