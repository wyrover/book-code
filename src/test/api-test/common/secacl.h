#ifndef secacl_h__
#define secacl_h__

#include <Windows.h>
#include <aclapi.h>
#include <sddl.h>
#include <string>



class mod_secacl
{
public:
    static bool simpleSidToString(PSID Sid, std::wstring * String);
    static bool sidToStrSid(PSID Sid, std::wstring * strSid);
    static bool nullSdToHandle(PHANDLE monHandle, SE_OBJECT_TYPE monType = SE_KERNEL_OBJECT);
    static bool sidToName(PSID Sid, std::wstring * strName, std::wstring * domainName = NULL, std::wstring * systemName = NULL, SID_NAME_USE * usage = NULL);
    static bool tokenUser(HANDLE tokenHandle, std::wstring * strName, std::wstring * domainName = NULL, std::wstring * systemName = NULL, SID_NAME_USE * usage = NULL);

    static bool exchangeDupToken(HANDLE * tokenHandle);
    static bool addWorldToMimikatz(SC_HANDLE * monHandle);
};

#endif // secacl_h__
