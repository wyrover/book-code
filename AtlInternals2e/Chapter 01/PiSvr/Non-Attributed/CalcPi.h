// CalcPi.h : Declaration of the CCalcPi
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "PiSvr.h"
#include "_ICalcPiEvents_CP.h"
#include "Advert.h"

// CCalcPi
class ATL_NO_VTABLE CCalcPi :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<ICalcPi, &IID_ICalcPi, &LIBID_PiSvrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IPersistStreamInitImpl<CCalcPi>,
    public IOleControlImpl<CCalcPi>,
    public IOleObjectImpl<CCalcPi>,
    public IOleInPlaceActiveObjectImpl<CCalcPi>,
    public IViewObjectExImpl<CCalcPi>,
    public IOleInPlaceObjectWindowlessImpl<CCalcPi>,
    public ISupportErrorInfo,
    public IConnectionPointContainerImpl<CCalcPi>,
    public CProxy_ICalcPiEvents<CCalcPi>,
    public IPersistStorageImpl<CCalcPi>,
    public ISpecifyPropertyPagesImpl<CCalcPi>,
    public IQuickActivateImpl<CCalcPi>,
    public IDataObjectImpl<CCalcPi>,
    public IProvideClassInfo2Impl<&CLSID_CalcPi, &__uuidof(_ICalcPiEvents), &LIBID_PiSvrLib>,
    public CComCoClass<CCalcPi, &CLSID_CalcPi>,
    public CComControl<CCalcPi>,
    public IPersistPropertyBagImpl<CCalcPi>,
    public IAdvertiseMyself
{
public:

    CCalcPi()
    {
    }

    DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
                           OLEMISC_CANTLINKINSIDE |
                           OLEMISC_INSIDEOUT |
                           OLEMISC_ACTIVATEWHENVISIBLE |
                           OLEMISC_SETCLIENTSITEFIRST
                          )

    DECLARE_REGISTRY_RESOURCEID(IDR_CALCPI)

    BEGIN_COM_MAP(CCalcPi)
    COM_INTERFACE_ENTRY(ICalcPi)
    COM_INTERFACE_ENTRY(IDispatch)
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
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IAdvertiseMyself)
    END_COM_MAP()

    BEGIN_PROP_MAP(CCalcPi)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    PROP_ENTRY("Digits", 1, CLSID_NULL)
    END_PROP_MAP()

    BEGIN_CONNECTION_POINT_MAP(CCalcPi)
    CONNECTION_POINT_ENTRY(__uuidof(_ICalcPiEvents))
    END_CONNECTION_POINT_MAP()

    BEGIN_MSG_MAP(CCalcPi)
    CHAIN_MSG_MAP(CComControl<CCalcPi>)
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        static const IID* arr[] = {
            &IID_ICalcPi,
        };

        for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
            if (InlineIsEqualGUID(*arr[i], riid))
                return S_OK;
        }

        return S_FALSE;
    }

// IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ICalcPi
public:
    HRESULT OnDraw(ATL_DRAWINFO& di)
    {
        CComBSTR  bstrPi;

        if (SUCCEEDED(this->CalcPi(&bstrPi))) {
            USES_CONVERSION;
            DrawText(di.hdcDraw, OLE2CT(bstrPi), -1, (RECT*)di.prcBounds,
                     DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        return S_OK;
    }

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        m_nDigits = 0;
        return S_OK;
    }

    void FinalRelease()
    {
    }
    STDMETHOD(get_Digits)(LONG* pVal);
    STDMETHOD(put_Digits)(LONG newVal);
    STDMETHOD(CalcPi)(BSTR* pbstrPi);

private:
    long m_nDigits;

public:
    BOOL m_bRequiresSave;

// IAdvertiseMyself
    STDMETHOD(ShowAd)(BSTR bstrClient)
    {
        CAdvert ad;
        ad.m_bstrClient = bstrClient;
        ad.DoModal();
        return S_OK;
    }

};

OBJECT_ENTRY_AUTO(__uuidof(CalcPi), CCalcPi)
