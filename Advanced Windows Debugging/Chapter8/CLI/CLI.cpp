/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <stdio.h>
#include <tchar.h>
#include <comdef.h>
#include <ole2.h>
#include "tchar.h"
#include "strsafe.h"
#include "menu.h"

#include "srv.h"
#include "srv_i.c"


WCHAR * targetHost = NULL;

void  MTAClientCall();
void  STAClientCall();

void __cdecl wmain(int argc, WCHAR* argv[])
{
    if (argc>=2 && _wcsnicmp(argv[1], L"SERVER:", wcslen(L"SERVER:"))==0)
    {
            targetHost = argv[1]+wcslen(L"SERVER:");
    }
    if (targetHost)
    {
        wprintf(L"\nCurrently connected to : %s\n",targetHost);
    }else
    {
        wprintf(L"Currently connected to <local host>\nUse 08cli.exe server:<server address> \nto connect to an alternate server\n\n");
    }

    OPTIONS options[]={
        {L'0',L"To connect to a COM server from a MTA apartment",MTAClientCall},
        {L'1',L"To connect to a COM server from a STA apartment",STAClientCall},
        {L'x',L"To exit",NULL},
    }; 

    AppInfo appInfo=AppInfo(options);
    appInfo.Loop();

}


void IntializeServerInfo(COSERVERINFO &si)
{
    si.pwszName = targetHost;
    si.dwReserved1 = 0;
    si.dwReserved2 = 0;
    COAUTHINFO& ai = *si.pAuthInfo;
    ai.dwAuthzSvc = RPC_C_AUTHZ_NONE;

    ai.dwAuthnLevel = RPC_C_AUTHN_LEVEL_DEFAULT;
    ai.dwImpersonationLevel = RPC_C_IMP_LEVEL_IMPERSONATE;
    ai.dwCapabilities = 0;

    ai.dwAuthnSvc = RPC_C_AUTHN_DEFAULT;
    ai.pwszServerPrincName = NULL;        
    COAUTHIDENTITY& authident = *ai.pAuthIdentityData;
    ai.pAuthIdentityData = NULL;
}

void  STAClientCall()
{
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
    {
        COSERVERINFO si;
        COAUTHINFO ai;
        si.pAuthInfo = &ai;
        COAUTHIDENTITY authident;
        ai.pAuthIdentityData = &authident;
        IntializeServerInfo (si);

        MULTI_QI mqi;
        mqi.pIID = &IID_ICalculator;
        mqi.pItf = 0;
        mqi.hr = 0;

        wprintf(L"\nCalling CoCreateInstanceEx ... ");
        if (SUCCEEDED(CoCreateInstanceEx(
            CLSID_Calculator,
            NULL,
            CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER,
            &si,
            1,
            &mqi
        )))
        {
            wprintf(L"done\n");
            ICalculator * pCalculator = NULL;
            pCalculator = (ICalculator*)mqi.pItf;
            __int32 result = 0;
            wprintf(L"Calling SumSlow ...");
            pCalculator->SumSlow(1,2, &result);
            pCalculator->Release();
            wprintf(L"done\n");
        }
        else
        {
	        wprintf(L"failed\n");
        }
        CoUninitialize();
    }
}
void MTAClientCall()
{
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
    {
        COSERVERINFO si;
        COAUTHINFO ai;
        si.pAuthInfo = &ai;
        COAUTHIDENTITY authident;
        ai.pAuthIdentityData = &authident;
        IntializeServerInfo (si);

        MULTI_QI mqi;
        mqi.pIID = &IID_ICalculator;
        mqi.pItf = 0;
        mqi.hr = 0;

        wprintf(L"\nCalling CoCreateInstanceEx ... ");
        if (SUCCEEDED(CoCreateInstanceEx(
            CLSID_Calculator,
            NULL,
            CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER,
            &si,
            1,
            &mqi
        )))
        {
            wprintf(L"done\n");
            ICalculator * pCalculator = NULL;
            pCalculator = (ICalculator*)mqi.pItf;
            __int32 result = 0;
            wprintf(L"Calling SumSlow ...");
            pCalculator->SumSlow(1,2, &result);
            pCalculator->Release();
            wprintf(L"done\n");
        }
        else
        {
	        wprintf(L"failed\n");
        }
        CoUninitialize();
    }
}
