#include "firewall.h"

BOOL Firewall::GetPortState(int nPort)
{
    INetFwOpenPorts* FwOpenPorts = NULL;
    INetFwOpenPort* FwOpenPort = NULL;
    HRESULT hr = m_NetFwProfile->get_GloballyOpenPorts(&FwOpenPorts);

    if (FAILED(hr)) goto Error_Port;

    hr = FwOpenPorts->Item(nPort, NET_FW_IP_PROTOCOL_TCP, &FwOpenPort);

    if (FAILED(hr)) goto Error_Port;

    VARIANT_BOOL bEnable = VARIANT_FALSE;
    hr = FwOpenPort->get_Enabled(&bEnable);

    if (FAILED(hr)) goto Error_Port;

    return bEnable == VARIANT_TRUE;
Error_Port:
    m_nError = (int)hr;

    if (FwOpenPort != NULL) FwOpenPort->Release();

    if (FwOpenPorts != NULL) FwOpenPorts->Release();

    return FALSE;
}

Firewall::Firewall() : m_NetFwProfile(NULL)
    , m_nError(0)
{
}



Firewall::~Firewall()
{
    //Don't call Uninit() here! May be CoUninitialize() called before dctor()
}

BOOL Firewall::Init()
{
    INetFwMgr* FwMgr = NULL;
    INetFwPolicy* FwPolicy = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(NetFwMgr), NULL, CLSCTX_INPROC_SERVER,
                                  __uuidof(INetFwMgr),
                                  (void**)&FwMgr);

    if (FAILED(hr)) goto Error_Pos;

    hr = FwMgr->get_LocalPolicy(&FwPolicy);

    if (FAILED(hr))goto Error_Pos;

    hr = FwPolicy->get_CurrentProfile(&m_NetFwProfile);

    if (FAILED(hr))goto Error_Pos;

    m_nError = hr;
    return TRUE;
Error_Pos:
    m_nError = (int)hr;

    if (FwPolicy != NULL) FwPolicy->Release();

    if (FwMgr != NULL) FwMgr->Release();

    return FALSE;
}

VOID Firewall::Uninit()
{
    if (m_NetFwProfile != NULL) {
        m_NetFwProfile->Release();
        m_NetFwProfile = NULL;
    }
}

BOOL Firewall::GetState()
{
    VARIANT_BOOL bEnabled = FALSE;
    HRESULT hr = m_NetFwProfile->get_FirewallEnabled(&bEnabled);

    if (FAILED(hr)) {
        m_nError = (int)hr;
        return FALSE;
    }

    m_nError = hr;
    return bEnabled == VARIANT_TRUE;
}

BOOL Firewall::TurnOn()
{
    if (!GetState() && SUCCEEDED((HRESULT)m_nError)) {
        HRESULT hr = m_NetFwProfile->put_FirewallEnabled(VARIANT_TRUE);

        if (FAILED(hr)) {
            m_nError = (HRESULT)hr;
            return FALSE;
        }
    }

    return GetState();
}

BOOL Firewall::TurnOff()
{
    if (GetState()
        && FAILED(m_NetFwProfile->put_FirewallEnabled(VARIANT_FALSE))) {
        return TRUE;
    }

    return GetState();
}

BOOL Firewall::GetAppState(generic_string sAppName)
{
    INetFwAuthorizedApplications* FwApps = NULL;
    INetFwAuthorizedApplication* FwApp = NULL;
    BSTR bstrAppName = NULL;
    HRESULT hr = m_NetFwProfile->get_AuthorizedApplications(&FwApps);

    if (FAILED(hr)) goto Error_App;

    bstrAppName = SysAllocString(sAppName.data());

    if (bstrAppName == NULL) goto Error_App;

    hr = FwApps->Item(bstrAppName, &FwApp);

    if (FAILED(hr)) goto Error_App;

    VARIANT_BOOL bEnable = VARIANT_FALSE;
    hr = FwApp->get_Enabled(&bEnable);

    if (FAILED(hr)) goto Error_App;

    m_nError = hr;
    return bEnable == VARIANT_TRUE;
Error_App:
    m_nError = (bstrAppName == NULL) ? -1 : hr;

    if (bstrAppName != NULL) SysFreeString(bstrAppName);

    if (FwApp != NULL) FwApp->Release();

    if (FwApps != NULL) FwApps->Release();

    return FALSE;
}

BOOL Firewall::AddAppToWall(generic_string sAppName, generic_string sTag)
{
    if (!GetAppState(sAppName)) {
        INetFwAuthorizedApplications* FwApps = NULL;
        INetFwAuthorizedApplication* fwApp = NULL;
        BSTR bstrAppName = NULL;
        BSTR bstrTag = NULL;
        HRESULT hr = m_NetFwProfile->get_AuthorizedApplications(&FwApps);

        if (FAILED(hr)) goto Error_AddWall;

        hr = CoCreateInstance(__uuidof(NetFwAuthorizedApplication), NULL,
                              CLSCTX_INPROC_SERVER, __uuidof(INetFwAuthorizedApplication),
                              (void**)&fwApp);

        if (FAILED(hr)) goto Error_AddWall;

        bstrAppName = SysAllocString(sAppName.data());

        if (bstrAppName == NULL) goto Error_AddWall;

        hr = fwApp->put_ProcessImageFileName(bstrAppName);

        if (FAILED(hr)) goto Error_AddWall;

        bstrTag = SysAllocString(sTag.data());

        if (bstrTag == NULL) goto Error_AddWall;

        hr = fwApp->put_Name(bstrTag);

        if (FAILED(hr)) goto Error_AddWall;

        hr = FwApps->Add(fwApp);

        if (FAILED(hr)) goto Error_AddWall;

        return TRUE;
Error_AddWall:

        if (bstrAppName != NULL) SysFreeString(bstrAppName);

        if (bstrTag     != NULL) SysFreeString(bstrTag);

        if (fwApp != NULL) fwApp->Release();

        if (FwApps != NULL) FwApps->Release();

        return FALSE;
    }

    return TRUE;
}

BOOL Firewall::AddPortToWall(int nPort, generic_string sTag)
{
    if (!GetPortState(nPort)) {
        INetFwOpenPort*  FwOpenPort  = NULL;
        INetFwOpenPorts* FwOpenPorts = NULL;
        BSTR bstrTag = NULL;
        HRESULT hr = m_NetFwProfile->get_GloballyOpenPorts(&FwOpenPorts);

        if (FAILED(hr)) goto Error_AddPort;

        hr = CoCreateInstance(__uuidof(NetFwOpenPort), NULL,
                              CLSCTX_INPROC_SERVER, __uuidof(INetFwOpenPort),
                              (void**)&FwOpenPort);

        if (FAILED(hr)) goto Error_AddPort;

        hr = FwOpenPort->put_Port(nPort);

        if (FAILED(hr)) goto Error_AddPort;

        hr = FwOpenPort->put_Protocol(NET_FW_IP_PROTOCOL_TCP);

        if (FAILED(hr)) goto Error_AddPort;

        bstrTag = SysAllocString(sTag.data());

        if (bstrTag == NULL) goto Error_AddPort;

        hr = FwOpenPort->put_Name(bstrTag);

        if (FAILED(hr)) goto Error_AddPort;

        hr = FwOpenPorts->Add(FwOpenPort);

        if (FAILED(hr)) goto Error_AddPort;

        return TRUE;
Error_AddPort:

        if (bstrTag == NULL) SysFreeString(bstrTag);

        if (FwOpenPort != NULL) FwOpenPort->Release();

        if (FwOpenPorts != NULL) FwOpenPorts->Release();

        return FALSE;
    }

    return TRUE;
}


