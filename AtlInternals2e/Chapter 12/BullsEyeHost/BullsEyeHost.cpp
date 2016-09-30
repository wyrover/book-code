// BullsEyeHost.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
//#include "BullsEyeHost.h"
#include "MainWindow.h"

class CBullsEyeHostModule : public CAtlExeModuleT< CBullsEyeHostModule >
{
public:
    HRESULT PreMessageLoop(int showCmd);
    HRESULT PostMessageLoop();

private:
    CMainWindow m_wnd;

};

CBullsEyeHostModule _AtlModule;

//
// Good 'ol WinMain. Starts up via the handy-dandy AtlExeModule.
//

extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

//
// Module methods to startup and shutdown
//

HRESULT CBullsEyeHostModule::PreMessageLoop(int showCmd)
{
    CString appTitle;

    if (!appTitle.LoadStringW(IDS_WINDOWTITLE)) {
        return E_FAIL;
    }

    HMENU hMenu = LoadMenu(_pModule->GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINMENU));

    if (hMenu == 0) {
        return E_FAIL;
    }

    m_wnd.Create(0, CWindow::rcDefault, appTitle, 0, 0, hMenu);

    if (!m_wnd) {
        return E_FAIL;
    }

    m_wnd.ShowWindow(showCmd);
    m_wnd.CenterWindow();
    return S_OK;
}

HRESULT CBullsEyeHostModule::PostMessageLoop()
{
    return S_OK;
}
