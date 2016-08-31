#include "guid.h"
#include "string.h"
#include <ObjBase.h>

std::string cactus::GenerateGUIDStringA()
{
    GUID pguid;
    CoCreateGuid(& pguid);
    return guid2string(pguid);
}

std::wstring cactus::GenerateGUIDStringW()
{
    GUID pguid;
    CoCreateGuid(& pguid);
    return guid2wstring(pguid);
}

std::wstring cactus::guid2wstring(REFGUID rguid)
{
    std::wstring progid;
    progid.resize(39);
    int len = 0;
    len = ::StringFromGUID2(rguid, static_cast<LPOLESTR>(
                                & * progid.begin()), progid.size());
    progid.resize(len - 1);
    return progid;
}

IID cactus::string2guid(const std::wstring& guid)
{
    IID iid = { 0 };
    ::IIDFromString(const_cast<LPOLESTR>(makeguid(guid).c_str()), & iid);
    return iid;
}

bool cactus::isguid(const std::wstring& guid)
{
    IID iid = { 0 };
    HRESULT hr;

    switch (hr = ::IIDFromString(const_cast<LPOLESTR>(makeguid(guid).c_str()), & iid)) {
    case E_INVALIDARG:
    case CO_E_IIDSTRING:
        return false;

    case S_OK:
        return true;

    default:
        return false;
    }

    return false;
}

bool cactus::isguid(const std::string& guid)
{
    return isguid(cactus::string2wstring(guid));
}

IID cactus::string2guid(const std::string& guid)
{
    return string2guid(cactus::string2wstring(guid));
}

std::string cactus::guid2string(REFGUID rguid)
{
    return wstring2string(guid2wstring(rguid));
}

std::wstring cactus::makeguid(const std::wstring& guid)
{
    std::wstring result;

    if (! guid.empty() && guid[0] != L'{')
        result += L"{";

    result += guid;

    if (! guid.empty() && guid[guid.length() - 1] != L'}')
        result += L"}";

    return result;
}

std::string cactus::makeguid(const std::string& guid)
{
    return wstring2string(makeguid(string2wstring(guid)));
}