// HelloAtlServer.h : Defines the ATL Server request handler class
//
#pragma once

class CHelloAtlServerHandler
    : public CRequestHandlerT<CHelloAtlServerHandler>
{

public:
    BEGIN_REPLACEMENT_METHOD_MAP(CHelloAtlServerHandler)
    REPLACEMENT_METHOD_ENTRY("NameGiven", OnNameGiven)
    REPLACEMENT_METHOD_ENTRY("Name", OnName)
    END_REPLACEMENT_METHOD_MAP()

    HTTP_CODE ValidateAndExchange()
    {
        m_name = m_HttpRequest.GetQueryParams().Lookup("name");
        return HTTP_SUCCESS;
    }

protected:
    HTTP_CODE OnNameGiven()
    {
        if (m_name.IsEmpty()) {
            return HTTP_S_FALSE;
        }

        return HTTP_SUCCESS;
    }

    HTTP_CODE OnName()
    {
        m_HttpResponse << m_name;
        return HTTP_SUCCESS;
    }

private:
    // Storage for the name in the query string
    CStringA m_name;
};

