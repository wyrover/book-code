// PostItCtl.h : Declaration of the CPostItCtl

#ifndef __POSTITCTL_H_
#define __POSTITCTL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "PostItCP.h"


/////////////////////////////////////////////////////////////////////////////
// CPostItCtl
class ATL_NO_VTABLE CPostItCtl :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CStockPropImpl<CPostItCtl, IPostItCtl, &IID_IPostItCtl, &LIBID_POSTITLib>,
    public CComControl<CPostItCtl>,
    public IPersistStreamInitImpl<CPostItCtl>,
    public IOleControlImpl<CPostItCtl>,
    public IOleObjectImpl<CPostItCtl>,
    public IOleInPlaceActiveObjectImpl<CPostItCtl>,
    public IViewObjectExImpl<CPostItCtl>,
    public IOleInPlaceObjectWindowlessImpl<CPostItCtl>,
    public IConnectionPointContainerImpl<CPostItCtl>,
    public IPersistStorageImpl<CPostItCtl>,
    public ISpecifyPropertyPagesImpl<CPostItCtl>,
    public IQuickActivateImpl<CPostItCtl>,
    public IDataObjectImpl<CPostItCtl>,
    public IProvideClassInfo2Impl<&CLSID_PostItCtl, &DIID__IPostItCtlEvents, &LIBID_POSTITLib>,
    public IPropertyNotifySinkCP<CPostItCtl>,
    public CComCoClass<CPostItCtl, &CLSID_PostItCtl>,
    public CProxy_IPostItCtlEvents< CPostItCtl >
{
public:
    CPostItCtl()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_POSTITCTL)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CPostItCtl)
    COM_INTERFACE_ENTRY(IPostItCtl)
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

    BEGIN_PROP_MAP(CPostItCtl)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    PROP_ENTRY("Appearance", DISPID_APPEARANCE, CLSID_NULL)
    PROP_ENTRY("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage)
    PROP_ENTRY("BorderStyle", DISPID_BORDERSTYLE, CLSID_NULL)
    PROP_ENTRY("Font", DISPID_FONT, CLSID_StockFontPage)
    PROP_ENTRY("ForeColor", DISPID_FORECOLOR, CLSID_StockColorPage)
    PROP_ENTRY("Text", DISPID_TEXT, CLSID_NULL)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()

    BEGIN_CONNECTION_POINT_MAP(CPostItCtl)
    CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
    CONNECTION_POINT_ENTRY(DIID__IPostItCtlEvents)
    END_CONNECTION_POINT_MAP()

    BEGIN_MSG_MAP(CPostItCtl)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    CHAIN_MSG_MAP(CComControl<CPostItCtl>)
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPostItCtl
public:

    HRESULT OnDraw(ATL_DRAWINFO& di)
    {
        USES_CONVERSION;
        COLORREF  colBack, colFore;
        HBRUSH    hOldBrush = 0;
        HBRUSH    hBackBrush = 0;
        HDC       hdc = di.hdcDraw;
        RECT& rc = *(RECT*)di.prcBounds;
        // Convert the OLE_COLOR types into COLORREFs
        OleTranslateColor(m_clrBackColor, NULL, &colBack);
        OleTranslateColor(m_clrForeColor, NULL, &colFore);
        // Create a brush using the background color
        // and select it into the DC
        hBackBrush = (HBRUSH) CreateSolidBrush(colBack);
        hOldBrush = (HBRUSH) SelectObject(hdc, hBackBrush);
        // Fill the background with our new brush
        FillRect(hdc, &rc, hBackBrush);

        // If the BorderStyle is 1, draw
        // a border around the control
        if (m_nBorderStyle) {
            HPEN hPen = (HPEN) CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HPEN hOldPen = (HPEN) SelectObject(hdc, hPen);
            Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

            if (hOldPen)
                SelectObject(hdc, hOldPen);

            DeleteObject(hPen);
        }

        // If the appearance is 3-D draw an edge
        if (m_nAppearance) {
            DrawEdge(hdc, &rc, EDGE_SUNKEN, BF_RECT);
            // Adjust our rectangle
            rc.left += 2;
            rc.top += 2;
            rc.bottom -= 2;
            rc.right -= 2;
        }

        // Get the user-selected font and select
        // it into our device context.
        CComQIPtr<IFont, &IID_IFont> pFont(m_pFont);
        HFONT hOldFont = 0;
        HFONT hFont = 0;

        if (pFont) {
            pFont->get_hFont(&hFont);
            pFont->AddRefHfont(hFont);
            hOldFont = (HFONT) SelectObject(hdc, hFont);
        }

        // Check to see if we're in design mode or
        // run-time mode. If in design mode, get the
        // ambient display name and draw it within
        // the control.
        BOOL bUserMode = FALSE;
        GetAmbientUserMode(bUserMode);

        if (bUserMode == FALSE) {
            BSTR bstr;

            if (SUCCEEDED(GetAmbientDisplayName(bstr))) {
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, colFore);
                DrawText(hdc,
                         W2A(bstr),
                         -1,
                         &rc,
                         DT_TOP | DT_SINGLELINE);
            }
        }

        // Draw the user-specified text
        if (m_bstrText.Length()) {
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, colFore);
            DrawText(hdc,
                     W2A(m_bstrText),
                     -1,
                     &rc,
                     DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        }

        // Release the IFont object. We don't
        // delete the font because the OLE font
        // object manages this
        if (pFont)
            pFont->ReleaseHfont(hFont);

        // Restore the old font
        if (hOldFont)
            SelectObject(hdc, hOldFont);

        // Restore the old brush and delete
        // the one we created earlier
        if (hOldBrush) {
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBackBrush);
        }

        return S_OK;
    }
    short m_nAppearance;
    OLE_COLOR m_clrBackColor;
    LONG m_nBorderStyle;
    CComPtr<IFontDisp> m_pFont;
    OLE_COLOR m_clrForeColor;
    CComBSTR m_bstrText;

private:
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam,
                          LPARAM lParam, BOOL& bHandled);

};

#endif //__POSTITCTL_H_
