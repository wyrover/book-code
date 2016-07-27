//-------------------------------------------------------------
//[  MulticastSender ]
//-------------------------------------------------------------


#include <winsock2.h>	
#include <ws2tcpip.h>	
#include <stdlib.h>	
#include <stdio.h>
#define BUFSIZE 512



// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}



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



int main(int argc, char* argv[])	
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)		
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	

	
	// 멀티캐스트 TTL 설정
	int ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");


	// 소켓 주소 구조체 초기화
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(9000);
	remoteaddr.sin_addr.s_addr = inet_addr("235.7.8.9");


	// 데이터 통신에 사용할 변수
	int len;
	char buf[BUFSIZE+1];


	// 멀티캐스트 데이터 보내기
	while(1)
	{
		// 데이터 입력
		printf("\n[보낼 데이터]");
		if( fgets(buf, BUFSIZE+1, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if( buf[len-1] == '\n')
			buf[len-1] = '\0';

		if( strlen(buf) == 0 )
			break;

		// 데이터 보내기
		retval	= sendto(sock, buf, strlen(buf), 0, 
				(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));
		if( retval == SOCKET_ERROR)
		{
			err_display("sendto");
			continue;
		}
		printf("%d바이트를 보냈습니다.\n", retval);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
