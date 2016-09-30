// BullsEyePropPage.cpp : Implementation of CBullsEyePropPage
#include "stdafx.h"
#include "BullsEye.h"
#include "BullsEyePropPage.h"

/////////////////////////////////////////////////////////////////////////////
// IPropertyPage interface methods

/**************/
/* SetObjects */
/**************/

// Implementation notes:
// Visual Basic 6 does not obey the stated requirements for this method.

// Here are the requirements:
// The property page is required to keep the pointers returned by this method
// or others queried through them. If these specific IUnknown pointers are held,
// the property page must call IUnknown::AddRef through each when caching them,
// until the time when IPropertyPage::SetObjects is called with cObjects equal to zero.
// At that time, the property page must call IUnknown::Release through each pointer,
// releasing any objects that it held.

// The caller must provide the property page with these objects before calling
// IPropertyPage::Activate, and should call IPropertyPage::SetObjects with zero
// as the parameter when deactivating the page or when releasing the object entirely.
// Each call to SetObjects with a non-NULL ppUnk parameter must be matched with a
// later call to SetObjects with zero in the cObjects parameter.

// Visual Basic 6 can repeatedly call SetObjects with non-zero in the cObjects parameter
// with no intervening call to SetObjects with zero as the cObjects parameter.
//   To reproduce:
//      In design mode: Add control(s) to form. Select control(s).
//      Display the controls' property page(s) (View Property Pages (Shift+F4)).
//      VB calls SetObjects with a non-zero cObjects to send the selected object(s)
//          to the property page (as it should).
//      Change a property using VB's View Property Window (F4)
//      VB calls SetObjects *again* with a non-zero cObjects presumably so that the
//          the property page reloads the changed property from the object(s).
//      Implementing IPropertyNotifySink on the property page (the correct way to
//          receive property change notifications) doesn't affect this behavior.

// Visual Basic 6 normally *never* calls SetObjects with zero in the cObjects parameter.
//  To reproduce:
//      Do the above and close the VB project.
//      Notice that SetObjects is never called to release the object array.
//      Workaround is to released the array in the property page destructor
//          if it isn't already released.

// Visual Basic 6 only calls SetObjects with zero in the cObjects parameter when
// a preceding call to SetObjects with a non-zero value for cObjects returns
// a failure status.

STDMETHODIMP CBullsEyePropPage::SetObjects(ULONG cObjects, IUnknown** ppUnk)
{
    ATLTRACE2(atlTraceControls, 2, _T("CBullsEyePropPage::SetObjects\n"));

    if (ppUnk == NULL) return E_POINTER;

    ///////////////////////////////////////////////////////
    // Remove previous objects, connections, and cookies //
    ///////////////////////////////////////////////////////
    CleanupObjectArray();

    ///////////////////////////////////////////////////////////////////
    // Allocate new object array, make connections, and save cookies //
    ///////////////////////////////////////////////////////////////////
    if (cObjects > 0) {
        // Allocate object array
        ATLTRY(m_ppUnk = new LPUNKNOWN[cObjects]);

        if (m_ppUnk == NULL) return E_OUTOFMEMORY;

        // Allocate connection cookies
        ATLTRY(m_pCookies = new DWORD[cObjects]);

        if (m_pCookies == NULL) {
            delete [] m_ppUnk;
            m_ppUnk = NULL;
            return E_OUTOFMEMORY;
        }

        // Make a connection to each object's connection point
        for (UINT i = 0; i < cObjects; i++) {
            // Ensure object supports the default interface...
            // We need this interface to send changes back to object
            HRESULT hr = ppUnk[i]->QueryInterface(IID_IBullsEye, (void**)&m_ppUnk[i]);

            if (FAILED(hr)) return hr;

            // Establish a connection point from object to our sink...
            // We need this to receive change notifications from the object
            hr = AtlAdvise(m_ppUnk[i],
                           static_cast<IPropertyNotifySink*>(this),
                           IID_IPropertyNotifySink,
                           &m_pCookies[i]);
//            if (FAILED(hr)) return hr;
        }
    }

    m_nObjects = cObjects;
    ////////////////////////////////////////////////////////////
    // Transfer properties from first object to property page //
    ////////////////////////////////////////////////////////////
    InitializeControlsFromObject(DISPID_UNKNOWN);
    return S_OK;
}

/************/
/* Activate */
/************/

STDMETHODIMP CBullsEyePropPage::Activate(HWND hWndParent, LPCRECT pRect, BOOL bModal)
{
    ATLTRACE(_T("CBullsEyePropPage::Activate\n"));
    HRESULT hr = IPropertyPage2Impl<CBullsEyePropPage>::Activate(hWndParent, pRect, bModal);

    if (SUCCEEDED(hr) && DISPID_UNKNOWN != m_EditProperty) {
        EditProperty(m_EditProperty);
        m_EditProperty = DISPID_UNKNOWN;
    }

    return hr;
}

/*********/
/* Apply */
/*********/

STDMETHODIMP CBullsEyePropPage::Apply(void)
{
    ATLTRACE(_T("CBullsEyePropPage::Apply\n"));
    SetPropertiesFromControls(DISPID_UNKNOWN);
    m_bDirty = FALSE;
    m_flags  = 0;
    return S_OK;
}

/****************/
/* EditProperty */
/****************/

STDMETHODIMP CBullsEyePropPage::EditProperty(DISPID dispid)
{
    ATLTRACE(_T("CBullsEyePropPage::EditProperty\n"));
    m_EditProperty = dispid;

    if (IsWindow()) {
        switch (dispid) {
        case DISPID_BEEP:
            ::SetFocus(GetDlgItem(IDC_BEEP));
            break;

        case DISPID_ENABLED:
            ::SetFocus(GetDlgItem(IDC_ENABLED));
            break;

        case DISPID_RINGCOUNT:
            ::SetFocus(GetDlgItem(IDC_RINGCOUNT));
            break;

        case DISPID_BACKSTYLE:
            ::SetFocus(GetDlgItem(IDC_TRANSPARENT));
            break;

        default:
            return E_INVALIDARG;
        }
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// IPropertyNotifySink interface methods

STDMETHODIMP CBullsEyePropPage::OnChanged(DISPID dispid)
{
    InitializeControlsFromObject(dispid);
    return S_OK;
}

STDMETHODIMP CBullsEyePropPage::OnRequestEdit(DISPID dispid)
{
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CBullsEyePropPage window message handlers

/****************/
/* OnInitDialog */
/****************/

LRESULT CBullsEyePropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    InitializeControlsFromObject(DISPID_UNKNOWN);
    return 0;
}

/************/
/* OnCreate */
/************/

LRESULT CBullsEyePropPage::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    INITCOMMONCONTROLSEX ic = { sizeof(ic), ICC_UPDOWN_CLASS };
    BOOL bInit = InitCommonControlsEx(&ic);
    return 0;
}

/*************/
/* OnDestroy */
/*************/

LRESULT CBullsEyePropPage::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CleanupObjectArray();
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBullsEyePropPage control notification handlers

/*****************/
/* OnClickedBeep */
/*****************/

LRESULT CBullsEyePropPage::OnClickedBeep(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    m_beepNew = (BST_CHECKED == SendDlgItemMessage(wID, BM_GETCHECK)) ? VARIANT_TRUE : VARIANT_FALSE;

    if (m_beepNew != m_beepOrig)
        m_flags |= BEEP_CHANGED;
    else
        m_flags &= ~BEEP_CHANGED;

    SetDirty(m_flags != 0);
    return 0;
}

/********************/
/* OnClickedEnabled */
/********************/

LRESULT CBullsEyePropPage::OnClickedEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    m_enabledNew = (BST_CHECKED == SendDlgItemMessage(wID, BM_GETCHECK)) ? VARIANT_TRUE : VARIANT_FALSE;

    if (m_enabledNew != m_enabledOrig)
        m_flags |= ENABLED_CHANGED;
    else
        m_flags &= ~ENABLED_CHANGED;

    SetDirty(m_flags != 0);
    return 0;
}

/*********************/
/* OnChangeRingCount */
/*********************/

LRESULT CBullsEyePropPage::OnChangeRingCount(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    BOOL bTranslated = FALSE;
    m_ringCountNew = (short) GetDlgItemInt(IDC_RINGCOUNT, &bTranslated, FALSE);

    if (!bTranslated || m_ringCountNew < 1 || m_ringCountNew > 9) {
        // Non-numeric or out of range value in edit control - reinitialize it
        SendDlgItemMessage(IDC_SPIN1, UDM_SETPOS,   0, MAKELONG(m_ringCountNew, 0));
        return 0;
    }

    if (m_ringCountOrig != m_ringCountNew)
        m_flags |= RINGCOUNT_CHANGED;
    else
        m_flags &= ~RINGCOUNT_CHANGED;

    SetDirty(m_flags != 0);
    return 0;
}

/************************/
/* OnClickedTransparent */
/************************/

LRESULT CBullsEyePropPage::OnClickedTransparent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    m_transparentNew = (BST_CHECKED == SendDlgItemMessage(wID, BM_GETCHECK)) ? 1 : 0;

    if (m_transparentNew != m_transparentOrig)
        m_flags |= TRANSPARENT_CHANGED;
    else
        m_flags &= ~TRANSPARENT_CHANGED;

    SetDirty(m_flags != 0);
    return 0;
}

/********************************/
/* InitializeControlsFromObject */
/********************************/

void CBullsEyePropPage::InitializeControlsFromObject(DISPID dispid)
{
    // The array already contains IBullsEye* so this is redundant...
    CComQIPtr<IBullsEye> pBullsEye = m_ppUnk[0];

    if (DISPID_RINGCOUNT == dispid || DISPID_UNKNOWN == dispid) {
        // Get RingCount property
        HRESULT hr = pBullsEye->get_RingCount(&m_ringCountOrig);
        ATLASSERT(SUCCEEDED(hr));
        m_ringCountNew = m_ringCountOrig;

        if (IsWindow()) {
            SendDlgItemMessage(IDC_SPIN1, UDM_SETRANGE, 0, MAKELONG(9, 1));
            SendDlgItemMessage(IDC_SPIN1, UDM_SETPOS,   0, MAKELONG(m_ringCountOrig, 0));
        }
    }

    if (DISPID_BEEP == dispid || DISPID_UNKNOWN == dispid) {
        // Get Beep property
        HRESULT hr = pBullsEye->get_Beep(&m_beepOrig);
        ATLASSERT(SUCCEEDED(hr));
        m_beepNew = m_beepOrig;

        if (IsWindow()) {
            SendDlgItemMessage(IDC_BEEP, BM_SETCHECK, m_beepOrig ? BST_CHECKED : BST_UNCHECKED);
        }
    }

    if (DISPID_ENABLED == dispid || DISPID_UNKNOWN == dispid) {
        // Get Enabled property
        HRESULT hr = pBullsEye->get_Enabled(&m_enabledOrig);
        ATLASSERT(SUCCEEDED(hr));
        m_enabledNew = m_enabledOrig;

        if (IsWindow()) {
            SendDlgItemMessage(IDC_ENABLED, BM_SETCHECK, m_enabledOrig ? BST_CHECKED : BST_UNCHECKED);
        }
    }

    if (DISPID_BACKSTYLE == dispid || DISPID_UNKNOWN == dispid) {
        // Get Enabled property
        HRESULT hr = pBullsEye->get_BackStyle(&m_transparentOrig);
        ATLASSERT(SUCCEEDED(hr));
        m_transparentNew = m_transparentOrig;

        if (IsWindow()) {
            SendDlgItemMessage(IDC_TRANSPARENT, BM_SETCHECK, m_transparentOrig ? BST_CHECKED : BST_UNCHECKED);
        }
    }
}

/*****************************/
/* SetPropertiesFromControls */
/*****************************/

void CBullsEyePropPage::SetPropertiesFromControls(DISPID dispid)
{
    // In case QI failed in SetObjects
    if (!m_ppUnk || !m_ppUnk[0]) return;

    // For all objects in array...
    for (UINT i = 0; i < m_nObjects; i++) {
        // Get the appropriate interface...
        CComQIPtr<IBullsEye> pBullsEye = m_ppUnk[i];

        // Update the RingCount property, if requested and required
        if ((DISPID_RINGCOUNT == dispid || DISPID_UNKNOWN == dispid) &&
            m_flags & RINGCOUNT_CHANGED) {
            pBullsEye->put_RingCount(m_ringCountNew);   // Update property
            m_ringCountOrig = m_ringCountNew;           // Prop page and property synced
            m_flags &= ~RINGCOUNT_CHANGED;              // Clear changed flag
        }

        // Update the Beep property, if requested and required
        if ((DISPID_BEEP == dispid || DISPID_UNKNOWN == dispid) &&
            m_flags & BEEP_CHANGED) {
            pBullsEye->put_Beep(m_beepNew);             // Update property
            m_beepOrig = m_beepNew;                     // Prop page and property synced
            m_flags &= ~BEEP_CHANGED;                   // Clear changed flag
        }

        // Update the Enabled property, if requested and required
        if ((DISPID_ENABLED == dispid || DISPID_UNKNOWN == dispid) &&
            m_flags & ENABLED_CHANGED) {
            pBullsEye->put_Enabled(m_enabledNew);       // Update property
            m_enabledOrig = m_enabledNew;               // Prop page and property synced
            m_flags &= ~ENABLED_CHANGED;                // Clear changed flag
        }

        // Update the BackStyle property, if requested and required
        if ((DISPID_BACKSTYLE == dispid || DISPID_UNKNOWN == dispid) &&
            m_flags & TRANSPARENT_CHANGED) {
            pBullsEye->put_BackStyle(m_transparentNew); // Update property
            m_transparentOrig = m_transparentNew;       // Prop page and property synced
            m_flags &= ~TRANSPARENT_CHANGED;            // Clear changed flag
        }
    }
}

/**********************/
/* CleanupObjectArray */
/**********************/

void CBullsEyePropPage::CleanupObjectArray()
{
    // Free existing array of objects, if any
    if (m_ppUnk != NULL && m_nObjects > 0) {
        for (UINT i = 0; i < m_nObjects; i++) {
            if (NULL == m_ppUnk[i]) break;

            // Unadvise the connection
            AtlUnadvise(m_ppUnk[i], IID_IPropertyNotifySink, m_pCookies[i]);
            m_ppUnk[i]->Release();
        }

        delete [] m_ppUnk;
        delete [] m_pCookies;
    }

    // Currently no objects in list
    m_ppUnk    = NULL;
    m_pCookies = NULL;
    m_nObjects = 0;
}
