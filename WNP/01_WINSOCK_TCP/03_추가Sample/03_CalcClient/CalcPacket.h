// CalcPacket.h

#pragma pack ( 1 )

enum { PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/' };

typedef struct _PACKET
{
	int cmd;		// 0 1 2 3 
	int first;
	int second;
} CALCPACKET;

