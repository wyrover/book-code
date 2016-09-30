#pragma once

#if !defined( INC_EDIT_FORUM_HANDLER_H_ )
#define INC_EDIT_FORUM_HANDLER_H_

#include "ForumList.h"

class EditForumHandler
    : public CRequestHandlerT< EditForumHandler >
{
public:
    EditForumHandler(void);
public:
    virtual ~EditForumHandler(void);

    BEGIN_REPLACEMENT_METHOD_MAP(EditForumHandler)
    REPLACEMENT_METHOD_ENTRY("ValidForumId", OnValidForumId)
    REPLACEMENT_METHOD_ENTRY("ForumId", OnForumId)
    REPLACEMENT_METHOD_ENTRY("ForumName", OnForumName)
    REPLACEMENT_METHOD_ENTRY("ForumDescription", OnForumDescription)
    REPLACEMENT_METHOD_ENTRY("ValidationErrors", OnValidationErrors)
    REPLACEMENT_METHOD_ENTRY("FormFields", OnFormFields)
    END_REPLACEMENT_METHOD_MAP()

    HTTP_CODE ValidateAndExchange();

private:
    HTTP_CODE OnValidForumId();
    HTTP_CODE OnForumId();
    HTTP_CODE OnForumName();
    HTTP_CODE OnForumDescription();
    HTTP_CODE OnValidationErrors();
    HTTP_CODE OnFormFields();
    HTTP_CODE UpdateForumData();
    void ValidateLegalForumId();
    void ValidateLegalForumName();
    CStringA FaultCodeToString(DWORD validationFault);

    int m_forumId;
    CString m_forumName;
    CString m_forumDescription;

    ForumList m_forumList;
    CValidateContext m_validationContext;
    CComPtr< ADODB::_Recordset > m_forumRecordset;
};

DECLARE_REQUEST_HANDLER("EditForum", EditForumHandler, ::EditForumHandler)

#endif
