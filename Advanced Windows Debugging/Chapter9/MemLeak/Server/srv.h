#ifndef __SRV_H
#define __SRV_H

#include <windows.h>

class CIdentity 
{
public:
    virtual BOOL GetUsername(WCHAR** pUserName) { return FALSE; }
    virtual BOOL GetDomain(WCHAR** pUserName) { return FALSE; }

protected:
    CIdentity(PVOID pIdentBlob):m_pIdentityBlob(pIdentBlob){};
    virtual ~CIdentity(){};
    PVOID GetBlob() { return m_pIdentityBlob; }

    PVOID    m_pIdentityBlob;
};

class CSID : CIdentity
{
public:
    CSID(PVOID pIdentBlob):CIdentity(pIdentBlob){}
    ~CSID(){}
};


class CServer
{
public:
    CServer(){};
    ~CServer(){};
    
    CIdentity* GetSID();
    DWORD GetPrivilegeCount();
    DWORD GetGroupCount();
    
private:
    HANDLE GetToken();
} ;




#endif
