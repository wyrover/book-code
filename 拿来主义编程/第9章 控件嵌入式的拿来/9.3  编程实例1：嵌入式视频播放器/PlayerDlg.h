// PlayerDlg.h : header file
//
//{{AFX_INCLUDES()
#include "vlccontrol2.h"
#include "vlcplaylist.h"
#include "vlcplaylistitems.h"
#include "vlcaudio.h"
//}}AFX_INCLUDES

#if !defined(AFX_PLAYERDLG_H__81678F84_1E2B_41F8_891F_F75F4734476C__INCLUDED_)
#define AFX_PLAYERDLG_H__81678F84_1E2B_41F8_891F_F75F4734476C__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

class CPlayerDlg : public CDialog
{
// Construction
public:
    BOOL m_bPause;
    CPlayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CPlayerDlg)
    enum { IDD = IDD_PLAYER_DIALOG };
    CVLCControl2    m_ctlVLC;
    CString m_strFilename;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPlayerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CPlayerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONPlay();
    afx_msg void OnBUTTONStop();
    afx_msg void OnBUTTONPause();
    afx_msg void OnBUTTONBrowser();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDLG_H__81678F84_1E2B_41F8_891F_F75F4734476C__INCLUDED_)
