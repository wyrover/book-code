#include "precomp.h"
#include "ntstrsafe.h"

typedef struct in_addr {
	union {
		struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { USHORT s_w1,s_w2; } S_un_w;
		ULONG S_addr;
	} S_un;
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;

typedef struct IP_HEADER
{
	unsigned char  VIHL;          // Version and IHL
	unsigned char  TOS;           // Type Of Service
	short          TotLen;        // Total Length
	short          ID;            // Identification
	short          FlagOff;       // Flags and Fragment Offset
	unsigned char  TTL;           // Time To Live
	unsigned char  Protocol;      // Protocol
	unsigned short Checksum;      // Checksum
	struct in_addr        iaSrc;  // Internet Address - Source
	struct in_addr        iaDst;  // Internet Address - Destination
}IP_HEADER, *PIP_HEADER;

#define IP_OFFSET                               0x0E

//IP Protocol Types
#define PROT_ICMP                               0x01 
#define PROT_TCP                                0x06 
#define PROT_UDP                                0x11 


// 输入参数：
//	Packet： 被分析的NDIS包描述符
//	bRecOrSend: 如果是接收包，为TRUE;如果为发送包，为FALSE。
// 返回值：
//	理想的情况下，调用者通过返回值以决定如何处理NDIS包：续传、失败、转发
FILTER_STATUS AnalysisPacket(PNDIS_PACKET Packet,  BOOLEAN bRecOrSend)
{
	FILTER_STATUS status = STATUS_PASS; // 默认全部通过
	PNDIS_BUFFER NdisBuffer ;
	UINT TotalPacketLength = 0;
	UINT copysize = 0;
	UINT DataOffset = 0 ;
	UINT PhysicalBufferCount;
	UINT BufferCount   ;
	PUCHAR pPacketContent = NULL;
	char* tcsPrintBuf = NULL;
	PUCHAR tembuffer = NULL ; 
	UINT j;

	__try{

		status = NdisAllocateMemoryWithTag( &pPacketContent, 2048, TAG); 
		if( status != NDIS_STATUS_SUCCESS ){
			status = NDIS_STATUS_FAILURE ;
			__leave;
		}

		NdisZeroMemory( pPacketContent, 2048 ) ;

		// 找到第一个Ndis_Buffer。然后通过通过NdisGetNextBuffer来获得后续的NDIS_BUFFER。
		// 如果只是找第一个节点，更快且方便的方法是调用NdisGetFirstBufferFromPacket。
		NdisQueryPacket(Packet,  // NDIS_PACKET        
			&PhysicalBufferCount,// 内存中的物理块数
			&BufferCount,		 // 多少个NDIS_BUFFER包
			&NdisBuffer,         // 将返回第一个包
			&TotalPacketLength	 // 总共的包数据长度
			);

		while(TRUE){

			// 取得Ndis_Buffer中存储缓冲区的虚拟地址。
			// 这个函数的另一个版本是NdisQueryBuffer。
			// 后者在系统资源低或者甚至耗尽的时候，会产生Bug Check，导致蓝屏。
			NdisQueryBufferSafe(NdisBuffer,
				&tembuffer,// 缓冲区地址
				&copysize, // 缓冲区大小
				NormalPagePriority
				);

			// 如果tembuffer为NULL，说明当前系统资源匮乏。
			if(tembuffer != NULL){
				NdisMoveMemory( pPacketContent + DataOffset , tembuffer, copysize) ;			
				DataOffset += copysize;
			}

			// 获得下一个NDIS_BUFFER。
			// 如果得到的是一个NULL指针，说明已经到了链式缓冲区的末尾，我们的循环应该结束了。
			NdisGetNextBuffer(NdisBuffer , &NdisBuffer ) ;

			if( NdisBuffer == NULL )
				break ;
		}

		// 取得数据包内容后，下面将对其内容进行过滤。
		// 我们在这个函数中的实现，仅仅简单地打印一些可读的Log信息。
		if(pPacketContent[12] == 8 &&  pPacketContent[13] == 0 )  //is ip packet
		{	
			PIP_HEADER pIPHeader = (PIP_HEADER)(pPacketContent + IP_OFFSET);
			switch(pIPHeader->Protocol)
			{
			case PROT_ICMP:
				if(bRecOrSend)
					DbgPrint("Receive ICMP packet");
				else
					DbgPrint("Send ICMP packet");

				//
				// 取得ICMP头，做出你的过滤判断。
				// 

				break;
			case PROT_UDP:
				if(bRecOrSend)
					DbgPrint("Receive UDP packet");
				else
					DbgPrint("Send UDP packet");

				//
				// 取得UDP头，做出你的过滤判断。
				//

				break;
			case PROT_TCP:
				if(bRecOrSend)
					DbgPrint("Receive TCP packet");
				else
					DbgPrint("Send TCP packet");

				//
				// 取得TCP头，做出你的过滤判断。
				//

				break;
			}
		}else if(pPacketContent[12] == 8 &&  pPacketContent[13] == 6 ){
			if(bRecOrSend)
				DbgPrint("Receive ARP packet");
			else
				DbgPrint("Send ARP packet");
		}else{
			if(bRecOrSend)
				DbgPrint("Receive unknown packet");
			else
				DbgPrint("Send unknown packet");
		}

		// 简单打印出包数据内容
		status = NdisAllocateMemoryWithTag( &tcsPrintBuf, 2048*3, TAG);  //分配内存块
		if( status != NDIS_STATUS_SUCCESS ){
			status = NDIS_STATUS_FAILURE ;
			__leave;
		}
		for(j=0;j<=DataOffset;j++)
			RtlStringCbPrintfA(tcsPrintBuf+j*3, 2048*3-j*3, "%2x ",pPacketContent[j]);

		DbgPrint(tcsPrintBuf);

	}__finally{
		if(pPacketContent)NdisFreeMemory(pPacketContent, 0, 0);
		if(tcsPrintBuf)NdisFreeMemory(tcsPrintBuf, 0, 0);
	}

	return STATUS_PASS;
}