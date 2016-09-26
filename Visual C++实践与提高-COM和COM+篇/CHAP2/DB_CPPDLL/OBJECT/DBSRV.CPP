#include "stdafx.h"

#include "..\interface\DBsrv.h" //CPPtoDLL: change to include implementation

#define new DEBUG_NEW

// Database object
HRESULT CDB::Read(short nTable, short nRow, LPWSTR lpszData)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
#ifndef UNICODE
    MultiByteToWideChar(CP_ACP, 0, (*pTable)[nRow], -1, lpszData, 80);
#else
    lstrcpy(lpszData, (*pTable)[nRow]);
#endif
    return NO_ERROR;
}

HRESULT CDB::Write(short nTable, short nRow, LPCWSTR lpszData)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
#ifdef UNICODE
    pTable->SetAtGrow(nRow, lpszData);
#else
    char szData[80];
    WideCharToMultiByte(CP_ACP, 0, lpszData, -1, szData, 80, NULL, NULL);
    pTable->SetAtGrow(nRow, szData);
#endif
    return NO_ERROR;
}

HRESULT CDB::Create(short &nTable, LPCWSTR lpszName)
{
    CStringArray *pTable = new CStringArray;
    nTable = m_arrTables.Add(pTable);
#ifdef UNICODE
    m_arrNames.SetAtGrow(nTable, lpszName);
#else
    char szName[80];
    WideCharToMultiByte(CP_ACP, 0, lpszName, -1, szName, 80, NULL, NULL);
    m_arrNames.SetAtGrow(nTable, szName);
#endif
    return NO_ERROR;
}

HRESULT CDB::Delete(short nTable)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
    delete pTable;
    m_arrTables[nTable] = NULL;
    m_arrNames[nTable] = "";

    if (nTable == m_arrTables.GetSize() - 1) {
        m_arrTables.RemoveAt(nTable);
        m_arrNames.RemoveAt(nTable);
    }

    return NO_ERROR;
}

HRESULT CDB::GetNumTables(short &nNumTables)
{
    nNumTables = m_arrTables.GetSize();
    return NOERROR;
}

HRESULT CDB::GetTableName(short nTable, LPWSTR lpszName)
{
#ifndef UNICODE
    MultiByteToWideChar(CP_ACP, 0, m_arrNames[nTable], -1, lpszName, 80);
#else
    lstrcpy(lpszName, m_arrNames[nTable]);
#endif
    return NO_ERROR;
}

HRESULT CDB::GetNumRows(short nTable, short &nRows)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
    return pTable->GetSize();
}

ULONG CDB::Release()
{
    delete this;
    return 0;
}

CDB::~CDB()
{
    short nNumTables;

    for (GetNumTables(nNumTables); nNumTables > 0; GetNumTables(nNumTables)) {
        Delete(nNumTables - 1);
    }
}

