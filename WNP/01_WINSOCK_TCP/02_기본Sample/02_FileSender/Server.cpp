// File Sender

#include <winsock2.h>	
#include <stdlib.h>	
#include <stdio.h>

#define BUFSIZE 4096

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);
// 소켓 함수 오류 출력
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;

	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR) 
			return SOCKET_ERROR;
		else if(received == 0) 
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");	
	
	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");
	
	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE];

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			continue;
		}
		printf("\nFileSender 접속: IP 주소=%s, 포트 번호=%d\n", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 파일 이름 받기
		char filename[256];
		ZeroMemory(filename, 256);
		retval = recvn(client_sock, filename, 256, 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			closesocket(client_sock);
			continue;
		}
		printf("-> 받을 파일 이름: %s\n", filename);
		
		// 파일 크기 받기
		int totalbytes;
		retval = recvn(client_sock, (char *)&totalbytes, sizeof(totalbytes), 0);
		if(retval == SOCKET_ERROR){
			err_display("recv()");
			closesocket(client_sock);
			continue;
		}
		printf("-> 받을 파일 크기: %d\n", totalbytes);

		// 파일 열기
		FILE *fp = fopen(filename, "wb");
		if(fp == NULL){
			perror("파일 입출력 오류");
			closesocket(client_sock);
			continue;
		}

		// 파일 데이터 받기
		int numtotal = 0;
		while(1){
			retval = recvn(client_sock, buf, BUFSIZE, 0);
			if(retval == SOCKET_ERROR){
				err_display("recv()");
				break;
			}
			else if(retval == 0) 
				break;
			else{
				fwrite(buf, 1, retval, fp);
				if(ferror(fp)){
					perror("파일 입출력 오류");
					break;
				}
				numtotal += retval;
			}
		}
		fclose(fp);

		// 전송 결과 출력
		if(numtotal == totalbytes)
			printf("-> 파일 전송 완료!\n");
		else
			printf("-> 파일 전송 실패!\n");

		// closesocket()
		closesocket(client_sock);
		printf("FileSender 종료: IP 주소=%s, 포트 번호=%d\n", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}



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














