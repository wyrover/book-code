#include "stdafx.h"
#include "..\interface\dbsrv.h"

// Create a new database object and return a pointer to it
HRESULT CDBSrvFactory::CreateDB(CDB** ppvDBObject)
{
    *ppvDBObject = new CDB;
    return NO_ERROR;
}

ULONG CDBSrvFactory::Release()
{
    delete this;
    return 0;
}

HRESULT DEF_EXPORT DllGetClassFactoryObject(CDBSrvFactory ** ppObject)
{
    *ppObject = new CDBSrvFactory;
    return NO_ERROR;
}
