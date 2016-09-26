// Session.h : Declaration of the CCarusoeProvSession
#ifndef __CCarusoeProvSession_H_
#define __CCarusoeProvSession_H_
#include "resource.h"       // main symbols
#include "CarusoeProvRS.h"
#include "CarusoeServerFile.h"
#include "SimpleCSVParser.h"

class CCarusoeProvSessionTRSchemaRowset;
class CCarusoeProvSessionColSchemaRowset;
class CCarusoeProvSessionPTSchemaRowset;
/////////////////////////////////////////////////////////////////////////////
// CCarusoeProvSession
class ATL_NO_VTABLE CCarusoeProvSession :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IGetDataSourceImpl<CCarusoeProvSession>,
    public IOpenRowsetImpl<CCarusoeProvSession>,
    public ISessionPropertiesImpl<CCarusoeProvSession>,
    public IObjectWithSiteSessionImpl<CCarusoeProvSession>,
    public IDBSchemaRowsetImpl<CCarusoeProvSession>,
    public IDBCreateCommandImpl<CCarusoeProvSession, CCarusoeProvCommand>
{
public:
    CCarusoeProvSession()
    {
    }
    HRESULT FinalConstruct()
    {
        return FInit();
    }
    STDMETHOD(OpenRowset)(IUnknown *pUnk, DBID *pTID, DBID *pInID, REFIID riid,
                          ULONG cSets, DBPROPSET rgSets[], IUnknown **ppRowset)
    {
        CCarusoeProvRowset* pRowset;
        return CreateRowset(pUnk, pTID, pInID, riid, cSets, rgSets, ppRowset, pRowset);
    }
    BEGIN_PROPSET_MAP(CCarusoeProvSession)
    BEGIN_PROPERTY_SET(DBPROPSET_SESSION)
    PROPERTY_INFO_ENTRY(SESS_AUTOCOMMITISOLEVELS)
    END_PROPERTY_SET(DBPROPSET_SESSION)
    END_PROPSET_MAP()
    BEGIN_COM_MAP(CCarusoeProvSession)
    COM_INTERFACE_ENTRY(IGetDataSource)
    COM_INTERFACE_ENTRY(IOpenRowset)
    COM_INTERFACE_ENTRY(ISessionProperties)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY(IDBCreateCommand)
    COM_INTERFACE_ENTRY(IDBSchemaRowset)
    END_COM_MAP()
    BEGIN_SCHEMA_MAP(CCarusoeProvSession)
    SCHEMA_ENTRY(DBSCHEMA_TABLES, CCarusoeProvSessionTRSchemaRowset)
    SCHEMA_ENTRY(DBSCHEMA_COLUMNS, CCarusoeProvSessionColSchemaRowset)
    SCHEMA_ENTRY(DBSCHEMA_PROVIDER_TYPES, CCarusoeProvSessionPTSchemaRowset)
    END_SCHEMA_MAP()
};
class CCarusoeProvSessionTRSchemaRowset :
    public CRowsetImpl< CCarusoeProvSessionTRSchemaRowset, CTABLESRow, CCarusoeProvSession>
{
public:
    HRESULT Execute(LONG* pcRowsAffected, ULONG, const VARIANT*)
    {
        USES_CONVERSION;
        CTABLESRow trData;
        HRESULT hr = S_OK;
        long rowsAffected = 0;
        IGetDataSource* pDataSource = NULL;
        IDBCarusoeProvSource* pDBCarusoeProvSource = NULL;

        try {
            hr = m_spUnkSite->QueryInterface(IID_IGetDataSource, (LPVOID*)&pDataSource);

            if (FAILED(hr))
                throw hr;

            hr = pDataSource->GetDataSource(IID_IDBCarusoeProvSource, (LPUNKNOWN*) &pDBCarusoeProvSource);

            if (FAILED(hr))
                throw hr;

            BSTR bstrServerName;
            hr = pDBCarusoeProvSource->GetServerFileName(&bstrServerName);

            if (FAILED(hr))
                throw hr;

            CString fileName(bstrServerName);
            ::SysFreeString(bstrServerName);
            lstrcpyW(trData.m_szType, OLESTR("TABLE"));
            lstrcpyW(trData.m_szDesc, OLESTR("Carusoe Server File"));
            CCarusoeServerFile serverFile;
            serverFile.Initialize(fileName);

            for (long l = 0; l < serverFile.GetTableCount(); l++) {
                CComBSTR tableName;
                serverFile.GetTableAt(l, tableName);
                lstrcpyW(trData.m_szTable, tableName.m_str);

                if (!m_rgRowData.Add(trData))
                    throw E_OUTOFMEMORY;

                rowsAffected++;
            }
        } catch (HRESULT errorCode) {
            TRACE("Error %X thrown in CCarusoeProvSessionTRSchemaRowset::Execute()\n", errorCode);
        }

        if (pDBCarusoeProvSource)
            pDBCarusoeProvSource->Release();

        if (pDataSource)
            pDataSource->Release();

        *pcRowsAffected = rowsAffected;
        return hr;
    }
};
class CCarusoeProvSessionColSchemaRowset :
    public CRowsetImpl< CCarusoeProvSessionColSchemaRowset, CCOLUMNSRow, CCarusoeProvSession>
{
public:
    HRESULT Execute(LONG* pcRowsAffected, ULONG, const VARIANT*)
    {
        USES_CONVERSION;
        CCOLUMNSRow trData;
        HRESULT hr = S_OK;
        long rowsAffected = 0;
        IGetDataSource* pDataSource = NULL;
        IDBCarusoeProvSource* pDBCarusoeProvSource = NULL;

        try {
            hr = m_spUnkSite->QueryInterface(IID_IGetDataSource, (LPVOID*)&pDataSource);

            if (FAILED(hr))
                throw hr;

            hr = pDataSource->GetDataSource(IID_IDBCarusoeProvSource, (LPUNKNOWN*) &pDBCarusoeProvSource);

            if (FAILED(hr))
                throw hr;

            BSTR bstrServerName;
            hr = pDBCarusoeProvSource->GetServerFileName(&bstrServerName);

            if (FAILED(hr))
                throw hr;

            CString fileName(bstrServerName);
            ::SysFreeString(bstrServerName);
            CCarusoeServerFile serverFile;
            serverFile.Initialize(fileName);

            for (long l = 0; l < serverFile.GetTableCount(); l++) {
                CSimpleCSVParser parser;
                CComBSTR bstrTableName;
                serverFile.GetTableAt(l, bstrTableName);
                parser.Initialize(OLE2T(bstrTableName));
                lstrcpyW(trData.m_szTableName, bstrTableName);

                for (int cols = 0; cols < parser.GetColumnCount(); cols++) {
                    CColumnDefinition* pColDef = parser.GetColumnDefinition(cols);
                    trData.m_ulOrdinalPosition = cols + 1;
                    trData.m_bIsNullable = VARIANT_TRUE;
                    trData.m_bColumnHasDefault = VARIANT_FALSE;
                    trData.m_ulCharMaxLength = pColDef->GetColumnSize();
                    trData.m_ulColumnFlags = 0;
                    lstrcpyW(trData.m_szColumnName, T2OLE(pColDef->GetColumnName()));

                    switch (pColDef->GetColumnType()) {
                    case stringType:
                        trData.m_nDataType = DBTYPE_STR;
                        trData.m_ulCharMaxLength +=  sizeof(TCHAR);
                        break;

                    case intType:
                        trData.m_nDataType = DBTYPE_UI4;
                        trData.m_ulCharMaxLength = sizeof(long);
                        break;

                    case floatType:
                        trData.m_nDataType = DBTYPE_R8;
                        trData.m_ulCharMaxLength = sizeof(double);
                        break;

                    case dateType:
                        trData.m_nDataType = DBTYPE_DBDATE;
                        trData.m_ulCharMaxLength = sizeof(DBDATE);
                        break;
                    }

                    // Add to the row data
                    m_rgRowData.Add(trData);
                }
            }
        } catch (HRESULT errorCode) {
            TRACE("Error %X thrown in CCarusoeProvSessionTRSchemaRowset::Execute()\n", errorCode);
        }

        if (pDBCarusoeProvSource)
            pDBCarusoeProvSource->Release();

        if (pDataSource)
            pDataSource->Release();

        return hr;
    }
};
class CCarusoeProvSessionPTSchemaRowset :
    public CRowsetImpl< CCarusoeProvSessionPTSchemaRowset, CPROVIDER_TYPERow, CCarusoeProvSession>
{
public:
    HRESULT Execute(LONG* pcRowsAffected, ULONG, const VARIANT*)
    {
        return S_OK;
    }
};
#endif //__CCarusoeProvSession_H_
