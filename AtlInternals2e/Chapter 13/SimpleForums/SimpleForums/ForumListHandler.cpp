#include "StdAfx.h"
#include "ForumListHandler.h"

using namespace std;

ForumListHandler::ForumListHandler(void)
{
}

ForumListHandler::~ForumListHandler(void)
{
}

HTTP_CODE ForumListHandler::ValidateAndExchange()
{
    // TODO: Put all initialization and validation code here
    // Set the content-type
    m_HttpResponse.SetContentType("text/html");

    // uncomment the service initialization(s) if you want to use
    // a service that was generated with your ISAPI extension

    // Get the ISessionStateService from the ISAPI extension
    if (FAILED(m_spServiceProvider->QueryService(__uuidof(ISessionStateService),
               &m_spSessionSvc))) {
        return HTTP_FAIL;
    }

    // Get the IFileCache service from the ISAPI extension
//      if (FAILED(m_spServiceProvider->QueryService(__uuidof(IFileCache),
//                      &m_spFileCache)))
//          return HTTP_FAIL;
    AS_HR(m_forums.Open());
    AS_HR(m_forums.ReadAllForums(&m_forumsRecordSet));
    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnNumForums()
{
    int numForums;
    AS_HR(m_forums.GetNumForums(&numForums));
    m_HttpResponse << numForums;
    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnMoreForums()
{
    VARIANT_BOOL endOfRecordSet;
    AS_HR(m_forumsRecordSet->get_adoEOF(&endOfRecordSet));

    if (endOfRecordSet == VARIANT_TRUE) {
        return HTTP_S_FALSE;
    }

    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnNextForum()
{
    if (OnMoreForums() == HTTP_SUCCESS) {
        AS_HR(m_forumsRecordSet->MoveNext());
    }

    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnForumName()
{
    CComBSTR name;
    AS_HR(m_forums.GetCurrentForumName(m_forumsRecordSet, &name));
    m_HttpResponse << CW2A(name);
    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnLinkToForum()
{
    long id;
    AS_HR(m_forums.GetCurrentForumId(m_forumsRecordSet, &id));
    m_HttpResponse << "showposts.srf?forumid=" << id;
    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnLinkToEditForum()
{
    long id;
    AS_HR(m_forums.GetCurrentForumId(m_forumsRecordSet, &id));
    m_HttpResponse << "editforum.srf?forumid=" << id;
    return HTTP_SUCCESS;
}

HTTP_CODE ForumListHandler::OnForumDescription()
{
    CComBSTR description;
    AS_HR(m_forums.GetCurrentForumDescription(m_forumsRecordSet, &description));
    m_HttpResponse << CW2A(description);
    return HTTP_SUCCESS;
}
