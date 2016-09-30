#pragma once
#include "ForumList.h"

class ShowPostsHandler :
    public CRequestHandlerT< ShowPostsHandler >
{
public:
    ShowPostsHandler(void);
    ~ShowPostsHandler(void);

    BEGIN_REPLACEMENT_METHOD_MAP(ShowPostsHandler)
    REPLACEMENT_METHOD_ENTRY("HasPosts", OnHasPosts)
    REPLACEMENT_METHOD_ENTRY("ForumName", OnForumName)
    REPLACEMENT_METHOD_ENTRY("PostId", OnPostId)
    REPLACEMENT_METHOD_ENTRY("PostTitle", OnPostTitle)
    REPLACEMENT_METHOD_ENTRY("PostAuthor", OnPostAuthor)
    REPLACEMENT_METHOD_ENTRY("NextPost", OnNextPost)
    REPLACEMENT_METHOD_ENTRY("ForumId", OnForumId)
    REPLACEMENT_METHOD_ENTRY("InvalidForumId", OnInvalidForumId)
    REPLACEMENT_METHOD_ENTRY("HitCount", OnHitCount)
    END_REPLACEMENT_METHOD_MAP()

    HTTP_CODE ValidateAndExchange();

    HTTP_CODE OnForumName();
    HTTP_CODE OnHasPosts();
    HTTP_CODE OnPostId();
    HTTP_CODE OnPostTitle();
    HTTP_CODE OnPostAuthor();
    HTTP_CODE OnPostDate();
    HTTP_CODE OnNextPost();
    HTTP_CODE OnForumId();
    HTTP_CODE OnInvalidForumId();
    HTTP_CODE OnHitCount();
private:
    HRESULT LoadForumPosts();
    HRESULT RetrieveOrCreateSession();
    HRESULT UpdateHitCount();
    CStringA GetSessionVarName();

    HRESULT GetWordOfDay(CStringA &result);

    int m_forumId;
    int m_hits;
    ForumList m_forumList;
    CComPtr< ADODB::_Recordset > m_forumRecordset;
    CComPtr< ADODB::_Recordset > m_postsRecordset;

    CValidateContext m_validationContext;

    // BLOB Cache interface
    CComPtr< IMemoryCache > m_spMemoryCache;

    // Session state service
    CComPtr< ISessionStateService > m_spSessionStateSvc;
    CComPtr< ISession > m_spSession;
};

DECLARE_REQUEST_HANDLER("ShowPosts", ShowPostsHandler, ::ShowPostsHandler)
