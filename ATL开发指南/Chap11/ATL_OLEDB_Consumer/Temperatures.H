// Temperatures.H : Declaration of the CTemperatures class

#ifndef __TEMPERATURES_H_
#define __TEMPERATURES_H_

class CTemperaturesAccessor
{
public:
    TCHAR m_City[27];
    unsigned long m_High;
    unsigned long m_Low;
    double m_Precipitation;
    unsigned long m_customerid;
    unsigned long m_orderid;
    DBDATE m_orderdate;
    double m_Amount;

    BEGIN_COLUMN_MAP(CTemperaturesAccessor)
    COLUMN_ENTRY(1, m_City)
    COLUMN_ENTRY(2, m_High)
    COLUMN_ENTRY(3, m_Low)
    COLUMN_ENTRY(4, m_Precipitation)
    END_COLUMN_MAP()


    // You may wish to call this function if you are inserting a record and wish to
    // initialize all the fields, if you are not going to explicitly set all of them.
    void ClearRecord()
    {
        memset(this, 0, sizeof(*this));
    }
};

class CTemperatures : public CTable<CAccessor<CTemperaturesAccessor> >
{
public:
    HRESULT Open()
    {
        HRESULT     hr;
        hr = OpenDataSource();

        if (FAILED(hr))
            return hr;

        return OpenRowset();
    }
    HRESULT OpenDataSource()
    {
        HRESULT     hr;
        CDataSource db;
        CDBPropSet  dbinit(DBPROPSET_DBINIT);
        dbinit.AddProperty(DBPROP_AUTH_PASSWORD, OLESTR("password"));
        dbinit.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
        dbinit.AddProperty(DBPROP_AUTH_USERID, OLESTR("rpatton"));
        dbinit.AddProperty(DBPROP_INIT_DATASOURCE, OLESTR(""));
        dbinit.AddProperty(DBPROP_INIT_LCID, (long)0);
        dbinit.AddProperty(DBPROP_INIT_LOCATION, OLESTR("d:\\my documents\\serverfile.txt"));
        dbinit.AddProperty(DBPROP_INIT_MODE, (long)0);
        dbinit.AddProperty(DBPROP_INIT_PROMPT, (short)2);
        dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, OLESTR(""));
        dbinit.AddProperty(DBPROP_INIT_TIMEOUT, (long)0);
        hr = db.Open(_T("ATL_OLEDB_Prov.CarusoeProv.1"), &dbinit);

        if (FAILED(hr))
            return hr;

        return m_session.Open(db);
    }
    HRESULT OpenRowset()
    {
        return CTable<CAccessor<CTemperaturesAccessor> >::Open(m_session, _T("d:\\my documents\\temperatures.txt"));
    }
    CSession    m_session;
};

#endif // __TEMPERATURES_H_
