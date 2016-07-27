/******************************************************************

  Chapter 1. 윈속에 대한 소개 

	1) 기타 다른 API  
******************************************************************/



//---------------------------------------------------------------- 
//   1) 기타 다른 API 
//---------------------------------------------------------------- 
/*

  *) getpeername  
     연결된 소켓을 통하여 상대방의 주소 정보를 얻는데 사용 

	 int getpeername(
		SOCKET				s,
		struct sockaddr FAR	*name,
		int				FAR *namelen
	 );
		

  *) getsockname  
     자신의 소켓 주소 정보를 얻는데 사용 

	 int getsockname(
		SOCKET				s,
		struct sockaddr FAR	*name,
		int				FAR *namelen
	 );
*/



/*
#include <winsock2.h>		
#include <stdlib.h>		
#include <stdio.h>
// 소켓 함수 오류 출력
void err_display(char *msg)
{	
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 도메인 이름 -> IP 주소
BOOL GetIPAddr(char *name, IN_ADDR *addr)	
{
	HOSTENT *ptr = gethostbyname(name);
	if(ptr == NULL){
		err_display("gethostbyname()");		return FALSE;
	}
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE;
}

// IP 주소 -> 도메인 이름
BOOL GetDomainName(IN_ADDR addr, char *name)
{	
	HOSTENT *ptr = gethostbyaddr((char *)&addr, 	sizeof(addr), AF_INET);
	if(ptr == NULL){
		err_display("gethostbyaddr()");		return FALSE;
	}
	strcpy(name, ptr->h_name);
	return TRUE;
}



int main(int argc, char* argv[])
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// 도메인 이름 -> IP 주소
	IN_ADDR addr;
	if(GetIPAddr("kr.yahoo.com", &addr)){
		// 성공이면 결과 출력
		printf("IP 주소 = %s\n", inet_ntoa(addr));
	
		// IP 주소 -> 도메인 이름
		char name[256];
		if(GetDomainName(addr, name)){
			// 성공이면 결과 출력
			printf("도메인 이름 = %s\n", name);
		}
	}

	WSACleanup();
	return 0;
}
*/


