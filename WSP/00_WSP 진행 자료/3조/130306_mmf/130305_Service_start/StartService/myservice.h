//myservice.h
void myservice_init();
void myservice_run();
void myservice_exit();
void myservice_newfile();


DWORD WINAPI ThreadFunc(LPVOID temp);

void myrefun(char*,int,SOCKET);

void mmf_init();