#include <stdio.h>
#include <string.h>

typedef struct _tagMyChar{
	int		iDataSize;
	char	*pData;
}MyChar;

int main()
{
	MyChar	myChar;
	char	*ptr = new char[5];
	strcpy(ptr, "test");

	myChar.iDataSize	= strlen(ptr);
	myChar.pData		= ptr;

	printf("%d %s\n", myChar.iDataSize, myChar.pData);
	return 1;
}

// 소켓을 이용한 데이터 전송시 주의할 점
/*

1. 보내는 쪽
	ret = send(socket, &myChar, sizeof(myChar), 0);
2. 받는 쪽 
	ret	= recv(socket, &myChar, sizeof(myChar), 0);

	1) myChar.pData : 포인터 변수임.. 따라서 pData가 전송하는 값은 현재 
					  메모리에 있는 스트링의 주소가 된다. 
					  따라서 이 주소를 클라이언트에서 사용해 봤자 전혀 
					  사용불가능...
					  도리어 잘못된 메모리 주소를 읽어서 문제 발생...

	2) 전송데이터 크기 : myChar 구조체의 크기 즉 int 형 4byte char 포인터 
	                     형 4byte 총 8byte 전송
						그런데 만약 8byte이상의 데이터를 넣는다면???

3. 따라서 올바른 전송 방법은..
	1) 포인터를 쓰지 않는다. --> 배열 사용

	typedef struct _tagMyChar{
		int		iDataSize;
		char	pData[28];
	}MyChar;
	

    2) 데이터를 나누어서 보낸다. 
	   포멧을 정해서 항상 앞 4byte는 데이터 크기를 나타낸다고 하고
	   다음과 같은 코드 작성 
	- 보내는 쪽 :
		myChar.iDataSize = strlen(myChar.pData);
		ret = send(socket, &myChar.iDataSize, sizeof(int),0);
		ret = send(socket, myChar.pData, myChar.iDataSize, 0);

   - 받는 쪽 :
		ret = recv(socket, &myChar.iDataSize, sizeof(int),0);
		myChar.pData = new char[myChar.iDataSize+1];
		ret	= recv(socket, myChar.pData, myChar.iDataSize, 0);
		myChar.pData[ret] = 0;

  */


/*
TCP 소켓의 특성

  1. 데이터의 순서를 보장..
  2. 몇번에 걸쳐서 받게 되는지는 보장을 안함..
     데이터 전송시 총 2300byte 전송 : 1000/500/800
	 데이터 수신시 총 2300byte 수신 : 1100/300/800/100
*/