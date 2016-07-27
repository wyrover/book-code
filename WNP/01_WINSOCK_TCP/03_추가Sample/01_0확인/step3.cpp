//--------------------------------------------------------------
// IPv4 : inet_addr & inet_ntoa
//
//----------------------------------------------------------------


#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>	
#pragma comment(lib, "ws2_32.lib")

void main()
{
	WSADATA wsadata;

	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		printf("Can't Initialize Socket !\n");
		return ;
	}

	//--------------------------------------------
	// 문자열 형태의 IP를 Long 으로 변형 하기. - IPv4, 
	long ip = inet_addr("100.100.100.200");
	printf("%x\n", ip );

	// IP 주소를 편리하게 다루기 위해 만들어진 공용체 - in_addr(크기는 결국 4byte)
	in_addr addr;  
	addr.s_addr = ip; // long 형으로 접근.
	
	printf("%d\n", addr.s_impno); // 마지막 4번째 byte를 얻는다.
	//------------------------------------------------------------
	// in_addr 에 담긴 주소를 다시 문자열로 바꾸는 함수.
	char* p = inet_ntoa( addr );

	printf("%s\n", p );


	//--------------------------------------------
	WSACleanup();
}









