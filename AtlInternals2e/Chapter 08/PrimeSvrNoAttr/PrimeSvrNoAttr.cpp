// PrimeSvrNoAttr.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "PrimeSvrNoAttr.h"

class CPrimeSvrNoAttrModule : public CAtlExeModuleT< CPrimeSvrNoAttrModule >
{
public :
    DECLARE_LIBID(LIBID_PrimeSvrNoAttrLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PRIMESVRNOATTR, "{C5C4266F-696D-40DC-9C64-D8B3449A3DF3}")
};

CPrimeSvrNoAttrModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

