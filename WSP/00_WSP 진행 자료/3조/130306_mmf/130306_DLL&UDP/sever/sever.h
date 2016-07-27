//sever.h

#ifdef DLL_SOURCE
	#define DLLFUNC	__declspec(dllexport)   
#else
	#define DLLFUNC __declspec(dllimport)
#endif



EXTERN_C DLLFUNC int sever_init();
EXTERN_C DLLFUNC int server_run();
EXTERN_C DLLFUNC int sever_exit();


typedef void (*RecvFun)(char*,int,SOCKET);
EXTERN_C DLLFUNC void sock_init(RecvFun);
