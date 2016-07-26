//////////////////////////////////////////////////////////
// RawEthernet.cpp文件


#include <windows.h>
#include <winioctl.h>
#include <ntddndis.h>
#include <stdio.h>
#include "protoutils.h"

int main()
{
	// 启动服务
	if(!ProtoStartService())
	{
		printf(" ProtoStartService() failed %d \n", ::GetLastError());
		return -1;
	}
	// 打开控制设备对象
	HANDLE hControlDevice = ProtoOpenControlDevice();
	if(hControlDevice == INVALID_HANDLE_VALUE)
	{
		printf(" ProtoOpenControlDevice() failed() %d \n", ::GetLastError());
		ProtoStopService();
		return -1;
	}
	// 枚举绑定的下层适配器
	CPROTOAdapters adapters;
	if(!adapters.EnumAdapters(hControlDevice))
	{
		printf(" Enume adapter failed \n"); 
		ProtoStopService();
		return -1;
	}
	
	// 创建一个原始封包（至少应为16个字节长）
	BYTE bytes[] =  {0xff,0xff,0xff,0xff,0xff,0xff,  // 目的MAC地址
					 0x00,0x02,0x3e,0x4c,0x49,0xaa,  // 源MAC地址
					 0x08,0x00,					     // 协议
					 0x01,0x02,0x03,0x04,0x05,0x06}; // 通常数据
	// 打印出每个下层适配器的信息，发送数据
	for(int i=0; i<adapters.m_nAdapters; i++)
	{
		char sz[256];
		wsprintf(sz, "\n\n Adapter:	%ws \n Symbolic Link: %ws \n\n ", 
								adapters.m_pwszAdapterName[i], adapters.m_pwszSymbolicLink[i]);
		printf(sz);

		CAdapter adapter;
		adapter.OpenAdapter(adapters.m_pwszSymbolicLink[i]);
		// 在此适配器上发送原始数据
		int nSend = adapter.SendData(bytes, sizeof(bytes));
		if(nSend > 0)
			printf(" Packet sent: %d bytes \n", nSend);
		else
			printf(" Packet sent failed \n");
	}

	::CloseHandle(hControlDevice);
	ProtoStopService();
	return 0;
}



