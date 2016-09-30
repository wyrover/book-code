// Advert.h : Declaration of the CAdvert

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>


// CAdvert

class CAdvert :
    public CAxDialogImpl<CAdvert>
{
public:
    CAdvert()
    {
    }

    ~CAdvert()
    {
    }

    enum { IDD = IDD_ADVERT };

    BEGIN_MSG_MAP(CAdvert)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
    COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
    CHAIN_MSG_MAP(CAxDialogImpl<CAdvert>)
    END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
//      CAxDialogImpl<CAdvert>::OnInitDialog(uMsg, wParam, lParam, bHandled);
        if (m_bstrClient.Length()) {
            CComBSTR  bstrCaption = OLESTR("Sponsered by ");
            bstrCaption += m_bstrClient;
            USES_CONVERSION;
            SetWindowText(OLE2CT(bstrCaption));
        }

        return 1;  // Let the system set the focus
    }

    LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        EndDialog(wID);
        return 0;
    }

    LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
        EndDialog(wID);
        return 0;
    }

    CComBSTR  m_bstrClient;
};


