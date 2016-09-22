// console_template.cpp : Defines the entry point for the console application.
//

#include <SDKDDKVer.h>
#include <Windows.h>
#include <aclapi.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <iostream>

using namespace std;




int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	_tsetlocale(LC_ALL, _T(""));
	HANDLE hfl;
	PSECURITY_DESCRIPTOR secPointer;
	PSID ownerSID, everyoneSID, meSID, adminSID;
	SECURITY_ATTRIBUTES sa;
	SID_NAME_USE type;
	_TCHAR v[100];
	DWORD temp, buffSize = 100;

	EXPLICIT_ACCESS ace[2], ace2[1];

	PACL pacl1, pacl2;

	//initializare
	ownerSID = malloc(SECURITY_MAX_SID_SIZE);
	everyoneSID = malloc(SECURITY_MAX_SID_SIZE);
	meSID = malloc(SECURITY_MAX_SID_SIZE);
	adminSID = malloc(SECURITY_MAX_SID_SIZE);

	secPointer = malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (!InitializeSecurityDescriptor(secPointer, SECURITY_DESCRIPTOR_REVISION))
	{
		printf("Could not initialize security descriptor!\n");
	}


	//Facem rost de sid-ul pentru owner
	temp = SECURITY_MAX_SID_SIZE;
	if (!CreateWellKnownSid(WinCreatorOwnerSid, NULL, ownerSID, &temp))
	{
		printf("Could not get OWNER SID\n");
	}
	//Facem rost de sid-ul pentru everyone
	temp = SECURITY_MAX_SID_SIZE;
	if (!CreateWellKnownSid(WinWorldSid, NULL, everyoneSID, &temp))
	{
		printf("Could not get EVERYONE SID\n");
	}
	//Facem rost de sid-ul pentru ADMINISTRATORI
	temp = SECURITY_MAX_SID_SIZE;
	if (!CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, adminSID, &temp))
	{
		printf("Could not get ADMIN SID\n");
	}
	//Facem rost de sid-ul meu
	temp = SECURITY_MAX_SID_SIZE;
	if (!LookupAccountName(NULL, TEXT("acitea"), meSID, &temp, v, &buffSize, &type))
	{
		printf("%d", GetLastError());
		printf("Could not get my SID");
	}

	//SET OWNER
	if (!SetSecurityDescriptorOwner(secPointer, meSID, false))
	{
		printf("Failed on setting owner");
	}


	//ACE's
	ZeroMemory(&ace, 2 * sizeof(EXPLICIT_ACCESS));
	ace[0].grfAccessPermissions = 1179785; // READ ACCESS
	ace[0].grfAccessMode = SET_ACCESS;
	ace[0].grfInheritance = NO_INHERITANCE;
	ace[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ace[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ace[0].Trustee.ptstrName = (LPTSTR)everyoneSID;

	ace[1].grfAccessPermissions = 1245631 | WRITE_DAC; // WRITE ACCESS
	ace[1].grfAccessMode = SET_ACCESS;
	ace[1].grfInheritance = NO_INHERITANCE;
	ace[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ace[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
	ace[1].Trustee.ptstrName = (LPTSTR)ownerSID;

	temp = SetEntriesInAcl(2, ace, NULL, &pacl1);
	if (temp != ERROR_SUCCESS)
	{
		printf("Failed to set entries in ACL\n");
		printf("Error code: %d\n", temp);
	}
	if (!SetSecurityDescriptorDacl(secPointer, TRUE, pacl1, FALSE))
	{
		printf("Failed to set security descriptor DACL\n");
	}

	// WRAPPER STRUCTURE
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = secPointer;
	sa.bInheritHandle = FALSE;


	// ACTUALLY MAKE THE FILE
	hfl = CreateFile(TEXT("test.txt"), GENERIC_WRITE, 0, &sa, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hfl == INVALID_HANDLE_VALUE)
	{
		printf("Invalid handle value - could not create file!");
	}



	//GIVING PRIVILEGES TO THE ADMINS 
	ZeroMemory(&ace2, 1 * sizeof(EXPLICIT_ACCESS));
	ace2[0].grfAccessPermissions = 1245631; // WRITE
	ace2[0].grfAccessMode = SET_ACCESS;
	ace2[0].grfInheritance = NO_INHERITANCE;
	ace2[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ace2[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ace2[0].Trustee.ptstrName = (LPTSTR) adminSID;

	temp = SetEntriesInAcl(1, ace2, pacl1, &pacl2);
	if (temp != ERROR_SUCCESS)
	{
		printf("Failed to set entries in ACL2\n");
		printf("Error code: %d\n", temp);
	}
	temp = SetSecurityInfo(hfl, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pacl2, NULL);
	if (temp != ERROR_SUCCESS)
	{
		printf("Failed on setting admin privileges!\n");
		printf("Error: %d", temp);
	}

	scanf("%d", &temp);
	CloseHandle(hfl);

	return 0;
}

