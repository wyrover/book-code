#include "StdAfx.h"
#include "ShowPostsHandler.h"
#include "hr.h"

ShowPostsHandler::ShowPostsHandler(void)
    : m_validationContext(CValidateContext::ATL_EMPTY_PARAMS_ARE_FAILURES)
{
}

ShowPostsHandler::~ShowPostsHandler(void)
{
}

HTTP_CODE ShowPostsHandler::ValidateAndExchange()
{
    HRESULT hr;
    hr = m_spServiceProvider->QueryService(__uuidof(IMemoryCache), &m_spMemoryCache);

    if (FAILED(hr)) return HTTP_FAIL;

    AS_HR(m_spServiceProvider->QueryService(__uuidof(ISessionStateService),
                                            &m_spSessionStateSvc));
    m_forumId = -1;
    AS_HR(m_forumList.Open());
    m_HttpRequest.GetQueryParams().Exchange("forumid", &m_forumId, &m_validationContext);
    AS_HR(LoadForumPosts());
    AS_HR(RetrieveOrCreateSession());
    AS_HR(UpdateHitCount());
    m_HttpResponse.SetContentType("text/html");
    return HTTP_SUCCESS;
}

HRESULT ShowPostsHandler::LoadForumPosts()
{
    if (m_forumId != - 1) {
        HR(m_forumList.ReadOneForum(m_forumId, &m_forumRecordset));
        bool containsData;
        HR(m_forumList.ContainsForumData(m_forumRecordset, &containsData));

        if (!containsData) {
            m_validationContext.AddResult("forumid", VALIDATION_E_FAIL);
        } else {
            HR(m_forumList.ReadForumPosts(m_forumId, &m_postsRecordset));
        }
    }

    return S_OK;
}

HTTP_CODE ShowPostsHandler::OnInvalidForumId()
{
    if (m_forumId != -1) {
        return HTTP_S_FALSE;
    }

    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnForumId()
{
    m_HttpResponse << m_forumId;
    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnForumName()
{
    CComBSTR name;
    AS_HR(m_forumList.GetCurrentForumName(m_forumRecordset, &name));
    m_HttpResponse << CW2A(name);
    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnHasPosts()
{
    bool hasPosts;
    AS_HR(m_forumList.ContainsPostData(m_postsRecordset, &hasPosts));
    return hasPosts ? HTTP_SUCCESS : HTTP_S_FALSE;
}

HTTP_CODE ShowPostsHandler::OnNextPost()
{
    if (OnHasPosts() == HTTP_SUCCESS) {
        AS_HR(m_postsRecordset->MoveNext());
    }

    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnPostId()
{
    long postId;
    AS_HR(m_forumList.GetCurrentPostId(m_postsRecordset, &postId));
    m_HttpResponse << postId;
    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnPostTitle()
{
    CComBSTR title;
    AS_HR(m_forumList.GetCurrentPostTitle(m_postsRecordset, &title));
    m_HttpResponse << CW2A(title);
    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnPostAuthor()
{
    CComBSTR author;
    AS_HR(m_forumList.GetCurrentPostPoster(m_postsRecordset, &author));
    m_HttpResponse << CW2A(author);
    return HTTP_SUCCESS;
}

HTTP_CODE ShowPostsHandler::OnHitCount()
{
    m_HttpResponse << m_hits;
    return HTTP_SUCCESS;
}

HRESULT ShowPostsHandler::RetrieveOrCreateSession()
{
    CStringA sessionId;
    m_HttpRequest.GetSessionCookie().GetValue(sessionId);

    if (sessionId.GetLength() == 0) {
        // No session yet, create one
        const size_t nCharacters = 64;
        CHAR szID[nCharacters + 1];
        szID[0] = 0;
        DWORD dwCharacters = nCharacters;
        HR(m_spSessionStateSvc->CreateNewSession(szID, &dwCharacters, &m_spSession));
        CSessionCookie theSessionCookie(szID);
        m_HttpResponse.AppendCookie(&theSessionCookie);
    } else {
        // Retrieve existing session
        HR(m_spSessionStateSvc->GetSession(sessionId, &m_spSession));
    }

    return S_OK;
}

HRESULT ShowPostsHandler::UpdateHitCount()
{
    CStringA sessionVarName = GetSessionVarName();
    CComVariant hits;

    if (FAILED(m_spSession->GetVariable(sessionVarName, &hits))) {
        // No such session variable. Gotta love nice specific HRESULTS
        hits = CComVariant(0, VT_I4);
    }

    m_hits = ++hits.lVal;
    HR(m_spSession->SetVariable(sessionVarName, hits));
    return S_OK;
}

CStringA ShowPostsHandler::GetSessionVarName()
{
    CStringA sessionVarName;
    sessionVarName.Format("forum-%d-hits", m_forumId);
    return sessionVarName;
}

HRESULT ShowPostsHandler::GetWordOfDay(CStringA &result)
{
    HRESULT hr;
    HCACHEITEM hItem;
    hr = m_spMemoryCache->LookupEntry("WordOfDay", &hItem);

    if (SUCCEEDED(hr)) {
        // Found it, pull out the entry
        void *pData;
        DWORD dataLength;
        hr = m_spMemoryCache->GetData(hItem, &pData, &dataLength);

        if (SUCCEEDED(hr)) {
            result = CStringA(static_cast< char * >(pData), dataLength);
        }

        m_spMemoryCache->ReleaseEntry(hItem);
    } else if (hr == E_FAIL) {
        // Not in cache
        char *wordOfTheDay = new char[ 6 ];
        memcpy(wordOfTheDay, "apple", 6);
        FILETIME ft = { 0 };
        hr = m_spMemoryCache->Add("WordOfDay", wordOfTheDay, 6 * sizeof(char), &ft, 0, 0, 0);
        result = CStringA("apple");
    }

    return hr;
}
