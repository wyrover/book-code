//pipe.h

typedef void (*FUNC)(char*, int);

void pipe_Create1(HANDLE hread, HANDLE hwrite, FUNC func, int size);

void recvthread(void* p);

void pipe_Write(char *buf, int length);