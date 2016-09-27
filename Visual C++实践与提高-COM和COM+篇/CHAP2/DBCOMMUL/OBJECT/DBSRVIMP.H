#ifndef _DBSERVERIMP_INCLUDE
#define _DBSERVERIMP_INCLUDE

#include "..\interface\dbsrv.h"

typedef long HRESULT;

class CDB : public IDB, public IDBAccess, public IDBManage, public IDBInfo
{
    // Interfaces
public:
    // Interface for data access
    HRESULT _stdcall Read(short nTable, short nRow, LPWSTR lpszData);
    HRESULT _stdcall Write(short nTable, short nRow, LPCWSTR lpszData);

    // Interface for database management
    HRESULT _stdcall Create(short &nTable, LPCWSTR lpszName);
    HRESULT _stdcall Delete(short nTable);

    // Interfase para obtenber informacion sobre la base de datos
    HRESULT _stdcall GetNumTables(short &nNumTables);
    HRESULT _stdcall GetTableName(short nTable, LPWSTR lpszName);
    HRESULT _stdcall GetNumRows(short nTable, short &nRows);

    HRESULT _stdcall QueryInterface(REFIID riid, void** ppObject);
    ULONG   _stdcall AddRef();
    ULONG   _stdcall Release();

    // Implementation
private:
    CPtrArray m_arrTables;   // Array of pointers to CStringArray (the "database")
    CStringArray m_arrNames; // Array of table names
    ULONG m_dwRefCount;
public:
    CDB();
    ~CDB();
};

extern ULONG g_dwRefCount;

class CDBSrvFactory : public IClassFactory
{
    // Interface
public:
    HRESULT _stdcall QueryInterface(REFIID riid, void** ppObject);
    ULONG   _stdcall AddRef();
    ULONG   _stdcall Release();

    HRESULT _stdcall CreateInstance(IUnknown *pUnkOuter, REFIID riid, void** ppObject);
    HRESULT _stdcall LockServer(BOOL fLock);
    // Implementation
private:
    ULONG m_dwRefCount;
public:
    CDBSrvFactory();
};

#endif
