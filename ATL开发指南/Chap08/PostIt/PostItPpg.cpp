// PostItPpg.cpp : Implementation of CPostItPpg
#include "stdafx.h"
#include "PostIt.h"
#include "PostItPpg.h"

/////////////////////////////////////////////////////////////////////////////
// CPostItPpg

LRESULT CPostItPpg::OnInitDialog(UINT, WPARAM wParam,
                                 LPARAM lParam, BOOL&)
{
    USES_CONVERSION;

    if (m_nObjects > 0) {
        CComQIPtr<IPostItCtl, &IID_IPostItCtl> pPostItCtl(m_ppUnk[0]);
        BSTR bstrText;

        if SUCCEEDED(pPostItCtl->get_Text(&bstrText))
            SetDlgItemText(IDC_TEXT, W2A(bstrText));

        // Initialize the Appearance combo box
        SendDlgItemMessage(IDC_APPEARANCE,
                           CB_ADDSTRING,
                           0,
                           (long) "0 - Flat");
        SendDlgItemMessage(IDC_APPEARANCE,
                           CB_ADDSTRING,
                           0,
                           (long) "1 - 3D");
        // Get the current value of the Appearance property
        short sAppearance;

        if SUCCEEDED(pPostItCtl->get_Appearance(&sAppearance))
            ::SendMessage(GetDlgItem(IDC_APPEARANCE),
                          CB_SETCURSEL,
                          sAppearance,
                          0);

        // Initialize the BorderStyle combo box
        SendDlgItemMessage(IDC_BORDERSTYLE,
                           CB_ADDSTRING,
                           0,
                           (long) "0 - None");
        SendDlgItemMessage(IDC_BORDERSTYLE,
                           CB_ADDSTRING,
                           0,
                           (long) "1 - Single Line");
        // Get the current value of the BorderStyle property
        long lBorderStyle;

        if SUCCEEDED(pPostItCtl->get_BorderStyle(&lBorderStyle))
            ::SendMessage(GetDlgItem(IDC_BORDERSTYLE),
                          CB_SETCURSEL,
                          lBorderStyle,
                          0);
    }

    return 1;
}

LRESULT CPostItPpg::OnPropertyChange(WORD wNotify, WORD wID, HWND hWnd,
                                     BOOL& bHandled)
{
    SetDirty(TRUE);
    m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY |
                                PROPPAGESTATUS_VALIDATE);
    return 0;
}

void CPostItPpg::HandleError()
{
    USES_CONVERSION;
    CComPtr<IErrorInfo> pError;
    CComBSTR strError;
    GetErrorInfo(0, &pError);
    pError->GetDescription(&strError);
    MessageBox(OLE2T(strError),
               _T("Error"),
               MB_ICONEXCLAMATION);
}
STDMETHODIMP CPostItPpg::Apply(void)
{
    USES_CONVERSION;

    for (UINT i = 0; i < m_nObjects; i++) {
        CComQIPtr<IPostItCtl, &IID_IPostItCtl> pPostItCtl(m_ppUnk[i]);
        BSTR bstrText;

        if (GetDlgItemText(IDC_TEXT, bstrText)) {
            if FAILED(pPostItCtl->put_Text(bstrText)) {
                HandleError();
                return E_FAIL;
            }
        }

        enum enumAppearance eAppearance;
        eAppearance = (enum enumAppearance)
                      SendDlgItemMessage(IDC_APPEARANCE,
                                         CB_GETCURSEL,
                                         0, 0);

        if FAILED(pPostItCtl->put_Appearance(eAppearance)) {
            HandleError();
            return E_FAIL;
        }

        long lBorderStyle;
        lBorderStyle = SendDlgItemMessage(IDC_BORDERSTYLE,
                                          CB_GETCURSEL,
                                          0, 0);

        if FAILED(pPostItCtl->put_BorderStyle(lBorderStyle)) {
            HandleError();
            return E_FAIL;
        }
    }

    m_bDirty = FALSE;
    return S_OK;
}
