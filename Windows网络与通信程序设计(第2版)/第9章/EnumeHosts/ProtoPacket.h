////////////////////////////////////////////////
// ProtoPacket.h文件

#ifndef __PROTOPACKET_H__
#define __PROTOPACKET_H__

#include "../common/protoinfo.h"

#define ARPFRAME_SIZE 100
class CArpPacket
{
public:
	CArpPacket(CAdapter *pAdapter);
	~CArpPacket();
	// 发送ARP封包
	BOOL SendPacket(u_char *pdEtherAddr, u_char *psEtherAddr, 
			int nOpcode, u_char *pdMac, DWORD dIPAddr, u_char *psMac, DWORD sIPAddr);
	// 等待ARP响应
	PARPHeader WaitReply(DWORD dwMillionSec = 1000*2);

protected:
	CAdapter *m_pAdapter;
	u_char m_ucFrame[ARPFRAME_SIZE];
	OVERLAPPED m_olRead;
	OVERLAPPED m_olWrite;
};

#endif // __PROTOPACKET_H__