//myservice.h

//초기화
void myservice_init();

//실행
void myservice_run();

//추가 핸들러
DWORD WINAPI MainThread(LPVOID temp);
DWORD WINAPI PipeThread(LPVOID temp);