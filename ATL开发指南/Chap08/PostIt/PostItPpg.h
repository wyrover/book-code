// PostItPpg.h : Declaration of the CPostItPpg

#ifndef __POSTITPPG_H_
#define __POSTITPPG_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_PostItPpg;

/////////////////////////////////////////////////////////////////////////////
// CPostItPpg
class ATL_NO_VTABLE CPostItPpg :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CPostItPpg, &CLSID_PostItPpg>,
    public IPropertyPageImpl<CPostItPpg>,
    public CDialogImpl<CPostItPpg>
{
public:
    CPostItPpg()
    {
        m_dwTitleID = IDS_TITLEPostItPpg;
        m_dwHelpFileID = IDS_HELPFILEPostItPpg;
        m_dwDocStringID = IDS_DOCSTRINGPostItPpg;
    }

    enum {IDD = IDD_POSTITPPG};

    DECLARE_REGISTRY_RESOURCEID(IDR_POSTITPPG)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CPostItPpg)
    COM_INTERFACE_ENTRY(IPropertyPage)
    END_COM_MAP()

    BEGIN_MSG_MAP(CPostItPpg)
    COMMAND_HANDLER(IDC_TEXT, EN_CHANGE, OnPropertyChange)
    COMMAND_HANDLER(IDC_APPEARANCE, CBN_SELCHANGE, OnPropertyChange)
    COMMAND_HANDLER(IDC_BORDERSTYLE, CBN_SELCHANGE, OnPropertyChange)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    CHAIN_MSG_MAP(IPropertyPageImpl<CPostItPpg>)
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&);
    LRESULT OnPropertyChange(WORD wNotify, WORD wID, HWND hWnd,
                             BOOL& bHandled);

    STDMETHOD(Apply)(void);
private:
    void HandleError();
};

#endif //__POSTITPPG_H_
