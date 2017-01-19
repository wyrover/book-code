#ifndef sid_h__
#define sid_h__

#include <windows.h>

DWORD GetCurrentUserSID(PSID *Sid);
bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
bool GetSeDebugPrivilege();
bool IsAdmin();
BOOL CheckUAC();

#endif // sid_h__
