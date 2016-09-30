// CoPenguin.h : Declaration of the CPenguin
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "Birds.h"

// CPenguin
class ATL_NO_VTABLE CPenguin :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IPenguin, &IID_IPenguin, &LIBID_BirdsLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    public IPersistStreamInitImpl<CPenguin>,
    public IOleControlImpl<CPenguin>,
    public IOleObjectImpl<CPenguin>,
    public IOleInPlaceActiveObjectImpl<CPenguin>,
    public IViewObjectExImpl<CPenguin>,
    public IOleInPlaceObjectWindowlessImpl<CPenguin>,
    public ISupportErrorInfo,
    public IPersistStorageImpl<CPenguin>,
    public ISpecifyPropertyPagesImpl<CPenguin>,
    public IQuickActivateImpl<CPenguin>,
    public IDataObjectImpl<CPenguin>,
    public IProvideClassInfo2Impl<&CLSID_Penguin, NULL, &LIBID_BirdsLib>,
    public CComCoClass<CPenguin, &CLSID_Penguin>,
    public CComControl<CPenguin>
{
public:

    CPenguin()
    {
    }

    DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
                           OLEMISC_CANTLINKINSIDE |
                           OLEMISC_INSIDEOUT |
                           OLEMISC_ACTIVATEWHENVISIBLE |
                           OLEMISC_SETCLIENTSITEFIRST
                          )

    DECLARE_REGISTRY_RESOURCEID(IDR_PENGUIN)

    BEGIN_COM_MAP(CPenguin)
    COM_INTERFACE_ENTRY(IPenguin)
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
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    END_COM_MAP()

    BEGIN_PROP_MAP(CPenguin)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()


    BEGIN_MSG_MAP(CPenguin)
    CHAIN_MSG_MAP(CComControl<CPenguin>)
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
            &IID_IPenguin,
        };

        for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
            if (InlineIsEqualGUID(*arr[i], riid))
                return S_OK;
        }

        return S_FALSE;
    }

// IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPenguin
public:
    HRESULT OnDraw(ATL_DRAWINFO& di)
    {
        RECT& rc = *(RECT*)di.prcBounds;
        // Set Clip region to the rectangle specified by di.prcBounds
        HRGN hRgnOld = NULL;

        if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
            hRgnOld = NULL;

        bool bSelectOldRgn = false;
        HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

        if (hRgnNew != NULL) {
            bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
        }

        Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
        SetTextAlign(di.hdcDraw, TA_CENTER | TA_BASELINE);
        LPCTSTR pszText = _T("ATL 7.0 : Penguin");
        TextOut(di.hdcDraw,
                (rc.left + rc.right) / 2,
                (rc.top + rc.bottom) / 2,
                pszText,
                lstrlen(pszText));

        if (bSelectOldRgn)
            SelectClipRgn(di.hdcDraw, hRgnOld);

        return S_OK;
    }


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }
    STDMETHOD(get_Wingspan)(LONG* pVal);
    STDMETHOD(put_Wingspan)(LONG newVal);
    STDMETHOD(get_Name)(BSTR* pVal);
    STDMETHOD(put_Name)(BSTR newVal);
    STDMETHOD(Fly)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(Penguin), CPenguin)
