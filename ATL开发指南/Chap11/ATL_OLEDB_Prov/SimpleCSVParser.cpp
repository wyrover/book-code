// SimpleCSVParser.cpp: implementation of the CSimpleCSVParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleCSVParser.h"
#include "SimpleCSVParser.h"

#ifdef _DEBUG
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleCSVParser::CSimpleCSVParser()
{
    m_pData = NULL;
    m_bInitialized = FALSE;
    m_pFirstRow = NULL;
    m_pLastRow = NULL;
    m_pCurRow = NULL;
    m_nRows = 0;
    m_nCols = 0;
    m_nCurRow = 0;
}

CSimpleCSVParser::~CSimpleCSVParser()
{
    Cleanup();
}

// This method opens the file passed and reads all
// of the data into memory. The file is expected
// to be a CSV file with the first record containing
// column names
BOOL CSimpleCSVParser::Initialize(LPCTSTR fileName)
{
    m_bInitialized = FALSE;
    m_pFirstRow = NULL;
    m_pLastRow = NULL;
    m_pCurRow = NULL;
    m_nRows = 0;
    m_nCols = 0;
    // Prepare for Initialize to be called more than once
    Cleanup();
    CFile file(fileName, CFile::modeRead | CFile::shareDenyNone);
    // allocate a buffer large enough to hold the file Data
    m_pData = new BYTE[file.GetLength() + 1];

    if (!m_pData)
        return FALSE;

    // Initialize the buffer with -1. -1 is what I decided to use
    // as a file delimiter. When I read the contents of the file
    // into the buffer it will overwrite all of the -1's but the
    // last one.
    memset(m_pData, (unsigned char) - 1, file.GetLength() + 1);
    // read the data into the buffer
    file.Read((void*)m_pData, file.GetLength());
    ParseColumns();
    InitializeRowPtrs();
    MoveFirst();
    m_bInitialized = TRUE;
    return TRUE;
}

void DeleteColumn(CColumnDefinition* pColDef)
{
    if (pColDef)
        delete pColDef;
}

void CSimpleCSVParser::Cleanup()
{
    if (m_pData) {
        delete [] m_pData;
        m_pData = NULL;
    }

    ForEachColumn(::DeleteColumn);
    m_columns.RemoveAll();
}

// Assumption: The first row of data contains the column information in
// the following format:
//      ColName:ColType[:ColSize(for string types)][:Precision(for decimal types)]
//
// Strings have the following format:
// ColName:ColType:ColSize
//
// Ints have the following format:
// ColName:ColType
//
// Floats have the following format:
// ColName:ColType:Precision
void CSimpleCSVParser::ParseColumns()
{
    CColumnDefinition*  pColDef;
    TCHAR               buf[256];
    TCHAR               colName[256];
    DWORD               colType = stringType;
    DWORD               colSize = 0;
    DWORD               colPrecision = 0;
    LPBYTE              pCurByte = m_pData;
    int                 nStartByte = 0;
    int                 nCurByte = 0;
    enum                States { getColName, getColType, getColSize, getColPrecision };
    States              nextState = getColName;

    // Only read until we reach the end of line
    while (pCurByte) {
        switch (*pCurByte) {
        // new column definition
        case '\r':
        case ',': {
            switch (nextState) {
            case getColType:
                memset(buf, NULL, sizeof(buf));
                memcpy(buf, &m_pData[nStartByte], nCurByte - nStartByte);
                colType = ColumnTypeFromString(buf);
                ASSERT((colType == dateType) || (colType == intType));
                nStartByte = nCurByte + 1; // skip the ','
                nextState = getColName;
                break;

            case getColSize:
                memset(buf, NULL, sizeof(buf));
                memcpy(buf, &m_pData[nStartByte], nCurByte - nStartByte);
                colSize = atol(buf);
                nStartByte = nCurByte + 1; // skip the ','
                nextState = getColName;
                break;

            case getColPrecision:
                memset(buf, NULL, sizeof(buf));
                memcpy(buf, &m_pData[nStartByte], nCurByte - nStartByte);
                colPrecision = atol(buf);
                nStartByte = nCurByte + 1; // skip the ','
                nextState = getColName;
                break;
            }

            pColDef = new CColumnDefinition();
            pColDef->SetColumnData(colName, colType, colSize, colPrecision);
            m_columns.Add((void*)pColDef);
            memset(colName, NULL, sizeof(colName));
            colType = stringType;
            colSize = 0;
            colPrecision = 0;
            break;
        }

        // column attribute
        case ':': {
            switch (nextState) {
            case getColName:
                memset(colName, NULL, sizeof(colName));
                memcpy(colName, &m_pData[nStartByte], nCurByte - nStartByte);
                nStartByte = nCurByte + 1; // skip the ':'
                nextState = getColType;
                break;

            case getColType:
                memset(buf, NULL, sizeof(buf));
                memcpy(buf, &m_pData[nStartByte], nCurByte - nStartByte);
                colType = ColumnTypeFromString(buf);
                ASSERT((colType == stringType) || (colType == floatType));
                nStartByte = nCurByte + 1; // skip the ':'

                if (colType == stringType)
                    nextState = getColSize;
                else
                    nextState = getColPrecision;

                break;

            case getColSize:
            case getColPrecision:
                ASSERT(FALSE);
                break;
            }

            break;
        }
        }

        // End of the line
        if (*pCurByte == '\r')
            break;

        nCurByte++;
        pCurByte++;
    }

    m_nCols = m_columns.GetSize();
    ASSERT(m_nCols > 0);
}

DWORD CSimpleCSVParser::ColumnTypeFromString(LPTSTR strColType)
{
    CString colType = strColType;

    if (colType == CString("integer"))
        return intType;

    if (colType == CString("float"))
        return floatType;

    if (colType == CString("date"))
        return dateType;

    // by default assume string data type
    return stringType;
}


void DisplayColumn(CColumnDefinition* pColDef)
{
    if (pColDef)
        AfxMessageBox(pColDef->GetColumnName());
}

void CSimpleCSVParser::DisplayColumns()
{
    ForEachColumn(::DisplayColumn);
}

void CSimpleCSVParser::InitializeRowPtrs()
{
    m_pFirstRow = NULL;
    m_pLastRow = NULL;
    m_pCurRow = NULL;
    // The First Row will be the 2nd row in the file since
    // the first row contains column definitions
    m_pFirstRow = FindNextRow(m_pData);
    m_pCurRow = m_pFirstRow;
    LPBYTE pHoldPos = m_pFirstRow;
    m_nRows = 0;

    while (pHoldPos != NULL) {
        m_pLastRow = pHoldPos;
        pHoldPos = FindNextRow(pHoldPos);
        m_nRows++;
    }
}

LPBYTE CSimpleCSVParser::FindPrevRow(LPBYTE pPos)
{
    ASSERT(pPos);

    if (!pPos)
        return NULL;

    ASSERT(m_pFirstRow != NULL);

    if (m_pFirstRow == NULL)
        return NULL;

    if (pPos == m_pFirstRow)
        return m_pFirstRow;

    BOOL bInPrevRow = FALSE;
    LPBYTE p = pPos;

    while (p != m_pFirstRow) {
        if (*p == '\r' || *p == '\n') {
            if (bInPrevRow) {
                p++;
                break;
            } else {
                while (*p == '\r' || *p == '\n') {
                    p--;
                }

                bInPrevRow = TRUE;
            }
        }

        p--;
    }

    return p;
}

LPBYTE CSimpleCSVParser::FindNextRow(LPBYTE pPos)
{
    ASSERT(pPos);

    if (!pPos)
        return NULL;

    LPBYTE p = pPos;
    unsigned char endOfFile = (unsigned char) - 1;

    while (*p != endOfFile) {
        if (*p == '\r' || *p == '\n') {
            while (*p == '\r' || *p == '\n') {
                p++;
            }

            return ((*p != endOfFile) ? p : NULL);
        }

        p++;
    }

    return NULL;
}

void CSimpleCSVParser::ForEachColumn(ForEachColumnFunc pForEachColumnFunc)
{
    if (!m_bInitialized)
        return;

    for (int i = 0; i < m_columns.GetSize(); i++) {
        CColumnDefinition* pColDef = (CColumnDefinition*) m_columns[i];
        (*pForEachColumnFunc)(pColDef);
    }
}

BOOL CSimpleCSVParser::ParseCurrentRow()
{
    CleanColumnData();
    ASSERT(m_pCurRow != NULL);

    if (m_pCurRow == NULL)
        return FALSE;

    BOOL bEndOfRow = FALSE;
    CString colValue;
    LPBYTE pStartPos = m_pCurRow;
    LPBYTE pEndPos = m_pCurRow;
    int curCol = 0;

    while (pEndPos) {
        switch (*pEndPos) {
        // new column definition
        case '\r':
        case 255: // end of file
            bEndOfRow = TRUE;
            colValue.Empty();

            if (pEndPos > pStartPos) {
                LPTSTR pBuffer = colValue.GetBuffer(pEndPos - pStartPos);
                _tcsncpy(pBuffer, (const char*)pStartPos, pEndPos - pStartPos);
                colValue.ReleaseBuffer();
                ((CColumnDefinition*)m_columns.GetAt(curCol))->SetColumnValue(colValue);
            }

            curCol++;
            break;

        case ',':
            colValue.Empty();

            if (pEndPos > pStartPos) {
                LPTSTR pBuffer = colValue.GetBuffer(pEndPos - pStartPos);
                _tcsncpy(pBuffer, (const char*)pStartPos, pEndPos - pStartPos);
                colValue.ReleaseBuffer();
                ((CColumnDefinition*)m_columns.GetAt(curCol))->SetColumnValue(colValue);
            }

            curCol++;
            pEndPos++;
            pStartPos = pEndPos;
            break;
        }

        if (bEndOfRow)
            break;

        pEndPos++;
    }

    return TRUE;
}

void EmptyData(CColumnDefinition* pColDef)
{
    if (pColDef)
        pColDef->SetColumnValue(_T(""));
}

void CSimpleCSVParser::CleanColumnData()
{
    ForEachColumn(EmptyData);
}

BOOL CSimpleCSVParser::MoveFirst()
{
    ASSERT(m_pFirstRow);

    if (!m_pFirstRow)
        return FALSE;

    m_nCurRow = 1;
    m_pCurRow = m_pFirstRow;
    return ParseCurrentRow();
}

BOOL CSimpleCSVParser::MoveNext()
{
    ASSERT(m_pCurRow);

    if (!m_pCurRow)
        return FALSE;

    ASSERT(m_pLastRow);

    if (!m_pLastRow)
        return FALSE;

    if (m_pCurRow == m_pLastRow)
        return FALSE;

    m_pCurRow = FindNextRow(m_pCurRow);

    if (ParseCurrentRow())
        m_nCurRow++;
    else
        return FALSE;

    return TRUE;
}

BOOL CSimpleCSVParser::MovePrev()
{
    ASSERT(m_pCurRow);

    if (!m_pCurRow)
        return FALSE;

    ASSERT(m_pFirstRow);

    if (!m_pFirstRow)
        return FALSE;

    if (m_pCurRow == m_pFirstRow)
        return FALSE;

    m_pCurRow = FindPrevRow(m_pCurRow);

    if (ParseCurrentRow())
        m_nCurRow--;
    else
        return FALSE;

    return TRUE;
}

BOOL CSimpleCSVParser::MoveLast()
{
    ASSERT(m_pLastRow);

    if (!m_pLastRow)
        return FALSE;

    m_pCurRow = m_pLastRow;

    if (ParseCurrentRow())
        m_nCurRow = m_nRows;
    else
        return FALSE;

    return TRUE;
}

long CSimpleCSVParser::GetColumnCount()
{
    return (long)m_columns.GetSize();
}

long CSimpleCSVParser::GetRowCount() const
{
    return (long)m_nRows;
}

CColumnDefinition* CSimpleCSVParser::GetColumnDefinition(int ndx)
{
    return (CColumnDefinition*)m_columns[ndx];
}

BOOL CSimpleCSVParser::GotoRow(long nIndex)
{
    if (nIndex < 0)
        return FALSE;

    if (nIndex >= m_nRows)
        return FALSE;

    if (nIndex == 0)
        MoveFirst();
    else if (nIndex == (m_nRows - 1))
        MoveLast();
    else if (nIndex > m_nCurRow) {
        while (nIndex > m_nCurRow) {
            MoveNext();
        }
    } else if (nIndex < m_nCurRow) {
        while (nIndex < m_nCurRow) {
            MovePrev();
        }
    }

    return TRUE;
}
