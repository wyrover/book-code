// MathExeNoAttr.cpp : Implementation of WinMain

#include "stdafx.h"
#include "resource.h"
#include "MathExeNoAttr.h"

class CMathExeNoAttrModule : public CAtlExeModuleT< CMathExeNoAttrModule >
{
public :
    DECLARE_LIBID(LIBID_MathExeNoAttrLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MATHEXENOATTR, "{FD676438-AB64-4BAE-A713-C04460E7C30C}")
};

CMathExeNoAttrModule _AtlModule;

CAtlAutoThreadModule _AtlAutoModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}

