#ifndef __SRV_H
#define __SRV_H

#include <windows.h>

class CServer
{
public:
    CServer(){};
    ~CServer(){};
    
    PSID GetSID();
    DWORD GetPrivilegeCount();
    DWORD GetGroupCount();
    
private:
    HANDLE GetToken();
} ;




#endif
