// DOMCtrl.h : Declaration of the CDOMCtrl

#ifndef __DOMCTRL_H_
#define __DOMCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>


/////////////////////////////////////////////////////////////////////////////
// CDOMCtrl
class ATL_NO_VTABLE CDOMCtrl :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IDOMCtrl, &IID_IDOMCtrl, &LIBID_DOMSAMPLib>,
    public CComCompositeControl<CDOMCtrl>,
    public IPersistStreamInitImpl<CDOMCtrl>,
    public IOleControlImpl<CDOMCtrl>,
    public IOleObjectImpl<CDOMCtrl>,
    public IOleInPlaceActiveObjectImpl<CDOMCtrl>,
    public IViewObjectExImpl<CDOMCtrl>,
    public IOleInPlaceObjectWindowlessImpl<CDOMCtrl>,
    public CComCoClass<CDOMCtrl, &CLSID_DOMCtrl>
{
public:
    CDOMCtrl()
    {
        m_bWindowOnly = TRUE;
        CalcExtent(m_sizeExtent);
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_DOMCTRL)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CDOMCtrl)
    COM_INTERFACE_ENTRY(IDOMCtrl)
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
    END_COM_MAP()

    BEGIN_PROP_MAP(CDOMCtrl)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()

    BEGIN_MSG_MAP(CDOMCtrl)
    CHAIN_MSG_MAP(CComCompositeControl<CDOMCtrl>)
    COMMAND_HANDLER(IDC_BTNCOLOR, BN_CLICKED, OnClickedBtncolor)
    COMMAND_HANDLER(IDC_BTNNEWWND, BN_CLICKED, OnClickedBtnnewwnd)
    COMMAND_HANDLER(IDC_BTNTITLE, BN_CLICKED, OnClickedBtntitle)
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    BEGIN_SINK_MAP(CDOMCtrl)
    //Make sure the Event Handlers have __stdcall calling convention
    END_SINK_MAP()

    STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
    {
        if (dispid == DISPID_AMBIENT_BACKCOLOR) {
            SetBackgroundColorFromAmbient();
            FireViewChange();
        }

        return IOleControlImpl<CDOMCtrl>::OnAmbientPropertyChange(dispid);
    }



// IViewObjectEx
    DECLARE_VIEW_STATUS(0)

// IDOMCtrl
public:

    enum { IDD = IDD_DOMCTRL };
    LRESULT OnClickedBtncolor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        TCHAR szColor[32];
        CComPtr<IOleContainer>spIE;
        //get the container interface pointer
        m_spClientSite->GetContainer(&spIE);
        //QI the container for the document interface
        CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2>spDoc(spIE);

        if (spDoc) {
            //get the user-entered color
            GetDlgItemText(IDC_EDTCOLOR, szColor, sizeof(szColor));
            //set the background color
            spDoc->put_bgColor(CComVariant(szColor));
        }

        return 0;
    }
    LRESULT OnClickedBtnnewwnd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        TCHAR szURL[128];
        CComPtr<IOleContainer>spIE;
        //get the container interface pointer
        m_spClientSite->GetContainer(&spIE);
        CComPtr<IHTMLWindow2>spWnd;
        //QI the container for the document interface
        CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2>spDoc(spIE);

        if (spDoc) {
            //get the document's window
            spDoc->get_parentWindow(&spWnd);

            if (spWnd) {
                CComPtr<IHTMLWindow2>spNewWnd;
                //get user-entered URL for new window
                GetDlgItemText(IDC_EDTURL, szURL, sizeof(szURL));
                //use current window to open new window
                spWnd->open(CComBSTR(szURL), CComBSTR(L"NewWindow"),
                            CComBSTR(L"toolbar:no,status:no,menubar:no,directories:no,width=350,height=200"),
                            VARIANT_FALSE, &spNewWnd);
            }
        }

        return 0;
    }
    LRESULT OnClickedBtntitle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        TCHAR szTitle[32];
        CComPtr<IOleContainer>spIE;
        //get the container interface pointer
        m_spClientSite->GetContainer(&spIE);
        //QI the container for the document interface
        CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2>spDoc(spIE);

        if (spDoc) {
            //get user-entered title
            GetDlgItemText(IDC_EDTTITLE, szTitle, sizeof(szTitle));
            //set the title
            spDoc->put_title(CComBSTR(szTitle));
        }

        return 0;
    }
};

#endif //__DOMCTRL_H_
