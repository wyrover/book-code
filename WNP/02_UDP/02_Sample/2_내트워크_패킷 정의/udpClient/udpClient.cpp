//---------------------------------------------------------------------------------------------------
//주       제 : UDP 로 TCP 구현하기
//클래스   명 : udpClient 
//클래스 설명 : Client 구현부로써, three-way handshake 기능을 구현해본다. 
//만 든 날 자 : 2003년 2월 8일
//----------------------------------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>


#define PACKET_SIZE 100

//---------------------------------------------------------------------------------------------------
// define 정의 부분 : 패킷 관련 정의 
//---------------------------------------------------------------------------------------------------
#define DATA						1 // 데이터 패킷
#define ACK							2 // 확인 응답 
#define FAIL						3 // 실패
#define SESSION_REQUEST				4 // connect 요청
#define SESSION_ACCEPTANCE			5 // accept
#define FILE_REQUEST				6 // 파일 전송 요청
#define FILE_ACCEPTANCE				7 // 파일전송 수락 accept
#define FILE_TRANSFER_COMPLETE		8 // 파일 전송 완료
#define FIN							9 // 연결 종료


//---------------------------------------------------------------------------------------------------
// 패킷 구조체 
//---------------------------------------------------------------------------------------------------
struct packet{
	char	buf[PACKET_SIZE];			 // 데이타 버퍼, size는 100으로 define함	
	long	seq;						 // 패킷 순번
	int		ctrl_flag;					 // 패킷의 내용을 나타내는 플래그 값 지정
}send_packet, recv_packet;


//---------------------------------------------------------------------------------------------------
// 함수 선언 
//---------------------------------------------------------------------------------------------------
void err_quit();						// 에러 발생시 종료 함수
void set_send_packet();					// 송신 패킷 설정함수
void send_state_print(int retval);		// 송신 상태 출력 함수
void recv_state_print(int retval);		// 수신 상태 출력 함수


int main()
{
	WSADATA		wsa;					//윈속 초기화시 넣어줄 기본 자료 구조 지정
	int			retval;					//에러 처리시 사용할 값
	SOCKET		s;						//send할 소켓 
	int			pack_count = 0;			//패킷의 갯수 Counting 

	
	//1. Winsock을 초기화 한다. 버전은 2.2
	retval  = WSAStartup(MAKEWORD(2,2), &wsa);
	if(retval != 0) err_quit();


	//2. 소켓을 생성한다.(UDP관련 내용으로 Setting) 
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == INVALID_SOCKET) err_quit();
	
	//3. 보내고자 하는 서버의 주소를  지정한다.
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5150);
	serv_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	struct sockaddr_in sender;
	int sender_size = sizeof(packet);  

	FILE* fp; // 파일 저장할 파일 포인터
	static int seq;

	memset(&recv_packet, 0, sizeof(packet));
	
	// 최초 세션 연결 요청 상태로 설정한다.
	recv_packet.ctrl_flag = SESSION_REQUEST; 

	printf("\t========================================================\n");
	printf("\t=             [Client PROGRAM]                         = \n"); 
	printf("\t========================================================\n");
	printf("\n-->[Client]Server로 연결 요청 패킷을 보내려면 앤터키를 누르세요 : [ ]\b\b");
	if(getchar() == '\n')
	{
		printf("\n\n[Client] : 연결 요청 패킷을 보냅니다.\n");
	}

	while(recv_packet.ctrl_flag != FIN) 
	{
		set_send_packet();
		
		//4.패킷을 전송한다. 
		retval = sendto(s, (char*)&send_packet, sizeof(send_packet), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

		send_state_print(retval); // 현재 송신 상태 출력

		//5. 패킷을 수신한다. 
		memset(&recv_packet, 0, sizeof(packet));
		retval = recvfrom(s, (char*)&recv_packet, sizeof(packet),  0, (SOCKADDR*)&sender, &sender_size);
		if(retval == SOCKET_ERROR) err_quit();
		else if(!retval) 
		{
			printf("[ERROR] can't receive data\n");
		break;
		}
		else
		{
			switch(recv_packet.ctrl_flag) 
			{			

			case FILE_ACCEPTANCE:		//파일 요청의 결과를 받을때 
				fp = fopen(recv_packet.buf, "wb");

				if(fp == NULL)
				{
					printf("%s 파일 생성 실패\n", recv_packet.buf);
					exit(1);
				}

				printf("recv : %s", recv_packet.buf);				
				break;

			case DATA:					//데이터가 넘어올때 
				fseek(fp, (recv_packet.seq*100), SEEK_SET); // 파일 CP 설정				
				fwrite(recv_packet.buf, 1, 100, fp);
				printf("recv : data packet(seq.%d)", recv_packet.seq);
				break;

			case FILE_TRANSFER_COMPLETE:	//파일 전송이 끝났을 경우
				fclose(fp);
				printf("recv : %s\n", "FILE TRANSPER COMPLETE");				
				printf("[CLIENT] : 파일 수신이 완료되었습니다. 'a.txt'파일을 확인하세요\n\n");
				printf("[CLIENT] : 연결을 종료합니다.\n\n");
				break;
			
			case ACK:		//파일 데이터를 받았을 경우
				fseek(fp, (seq*100), SEEK_SET); // 파일 CP 설정				
				fwrite(recv_packet.buf, 1, 100, fp);
				printf("recv : data packet(seq.%d)", seq);
				break;

			default:
				printf("recv : %s\n", recv_packet.buf);				
			}	
		}
		printf("\n");		
		Sleep(1000);
	}

	//6. 소켓을 닫는다.
	retval = closesocket(s);
	if(retval == SOCKET_ERROR) err_quit();

	//7. 윈소켓을 종료한다.  
	retval = WSACleanup();
	if(retval != 0) err_quit();

	return 0;
}

//---------------------------------------------------------------------------------------
// 함수   명 : err_quit()
//      내용 : 관련된 에러의 내용을 메시지 박스를 통해서 알려준다.
//             또한 메시지 박스 확인시 종료시킨다.
//      설명 : 소켓 생성 및 사용과정에서의 에러 확인 기능
//---------------------------------------------------------------------------------------
void err_quit()
{
	LPVOID lpMsgBuf;

	//에러 원인 메세지 버퍼에 기록
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, "에러", MB_OK | MB_ICONINFORMATION);

	LocalFree(lpMsgBuf); // 메세지 버퍼 해제
	
	exit(1);
}


//---------------------------------------------------------------------------------------
// 함수   명 : send_state_print(int retval) 
//      내용 : 현재 송신 상태를 console 화면에 출력시켜준다. 
//      설명 : retval - 에러를 체크하기위해 sendTo시 리턴값을 받아 온다.
//				        만약 0이면 송신이 안됬다는 메시지를 출력하고,
//						그 이외의 값이면 패킷에 담긴 buf의 내용을 출력한다.
//---------------------------------------------------------------------------------------
void send_state_print(int retval) // 현재 송신 상태 출력
{
	if(retval == SOCKET_ERROR) err_quit();
	else if(retval == 0) 
	{
		printf("[ERROR] can't send data\n");
		exit(1);
	}
	else printf("send : %s\n", send_packet.buf);	
}


//---------------------------------------------------------------------------------------
// 함수   명 : recv_state_print(int retval) 
//      내용 : 현재 수신 상태를 console 화면에 출력시켜준다. 
//      설명 : retval - 에러를 체크하기위해 sendTo시 리턴값을 받아 온다.
//				        만약 0이면 송신이 안됬다는 메시지를 출력하고,
//						그 이외의 값이면 패킷에 담긴 buf의 내용을 출력한다.
//---------------------------------------------------------------------------------------
void recv_state_print(int retval) // 현재 수신 상태 출력
{
	if(retval == SOCKET_ERROR) err_quit();
	else if(!retval) 
	{
		printf("[ERROR] can't receive data\n");
		exit(1);
	}
	else printf("recv : %s\n", recv_packet.buf);
	printf("\n");		
}


//---------------------------------------------------------------------------------------
// 함수   명 : set_send_packet()
//      내용 : 보내고자 하는 패킷의 내용을 초기화, 설정한다. 
//      설명 : 패킷의 flag 값에 따른 구분된 패킷 전송 
//---------------------------------------------------------------------------------------
void set_send_packet() // 송신 패킷 설정
{	
	// 송신 패킷 초기화
	memset(&send_packet, 0, sizeof(send_packet));
	// 수신된 패킷의 플래그에 따라 송신 패킷 설정
	
	switch(recv_packet.ctrl_flag) 
	{			
		case SESSION_ACCEPTANCE: // 세션 수락		
			printf("----> 연결이 성공적으로 이루어졌습니다.\n\n\n");
			printf("-->[Client] : 파일을 요청하려면 앤터키를 누르세요 : [ ]\b\b");
			if(getchar() == '\n')
			{
				//연결이 수락되었을 경우 파일 전송을 요청한다. 
				send_packet.ctrl_flag = FILE_REQUEST;
				sprintf(send_packet.buf, "FILE_REQUEST -> for a.txt");
			}
			break;

		case FILE_ACCEPTANCE: // 파일 전송요청 수락
			send_packet.ctrl_flag = DATA;
			sprintf(send_packet.buf, "DATA REQUEST");
			break;

		case DATA: // 데이타 전송일 때 
			send_packet.ctrl_flag  = ACK;
			sprintf(send_packet.buf, "ACK data packet(seq, %d)\n", recv_packet.seq);				
			break;

		case FILE_TRANSFER_COMPLETE: // 파일 전송 완료 일때							
			send_packet.ctrl_flag = FILE_TRANSFER_COMPLETE;
			sprintf(send_packet.buf, "FILE TRANSFER COMPLETE");
			break;

		case ACK:		//파일 도착에 대한 ACK를 보냄 
			send_packet.ctrl_flag = ACK;
			sprintf(send_packet.buf, "ACK (seq, %d)", recv_packet.seq);
			break;
		
		case SESSION_REQUEST:  		// 최초 세션 연결요청
			sprintf(send_packet.buf, "SESSION REQUEST");
			send_packet.ctrl_flag = SESSION_REQUEST;
			break;
	}
}