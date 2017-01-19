#include "sid.h"
#include <stdlib.h>

DWORD GetCurrentUserSID(PSID *Sid)
{
    TOKEN_USER  *tokenUser = NULL;
    HANDLE      tokenHandle;
    DWORD       tokenSize;
    DWORD       sidLength;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tokenHandle)) {
        GetTokenInformation(tokenHandle,
                            TokenUser,
                            tokenUser,
                            0,
                            &tokenSize);
        tokenUser = (TOKEN_USER *) malloc(tokenSize);

        if (GetTokenInformation(tokenHandle,
                                TokenUser,
                                tokenUser,
                                tokenSize,
                                &tokenSize)) {
            sidLength = GetLengthSid(tokenUser->User.Sid);
            *Sid = (PSID) malloc(sidLength);
            memcpy(*Sid, tokenUser->User.Sid, sidLength);
            CloseHandle(tokenHandle);
        } else {
            free(tokenUser);
            return GetLastError();
        }
    } else {
        free(tokenUser);
        return GetLastError();
    }

    free(tokenUser);
    return ERROR_SUCCESS;
}



bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	LUID luid;
	

	// lookup passed privilege on the local system and store it into LUID
	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid ))
	{
		
		return false;
	}

	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}

	// enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL))
	{ 
		
		return false;
	} 

	return true;
}


bool GetSeDebugPrivilege()
{
	HANDLE hToken = NULL;

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		
		return false;
	}

	return SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);
}


bool IsAdmin()
{
	bool bIsAdmin = false;
	HANDLE hToken = NULL;
	HANDLE hImpersonationToken = NULL;
	PACL pACL = NULL;
	PSECURITY_DESCRIPTOR psdAdmin = NULL;
	PSID psidAdmin = NULL;

	// obtain a primary token and then create a duplicate impersonation token.
	if (!OpenThreadToken(GetCurrentThread(), TOKEN_DUPLICATE|TOKEN_QUERY, TRUE, &hToken))
	{
		if (GetLastError() != ERROR_NO_TOKEN)
		{

			return bIsAdmin;
		}

		// if we cannot obtain thread token, try to obtain the process token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE|TOKEN_QUERY, &hToken))
		{
			
			return bIsAdmin;
		}
	}

	if (!DuplicateToken (hToken, SecurityImpersonation, &hImpersonationToken))
	{
		
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	// create SID that represents the local administrators group.
	SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psidAdmin))
	{
		
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	// create the security descriptor and DACL with an ACE that allows only local admins access.
	psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (psdAdmin == NULL)
	{
		
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	if (!InitializeSecurityDescriptor(psdAdmin, SECURITY_DESCRIPTOR_REVISION))
	{
		
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	// compute size needed for the ACL and allocate memory
	DWORD dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(psidAdmin) - sizeof(DWORD);
	pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
	if (pACL == NULL)
	{
		
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
	{
		
		if (pACL) LocalFree(pACL);
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	const DWORD ACCESS_READ = 1;
	const DWORD ACCESS_WRITE = 2;
	DWORD dwAccessMask= ACCESS_READ | ACCESS_WRITE;

	// grant access to the passed admin SID
	if (!AddAccessAllowedAce(pACL, ACL_REVISION2, dwAccessMask, psidAdmin))
	{
		
		if (pACL) LocalFree(pACL);
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
	{
		
		if (pACL) LocalFree(pACL);
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	// set the group and owner so that enough of the security descriptor is filled out to
	// fulfill AccessCheck
	SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
	SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);

	if (!IsValidSecurityDescriptor(psdAdmin))
	{
		
		if (pACL) LocalFree(pACL);
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	DWORD dwAccessDesired = ACCESS_READ;

	// after that, perform the access check. This will determine whether
	// the current user is a local admin.
	// initialize GenericMapping structure even though you do not use generic rights
	GENERIC_MAPPING GenericMapping;
	GenericMapping.GenericRead    = ACCESS_READ;
	GenericMapping.GenericWrite   = ACCESS_WRITE;
	GenericMapping.GenericExecute = 0;
	GenericMapping.GenericAll     = ACCESS_READ | ACCESS_WRITE;

	DWORD dwStatus;
	DWORD dwStructureSize = sizeof(PRIVILEGE_SET);
	PRIVILEGE_SET ps;

	BOOL tmpIsAdmin = FALSE;
	if (!AccessCheck(psdAdmin, hImpersonationToken, dwAccessDesired, &GenericMapping, &ps, &dwStructureSize, &dwStatus, &tmpIsAdmin))
	{
		bIsAdmin = false;
		
		if (pACL) LocalFree(pACL);
		if (psdAdmin) LocalFree(psdAdmin);
		if (psidAdmin) FreeSid(psidAdmin);
		if (hImpersonationToken) CloseHandle (hImpersonationToken);
		if (hToken) CloseHandle (hToken);
		return bIsAdmin;
	}

	bIsAdmin = (bool) tmpIsAdmin;
	return bIsAdmin;
}

BOOL CheckUAC()
{
	BOOL fIsElevated = FALSE;
	HANDLE hToken = NULL;

	if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
		TOKEN_ELEVATION elevation;
		DWORD dwSize;

		if (GetTokenInformation(hToken,TokenElevation,&elevation,sizeof(elevation),&dwSize))
		{
			fIsElevated = !elevation.TokenIsElevated;
		}

		CloseHandle(hToken);
	}

	return fIsElevated;
}