//pack.h

#define RECV_PORT	9000

typedef struct tagPacket
{
	int flag;
	char s_name[20];
	char s_time[20];		// 00:00:00 
	char s_message[50];
}PACKET;