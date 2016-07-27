
// WSAAsyncSelect 모델 

 /* 
	WSAAsyncSelect() 함수가 핵심적인 역할을 한다는 뜻에서 붙인 이름임

    이 모델을 사용하면 윈도우 메시지 형태로 소켓과 관련된 네트워크
	이벤트를 처리할 수 있다. 
	모든 소켓과 관련 메시지는 하나의 윈도우, 즉 하나의 윈도우 프로시저
	로 전달되므로 멀티스레드를 사용하지 않고도 여러 소켓을
	처리 할 수 있다. 




//-----------------------------------
// 01  동작원리 
//-----------------------------------

  입출력 절차

  1) WSAAsyncSelect() 함수를 이용하여 소켓을 위한 윈도우 메시지와 처리
     할 네트워크 이벤트를 등록한다.
	 ex) 소켓을 통해 데이터를 보내거나 받을 수 있는 상황이 되면 특정 
	     윈도우 메시지로 알려달라는 내용을 등록한다.

  2) 등록한 네트워크 이벤트가 발생하면 윈도우 메시지가 발생하고 윈도우 
     프로시저가 호출된다.

  3) 윈도우 프로시저에서는 받은 메시지의 종류에 따라 적절한 소켓 함수를 
     호출하여 처리한다.


  WSAAsyncSelect() 함수 원형 

  int  WSAAsyncSelect (  SOCKET s, HWND hwnd, 
						unsigned int wMsg, long lEvent);

  1) 처리하고자 하는 소켓
  2) 메시지를 받을 윈도우를 나타내는 핸들값
  3) 윈도우가 받을 메시지 
     소켓을 위한 메시지는 따로 정의되어 있지 않으므로 사용자 저으이 
	 메시지를 이용한다.

  4) 처리할 네트워크 이벤트 종료를 비트 마스크 조합으로 나타낸다.

     FD_ACCEPT		: 클라이언트가 접속하면 윈도우 메시지 발생시킴
	 FD_READ		: 데이터 수신 가능하면 윈도우 메시지 발생시킴
	 FD_WRITE		: 데이터 송신이 가능하면 윈도우 메시지 발생시킴
	 FD_CLOSE		: 상대가 접속을 종료하면 윈도우 메시지 발생시킴
	 FD_CONNECT		: 접속이 완료되면 윈도우 메시지를 발생시킴
	 


  ex) 소켓 S에 대해 FD_READ & FD_WRITE 이벤트를 등록하는 예
  
  #define	WM_SOCKET (WM_USER + 1) 
  ..
  WSAAsyncSelect(s, hwnd, WM_SOCKET, FD_READ|FD_WRITE);


  * 사용시 유의점

  - WSAAsyncSelect() 함수를 호출하면 해당 소켓은 자동으로 넌블로킹 모드
    로 전환됨
	블로킹 모드는 메시지 루프를 정지시킬 가능성이 있기 때문에 넌블로킹
	모드만 사용되도록 규정되어 있다.

  - accept() 함수가 리턴하는 소켓은 연결 대기 소켓과 동일한 소켓을 지
    니게 된다.
	연결 대기 소켓은 직접 데이터를 송수신하지 않으므로 FD_READ, FD_WRITE
	이벤트를 처리하지 않는다.
	반면 자소켓은 FD_READ |FD_WRITE 이벤트를 처리해야 하므로, 
	다시 WSAAsyncSelect() 함수를 호출해서 속성을 변경해야 한다.

  - 윈도우 메시지에 대응하여 소켓 함수를 호출하면 대부분 성공하지만
    WSAEWOULDBLOCK 오류 코드가 발생하는 경우도 있으므로 이를 체크한다.

  - 윈도우 메시지를 받았을 때 적절한 소켓 함수를 호출하지 않으면, 다음 번
    에는 같은 윈도우 메시지가 발생하지 않는다.
	ex) FD_READ 이벤트와 대응하여 recv() 함수를 호출하지 않으면, 동일한 
	    소켓에 대하여 FD_READ 이벤트는 더 이상 발생하지 않는다.

    따라서 윈도우 메시지가 발생하면 해당되는 대응 함수를 호출해야 한다.

    * 이벤트 대응함수 

     FD_ACCEPT		: accept()
	 FD_READ		: recv() , recvfrom()
	 FD_WRITE		: send(), sendto()
	 FD_CLOSE		: 없슴
	 FD_CONNECT		: 없슴 


    * 네트워크 이벤트 발생시 윈도우 프로시져를 통해 전달되는 내용

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w , LPARAM l);

    1) 메시지가 발생한 윈도우 핸들
	2) WSAAsyncSelect() 함수 호출시 등록한 사용자 정의 메시지 
	3) 네트워크 이벤트가 발생한 소켓
	4) 하위 16비트 : 발생한 네트워크 이벤트
	   상위 16비트 : 오류 코드 

		#define WSAGETSELECTERROR(lParam) HIWORD(lParam)
		#define WSAGETSELECTEVENT(lParam) LOWORD(lParam)

  [ 실  습 ]

  echo 서버 구현 
*/


#pragma comment(linker, "/subsystem:console")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

// 소켓 정보 저장을 위한 구조체
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE+1];
	int recvbytes;
	int sendbytes;
	BOOL recvdelayed;
	SOCKETINFO *next;
};

SOCKETINFO *SocketInfoList;

// 윈도우 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);

// 소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
SOCKETINFO *GetSocketInfo(SOCKET sock);
void RemoveSocketInfo(SOCKET sock);

// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);
void err_display(int errcode);

int main(int argc, char* argv[])
{
	int retval;

	// 윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = NULL;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.lpszClassName = "MyWindowClass";
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	if(!RegisterClass(&wndclass)) return -1;

	// 윈도우 생성
	HWND hWnd = CreateWindow("MyWindowClass", "TCP 서버",
		WS_OVERLAPPEDWINDOW, 0, 0, 600, 300,
		NULL, (HMENU)NULL, NULL, NULL);
	if(hWnd == NULL) return -1;
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// WSAAsyncSelect()
	retval = WSAAsyncSelect(listen_sock, hWnd, 
		WM_SOCKET, FD_ACCEPT|FD_CLOSE);
	if(retval == SOCKET_ERROR) err_quit("WSAAsyncSelect()");

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

	// 메시지 루프
	MSG msg;
	while(GetMessage(&msg, 0, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 윈속 종료
	WSACleanup();
	return msg.wParam;
}

// 윈도우 메시지 처리
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, 
	WPARAM wParam, LPARAM lParam)
{
	switch(uMsg){
	case WM_SOCKET: // 소켓 관련 윈도우 메시지
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 소켓 관련 윈도우 메시지 처리
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 데이터 통신에 사용할 변수
	SOCKETINFO *ptr;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	// 오류 발생 여부 확인
	if(WSAGETSELECTERROR(lParam)){
		err_display(WSAGETSELECTERROR(lParam));
		RemoveSocketInfo(wParam);
		return;
	}

	// 메시지 처리
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			if(WSAGetLastError() != WSAEWOULDBLOCK)
				err_display("accept()");
			return;
		}
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		AddSocketInfo(client_sock);
		retval = WSAAsyncSelect(client_sock, hWnd, 
			WM_SOCKET, FD_READ|FD_WRITE|FD_CLOSE);
		if(retval == SOCKET_ERROR){
			err_display("WSAAsyncSelect()");
			RemoveSocketInfo(client_sock);
		}
		break;
	case FD_READ:
		ptr = GetSocketInfo(wParam);
		if(ptr->recvbytes > 0){
			ptr->recvdelayed = TRUE;
			return;
		}
		// 데이터 받기
		retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			if(WSAGetLastError() != WSAEWOULDBLOCK){
				err_display("recv()");
				RemoveSocketInfo(wParam);
			}
			return;
		}
		ptr->recvbytes = retval;
		// 받은 데이터 출력
		ptr->buf[retval] = '\0';
		addrlen = sizeof(clientaddr);
		getpeername(wParam, (SOCKADDR *)&clientaddr, &addrlen);
		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), ptr->buf);
	case FD_WRITE:
		ptr = GetSocketInfo(wParam);
		if(ptr->recvbytes <= ptr->sendbytes)
			return;
		// 데이터 보내기
		retval = send(ptr->sock, ptr->buf + ptr->sendbytes, 
			ptr->recvbytes - ptr->sendbytes, 0);
		if(retval == SOCKET_ERROR){
			if(WSAGetLastError() != WSAEWOULDBLOCK){
				err_display("send()");
				RemoveSocketInfo(wParam);
			}
			return;
		}
		ptr->sendbytes += retval;
		// 받은 데이터를 모두 보냈는지 체크
		if(ptr->recvbytes == ptr->sendbytes){
			ptr->recvbytes = ptr->sendbytes = 0;
			if(ptr->recvdelayed){
				ptr->recvdelayed = FALSE;
				PostMessage(hWnd, WM_SOCKET, wParam, FD_READ);
			}
		}
		break;
	case FD_CLOSE:
		RemoveSocketInfo(wParam);
		break;
	}
}

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	SOCKETINFO *ptr = new SOCKETINFO;
	if(ptr == NULL){
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	ptr->recvdelayed = FALSE;
	ptr->next = SocketInfoList;
	SocketInfoList = ptr;

	return TRUE;
}

// 소켓 정보 얻기
SOCKETINFO *GetSocketInfo(SOCKET sock)
{
	SOCKETINFO *ptr = SocketInfoList;

	while(ptr){
		if(ptr->sock == sock)
			return ptr;
		ptr = ptr->next;
	}

	return NULL;
}

// 소켓 정보 제거
void RemoveSocketInfo(SOCKET sock)
{
	// 클라이언트 정보 얻기
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR *)&clientaddr, &addrlen);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n", 
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	SOCKETINFO *curr = SocketInfoList;
	SOCKETINFO *prev = NULL;

	while(curr){
		if(curr->sock == sock){
			if(prev)
				prev->next = curr->next;
			else
				SocketInfoList = curr->next;
			closesocket(curr->sock);
			delete curr;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
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

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %s", (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

