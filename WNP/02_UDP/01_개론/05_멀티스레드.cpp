/******************************************************************

  Chapter 1. 윈속에 대한 소개 

	1) 멀티 스레드  
******************************************************************/

/*
학습 목표 
  멀티스레드의 필요성을 이해하고 기본 개념을 익힌다.
  멀티스레드를 이용하여 TCP 서버를 작성한다.
  스레드 동기화 기법을 익힌다.


1. 멀티스레드 기초
    이전 소스의 문제점 : 

	1) 하나의 클라이언트만 서버 접속 가능
	2) 서버와 클라이언트의 send(), recv() 함수 호출 순서가 맞아야 
	   한다.
	   => 틀리면 교착상태(deadlock) 이 발생 => 영원히 일어나지 않을 
	      사건을 두 프로세스가 기다리는 상황 


    * 1)번 문제의 해결책과 장단점 
	 - 서버가 하나의 클라이언트와 통신하는 시간을 짧게 한다. 
	   즉, 클라이언트는 매번 데이터를 보내기 전에 서버에 접속하고 
	    데이터를 주고 받은 후 
	       곧바로 접속을 끊는다.
		 장점) 서버의 시스템 자원을 적게 소모
		       특별한 기법없이 구현 가능
		 단점) 파일 전송 등 대용량의 데이터 전송시 애플리케이션으로
		       비적합 
		       서버에 접속하려는 클라이언트의 수가 많을 경우 접속 실패 
			   발생 가능

     - 서버에 접속한 클라이언트를 스레드를 이용하여 독립적으로 처리 
	    장점)  비교적 쉽게 구현 
		단점)  접속한 클라이언트 수에 비례하여 스레드 생성하므로 서버의
		       시스템 자원 소모가 상대적으로 크다. 

     - 소켓 입출력 모델 사용
	    장점) 하나 또는 소수의 스레드를 이용하여 다수의 클라이언트 처리
		      가능 
		      두번째 방법에 비해 상대적 시스템 자원 소모 감소 
		단점) 다른 두 방법에 비해 구현이 어렵다. 

   * 2) 번 문제의 해결책과 장단점
      - 데이터 송수신 부분을 잘 설계하여 교착 상태가 발생하지 않도록 
	    한다.
	    장점) 특별한 기법없이 구현 가능
		단점) 데이터 송수신 패턴에 따라 교착 상태 발생 가능
		      모든 경우에 적용 불가 
	 
	  - 소켓에 타임아웃(timeout) 옵션 적용, send(), recv() 함수 호출 시 
	    일정 시간이 지나면 리턴 하도록 구현
		장점) 비교적 간단히 구현 가능
		단점) 다른 방법에 비해 성능이 떨어진다.

      - 넌블로킹(nonblocking) 소켓 사용
	    장점) 교착 상태를 막을 수 있다. 
		단점) 구현이 복잡하다. 시스템 자원(CPU시간) 을 불필요하게 
		      낭비할 가능성

      - 소켓 입출력 모델을 사용
	    장점) 블로킹과 넌블로킹 소켓의 단점을 보완, 교착 상태 
		      막을 수 있다. 
		단점) 첫번째 방법에 비해 구현이 어렵다. 
		      두번째 세번째 방법보다는 쉽고, 일관성 있게 구현 
			  가능하다. 

     => 여기서는 첫번째 문제 해결 방법중 하나인
	    멀티 스레드에 대한 이해를 한다.


1) 기본 개념 
   - 프로세스(process) : 메모리를 비롯한 각종 리소스를 담고 있는
                         컨테이너로서 정적인 개념
   - 스레드(thread)    : 실제 CPU 시간을 할당 받아 수행되는 실행 
                         단위 로서 동적인 개념 

     최소 하나의 스레드 필요 , main, WinMain 함수에서 시작되는 
	 스레드를 primary thread라 한다.
	 컨텍스트 전환(context switch) : 운영체제에서 이루어지는
	 스레드 실행 상태의 저장과 복원작업

     작업 관리자를 확인해보면 일반적으로 두 개 이상의 스레드를 
	 사용하는 애플리케이션이 단일 스레드를 사용하는 것보다 많다. 
	  
     콘솔, GUI 애플리케이션 모두 멀티스레드 구현 가능하다. 
	 상대적으로 이벤트 방식의  GUI 환경에서 더 많이 사용된다. 

2) 스레드의 생성과 종료 
   - 스레드 생성에 필요한 요소 
     -> 함수의 시작 주소( 스레드 함수라 함- thread funtion )
	 -> 스레드 함수 실행시 사용할 스택 영역의 크기 
        스레드가 실행될때마다 1M의 새로운 스택영역에 제공된다. 
		2G / 1MB = 2048개의 스레드 생성 가능?

     CreateThread 함수를 통한 생성

   - 스레드 종료 방법
     스레드 함수 리턴
	 스레드 함수 내에서 ExitThread() 함수 호출
	 TerminateThread() 함수 호출
	 주 스레드가 종료하면 모든 스레드가 종료 


     #define CALLBACK	__stdcall
	 #define WINAPI		__stdcall
	 #define APIENTRY	WINAPI
	 #define PASCAL		__stdcall

     stdcall 은 표준 윈도우 API 에서 사용하는 호출 규약임 
	 반드시 stdcall 호출규약을 따라야 한다. 

     일반적인 윈도우 API 함수는 애플리케이션에서 호출하는 데 반해 
	 스레드 함수는 운영체제가 직접 호출한다.   
	 운영체제가 __stdcall 호출 규약에 따라 스레드 함수를 호출시킴으로 
	 사용자가 정의한 스레드 함수도 동일한 호출 규약을 따라야 한다. 
	 이러한 약속을 강제하는 것이 WINAPI 키워드...
	 HANDLE WINAPI CreateThread(...)


     애플리케이션   API 함수 호출				스레드 함수 호출

                        
		운영체제    API 함수 코드				스레드 함수 호출 

  [ Thread Sample ]
*/

/*

//-------------------------------------
// 1. Sample : 멀티 스레드 
//-------------------------------------

#include <windows.h>
#include <stdio.h>

struct Point3D
{
	int x, y, z;
};

DWORD WINAPI MyThread(LPVOID arg)
{
	Point3D *pt = (Point3D *)arg;
	while(1){
		printf("Running another thread: %d, %d, %d\n", 
			pt->x, pt->y, pt->z);
		Sleep(1000);
	}

	return 0;
}

int main()
{
	// 첫 번째 스레드 생성
	Point3D pt1 = {10, 20, 30};
	DWORD ThreadId1;
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, 
		(LPVOID)&pt1, 0, &ThreadId1);
	if(hThread1 == NULL) return -1;
	CloseHandle(hThread1);

	// 두 번째 스레드 생성
	Point3D pt2 = {40, 50, 60};
	DWORD ThreadId2;
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, 
		(LPVOID)&pt2, 0, &ThreadId2);
	if(hThread2 == NULL) return -1;
	CloseHandle(hThread2);

	while(1){
		printf("Running primary thread...\n");
		Sleep(1000);
	}

	return 0;
}
*/



//-------------------------------------
// 2. Sample : 스레드 우선 순위 변경 : 아사 상태가 되지는 않는다.
//-------------------------------------
/*
#include <windows.h>
#include <stdio.h>

DWORD WINAPI MyThread(LPVOID arg)
{
	while(1)
		printf("Running MyThread()\n");

	return 0;
}

int main()
{
	// 스레드 생성
	DWORD ThreadId;
	HANDLE hThread = CreateThread(NULL, 0, MyThread, NULL, 0, &ThreadId);
	if(hThread == NULL) return -1;

	// 우선순위 변경
	SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);

	while(1)
		printf("Running main()....................\n");

	return 0;
}
*/



//-------------------------------------
// 3. Sample : 스레드 대기 
//-------------------------------------
/*
#include <windows.h>
#include <stdio.h>

int sum = 0;

DWORD WINAPI MyThread(LPVOID arg)
{
	int num = (int)arg;
	for(int i=1; i<=num; i++)
		sum += i;

	return 0;
}

int main()
{
	// 스레드 생성
	DWORD ThreadId;
	int num = 100;
	HANDLE hThread = CreateThread(NULL, 0, MyThread, 
		(LPVOID)num, CREATE_SUSPENDED, &ThreadId);
	if(hThread == NULL) return -1;

	printf("스레드 실행 전. 계산 결과 = %d\n", sum);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	printf("스레드 실행 후. 계산 결과 = %d\n", sum);
	CloseHandle(hThread);
	
	return 0;
}
*/





/*
2. 멀티스레드 TCP 서버

  멀티스레드를 이용한 TCP 서버 구현

  멀티 스레드 TCP 서버의 기본 형태 

  DWORD WINAPI ProcessClient(LPVOID arg)
  {
	 // 전달된 소켓(3)
	 SOCKET client_sock = (SOCKET)arg;

	// 클라이언트 정보 얻기(4)
	addrlen	= sizeof(clientaddr);

    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

   // 클라이언트와 데이터 통신(5)
   while(1)
   {
		....
   }
   closesocket(client_sock);
  }

  int main()
  {
	...
	whlie(1)
	{
		// 클라이언트 접속 수용(1)
		client_sock = accept( listen_sock, ...);

		..

		// 스레드 생성(2)
		CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, &threadId);
	}
	...
  }

  (1) 클라이언트가 접속하면 accept() 함수는 소켓을 리턴한다.
  (2) 클라이언트 당 하나의 스레드를 생성한다.  이때 스레드 함수에 연결된 소켓을 넘겨준다.
  (3) 스레드 함수는 인자로 전달된 소켓을 SOCKET 타입으로 케스팅 하여 저장한다.
  (4) getpeername() 함수를 호출하여 클라이언트의 IP, PORT 를 얻는다.
      이 코드는 필수적인 부분이 아니며, 클라이언트 정보를 출력할 때만 필요하다. 
  (5) 클라이언트와 통신을 수행한 후 접속을 종료한다.


  * int getpeername(	SOCKET s, struct sockaddr* name, int *namelen	);

  * int getsockname(	SOCKET s, struct sockaddr* name, int *namelen	);

  => 성공 0, 실패 : SOCKET_ERROR

  getpeername()
    데이터 구조체에 저장된 원격 IP 주소와 원격 포트 번호 리턴
  getsockname()
    데이터 구조체에 저장된 지역 주소와 지역 포트 번호 리턴 


  [ TCP Server 예제 수정 ]



3. 스레드 동기화 
   
	공유 변수 접근시 문제 발생 가능 

    임계영역(Critical Section)
		: 공유 리소스에 대해 오직 하나의 스레드만 접근 허용

    뮤택스(Mutex)
		: 공유 리소스에 대해 오직 하나의 스레드만 접근 허용

    세마포어(semaphore)
		: 한정된 개수의 자원을 여러 스레드가 사용하려고  할 때 접근 제한

    대기 가능 타이머
		: 특정 시간이 되면 대기 중인 스레드를 깨운다.


   * 일반적으로 동기화가 필요한 상황
   (1) 두 개 이상의 스레드가 공유 리소르를 접근할 때, 오직 한 스레드만 
       접근을 허용해야 하는 경우 

   (2) 특정 사건 발생을 다른 스레드에게 알리는 경우, 
       예를 들면, 한 스레드가 작업을 완료한 후 대기 중인 다른 스레드를 
	   깨우는 경우 


  [ Critical Section ]

*/

/*
#include <windows.h>
#include <stdio.h>

int A[100];
CRITICAL_SECTION cs;

DWORD WINAPI MyThread1(LPVOID arg)
{
//	EnterCriticalSection(&cs);
	for(int i=0; i<100; i++){
		A[i] = 3;
		Sleep(10);
	}
//	LeaveCriticalSection(&cs);

	return 0;
}

DWORD WINAPI MyThread2(LPVOID arg)
{
//	EnterCriticalSection(&cs);
	for(int i=99; i>=0; i--){
		A[i] = 4;
		Sleep(10);
	}
//	LeaveCriticalSection(&cs);

	return 0;
}

int main()
{
	// 임계영역 초기화
	InitializeCriticalSection(&cs);

	// 두 개의 스레드 생성
	HANDLE hThread[2];
	DWORD ThreadId[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1,
		NULL, 0, &ThreadId[0]);
	hThread[1] = CreateThread(NULL, 0, MyThread2,
		NULL, 0, &ThreadId[1]);

	// 스레드 종료 대기
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// 임계영역 제거
	DeleteCriticalSection(&cs);

	// 결과 출력
	for(int i=0; i<100; i++){
		printf("%d ", A[i]);
	}
	printf("\n");
	
	return 0;
}
*/


/*

  이벤트 

  특정 사건의 발생을 알리는 데 주로 사용

  1) 이벤트를 비신호 상태로 생성

  2) 한 스레드가 작업을 진행하고, 나머지 스레드는 이벤트에 대해 
     Wait* () 함수로 호출함으로써 이벤트가 신호 상태가 되기를 기다린다.

  3) 스레드가 작업을 완료하면, 이벤트를 신호 상태로 바꾼다.

  4) 기다리고 있던 모든 스레드가 깨어나서 작업을 진행한다.
  
*/

/*
#include <windows.h>
#include <stdio.h>

#define BUFSIZE 16

HANDLE hReadEvent;
HANDLE hWriteEvent;
char buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg)
{
	DWORD retval;

	for(int k=0; k<10; k++){
		// 읽기 완료를 기다림
		retval = WaitForSingleObject(hReadEvent, INFINITE);		// 종료 상태 되기를 기다림
		if(retval == WAIT_FAILED) break;						// 처음은 신호 상태이므로 진행됨 

		// 공유 버퍼에 데이터 쓰기
		for(int i=0; i<BUFSIZE; i++)
			buf[i] = 3;

		// 쓰기 완료를 알림
		SetEvent(hWriteEvent);
	}

	// 이벤트 제거
	CloseHandle(hWriteEvent);

	return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
	DWORD retval;

	while(1){
		// 쓰기 완료를 기다림
		retval = WaitForSingleObject(hWriteEvent, INFINITE);	// 신호상태가 되기를 대기 
		if(retval == WAIT_FAILED) break;

		// 읽은 데이터를 출력
		printf("Thread %d:\t", GetCurrentThreadId());
		for(int i=0; i<BUFSIZE; i++)
			printf("%d ", buf[i]);
		printf("\n");
		
		// 버퍼를 0으로 초기화
		ZeroMemory(buf, sizeof(buf));

		// 읽기 완료를 알림
		SetEvent(hReadEvent);
	}

	return 0;
}

int main()
{
	// 이벤트 생성
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);			// 신호상태
	if(hReadEvent == NULL) return -1;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);		// 비신호상태 
	if(hWriteEvent == NULL) return -1;

	// 세 개의 스레드 생성
	HANDLE hThread[3];
	DWORD ThreadId[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread,				// 쓰기
		NULL, 0, &ThreadId[0]);
	hThread[1] = CreateThread(NULL, 0, ReadThread,				// 읽기
		NULL, 0, &ThreadId[1]);
	hThread[2] = CreateThread(NULL, 0, ReadThread,				// 읽기 
		NULL, 0, &ThreadId[2]);

	// 스레드 종료 대기
	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);			// 종료 대기(모두)

	// 이벤트 제거
	CloseHandle(hReadEvent);

	printf("모든 작업을 완료했습니다.\n");

	return 0;
}


*/
/*
[ 요 약 ]
1. 윈도우에서 프로세스는 메모리를 비롯한 각종 리소스를 담고 있는 컨테이너로써 정적인 개념이고
    스레드는 실제 CPU 시간을 할당받아 수행되는 실행 단위로서 동적인 개념이다.

2. main() 또는 WinMain() 함수에서 시작되는 스레드를 주 스레드라 부르며, 프로세스가 시작할 때
  생성된다.

3. CreateThread() 함수는 스레드를 생성한 후 스레드 핸들을 리턴한다.

4. CreateThread() 함수에 전달하는 핵심 요소는 스레드 함수 주소와 함수 인자다.

5. C/C++ 라이브러리 함수를 사용하는 애플리케이션에서는 CreateThread(), ExitThread() 함수보다는

   _beginthreadex(), _endthreadex() 함수를 사용하는 것이 바람직하다.

6. 윈도우는 각 스레드에 CPU 시간을 적절히 분배하기 위한 정책을 사용하는데, 이를 스레드 스케줄링
   또는 CPU 스케줄링 이라 부른다. 

7. SetThreadPriority() 함수는 우선순위 레벨을 변경할 때, GetThreadPriority() 함수는 우선 순위 
   레벨 값을 얻을 떄 사용한다.

8. WaitForSingleObject(), WaitForMultipleObject() 함수를 사용하면 특정 스레드가 종료할 때까지 대기
   할 수 있다.

9. SuspendThread() 함수는 스레드 실행을 일시 중지하고, ResumeThread() 함수는 스레드를 재실행한다.
   한편 Sleep() 함수는 스레드가 실행을 멈추고 일정 시간 동안 대기하도록 한다.

10. getpeername() getsockname() 함수는 소켓만 가지고 있을 떄 이 소켓과 연관된 주소 정보를 리턴하는 
    역할을 한다.

11. 스레드 동기화란 멀티스레드환경에서 발생하는 문제를 해결하기 위한 일련의 작업을 뜻한다.


12. 동기화 객체는 평소에는 비신호 상태로 있다가 특정 조건이 만족되면 신호 상태가 된다.
    비 신호 => 신호 상태는 Wait*()함수를 사용하여 감지할 수 있다.

13. 임계 영역은 두 개 이상의 스레드가 공유 리소스를 접근할 때, 오직 하나의 스레드 접근만 허용해야 
    하는 경우에 사용된다. 
*/

