// SimpleCSVParser.h: interface for the CSimpleCSVParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLECSVPARSER_H__0547B8AF_6921_11D3_B1D4_00E09801D33E__INCLUDED_)
#define AFX_SIMPLECSVPARSER_H__0547B8AF_6921_11D3_B1D4_00E09801D33E__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

enum dataTypes { stringType, intType, floatType, dateType };


class CColumnDefinition
{
public:
    CColumnDefinition()
    {
        m_dwColType = stringType;
        m_dwSize = 0;
        m_dwPrecision = 0;
    };
    void SetColumnData(LPCTSTR colName, DWORD dwColType, DWORD dwSize, DWORD dwPrecision)
    {
        m_colName = colName;
        m_dwColType = dwColType;
        m_dwSize = dwSize;
        m_dwPrecision = dwPrecision;
    }

    LPCTSTR GetColumnName()
    {
        return m_colName;
    };
    DWORD   GetColumnType()
    {
        return m_dwColType;
    };
    DWORD   GetColumnSize()
    {
        return m_dwSize;
    };
    DWORD   GetColumnPrecision()
    {
        return m_dwPrecision;
    };

    LPCTSTR GetColumnValue()
    {
        return m_colValue;
    };
    void    SetColumnValue(LPCTSTR colValue)
    {
        m_colValue = colValue;
    };


protected:
    CString m_colName;
    DWORD m_dwColType;
    DWORD m_dwSize;
    DWORD m_dwPrecision;
    CString m_colValue;
};

typedef void (*ForEachColumnFunc)(CColumnDefinition* pColDef);
class CSimpleCSVParser
{
public:
    BOOL GotoRow(long nIndex);
    CColumnDefinition* GetColumnDefinition(int ndx);
    long GetColumnCount();
    long GetRowCount() const;
    void CleanColumnData();

    BOOL ParseCurrentRow();
    void InitializeRowPtrs();
    void DisplayColumns();
    DWORD ColumnTypeFromString(LPTSTR strColType);
    void ParseColumns();
    void Cleanup();
    BOOL Initialize(LPCTSTR fileName);
    CSimpleCSVParser();
    virtual ~CSimpleCSVParser();

    void ForEachColumn(ForEachColumnFunc pForEachColumnFunc);

    // Movement functions
    BOOL MoveFirst();
    BOOL MoveLast();
    BOOL MoveNext();
    BOOL MovePrev();

protected:
    long m_nCurRow;
    LPBYTE m_pFirstRow;
    LPBYTE m_pLastRow;
    LPBYTE m_pCurRow;
    BOOL m_bInitialized;
    LPBYTE m_pData;
    CPtrArray m_columns;
    long    m_nRows;
    long    m_nCols;

    LPBYTE FindNextRow(LPBYTE pPos);
    LPBYTE FindPrevRow(LPBYTE pPos);

    friend class CColumnDefinition;
};

#endif // !defined(AFX_SIMPLECSVPARSER_H__0547B8AF_6921_11D3_B1D4_00E09801D33E__INCLUDED_)
