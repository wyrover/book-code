#include "stdafx.h"
#include "DBsrvImp.h"

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

HRESULT DEF_EXPORT DllGetClassFactoryObject(IDBSrvFactory ** ppObject)
{
    *ppObject = (IDBSrvFactory*) new CDBSrvFactory;
    return NO_ERROR;
}
