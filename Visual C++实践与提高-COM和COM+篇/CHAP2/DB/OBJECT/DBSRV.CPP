#include "stdafx.h"
#include "..\Interface\DBsrv.h"

#define new DEBUG_NEW

// Database object
HRESULT CDB::Read(short nTable, short nRow, LPTSTR lpszData)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
    lstrcpy(lpszData, (*pTable)[nRow]);
    return NO_ERROR;
}

HRESULT CDB::Write(short nTable, short nRow, LPCTSTR lpszData)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
    pTable->SetAtGrow(nRow, lpszData);
    return NO_ERROR;
}

HRESULT CDB::Create(short &nTable, LPCTSTR lpszName)
{
    CStringArray *pTable = new CStringArray;
    nTable = m_arrTables.Add(pTable);
    m_arrNames.SetAtGrow(nTable, lpszName);
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

HRESULT CDB::GetTableName(short nTable, LPTSTR lpszName)
{
    lstrcpy(lpszName, m_arrNames[nTable]);
    return NO_ERROR;
}

HRESULT CDB::GetNumRows(short nTable, short &nRows)
{
    CStringArray *pTable;
    pTable = (CStringArray*) m_arrTables[nTable];
    return pTable->GetSize();
}

CDB::~CDB()
{
    short nNumTables;

    for (GetNumTables(nNumTables); nNumTables > 0; GetNumTables(nNumTables)) {
        Delete(nNumTables - 1);
    }
}
