#pragma once
#include "stdafx.h"

const UINT ID_BULLSEYE = 1;

class CMainWindow :
    public CWindowImpl< CMainWindow, CWindow, CFrameWinTraits  >,
    public IDispEventImpl< ID_BULLSEYE, CMainWindow, &__uuidof(_IBullsEyeEvents), &__uuidof(__BullsEyeControlLib), 1, 0 >
{
public:
    CMainWindow(void);
    virtual ~CMainWindow(void);

private:
    BEGIN_MSG_MAP(CMainWindow)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    COMMAND_HANDLER(ID_FILE_EXIT, 0, OnFileExit)
    COMMAND_HANDLER(ID_WINDOWS_DARTBOARD, 0, OnWindowsDartboard)
    END_MSG_MAP()

    BEGIN_SINK_MAP(CMainWindow)
    SINK_ENTRY_EX(ID_BULLSEYE, __uuidof(_IBullsEyeEvents), 1, OnRingHit)
    SINK_ENTRY_EX(ID_BULLSEYE, __uuidof(_IBullsEyeEvents), 2, OnScoreChanged)
    END_SINK_MAP()
    //
    // Window message handlers
    //
    LRESULT OnCreate(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled);
    LRESULT OnDestroy(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled);
    LRESULT OnSize(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled);
    LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hwndCtl, BOOL &bHandled);
    LRESULT OnWindowsDartboard(WORD wNotifyCode, WORD wID, HWND hwndCtl, BOOL &bHandled);
    LRESULT OnClose(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled);

    //
    // Sinking events on the bullseye control
    //
    void __stdcall OnRingHit(short ringNum);
    void __stdcall OnScoreChanged(long ringValue);

private:
    // Member data

    // Our child control
    CAxWindow m_bullseyeWin;
    CComPtr< IBullsEye > m_bullseyeControl;
};
