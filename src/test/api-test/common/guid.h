#ifndef guid_h__
#define guid_h__

#include <Windows.h>
#include <string>

namespace cactus
{
// converts a globally unique identifier (GUID) into a string of printable characters
std::string guid2string(REFGUID rguid);
std::wstring guid2wstring(REFGUID rguid);
// converts a string into a GUID
IID string2guid(const std::wstring& guid);
IID string2guid(const std::string& guid);
// is a string a guid
bool isguid(const std::wstring& guid);
bool isguid(const std::string& guid);
// generate and converts a globally unique identifier (GUID) into a string of printable characters
std::string GenerateGUIDStringA();
std::wstring GenerateGUIDStringW();
// add { } to a GUID where necessary
std::wstring makeguid(const std::wstring& guid);
std::string makeguid(const std::string& guid);
}

#endif // guid_h__
