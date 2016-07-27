//pipe.h

typedef void (*FUNC)(char*, int);

void pipe_Create(FUNC func, int size);
void recvthread(void* p);

void pipe_Write(char *buf, int length);