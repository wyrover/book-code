/*

시스템의 불법 패킷(스푸핑같은 해킬들..)을 막는 에이전트를

만들려구 하는데요.. 물론. 화면상에 윈도우를 보이지 않고.

돌아 가구요. 문제는 사용자가 이를 알고 프로그램을 죽여버리면.

의미가 없자나요.. 그래서 강제 종료가 불가능하게 만들려구 합니다.

----------------------------------------------------------------

먼저 작업 관리자에서 죽일수 없게 만들려면..
해당 프로세스를 생성할때 CreateProcess의 3번째 파라미터인 PKO의 보안을 지정한다.
이때 모든 사용자(EveryOne)의 SID를 얻은후 Access 권한을 0으로 설정 하면서 생성하면 됨 

즉 계산기를 작업관리자에서 죽일수 없게 하려면
1. EveryOne의 SID를 얻는다.
2. ACE를 생성한후 1에서 얻은 SID에 대해 접근 권한을 0지정.
3. ACL을 만든후, Security Descriptor를 만들고, SECURITY_ATTRIBUTES 구조체를 완성한다.
4. CreateProcess( 0, "calc.exe", &sa,/// 3에서 만든 구조체...

로 실행 하면 작업관리자에서 죽일수 없다. 
물론 TerminateProcess() 를 사용해도 죽일수 없다.

//-------------------------------------------------
단, 
하지만 위처럼 생성된 계산기를 전혀 죽일수 없는 것은 아니다..

특정 사용자(Process)가 SE_DEBUG_NAME 이라는 특권을 얻은후 TerminateProcess()를 수행하면 접근 권한이 
없는 KO라도 접근이 허용되기 때문에 위 계산기를 죽일수 있다.

물론 WinLogOn.exe도 죽일수 있습니다.
서비스도 물론 죽일수 있습니다.

작업관리자는 SE_DEBUG_NAME 특권을 사용하고 있지 않기 때문에 못죽일 뿐이다. 
*/


// 작업 관리자에서 죽일수 없도록 계산기를 실행하기

#define _WIN32_WINNT 0x0500
#define WINVER      0x0500
#include <windows.h>
#include <iostream.h>
#include <aclapi.h>

void main()
{
	DWORD cbSid=255,cbDomain=255;
	
	// Everyone의 SID를 구한다.
	SID_IDENTIFIER_AUTHORITY SIDEvery=SECURITY_WORLD_SID_AUTHORITY;
	PSID pEverySID;
	AllocateAndInitializeSid(&SIDEvery,1,SECURITY_WORLD_RID,0,0,0,0,0,0,0,&pEverySID);
	
	// ACE 만들기.!!!!
	EXPLICIT_ACCESS EA[1];
	memset(EA,0,sizeof(EXPLICIT_ACCESS)*1);
	EA[0].grfAccessPermissions= 0;  // everyone 의 접근 권한을 NO Access 로 설정.
	EA[0].grfAccessMode=SET_ACCESS;
	EA[0].grfInheritance=NO_INHERITANCE;
	EA[0].Trustee.TrusteeForm=TRUSTEE_IS_SID;
	EA[0].Trustee.TrusteeType=TRUSTEE_IS_WELL_KNOWN_GROUP;
	EA[0].Trustee.ptstrName=(LPTSTR)pEverySID;
	
	// ACL 만들기.
	PACL pAcl;
	SetEntriesInAcl(1,EA,NULL,&pAcl);
	
	// 보안 설명자 만들기.
	SECURITY_DESCRIPTOR SD;
	// SD를 초기화한다.
	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
	
	// ACL을 SD에 포함시킨다.
	SetSecurityDescriptorDacl(&SD,TRUE,pAcl,FALSE);
	
	// 보안 속성을 만든다.
	SECURITY_ATTRIBUTES SA;
	
	SA.nLength=sizeof(SECURITY_ATTRIBUTES);
	SA.lpSecurityDescriptor=&SD;
	SA.bInheritHandle=FALSE;
	
	// 프로세스 생성.
	STARTUPINFO si = { 0 };
	si.cb = sizeof( si );
	
	PROCESS_INFORMATION pi;
	
	BOOL b= CreateProcess( 0, "calc.exe", 
		&SA, 0, // 프로세스 KO 의 보안 설정을 지정한다. (every one에 대해 NO_ACCESS )이다.
		0, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi );
	
	if ( b ) 
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	// 실행중에 할당한 메모리를 해제한다.
	FreeSid(pEverySID);
	LocalFree(pAcl);
}



