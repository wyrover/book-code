/*
파일 보안

*파일 보안은 NTFS에서만 가능

 

1. ACE(Access Control Entry) 만들기

2. ACL(Access Control List)에  ACE추가

3. ACL을 SECURITY_DESCRIPTOR에 추가

4. SD를 SECURITY_ATTRIBUTES에 추가

--------------------------------------------------------------------------
*/


#include <windows.h>

#include <aclapi.h>       // 보안관련 헤더

 

void main()

{

        // 1. sid 얻기

        char sid[256] = {0};

        char domain[256] = {0};

 

        DWORD sz1 = 256;

        DWORD sz2 = 256;

 

        SID_NAME_USE use;

 

        LookupAccountName(".", "everyone", sid, &sz1, domain, &sz2, &use);

 

        // 2. ACE 만들기

        EXPLICIT_ACCESS ea = {0};

        ea.grfAccessPermissions = GENERIC_READ;        // 접근권한

        ea.grfInheritance = NO_INHERITANCE;

        ea.grfAccessMode = SET_ACCESS;

        ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;

        ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;

        ea.Trustee.ptstrName = (LPSTR)sid;

 

        // 3. ACL 만들기

        PACL pAcl;

        SetEntriesInAcl(1, &ea, 0, &pAcl);

 

        // 4. SECURITY_DESCRIPTOR에 ACL 넣기

        SECURITY_DESCRIPTOR sd;

 

        InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

        SetSecurityDescriptorDacl(&sd, TRUE, pAcl, FALSE);

 

        // 5. SA 만들기

        SECURITY_ATTRIBUTES sa;

        sa.bInheritHandle                = FALSE;  // 상속 가능여부

        sa.lpSecurityDescriptor = &sd;        // 보안 설명자.

        sa.nLength = sizeof(sa);

 

        //-----------------------------------------------------------------

        // KO 생성시 sa를 전달하면 보안 속성이 지정된다

/*      HANDLE hFile = CreateFile("c:\\a.txt",

                           GENERIC_READ | GENERIC_WRITE,

                           0,

                           &sa,        // 보안속성

                           CREATE_ALWAYS,

                           FILE_ATTRIBUTE_NORMAL, 0);

                                                          */

        PROCESS_INFORMATION pi;

        STARTUPINFO si = {0};

        si.cb = sizeof(si);

 

        char name[256] = "calc.exe";

 

        // 모든 사용자가 접근이 금지된 프로세스를 생성한다.

        BOOL b = CreateProcess(0, name, &sa, &sa, // 보안

                                                   0, 0, 0, 0, &si, &pi);

 

        if(b)

        {

                CloseHandle(pi.hProcess);

                CloseHandle(pi.hThread);

        }

 

//        CloseHandle(hFile);

        LocalFree(sid);

}

 

// 실행하라

// 탐색기를 열어서 c:\\a.txt에 오른쪽 버튼을 누르고 속성 -> 보안 탭

// 으로 가보아라

