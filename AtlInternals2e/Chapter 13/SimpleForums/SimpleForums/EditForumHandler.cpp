#include "StdAfx.h"
#include "EditForumHandler.h"

using namespace std;

EditForumHandler::EditForumHandler(void)
    : m_validationContext(CValidateContext::ATL_EMPTY_PARAMS_ARE_FAILURES)
{
}

EditForumHandler::~EditForumHandler(void)
{
}

HTTP_CODE EditForumHandler::ValidateAndExchange()
{
    m_forumId = -1;
    AS_HR(m_forumList.Open());
    m_HttpRequest.GetQueryParams().Exchange("forumid", &m_forumId, &m_validationContext);
    ValidateLegalForumId();

    if (m_HttpRequest.GetMethod() == CHttpRequest::HTTP_METHOD_POST) {
        const CHttpRequestParams& formFields = m_HttpRequest.GetFormVars();
        formFields.Validate("forumName", &m_forumName, 1, 50, &m_validationContext);
        formFields.Validate("forumDescription", &m_forumDescription, 1, 255, &m_validationContext);

        if (m_validationContext.ParamsOK()) {
            ValidateLegalForumName();
        }

        if (m_validationContext.ParamsOK()) {
            return UpdateForumData();
        }
    }

    m_HttpResponse.SetContentType("text/html");
    return HTTP_SUCCESS;
}

void EditForumHandler::ValidateLegalForumId()
{
    if (m_forumId != -1) {
        if (SUCCEEDED(m_forumList.ReadOneForum(m_forumId, &m_forumRecordset))) {
            bool containsData;

            if (SUCCEEDED(m_forumList.ContainsForumData(m_forumRecordset, &containsData))) {
                if (!containsData) {
                    m_validationContext.AddResult("forumid", VALIDATION_E_FAIL);
                    m_forumId = -1;
                }
            }
        }
    }
}

void EditForumHandler::ValidateLegalForumName()
{
    CAtlRegExp< CAtlRECharTraitsW > re;
    CAtlREMatchContext< CAtlRECharTraitsW > match;
    ATLVERIFY(re.Parse(L"^[a-zA-Z ,]*$") == REPARSE_ERROR_OK);

    if (!re.Match(m_forumName.GetBuffer(), &match)) {
        m_validationContext.AddResult("forumName", VALIDATION_E_FAIL);
    }
}

HTTP_CODE EditForumHandler::OnValidForumId()
{
    if (m_forumId != -1) {
        return HTTP_SUCCESS;
    }

    return HTTP_S_FALSE;
}

HTTP_CODE EditForumHandler::OnForumId()
{
    m_HttpResponse << m_forumId;
    return HTTP_SUCCESS;
}

HTTP_CODE EditForumHandler::OnForumName()
{
    CComBSTR name;
    AS_HR(m_forumList.GetCurrentForumName(m_forumRecordset, &name));
    m_HttpResponse << CW2A(name);
    return HTTP_SUCCESS;
}

HTTP_CODE EditForumHandler::OnForumDescription()
{
    CComBSTR description;
    AS_HR(m_forumList.GetCurrentForumDescription(m_forumRecordset, &description));
    m_HttpResponse << CW2A(description);
    return HTTP_SUCCESS;
}

HTTP_CODE EditForumHandler::OnValidationErrors()
{
    if (m_validationContext.ParamsOK()) {
        m_HttpResponse << "No validation errors occurred";
    } else {
        m_HttpResponse << "Validation Errors:";
        int numValidationFailures = m_validationContext.GetResultCount();
        m_HttpResponse << "<ol>";

        for (int i = 0; i < numValidationFailures; ++i) {
            CStringA faultName;
            DWORD faultCode;
            m_validationContext.GetResultAt(i, faultName, faultCode);
            m_HttpResponse << "<li>" << faultName << ": " << FaultCodeToString(faultCode) << "</li>";
        }

        m_HttpResponse << "</ol>";
    }

    return HTTP_SUCCESS;
}

CStringA EditForumHandler::FaultCodeToString(DWORD faultCode)
{
    switch (faultCode) {
    case VALIDATION_S_OK:
        return "Validation succeeded";

    case VALIDATION_S_EMPTY:
        return "Name present but contents were empty";

    case VALIDATION_E_PARAMNOTFOUND:
        return "The named value was not found";

    case VALIDATION_E_LENGTHMIN:
        return "Value was present and converted, but too small";

    case VALIDATION_E_LENGTHMAX:
        return "Value was present and converted, but too large";

    case VALIDATION_E_INVALIDLENGTH:
        return "(Unused error code)";

    case VALIDATION_E_INVALIDPARAM:
        return "The value was present but could not be converted to the given data type";

    case VALIDATION_E_FAIL:
        return "Validation failed";

    default:
        return "Unknown validation failure code";
    }
}

HTTP_CODE EditForumHandler::OnFormFields()
{
    if (m_HttpRequest.GetMethod() == CHttpRequest::HTTP_METHOD_POST) {
        const CHttpRequestParams &formFields = m_HttpRequest.GetFormVars();
        POSITION pos = formFields.GetStartPosition();
        m_HttpResponse << "Form fields:<br>" << "<ul>";

        for (const CHttpRequestParams::CPair *pField = formFields.GetNext(pos); pField != 0; pField = formFields.GetNext(pos)) {
            m_HttpResponse << "<li>" << pField->m_key << ": " << pField->m_value << "</li>";
        }

        m_HttpResponse << "</ul>";
    }

    return HTTP_SUCCESS;
}

HTTP_CODE EditForumHandler::UpdateForumData()
{
    AS_HR(m_forumList.UpdateForum(m_forumId, m_forumName, m_forumDescription));
    CStringA destUrl;
    destUrl.Format("forumlist.srf");
    m_HttpResponse.Redirect(destUrl, CHttpResponse::HTTP_REDIRECT_SEE_OTHER);
    return HTTP_SUCCESS;
}
