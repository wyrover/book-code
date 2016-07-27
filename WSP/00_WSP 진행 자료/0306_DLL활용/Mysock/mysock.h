//mysock.h


#ifdef DLL_SOURCE
	#define DLLFUNC	__declspec(dllexport)   
#else
	#define DLLFUNC __declspec(dllimport)
#endif

typedef void (*RECVFUN)(char*, int);

//===================================================
//라이브러리 초기화 해제
//===================================================
EXTERN_C DLLFUNC int mysock_init();
EXTERN_C DLLFUNC void mysock_exit();

EXTERN_C DLLFUNC void mysock_inirproc(RECVFUN OnRecv);

//====================================================
// 소켓 생성 
//====================================================
EXTERN_C DLLFUNC void mysock_recvsock(int size, int port);
void recvthread(void* p);

EXTERN_C DLLFUNC void mysock_sendsock(int size, int port);


//====================================================
// 데이터 전송
EXTERN_C DLLFUNC void mysock_senddata(char* data, int length);


//====================================================
// 오류 관련 함수 
//====================================================
void err_quit(char *msg);
void err_display(char *msg);