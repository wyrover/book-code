#include "StdAfx.h"
#include "hr.h"
#include "ForumList.h"
#include "ForumConnectionString.h"

using namespace std;
using namespace ADODB;

ForumList::ForumList(void)
{
    m_numForums = -1;
}

ForumList::~ForumList(void)
{
}

HRESULT ForumList::Open()
{
    HR(m_forumConnection.CoCreateInstance(__uuidof(Connection)));
    HR(m_forumConnection->Open(CComBSTR(FORUM_CONNECTION_STRING), 0, 0, 0));
    return S_OK;
}

static HRESULT CreateADOCommand(BSTR sql, _Connection *pConnection, _Command **pCommand)
{
    CComPtr< _Command > command;
    HR(command.CoCreateInstance(__uuidof(Command)));
    HR(command->putref_ActiveConnection(pConnection));
    HR(command->put_CommandText(sql));
    HR(command->put_CommandType(adCmdText));
    *pCommand = command.Detach();
    return S_OK;
}

static HRESULT AddParameter(_Command *pCommand, _Parameter *param)
{
    CComPtr< Parameters > parameters;
    HR(pCommand->get_Parameters(&parameters));
    HR(parameters->Append(param));
    return S_OK;
}

static HRESULT ExecuteCommand(_Command *pCommand, _Recordset **pResult)
{
    CComVariant recordsAffected;
    HR(pCommand->Execute(&recordsAffected, 0, 0, pResult));
    return S_OK;
}

static HRESULT ExecuteSql(BSTR sql, _Connection *pConnection, _Recordset **pResult)
{
    CComPtr< _Command > command;
    HR(CreateADOCommand(sql, pConnection, &command));
    HR(ExecuteCommand(command, pResult));
    return S_OK;
}

HRESULT ForumList::ReadAllForums(_Recordset **pResult)
{
    HR(ExecuteSql(CComBSTR(L"SELECT id, name, description FROM Forum"), m_forumConnection, pResult));
    return S_OK;
}

HRESULT ForumList::ReadOneForum(int id, _Recordset **pResult)
{
    CComPtr< _Command > command;
    HR(CreateADOCommand(CComBSTR(L"SELECT id, name, description FROM Forum WHERE id=?"), m_forumConnection, &command));
    CComPtr< _Parameter > forumIdParam;
    CComVariant forumIdValue(id);
    HR(command->CreateParameter(CComBSTR(L""), adInteger, adParamInput, sizeof(int), forumIdValue, &forumIdParam));
    HR(AddParameter(command, forumIdParam));
    HR(ExecuteCommand(command, pResult));
    return S_OK;
}

static HRESULT GetFieldValue(_Recordset *pRecordSet, BSTR fieldName, VARIANT *pResult)
{
    CComPtr< Fields > fields;
    CComPtr< Field > field;
    HR(pRecordSet->get_Fields(&fields));
    HR(fields->get_Item(CComVariant(fieldName), &field));
    HR(field->get_Value(pResult));
    return S_OK;
}

HRESULT ForumList::GetNumForums(int *pNumForums)
{
    if (m_numForums == -1) {
        CComPtr< _Recordset > resultSet;
        HR(ExecuteSql(CComBSTR(L"SELECT COUNT(*) as num_forums FROM Forum"), m_forumConnection, &resultSet));
        CComVariant numForums;
        HR(GetFieldValue(resultSet, CComBSTR(L"num_forums"), &numForums));
        HR(numForums.ChangeType(VT_I4));
        m_numForums = numForums.lVal;
    }

    *pNumForums = m_numForums;
    return S_OK;
}

HRESULT ForumList::ContainsForumData(_Recordset *pRecordSet, bool *pResult)
{
    *pResult = true;
    VARIANT_BOOL atEOF;

    if (SUCCEEDED(pRecordSet->get_adoEOF(&atEOF))) {
        if (atEOF == VARIANT_TRUE) {
            *pResult = false;
        }
    }

    return S_OK;
}

HRESULT ForumList::GetCurrentForumName(_Recordset *pRecordSet, BSTR *pName)
{
    CComVariant forumName;
    HR(GetFieldValue(pRecordSet, CComBSTR(L"name"), &forumName));
    *pName = forumName.bstrVal;
    forumName.vt = VT_EMPTY;
    return S_OK;
}

HRESULT ForumList::GetCurrentForumId(_Recordset *pRecordSet, long *pId)
{
    CComVariant forumId;
    HR(GetFieldValue(pRecordSet, CComBSTR(L"id"), &forumId));
    *pId = forumId.lVal;
    return S_OK;
}

HRESULT ForumList::GetCurrentForumDescription(_Recordset *pRecordSet, BSTR *pDesc)
{
    CComVariant forumDesc;
    HR(GetFieldValue(pRecordSet, CComBSTR(L"description"), &forumDesc));
    *pDesc = forumDesc.bstrVal;
    forumDesc.vt = VT_EMPTY;
    return S_OK;
}

HRESULT ForumList::UpdateForum(int forumId, const CString &forumName, const CString &forumDescription)
{
    CComPtr< _Command > updateCommand;
    HR(CreateADOCommand(CComBSTR(L"UPDATE Forum SET name = ?, description = ? WHERE id = ?"),
                        m_forumConnection, &updateCommand));
    CComPtr< _Parameter > forumIdParam;
    CComPtr< _Parameter > forumNameParam;
    CComPtr< _Parameter > forumDescriptionParam;
    CComVariant idValue(forumId);
    CComVariant nameValue(forumName.AllocSysString());
    CComVariant descriptionValue(forumDescription.AllocSysString());
    HR(updateCommand->CreateParameter(CComBSTR(L"name"), adVarWChar, adParamInput, sizeof(wchar_t) * 64, nameValue, &forumNameParam));
    HR(updateCommand->CreateParameter(CComBSTR(L"description"), adVarWChar, adParamInput, sizeof(wchar_t) * 2000, descriptionValue, &forumDescriptionParam));
    HR(updateCommand->CreateParameter(CComBSTR(L"id"), adInteger, adParamInput, sizeof(int), idValue, &forumIdParam));
    HR(AddParameter(updateCommand, forumNameParam));
    HR(AddParameter(updateCommand, forumDescriptionParam));
    HR(AddParameter(updateCommand, forumIdParam));
    CComPtr< _Recordset > results;
    HR(ExecuteCommand(updateCommand, &results));
    return S_OK;
}

HRESULT ForumList::ReadForumPosts(int forumId, _Recordset **pResult)
{
    CComPtr< _Command > cmd;
    HR(CreateADOCommand(
           CComBSTR(L"SELECT id, forum_id, poster, posted_at, in_reply_to_id, title, text FROM Message WHERE forum_id = ? AND in_reply_to_id = 0"),
           m_forumConnection, &cmd));
    CComPtr< _Parameter > forumIdParam;
    CComVariant idValue(forumId);
    HR(cmd->CreateParameter(CComBSTR(L"forum_id"), adInteger, adParamInput, sizeof(int), idValue, &forumIdParam));
    HR(AddParameter(cmd, forumIdParam));
    HR(ExecuteCommand(cmd, pResult));
    return S_OK;
}

HRESULT ForumList::ContainsPostData(_Recordset *pRecordSet, bool *pResult)
{
    // The implementation is accidentally the same
    return ContainsForumData(pRecordSet, pResult);
}

HRESULT ForumList::GetCurrentPostId(_Recordset *pRecordSet, long *pId)
{
    CComVariant id;
    HR(GetFieldValue(pRecordSet, CComBSTR(L"id"), &id));
    *pId = id.lVal;
    return S_OK;
}

HRESULT ForumList::GetCurrentPostTitle(_Recordset *pRecordSet, BSTR *pTitle)
{
    CComVariant title;
    HR(GetFieldValue(pRecordSet, CComBSTR(L"title"), &title));
    *pTitle = title.bstrVal;
    title.vt = VT_EMPTY;
    return S_OK;
}

HRESULT ForumList::GetCurrentPostPoster(ADODB::_Recordset *pRecordSet, BSTR *pPoster)
{
    CComVariant poster;
    HR(GetFieldValue(pRecordSet, CComBSTR(L"poster"), &poster));
    *pPoster = poster.bstrVal;
    poster.vt = VT_EMPTY;
    return S_OK;
}
