#ifndef _DBSERVER_INCLUDE
#define _DBSERVER_INCLUDE

typedef long HRESULT;

class CDB
{
    // Interfaces
public:
    // Interface for data access
    HRESULT Read(short nTable, short nRow, LPTSTR lpszData);
    HRESULT Write(short nTable, short nRow, LPCTSTR lpszData);

    // Interface for database management
    HRESULT Create(short &nTable, LPCTSTR lpszName);
    HRESULT Delete(short nTable);

    // Interface for database information
    HRESULT GetNumTables(short &nNumTables);
    HRESULT GetTableName(short nTable, LPTSTR lpszName);
    HRESULT GetNumRows(short nTable, short &nRows);

    // Implementation
private:
    CPtrArray    m_arrTables;    // Array of pointers to CStringArray (the "database")
    CStringArray m_arrNames; // Array of table names
public:
    ~CDB();
};

#endif
