	#define _WIN32_WINNT 0x0500
	#define WINVER 0x0500

	#include <windows.h>
	#include <iostream.h>
	#include <sddl.h>
	void main()
	{
		char sid[256]; // sid를 담을 변수 가변길이 이므로 충분한 크기로 버퍼를 만든다
		char domain[256];

		DWORD sid_size = 256;
		DWORD domain_size = 256;

		SID_NAME_USE use; // sid의 종류..

		// power의 sid를 구한다.
		LookupAccountName("." ,    // PC이름
						  "power", // 사용자 id
						  sid,    &sid_size,    // sid를 담을 변수,. 크기
						  domain, &domain_size,
						  &use); // sid의 종류

		cout << "Domain : " << domain << endl;
		cout << "SID 종류 : " << use << endl;

		// sid는 문자열이 아닌 이진 값이 므로 바로 출력할수는 없다.
		// 2진 sid를 문자열 sid로 변경하기. - sddl.h 가 필요..( platform sdk를 설치해야 가능)
		char* temp;
		ConvertSidToStringSid( sid, &temp);// 내부적으로 temp에 메모리할당을 해준다.

		cout << "SID : " << temp << endl;
		LocalFree( temp );
	}









