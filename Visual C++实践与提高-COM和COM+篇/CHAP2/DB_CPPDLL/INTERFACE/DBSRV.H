#ifndef _DBSERVER_INCLUDE
#define _DBSERVER_INCLUDE

typedef long HRESULT;

#define DEF_EXPORT _declspec(dllexport)

class CDB
{
    // Interfaces
public:
    // Interface for data access
    HRESULT DEF_EXPORT Read(short nTable, short nRow, LPWSTR lpszData);
    HRESULT DEF_EXPORT Write(short nTable, short nRow, LPCWSTR lpszData);

    // Interface for database management
    HRESULT DEF_EXPORT Create(short &nTable, LPCWSTR lpszName);
    HRESULT DEF_EXPORT Delete(short nTable);

    // Interfase para obtenber informacion sobre la base de datos
    HRESULT DEF_EXPORT GetNumTables(short &nNumTables);
    HRESULT DEF_EXPORT GetTableName(short nTable, LPWSTR lpszName);
    HRESULT DEF_EXPORT GetNumRows(short nTable, short &nRows);

    ULONG DEF_EXPORT Release(); //CPPTOCOM: need to free an object in the DLL, since it was allocated here

    // Implementation
private:
    CPtrArray m_arrTables;   // Array of pointers to CStringArray (the "database")
    CStringArray m_arrNames; // Array of table names
public:
    ~CDB();
};

class CDBSrvFactory
{
    // Interface
public:
    HRESULT DEF_EXPORT CreateDB(CDB** ppObject);
    ULONG   DEF_EXPORT Release();
};

HRESULT DEF_EXPORT DllGetClassFactoryObject(CDBSrvFactory ** ppObject);

#endif
