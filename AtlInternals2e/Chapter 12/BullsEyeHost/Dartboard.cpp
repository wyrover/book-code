// Dartboard.cpp : Implementation of CDartboard

#include "stdafx.h"
#include "Dartboard.h"


// CDartboard

// Window Message Handlers

LRESULT CDartboard::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CAxDialogImpl<CDartboard>::OnInitDialog(uMsg, wParam, lParam, bHandled);
    m_axBullsEye.Attach(GetDlgItem(IDC_BULLSEYE));
    m_axBullsEye.QueryControl(&m_spBullsEye);
    UpdateScoreDisplay();
    return 1;  // Let the system set the focus
}

LRESULT CDartboard::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

// COM Event handlers

void __stdcall CDartboard::OnScoreChangedBullseye(long ringValue)
{
    m_score += ringValue;
    UpdateScoreDisplay();
}

// Helper functions

void CDartboard::UpdateScoreDisplay()
{
    CWindow scoreWnd;
    scoreWnd.Attach(GetDlgItem(IDC_SCORE));
    CString strScore;
    strScore.Format(L"%d", m_score);
    scoreWnd.SetWindowTextW(strScore);
}

