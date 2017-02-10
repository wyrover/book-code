/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include <stdio.h>
#include <tchar.h>
#include <srv.h>
#include <comdef.h>
#include <ole2.h>

#include "resource.h"
#include "SRV.h"
#include "SRV_i.c"
#include "comdef.h"

#define _ATL_APARTMENT_THREADED


#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

using namespace ATL;
class CSRVModule : public CAtlExeModuleT< CSRVModule >
{
public :
    DECLARE_LIBID(LIBID_SRVLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SRV, "{EC494AB8-2256-4D49-A581-79ABA1D9501A}")
};

CSRVModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    return _AtlModule.WinMain(nShowCmd);
}

