// Ticker.h : Declaration of the CTicker

#ifndef __TICKER_H_
#define __TICKER_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "AtlTickerCtrlCP.h"


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
    public CProxy_ITickerEvents< CTicker >
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
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_TICKER)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CTicker)
    COM_INTERFACE_ENTRY(ITicker)
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
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
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
};

#endif //__TICKER_H_
