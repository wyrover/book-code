// TickerPP.h : Declaration of the CTickerPP

#ifndef __TICKERPP_H_
#define __TICKERPP_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_TickerPP;

/////////////////////////////////////////////////////////////////////////////
// CTickerPP
class ATL_NO_VTABLE CTickerPP :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CTickerPP, &CLSID_TickerPP>,
    public IPropertyPageImpl<CTickerPP>,
    public CDialogImpl<CTickerPP>
{
public:
    CTickerPP()
    {
        m_dwTitleID = IDS_TITLETickerPP;
        m_dwHelpFileID = IDS_HELPFILETickerPP;
        m_dwDocStringID = IDS_DOCSTRINGTickerPP;
    }

    enum {IDD = IDD_TICKERPP};

    DECLARE_REGISTRY_RESOURCEID(IDR_TICKERPP)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CTickerPP)
    COM_INTERFACE_ENTRY(IPropertyPage)
    END_COM_MAP()

    BEGIN_MSG_MAP(CTickerPP)
    CHAIN_MSG_MAP(IPropertyPageImpl<CTickerPP>)
    COMMAND_HANDLER(ID_SPEED, EN_CHANGE, OnChangeSpeed)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    STDMETHOD(Apply)(void)
    {
        ATLTRACE(_T("CTickerPP::Apply\n"));

        for (UINT i = 0; i < m_nObjects; i++) {
            //Apply the change to speed property
            ITicker* pT;
            //get an ITicker interface
            m_ppUnk[i]->QueryInterface(IID_ITicker, (void**)&pT);
            //call the put_Speed method
            pT->put_Speed((long)GetDlgItemInt(ID_SPEED));
            //release the interface
            pT->Release();
        }

        //we are clean now
        m_bDirty = FALSE;
        return S_OK;
    }
    LRESULT OnChangeSpeed(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        SetDirty(TRUE);
        return 0;
    }
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        //read the speed value and display it

        //don't do anything if multiple controls
        if (m_nObjects > 1)
            return 0;

        ITicker* pT;
        //get an ITicker interface
        m_ppUnk[0]->QueryInterface(IID_ITicker, (void**)&pT);
        //call the gut_Speed method
        long speed = 0;
        pT->get_Speed(&speed);
        //release the interface
        pT->Release();
        //set the edit box text
        SetDlgItemInt(ID_SPEED, speed, FALSE);
        return 0;
    }
};

#endif //__TICKERPP_H_
