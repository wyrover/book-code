#ifndef _DBSERVER_INCLUDE
#define _DBSERVER_INCLUDE

typedef long HRESULT;

#define DEF_EXPORT __declspec(dllexport)

class IDB
{
    // Interfaces
public:
    // Interface for data access
    virtual HRESULT Read(short nTable, short nRow, LPWSTR lpszData) = 0;
    virtual HRESULT Write(short nTable, short nRow, LPCWSTR lpszData) = 0;

    // Interface for database management
    virtual HRESULT Create(short &nTable, LPCWSTR lpszName) = 0;
    virtual HRESULT Delete(short nTable) = 0;

    // Interfase para obtenber informacion sobre la base de datos
    virtual HRESULT GetNumTables(short &nNumTables) = 0;
    virtual HRESULT GetTableName(short nTable, LPWSTR lpszName) = 0;
    virtual HRESULT GetNumRows(short nTable, short &nRows) = 0;

    virtual ULONG Release() = 0;
};

class IDBSrvFactory
{
    // Interface
public:
    virtual HRESULT CreateDB(IDB** ppObject) = 0;
    virtual ULONG   Release() = 0;
};

HRESULT DEF_EXPORT DllGetClassFactoryObject(IDBSrvFactory ** ppObject);

#endif
