//---------------------------------------------------------------------------------------------------
//주       제 : UDP 로 TCP 구현하기
//클래스   명 : udpServer 
//클래스 설명 : Server 구현부로써, three-way handshake 기능을 구현해본다. 
//만 든 날 자 : 2003년 2월 8일
//----------------------------------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 100 

//---------------------------------------------------------------------------------------------------
// define 정의 부분 : 패킷 관련 정의 
//---------------------------------------------------------------------------------------------------
#define DATA						1 // 데이터 패킷
#define ACK							2 // 확인 응답 
#define FAIL						3 // 실패
#define SESSION_REQUEST				4 // connect 요청
#define SESSION_ACCEPTANCE			5 // accept
#define FILE_REQUEST				6 // 파일 전송 요청
#define FILE_REQUEST_ACCEPTE		7 // 파일전송 수락 accept
#define FILE_TRANSFER_COMPLETE		8 // 파일 전송 완료
#define FIN							9 // 연결 종료


//---------------------------------------------------------------------------------------------------
// 패킷 구조체 
//---------------------------------------------------------------------------------------------------
struct packet{
	char		buf[BUFFER_SIZE];   // 데이타 버퍼, size는 100으로 define함	
	long		seq;				// 패킷 순번
	int			ctrl_flag;			// 패킷의 내용을 나타내는 플래그 값 지정
}send_packet, recv_packet;


//---------------------------------------------------------------------------------------------------
// 함수 선언 
//---------------------------------------------------------------------------------------------------
void err_quit();					// 에러 발생시 종료 함수
void set_send_packet();				// 송신 패킷 설정함수
void send_state_print(int retval);  // 송신 상태 출력 함수


int main()
{
	
	WSADATA				wsa;					//윈속 초기화시 넣어줄 기본 자료 구조 지정
	int					retval;					//에러 처리시 사용할 값
	SOCKET				s;						//accept할 소켓 
	SOCKADDR_IN			serv_addr;				//주소를 지정해주는 구조체 

	struct sockaddr_in  client_addr;			//연결 요청한 Client의 내용 저장 
	int addr_size	  = sizeof(client_addr);


	//1. Winsock을 초기화 한다. 버전은 2.2
	retval  = WSAStartup(MAKEWORD(2,2), &wsa);
	if(retval != 0) err_quit();


	//2. 소켓을 생성한다.(UDP관련 내용으로 Setting) 
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == INVALID_SOCKET) err_quit();


	//3. 소켓의 주소 구조체의 내용을 넣는다.(주소 지정)  
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5150);
	serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//4. bid : 소켓과 ip, port를 연결시켜준다.
	retval = bind(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(retval == SOCKET_ERROR) err_quit();


	printf("\t========================================================\n");
	printf("\t=             [SERVER PROGRAM]                         = \n"); 
	printf("\t========================================================\n");
	printf("\n-->[server]를 실행하려면 엔턴키를 누르세요 : [ ]\b\b");
	if(getchar() == '\n')
	{
		printf("\n\n[server] : 연결 요청을 기다립니다.\n");
	}

	while(1) 
	{
		//--------------------------------------------------------------------
		// 5. recv : 클라이언트로부터 send 되는 패킷을 받는다. 
		//--------------------------------------------------------------------
		
		// 5.1 초기화
		memset(&recv_packet, 0, sizeof(packet));
		addr_size = sizeof(client_addr);
		
		//5.2 recvfrom
		retval = recvfrom(s, (char*)&recv_packet, sizeof(packet), 
							0, (struct sockaddr*)&client_addr, &addr_size);
		if(retval == SOCKET_ERROR) err_quit();
		else if(retval == 0) break;
		
		//5.3 정상적으로 들어오는 클라이언트의 패킷의 버퍼 내용을 출력한다.
		else
		{
			switch(recv_packet.ctrl_flag)
			{

			case SESSION_REQUEST:	//접속 요청
				printf("recv : SESION INIIATION REQUEST");
				break;

			case FILE_REQUEST:		//파일 요청
				printf("recv : FILE REQUEST --> a.txt\n");
				break;
			
			case ACK:				//파일 수신 확인 
				printf("recv : %s\n", recv_packet.buf) ;
				break;
			}
		}
	

		//--------------------------------------------------------------------
		// 6. send : 클라이언트로 패킷을 전송한다. 
		//--------------------------------------------------------------------

		//6.1 송신 패킷을 설정한다.
		set_send_packet(); // 송신 패킷 설정
		
		//6.2 연결된 클라이언트로 패킷을 전송한다.
		retval = sendto(s, (char*)&send_packet, sizeof(send_packet), 
							0, (struct sockaddr*)&client_addr, addr_size);
		
		//6.3 송신 상태를 출력한다.
		if(send_packet.ctrl_flag == DATA)
		{
			//송신된 데이터의 내용은 출력하지 않는다. 
		}
		else
		{
			send_state_print(retval); 
		}
		printf("\n");
	}

	//7. 소켓을 종료한다. 
	retval = closesocket(s);
	if(retval == SOCKET_ERROR) err_quit();

	//8. 윈속을 종료한다.
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

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, "에러", MB_OK | MB_ICONINFORMATION);

	LocalFree(lpMsgBuf);
	exit(1);
}


//---------------------------------------------------------------------------------------
// 함수   명 : send_state_print(int retval) 
//      내용 : 현재 송신 상태를 console 화면에 출력시켜준다. 
//      설명 : retval - 에러를 체크하기위해 sendTo시 리턴값을 받아 온다.
//				        만약 0이면 송신이 안됬다는 메시지를 출력하고,
//						그 이외의 값이면 패킷에 담긴 buf의 내용을 출력한다.
//---------------------------------------------------------------------------------------

void send_state_print(int retval) 
{
	if(retval == SOCKET_ERROR) err_quit();
	else if(retval == 0) 
	{
		printf("[ERROR] can't send data\n");
		exit(1);
	}
	else 
		printf("\nsend : %s\n", send_packet.buf);	
}


//---------------------------------------------------------------------------------------
// 함수   명 : set_send_packet()
//      내용 : 보내고자 하는 패킷의 내용을 초기화, 설정한다. 
//      설명 : 패킷의 flag 값에 따른 구분된 패킷 전송 
//---------------------------------------------------------------------------------------

void set_send_packet() 
{
	static long seq = 1;
	// 송신 패킷 초기화
	memset(&send_packet, 0, sizeof(send_packet));


	// 수신된 패킷의 플래그값에 따른 각기 다른 처리 
	switch(recv_packet.ctrl_flag) 
	{			

		case SESSION_REQUEST: 	// 세션 요청						
			send_packet.ctrl_flag = SESSION_ACCEPTANCE;
			sprintf(send_packet.buf, "SESSION_ACCEPTANCE\n");
			break;

		case FILE_REQUEST: // 파일 요청 처리를 받았을 경우 
			send_packet.ctrl_flag = FILE_REQUEST_ACCEPTE;
			printf("send : FILE_REQUEST_ACCEPT\n");
			sprintf(send_packet.buf, "a.txt");
			break;

		case ACK: 
		case DATA:   // 데이타 전송 
			FILE* fp;
			int readdata;
			if((fp = fopen("a.txt", "rb")) == NULL ) 
			{
				printf("-->error : Not found file;\n");
				exit(1);
			}
			fseek(fp, ((seq-1)*100), SEEK_SET);
			readdata = fread(send_packet.buf, 1, 100, fp);
			fclose(fp);
			if(readdata == 0) 
			{
				send_packet.ctrl_flag = FILE_TRANSFER_COMPLETE;
				sprintf(send_packet.buf, "FILE TRANSFER COMPLETE");		
			}
			else
			{
							send_packet.ctrl_flag  = DATA;
			send_packet.seq = seq;
			printf("send : data packet(seq, %d)", send_packet.seq);						
			seq++;

			}
	break;
	
		case FILE_TRANSFER_COMPLETE: // 파일 전송 완료							
			send_packet.ctrl_flag = FIN;
			printf("\n[SERVER] : 파일 전송이 완료되었습니다.\n\n");
			printf("[SERVER] : 연결을 종료합니다.\n\n");				
			sprintf(send_packet.buf, "FIN");
			break;	
	}
}