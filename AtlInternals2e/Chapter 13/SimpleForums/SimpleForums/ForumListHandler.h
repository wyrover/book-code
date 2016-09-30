#pragma once
#include "ForumList.h"

class ForumListHandler :
    public CRequestHandlerT<ForumListHandler>
{
public:
    ForumListHandler(void);
public:
    virtual ~ForumListHandler(void);

public:
    // Put public members here

    // TODO: Add additional tags to the replacement method map
    BEGIN_REPLACEMENT_METHOD_MAP(ForumListHandler)
    REPLACEMENT_METHOD_ENTRY("NumForums", OnNumForums)
    REPLACEMENT_METHOD_ENTRY("MoreForums", OnMoreForums)
    REPLACEMENT_METHOD_ENTRY("NextForum", OnNextForum)
    REPLACEMENT_METHOD_ENTRY("ForumName", OnForumName)
    REPLACEMENT_METHOD_ENTRY("ForumDescription", OnForumDescription)
    REPLACEMENT_METHOD_ENTRY("LinkToForum", OnLinkToForum)
    REPLACEMENT_METHOD_ENTRY("LinkToEditForum", OnLinkToEditForum)
    END_REPLACEMENT_METHOD_MAP()

    HTTP_CODE ValidateAndExchange();

private:

    HTTP_CODE OnNumForums();
    HTTP_CODE OnMoreForums();
    HTTP_CODE OnNextForum();
    HTTP_CODE OnForumName();
    HTTP_CODE OnLinkToForum();
    HTTP_CODE OnLinkToEditForum();
    HTTP_CODE OnForumDescription();

private:

    CComPtr< ISessionStateService > m_spSessionSvc;
    ForumList m_forums;
    CComPtr< ADODB::_Recordset > m_forumsRecordSet;
};

DECLARE_REQUEST_HANDLER("ForumList", ForumListHandler, ::ForumListHandler)
