// Process 열거 

// 1. ToolHelpAPI 이용 : 2000, 98, 95 사용 가능
#include <windows.h>
#include <TLHelp32.h>
#include <iostream>
using namespace std;

/*
  레지스트리 안에 보이지 않는 HKEY_PERFORMANCE_DATA 라는 곳에 
  모든 시스템 정보가 들어있다.

  직접 접근은 하지 못하고 정보를 제공하는 함수를 통해 접근한다.

   TLHelp32.h에 있는 함수들을 이용하여 정보를 얻어옴...
   MSDN   .. toolhelpmodule 확인..
*/

// Tool Help API 를 사용하기 위한헤더.
// Tool Help API : 프로세스, 스레드, 모듈, 힙 등을 열거 할때 사용하는 함수들 모음
/*
void main()
{
	HANDLE	hSnap = CreateToolhelp32Snapshot(
						TH32CS_SNAPPROCESS, 0);	// process ID

	if( hSnap == 0 )
		return ;

	PROCESSENTRY32	ppe;
	BOOL	b = Process32First(hSnap, &ppe);
	while ( b )
	{
		printf("%04d %04d %s\n", ppe.th32ProcessID,
								 ppe.th32ParentProcessID,
								 ppe.szExeFile);

		b = Process32Next( hSnap, &ppe );
	}
	CloseHandle(hSnap);
}
*/

// 2. PSAPI : EnumProcess()
/*
#include <windows.h>
#include <stdio.h>
#include "psapi.h"		// psapi.lib 추가..

void PrintProcessNameAndID( DWORD processID)
{
	char szProcessName[ MAX_PATH ] = "unknown";

	// 프로세스의 핸들 얻기
	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | 
								   PROCESS_VM_READ, 
								   FALSE, processID);

	// process 이름 가져오기
	if( NULL != hProcess)
	{
		HMODULE hMod;
		DWORD	cbNeeded;
		if( EnumProcessModules( hProcess, &hMod, sizeof( hMod), &cbNeeded))
		{
			GetModuleBaseName( hProcess, hMod, szProcessName, sizeof( szProcessName));
		}
		else
			return;
	}
	else return;

	//print
	printf("%s ( PROCESS ID : %u )\n", szProcessName, processID);
	CloseHandle(hProcess);

}
void main()
{
	// process list 가져오기(id값)
	DWORD aProcess[1024], cbNeeded, cProcesses;
	unsigned int i;

	if( !EnumProcesses(aProcess, sizeof( aProcess), &cbNeeded) )
		// 배열 수, 리턴되는 바이트 수 
		// 배열에 id값들이 들어간다.
		return;

	// 얼마나 많은 프로세스들이 리턴되었나 계산
	cProcesses = cbNeeded / sizeof( DWORD);

	// process 이름 출력
	for( i = 0; i < cbNeeded; i++)
		PrintProcessNameAndID( aProcess[i] );
}
*/

// 3. 모듈 열거
#include <windows.h>
#include <TLHelp32.h>
#include <iostream>
using namespace std;

void main()
{
	// 계산기 ProcessID얻기
	HWND hCalc = FindWindow(0, "계산기");

	DWORD pid;
	GetWindowThreadProcessId(hCalc, &pid);

	HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pid);

	MODULEENTRY32	mme;
	BOOL b = Module32First( hSnap, &mme);

	while( b )
	{
		cout << (void*)mme.modBaseAddr << " : "
			<< mme.szModule << " PATH : " << mme.szExePath << endl;

		b = Module32Next( hSnap, &mme);
	}
	CloseHandle(hSnap);
}





























