//mysock.h

typedef void (*RECVFUN)(char*, int);

//===================================================
//라이브러리 초기화 해제
//===================================================
int mysock_init();
void mysock_exit();

void mysock_inirproc(RECVFUN OnRecv);

//====================================================
// 소켓 생성 
//====================================================
void mysock_recvsock(int size, int port);
void recvthread(void* p);

void mysock_sendsock(int size, int port);


//====================================================
// 데이터 전송
void mysock_senddata(char* data, int length);


//====================================================
// 오류 관련 함수 
//====================================================
void err_quit(char *msg);
void err_display(char *msg);