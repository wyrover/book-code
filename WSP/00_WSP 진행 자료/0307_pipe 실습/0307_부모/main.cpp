//main.cpp
#include "std.h"

void pipe_recv(char* buf, int length)
{
	printf("[%dbyte]%s\n", length, buf );
}

void main()
{
	pipe_Create(pipe_recv, 4096);

	//6) 전송 테스트 
	//--------------------------------------------
	char buf[4096];
	while ( 1 )
	{
		gets( buf );		
		pipe_Write(buf, strlen(buf)+1);
	}
}