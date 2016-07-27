// [ File Sender ]

#include <winsock2.h>		
#include <stdlib.h>		
#include <stdio.h>

#define BUFSIZE 4096

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);
// 소켓 함수 오류 출력
void err_display(char *msg);

int main(int argc, char* argv[])
{
	int retval;

	if(argc < 2){
		fprintf(stderr, "Usage: %s <FileName>\n", argv[0]);
		return -1;
	}

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");	
	
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");



	// 파일 열기
	FILE *fp = fopen(argv[1], "rb");
	if(fp == NULL){			perror("파일 입출력 오류");		return -1;	}
	
	// 1. 파일 이름 보내기
	char filename[256];			
	ZeroMemory(filename, 256);
	sprintf(filename, argv[1]);		
	retval = send(sock, filename, 256, 0);
	if(retval == SOCKET_ERROR) err_quit("send()");	


	// 2. 파일 크기 얻기
	fseek(fp, 0, SEEK_END);			
	int totalbytes = ftell(fp);
	
	// 파일 크기 보내기
	retval = send(sock, (char *)&totalbytes, sizeof(totalbytes), 0);
	if(retval == SOCKET_ERROR) err_quit("send()");	


	
	// 3. 파일 데이터 전송에 사용할 변수
	char buf[BUFSIZE];	//4096byte	
	int numread;			
	int numtotal = 0;

	// 파일 데이터 보내기
	rewind(fp); // 파일 포인터를 제일 앞으로 이동
	while(1){
		numread = fread(buf, 1, BUFSIZE, fp);
		if(numread > 0){
			retval = send(sock, buf, numread, 0);
			if(retval == SOCKET_ERROR){
				err_display("send()");				break;
			}
			numtotal += numread;
		}
		else if(numread == 0 && numtotal == totalbytes){
			printf("파일 전송 완료!: %d 바이트\n", numtotal);			break;
		}
		else{
			perror("파일 입출력 오류");			break;
		}
	}
	fclose(fp);
	// closesocket()
	closesocket(sock);		
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
//=> Project -> Settign -> Debuf -> test.jpg

