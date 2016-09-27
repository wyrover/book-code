// CarusoeProvRS.h : Declaration of the CCarusoeProvRowset
#ifndef __CCarusoeProvRowset_H_
#define __CCarusoeProvRowset_H_
#include "resource.h"       // main symbols
#include "SimpleCSVParser.h"
#include "comdef.h"

class CCarusoeProvTextFile
{
public:

    CCarusoeProvTextFile()
    {
        m_cacheNdxStart = -1;
        m_cacheRows = 0;
        m_nRowSize = 0;
        m_pData = NULL;
    }
    ~CCarusoeProvTextFile()
    {
        RemoveAll();
    }

    void LoadCache(long nStart)
    {
        if (m_pData == NULL)
            return;

        memset(m_pData, NULL, (int)(m_cacheRows * (m_nRowSize + 1)*sizeof(TCHAR)));

        if (nStart < 0 || nStart > m_parser.GetRowCount())
            return;

        m_parser.GotoRow(nStart);
        long rows = min(m_cacheRows, m_parser.GetRowCount() - nStart);
        LPBYTE pLoc = m_pData;
        long tmpLong;
        double tmpDouble;
        CString tmpString;

        for (long i = 0; i < rows; i++) {
            for (long j = 0; j < m_parser.GetColumnCount(); j++) {
                CColumnDefinition* pColDef = m_parser.GetColumnDefinition(j);

                if (pColDef) {
                    switch (pColDef->GetColumnType()) {
                    case stringType:
                        _tcscpy((LPTSTR) pLoc, pColDef->GetColumnValue());
                        pLoc += pColDef->GetColumnSize() + sizeof(TCHAR);
                        break;

                    case intType:
                        tmpLong = atol(pColDef->GetColumnValue());
                        memcpy(pLoc, (void*)&tmpLong, sizeof(long));
                        pLoc += sizeof(long);
                        break;

                    case floatType:
                        tmpDouble = atof(pColDef->GetColumnValue());
                        memcpy(pLoc, &tmpDouble, sizeof(double));
                        pLoc += sizeof(double);
                        break;

                    case dateType:
                        tmpString = pColDef->GetColumnValue();
                        DBDATE dbDate;
                        ConvertStringToDate(tmpString.GetBuffer(0), &dbDate);
                        memcpy(pLoc, &dbDate, sizeof(DBDATE));
                        pLoc += sizeof(DBDATE);
                        break;
                    }
                }
            }

            m_parser.MoveNext();
        }

        m_cacheNdxStart = nStart;
    }

    // Assumption: The Parser ALWAYS gives us date in MM/DD/YYYY format
    void ConvertStringToDate(LPTSTR str, DBDATE* pDBDate) const
    {
        char strSep[2];
        strcpy(strSep, "/");
        // Extract First Item
        LPTSTR  token;
        unsigned short number;
        token = strtok(str, strSep);

        if (token) {
            number = (unsigned short) atoi(token);
            pDBDate->month = number;
        }

        token = strtok(NULL, strSep);

        if (token) {
            number = (unsigned short) atoi(token);
            pDBDate->day = number;
        }

        token = strtok(NULL, strSep);

        if (token) {
            number = (unsigned short) atoi(token);
            pDBDate->year = number;
        }
    }

    HRESULT Initialize(CComBSTR& strTableLocation, CSimpleArray<ATLCOLUMNINFO>& rgColInfo)
    {
        USES_CONVERSION;

        if (!m_parser.Initialize(_bstr_t(strTableLocation.m_str)))
            return E_FAIL;

        // Calculate the row size
        m_nRowSize = 0;
        int offset = 0;

        for (long i = 0; i < m_parser.GetColumnCount(); i++) {
            CColumnDefinition* pColDef = m_parser.GetColumnDefinition(i);

            if (pColDef) {
                ATLCOLUMNINFO colInfo;
                memset(&colInfo, 0, sizeof(ATLCOLUMNINFO));
                colInfo.pwszName = ::SysAllocString(T2OLE(pColDef->GetColumnName()));
                colInfo.iOrdinal = i + 1;
                colInfo.dwFlags = NULL;
                colInfo.bPrecision = 0xFF;
                colInfo.bScale = 0xFF;
                colInfo.columnid.uName.pwszName = colInfo.pwszName;

                switch (pColDef->GetColumnType()) {
                case stringType:
                    colInfo.wType = DBTYPE_STR;
                    m_nRowSize += pColDef->GetColumnSize() + sizeof(TCHAR);
                    colInfo.ulColumnSize = pColDef->GetColumnSize() + sizeof(TCHAR);
                    break;

                case intType:
                    colInfo.wType = DBTYPE_I4;
                    m_nRowSize += sizeof(long);
                    colInfo.ulColumnSize = sizeof(long);
                    break;

                case floatType:
                    colInfo.wType = DBTYPE_R8;
                    colInfo.bPrecision = (BYTE) pColDef->GetColumnPrecision();
                    m_nRowSize += sizeof(double);
                    colInfo.ulColumnSize = sizeof(double);
                    break;

                case dateType:
                    colInfo.wType = DBTYPE_DBDATE;
                    m_nRowSize += sizeof(DBDATE);
                    colInfo.ulColumnSize = sizeof(DBDATE);
                    break;
                }

                colInfo.cbOffset = offset;
                offset += colInfo.ulColumnSize;
                rgColInfo.Add(colInfo);
            }
        }

        if (m_nRowSize > 0) {
            // calculate to use 500K for cache space
            m_cacheRows = 500000 / m_nRowSize;
            m_pData = new BYTE[(int)(m_cacheRows * (m_nRowSize + 1))];

            if (m_pData)
                memset(m_pData, NULL, (int)(m_cacheRows * (m_nRowSize + 1)*sizeof(TCHAR)));
        } else
            return E_FAIL;

        return S_OK;
    }

    template <class T>
    static ATLCOLUMNINFO* GetColumnInfo(T* pT, ULONG* pcCols)
    {
        CComQIPtr<ICommand> spCommand = pT->GetUnknown();

        if (spCommand == NULL) {
            if (pcCols != NULL)
                *pcCols = pT->m_rgColInfo.GetSize();

            return pT->m_rgColInfo.m_aT;
        }

        CComPtr<IRowset> pRowset;

        if (pT->m_rgColInfo.m_aT == NULL) {
            LONG cRows;
            HRESULT hr = spCommand->Execute(NULL, IID_IRowset, NULL, &cRows, (IUnknown**)&pRowset);
        }

        if (pcCols != NULL)
            *pcCols = pT->m_rgColInfo.GetSize();

        return pT->m_rgColInfo.m_aT;
    }

    LONG GetSize() const
    {
        return (m_parser.GetRowCount());
    }

    BYTE& operator[](long nIndex)
    {
        long lowRange = m_cacheNdxStart;
        long highRange = m_cacheNdxStart + m_cacheRows - 1;

        // If the row isn't in the cache then reload the cache
        if ((m_cacheNdxStart == -1) || (nIndex < lowRange || nIndex > highRange)) {
            LoadCache(nIndex);
            m_cacheNdxStart = nIndex;
        }

        return  m_pData[(nIndex - m_cacheNdxStart) * (m_nRowSize)];
    }

    void RemoveAll()
    {
        if (m_pData) {
            delete [] m_pData;
            m_pData = NULL;
        }
    }

protected:
    CSimpleCSVParser m_parser;
    LPBYTE m_pData;

    long m_cacheNdxStart;
    long m_cacheRows;
    long m_nRowSize;
};

// CCarusoeProvCommand
class ATL_NO_VTABLE CCarusoeProvCommand :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IAccessorImpl<CCarusoeProvCommand>,
    public ICommandTextImpl<CCarusoeProvCommand>,
    public ICommandPropertiesImpl<CCarusoeProvCommand>,
    public IObjectWithSiteImpl<CCarusoeProvCommand>,
    public IConvertTypeImpl<CCarusoeProvCommand>,
    public IColumnsInfoImpl<CCarusoeProvCommand>
{
public:
    BEGIN_COM_MAP(CCarusoeProvCommand)
    COM_INTERFACE_ENTRY(ICommand)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY(IAccessor)
    COM_INTERFACE_ENTRY(ICommandProperties)
    COM_INTERFACE_ENTRY2(ICommandText, ICommand)
    COM_INTERFACE_ENTRY(IColumnsInfo)
    COM_INTERFACE_ENTRY(IConvertType)
    END_COM_MAP()
// ICommand
public:
    HRESULT FinalConstruct()
    {
        HRESULT hr = CConvertHelper::FinalConstruct();

        if (FAILED(hr))
            return hr;

        hr = IAccessorImpl<CCarusoeProvCommand>::FinalConstruct();

        if (FAILED(hr))
            return hr;

        return CUtlProps<CCarusoeProvCommand>::FInit();
    }
    void FinalRelease()
    {
        IAccessorImpl<CCarusoeProvCommand>::FinalRelease();
    }
    HRESULT WINAPI Execute(IUnknown * pUnkOuter, REFIID riid, DBPARAMS * pParams,
                           LONG * pcRowsAffected, IUnknown ** ppRowset);
    static ATLCOLUMNINFO* GetColumnInfo(CCarusoeProvCommand* pv, ULONG* pcInfo)
    {
        return CCarusoeProvTextFile::GetColumnInfo(pv, pcInfo);
    }
    BEGIN_PROPSET_MAP(CCarusoeProvCommand)
    BEGIN_PROPERTY_SET(DBPROPSET_ROWSET)
    PROPERTY_INFO_ENTRY(IAccessor)
    PROPERTY_INFO_ENTRY(IColumnsInfo)
    PROPERTY_INFO_ENTRY(IConvertType)
    PROPERTY_INFO_ENTRY(IRowset)
    PROPERTY_INFO_ENTRY(IRowsetIdentity)
    PROPERTY_INFO_ENTRY(IRowsetInfo)
    PROPERTY_INFO_ENTRY(IRowsetLocate)
    PROPERTY_INFO_ENTRY(BOOKMARKS)
    PROPERTY_INFO_ENTRY(BOOKMARKSKIPPED)
    PROPERTY_INFO_ENTRY(BOOKMARKTYPE)
    PROPERTY_INFO_ENTRY(CANFETCHBACKWARDS)
    PROPERTY_INFO_ENTRY(CANHOLDROWS)
    PROPERTY_INFO_ENTRY(CANSCROLLBACKWARDS)
    PROPERTY_INFO_ENTRY(LITERALBOOKMARKS)
    PROPERTY_INFO_ENTRY(ORDEREDBOOKMARKS)
    END_PROPERTY_SET(DBPROPSET_ROWSET)
    END_PROPSET_MAP()
    CSimpleArray<ATLCOLUMNINFO>     m_rgColInfo;
};
class CCarusoeProvRowset
    : public CRowsetImpl< CCarusoeProvRowset, CCarusoeProvTextFile, CCarusoeProvCommand, CCarusoeProvTextFile>
{
public:
    HRESULT Execute(DBPARAMS * pParams, LONG* pcRowsAffected)
    {
        USES_CONVERSION;
        ObjectLock lock(this);

        if (m_strCommandText.Length() <= 0)
            return E_FAIL;

        HRESULT hr = m_rgRowData.Initialize(m_strCommandText, m_rgColInfo);

        if (pcRowsAffected != NULL)
            *pcRowsAffected = 1;

        return hr;
    }
    CSimpleArray<ATLCOLUMNINFO>     m_rgColInfo;
};
#endif //__CCarusoeProvRowset_H_
