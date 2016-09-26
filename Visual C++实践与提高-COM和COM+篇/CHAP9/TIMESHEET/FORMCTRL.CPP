/*****************************************************************************
  File: FormControl.cpp

  Purpose:
     Implement the CFormControl class from which the Form OLE Controls can be
     derived. The primary purpose of this class is to create the controls
     window using CWnd::CreateDlg. Also provided is a default design time
     representation of the control, a means of forcing the control to the
     size of the dialog template, and code to handle mouse and keyboard
     processing.

  Includes:
     #include "FormControl.h"
     #include "Wnd2.h"
     #include "AfxPriv.h"
     #include "AfxImpl.h"
     #include "CtlImpl.h"
     #include "OccImpl.h"

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


#include "stdafx.h"
#include "FormCtrl.h"
#include "AfxPriv.h"
#include "AfxImpl.h"
#include "CtlImpl.h"
#include "OccImpl.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CFormControl, COleControl)

//**************************************************************************
// Initialize member variables and calculate the dialog template size.
CFormControl::CFormControl(LPCTSTR lpszTemplateName, BOOL bAutoSize):
    m_bAutoSize(bAutoSize),
    m_lpszTemplateName(lpszTemplateName),
    m_hWndCurrentChild(NULL),
    m_cTemplateSize(0, 0),
    m_sFormName("Form Control")
{
    // load dialog resource
    LPDLGTEMPLATE lpDialogTemplate = NULL;
    HGLOBAL hDialogTemplate = NULL;
    HINSTANCE hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
    HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
    hDialogTemplate = LoadResource(hInst, hResource);

    if (hDialogTemplate != NULL)
        lpDialogTemplate = (LPDLGTEMPLATE)LockResource(hDialogTemplate);

    ASSERT(lpDialogTemplate != NULL);
    // Get the dialog template size in pixels for OnSetExtent.
    CDialogTemplate cdt_Form(lpDialogTemplate);
    tagSIZE dlgSize;
    cdt_Form.GetSizeInPixels(&dlgSize);
    m_cTemplateSize.cx = dlgSize.cx;
    m_cTemplateSize.cy = dlgSize.cy;
    // free dialog resource
    UnlockResource(hDialogTemplate);
    FreeResource(hDialogTemplate);
}

//**************************************************************************
// Initialize member variables and calculate the dialog template size.
CFormControl::CFormControl(UINT nIDTemplate, BOOL bAutoSize):
    m_bAutoSize(bAutoSize),
    m_hWndCurrentChild(NULL),
    m_cTemplateSize(0, 0),
    m_sFormName("Form Control")

{
    ASSERT_VALID_IDR(nIDTemplate);
    m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
    // load dialog resource
    LPDLGTEMPLATE lpDialogTemplate = NULL;
    HGLOBAL hDialogTemplate = NULL;
    HINSTANCE hInst = AfxGetInstanceHandle();
    HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
    hDialogTemplate = LoadResource(hInst, hResource);

    if (hDialogTemplate != NULL)
        lpDialogTemplate = (LPDLGTEMPLATE)LockResource(hDialogTemplate);

    ASSERT(lpDialogTemplate != NULL);
    // Get the dialog template size in pixels for OnSetExtent.
    CDialogTemplate cdt_Form(lpDialogTemplate);
    tagSIZE dlgSize;
    cdt_Form.GetSizeInPixels(&dlgSize);
    m_cTemplateSize.cx = dlgSize.cx;
    m_cTemplateSize.cy = dlgSize.cy;
    // free dialog resource
    UnlockResource(hDialogTemplate);
    FreeResource(hDialogTemplate);
}

//**************************************************************************
CFormControl::~CFormControl()
{
}

//**************************************************************************
// Set the size of the control to the size of the dialog template it was
// created from. This functionality can be turned off.
BOOL CFormControl::OnSetExtent(LPSIZEL lpSizeL)
{
    if (m_bAutoSize) {
        // This function limits the height of a control to
        // to the size of the dialog template
        // Use the desktop window to get a DC so we can use
        // CDC::HIMETRICtoDP and CDC::DPtoHIMETRIC
        CWnd *pWnd = CWnd::FromHandle(::GetDesktopWindow());
        CClientDC dc(pWnd);
        CSize sz(m_cTemplateSize.cx, m_cTemplateSize.cy);
        dc.DPtoHIMETRIC(&sz);//re-convert to HIMETRIC
        lpSizeL->cx = sz.cx;
        lpSizeL->cy = sz.cy;
    }

    return COleControl::OnSetExtent(lpSizeL);
}

//**************************************************************************
// Modify CreateControlWindow to create m_hWnd using CreateDlg. This will
// create the window using the dialog resource parameter in the constructor.
BOOL CFormControl::CreateControlWindow(HWND hWndParent, const CRect& rcPos,
                                       LPCRECT prcClipped)
{
    ASSERT(hWndParent != NULL);
    ASSERT(m_lpszTemplateName != NULL);
    // If m_hWnd is NULL then call OnInitialUpdate.
    BOOL bInitialUpdate = (m_hWnd == NULL);

    if (prcClipped == NULL)
        prcClipped = &rcPos;

    if (m_hWnd == NULL) {
        // If window doesn't exist, create it.

        // Test if:
        // we're not subclassing a Windows control, or
        // container reflects messages for us...
        // we create normally if:
        //       (we're not subclassing -or- the container reflects)
        // -and- the container autoclips for us
        if ((!IsSubclassedControl() || m_bMsgReflect) && m_bAutoClip) {
            // Just create the control's window.
#ifdef _DEBUG
            // dialog template must exist and be invisible with WS_CHILD set
            if (!_AfxCheckDialogTemplate(m_lpszTemplateName, TRUE)) {
                ASSERT(FALSE);          // invalid dialog template name
                PostNcDestroy();        // cleanup if Create fails too soon
                return FALSE;
            }

#endif //_DEBUG
#ifdef _MAC
            HINSTANCE hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
            _AfxStripDialogCaption(hInst, m_lpszTemplateName);
#endif

            // initialize common controls
            if (AfxDeferRegisterClass(AFX_WNDOLECONTROL_REG)) {
                // create a modeless dialog window.
                CWnd* pParent = CWnd:: FromHandle(hWndParent);

                if (!CreateDlg(m_lpszTemplateName, pParent))
                    return FALSE;

                if (m_hWnd != NULL) {
                    MoveWindow(rcPos, TRUE);
                    ::ShowWindow(hWndParent, SW_SHOW);
                    ::ShowWindow(m_hWnd, SW_SHOW);
                    SetFocus();
                }
            }
        } else { // ...we're subclassing a Windows control.
            if (m_pReflect == NULL) {
                // Create a window to reflect notification messages.
                m_pReflect = new CReflectorWnd;

                if (!m_pReflect->Create(prcClipped, hWndParent)) {
                    // If m_pReflect->Create failed, then m_pReflect deleted itself.
                    m_pReflect = NULL;
                }
            } else {
                // Reflector window already exists... just reparent it.
                if (m_pReflect->m_hWnd != NULL) {
                    ::SetParent(m_pReflect->m_hWnd, hWndParent);
                    ::SetWindowPos(m_pReflect->m_hWnd, NULL, 0, 0, 0, 0,
                                   SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                                   SWP_SHOWWINDOW);
                }
            }

            if (m_pReflect != NULL && m_pReflect->m_hWnd != NULL) {
#ifdef _DEBUG

                // dialog template must exist and be invisible with WS_CHILD set
                if (!_AfxCheckDialogTemplate(m_lpszTemplateName, TRUE)) {
                    ASSERT(FALSE);          // invalid dialog template name
                    PostNcDestroy();        // cleanup if Create fails too soon
                    return FALSE;
                }

#endif //_DEBUG
#ifdef _MAC
                HINSTANCE hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
                _AfxStripDialogCaption(hInst, m_lpszTemplateName);
#endif
                // Create the control's window.
                CreateDlg(m_lpszTemplateName, m_pReflect);

                if (m_hWnd == NULL) {
                    // Window creation failed: cleanup.
                    m_pReflect->DestroyWindow();
                    m_pReflect = NULL;
                } else {
                    // Make both windows visible.
                    m_pReflect->ShowWindow(SW_SHOW);
                    MoveWindow(rcPos, TRUE);
                    ::ShowWindow(m_hWnd, SW_SHOW);
                    SetFocus();
                }
            }
        }

        // Set the new window's font.
        OnFontChanged();
    } else {
        // If window does exist, reparent and reposition it.
        CWnd* pWndOuter = GetOuterWindow();
        ASSERT(pWndOuter != NULL);

        if (::GetParent(pWndOuter->m_hWnd) != hWndParent)
            ReparentControlWindow(pWndOuter->m_hWnd, hWndParent);

        ::SetWindowPos(pWndOuter->m_hWnd, NULL, 0, 0, 0, 0,
                       SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_SHOWWINDOW);

        if (m_pReflect == NULL)
            ::MoveWindow(m_hWnd, rcPos.left, rcPos.top,
                         rcPos.Width(), rcPos.Height(), TRUE);
        else {
            pWndOuter->MoveWindow(prcClipped, TRUE);
            ::MoveWindow(m_hWnd, m_ptOffset.x, m_ptOffset.y,
                         rcPos.Width(), rcPos.Height(), TRUE);
        }
    }

    ASSERT(m_hWnd != NULL);

    // Send message to overridable OnInitialUpdate.
    if (bInitialUpdate) {
        InitControlContainer();
        UpdateData(FALSE);
        PostMessage(WM_INITIALUPDATE, 0, 0);
    }

    return (m_hWnd != NULL);
}

//**************************************************************************
// Diagnostic routine to check for and decode dialog templates returns FALSE
// if a program error occurs (i.e. bad resource ID or bad dialog styles).
// Copied from MFC.
BOOL AFXAPI _AfxCheckDialogTemplate(LPCTSTR lpszResource, BOOL bInvisibleChild)
{
    ASSERT(lpszResource != NULL);
    HINSTANCE hInst = AfxFindResourceHandle(lpszResource, RT_DIALOG);
    HRSRC hResource = ::FindResource(hInst, lpszResource, RT_DIALOG);

    if (hResource == NULL) {
        if (HIWORD(lpszResource) != 0)
            TRACE1("ERROR: Cannot find dialog template named '%s'.\n",
                   lpszResource);
        else
            TRACE1("ERROR: Cannot find dialog template with IDD 0x%04X.\n",
                   LOWORD((DWORD)lpszResource));

        return FALSE;
    }

    if (!bInvisibleChild)
        return TRUE;        // that's all we need to check

    // we must check that the dialog template is for an invisible child
    //  window that can be used for a form-view or dialog-bar
    HGLOBAL hTemplate = LoadResource(hInst, hResource);

    if (hTemplate == NULL) {
        TRACE0("Warning: LoadResource failed for dialog template.\n");
        // this is only a warning, the real call to CreateDialog will fail
        return TRUE;        // not a program error - just out of memory
    }

    DLGTEMPLATEEX* pTemplate = (DLGTEMPLATEEX*)LockResource(hTemplate);
    DWORD dwStyle;

    if (pTemplate->signature == 0xFFFF)
        dwStyle = pTemplate->style;
    else
        dwStyle = ((DLGTEMPLATE*)pTemplate)->style;

    UnlockResource(hTemplate);
    FreeResource(hTemplate);

    if (dwStyle & WS_VISIBLE) {
        if (HIWORD(lpszResource) != 0)
            TRACE1("ERROR: Dialog named '%s' must be invisible.\n",
                   lpszResource);
        else
            TRACE1("ERROR: Dialog with IDD 0x%04X must be invisible.\n",
                   LOWORD((DWORD)lpszResource));

        return FALSE;
    }

    if (!(dwStyle & WS_CHILD)) {
        if (HIWORD(lpszResource) != 0)
            TRACE1("ERROR: Dialog named '%s' must have the child style.\n",
                   lpszResource);
        else
            TRACE1("ERROR: Dialog with IDD 0x%04X must have the child style.\n",
                   LOWORD((DWORD)lpszResource));

        return FALSE;
    }

    return TRUE;
}

//**************************************************************************
// Handles keyboard and mouse processing such as tabbing through the
// form control.
BOOL CFormControl::PreTranslateMessage(MSG* pMsg)
{
    ASSERT(pMsg != NULL);
    CWnd*   pWndFocus = CWnd::GetFocus();
    BOOL    bDefault = TRUE;

    if (m_hWnd) {
        // If an OLE Control has the focus, then give it the first crack at key
        // and mouse messages.
        HWND hWndFocus = pWndFocus->GetSafeHwnd();
        UINT uMsg = pMsg->message;

        if (((uMsg >= WM_KEYFIRST) && (uMsg <= WM_KEYLAST)) ||
            ((uMsg >= WM_MOUSEFIRST) && (uMsg <= WM_MOUSELAST))) {
            CWnd2* pWndCtrl = (CWnd2*)pWndFocus;

            // Walk up the parent chain, until we find an OLE control.
            while ((pWndCtrl != NULL) && (pWndCtrl->GetControlSite() == NULL) &&
                   (pWndCtrl->GetParent() != this)) {
                pWndCtrl = (CWnd2*)pWndCtrl->GetParent();
            }

            if ((pWndCtrl != NULL) && (pWndCtrl->GetControlSite() != NULL) &&
                (pWndCtrl->GetControlSite()->m_pActiveObject != NULL) &&
                (pWndCtrl->GetControlSite()->m_pActiveObject->TranslateAccelerator(
                     pMsg) == S_OK)) {
                return TRUE;
            }
        }

        HWND hFirstDlgItem = ::GetNextDlgTabItem(m_hWnd, NULL, FALSE);
        HWND hLastDlgItem = ::GetNextDlgTabItem(m_hWnd, hFirstDlgItem, TRUE);
        BOOL ShiftKey = ::GetKeyState(VK_SHIFT) < 0;

        // Check OLE controls for boundry conditions
        switch (uMsg) {
        case WM_KEYDOWN: {
            switch (LOWORD(pMsg->wParam)) {
            case VK_LEFT:
            case VK_UP: {
                ShiftKey = TRUE;
            }

            case VK_TAB:
            case VK_RIGHT:
            case VK_DOWN: {
                if (ShiftKey) {
                    // Are we at the first control on the form and moving up?
                    if (hWndFocus == hFirstDlgItem) {
                        CWnd* pCntr = GetOuterWindow()->GetParent();

                        if (pCntr && pCntr->GetNextDlgTabItem(this, TRUE) != this) {
                            bDefault = FALSE;
                        }
                    }

                    // Are we at the last control on the form and moving down?
                } else if (hWndFocus == hLastDlgItem) {
                    CWnd* pCntr = GetOuterWindow()->GetParent();

                    if (pCntr && pCntr->GetNextDlgTabItem(this, FALSE) != this) {
                        bDefault = FALSE;
                    }
                }
            }
            }
        }
        }

        if (bDefault) {
            return PreTranslateInput(pMsg);
        } else {
            // Update the default push button's control ID.
            ::SendMessage(m_hWnd, DM_SETDEFID, 0L, 0L);
            // Reset the current default push button to a regular button.
            UINT bn_style = (UINT)GetWindowLong(pWndFocus->m_hWnd, GWL_STYLE) & 0xff;

            if (bn_style & BS_DEFPUSHBUTTON) {
                ::SendMessage(pWndFocus->m_hWnd, BM_SETSTYLE,
                              MAKELONG(0, bn_style & ~BS_DEFPUSHBUTTON), (LPARAM)TRUE);
            }

            return FALSE;
        }
    } else {
        return FALSE;
    }
}

//**************************************************************************
BEGIN_MESSAGE_MAP(CFormControl, COleControl)
    //{{AFX_MSG_MAP(CFormControl)
    ON_WM_SETFOCUS()
    ON_MESSAGE(WM_ACTIVATE_CONTROL, OnActivateControl)
    ON_WM_MOUSEACTIVATE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONDBLCLK()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
    ON_WM_RBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_RBUTTONUP()
    //}}AFX_MSG_MAP
    // special command for Initial Update
    ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
#ifndef _AFX_NO_OCC_SUPPORT
    ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
#endif
END_MESSAGE_MAP()

//**************************************************************************
// Reset the default button and set the focus to the appropriate child control
// on the form.
void CFormControl::OnSetFocus(CWnd* pOldWnd)
{
    short ShiftState = ::GetKeyState(VK_SHIFT);
    short TabState = ::GetKeyState(VK_TAB);
    HWND hFirstDlgItem = ::GetNextDlgTabItem(m_hWnd, NULL, FALSE);
    HWND hLastDlgItem = ::GetNextDlgTabItem(m_hWnd, hFirstDlgItem, TRUE);

    // Set the focus to the appropriate child control in our form.
    if (TabState < 0) {
        if (ShiftState < 0) {
            m_hWndCurrentChild = hLastDlgItem;
        } else {
            m_hWndCurrentChild = hFirstDlgItem;
        }
    } else if ((::GetKeyState(VK_UP) < 0) || (GetKeyState(VK_LEFT) < 0)) {
        m_hWndCurrentChild = hLastDlgItem;
    } else if ((::GetKeyState(VK_DOWN) < 0) || (GetKeyState(VK_RIGHT) < 0)) {
        m_hWndCurrentChild = hFirstDlgItem;
    } else {
        m_hWndCurrentChild = ::GetFocus();
    }

    ::SetFocus(m_hWndCurrentChild);
}

//**************************************************************************
// When a Child control gets the focus, it will send this message to
// activate the Control.
LONG CFormControl::OnActivateControl(UINT, LONG)
{
    if (!m_bUIActive) {
        OnActivateInPlace(TRUE, NULL);
        GetOuterWindow()->GetParent()->SendMessage(WM_ACTIVATE_CONTROL, 0, 0);
    }

    return 0;
}

//**************************************************************************
// Initialize the child controls.
void CFormControl::OnInitialUpdate()
{
    ASSERT_VALID(this);

    if (!UpdateData(FALSE))
        TRACE0("UpdateData failed during CFormControl initial update.\n");
}

//**************************************************************************
// Functions adding OLE control container support. Copied from MFC.
#ifndef _AFX_NO_OCC_SUPPORT

LRESULT CFormControl::HandleInitDialog(WPARAM, LPARAM)
{
    Default();  // allow default to initialize first (common dialogs/etc)
    // create OLE controls
    COccManager* pOccManager = afxOccManager;

    if ((pOccManager != NULL) && (m_pOccDialogInfo != NULL)) {
        if (!pOccManager->CreateDlgControls(this, m_lpszTemplateName,
                                            m_pOccDialogInfo)) {
            TRACE0("Warning: CreateDlgControls failed during form view init.\n");
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CFormControl::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo)
{
    m_pOccDialogInfo = pOccDialogInfo;
    return TRUE;
}

#endif //!_AFX_NO_OCC_SUPPORT


//**************************************************************************
// Remove the default flag fastBeginPaint so that the dialog template will
// be allowed to draw itself.
DWORD CFormControl::GetControlFlags()
{
    return 0;
}

//**************************************************************************
// Draw a default design time representation of the control.
void CFormControl::OnDraw(
    CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
    if (!m_hWnd) {
        CPen* pOldPen = (CPen*)pdc->SelectStockObject(BLACK_PEN);
        CString temp = AmbientDisplayName();

        if (!temp.IsEmpty()) {
            m_sFormName = temp;
        }

        // Initialize template dimensions.
        CRect rc(rcBounds.left,
                 rcBounds.top,
                 rcBounds.left + m_cTemplateSize.cx,
                 rcBounds.top + m_cTemplateSize.cy);

        if (m_bAutoSize) {
            pdc->DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
        } else {
            // Draw the current boundries in solid black.
            pdc->Rectangle(rcBounds);
            // Draw the template dimensions as EDGE_SUNKEN.
            pdc->DrawEdge(rc, EDGE_SUNKEN, BF_RECT);
        }

        // Print out the name of the form.
        pdc->SetTextAlign(TA_LEFT | TA_TOP);
        pdc->ExtTextOut(rcBounds.left + 10, rcBounds.top + 10,
                        ETO_CLIPPED, rcBounds, m_sFormName, m_sFormName.GetLength(), NULL);
        pdc->SelectObject(pOldPen);
    }
}

//**************************************************************************
// Disable Mouse messages in the FormControl itself. We want this to pretend to be
// the background.
int CFormControl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    return MA_NOACTIVATE;
}

void CFormControl::OnMouseMove(UINT nFlags, CPoint point) {}
void CFormControl::OnLButtonDblClk(UINT nFlags, CPoint point) {}
void CFormControl::OnLButtonDown(UINT nFlags, CPoint point) {}
void CFormControl::OnLButtonUp(UINT nFlags, CPoint point) {}
void CFormControl::OnMButtonDblClk(UINT nFlags, CPoint point) {}
void CFormControl::OnMButtonDown(UINT nFlags, CPoint point) {}
void CFormControl::OnMButtonUp(UINT nFlags, CPoint point) {}
void CFormControl::OnRButtonDblClk(UINT nFlags, CPoint point) {}
void CFormControl::OnRButtonDown(UINT nFlags, CPoint point) {}
void CFormControl::OnRButtonUp(UINT nFlags, CPoint point) {}
