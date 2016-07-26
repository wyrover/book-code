///////////////////////////////////////////////////////////
// filter.h文件

#ifndef __FILTER__H
#define __FILTER__H



#define MAX_PACKET_HEADER_LEN    100

/*

Mac 14 ip 20 tcp 20       = 54

*/

//////////////////////////////////////////////////////////
// 结构

// 状态信息

typedef struct _ADAPT_FILTER_RSVD ADAPT_FILTER_RSVD, *PADAPT_FILTER_RSVD;

typedef struct _PassthruFilterList  PassthruFilterList, *PPassthruFilterList;

void ClearFilterList(PADAPT_FILTER_RSVD pFilterContext);
VOID FltOnDeinitAdapter(PADAPT pAdapt);

VOID FltOnInitAdapter(PADAPT pAdapt);

BOOLEAN FltFilterSendPacket(PADAPT pAdapt,PNDIS_PACKET pSendPacket,BOOLEAN bDispatchLevel);

void FltReadPacketData(PNDIS_PACKET pPacket, 
						 PUCHAR lpBufferIn, ULONG nNumberToRead, PUINT lpNumberOfRead);

BOOLEAN FltCheckFilterRules(PPassthruFilterList pFilterList, PUCHAR pPacketData, ULONG nDataLen, BOOLEAN bIncludeETHdr);

BOOLEAN FltFilterReceive(
   IN PADAPT         pAdapt,
   IN NDIS_HANDLE    MacReceiveContext,
   IN PVOID          HeaderBuffer,
   IN UINT           HeaderBufferSize,
   IN PVOID          LookAheadBuffer,
   IN UINT           LookAheadBufferSize,
   IN UINT           PacketSize
   );

BOOLEAN FltFilterReceivePacket(PADAPT pAdapt,PNDIS_PACKET   pReceivedPacket);

BOOLEAN FltFilterSendPacket(PADAPT pAdapt,PNDIS_PACKET pSendPacket,BOOLEAN bDispatchLevel);

















#endif	// __FILTER__H