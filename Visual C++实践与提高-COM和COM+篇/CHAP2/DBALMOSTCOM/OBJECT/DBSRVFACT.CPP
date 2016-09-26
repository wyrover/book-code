#include "stdafx.h"
#include "DBsrvImp.h"

// {30DF3430-0266-11cf-BAA6-00AA003E0EED}
static const GUID CLSID_DBSAMPLE =
{ 0x30df3430, 0x266, 0x11cf, { 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed } };

// {30DF3431-0266-11cf-BAA6-00AA003E0EED}
static const GUID IID_IDBSrvFactory =
{ 0x30df3431, 0x266, 0x11cf, { 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed } };

// Create a new database object and return a pointer to it
HRESULT CDBSrvFactory::CreateDB(IDB** ppvDBObject)
{
    *ppvDBObject = (IDB*) new CDB;
    return NO_ERROR;
}

ULONG CDBSrvFactory::Release()
{
    delete this;
    return 0;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject)
{
    if (rclsid != CLSID_DBSAMPLE) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    if (riid != IID_IDBSrvFactory) {
        return E_INVALIDARG;
    }

    *ppObject = (IDBSrvFactory*) new CDBSrvFactory;
    return NO_ERROR;
}
