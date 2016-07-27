/******************************************************************

  Chapter 1. 윈속에 대한 소개 

	1) 소켓 옵션  
******************************************************************/
/*
학습 목표 

	TCP/IP 애플리케이션에 적용 가능한 다양한 소켓 옵션을 이해하고
	활용한다.


 
//-------------------------------------------------
// 01. 개요 
//-------------------------------------------------

    소켓 프로그래밍?
	  - 소켓을 통해 간접적으로 TCP, IP 등과 같은 하부 프로토콜을 
	    다룸으로써 애플리케이션을 작성하는 것을 의미 

      - 즉, 소켓코드가 제공하는 인터페이스인 소켓 함수를 호출함으로써
	     하부 프로토콜이 제공하는 다양한 기능을 사용하는 것임 


    지금까지는 특별한 변경없이 소켓 함수를 그대로 호출함으로 구현함

    소켓 옵션(socket options)를 적용하면 소켓 함수의 기본 동작을 변경
	가능하다. 
	이를 이용하여 애플리케이션은 소켓 코드와 프로토콜 구현 코드에 대한 
	좀더 세부적인 제어를 할 수 있게 된다.



    소켓 옵션은 처리하는 코드의 위치에 따라 두 종류로 구분된다.


    1) 소켓 코드가 담당하는 부분
	   옵션을 설정하면 소켓 코드에서 해석하고 처리한다.
	   프로토콜 독립적인 성격이 있으나 옵션의 실제적인 적용은 프로토콜의 
	   종류에 따라 달라진다. 

    2) 프로토콜 구현 코드가 담당하는 부분
	   옵션을 설정하면 프로토콜 구현 코드에서 해석하고 처리한다.
	   프로토콜 의존적인 성격이 있으므로 프로토콜의 종류에 따라 
	   옵션이 달라진다.



  *  소켓 옵션 설정함수  : setsockopt()

     int setsockopt ( SOCKET s, int level, int optname, 
					const char*optval, int optlen)

     1) 옵션을 적용할 소켓

	 2) 옵션을 해석하고 처리하는 코드, 옵션레벨이라 함
	    소켓 코드가 처리하면 SOL_SOCKET,
		IP 프로토콜 코드가 처리하면 IPPROTO_IP
		TCP 프로토콜 코드가 처리하면 IPPROTO_TCP를 사용


     3) 구체적인 옵션 이름

     4) 설정할 옵션값을 담고 있는 버퍼의 주소 

     5) optval이 가리키는 버퍼의 크기 


 
  * 소켓 옵션 획득 함수 : getsockopt()
     
	 int getsockopt ( SOCET s, int leval, int optname,
					  char* optval,  int *optlen);


     1) 옵션값을 얻을 소켓 

 	 2) 옵션을 해석하고 처리하는 코드, 옵션레벨이라 함
	    소켓 코드가 처리하면 SOL_SOCKET,
		IP 프로토콜 코드가 처리하면 IPPROTO_IP
		TCP 프로토콜 코드가 처리하면 IPPROTO_TCP를 사용


     3) 구체적인 옵션 이름

     4) 옵션값을 저장할 버퍼의 주소  

     5)  값-결과 인자, 함수 호출 후에는 옵션값의 크기가 저장됨 
	 


    [ 소켓 옵션 설정 ]

    => SOL_SOCKET

     optname		optval타입		get		set		설명
	 SO_BROADCAST	BOOL			o		o       브로드캐스팅 허용
	 SO_DONTROUTE	BOOL			o		o		데이터 전송시 라우팅 테이블 참조 
												    과정 생략
	 SO_KEEPALIVVE	BOOL			o		o		주기적으로 연결 여부 확인
	 SO_LINGER		linger()		o		o		보낼 데이터가 있을 경우 closesocket()함수지연
	 SO_SNDBUF		int				o		o		소켓 송/수신 버퍼 크기 설정
	 SO_RCVBUF
	 SO_SNDTIMEO	int				o		o		send().recv()등의 함수에 대한 타임아웃 설정
	 SO_RCVTIMEO
	 SO_REUSEADDR	BOOL			o		o		지역주소  재사용 허용 


    => IPPROTO_IP

      optname		optval타입		get		set		설명
	  IP_HDRNCL		BOOL			o		o		데이터를 보낼 때 IP 헤더 포함
	  IP_TTL		int				o		o		IP 패킷의 TTL(time-to-live) 변경
	  IP_MULTICAST_IF  IN_ADDR()	o		o		멀티케스트 패킷을 보낼 인터페이스 설정
	  IP_MULTICAST_TTL	in			o		o		멀티캐스트 패킷의 TTL 변경
	  IP_MULTICAST_LOOP	BOOL		o		o		멀티캐스트 패킷의 루프백 여부 설정
	  IP_ADD_MEMBERSHIP	ip_mreq()			o		멀티캐스트 그룹 가입과 탈퇴
	  IP_DROP_MEMBERSHIP



    => IPPROTO_TCP

      optname		optval타입		get		set		설명
	  TCP_NODELAY	BOOL			o		o		Nagle 알고리즘 작동 중지 


 

//-------------------------------------------------
// 02. SOL_SOCKET
//-------------------------------------------------

	SOL_SOCKET 레벨 옵션은 소켓 코드에서 해석하여 처리하므로 
	프로토콜 독립적이다. 
	그러나 여기에 속한 옵션을 모든 프로토콜에 적용할 수 있는 것은 아니다.


1.  SO_BROADCAST

  해당 소켓을 이용하여 브로드캐스트 데이터를 보낼 수 있다.
  프로토콜 특성상 TCP 소켓에는 사용 불가
  UDP 소켓에만 사용 가능


2. SO_DONTROUTE

  TCP/IP 애플리케이션이 보내는 데이터는 라우팅 테이블(routing table)
  이라 부르는 정보를 참조하여 하부의 IP프로토콜이 보내게 된다.

  이 옵션은 데이터 전송 시 라우팅 테이블 참조를생략하고 곧바로 
  bind() 함수로 설정한 네트워크 인터페이스로 모든 데이터를 보내게 된다.

  사용 예)
  BOOL optval	= TRUE;
  if( setsockopt( listen_socket, SOL_SOCKET, SO_DONTROUTE, 
			     (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
	{
		err_quit("setsocket()");
	}

  TCP / UDP 모두 사용 가능
  TCP 서버의 연결 대기 소켓에 대해 이 옵션을 설정해두면 자소켓도 
  동일한 옵션으로 설정된다.


  [ 라우팅 테이블 정보 출력 ] 
  
  netstat  -r



3. SO_KEEPLIVE

  TCP 프로토콜 수준에서 연결 여부를 확인하기 위해 상대 TCP에 주기적으로
  TCP 패킷을 보낸다.(약 2시간 간격)
  이때 상대 TCP의 반응에 따라 다음과 같은 두 경우로 나뉜다.

  1) 상대 TCP가 정해진 시간 이내에 응답을 하는경우 
     - TCP 연결에 문제가 없는 경우므로 애플리케이션은 이를 알아차리지
	   못한다.

  2) 상대 TCP가 정해진 시간 이내에 응답을 하지 않거나 RST(reset) 패킷(비정상종료)으로 
     응답할 경우 
	 - TCP 연결에문제가 있는 경우이므로 자동으로 소켓을 닫는다.
	   이 경우 애플리케이션이 해당 소켓으로 데이터 통신을 하려고 하면 오류가 
	   발생한다.

  왜 이 옵션을 사용할까?

  자원의 효율성...
  데이터 교환 없이는 상대 호스트가 다운되거나 전원이 나간 경우를 감지하지 못함
  TCP 프로토콜 수준에서 주기적으로 끊어진 연결을 감지하여 불필요하게 열린 소켓
  을 담을 수 있게 됨 

  TCP 소켓에만 사용할 수 있다. 
  보통 대기 소켓에 설정하고, 자동으로 자소켓도 이 옵션으로 설정된다. 

  BOOL optval = TRUE;
  if( setsockopt(listen_sock, SOL_SOCKET, SO_KEEPALIVE, 
				(char*)&optval, sizeof(optval) ) == SOCKET_ERROR)
	{
		err_quit("setsockopt()");
	}


4. SO_LINGER

   TCP 소켓에서의 closesocket()의 의미
   1) TCP 프로토콜 수준에서 연결 종료 절차를 작동한다. 
      TCP 연결 종료를 위한 패킷 교환이 발생한다.

   2) 소켓과 관련된 리소스를 운영체제에 반환한다.
      함수 호출이 끝나면 해당 소켓은 통신을 위해 재사용할 수 없게 된다.


   만약 closesocket() 함수 호출시 send() 함수로 보내려고 했던 데이터가 
   아직 송신버퍼에 남아 있을 경우??

   - closesocket()은 언제 리턴하나?
   - 송신 버퍼의 데이터를 보낼 것인가? 삭제할 것인가?
   - TCp 연결 종료는 어떤 방식으로 할 것인가?(정상 or 강제)


   1) closesocke() 함수 곧바로 리턴, 송신 버퍼의 데이터는 백그라운드로 
      보낸 후 TCP 연결 정상 종료

   2) 곧바로 리턴, 송신 버퍼의 데이터는 삭제, TCP 강제 종료 

   3) 송신 버퍼의 데이터를 모두 보내고 TCP 연결을 정상 종료한후 
      closesocket() 함수가 리턴함.
	  만약 일정 시간내에 송신 버퍼의 데이터를 모두 보내지 못하면
	  연결을 강제 종료한 후 함수 리턴한다. 데이터는 삭제한다. 


   => SO_LINGER 옵션은 closesocket() 함수의 이중적인 기능을 
      세부적으로 제어하기 위한 옵션이다. 
	  위의 3방식중 하나의 방식으로 동작케 한다. 


      다음의 구조체를 사용함
	  struct linger {
			u_short l_onoff;		//option on/off
			u_short l_linger;		//linger time
	  };
	  typedef struct linger LINGER;

      - l_onoff
	    이 값이 0이면 closesocket() 함수는 곧바로 리턴한다.
		0이 아니면 l_linger로 설정한 시간 동안 리턴하지 않고 대기한다.

      - l_linger
	    closesocket() 함수가 대기할 시간을 초단위로 설정한다.
		0 또는 양수 값을 사용한다.


      SO_LINGER 옵션 사용예

      LINGER	optval;
	  optval.l_onoff		= 1;	// linger on
	  optval.l_linger		= 10;	// 10초
	  if( setsockopt(sock, SOL_SOCKET, SO_LINGER,
					(char*)&optval, sizeof(optval) ) == SOCKET_ERROR)
	  {
		 err_quit("setsockopt()");
	  }

      => 옵션에 따른 함수 동작

      linger()				closesocket함수 동작
	  l_onoff	l_linger	
	  0			사용안함	1과 동일
	  1			0			2와 동일
	  1			양수		3과 동일

     => TCP 소켓에만 사용할 수 있다.
	    특히 TCP 서버의 연결 소켓에 옵션을 설정하면 자소켓도 
		동일한 옵션이 설정된다.


5. SO_SNDBUF,   SO_RCVBUF

   소켓의 송신 버퍼와 수신 버퍼 크기를 변경할 수 있다.
   두 옵션 사용방법은 동일하다.

   TCP 서버에서 연결 대기 소켓의 수신 버퍼 크기를 두배로 늘려보자....
   크기가 늘어났는지 확인키 위해 getsockopt() 함수를 다시 호출한다.

   // 수신 버퍼의 크기를 얻는다.
   int optval;
   int optval  = sizeof(optval);

   if( getsockopt( listen_sock, SOL_SOCKET, SO_RCVBUF, 
				(char*)&optval, &optlen) == SOCKET_ERROR)
	{
		err_quit("getsockopt()");
	}
	printf("수신 버퍼 크기 = %d 바이트\n", optval);

    // 수신 버퍼 크기를 두 배로 늘린다.
	optval *= 2;

    if(setsockopt( listen_sock, SOL_SOCKET, SO_RCVBUF, 
					(char*)&optval, sizeof(optval)) == SOCKET_ERROR)
	{
		err_quit("setsockopt()");
	}

    // 수신 버퍼 크기를 얻는다.
	optlen	= sizeof( optval);
	if( getsockopt( listen_sock, SOL_SOCKET, SO_RCVBUF, 
					(char*)&optval, &optlen) == SOCKET_ERROR)
	{
		err_quit("getsockopt()");
	}
	printf("새로 설정한 수신 버퍼 크기 = %d바이트\n", optval);


    => UDP 소켓은 언제든 이 함수로 소켓 버퍼 크기 변경 가능
	   TCP 소켓은 연결이 이루어지기 전에 변경해야 함.


6. SO_SNDTIMEO, SO_RCVTIMEO
    
   기본 소켓은 블로킹 소켓이다.
   타임아웃을 설정해두면 데이터 전송 함수들이 작업 완료와 
   상관없이 일정 시간 후 리턴하도록 할 수 있다. 

   밀리초(milliseconds) 단위로 타임 아웃 설정 가능 

   int optval = 3000;
   if( setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO,
			(char*)&optval, sizeof(optval)) == SOCKET_ERROR)
	{
		err_quit("setsockopt()");
	}

    UDP 클라이언트에 위의 옵션을 추가하면
	UDP 서버가 3초 이내에 응답하지 않도록 조작해두면,
	클라이언트 화면에는 오류 메시지가 뜬다. 

    오류메시지) 연결된 구성원으로부터 응답이 없어 연결하지 못했거나,
			    호스트로부터 응답이 없어 연결이 끊어졌습니다..

    TCP/ UDP 모두 사용 가능
	교착 상태 방지용
	상대가 일정 시간 이내에 응답하는지 여부를 알고 싶을 때
	유용하게 사용 


7. SO_REUSEADDR
   이미 사용 중인 IP 주소와 포트번호를 재사용할 수 있다.
   
   TCP/UDP 모두 사용 가능


   [ 소켓 옵션 확인 ]

				
//-------------------------------------------------
// 03. IPPROTO_IP
//-------------------------------------------------

   IPPROTO_IP 레벨 옵션은 IP 프로토콜 코드에서 해석하여 처리한다.
   따라서 AF_INET 주소 체계를 사용하는 소켓에 대해서만 적용할 수 있다.

   
   멀티캐스팅에 관련된 옵션임


1. 멀티캐스팅 개념

   하나 또는 여러 네트워크에 속한 선별된 개체끼리 통신할 수 있다.

   멀티캐스팅을 위한 IP 주소는 224.0.0.0 ~ 239.255.255.255 범위값을 
   가질 수 있다.

   고정된 상위 4비트를 제외한 28비트를 멀티캐스트 그룹ID라 하고, 
   이 값이 특정 그룹을 지정하는 용도로 사용된다.
   애플리메이션은 멀티캐스트 데이터를 수신하기 위해 멀티캐스트 그룹에 
   자유롭게 가입할 수 있고, 탈퇴할 수 있다.
   그룹 가입과 탈퇴는 소켓 옵션을 이용하여 할 수 있다. 


   * 멀티캐스팅의 특징
     1) 그룹 가입과 탈퇴가 자유롭고, 그룹 구성원 모두가 평등하다.
	 2) 멀티캐스트 데이터를 받으려면 반드시 그룹에 가입해야 한다.
	 3) 멀티캐스트 데이터를 보내기 위해 그룹에 가입할 필요는 없다. 
	    - 멀티캐스트 그룹에 가입하지 않은 호스트도 언제든지 특정 
		  멀티캐스트 그룹에 데이터를 보낼 수 있다.


  [ 실 습 ]

   - 송신자  : 사용자가 키보드로 입력한 문자열을(fgets) 멀티캐스트 주소
               로 보낸다.(sendto)

   - 수신자 :  멀티캐스트 그룹에 가입한다.
               멀티캐스트 데이터를 받아(recvfrom) 이를 문자열로 간주하여
			   무조건 화면에 출력한다.(printf)



2. 코드 분석 

   IP_MULTICAST_TTL

   - 멀티캐스트 패킷이 생성될 때 IP 해더의 TTL(time-to-live)필드는 디폴트값
     1로 설정된다.
	 이것은 멀티캐스트 패킷이 라우터 경계를 넘어갈 수 없음을 뜻한다.
	 라우터 경계를 넘어 특정한 범위까지 멀티캐스트 패킷을 보내려면
	 위의 옵션을 이용하여 TTL의 값을 변경시켜야 한다.

	// 멀티캐스트 TTL 설정
	int ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");



   IP_ADD_MEMBERSHIP, IP_DROP_MEMBERSHIP

   멀티캐스트 데이터를 받기 위해서는 반드시 그룹에 가입해야 한다. 

   #include <ws2tcpip.h>

   struct ip_mreg{
		struct in_addr   imr_multiaddr;	
		struct in-addr   imr_interface;
   };

   1) 가입하거나 탈퇴할 멀티캐스트 그룹 주소
   2) 멀티캐스트 패킷을 받을 네트워크 인터페이스
      INADDR_ANY를 사용하면 자동 설정됨 



   // 멀티캐스트 그룹 가입
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");


	// 멀티캐스트 그룹 탈퇴
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");




//-------------------------------------------------
// 04. IPPROTO_TCP
//-------------------------------------------------

	IPPROTO_TCP 레벨 옵션은 TCP 프로토콜 코드에서 해석하여 처리한다.
	따라서 TCP 소켓에 대해서만 적용할 수 있다.


    TCP는 신뢰성있는 데이터 전송을 위한 방법 중 하나로 ACK(acknowledgement)
	에 기반한 데이터 재전송을 한다.
	즉, 데이터를 보낸 후 상대방이 잘 받았는지 확인하고 그렇지 않을 경우 
	재전송하는 것이다.


               ---- 데이터1 --->

               <--- ACK2  -----

               -----데이터2 ---->

               <--- ACK3   -----

                [ TCP 동작 원리 ]

    => TCP는 마지막으로 성공적으로 수신한 데이터 번호에 1을 더한 값을 
	   ACK 번호로 보낸다. 즉 다음에 받을 데이터 번호를 ACK로 
	   보내는 것이다. 

    => 매번 TCP 패킷을 보낼때마다 ACK 가 도착할 때까지 대기하게 되면 
	   네트워크 자원 활용도가 낮아지므로 슬라이딩 윈도우 방식을 사용한다.

       
    => 모든 TCP 패킷 마다 상대편에 ACK를 보낼 것을 요구할 경우 성능이 저하
	   되기 때문에  마지막으로 성공적으로 받은 데이터에 대해서만 ACK를 보내도록
	   한다.  
	   ACK4는 데이터1, 데이터2, 데이터3을 잘 받았다는 의미 



    [ Nagle 알고리즘 ]

     1) 보낼 데이터가 MSS(maximum segment size)로 정의된 크기만큼 싸이면, 상대편에
	     무조건 보낸다.
		 이 경우, 슬라이딩 윈도우 방식으로 계속 데이터를 보낼 수 있다.

     2) 보낼 데이터가 작을 경우 이전에 보낸 데이터에 대한 ACK가 오기를 기다린다.
	    ACK가 도달하면 보낼 데이터가 MSS보다 작더라도 상대편에 보낸다.

        이 경우, TCP 패킷 하나에 대해 ACK를 기다리고, ACK를 받으면 다시 TCP 패킷
		하나를 보내는 동작이 반복된다.

      => 데이터가 충분히 크면 곧바로 보내고, 그렇지 않으면 데이터가 쌓일 때까지 
	     대기한다. 단, 데이터가 충분히 쌓이지 않았더라도 이전에 보낸 데이터를 
		 상대편이 받았다면 다음 데이터를 보낸다.


	장점) 작은 패킷이 불필요하게 많이 생성되는 것을 미연에 방지함으로써 네트워크 
	      트래픽을 감소시킴
    단점) 데이터가 충분히 쌓일 때까지 또는 ACK가 도달할 때까지 대기하는 시간 때문에
	      애플리케이션의 반응시간이 길어질 가능성이 있다.

    TCP_NODELAY 옵션은 Nagle 알고리즘의 장점을 포기하는 대신 애플리케이션의 반응 속도
	를 높이고자 할때사용되는 최후의 방법이다.

    BOOL optval = TRUE;		//Nagle 알고리즘 중지
	if( setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, 
					(char*)&optval, sizeof(optval)) == SOCKET_ERROR)
	{
		err_quit("setsockopt()");
	}



[ 요 약 ]
1. 소켓 옵션을 설정하려면 setsockopt() 함수, 현재 설정된 소켓 옵션값을 얻으려면
   getsockopt() 함수를 호출한다.

2. SO_BROADCAST 옵션을 설정하면 해당 소켓을 이용하여 브로드 캐스트 데이터를 보낼
   수 있다.

3. SO_KEEPALIVE 옵션을 설정해두면, TCP 프로토콜 수준에서 연결 여부를 확인하기 위해
   상대 TCP 에 주기적으로 TCP 패킷을 보낸다. 
   이때 상대 TCP가 정해진 시간 이내에 응답을 하지 않거나 RST(reset) 패킷으로 
   응답을 할 경우 자동 소켓을 닫는다.

4. SO_LINGER 옵션을 이용하면 closesocket() 함수의 이중적인 기능을 세부적으로 
   제어할 수 있다.

5. SO_SNDBUF와  SO_RCVBUF 옵션으로 소켓의 송신 수신 버퍼의 크기를 변경할 수 있다.


6. SO_SNDTIMEO 과 SO_RCVTIMEO 옵션으로 타임아웃을 설정해두면 데이터 전송 함수 
   가 작업 완료와 상관없이 일정시간 후 리턴하도록 할 수 있다.

7. SO_REUSEADDR 옵션을 설정하면 이미 사용 중인 IP PORT를 재사용할 수 있다.

8. IP_MULTICAST_TTL 옵션을 이용하여 TTL값을 변경하면, 라우터 경계를 넘어 특정 범위
   까지 멀티캐스트 패킷을 보낼 수 있다.

9. IP_ADD_MEMBERSHIP 옵션은 멀티캐스트 그룹에 가입할 때, IP_DROP_MEMBERSHIP 옵션은
   멀티캐스트 그룹에서 탈퇴할 때 사용한다.

10. TCP_NODELAY 옵션은 Nagle 알고리즘 작동을 중지하는 역할을 한다.



[ 확  인 ]

1. FileSender / FileReceiver 예제에 SO_SNDBUF, SO_RCVBUF 옵션을 적용하여 소켓 송수신
   버퍼를 변화시키면서 데이터 전송 속도를 확인

2. 멀티캐스팅을 이용한 다자 간 채팅 프로그램을 작성

   

*/