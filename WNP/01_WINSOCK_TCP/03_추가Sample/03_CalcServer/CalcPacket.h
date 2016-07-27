// CalcPacket.h

#pragma pack ( 1 )

enum { PLUS = '+', MINUS = '-', MULTIPLY = '*', DIVIDE = '/' };

typedef struct _PACKET
{
	int cmd;
	int first;
	int second;
} CALCPACKET;

