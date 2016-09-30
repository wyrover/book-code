#pragma once

#if !defined( INC_FORUMLIST_H_ )
#define INC_FORUMLIST_H_

#include <string>
#include "ForumConsumer.h"

class ForumList
{
public:
    ForumList();
    virtual ~ForumList();

    // Retrieve
    HRESULT Open();
    HRESULT ReadAllForums(ADODB::_Recordset **pResult);
    HRESULT ReadOneForum(int id, ADODB::_Recordset **pResult);
    HRESULT GetNumForums(int *pNumForums);
    HRESULT ReadForumPosts(int id, ADODB::_Recordset **pResult);

    // Recordset extraction functions - forums
    HRESULT ContainsForumData(ADODB::_Recordset *pRecordSet, bool *pResult);
    HRESULT GetCurrentForumName(ADODB::_Recordset *pRecordSet, BSTR *pName);
    HRESULT GetCurrentForumId(ADODB::_Recordset *pRecordSet, long *pId);
    HRESULT GetCurrentForumDescription(ADODB::_Recordset *pRecordSet, BSTR *pDescription);

    // Recordset extraction functions - posts
    HRESULT ContainsPostData(ADODB::_Recordset *pRecordSet, bool *pResult);
    HRESULT GetCurrentPostId(ADODB::_Recordset *pRecordSet, long *pId);
    HRESULT GetCurrentPostPoster(ADODB::_Recordset *pRecordSet, BSTR *pPoster);
    HRESULT GetCurrentPostInReplyTo(ADODB::_Recordset *pRecordSet, long *pReplyToId);
    HRESULT GetCurrentPostTitle(ADODB::_Recordset *pRecordSet, BSTR *pTitle);
    HRESULT GetCurrentPostText(ADODB::_Recordset *pRecordSet, BSTR *pText);

    // Update functions
    HRESULT UpdateForum(int forumId, const CString &forumName, const CString &forumDescription);

private:
    CComPtr< ADODB::_Connection > m_forumConnection;
    int m_numForums;
};

#endif
