/******************************************************************

  Chapter 1. 윈속에 대한 소개 

	1) 윈속의 헤더와 라이브러리
	2) 윈속의 초기화 
	3) 윈속의 해제 
	4) 에러 검출과 처리
	5) example

******************************************************************/


//---------------------------------------------------------------- 
//   1) 윈속의 헤더와 라이브러리 
//---------------------------------------------------------------- 
/*
    winsock : 하나 이상의 응용프로그램들이 같은 장치 내에서 혹은 
	          네트웤을 통하여 통신하기 위한 프로그래밍 인터페이스
			  (API)
			  즉, 프로토콜이 아니다. 
			  TCP/IP 나  IPX 와 같은 네트웤 프로토콜에 대한 프로그래
			  밍 인터페이스를 제공한다. 

    버전           

	  윈속 1 
	   winsock.h   /  wsock32.lib 링크 

      윈속 2
	   winsock2.h  /  ws2_32.lib 링크 
	   WSA로 시작되는 함수들(새로 추가되거나 수정된 함수들)
	   ex) WSASocket...

           WSAStaratup, WSACleanup, WSARecvEx, WSAGetLastError...
		   등은 Winsock1.1에서 정의된 함수들 
         

      * Windows CE를 제외한 모든 윈도우 플렛폼에서 모두 가능 
*/



//---------------------------------------------------------------- 
//   2) 윈속의 초기화  
//---------------------------------------------------------------- 
/*
	모든 윈속 응용프로그램은 적절한 버전의 윈속 DLL을 로드한다.
	만일 윈속 라이브러리를 로드하지 않고 윈속 API를 사용하면 
	SOCKET_ERROR 라는 에러가 리턴된다.
	이때 WSAGetLastError() 를 호출하면 에러 코드를 얻을 수 있다.

    
    int WSAStartup(
		WORD wVersionRequested,
		LPWSADATA lpWSAData
	);

    성공시 : 0
	실패시 : GetLastError를 사용하지 않음 
			 WS2_32.dll이 메모리에 로드되지 않는다.
			이 경우 WSAGetLastError() 함수가 리턴하는 값이 부정확 
			하므로 함수의 리턴값으로 오류 코드가 리턴된다. 

    1인자  : 로드하기를 원하는 윈속 라이브러리의 버전 
	         상위 바이트(윈속 마이너 버전)
			 하위 바이트(윈속 메이저 버전)
			 MAKEWORD(x, y) 매크로를 주로 사용 

    2인자  : 로드된 라이브러리에 대한 정보 리턴 

   
	*) 플렛폼별 윈속 버전 

    Windows 95		1.1(2.2)	 Windows 98		2.2
    Windows Me		2.2			 Windows NT		2.2
    Windows 2000	2.2			 Windows XP		2.2
    Windows CE		1.1

*/



//---------------------------------------------------------------- 
//   3) 윈속의 해제
//---------------------------------------------------------------- 
/*
	윈속 인터페이스의 사용이 끝났으면
	점유하고 있던 리소스를 해제

    int WSACleanup(void);

	윈속 사용을 중지함을 운영체제에 알려 관련 리소스를 반환하는 
	역할을 한다.
    함수 호출이 실패할 경우 


*/



//---------------------------------------------------------------- 
//   4) 에러 검출과 처리
//---------------------------------------------------------------- 
/*
	윈속 응용프로그램을 만들기 위해서 필수 불가결한 요소

    대부분의 윈속 함수들의 에러 발생시 SOCKET_ERROR리턴(-1)

    반드시 WSAGetLastError()함수를 호출하여 에러 원인 확인

    int WSAGetLastError(void);

    에러 코드값은 윈속 버전에 따라 WINSOCK.h or WINSOCK2.h
	에 정의되어 있슴

    네트워크 프로그램에서는 여러 원인으로 인해 오류가 발생할 수 있다. 
    적절한 오류 처리가 필수적이다. 

   윈속함수의 오류 처리 3가지 유형
   1) 오류 처리를 할 필요가 없는 경우 
      리턴값이 없거나 호출 시 항상 성공하는 일부 소켓 함수 
   2) 리턴값만으로 오류를 처리하는 경우
      WSAStartup() 함수
   3) 리턴값으로 오류 발생을 확인하고 구체적인 내용은 오류 코드를 
      이용하여 확인하는 경우 대부분의 소켓 함수 


   => 오류 코드 얻는 함수 
      int WSAGetLastError(void);

      사용예)
	  if( 소켓함수(...) == 오류 )
	  {
			int errcode = WSAGetLastError();
			printf(errorcode에 해당하는 오류메시지);
	  }

       - 리턴값은 오류 코드임 => 
	     적절하게 문자열 형태로 출력하는 것이 바람직
		 FormatMessage() 함수로 가능함 

      DWORD FormatMessage {
		DWORD dwFlags,	LPCVOID lpSource, DWORD dwMessageId, 
		DWORD dwLanguageId, LPTSTR lpBuffer, DWORD nSize, 
		va_list *Arguments
	  };
	  => 성공 : 오류 메시지 길이,  실패 : 0

      1) dwFlags 
		 FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
		 => 오류 메시지를 저장할 공간을 함수가 알아서 할당한다는 의미
		    운영체제로부터 오류 메시지를 가져온다는 의미 
			위와 같이 설정할 경우 
			함수의 두 번째 인자에는 NULL값, 여섯번째, 일곱번째 인자에는 
			0, NULL값을 사용하면 된다.
	  3) dwMessageId
	     오류 코드를 나타내며, WSAGetLastError()함수의 리턴값을 사용한다.
      4) dwLanguageId
	     오류 메시지를 어떤 언어로 표시할 것인지를 나타내며,
		 MAKEELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)를 사용하면
		 제어판에서 설정한 기본언어로 오류 메시지를 얻을 수 있다. 
	  5) lpBuffer
	     오류 메시지의 시작 주소가 여기에 저장된다.
		 시스템에서 할당한 메모리를 반환하여야 한다. LocalFree()
       => 위의 내용대로 구현해보자
	   
	  #include <winsock2.h>
	  #include <stdlib.h>

	  void err_quit( char *msg)
	  {
		LPVOID lpMsgBuf;
		FormatMessage (
		   FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL, WSAGetLastError(), 
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAUL),
		   (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	  }
	  => 사용예
	  if( socket(...) == SOCK_ERROR)	err_quit("socket()");
	  if( bind(...)   == SOCK_ERROR)	err_quit("bind()");

      * 위의 함수 변형
	    MessageBox 대신 printf를 사용
		오류메시지 출력 후 애플리케이션을 종료하지 않음 
	  void err_display( char *msg)
	  {
		LPVOID lpMsgBuf;
		FormatMessage (
		   FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL, WSAGetLastError(), 
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAUL),
		   (LPTSTR)&lpMsgBuf, 0, NULL);
		printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
	  }

*/



//---------------------------------------------------------------- 
//   5) example
//---------------------------------------------------------------- 

#include <stdio.h>
#include <winsock2.h>

void main()
{
	WSADATA		wsaData;
	int			Ret;

	// 1. 윈속 2.2로 초기화
	//    윈속 로드시에 실패한 경우 WSAGetLastError()사용 불가
	//    리턴값을 확인해라
	if( ( Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0 )
	{
		printf("WSAStartup failed with error %d\n", Ret);
		return;
	}

	// 2. 윈속 사용....


	// 3. 윈속 사용 종료 
	if( WSACleanup() == SOCKET_ERROR )
	{
		printf("WSACleanup failed with error %d\n", 
					WSAGetLastError() );
	}
}
