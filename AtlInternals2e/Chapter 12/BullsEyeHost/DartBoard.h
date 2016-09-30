// Dartboard.h : Declaration of the CDartboard

#pragma once

#include "resource.h"       // main symbols

#include <atlhost.h>


// CDartboard

class CDartboard :
    public CAxDialogImpl<CDartboard>,
    public IDispEventImpl<IDC_BULLSEYE, CDartboard>
{
public:
    CDartboard()
    {
        m_score = 0;
    }

    ~CDartboard()
    {
    }

    enum { IDD = IDD_DARTBOARD };

    BEGIN_MSG_MAP(CDartboard)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
    CHAIN_MSG_MAP(CAxDialogImpl<CDartboard>)
    END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
private:

    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
    BEGIN_SINK_MAP(CDartboard)
    SINK_ENTRY(IDC_BULLSEYE, 2, OnScoreChangedBullseye)
    END_SINK_MAP()

    void __stdcall OnScoreChangedBullseye(long ringValue);

private:
    void UpdateScoreDisplay();

    CAxWindow m_axBullsEye;
    CComPtr<IBullsEye> m_spBullsEye;
    int m_score;
};
