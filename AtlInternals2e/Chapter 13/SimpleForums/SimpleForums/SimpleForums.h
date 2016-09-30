// SimpleForums.h : Defines the ATL Server request handler class
//
#pragma once

class CSimpleForumsHandler
    : public CRequestHandlerT<CSimpleForumsHandler>
{
private:
    // Put private members here
    // uncomment the service declaration(s) if you want to use
    // a service that was generated with your ISAPI extension

    // Session service support
//  CComPtr<ISessionStateService> m_spSessionSvc;
//  CComPtr<ISession> m_spSession;

    // File cache support
//  CComPtr<IFileCache> m_spFileCache;

protected:
    // Put protected members here

public:
    // Put public members here

    // TODO: Add additional tags to the replacement method map
    BEGIN_REPLACEMENT_METHOD_MAP(CSimpleForumsHandler)
    REPLACEMENT_METHOD_ENTRY("Hello", OnHello)
    END_REPLACEMENT_METHOD_MAP()

    HTTP_CODE ValidateAndExchange()
    {
        // TODO: Put all initialization and validation code here
        // Set the content-type
        m_HttpResponse.SetContentType("text/html");
        // uncomment the service initialization(s) if you want to use
        // a service that was generated with your ISAPI extension
        // Get the ISessionStateService from the ISAPI extension
//      if (FAILED(m_spServiceProvider->QueryService(__uuidof(ISessionStateService),
//                      &m_spSessionSvc)))
//          return HTTP_FAIL;
        // Get the IFileCache service from the ISAPI extension
//      if (FAILED(m_spServiceProvider->QueryService(__uuidof(IFileCache),
//                      &m_spFileCache)))
//          return HTTP_FAIL;
        // Uncomment the following code to retrieve a data source
        // connection from the data source cache. Replace connection_name
        // with a string used to identify the connection and
        // connection_string with an OLEDB connection string
        // which is valid for your data source. This code assumes that
        // the service provider pointed to by m_spServiceProvider
        // can provide an IDataSourceCache pointer to a data source
        // cache service (usually implemented in the ISAPI extension).
//      CDataConnection dc;
//      if (S_OK != GetDataSource(  m_spServiceProvider,
//                                  _T("connection_name"),
//                                  L"connection_string",
//                                  &dc ))
//          return HTTP_FAIL;
        return HTTP_SUCCESS;
    }

protected:
    // Here is an example of how to use a replacement tag with the stencil processor
    HTTP_CODE OnHello(void)
    {
        m_HttpResponse << "Hello World!";
        return HTTP_SUCCESS;
    }
}; // class CSimpleForumsHandler
