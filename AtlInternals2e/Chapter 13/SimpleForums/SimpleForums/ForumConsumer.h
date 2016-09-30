// ForumConsumer.h : Declaration of the ForumConsumer

#pragma once

// code generated on Thursday, December 22, 2005, 10:26 PM

class ForumConsumerAccessorBase
{
public:
    LONG m_id;
    TCHAR m_name[65];
    TCHAR m_description[2001];

    // The following wizard-generated data members contain status
    // values for the corresponding fields in the column map. You
    // can use these values to hold NULL values that the database
    // returns or to hold error information when the compiler returns
    // errors. See Field Status Data Members in Wizard-Generated
    // Accessors in the Visual C++ documentation for more information
    // on using these fields.
    // NOTE: You must initialize these fields before setting/inserting data!

    DBSTATUS m_dwidStatus;
    DBSTATUS m_dwnameStatus;
    DBSTATUS m_dwdescriptionStatus;

    // The following wizard-generated data members contain length
    // values for the corresponding fields in the column map.
    // NOTE: For variable-length columns, you must initialize these
    //       fields before setting/inserting data!

    DBLENGTH m_dwidLength;
    DBLENGTH m_dwnameLength;
    DBLENGTH m_dwdescriptionLength;


    void GetRowsetProperties(CDBPropSet* pPropSet)
    {
        pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
        pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
        pPropSet->AddProperty(DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL);
        pPropSet->AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
    }

    HRESULT OpenDataSource()
    {
        CDataSource _db;
        HRESULT hr;
        hr = _db.OpenFromInitializationString(L"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=Forums;Data Source=.\\SQLEXPRESS");

        if (FAILED(hr)) {
#ifdef _DEBUG
            AtlTraceErrorRecords(hr);
#endif
            return hr;
        }

        return m_session.Open(_db);
    }

    void CloseDataSource()
    {
        m_session.Close();
    }

    operator const CSession&()
    {
        return m_session;
    }

    CSession m_session;

    //DEFINE_COMMAND_EX(ForumConsumerAccessor, L" \
    //SELECT \
    //  id, \
    //  name, \
    //  description \
    //  FROM dbo.Forum")


    //// In order to fix several issues with some providers, the code below may bind
    //// columns in a different order than reported by the provider

    //BEGIN_COLUMN_MAP(ForumConsumerAccessor)
    //  COLUMN_ENTRY_LENGTH_STATUS(1, m_id, m_dwidLength, m_dwidStatus)
    //  COLUMN_ENTRY_LENGTH_STATUS(2, m_name, m_dwnameLength, m_dwnameStatus)
    //  COLUMN_ENTRY_LENGTH_STATUS(3, m_description, m_dwdescriptionLength, m_dwdescriptionStatus)
    //END_COLUMN_MAP()

//   // Added to allow for query parameters to select a single forum
//   BEGIN_PARAM_MAP(ForumConsumerAccessor)
//       SET_PARAM_TYPE(DBPARAMIO_INPUT)
//       COLUMN_ENTRY(1, m_id)
//   END_PARAM_MAP()
};

class ForumConsumerAccessor : public ForumConsumerAccessorBase
{
public:
    DEFINE_COMMAND_EX(ForumConsumerAccessor, L" \
	SELECT \
		id, \
		name, \
		description \
		FROM dbo.Forum")


    // In order to fix several issues with some providers, the code below may bind
    // columns in a different order than reported by the provider

    BEGIN_COLUMN_MAP(ForumConsumerAccessor)
    COLUMN_ENTRY_LENGTH_STATUS(1, m_id, m_dwidLength, m_dwidStatus)
    COLUMN_ENTRY_LENGTH_STATUS(2, m_name, m_dwnameLength, m_dwnameStatus)
    COLUMN_ENTRY_LENGTH_STATUS(3, m_description, m_dwdescriptionLength, m_dwdescriptionStatus)
    END_COLUMN_MAP()

    // Added to allow for query parameters to select a single forum
    BEGIN_PARAM_MAP(ForumConsumerAccessor)
    SET_PARAM_TYPE(DBPARAMIO_INPUT)
    COLUMN_ENTRY(1, m_id)
    END_PARAM_MAP()
};


class ForumConsumer : public CCommand<CAccessor<ForumConsumerAccessor> >
{
public:
    HRESULT OpenAll()
    {
        HRESULT hr;
        hr = OpenDataSource();

        if (FAILED(hr))
            return hr;

        __if_exists(GetRowsetProperties) {
            CDBPropSet propset(DBPROPSET_ROWSET);
            __if_exists(HasBookmark) {
                if (HasBookmark())
                    propset.AddProperty(DBPROP_IRowsetLocate, true);
            }
            GetRowsetProperties(&propset);
            return OpenRowset(&propset);
        }
        __if_not_exists(GetRowsetProperties) {
            __if_exists(HasBookmark) {
                if (HasBookmark()) {
                    CDBPropSet propset(DBPROPSET_ROWSET);
                    propset.AddProperty(DBPROP_IRowsetLocate, true);
                    return OpenRowset(&propset);
                }
            }
        }
        return OpenRowset();
    }

    HRESULT OpenRowset(DBPROPSET *pPropSet = NULL)
    {
        HRESULT hr = Open(m_session, NULL, pPropSet);
#ifdef _DEBUG

        if (FAILED(hr))
            AtlTraceErrorRecords(hr);

#endif
        return hr;
    }

    void CloseAll()
    {
        Close();
        ReleaseCommand();
        CloseDataSource();
    }
};

class ForumCountAccessor
{
public:
    LONG m_numForums;

    DEFINE_COMMAND_EX(ForumCountAccessor, L" \
    SELECT COUNT(id) AS numForums FROM dbo.Forum")

    BEGIN_COLUMN_MAP(ForumCountAccessor)
    COLUMN_ENTRY(1, m_numForums)
    END_COLUMN_MAP()

};

typedef CCommand<CAccessor<ForumCountAccessor> > ForumCountConsumer;
