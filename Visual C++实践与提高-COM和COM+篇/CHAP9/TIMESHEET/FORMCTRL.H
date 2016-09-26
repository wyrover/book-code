/*****************************************************************************
  File: FormControl.h

  Purpose:
     Define the CFormControl class from which the Form OLE Controls can be
     derived. The primary purpose of this class is to create the controls
     window using CWnd::CreateDlg. Also provided is a default design time
     representation of the control, a means of forcing the control to the
     size of the dialog template, and code to handle mouse and keyboard
     processing.

  Includes:
     #include "Wnd2.h"
     #include "BaseCtrl.h"

  Variables:
     m_lpszTemplateName  - Dialog template resource name.
     m_cTemplateSize     - Dialog template size in pixels.
     m_bAutoSize         - Use the template size to fix the size of the control.
     m_hWndCurrentChild  - Holder for the current child window.
     m_sFormName         - String dispayed at design time.
     m_pOccDialogInfo    - Copied from CFormView

  Functions:
     CFormControl        - Initialize member variables and calculate the dialog
                           template size.
     OnSetExtent         - Sets the size of the control.
     OnInitialUpdate     - Handles the child control initialization.
     OnDraw              - Provides a border at design time.
     PreTranslateMessage - If m_hWnd is valid, allows IsDialogMessage to
                           to try and handle the message.
     OnActivateControl   - Handler to be executed when a child window gets the
                           focus.
     OnSetFocus          - Handles the case when tabbing into the OLE Form Control.
     CreateControlWindow - Creates m_hWnd using CWnd::CreateDlg

     SetOccDialogInfo    - Copied from CFormView.
     HandleInitDialog    - Copied from CFormView

     GetClassID          - Had to override COleControl pure virtual function.
     GetUserTypeNameID   - Had to override COleControl pure virtual function.
     GetMiscStatus       - Had to override COleControl pure virtual function.

     OnMouseActivate     - To make the OLE control layer transparent mouse
     OnMouseMove         - messages should come through the child controls.
     OnLButtonDblClk     - These functions have empty implementations so as
     OnLButtonDown       - to turn off the default mouse message processing
     OnLButtonUp         - of an OLE control.
     OnMButtonDblClk     - See Above.
     OnMButtonDown       - See Above.
     OnMButtonUp         - See Above.
     OnRButtonDblClk     - See Above.
     OnRButtonDown       - See Above.
     OnRButtonUp         - See Above.

  Other:
     _AfxCheckDialogTemplate - Global helper function copied from CFormView.

  Development Team: ShawnK

  Written by Microsoft Product Support Services, Languages Developer Support
  Copyright (c) 1993 Microsoft Corporation. All rights reserved.
\****************************************************************************/

#ifndef FORMCONTROL_H
#define FORMCONTROL_H

/////////////////////////////////////////////////////////////////////////////
// CFormControl window
#include "Wnd2.h"

class CFormControl : public COleControl
{
    DECLARE_DYNCREATE(CFormControl)

    CFormControl() {};

// Construction
public:
    CFormControl(LPCTSTR lpszTemplateName, BOOL bAutoSize = TRUE);
    CFormControl(UINT nIDTemplate, BOOL bAutoSize = TRUE);
    virtual ~CFormControl();
    virtual BOOL OnSetExtent(LPSIZEL lpSizeL);
    virtual void OnInitialUpdate();
    virtual DWORD GetControlFlags();

// Attributes
protected:
    LPCTSTR     m_lpszTemplateName;     // Template resource name.
    CSize       m_cTemplateSize;        // Template size in pixels.
    BOOL        m_bAutoSize;            // Auto size the control by Template size.
    HWND        m_hWndCurrentChild;     // Holder for the current child window.
    CString     m_sFormName;            // String dispayed at design time.

// Operations
public:
    // Drawing function
    virtual void OnDraw(
        CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

    // Handle Tab Key (etc.) processing.
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LONG OnActivateControl(UINT, LONG);

protected:
    virtual BOOL CreateControlWindow(HWND hWndParent, const CRect& rcPos,
                                     LPCRECT prcClipped = NULL);

    void GetFormName();
    BOOL GetExtendedProp(LPWSTR strPropName, LPDISPATCH& lpDisp, VARIANT& va);

#ifndef _AFX_NO_OCC_SUPPORT
    // data and functions necessary for OLE control containment
    _AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
    virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
    afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif

// Abstract functions in COleControl.
    // Class ID (implemented by IMPLEMENT_OLECREATE_EX macro)
    virtual HRESULT GetClassID(LPCLSID pclsid)
    {
        return 0;
    };
    virtual UINT GetUserTypeNameID()
    {
        return 0;
    };
    virtual DWORD GetMiscStatus()
    {
        return 0;
    };

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFormControl)
    //}}AFX_VIRTUAL

    // Generated message map functions
protected:
    //{{AFX_MSG(CFormControl)
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

BOOL AFXAPI _AfxCheckDialogTemplate(LPCTSTR lpszResource, BOOL bInvisibleChild);

#endif //FORMCONTROL_H
