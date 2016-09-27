// Ticker.h : Declaration of the CTicker

#ifndef __TICKER_H_
#define __TICKER_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "AtlTickerCtrlCP.h"

//#include <Dispex.h>

/////////////////////////////////////////////////////////////////////////////
// CTicker
class ATL_NO_VTABLE CTicker :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CStockPropImpl<CTicker, ITicker, &IID_ITicker, &LIBID_ATLTICKERCTRLLib>,
    public CComControl<CTicker>,
    public IPersistStreamInitImpl<CTicker>,
    public IOleControlImpl<CTicker>,
    public IOleObjectImpl<CTicker>,
    public IOleInPlaceActiveObjectImpl<CTicker>,
    public IViewObjectExImpl<CTicker>,
    public IOleInPlaceObjectWindowlessImpl<CTicker>,
    public IConnectionPointContainerImpl<CTicker>,
    public IPersistStorageImpl<CTicker>,
    public ISpecifyPropertyPagesImpl<CTicker>,
    public IQuickActivateImpl<CTicker>,
    public IDataObjectImpl<CTicker>,
    public IProvideClassInfo2Impl<&CLSID_Ticker, &DIID__ITickerEvents, &LIBID_ATLTICKERCTRLLib>,
    public IPropertyNotifySinkCP<CTicker>,
    public CComCoClass<CTicker, &CLSID_Ticker>,
    public CProxy_ITickerEvents< CTicker >,
    public IObjectSafety,
    public IPersistPropertyBagImpl<CTicker>
{
public:
    CTicker()
    {
        m_textStart = 0;
        m_textEnd = 0;
        m_step = 10;
        m_speed = 100;
        m_Timer = 0;
        m_bScrollingMode = FALSE;
        m_sText = "水清木华 575 +131.00, 广州超讯 493 +121.23, 凯信科技145.33 +80.04";
        m_dwSafety = 0;
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_TICKER)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CTicker)
    COM_INTERFACE_ENTRY(ITicker)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
    COM_INTERFACE_ENTRY2(IPersist, IPersistPropertyBag)
    COM_INTERFACE_ENTRY(IViewObjectEx)
    COM_INTERFACE_ENTRY(IViewObject2)
    COM_INTERFACE_ENTRY(IViewObject)
    COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceObject)
    COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
    COM_INTERFACE_ENTRY(IOleControl)
    COM_INTERFACE_ENTRY(IOleObject)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IObjectSafety)
    END_COM_MAP()

    BEGIN_PROP_MAP(CTicker)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    PROP_ENTRY("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage)
    PROP_ENTRY("BorderColor", DISPID_BORDERCOLOR, CLSID_StockColorPage)
    PROP_ENTRY("Font", DISPID_FONT, CLSID_StockFontPage)
    PROP_ENTRY("ForeColor", DISPID_FORECOLOR, CLSID_StockColorPage)
    PROP_ENTRY("Speed", 0x4, CLSID_TickerPP)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()

    BEGIN_CONNECTION_POINT_MAP(CTicker)
    CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
    CONNECTION_POINT_ENTRY(DIID__ITickerEvents)
    END_CONNECTION_POINT_MAP()

    BEGIN_MSG_MAP(CTicker)
    CHAIN_MSG_MAP(CComControl<CTicker>)
    DEFAULT_REFLECTION_HANDLER()
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

//IOleObjectImpl
    STDMETHOD(SetClientSite)(LPOLECLIENTSITE pSite);

//IOleControlImpl
    STDMETHOD(OnAmbientPropertyChange)(DISPID dispid);

// ITicker
public:
    STDMETHOD(get_Speed)(/*[out, retval]*/ long *pVal);
    STDMETHOD(put_Speed)(/*[in]*/ long newVal);
    void Scroll();
    STDMETHOD(StartScroll)();
    STDMETHOD(StopScroll)();
    STDMETHOD(GetQuotes)(/*[out,retval]*/ VARIANT_BOOL *pbSuccess);
    HRESULT OnDraw(ATL_DRAWINFO& di);

    OLE_COLOR m_clrBackColor;
    OLE_COLOR m_clrBorderColor;
    CComPtr<IFontDisp> m_pFont;
    OLE_COLOR m_clrForeColor;
private:
    //this is added to allow setting and
    //saving color as string
    CComBSTR m_BackColor;
    //current safety setting
    DWORD m_dwSafety;

    BOOL IsInUserMode();
    void FirstPosition(ATL_DRAWINFO& di);
    int m_textStart;    //the starting position of the control text
    int m_textEnd; //the ending position of the control text
    int m_step; //the step used to scroll the control text
    long m_speed; //the number of milliseconds between ticker scrolling
    UINT m_Timer; //Identifier of a timer objec
    bool m_bScrollingMode; //Whether we should scroll or not
    CComBSTR m_sText;//The stock quote text, this is what gets displayed
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        //x coordinate of mouse cursor
        WORD x = LOWORD(lParam);
        //y coordinate of mouse cursor
        WORD y = HIWORD(lParam);
        // vertical position of cursor
        Fire_Click(x, y);
        return 0;
    }

public:
    //IPersistPropertyBag
    STDMETHOD(Load)(IPropertyBag *pBag, IErrorLog *pLog)
    {
        //create variant to hold propery values
        CComVariant v;
        //set variant type to long
        v.vt = VT_I4;
        v = 0L;
        //get the speed
        HRESULT hr = pBag->Read(OLESTR("Speed"), &v, pLog);

        if (SUCCEEDED(hr))put_Speed(v.lVal);

        //set variant type to BSTR
        v.vt = VT_BSTR;
        //get backcolor
        hr = pBag->Read(OLESTR("BackColor"), &v, pLog);

        if (SUCCEEDED(hr))m_clrBackColor = wcstol(v.bstrVal, NULL, 16);

        return S_OK;
    }
    STDMETHOD(Save)(IPropertyBag *pBag, BOOL fClearDirty, BOOL fSaveAllProperties)
    {
        //convert backcolor to string
        wchar_t buffer[9] = {0};
        swprintf(buffer, L"%08x", m_clrBackColor);
        CComVariant v = buffer;
        //write out backcolor
        HRESULT hr = pBag->Write(OLESTR("BackColor"), &v);

        if FAILED((hr)) return hr;

        //write out speed
        v = m_speed;
        hr = pBag->Write(OLESTR("Speed"), &v);

        if (FAILED(hr)) return hr;

        return S_OK;
    }
    //IObjectSafety
    STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD __RPC_FAR *pdwSupportedOptions,
                                         DWORD __RPC_FAR *pdwEnabledOptions)
    {
        if (riid == IID_IDispatch) {
            //Client wants to know
            // if object is safe for scripting. Only indicate safe for
            // scripting when the interface is safe.
            *pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER;
            *pdwEnabledOptions = m_dwSafety & INTERFACESAFE_FOR_UNTRUSTED_CALLER;
            return S_OK;
        } else if ((riid == IID_IPersistStreamInit) ||
                   (riid == IID_IPersistStorage)) {
            // Client wants to know if object is
            // safe for initializing. Only indicate safe for initializing
            // when the interface is safe.
            *pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA;
            *pdwEnabledOptions = m_dwSafety &
                                 INTERFACESAFE_FOR_UNTRUSTED_DATA;
            return S_OK;
        } else {
            // We are saying that no other interfaces in this control are
            // safe for initializing or scripting.
            *pdwSupportedOptions = 0;
            *pdwEnabledOptions = 0;
            return E_FAIL;
        }
    }
    STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask,
                                         DWORD dwEnabledOptions)
    {
        // Store our current safety level to return in
        // GetInterfaceSafetyOptions
        m_dwSafety |= dwEnabledOptions & dwOptionSetMask;

        if ((riid == IID_IDispatch) &&
            (m_dwSafety & INTERFACESAFE_FOR_UNTRUSTED_CALLER)) {
            // Client wants us to disable any functionality that would
            // make the control unsafe for scripting. The same applies to
            // any other IDispatch or Dual interfaces your control may
            // support. Because our control is safe for scripting by
            // default we just return S_OK.
            return S_OK;
        } else if (((riid == IID_IPersistStreamInit) ||
                    (riid == IID_IPersistStorage)) &&
                   (m_dwSafety & INTERFACESAFE_FOR_UNTRUSTED_DATA)) {
            // Client wants us to make the control safe for initializing
            // from persistent data. For these interfaces, this control
            // is safe so we return S_OK.
            return E_FAIL;
        } else {
            // This control doesn't allow Initialization or Scripting
            // from any other interfaces so return E_FAIL.
            return E_FAIL;
        }
    }
};
#endif //__TICKER_H_
