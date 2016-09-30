// BullsEyePropPage.h : Declaration of the CBullsEyePropPage

#ifndef __BULLSEYEPROPPAGE_H_
#define __BULLSEYEPROPPAGE_H_

#include "resource.h"       // main symbols
#include <commctrl.h>

EXTERN_C const CLSID CLSID_BullsEyePropPage;

/////////////////////////////////////////////////////////////////////////////
// CBullsEyePropPage
class ATL_NO_VTABLE CBullsEyePropPage :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CBullsEyePropPage, &CLSID_BullsEyePropPage>,
    public IPropertyPage2Impl<CBullsEyePropPage>,
    public CDialogImpl<CBullsEyePropPage>,
    public IPropertyNotifySink
{
public:
    CBullsEyePropPage()
    {
        m_dwTitleID     = IDS_TITLEBullsEyePropPage;
        m_dwHelpFileID  = IDS_HELPFILEBullsEyePropPage;
        m_dwDocStringID = IDS_DOCSTRINGBullsEyePropPage;
        m_flags         = 0;    // No changes yet
        m_pCookies      = NULL;
        m_EditProperty  = DISPID_UNKNOWN;
    }

    ~CBullsEyePropPage()
    {
    }

    enum {IDD = IDD_BULLSEYEPROPPAGE};

    DECLARE_REGISTRY_RESOURCEID(IDR_BULLSEYEPROPPAGE)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CBullsEyePropPage)
    COM_INTERFACE_ENTRY(IPropertyPage)
    COM_INTERFACE_ENTRY_IMPL(IPropertyPage2)
    COM_INTERFACE_ENTRY(IPropertyNotifySink)
    END_COM_MAP()

    BEGIN_MSG_MAP(CBullsEyePropPage)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    COMMAND_HANDLER(IDC_BEEP, BN_CLICKED, OnClickedBeep)
    COMMAND_HANDLER(IDC_RINGCOUNT, EN_CHANGE, OnChangeRingCount)
    COMMAND_HANDLER(IDC_ENABLED, BN_CLICKED, OnClickedEnabled)
    COMMAND_HANDLER(IDC_TRANSPARENT, BN_CLICKED, OnClickedTransparent)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    CHAIN_MSG_MAP(IPropertyPageImpl<CBullsEyePropPage>)
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IPropertyPage2
    STDMETHODIMP Activate(HWND hWndParent, LPCRECT pRect, BOOL /* bModal */);
    STDMETHODIMP Apply(void);
    STDMETHODIMP EditProperty(DISPID dispID);
    STDMETHODIMP SetObjects(ULONG nObjects, IUnknown** ppUnk);

// IPropertyNotifySink
    STDMETHODIMP OnChanged(DISPID dispid);
    STDMETHODIMP OnRequestEdit(DISPID dispid);

// Dialog box messge handlers
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClickedBeep(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedEnabled(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnChangeRingCount(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedTransparent(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
    DISPID m_EditProperty;
    enum {BEEP_CHANGED = 1, RINGCOUNT_CHANGED = 2, ENABLED_CHANGED = 4, TRANSPARENT_CHANGED = 8};

    short                       m_flags;            // Change flags
    VARIANT_BOOL                m_beepOrig;         // Beep on ring hit?
    VARIANT_BOOL                m_beepNew;          // Beep on ring hit?
    VARIANT_BOOL                m_enabledOrig;      // Control enabled?
    VARIANT_BOOL                m_enabledNew;       // Control enabled?
    long                        m_transparentOrig;  // BackStyle property
    long                        m_transparentNew;   // BackStyle property
    short                       m_ringCountOrig;    // Number of rings
    short                       m_ringCountNew;     // Number of rings

    LPDWORD m_pCookies;    // Array of connection tokens used by
    // IConnectionPoint::Advise/Unadvise.

protected:
    void InitializeControlsFromObject(DISPID dispid);
    void SetPropertiesFromControls(DISPID dispid);
    void CleanupObjectArray();
};

OBJECT_ENTRY_AUTO(__uuidof(BullsEyePropPage), CBullsEyePropPage)

#endif //__BULLSEYEPROPPAGE_H_