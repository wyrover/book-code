//client.h
#ifdef DLL_SOURCE
	#define DLLFUNC	__declspec(dllexport)   
#else
	#define DLLFUNC __declspec(dllimport)
#endif

EXTERN_C DLLFUNC int client_init();
EXTERN_C DLLFUNC int client_sendpack(char*);

EXTERN_C DLLFUNC int client_exit();
void err_quit(char *msg);
void err_display(char *msg);