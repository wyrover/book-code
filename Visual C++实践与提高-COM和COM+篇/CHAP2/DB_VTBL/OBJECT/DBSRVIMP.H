#ifndef _DBSERVERIMP_INCLUDE
#define _DBSERVERIMP_INCLUDE

#include "..\interface\dbsrv.h"

typedef long HRESULT;

class CDB : public IDB
{
    // Interfaces
public:
    // Interface for data access
    HRESULT Read(short nTable, short nRow, LPWSTR lpszData);
    HRESULT Write(short nTable, short nRow, LPCWSTR lpszData);

    // Interface for database management
    HRESULT Create(short &nTable, LPCWSTR lpszName);
    HRESULT Delete(short nTable);

    // Interfase para obtenber informacion sobre la base de datos
    HRESULT GetNumTables(short &nNumTables);
    HRESULT GetTableName(short nTable, LPWSTR lpszName);
    HRESULT GetNumRows(short nTable, short &nRows);

    ULONG Release(); //CPPTOCOM: need to free an object in the DLL, since it was allocated here

    // Implementation
private:
    CPtrArray m_arrTables;   // Array of pointers to CStringArray (the "database")
    CStringArray m_arrNames; // Array of table names
public:
    ~CDB();
};

class CDBSrvFactory : public IDBSrvFactory
{
    // Interface
public:
    HRESULT CreateDB(IDB** ppObject);
    ULONG   Release();
};

#endif
