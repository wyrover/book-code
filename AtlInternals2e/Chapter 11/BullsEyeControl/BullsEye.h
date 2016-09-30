// BullsEye.h : Declaration of the CBullsEye
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "BullsEyeControl.h"
#include "_IBullsEyeEvents_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
    #error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

const int PROPCAT_Scoring = 1;

// CBullsEye
class ATL_NO_VTABLE CBullsEye :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CStockPropImpl<CBullsEye, IBullsEye>,
    public ICategorizeProperties,
    public IPerPropertyBrowsingImpl<CBullsEye>,
    public IPersistStreamInitImpl<CBullsEye>,
    public IPersistStorageImpl<CBullsEye>,
    public IPersistPropertyBagImpl<CBullsEye>,
    public IOleControlImpl<CBullsEye>,
    public IOleObjectImpl<CBullsEye>,
    public IOleInPlaceActiveObjectImpl<CBullsEye>,
    public IViewObjectExImpl<CBullsEye>,
    public IOleInPlaceObjectWindowlessImpl<CBullsEye>,
    public ISupportErrorInfo,
    public IConnectionPointContainerImpl<CBullsEye>,
    public IPropertyNotifySinkCP<CBullsEye>,
    public CProxy_IBullsEyeEvents<CBullsEye>,
    public ISpecifyPropertyPagesImpl<CBullsEye>,
    public IQuickActivateImpl<CBullsEye>,
#ifndef _WIN32_WCE
    public IDataObjectImpl<CBullsEye>,
#endif
    public IProvideClassInfo2Impl<&CLSID_BullsEye, &__uuidof(_IBullsEyeEvents), &LIBID_BullsEyeControlLib>,
    public IObjectSafetyImpl < CBullsEye, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA >,
    public CComCoClass<CBullsEye, &CLSID_BullsEye>,
    public CComControl<CBullsEye>
{
public:
    // Construction/Destruction
    CBullsEye();
    ~CBullsEye();

    HRESULT FinalConstruct();

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
                           OLEMISC_CANTLINKINSIDE |
                           OLEMISC_INSIDEOUT |
                           OLEMISC_ACTIVATEWHENVISIBLE |
                           OLEMISC_SETCLIENTSITEFIRST
                          )

    DECLARE_REGISTRY_RESOURCEID(IDR_BULLSEYE)


    BEGIN_COM_MAP(CBullsEye)
    COM_INTERFACE_ENTRY(IBullsEye)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ICategorizeProperties)
    COM_INTERFACE_ENTRY(IPerPropertyBrowsing)
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
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
#ifndef _WIN32_WCE
    COM_INTERFACE_ENTRY(IDataObject)
#endif
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
    END_COM_MAP()

    BEGIN_PROP_MAP(CBullsEye)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
#ifndef _WIN32_WCE
    PROP_ENTRY("AlternateColor", DISPID_ALTERNATECOLOR, CLSID_StockColorPage)
    PROP_ENTRY("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage)
    PROP_ENTRY("CenterColor", DISPID_CENTERCOLOR, CLSID_StockColorPage)
    PROP_ENTRY("ForeColor", DISPID_FORECOLOR, CLSID_StockColorPage)
#endif
    PROP_ENTRY("BackStyle", DISPID_BACKSTYLE, CLSID_BullsEyePropPage)
    PROP_ENTRY("Beep", DISPID_BEEP, CLSID_BullsEyePropPage)
    PROP_ENTRY("Enabled", DISPID_ENABLED, CLSID_BullsEyePropPage)
    PROP_ENTRY("RingCount", DISPID_RINGCOUNT, CLSID_BullsEyePropPage)

    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()

    BEGIN_CONNECTION_POINT_MAP(CBullsEye)
    CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
    CONNECTION_POINT_ENTRY(__uuidof(_IBullsEyeEvents))
    END_CONNECTION_POINT_MAP()

    BEGIN_CATEGORY_MAP(CBullsEye)
    IMPLEMENTED_CATEGORY(CATID_ATLINTERNALS_SAMPLES)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
    END_CATEGORY_MAP()

    BEGIN_MSG_MAP(CBullsEye)
    CHAIN_MSG_MAP(CComControl<CBullsEye>)
    DEFAULT_REFLECTION_HANDLER()
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    END_MSG_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
    STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IBullsEye
public:
    // CComControlBase
    HRESULT IPersistStreamInit_Load(LPSTREAM pStm, ATL_PROPMAP_ENTRY* pMap);
    HRESULT IPersistStreamInit_Save(LPSTREAM pStm, BOOL fClearDirty, ATL_PROPMAP_ENTRY* pMap);
    HRESULT IPersistPropertyBag_Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, ATL_PROPMAP_ENTRY* pMap);
    HRESULT IPersistPropertyBag_Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties, ATL_PROPMAP_ENTRY* pMap);
    STDMETHODIMP InitNew();

    HRESULT IQuickActivate_QuickActivate(QACONTAINER *pQACont, QACONTROL *pQACtrl);
    HRESULT IOleObject_SetClientSite(IOleClientSite *pClientSite);

    // IOleControl
    STDMETHODIMP GetControlInfo(CONTROLINFO *pci);
    STDMETHODIMP GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus);

    // IOleInPlaceActiveObject
    STDMETHODIMP TranslateAccellerator(MSG *pMsg);

    // ICategorizeProperties
    STDMETHODIMP MapPropertyToCategory(DISPID dispid, PROPCAT *ppropcat);
    STDMETHODIMP GetCategoryName(PROPCAT propcat, LCID lcid, BSTR *pbstrName);

    // IPerPropertyBrowsing
    STDMETHODIMP GetDisplayString(DISPID dispid, BSTR *pBstr);
    STDMETHODIMP GetPredefinedStrings(DISPID dispid, CALPOLESTR *pcaStringsOut, CADWORD *pcaCookiesOut);
    STDMETHODIMP GetPredefinedValue(DISPID dispid, DWORD dwCookie, VARIANT *pVarOut);
    STDMETHODIMP MapPropertyToPage(DISPID dispid, CLSID *pClsid);

    // Drawing methods
    HRESULT OnDraw(ATL_DRAWINFO& di);
    void DrawBullsEye(ATL_DRAWINFO &di);

    // Stock methods
    STDMETHODIMP AboutBox();
    STDMETHODIMP DoClick();
    STDMETHODIMP Refresh();

    // Stock property change notifications
    void OnForeColorChanged();
    void OnBackColorChanged();
    void OnBackStyleChanged();

    // Custom Properties
    STDMETHODIMP get_Application(IDispatch** pVal);

    STDMETHODIMP get_AlternateColor(OLE_COLOR* pVal);
    STDMETHODIMP put_AlternateColor(OLE_COLOR newVal);

    STDMETHODIMP get_Beep(VARIANT_BOOL* pVal);
    STDMETHODIMP put_Beep(VARIANT_BOOL newVal);

    STDMETHODIMP get_CenterColor(OLE_COLOR* pVal);
    STDMETHODIMP put_CenterColor(OLE_COLOR newVal);

    STDMETHODIMP get_Parent(IDispatch** pVal);

    STDMETHODIMP get_RingCount(short* pVal);
    STDMETHODIMP put_RingCount(short newVal);

    STDMETHODIMP get_RingValue(short ringNum, long* pVal);
    STDMETHODIMP put_RingValue(short ringNum, long newVal);


    // Implementation
private:

    HRESULT ValidateRingNumberInRange(short ringNum);

    HRESULT GetApplicationFromServiceProvider(IDispatch **ppDisp);
    HRESULT GetPropertyFromDocument(LPOLESTR pszProperty, IDispatch **ppDisp);

    void HitRing(short ringNum);
    int HitTest(const CPoint &pt);

    // Windows message handlers
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


    // Member data - has to be public so ATL implementations can access
public:
    OLE_COLOR                   m_clrBackColor;     // BackColor stock property
    OLE_COLOR                   m_clrForeColor;     // ForeColor stock property
    long                        m_nBorderStyle;     // BorderStyle stock property
    long                        m_nBorderWidth;     // BorderWidth stock property
    long                        m_nBackStyle;       // BackStyle stock property
    BOOL                        m_bEnabled;         // Enabled stock property

    OLE_COLOR                   m_clrCenterColor ;      // Center (even rings) color
    OLE_COLOR                   m_clrAlternateColor ;   // Alternate (odd rings) color
    VARIANT_BOOL                m_beep ;                // Beep on ring hit?
    CSimpleArray<long>          m_ringValues ;          // Point values for rings

    short                       m_nAppearance;      // Ambient Appearance property

    bool                        m_bAmbientsFetched; // Have we retrieved the ambients yet?

    HPEN                        m_borderPen ;       // Pen for border of rings
    HBRUSH                      m_backBrush ;       // Brush for background around bullseye
    HBRUSH                      m_alternateBrush ;  // Brush for alternate ring color
    HBRUSH                      m_centerBrush ;     // Brush for center ring color

};

OBJECT_ENTRY_AUTO(__uuidof(BullsEye), CBullsEye)
