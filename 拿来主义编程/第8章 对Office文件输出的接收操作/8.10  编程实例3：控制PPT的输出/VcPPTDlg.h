// VcPPTDlg.h : header file
//

#if !defined(AFX_VCPPTDLG_H__16668C98_2408_4C70_B66F_2E3D69BE16E5__INCLUDED_)
#define AFX_VCPPTDLG_H__16668C98_2408_4C70_B66F_2E3D69BE16E5__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

class CVcPPTDlgAutoProxy;
#include "msppt.h"

/////////////////////////////////////////////////////////////////////////////
// CVcPPTDlg dialog

class CVcPPTDlg : public CDialog
{
    DECLARE_DYNAMIC(CVcPPTDlg);
    friend class CVcPPTDlgAutoProxy;

// Construction
public:
    void ShowComment(CString str, int i, int j, int k);

    _Application m_pptApp; //应用程序对象。
    Presentations m_pptPresentations;//表现集合
    _Presentation m_pptPresentation;

    SlideShowWindow m_pptSlideShowWindow;//一张张画面的集合。
    SlideShowView m_pptSlideShowView;//当前的某一张画面。

    SlideShowSettings m_pptSlideShowSettings;//画面间切换的设置。

    Slides m_pptSlides;
    _Slide m_pptSlide;

    CVcPPTDlg(CWnd* pParent = NULL);    // standard constructor
    virtual ~CVcPPTDlg();

// Dialog Data
    //{{AFX_DATA(CVcPPTDlg)
    enum { IDD = IDD_VCPPT_DIALOG };
    CString m_strComment;
    CString m_strFilename;
    CString m_strContent;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVcPPTDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    CVcPPTDlgAutoProxy* m_pAutoProxy;
    HICON m_hIcon;


    // Generated message map functions
    //{{AFX_MSG(CVcPPTDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnBUTTONStart();
    afx_msg void OnBUTTONRun();
    afx_msg void OnBUTTONClose();
    afx_msg void OnBUTTONFirstSlide();
    afx_msg void OnBUTTONPreviousSlide();
    afx_msg void OnBUTTONNextSlide();
    afx_msg void OnBUTTONLastSlide();
    afx_msg void OnBUTTONPlay();
    afx_msg void OnBUTTONMinimize();
    afx_msg void OnBUTTONBrowser();
    afx_msg void OnBUTTONGetText();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCPPTDLG_H__16668C98_2408_4C70_B66F_2E3D69BE16E5__INCLUDED_)
