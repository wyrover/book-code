// BullsEye.cpp : Implementation of CBullsEye
#include "stdafx.h"
#include "Designer.h"
#include "BullsEye.h"
#include "AboutDlg.h"

// CBullsEye

// Constructor
CBullsEye::CBullsEye()
{
    // Stock properties
    m_nBackStyle        = 0;        // Opaque    = 0, Transparent  = 1
    m_nBorderStyle      = 0;        // No border = 0, Fixed single = 1
    m_nBorderWidth      = 1;
    m_clrBackColor      = RGB(255, 255, 255);
    m_clrForeColor      = RGB(0,  0,  0);
    m_bEnabled          = TRUE;
    // Custom properties
    m_clrAlternateColor = RGB(0,  0,  0);
    m_clrCenterColor    = RGB(255,  0,  0);
    m_beep              = VARIANT_TRUE;
    // Control state
    m_alternateBrush    = NULL;
    m_backBrush         = NULL;
    m_centerBrush       = NULL;
    m_borderPen         = NULL;
    m_nAppearance       = 0;
    m_bAmbientsFetched  = false;
}

HRESULT CBullsEye::FinalConstruct()
{
    FreezeEvents(TRUE);
    put_RingCount(5);
    FreezeEvents(FALSE);
    return S_OK;
}

CBullsEye::~CBullsEye()
{
    if (m_alternateBrush) ::DeleteObject(m_alternateBrush);

    if (m_backBrush)      ::DeleteObject(m_backBrush);

    if (m_centerBrush)    ::DeleteObject(m_centerBrush);

    if (m_borderPen)      ::DeleteObject(m_borderPen);
}


// Stock Methods
// TODO: Provide actual implementation

STDMETHODIMP CBullsEye::AboutBox()
{
    CAboutDlg dlg;
    dlg.DoModal();
    return S_OK;
}

STDMETHODIMP CBullsEye::DoClick()
{
    HitRing(0);
    return S_OK;
}

STDMETHODIMP CBullsEye::Refresh()
{
    FireViewChange();
    return S_OK;
}

STDMETHODIMP CBullsEye::InterfaceSupportsErrorInfo(const IID &riid)
{
    static const IID* arr[] = {
        &IID_IBullsEye,
    };

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        if (InlineIsEqualGUID(*arr[i], riid))
            return S_OK;
    }

    return S_FALSE;
}

#define ASSERT_SUCCESS( b ) ATLASSERT( ( b ) != 0 )
#define VERIFY_SUCCESS( c ) { BOOL bSuccess = ( c ); ATLASSERT( bSuccess != 0 ); }

// Drawing code

static const int LOGWIDTH = 1000;

// Note on optimized drawing:
// If TRUE, indicates that the object can support optimized rendering of itself.
// Since most objects on a form share the same font, background color, and border types,
// leaving these values in the device context allows the next object to use them without
// having to re-select them. Specifically, the object can leave the font, brush, and pen
// selected on return from the IViewObject::Draw method instead of deselecting these from
// the device context. The container then must deselect these values at the end of the
// overall drawing process. The object can also leave other drawing state changes in the
// device context, such as the background color, the text color, raster operation code,
// the current point, the line drawing, and the poly fill mode. The object cannot change
// state values unless other objects are capable of restoring them.

HRESULT CBullsEye::OnDraw(ATL_DRAWINFO &di)
{
    CRect rc = *((RECT *)di.prcBounds);
    HDC hdc  = di.hdcDraw;

    // Create the background color brush only when necessary
    if (m_backBrush == 0) {
        OLE_COLOR ocBack;
        HRESULT hr = get_BackColor(&ocBack);        // Get the background color
        ATLASSERT(SUCCEEDED(hr));
        COLORREF  crBack;                          // Translate the color to a COLORREF
        hr = ::OleTranslateColor(ocBack, NULL, &crBack);
        ATLASSERT(SUCCEEDED(hr));
        m_backBrush = ::CreateSolidBrush(crBack);    // Create the background brush
        ATLASSERT(m_backBrush != 0);
    }

    // First, fill in background color in invalid area when BackStyle is Opaque
    if (m_nBackStyle == 1 /* Opaque*/) {
        VERIFY_SUCCESS(::FillRect(hdc, &rc, m_backBrush));
    }

    int nPrevMapMode;
    POINT   ptWOOrig, ptVOOrig;
    SIZE szWEOrig, szVEOrig;
    BOOL bMetafile = GetDeviceCaps(di.hdcDraw, TECHNOLOGY) == DT_METAFILE;

    if (!bMetafile) {
        // OnDrawAdvanced normalized the device context
        // We are now using MM_TEXT and the coordinates are in device coordinates.
        // Establish convenient coordinate system
        VERIFY_SUCCESS(nPrevMapMode = ::SetMapMode(hdc, MM_ISOTROPIC));
        // Map logical 0,0 to physical center of rectangle
        VERIFY_SUCCESS(::SetWindowOrgEx(hdc, 0, 0, &ptWOOrig));
        VERIFY_SUCCESS(::SetViewportOrgEx(hdc, rc.left + rc.Width() / 2,
                                          rc.top  + rc.Height() / 2, &ptVOOrig));
        // Map logical extent (LOGWIDTH, LOGWIDTH) to physical extent of rectangle
        VERIFY_SUCCESS(::SetWindowExtEx(hdc, LOGWIDTH, LOGWIDTH, &szWEOrig));
        VERIFY_SUCCESS(::SetViewportExtEx(hdc, rc.Width(), -rc.Height(), &szVEOrig));
    } else {
        // We will be played back in ANISOTROPIC mapping mode
        // The rectangle will be in device units
        CRect rcBoundsDP(rc);
        // We can't use SetViewportOrg and SetViewportExt in a metafile
        // because the container will want to place the metafile.
        //
        // Find the center coordinate and the shorter side
        CSize size = rcBoundsDP.Size();
        int iShortSide = min(size.cx, size.cy);
        CPoint ptCenter(rcBoundsDP.left + size.cx / 2,
                        rcBoundsDP.top  + size.cy / 2);
        // Compute the ratio of LOGWIDTH / shorter side
        double dRatio = double(LOGWIDTH) / double(iShortSide);
        // Set the logical origin of the window and swap coordinate axes
        VERIFY_SUCCESS(SetWindowOrgEx(hdc, -int(ptCenter.x * dRatio), int(ptCenter.y * dRatio), &ptWOOrig));
        // Set the logical extent of the window
        // Compensate for the drawing code which draws from -LOGWIDTH to +LOGWIDTH
        VERIFY_SUCCESS(SetWindowExtEx(hdc, int(size.cx * dRatio), -int(size.cy * dRatio), &szWEOrig));
    }

    // Draw the BullsEye
    DrawBullsEye(di);

    // Note on optimized drawing:
    // Even when using optimized drawing, a control cannot
    // leave a changed mapping mode, coordinate transformation value,
    // selected bitmap, clip region, or metafile in the device context.

    if (!bMetafile) {
        ::SetMapMode(hdc, nPrevMapMode);
        ::SetViewportOrgEx(hdc, ptVOOrig.x,  ptVOOrig.y,  NULL);
        ::SetViewportExtEx(hdc, szVEOrig.cx, szVEOrig.cy, NULL);
    }

    ::SetWindowOrgEx(hdc, ptWOOrig.x,  ptWOOrig.y,  NULL);
    ::SetWindowExtEx(hdc, szWEOrig.cx, szWEOrig.cy, NULL);
    return S_OK;
}

void CBullsEye::DrawBullsEye(ATL_DRAWINFO &di)
{
    HDC hdc  = di.hdcDraw;

    // Create the border pen only when necessary
    if (m_borderPen == 0) {
        OLE_COLOR ocFore;
        HRESULT hr = get_ForeColor(&ocFore);
        ATLASSERT(SUCCEEDED(hr));
        COLORREF crFore;
        hr = ::OleTranslateColor(ocFore, NULL, &crFore);
        ATLASSERT(SUCCEEDED(hr));
        m_borderPen = ::CreatePen(PS_SOLID, 0, crFore);
        ATLASSERT(m_borderPen != 0);
    }

    // Create the center color brush only when necessary
    if (m_centerBrush == 0) {
        COLORREF  crCenter;
        HRESULT hr = ::OleTranslateColor(m_clrCenterColor, NULL, &crCenter);
        ATLASSERT(SUCCEEDED(hr));
        m_centerBrush = ::CreateSolidBrush(crCenter);
        ATLASSERT(m_centerBrush != 0);
    }

    // Create the alternate color brush only when necessary
    if (m_alternateBrush == 0) {
        COLORREF  crAlternate;
        HRESULT hr = ::OleTranslateColor(m_clrAlternateColor, NULL, &crAlternate);
        ATLASSERT(SUCCEEDED(hr));
        m_alternateBrush = ::CreateSolidBrush(crAlternate);
        ATLASSERT(m_alternateBrush != 0);
    }

    // Compute the width of a ring
    short sRingCount;
    HRESULT hr = get_RingCount(&sRingCount);
    ATLASSERT(SUCCEEDED(hr));
    int ringWidth = LOGWIDTH / (sRingCount * 2 - 1);
    // Draw the border between rings using the border pen
    HPEN hOldPen = (HPEN)SelectObject(hdc, m_borderPen);
    HBRUSH hOldBrush = 0;

    // Draw each ring from outermost to innermost
    // This isn't nearly as efficient as it could be
    for (short i = sRingCount - 1; i >= 0; --i) {
        // Compute the ring's bounding rectangle
        int ringDiameter = i * 2 * ringWidth + ringWidth;
        int ringRadius   = ringDiameter / 2;
        CRect rcRing(-ringRadius, ringRadius, ringRadius, -ringRadius);
        // Rings are numbered from 1 to N from center to outside.
        // However the loop iterates from N-1 to 0 from outside to center.
        // Therefore even numbered rings should be the center color
        // which implies odd numbered rings use the alternate color.
        HBRUSH& ringBrush = i & 1 ? m_alternateBrush : m_centerBrush;
        // Set the correct ring color
        HBRUSH hBrush = (HBRUSH)::SelectObject(hdc, ringBrush);

        if (hOldBrush == 0) {      // First time through, save the original brush
            hOldBrush = hBrush;
        }

        BOOL bStatus = ::Ellipse(hdc, rcRing.left, rcRing.right, rcRing.top, rcRing.bottom);      // Draw the ring
        ATLASSERT(bStatus != 0);
    }

    // When optimized drawing not in effect, restore the original pen and brush
    if (!di.bOptimize) {
        ::SelectObject(hdc, hOldPen);
        ::SelectObject(hdc, hOldBrush);
    }
}

//
// Stock property change notifications
//
void CBullsEye::OnForeColorChanged()
{
    if (m_borderPen != 0) {
        ::DeleteObject(m_borderPen);
        m_borderPen = 0;
    }

    FireViewChange();
}

void CBullsEye::OnBackColorChanged()
{
    if (m_backBrush != 0) {
        ::DeleteObject(m_backBrush);
        m_backBrush = 0;
    }

    FireViewChange();
}

void CBullsEye::OnBackStyleChanged()
{
    FireViewChange();
}

// Custom properties

STDMETHODIMP CBullsEye::get_CenterColor(OLE_COLOR *pVal)
{
    if (pVal == 0) return E_POINTER;

    *pVal = m_clrCenterColor;
    return S_OK;
}

STDMETHODIMP CBullsEye::put_CenterColor(OLE_COLOR newVal)
{
    if (m_clrCenterColor == newVal) return S_OK;

    if (!m_nFreezeEvents) {
        if (FireOnRequestEdit(DISPID_CENTERCOLOR) == S_FALSE) {
            return S_FALSE;
        }
    }

    m_clrCenterColor = newVal;             // Save new color
    ::DeleteObject(m_centerBrush);         // Clear old brush color
    m_centerBrush = 0;
    m_bRequiresSave = TRUE;                // Set dirty flag

    if (!m_nFreezeEvents) {
        FireOnChanged(DISPID_CENTERCOLOR); // Notify container of change
    }

    FireViewChange();                      // Request redraw
    SendOnDataChange(0);                   // Notify advise sinks of change
    return S_OK;
}

STDMETHODIMP CBullsEye::get_AlternateColor(OLE_COLOR *pVal)
{
    if (pVal == 0) return E_POINTER;

    *pVal = m_clrAlternateColor;
    return S_OK;
}

STDMETHODIMP CBullsEye::put_AlternateColor(OLE_COLOR newVal)
{
    if (m_clrAlternateColor == newVal) return S_OK;

    if (!m_nFreezeEvents) {
        if (FireOnRequestEdit(DISPID_ALTERNATECOLOR) == S_FALSE) {
            return S_FALSE;
        }
    }

    m_clrAlternateColor = newVal;            // Save new color
    ::DeleteObject(m_alternateBrush);        // Clear old brush color
    m_alternateBrush = 0;
    m_bRequiresSave = TRUE;                   // Set dirty flag

    if (!m_nFreezeEvents) {
        FireOnChanged(DISPID_ALTERNATECOLOR); // Notify container of change
    }

    FireViewChange();                         // Request redraw
    SendOnDataChange(0);                      // Notify advise sinks of change
    return S_OK;
}

STDMETHODIMP CBullsEye::get_Beep(VARIANT_BOOL *pVal)
{
    if (pVal == 0) return E_POINTER;

    *pVal = m_beep;
    return S_OK;
}

STDMETHODIMP CBullsEye::put_Beep(VARIANT_BOOL newVal)
{
    if (m_beep == newVal) return S_OK;

    if (!m_nFreezeEvents) {
        if (FireOnRequestEdit(DISPID_BEEP) == S_FALSE) {
            return S_FALSE;
        }
    }

    m_beep = newVal;                // Save new value
    m_bRequiresSave = TRUE;         // Set dirty flag

    if (!m_nFreezeEvents) {
        FireOnChanged(DISPID_BEEP); // Notify container of change
    }

    SendOnDataChange(0);            // Notify advise sinks of change
    return S_OK;
}

STDMETHODIMP CBullsEye::get_RingCount(short *pVal)
{
    if (pVal == 0) return E_POINTER;

    *pVal = m_ringValues.GetSize();
    return S_OK;
}

STDMETHODIMP CBullsEye::put_RingCount(short newVal)
{
    if (newVal <= 0 || newVal > 9) {
        return Error(IDS_INVALIDRINGCOUNT, CLSID_BullsEye, CTL_E_INVALIDPROPERTYARRAYINDEX);
    }

    if (m_ringValues.GetSize() == newVal) { // Cannot call get_RingCount here
        return S_OK;
    }

    if (!m_nFreezeEvents) {
        if (FireOnRequestEdit(DISPID_RINGCOUNT) == S_FALSE) {
            return S_FALSE;
        }
    }

    m_ringValues.RemoveAll();
    long defValue = 512;

    for (short i = 0; i < newVal; i++) {
        m_ringValues.Add(defValue);
        defValue /= 2;

        if (defValue == 0)              // Minimum value is always one
            defValue = 1;
    }

    m_bRequiresSave = TRUE;             // Set dirty flag

    if (!m_nFreezeEvents) {
        FireOnChanged(DISPID_RINGCOUNT);// Notify container of change
    }

    FireViewChange();                   // Request redraw
    SendOnDataChange(0);             // Notify advise sinks of change
    return S_OK;
}

HRESULT CBullsEye::ValidateRingNumberInRange(short ringNum)
{
    if (ringNum < 1) {
        return Error(IDS_INVALIDRINGLESSTHANONE, CLSID_BullsEye, CTL_E_INVALIDPROPERTYARRAYINDEX);
    }

    if (ringNum > m_ringValues.GetSize()) {
        return Error(IDS_INVALIDRINGNUMBER, CLSID_BullsEye, CTL_E_INVALIDPROPERTYARRAYINDEX);
    }

    return S_OK;
}

STDMETHODIMP CBullsEye::get_RingValue(short ringNum, long *pVal)
{
    if (pVal == 0) return E_POINTER;

    HRESULT hr = ValidateRingNumberInRange(ringNum);

    if (FAILED(hr)) return hr;

    *pVal = m_ringValues[ ringNum - 1 ];
    return S_OK;
}

STDMETHODIMP CBullsEye::put_RingValue(short ringNum, long newVal)
{
    HRESULT hr = ValidateRingNumberInRange(ringNum);

    if (FAILED(hr)) return hr;

    if (m_ringValues[ ringNum - 1 ] == newVal) return S_OK;

    if (!m_nFreezeEvents) {
        if (FireOnRequestEdit(DISPID_RINGVALUE) == S_FALSE) {
            return S_FALSE;
        }
    }

    m_ringValues[ ringNum - 1 ] = newVal;
    m_bRequiresSave = TRUE;              // Set dirty flag

    if (!m_nFreezeEvents) {
        FireOnChanged(DISPID_RINGVALUE); // Notify container of change
    }

    SendOnDataChange(0);                 // Notify advise sinks of change
    return S_OK;
}

STDMETHODIMP CBullsEye::get_Application(LPDISPATCH *ppDisp)
{
    if (!ppDisp) return E_POINTER;

    // First try to get the Application object directly
    HRESULT hr = GetApplicationFromServiceProvider(ppDisp);

    if (SUCCEEDED(hr)) return hr;

    static OLECHAR szApplication [] = OLESTR("Application");
    // Try by asking the Document object (our container) for its Application property
    return GetPropertyFromDocument(szApplication, ppDisp);
}

STDMETHODIMP CBullsEye::get_Parent(IDispatch **ppDisp)
{
    if (ppDisp == 0) return E_POINTER;

    *ppDisp = NULL;
    CComPtr<IOleClientSite> pClientSite;
    HRESULT hr = GetClientSite(&pClientSite);
    ATLASSERT(SUCCEEDED(hr));

    if (FAILED(hr)) return hr;       // We really, really, really should have had a client site!!!

    // Get containing document object
    CComPtr<IOleContainer> pOleContainer;
    hr = pClientSite->GetContainer(&pOleContainer);

    if (FAILED(hr)) return hr;        // GetContainer is technically an optional method...

    // Get the automation interface of the Document object
    return pOleContainer->QueryInterface(ppDisp);
}


HRESULT CBullsEye::GetApplicationFromServiceProvider(/* [out] */ IDispatch** ppDisp)
{
    if (NULL == ppDisp) return E_POINTER;

    *ppDisp = NULL;
    CComPtr<IOleClientSite> pClientSite;
    HRESULT hr = GetClientSite(&pClientSite);
    ATLASSERT(SUCCEEDED(hr));

    if (FAILED(hr)) return hr;     // We really, really, really should have had a client site!!!

    // Does container implement IServiceProvider?
    CComQIPtr<IServiceProvider> pServiceProvider = pClientSite;

    if (!pServiceProvider) return E_NOINTERFACE;

    // Yes it does! Ask for the Application object...
    return pServiceProvider->QueryService(SID_SApplicationObject, ppDisp);
}

/***********************************/
/* GetDispatchPropertyFromDocument */
/***********************************/

HRESULT CBullsEye::GetPropertyFromDocument(/* [in] */ LPOLESTR pszProperty,
        /* [out] */ IDispatch** ppDisp)
{
    if (NULL == pszProperty) return E_INVALIDARG;

    if (OLESTR('\0') == *pszProperty) return E_INVALIDARG;

    if (NULL == ppDisp) return E_POINTER;

    *ppDisp = NULL;
    // Get the Automation interface of the Document object
    CComDispatchDriver  pDispatch;  // Destructor will release pDispatch!
    HRESULT hr = get_Parent(&pDispatch);

    if (SUCCEEDED(hr)) {
        CComVariant v;
        hr = pDispatch.GetPropertyByName(pszProperty, &v);

        if (SUCCEEDED(hr)) {
            if (v.vt != VT_DISPATCH) {
                hr = v.ChangeType(VT_DISPATCH);

                if (SUCCEEDED(hr)) {
                    *ppDisp = v.pdispVal;
                    v.vt = VT_EMPTY;
                    return S_OK;
                }
            }
        }
    }

    return hr;
}

void CBullsEye::HitRing(short ringNum)
{
    if (ringNum != -1) {
        if (m_beep) {
            ::PlaySound((LPCTSTR)IDR_ARROW, _AtlBaseModule.GetResourceInstance(), SND_RESOURCE | SND_ASYNC);
        }

        if (!m_nFreezeEvents) {
            Fire_OnRingHit(ringNum + 1);
            Fire_OnScoreChanged(m_ringValues[ ringNum ]);
        }
    }
}

//
// Custom serialization implementations
//

/***************************/
/* IPersistStreamInit_Save */
/***************************/

HRESULT CBullsEye::IPersistStreamInit_Save(LPSTREAM pStm, BOOL fClearDirty, ATL_PROPMAP_ENTRY* pMap)
{
    if (NULL == pStm) return E_POINTER;

    // Save the properties described in the PROP_MAP
    HRESULT hr = IPersistStreamInitImpl<CBullsEye>::IPersistStreamInit_Save(pStm,  fClearDirty, pMap);

    if (FAILED(hr)) return hr;

    // Save the indexed property - RingValues
    // Get the number of rings
    short sRingCount;
    get_RingCount(&sRingCount);

    // For each ring, write its value
    for (short nIndex = 1; nIndex <= sRingCount; nIndex++) {
        // Get the ring value
        long lRingValue ;
        get_RingValue(nIndex, &lRingValue);
        // Write it to the stream
        ULONG cbWritten;
        hr = pStm->Write(&lRingValue, sizeof(lRingValue), &cbWritten);
        ATLASSERT(SUCCEEDED(hr));
        ATLASSERT(sizeof(lRingValue) == cbWritten);

        if (FAILED(hr) || cbWritten != sizeof(lRingValue)) {
            hr = E_UNEXPECTED;
            break;
        }
    }

    return hr;
}

/***************************/
/* IPersistStreamInit_Load */
/***************************/

HRESULT CBullsEye::IPersistStreamInit_Load(LPSTREAM pStm, ATL_PROPMAP_ENTRY* pMap)
{
    if (NULL == pStm) return E_POINTER;

    // Load the properties described in the PROP_MAP
    HRESULT hr = IPersistStreamInitImpl<CBullsEye>::IPersistStreamInit_Load(pStm, pMap);

    if (FAILED(hr)) return hr;

    // Load the indexed property - RingValues
    // Get the number of rings
    short sRingCount;
    get_RingCount(&sRingCount);

    // For each ring, read its value
    for (short nIndex = 1; nIndex <= sRingCount; nIndex++) {
        // Read ring value from the stream
        long lRingValue ;
        ULONG cbRead;
        hr = pStm->Read(&lRingValue, sizeof(lRingValue), &cbRead);
        ATLASSERT(SUCCEEDED(hr));
        ATLASSERT(sizeof(lRingValue) == cbRead);

        if (FAILED(hr) || cbRead != sizeof(lRingValue)) {
            hr = E_UNEXPECTED;
            break;
        }

        // Set the ring value
        put_RingValue(nIndex, lRingValue);
    }

    if (SUCCEEDED(hr)) m_bRequiresSave = TRUE;

    return hr;
}

/****************************/
/* IPersistPropertyBag_Save */
/****************************/

HRESULT CBullsEye::IPersistPropertyBag_Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties, ATL_PROPMAP_ENTRY* pMap)
{
    if (NULL == pPropBag) return E_POINTER;

    // Save the properties described in the PROP_MAP
    HRESULT hr = IPersistPropertyBagImpl<CBullsEye>::IPersistPropertyBag_Save(pPropBag, fClearDirty, fSaveAllProperties, pMap);

    if (FAILED(hr)) return hr;

    // Save the indexed property - RingValues
    // Get the number of rings
    short sRingCount;
    get_RingCount(&sRingCount);

    // For each ring, write its value
    for (short nIndex = 1; nIndex <= sRingCount; nIndex++) {
        // Create the property name
        CComBSTR bstrName = OLESTR("RingValue");
        CComVariant vRingNumber = nIndex;
        hr = vRingNumber.ChangeType(VT_BSTR);
        ATLASSERT(SUCCEEDED(hr));
        bstrName += vRingNumber.bstrVal;
        // Get the ring value
        long lRingValue ;
        get_RingValue(nIndex, &lRingValue);
        // Write it to the stream
        CComVariant vValue = lRingValue;
        hr = pPropBag->Write(bstrName, &vValue);
        ATLASSERT(SUCCEEDED(hr));

        if (FAILED(hr)) {
            hr = E_UNEXPECTED;
            break;
        }
    }

    return hr;
}

/****************************/
/* IPersistPropertyBag_Load */
/****************************/

HRESULT CBullsEye::IPersistPropertyBag_Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, ATL_PROPMAP_ENTRY* pMap)
{
    if (NULL == pPropBag) return E_POINTER;

    // Load the properties described in the PROP_MAP
    HRESULT hr = IPersistPropertyBagImpl<CBullsEye>::IPersistPropertyBag_Load(pPropBag, pErrorLog, pMap);

    if (SUCCEEDED(hr))
        m_bRequiresSave = FALSE;

    if (FAILED(hr)) return hr;

    // Load the indexed property - RingValues
    // Get the number of rings
    short sRingCount;
    get_RingCount(&sRingCount);

    // For each ring, read its value
    for (short nIndex = 1; nIndex <= sRingCount; nIndex++) {
        // Create the base property name
        CComBSTR bstrName = OLESTR("RingValue");
        // Create ring number as a string
        CComVariant vRingNumber = nIndex;
        hr = vRingNumber.ChangeType(VT_BSTR);
        ATLASSERT(SUCCEEDED(hr));
        // Concatenate the two strings to form property name
        bstrName += vRingNumber.bstrVal;
        // Read ring value from the property bag
        CComVariant vValue = 0L;
        hr = pPropBag->Read(bstrName, &vValue, pErrorLog);
        ATLASSERT(SUCCEEDED(hr));
        ATLASSERT(VT_I4 == vValue.vt);

        if (FAILED(hr)) {
            hr = E_UNEXPECTED;
            break;
        }

        // Set the ring value
        put_RingValue(nIndex, vValue.lVal);
    }

    if (SUCCEEDED(hr)) m_bRequiresSave = TRUE;

    return hr;
}

/***************************************************/
/* IPersistPropertyBag/IPersistStreamInit::InitNew */
/***************************************************/

/*

  *** Special Initialization required to run with Access ***

Control Initialization

After insertion, switch immediately to browse view (View | Form View) to
make sure your control doesn’t cause the error - "No object in this
control." Many lightweight custom controls forget to mark themselves as
dirty after insertion in the container. Access optimizes its code-path
for saving cached merged type information for the extender object and
the controls inserted to just those controls that have been dirtied in
the current design session. When the user views the form in browse view,
the form gets destroyed, and recreated. If the control has not marked
itself as dirty, then just the extender object is on the form with no
control inside of it. This is a common mistake when control developers
use the BaseCtl framework, but the fix is simple. MFC and VB control
writers don’t have to worry about this since the frameworks handle this
for you. Here are some additional comments from an email thread on a
similar case:

Some BaseCtl framework OCXs are getting the "There is no object in this
control" error message when an Access ‘97 form containing the control is
reloaded. This is because Access ‘97 expects all controls to save their
binary state to a stream, and our controls only save when m_fDirty is
TRUE.

To behave correctly, controls must dirty themselves before returning
from their implementation of IPersistStreamInit::InitNew.
*/

HRESULT CBullsEye::InitNew()
{
    m_bRequiresSave = TRUE;
    return S_OK;
}

HRESULT CBullsEye::IQuickActivate_QuickActivate(QACONTAINER *pQACont, QACONTROL *pQACtrl)
{
    m_clrForeColor = pQACont->colorFore;
    m_clrBackColor = pQACont->colorBack;
    m_nAppearance  = (short) pQACont->dwAppearance;
    m_bAmbientsFetched = true;
    HRESULT hr = CComControlBase::IQuickActivate_QuickActivate(pQACont, pQACtrl);
    return hr;
}

/****************************/
/* IOleObject_SetClientSite */
/****************************/

HRESULT CBullsEye::IOleObject_SetClientSite(IOleClientSite *pClientSite)
{
    HRESULT hr = CComControlBase::IOleObject_SetClientSite(pClientSite);

    // If we QuickActivated, we already have the ambients desired
    // otherwise, this is the time to grab them
    if (!m_bAmbientsFetched) {
        HRESULT hr = GetAmbientBackColor(m_clrBackColor);
        hr = GetAmbientForeColor(m_clrForeColor);
        hr = GetAmbientAppearance(m_nAppearance);
    }

    return hr;
}

//
// ICategorizeProperties
//

STDMETHODIMP CBullsEye::MapPropertyToCategory(DISPID dispid, PROPCAT *ppropcat)
{
    switch (dispid) {
    case DISPID_FORECOLOR:
    case DISPID_BACKCOLOR:
    case DISPID_CENTERCOLOR:
    case DISPID_ALTERNATECOLOR:
    case DISPID_RINGCOUNT:
    case DISPID_BACKSTYLE:
        *ppropcat = PROPCAT_Appearance;
        return S_OK;

    case DISPID_BEEP:
    case DISPID_ENABLED:
        *ppropcat = PROPCAT_Behavior;
        return S_OK;

    case DISPID_RINGVALUE:
        *ppropcat = PROPCAT_Scoring;
        return S_OK;

    default:
        return E_FAIL;
    }
}

STDMETHODIMP CBullsEye::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR *pbstrName)
{
    if (propcat == PROPCAT_Scoring) {
        *pbstrName = ::SysAllocString(L"Scoring");
        return S_OK;
    }

    return E_FAIL;
}

//
// IPerPropertyBrowsing
//

STDMETHODIMP CBullsEye::GetDisplayString(DISPID dispid, BSTR *pBstr)
{
    switch (dispid) {
    case DISPID_BEEP:
        if (m_beep == VARIANT_TRUE) {
            *pBstr = SysAllocString(OLESTR("Yes"));
        } else {
            *pBstr = SysAllocString(OLESTR("No"));
        }

        return *pBstr ? S_OK : E_OUTOFMEMORY;

    case DISPID_BACKSTYLE:
        if (m_nBackStyle == 1) {
            *pBstr = SysAllocString(OLESTR("Opaque"));
        } else {
            *pBstr = SysAllocString(OLESTR("Transparent"));
        }

        return *pBstr ? S_OK : E_OUTOFMEMORY;

    case DISPID_ALTERNATECOLOR:
    case DISPID_BACKCOLOR:
    case DISPID_CENTERCOLOR:
    case DISPID_FORECOLOR:
        return S_FALSE;
    }

    return IPerPropertyBrowsingImpl<CBullsEye>::GetDisplayString(dispid, pBstr);
}

/**************/
/* SetStrings */
/**************/

// Copy array of strings to counted array of strings
HRESULT SetStrings(/*[in*/ ULONG cElems, /*[in]*/ const LPCOLESTR* rgpsz, /*[out]*/ CALPOLESTR* pStrings)
{
    ATLASSERT(0 != cElems);
    ATLASSERT(NULL != rgpsz);
    ATLASSERT(NULL != pStrings);
    pStrings->cElems = 0;
    // Allocate array of string pointers
    ULONG len = cElems * sizeof(LPOLESTR);
    pStrings->pElems = (LPOLESTR*) CoTaskMemAlloc(len);

    if (NULL == pStrings->pElems) {
        return E_OUTOFMEMORY;
    }

#ifdef _DEBUG
    // Initialize the array to zero
    ::ZeroMemory(pStrings->pElems, len);
#endif

    // Allocate each string
    for (ULONG i = 0; i < cElems; i++) {
        // Allocate memory for the string and terminating NUL character
        ULONG len = (ocslen(rgpsz[i]) + 1) * sizeof(OLECHAR);
        pStrings->pElems[i] = (LPOLESTR) CoTaskMemAlloc(len);

        if (NULL == pStrings->pElems[i]) return E_OUTOFMEMORY;

        ocscpy_s(pStrings->pElems[i], len, rgpsz[i]);
        pStrings->cElems++;
    }

    return S_OK;
}

/**************/
/* SetCookies */
/**************/

// Set counted array of DWORDs to provided DWORD cookies
HRESULT SetCookies(/*[in*/ ULONG cElems, /*[in*/ const DWORD* pdwCookies, /*[out]*/ CADWORD* pcaCookies)
{
    ATLASSERT(0 != cElems);
    ATLASSERT(NULL != pcaCookies);
    pcaCookies->cElems = 0;
    // Allocate array of cookies
    ULONG len = cElems * sizeof(DWORD);
    pcaCookies->pElems = (LPDWORD) CoTaskMemAlloc(len);

    if (NULL == pcaCookies->pElems) {
        return E_OUTOFMEMORY;
    }

    pcaCookies->cElems = cElems;
    ::CopyMemory(pcaCookies->pElems, pdwCookies, cElems * sizeof(DWORD));
    return S_OK;
}

/************************/
/* GetPredefinedStrings */
/************************/

#define DIM(a) (sizeof(a)/sizeof(a[0]))

static const LPCOLESTR    rszBeepStrings [] = { OLESTR("Yes, make noise"), OLESTR("No, be mute") };
static const DWORD        rdwBeepCookies [] = { 0, 1 };
static const VARIANT_BOOL rvbBeepValues  [] = { VARIANT_TRUE, VARIANT_FALSE };

static const UINT cBeepStrings = DIM(rszBeepStrings);
static const UINT cBeepCookies = DIM(rdwBeepCookies);
static const UINT cBeepValues  = DIM(rvbBeepValues);

static const LPCOLESTR    rszBackStyleStrings [] = { OLESTR("Opaque"), OLESTR("Transparent") };
static const DWORD        rdwBackStyleCookies [] = { 0, 1 };
static const long         rvbBackStyleValues  [] = { 1, 0 };

static const UINT cBackStyleStrings = DIM(rszBackStyleStrings);
static const UINT cBackStyleCookies = DIM(rdwBackStyleCookies);
static const UINT cBackStyleValues  = DIM(rvbBackStyleValues);

STDMETHODIMP CBullsEye::GetPredefinedStrings(/*[in]*/ DISPID dispid, /*[out]*/ CALPOLESTR *pcaStringsOut, /*[out]*/ CADWORD *pcaCookiesOut)
{
    ATLTRACE2(atlTraceControls, 2, _T("CBullsEye::GetPredefinedStrings\n"));

    if (NULL == pcaStringsOut || NULL == pcaCookiesOut) return E_POINTER;

    ATLASSERT(cBeepStrings == cBeepCookies);
    ATLASSERT(cBeepStrings == cBeepValues);
    ATLASSERT(cBackStyleStrings == cBackStyleCookies);
    ATLASSERT(cBackStyleStrings == cBackStyleValues);
    pcaStringsOut->cElems = 0;
    pcaStringsOut->pElems = NULL;
    pcaCookiesOut->cElems = 0;
    pcaCookiesOut->pElems = NULL;
    HRESULT hr = S_OK;

    switch (dispid) {
    case DISPID_BEEP:
        hr = SetStrings(cBeepValues, rszBeepStrings, pcaStringsOut);

        if (FAILED(hr)) return hr;

        return SetCookies(cBeepValues, rdwBeepCookies, pcaCookiesOut);

    case DISPID_BACKSTYLE:
        hr = SetStrings(cBackStyleValues, rszBackStyleStrings, pcaStringsOut);

        if (FAILED(hr)) return hr;

        return SetCookies(cBackStyleValues, rdwBackStyleCookies, pcaCookiesOut);
    }

    return IPerPropertyBrowsingImpl<CBullsEye>::GetPredefinedStrings(dispid, pcaStringsOut, pcaCookiesOut);
}

/**********************/
/* GetPredefinedValue */
/**********************/

STDMETHODIMP CBullsEye::GetPredefinedValue(DISPID dispid, DWORD dwCookie, VARIANT* pVarOut)
{
    if (NULL == pVarOut) return E_POINTER;

    ULONG i;

    switch (dispid) {
    case DISPID_BEEP:

        // Walk through cookie array looking for matching value
        for (i = 0; i < cBeepCookies; i++) {
            if (rdwBeepCookies[i] == dwCookie) {
                pVarOut->vt = VT_BOOL;
                pVarOut->boolVal = rvbBeepValues [i];
                return S_OK;
            }
        }

        return E_INVALIDARG;

    case DISPID_BACKSTYLE:

        // Walk through cookie array looking for matching value
        for (i = 0; i < cBackStyleCookies; i++) {
            if (rdwBackStyleCookies[i] == dwCookie) {
                pVarOut->vt = VT_I4;
                pVarOut->lVal = rvbBackStyleValues [i];
                return S_OK;
            }
        }

        return E_INVALIDARG;
    }

    return IPerPropertyBrowsingImpl<CBullsEye>::GetPredefinedValue(dispid, dwCookie, pVarOut);
}

STDMETHODIMP CBullsEye::MapPropertyToPage(DISPID dispid, CLSID *pClsid)
{
    HRESULT hr =
        IPerPropertyBrowsingImpl< CBullsEye >::MapPropertyToPage(dispid, pClsid);

    if (SUCCEEDED(hr) && *pClsid == CLSID_NULL) {
        hr = PERPROP_E_NOPAGEAVAILABLE;
    }

    return hr;
}

//
// IOleControl
//
STDMETHODIMP CBullsEye::GetControlInfo(CONTROLINFO *pci)
{
    if (!pci) return E_POINTER;

    pci->hAccel = 0;
    pci->cAccel = 0;
    pci->dwFlags = 0;
    return S_OK;
}

typedef enum tagKEYMODIFIERS {
    KEYMOD_SHIFT = 0x01,
    KEYMOD_CONTROL = 0x02,
    KEYMOD_ALT = 0x04
} KEYMODIFIERS;

STDMETHODIMP CBullsEye::TranslateAccellerator(MSG *pMsg)
{
    if (((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST)) &&
        ((pMsg->wParam == VK_TAB) || (pMsg->wParam == VK_RETURN))) {
        CComQIPtr< IOleControlSite > spCtrlSite(m_spClientSite);

        if (spCtrlSite) {
            DWORD km = 0;
            km |= GetKeyState(VK_SHIFT) < 0 ? KEYMOD_SHIFT : 0 ;
            km |= GetKeyState(VK_CONTROL) < 0 ? KEYMOD_CONTROL : 0 ;
            km |= GetKeyState(VK_MENU) < 0 ? KEYMOD_ALT : 0;
            return spCtrlSite->TranslateAccelerator(pMsg, km);
        }
    }

    return S_FALSE;
}

STDMETHODIMP CBullsEye::GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus)
{
    if (!pdwStatus) return E_POINTER;

    *pdwStatus =
        OLEMISC_SETCLIENTSITEFIRST |
        OLEMISC_ACTIVATEWHENVISIBLE |
        OLEMISC_INSIDEOUT |
        OLEMISC_CANTLINKINSIDE |
        OLEMISC_RECOMPOSEONRESIZE |
        OLEMISC_NOUIACTIVATE ;
    return S_OK;
}

LRESULT CBullsEye::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    //    if (!m_bEnabled) return 0;
    CPoint pt(LOWORD(lParam), HIWORD(lParam));

    // Point is relative to container's origin when windowless
    if (m_bWndLess)     // Make relative to control's origin
        pt.Offset(-m_rcPos.left, -m_rcPos.top);

    BOOL bUserMode = TRUE;
    HRESULT hRet = GetAmbientUserMode(bUserMode);
    // Perform hit test and ring hits in user mode or
    // when we can't determine mode

    if (FAILED(hRet) || bUserMode) {
        short ring = HitTest(pt);
        HitRing(ring);
        return 0;
    }

    // In design mode, start drag and drop
    // DoDragDrop(DROPEFFECT_COPY, NULL);
    return 0;
}

int CBullsEye::HitTest(const CPoint& pt)
{
    // Compute size of control in device units
    CSize size;
    AtlHiMetricToPixel(&m_sizeExtent, &size);
    // Calculate diameter of BullsEye in device units
    int diameter = min(size.cx, size.cy);
    // Compute the width of a ring
    short sRingCount;
    HRESULT hr = get_RingCount(&sRingCount);
    ATLASSERT(SUCCEEDED(hr));
    int ringWidth = diameter / (sRingCount * 2 - 1);
    // Determine delta x, delta y from center of window
    int x = pt.x - size.cx / 2;
    int y = pt.y - size.cy / 2;
    // Compute distance^2 to center
    int rSquared = x * x + y * y;

    // Hit test each ring from center out
    for (short i = 0; i < sRingCount; i++) {
        // Compute the radius of this ring
        int radiusSquared = i * ringWidth + ringWidth / 2;
        radiusSquared *= radiusSquared;

        // Did the click hit in this ring?
        if (rSquared < radiusSquared)
            return i;
    }

    return -1;
}

