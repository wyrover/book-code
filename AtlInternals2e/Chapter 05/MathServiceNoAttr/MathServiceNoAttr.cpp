// MathServiceNoAttr.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "MathServiceNoAttr.h"
#include <stdio.h>

class CMathServiceNoAttrModule : public CAtlServiceModuleT< CMathServiceNoAttrModule, IDS_SERVICENAME >
{
public :
    DECLARE_LIBID(LIBID_MathServiceNoAttrLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MATHSERVICENOATTR, "{2D1CEBC5-3664-4FDA-A565-4FFC3875963A}")
};

CMathServiceNoAttrModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

