// CarusoeProvDS.h : Declaration of the CCarusoeProvSource
#ifndef __CCarusoeProvSource_H_
#define __CCarusoeProvSource_H_
#include "resource.h"       // main symbols
#include "CarusoeProvRS.h"
/////////////////////////////////////////////////////////////////////////////
// CDataSource
class ATL_NO_VTABLE CCarusoeProvSource :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CCarusoeProvSource, &CLSID_CarusoeProv>,
    public IDBCreateSessionImpl<CCarusoeProvSource, CCarusoeProvSession>,
    public IDBInitializeImpl<CCarusoeProvSource>,
    public IDBPropertiesImpl<CCarusoeProvSource>,
    public IPersistImpl<CCarusoeProvSource>,
    public IInternalConnectionImpl<CCarusoeProvSource>,
    public IDBCarusoeProvSource
{
protected:
    CString m_strServerFile;
    CString m_strUserId;
    CString m_strPassword;

public:
    HRESULT FinalConstruct()
    {
        return FInit();
    }

    HRESULT IsValidValue(ULONG iCurSet, DBPROP* pDBProp)
    {
        ATLASSERT(pDBProp != NULL);
        CComVariant var = pDBProp->vValue;

        switch (pDBProp->dwPropertyID) {
        case DBPROP_INIT_LOCATION:
            m_strServerFile = var.bstrVal;
            break;

        case DBPROP_AUTH_USERID:
            m_strUserId = var.bstrVal;
            break;

        case DBPROP_AUTH_PASSWORD:
            m_strPassword = var.bstrVal;
            break;

        default:
            break;
        }

        return IDBPropertiesImpl<CCarusoeProvSource>::IsValidValue(iCurSet, pDBProp);
    }

    STDMETHODIMP CreateSession(IUnknown* pUnkOuter, REFIID riid, IUnknown** ppDBSession)
    {
        HRESULT hr = S_OK;

        // Make sure we have a server file
        if (m_strServerFile.IsEmpty())
            hr = E_FAIL;

        // Make sure we have valid logon information
        if (SUCCEEDED(hr))
            hr = AuthenticateLogonInfo();

        // Create the Session
        if (SUCCEEDED(hr))
            hr = IDBCreateSessionImpl<CCarusoeProvSource, CCarusoeProvSession>::CreateSession(pUnkOuter, riid, ppDBSession);

        return hr;
    }

    HRESULT AuthenticateLogonInfo()
    {
        HRESULT hr = E_ACCESSDENIED;

        if (m_strUserId == CString("rpatton"))
            if (m_strPassword == CString("password"))
                hr = S_OK;

        return hr;
    }

    // IDBCarusoeProvSource
    STDMETHODIMP GetServerFileName(BSTR* pBSTRFileName)
    {
        *pBSTRFileName = m_strServerFile.AllocSysString();
        return S_OK;
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_CARUSOEPROV)
    BEGIN_PROPSET_MAP(CCarusoeProvSource)
    BEGIN_PROPERTY_SET(DBPROPSET_DATASOURCEINFO)
    PROPERTY_INFO_ENTRY(ACTIVESESSIONS)
    PROPERTY_INFO_ENTRY(DATASOURCEREADONLY)
    PROPERTY_INFO_ENTRY(BYREFACCESSORS)
    PROPERTY_INFO_ENTRY(OUTPUTPARAMETERAVAILABILITY)
    PROPERTY_INFO_ENTRY(PROVIDEROLEDBVER)
    PROPERTY_INFO_ENTRY(DSOTHREADMODEL)
    PROPERTY_INFO_ENTRY(SUPPORTEDTXNISOLEVELS)
    PROPERTY_INFO_ENTRY(USERNAME)
    PROPERTY_INFO_ENTRY_EX(SQLSUPPORT, VT_I4, 0, DBPROPVAL_SQL_NONE, DBPROPOPTIONS_REQUIRED)
    END_PROPERTY_SET(DBPROPSET_DATASOURCEINFO)
    BEGIN_PROPERTY_SET(DBPROPSET_DBINIT)
    PROPERTY_INFO_ENTRY(AUTH_PASSWORD)
    PROPERTY_INFO_ENTRY(AUTH_PERSIST_SENSITIVE_AUTHINFO)
    PROPERTY_INFO_ENTRY(AUTH_USERID)
    PROPERTY_INFO_ENTRY(INIT_DATASOURCE)
    PROPERTY_INFO_ENTRY(INIT_HWND)
    PROPERTY_INFO_ENTRY(INIT_LCID)
    PROPERTY_INFO_ENTRY(INIT_LOCATION)
    PROPERTY_INFO_ENTRY(INIT_MODE)
    PROPERTY_INFO_ENTRY(INIT_PROMPT)
    PROPERTY_INFO_ENTRY(INIT_PROVIDERSTRING)
    PROPERTY_INFO_ENTRY(INIT_TIMEOUT)
    END_PROPERTY_SET(DBPROPSET_DBINIT)
    CHAIN_PROPERTY_SET(CCarusoeProvCommand)
    END_PROPSET_MAP()
    BEGIN_COM_MAP(CCarusoeProvSource)
    COM_INTERFACE_ENTRY(IDBCreateSession)
    COM_INTERFACE_ENTRY(IDBInitialize)
    COM_INTERFACE_ENTRY(IDBProperties)
    COM_INTERFACE_ENTRY(IPersist)
    COM_INTERFACE_ENTRY(IInternalConnection)
    COM_INTERFACE_ENTRY(IDBCarusoeProvSource)
    END_COM_MAP()
public:
};
#endif //__CCarusoeProvSource_H_
