#ifndef _DBSERVER_INCLUDE
#define _DBSERVER_INCLUDE

typedef long HRESULT;

#define DEF_EXPORT __declspec(dllexport)

DEFINE_GUID(CLSID_DBSAMPLE, 0x30df3430, 0x266, 0x11cf, 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed);
// {30DF3430-0266-11cf-BAA6-00AA003E0EED}

DEFINE_GUID(IID_IDB, 0x30df3432, 0x266, 0x11cf, 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed);
// {30DF3432-0266-11cf-BAA6-00AA003E0EED}

DEFINE_GUID(IID_IDBAccess, 0x30df3433, 0x266, 0x11cf, 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed);
// {30DF3433-0266-11cf-BAA6-00AA003E0EED}

DEFINE_GUID(IID_IDBManage, 0x30df3434, 0x266, 0x11cf, 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed);
// {30DF3434-0266-11cf-BAA6-00AA003E0EED}

DEFINE_GUID(IID_IDBInfo, 0x30df3435, 0x266, 0x11cf, 0xba, 0xa6, 0x0, 0xaa, 0x0, 0x3e, 0xe, 0xed);
// {30DF3435-0266-11cf-BAA6-00AA003E0EED}

class IDB : public IUnknown
{
    // Interfaces
public:
    // Interface for data access
    virtual HRESULT _stdcall Read(short nTable, short nRow, LPWSTR lpszData) = 0;
    virtual HRESULT _stdcall Write(short nTable, short nRow, LPCWSTR lpszData) = 0;

    // Interface for database management
    virtual HRESULT _stdcall Create(short &nTable, LPCWSTR lpszName) = 0;
    virtual HRESULT _stdcall Delete(short nTable) = 0;

    // Interfase para obtenber informacion sobre la base de datos
    virtual HRESULT _stdcall GetNumTables(short &nNumTables) = 0;
    virtual HRESULT _stdcall GetTableName(short nTable, LPWSTR lpszName) = 0;
    virtual HRESULT _stdcall GetNumRows(short nTable, short &nRows) = 0;

    //virtual ULONG Release() =0;
};

class IDBAccess : public IUnknown
{
public:
    // Interface for data access
    virtual HRESULT _stdcall Read(short nTable, short nRow, LPWSTR lpszData) = 0;
    virtual HRESULT _stdcall Write(short nTable, short nRow, LPCWSTR lpszData) = 0;
};

class IDBManage : public IUnknown
{
    // Interface for database management
public:
    virtual HRESULT _stdcall Create(short &nTable, LPCWSTR lpszName) = 0;
    virtual HRESULT _stdcall Delete(short nTable) = 0;
};

class IDBInfo : public IUnknown
{
    // Interfase para obtenber informacion sobre la base de datos
public:
    virtual HRESULT _stdcall GetNumTables(short &nNumTables) = 0;
    virtual HRESULT _stdcall GetTableName(short nTable, LPWSTR lpszName) = 0;
    virtual HRESULT _stdcall GetNumRows(short nTable, short &nRows) = 0;
};

#endif
